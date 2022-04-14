#include <mlx.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <immintrin.h>
#define WIDTH 918
#define HEIGHT 720
#define MAX_IT 255

typedef struct s_mlx_var
{
	void	*mlx_ptr;
	void	*window_ptr;
}	t_mlx_var;



typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;


typedef struct s_draw
{
	double	lx;
	double	rx;
	double	ly;
	double	ry;
}	t_draw;


typedef struct s_all
{
	t_data		data;
	t_mlx_var	mlx;
	t_draw		draw;

}	t_all;
void	draw_mandelbrot(t_all *d_all, int zone_x, int zone_y, int max_iteration);
int close(int key, t_all *arg)
{
	mlx_destroy_window(arg->mlx.mlx_ptr, arg->mlx.window_ptr);
	return (0);
}


double map(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int	create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

int	handle_key(int code, t_all *data)
{
	double x = 0.05 * (fabs(data->draw.lx - data->draw.rx));
	double y = 0.05 * (fabs(data->draw.ly - data->draw.ry));
	if (code == 97)
	{
		data->draw.lx -= x;
		data->draw.rx -= x;
		draw_mandelbrot(data, WIDTH, HEIGHT, MAX_IT);
	}
	if (code == 100)
	{
		data->draw.lx += x;
		data->draw.rx += x;
		draw_mandelbrot(data, WIDTH, HEIGHT, MAX_IT);
	}
	if (code == 119)
	{
		data->draw.ly -= y;
		data->draw.ry -= y;
		draw_mandelbrot(data, WIDTH, HEIGHT, MAX_IT);
	}
	if (code == 115)
	{
		data->draw.ly += y;
		data->draw.ry += y;
		draw_mandelbrot(data, WIDTH, HEIGHT, MAX_IT);
	}
	return (0);
}


int handle_mouse(int code, int x, int y, t_all *data)
{
	double bx = map(x, 0, WIDTH, data->draw.lx, data->draw.rx);
	double by = map(y, 0, HEIGHT, data->draw.ly, data->draw.ry);
	double ax;
	double ay;
	if (code == 5)
	{
		double in = 0.98;
		data->draw.lx *= in; data->draw.rx *= in; data->draw.ly *= in; data->draw.ry *= in;
		ax = map(x, 0, WIDTH, data->draw.lx, data->draw.rx);
		ay = map(y, 0, HEIGHT, data->draw.ly, data->draw.ry);
		data->draw.lx += (bx-ax); data->draw.rx += (bx-ax); data->draw.ly += (by-ay); data->draw.ry += (by-ay);
		draw_mandelbrot(data, WIDTH, HEIGHT,  MAX_IT);
	}
	else if (code == 4)
	{
		double out = 1.1;
		data->draw.lx *= out; data->draw.rx *= out; data->draw.ly *= out; data->draw.ry *= out;
		ax = map(x, 0, WIDTH, data->draw.lx, data->draw.rx);
		ay = map(y, 0, HEIGHT, data->draw.ly, data->draw.ry);
		data->draw.lx += (ax-bx); data->draw.rx += (ax-bx); data->draw.ly += (ay-by); data->draw.ry += (ay-by);
		draw_mandelbrot(data, WIDTH, HEIGHT,  MAX_IT);
	}
}
void	draw_mandelbrot(t_all *d_all, int zone_x, int zone_y, int max_iteration)
{
	int	x;
	int	y;
	int	it;
	for (y = 0; y < zone_y; y += 1)
	{
		for (x = 0; x < zone_x; x += 1)
		{
			double x0 = map(x, 0, zone_x, d_all->draw.lx, d_all->draw.rx);
			double y0 = map(y, 0, zone_y, d_all->draw.ry, d_all->draw.ly);
			double a = 0.0;
			double b = 0.0;
			it = 0;
			while (a * a + b * b <= 4 && it < max_iteration)
			{
				double x_temp = a * a - b * b + x0;
				b = 2 * a * b + y0; 
				a = x_temp;
				it += 1;
			}
			my_mlx_pixel_put(&d_all->data, x, y, create_trgb(0, it, 0, 0));
		}
	}
	mlx_put_image_to_window(d_all->mlx.mlx_ptr, d_all->mlx.window_ptr, d_all->data.img, 0, 0);
}

int	main(int ac, char **av)
{
	if (ac == 2)
	{
		t_all	d_all;
		d_all.draw.lx = -2.00;
		d_all.draw.rx = 0.47;
		d_all.draw.ry = -1.12;
		d_all.draw.ly = 1.12;
		d_all.mlx.mlx_ptr = mlx_init();
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
		draw_mandelbrot(&d_all, WIDTH, HEIGHT, MAX_IT);
		mlx_hook(d_all.mlx.window_ptr, 17, 0x0, close, &d_all);
		mlx_key_hook(d_all.mlx.window_ptr, handle_key, &d_all);
		mlx_mouse_hook(d_all.mlx.window_ptr, handle_mouse, &d_all);
		mlx_loop(d_all.mlx.mlx_ptr);
	}
	return (0);
}

