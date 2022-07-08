/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 23:10:52 by yakhoudr          #+#    #+#             */
/*   Updated: 2022/07/02 23:10:52 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

t_task	Tq[256];
int task_count = 0;
pthread_mutex_t	q_mutex;
pthread_cond_t wait_cond;
int pixels[HEIGHT][WIDTH];
int finished = 0;

void	draw_mandelbrot(t_all *d_all,int sx, int sy, int zone_x, int zone_y, int max_iteration)
{
	int	x;
	int	y;
	__m256d  _zr, _zi, _cr, _ci, _a, _b, _zr2, _zi2, _two, _four, _mask1;
	__m256i _n, _iterations, _mask2, _c, _one;
	_two = _mm256_set1_pd(2.0);
	_four = _mm256_set1_pd(4.0);
	_iterations = _mm256_set1_epi64x(max_iteration);
	_one = _mm256_set1_epi64x(1);
	for (y = sy; y < zone_y; y += 1)
	{
		_ci = _mm256_set1_pd(map(y, 0, HEIGHT, d_all->draw.ly, d_all->draw.ry));
		for (x = sx; x < zone_x; x += 4)
		{
			double ex1, ex2, ex3, ex4;
			ex1 = map(x, 0, WIDTH, d_all->draw.lx, d_all->draw.rx);
			ex2 = map(x + 1, 0, WIDTH, d_all->draw.lx, d_all->draw.rx);
			ex3 = map(x + 2, 0, WIDTH, d_all->draw.lx, d_all->draw.rx);
			ex4 = map(x + 3, 0, WIDTH, d_all->draw.lx, d_all->draw.rx);
			_cr = _mm256_set_pd(ex1, ex2, ex3, ex4);
			_zr = _mm256_setzero_pd();
			_zi = _mm256_setzero_pd();
			_n = _mm256_setzero_si256();
repeat : 
			_zr2 = _mm256_mul_pd(_zr, _zr);
			_zi2 = _mm256_mul_pd(_zi, _zi);
			_a = _mm256_sub_pd(_zr2, _zi2);
			_a = _mm256_add_pd(_a, _cr);
			_b = _mm256_mul_pd(_zr, _zi);
			_b = _mm256_fmadd_pd(_b, _two, _ci);
			_zr = _a;
			_zi = _b;
			_a = _mm256_add_pd(_zr2, _zi2);
			_mask1 = _mm256_cmp_pd(_a, _four, _CMP_LT_OQ);
			_mask2 = _mm256_cmpgt_epi64(_iterations, _n);
			_mask2 = _mm256_and_si256(_mask2, _mm256_castpd_si256(_mask1));
			_c = _mm256_and_si256(_one, _mask2);
			_n = _mm256_add_epi64(_n, _c);
			if (_mm256_movemask_pd(_mm256_castsi256_pd(_mask2)) > 0)
				goto repeat;
			pixels[y][x] = (int) _n[3];
			pixels[y][x + 1] = (int) _n[2];
			pixels[y][x + 2] = (int) _n[1];
			pixels[y][x + 3] = (int) _n[0];
		}
	}
}

void draw(t_all *data)
{
	for (int i = 0;i < HEIGHT;i += 1)
	{
		for (int j = 0; j < WIDTH;j += 1)
		{
			my_mlx_pixel_put(&data->data,i, j, pixels[i][j]);
		}
	}
	mlx_put_image_to_window(data->mlx.mlx_ptr, data->mlx.window_ptr, data->data.img, 0, 0);
}

int	main(int ac, char **av)
{
	(void) av;
	if (ac == 2)
	{
		t_all	d_all;
		d_all.draw.lx = -2.00;
		d_all.draw.rx = 0.47;
		d_all.draw.ry = -1.12;
		d_all.draw.ly = 1.12;
		d_all.mlx.mlx_ptr = mlx_init();
		for (int i = 0;i < HEIGHT;i += 1)
		{
			for (int j = 0;j < WIDTH; j += 1)
			{
				pixels[i][j] = 0;
			}
		}
		if (!d_all.mlx.mlx_ptr)
			exit(1);
		d_all.mlx.window_ptr = mlx_new_window(d_all.mlx.mlx_ptr, WIDTH, HEIGHT, "fe");
		if (!d_all.mlx.window_ptr)
			exit(1);
		d_all.data.img = mlx_new_image(d_all.mlx.mlx_ptr, WIDTH, HEIGHT);
		if (!d_all.data.img)
			exit(1);
		d_all.data.addr = mlx_get_data_addr(d_all.data.img, &d_all.data.bits_per_pixel,\
		&d_all.data.line_length, &d_all.data.endian);
		if (!d_all.data.addr)
			exit(1);
		int	i;
		pthread_t	th[TH_N];
		pthread_mutex_init(&q_mutex, 0x0);
		pthread_cond_init(&wait_cond, 0x0);
		for (i = 0;i < TH_N;i += 1)
		{
			if (pthread_create(&th[i], 0x0, &start_thread, 0x0) != 0)
				perror("Creation of thread failed!");

		}
		int incr = WIDTH / (TH_N / 2);
		int sx = 0;
		int sy = 0;
		int fx = incr;
		int fy = HEIGHT;
		for (i = 0;i < TH_N / 2;i += 1)
		{
			t_task t;
			t.max_iteration = 255;
			t.sx = sx;
			t.sy = sy;
			t.fx = fx;
			t.fy = fy;
			t.info = &d_all;
			t.draw = &draw_mandelbrot;
			submit_task(t);
			sx += incr;
			fx += incr;
		}
		while (finished < TH_N / 2)
		{
			printf("finished : %d\n", finished);
			usleep(25);
		}
		draw(&d_all);
		mlx_hook(d_all.mlx.window_ptr, 17, 0x0, ft_close, &d_all);
		mlx_key_hook(d_all.mlx.window_ptr, handle_key, &d_all);
		mlx_mouse_hook(d_all.mlx.window_ptr, handle_mouse, &d_all);
		mlx_loop(d_all.mlx.mlx_ptr);
	}
	return (0);
}


