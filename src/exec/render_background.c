/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_background.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmassin <gmassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 10:04:24 by gmassin           #+#    #+#             */
/*   Updated: 2025/12/12 10:04:31 by gmassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

void	render_draw_background(t_engine *engine)
{
	int	x;
	int	y;

	y = 0;
	while (y < WIN_H / 2)
	{
		x = 0;
		while (x < WIN_W)
		{
			image_put_pixel(&engine->frame, x, y, engine->cub.ceiling_color);
			x++;
		}
		y++;
	}
	while (y < WIN_H)
	{
		x = 0;
		while (x < WIN_W)
		{
			image_put_pixel(&engine->frame, x, y, engine->cub.floor_color);
			x++;
		}
		y++;
	}
}
