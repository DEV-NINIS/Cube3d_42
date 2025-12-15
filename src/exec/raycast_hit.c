/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_hit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmassin <gmassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 10:07:55 by gmassin           #+#    #+#             */
/*   Updated: 2025/12/12 10:08:04 by gmassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

static void	set_step_x(t_player *player, t_ray *ray)
{
	ray->step_x = 1;
	if (ray->dir_x < 0.0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (player->x - ray->map_x) * ray->delta_dist_x;
		return ;
	}
	ray->side_dist_x = (ray->map_x + 1.0 - player->x) * ray->delta_dist_x;
}

static void	set_step_y(t_player *player, t_ray *ray)
{
	ray->step_y = 1;
	if (ray->dir_y < 0.0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (player->y - ray->map_y) * ray->delta_dist_y;
		return ;
	}
	ray->side_dist_y = (ray->map_y + 1.0 - player->y) * ray->delta_dist_y;
}

static void	dda_advance(t_ray *ray)
{
	if (ray->side_dist_x < ray->side_dist_y)
	{
		ray->side_dist_x += ray->delta_dist_x;
		ray->map_x += ray->step_x;
		ray->hit_side = 0;
		return ;
	}
	ray->side_dist_y += ray->delta_dist_y;
	ray->map_y += ray->step_y;
	ray->hit_side = 1;
}

static int	is_outside_map(t_engine *engine, t_ray *ray)
{
	if (ray->map_x < 0 || ray->map_y < 0)
		return (1);
	if (ray->map_x >= engine->cub.map_width)
		return (1);
	if (ray->map_y >= engine->cub.map_height)
		return (1);
	return (0);
}

int	raycast_find_wall_hit(t_engine *engine, t_ray *ray)
{
	t_player	*player;

	player = &engine->player;
	set_step_x(player, ray);
	set_step_y(player, ray);
	while (1)
	{
		dda_advance(ray);
		if (is_outside_map(engine, ray))
			return (0);
		if (engine->cub.map[ray->map_y][ray->map_x] == '1')
			return (1);
	}
}
