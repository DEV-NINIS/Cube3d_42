/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_collision.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmassin <gmassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 10:03:13 by gmassin           #+#    #+#             */
/*   Updated: 2025/12/12 10:03:23 by gmassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

int	cub_is_walkable(t_cub *cub, double x, double y)
{
	int		map_x;
	int		map_y;
	char	cell;

	map_x = (int)x;
	map_y = (int)y;
	if (map_x < 0 || map_y < 0
		|| map_x >= cub->map_width || map_y >= cub->map_height)
		return (0);
	cell = cub->map[map_y][map_x];
	return (cell == '0');
}

int	player_can_move_x(t_cub *cub, double new_x, double y, double dir_x)
{
	double	radius;
	double	sign;

	radius = PLAYER_RADIUS;
	sign = 1.0;
	if (dir_x < 0.0)
		sign = -1.0;
	if (!cub_is_walkable(cub, new_x + sign * radius, y))
		return (0);
	if (!cub_is_walkable(cub, new_x + sign * radius, y + radius))
		return (0);
	if (!cub_is_walkable(cub, new_x + sign * radius, y - radius))
		return (0);
	return (1);
}

int	player_can_move_y(t_cub *cub, double x, double new_y, double dir_y)
{
	double	radius;
	double	sign;

	radius = PLAYER_RADIUS;
	sign = 1.0;
	if (dir_y < 0.0)
		sign = -1.0;
	if (!cub_is_walkable(cub, x, new_y + sign * radius))
		return (0);
	if (!cub_is_walkable(cub, x + radius, new_y + sign * radius))
		return (0);
	if (!cub_is_walkable(cub, x - radius, new_y + sign * radius))
		return (0);
	return (1);
}
