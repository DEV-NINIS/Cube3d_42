/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmassin <gmassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:52:43 by gmassin           #+#    #+#             */
/*   Updated: 2025/12/12 10:06:56 by gmassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

static void	set_dir_north(t_player *player)
{
	player->dir_x = 0.0;
	player->dir_y = -1.0;
	player->plane_x = 0.66;
	player->plane_y = 0.0;
}

static void	set_dir_south(t_player *player)
{
	player->dir_x = 0.0;
	player->dir_y = 1.0;
	player->plane_x = -0.66;
	player->plane_y = 0.0;
}

static void	set_dir_east(t_player *player)
{
	player->dir_x = 1.0;
	player->dir_y = 0.0;
	player->plane_x = 0.0;
	player->plane_y = 0.66;
}

static void	set_dir_west(t_player *player)
{
	player->dir_x = -1.0;
	player->dir_y = 0.0;
	player->plane_x = 0.0;
	player->plane_y = -0.66;
}

void	player_init(t_engine *engine)
{
	t_player	*player;
	t_cub		*cub;

	player = &engine->player;
	cub = &engine->cub;
	player->x = (double)cub->player_x + 0.5;
	player->y = (double)cub->player_y + 0.5;
	if (cub->player_dir == 'N')
		set_dir_north(player);
	else if (cub->player_dir == 'S')
		set_dir_south(player);
	else if (cub->player_dir == 'E')
		set_dir_east(player);
	else
		set_dir_west(player);
}
