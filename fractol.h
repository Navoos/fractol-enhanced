/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 23:10:43 by yakhoudr          #+#    #+#             */
/*   Updated: 2022/07/02 23:19:02 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <immintrin.h>
#include <pthread.h>
#ifndef TH_N
	#define TH_N 8
#endif
#define WIDTH 920
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

typedef struct s_task
{
	void (*draw)(t_all *,int , int, int , int, int);
	t_all *info;
	int sx;
	int sy;
	int fx;
	int fy;
	int max_iteration;

}	t_task;

extern t_task Tq[256];
extern	int task_count;
extern pthread_mutex_t	q_mutex;
extern pthread_cond_t wait_cond;

void	draw_mandelbrot(t_all *d_all, int sx, int sy, int zone_x, int zone_y, int max_iteration);
double map(double x, double in_min, double in_max, double out_min, double out_max);
int		ft_close(int key, t_all *arg);
int		create_trgb(int t, int r, int g, int b);
void	my_mlx_pixel_put(t_data *data, int x, int y, int color);
int		handle_key(int code, t_all *data);
int 	handle_mouse(int code, int x, int y, t_all *data);
void	exec_task(t_task *task);
void	*start_thread(void *args);
void	submit_task(t_task task);
