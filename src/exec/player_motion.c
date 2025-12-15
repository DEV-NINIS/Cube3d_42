/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_motion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmassin <gmassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 10:03:40 by gmassin           #+#    #+#             */
/*   Updated: 2025/12/12 10:10:34 by gmassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"
#include <math.h>

static void	move_forward_backward(t_engine *engine, double speed)
{
	t_player	*player;
	double		new_x;
	double		new_y;
	double		step_x;
	double		step_y;

	player = &engine->player;
	if (engine->keys.move_forward || engine->keys.move_backward)
	{
		step_x = player->dir_x * speed;
		step_y = player->dir_y * speed;
		if (engine->keys.move_backward)
		{
			step_x = -step_x;
			step_y = -step_y;
		}
		new_x = player->x + step_x;
		new_y = player->y + step_y;
		if (step_x != 0.0 && player_can_move_x(&engine->cub, new_x,
				player->y, step_x))
			player->x = new_x;
		if (step_y != 0.0 && player_can_move_y(&engine->cub, player->x,
				new_y, step_y))
			player->y = new_y;
	}
}

static void	strafe_left_right(t_engine *engine, double speed)
{
	t_player	*player;
	double		new_x;
	double		new_y;
	double		step_x;
	double		step_y;

	player = &engine->player;
	if (engine->keys.strafe_left || engine->keys.strafe_right)
	{
		step_x = player->plane_x * speed;
		step_y = player->plane_y * speed;
		if (engine->keys.strafe_left)
		{
			step_x = -step_x;
			step_y = -step_y;
		}
		new_x = player->x + step_x;
		new_y = player->y + step_y;
		if (step_x != 0.0 && player_can_move_x(&engine->cub, new_x,
				player->y, step_x))
			player->x = new_x;
		if (step_y != 0.0 && player_can_move_y(&engine->cub, player->x,
				new_y, step_y))
			player->y = new_y;
	}
}

static void	rotate_left_right(t_engine *engine, double speed)
{
	t_player	*player;
	double		rot;
	double		old_dir_x;
	double		old_plane_x;

	player = &engine->player;
	rot = 0.0;
	if (engine->keys.turn_left)
		rot = speed;
	if (engine->keys.turn_right)
		rot = -speed;
	if (rot == 0.0)
		return ;
	old_dir_x = player->dir_x;
	player->dir_x = player->dir_x * cos(rot) - player->dir_y * sin(rot);
	player->dir_y = old_dir_x * sin(rot) + player->dir_y * cos(rot);
	old_plane_x = player->plane_x;
	player->plane_x = player->plane_x * cos(rot) - player->plane_y * sin(rot);
	player->plane_y = old_plane_x * sin(rot) + player->plane_y * cos(rot);
}

void	player_update(t_engine *engine)
{
	move_forward_backward(engine, MOVE_SPEED);
	strafe_left_right(engine, MOVE_SPEED);
	rotate_left_right(engine, ROT_SPEED);
}
