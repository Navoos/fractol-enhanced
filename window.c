/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 00:12:40 by yakhoudr          #+#    #+#             */
/*   Updated: 2022/07/09 00:19:19 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int ft_close(int key, t_all *arg)
{
	(void) key;
	(void) arg;
	exit(0);
	return (0);
}

int	handle_key(int code, t_all *data)
{
	double x = 0.05 * (fabs(data->draw.lx - data->draw.rx));
	double y = 0.05 * (fabs(data->draw.ly - data->draw.ry));
	if (code == 97)
	{
		data->draw.lx -= x;
		data->draw.rx -= x;
		draw_mandelbrot(data, 0,0, WIDTH, HEIGHT, MAX_IT);
	}
	if (code == 100)
	{
		data->draw.lx += x;
		data->draw.rx += x;
		draw_mandelbrot(data, 0,0,WIDTH, HEIGHT, MAX_IT);
	}
	if (code == 115)
	{
		data->draw.ly -= y;
		data->draw.ry -= y;
		draw_mandelbrot(data, 0,0,WIDTH, HEIGHT, MAX_IT);
	}
	if (code == 119)
	{
		data->draw.ly += y;
		data->draw.ry += y;
		draw_mandelbrot(data, 0,0,WIDTH, HEIGHT, MAX_IT);
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
		draw_mandelbrot(data, 0,0,WIDTH, HEIGHT,  MAX_IT);
	}
	else if (code == 4)
	{
		double out = 1.1;
		data->draw.lx *= out; data->draw.rx *= out; data->draw.ly *= out; data->draw.ry *= out;
		ax = map(x, 0, WIDTH, data->draw.lx, data->draw.rx);
		ay = map(y, 0, HEIGHT, data->draw.ly, data->draw.ry);
		data->draw.lx += (bx-ax); data->draw.rx += (bx-ax); data->draw.ly += (by-ay); data->draw.ry += (by-ay);
		draw_mandelbrot(data, 0,0,WIDTH, HEIGHT,  MAX_IT);
	}
	return (0);
}
