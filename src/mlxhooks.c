/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlxhooks.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgrossma <pgrossma@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 18:53:50 by pgrossma          #+#    #+#             */
/*   Updated: 2023/11/13 17:35:35 by pgrossma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fract-ol.h"

void	ft_resize_hook(int width, int height, void *param)
{
	t_window	*window;

	window = (t_window *) param;
	window->width = width;
	window->height = height;
	ft_rebuild_fractal(window);
}

void	ft_scroll_hook(double xdelta, double ydelta, void *param)
{
	t_window	*window;
	int			mouse_x;
	int			mouse_y;
	double		factor;

	xdelta = 0;
	window = (t_window *) param;
	if (ydelta > 0)
		factor = 1.1;
	else
		factor = 0.9;
	double zoom_factor = 1 / factor;
	mlx_get_mouse_pos(window->mlx, &mouse_x, &mouse_y);
	double mouse_relativ_x = (double)mouse_x - ((double)window->width / 2);
	double mouse_relativ_y = (double)mouse_y - ((double)window->height / 2);
	mouse_relativ_x *= 1.35;
	mouse_relativ_y *= 1.35;
	window->offset_x = zoom_factor * (window->offset_x - mouse_relativ_x) + mouse_relativ_x;
	window->offset_y = zoom_factor * (window->offset_y - mouse_relativ_y) + mouse_relativ_y;
	window->scale *= factor;
	ft_rebuild_fractal(window);
}

void	ft_key_hook(mlx_key_data_t keydata, void *param)
{
	t_window	*window;

	window = (t_window *) param;
	if (keydata.key == MLX_KEY_ESCAPE)
	{
		ft_exit_loop(NULL, window);
		return ;
	}
	else if (keydata.key == MLX_KEY_W || keydata.key == MLX_KEY_UP)
		window->offset_y += 15;
	else if (keydata.key == MLX_KEY_S || keydata.key == MLX_KEY_DOWN)
		window->offset_y -= 15;
	else if (keydata.key == MLX_KEY_A || keydata.key == MLX_KEY_LEFT)
		window->offset_x += 15;
	else if (keydata.key == MLX_KEY_D || keydata.key == MLX_KEY_RIGHT)
		window->offset_x -= 15;
	ft_rebuild_fractal(window);
}

void	ft_loop_hook(void *param)
{
	t_window	*window;
	int			mouse_x;
	int			mouse_y;

	window = (t_window *) param;
	if (mlx_is_mouse_down(window->mlx, MLX_MOUSE_BUTTON_LEFT))
	{
		mlx_get_mouse_pos(window->mlx, &mouse_x, &mouse_y);
		if (mlx_is_key_down(window->mlx, MLX_KEY_LEFT_SHIFT) || mlx_is_key_down(window->mlx, MLX_KEY_RIGHT_SHIFT))
		{
			window->fractal->ca = ft_map(mouse_x, 0, window->width, -1, 1);
			window->fractal->cb = ft_map(mouse_y, 0, window->height, -1, 1);
			ft_rebuild_fractal(window);
		}
		else
		{
			if (window->mouse.button_pressed)
			{
				window->offset_x += mouse_x - window->mouse.last_x;
				window->offset_y += mouse_y - window->mouse.last_y;
				window->mouse.last_x = mouse_x;
				window->mouse.last_y = mouse_y;
				ft_rebuild_fractal(window);
			}
			else
			{
				window->mouse.button_pressed = true;
				window->mouse.last_x = mouse_x;
				window->mouse.last_y = mouse_y;
			}
		}
	}
	else
		window->mouse.button_pressed = false;
}
