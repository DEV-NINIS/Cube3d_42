/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmassin <gmassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:50:04 by gmassin           #+#    #+#             */
/*   Updated: 2025/12/12 09:59:58 by gmassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

static int	create_window(t_engine *engine)
{
	engine->win = mlx_new_window(engine->mlx, WIN_W, WIN_H, "cub3D");
	if (!engine->win)
		return (0);
	return (1);
}

static int	create_framebuffer(t_engine *engine)
{
	engine->frame.handle = mlx_new_image(engine->mlx, WIN_W, WIN_H);
	if (!engine->frame.handle)
		return (0);
	engine->frame.addr = mlx_get_data_addr(engine->frame.handle,
			&engine->frame.bpp, &engine->frame.line_len,
			&engine->frame.endian);
	if (!engine->frame.addr)
		return (0);
	return (1);
}

int	engine_init(t_engine *engine, int argc, char **argv)
{
	ft_bzero(engine, sizeof(t_engine));
	if (argc != 2)
		return (0);
	if (!parse_cub_file(argv[1], &engine->cub))
		return (0);
	engine->mlx = mlx_init();
	if (!engine->mlx)
		return (0);
	if (!create_window(engine))
		return (0);
	if (!create_framebuffer(engine))
		return (0);
	if (!textures_load(engine))
		return (0);
	player_init(engine);
	return (1);
}

int	textures_load(t_engine *engine)
{
	int	i;

	i = 0;
	while (i < MAX_TEXTURES)
	{
		engine->textures[i].handle = mlx_xpm_file_to_image(engine->mlx,
				engine->cub.texture_paths[i], &engine->textures[i].width,
				&engine->textures[i].height);
		if (!engine->textures[i].handle)
			return (0);
		engine->textures[i].addr = mlx_get_data_addr(engine->textures[i].handle,
				&engine->textures[i].bpp, &engine->textures[i].line_len,
				&engine->textures[i].endian);
		if (!engine->textures[i].addr)
			return (0);
		i++;
	}
	return (1);
}
