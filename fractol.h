#include <mlx.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <immintrin.h>
#include <pthread.h>
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

void	draw_mandelbrot(t_all *d_all, int sx, int sy, int zone_x, int zone_y, int max_iteration);
double map(double x, double in_min, double in_max, double out_min, double out_max);
int ft_close(int key, t_all *arg);
int	create_trgb(int t, int r, int g, int b);
void	my_mlx_pixel_put(t_data *data, int x, int y, int color);
int	handle_key(int code, t_all *data);
int handle_mouse(int code, int x, int y, t_all *data);
