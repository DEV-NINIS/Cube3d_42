/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_destroy.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmassin <gmassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 16:30:14 by gmassin           #+#    #+#             */
/*   Updated: 2025/12/12 10:02:26 by gmassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

void	engine_destroy(t_engine *engine)
{
	int	i;

	if (!engine)
		return ;
	i = 0;
	while (i < MAX_TEXTURES)
	{
		if (engine->textures[i].handle)
			mlx_destroy_image(engine->mlx, engine->textures[i].handle);
		engine->textures[i].handle = NULL;
		i++;
	}
	if (engine->frame.handle)
		mlx_destroy_image(engine->mlx, engine->frame.handle);
	engine->frame.handle = NULL;
	if (engine->win)
		mlx_destroy_window(engine->mlx, engine->win);
	engine->win = NULL;
	free_cub(&engine->cub);
	if (engine->mlx)
	{
		mlx_destroy_display(engine->mlx);
		free(engine->mlx);
	}
	engine->mlx = NULL;
}
