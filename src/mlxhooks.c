/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlxhooks.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgrossma <pgrossma@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 18:53:50 by pgrossma          #+#    #+#             */
/*   Updated: 2023/11/25 12:21:30 by pgrossma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	ft_resize_hook(int width, int height, void *param)
{
	t_win	*win;

	win = (t_win *) param;
	win->width = width;
	win->height = height;
	ft_rebuild_fract(win);
}

double	ft_get_factor(double ydelta, t_win *win)
{
	double	factor;

	if (win->fract->type == FERN)
	{
		if (ydelta > 0)
			factor = 1.1;
		else
			factor = 0.9;
	}
	else
	{
		if (ydelta > 0)
			factor = 0.9;
		else
			factor = 1.1;
	}
	return (factor);
}

void	ft_scroll_hook(double xdelta, double ydelta, void *param)
{
	t_win	*win;
	double	mouse_x;
	double	mouse_y;
	double	factor;
	t_mouse	mouse;

	xdelta = 0;
	win = (t_win *) param;
	factor = ft_get_factor(ydelta, win);
	mlx_get_mouse_pos(win->mlx, &mouse.last_x, &mouse.last_y);
	mouse_x = mouse.last_x;
	mouse_y = mouse.last_y;
	win->scale *= factor;
	if (win->fract->type != FERN)
		ft_scroll_not_fern(&mouse_x, &mouse_y, &factor, win);
	win->off_x = factor * (win->off_x + mouse_x) - mouse_x;
	win->off_y = factor * (win->off_y + mouse_y) - mouse_y;
	ft_rebuild_fract(win);
}

void	ft_key_hook(mlx_key_data_t keydata, void *param)
{
	t_win	*win;

	win = (t_win *) param;
	if (keydata.key == MLX_KEY_ESCAPE)
	{
		ft_exit_loop(NULL, win);
		return ;
	}
	else if (keydata.key == MLX_KEY_W || keydata.key == MLX_KEY_UP)
		win->off_y -= 15;
	else if (keydata.key == MLX_KEY_S || keydata.key == MLX_KEY_DOWN)
		win->off_y += 15;
	else if (keydata.key == MLX_KEY_A || keydata.key == MLX_KEY_LEFT)
		win->off_x -= 15;
	else if (keydata.key == MLX_KEY_D || keydata.key == MLX_KEY_RIGHT)
		win->off_x += 15;
	else if (!ft_key_change_values(keydata, win))
		return ;
	ft_rebuild_fract(win);
}

void	ft_loop_hook(void *param)
{
	t_win	*win;
	int		mouse_x;
	int		mouse_y;

	win = (t_win *) param;
	if (mlx_is_mouse_down(win->mlx, MLX_MOUSE_BUTTON_LEFT))
	{
		mlx_get_mouse_pos(win->mlx, &mouse_x, &mouse_y);
		if (mlx_is_key_down(win->mlx, MLX_KEY_LEFT_SHIFT)
			|| mlx_is_key_down(win->mlx, MLX_KEY_RIGHT_SHIFT))
			ft_drag_mouse_shift(mouse_x, mouse_y, win);
		else
			ft_drag_mouse(mouse_x, mouse_y, win);
	}
	else
		win->mouse.button_pressed = false;
	if (win->fract->type == FERN)
		ft_fill_fern(win);
}
