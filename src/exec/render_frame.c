/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_frame.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmassin <gmassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 10:05:30 by gmassin           #+#    #+#             */
/*   Updated: 2025/12/12 10:05:36 by gmassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

int	render_frame(t_engine *engine)
{
	int	x;

	player_update(engine);
	render_draw_background(engine);
	x = 0;
	while (x < WIN_W)
	{
		raycast_draw_column(engine, x);
		x++;
	}
	mlx_put_image_to_window(engine->mlx, engine->win,
		engine->frame.handle, 0, 0);
	return (0);
}
