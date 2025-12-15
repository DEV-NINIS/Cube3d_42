/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmassin <gmassin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 10:04:40 by gmassin           #+#    #+#             */
/*   Updated: 2025/12/12 10:10:50 by gmassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"
#include <math.h>

static void	ray_init(t_engine *engine, t_ray *ray, int x)
{
	t_player	*player;

	player = &engine->player;
	ray->camera_x = 2.0 * (double)x / (double)WIN_W - 1.0;
	ray->dir_x = player->dir_x + player->plane_x * ray->camera_x;
	ray->dir_y = player->dir_y + player->plane_y * ray->camera_x;
	ray->map_x = (int)player->x;
	ray->map_y = (int)player->y;
	ray->delta_dist_x = 1e30;
	if (ray->dir_x != 0.0)
		ray->delta_dist_x = fabs(1.0 / ray->dir_x);
	ray->delta_dist_y = 1e30;
	if (ray->dir_y != 0.0)
		ray->delta_dist_y = fabs(1.0 / ray->dir_y);
}

static void	ray_compute_projection(t_ray *ray)
{
	ray->perp_wall_dist = ray->side_dist_x - ray->delta_dist_x;
	if (ray->hit_side == 1)
		ray->perp_wall_dist = ray->side_dist_y - ray->delta_dist_y;
	if (ray->perp_wall_dist <= 0.0001)
		ray->perp_wall_dist = 0.0001;
	ray->line_height = (int)((double)WIN_H / ray->perp_wall_dist);
	ray->draw_start = -ray->line_height / 2 + WIN_H / 2;
	ray->draw_end = ray->line_height / 2 + WIN_H / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	if (ray->draw_end >= WIN_H)
		ray->draw_end = WIN_H - 1;
}

static void	ray_prepare_texture(t_engine *engine, t_ray *ray)
{
	t_player	*player;
	t_texture	*tex;

	player = &engine->player;
	ray->tex_id = TEX_NORTH;
	if (ray->hit_side == 0 && ray->dir_x > 0.0)
		ray->tex_id = TEX_EAST;
	else if (ray->hit_side == 0 && ray->dir_x <= 0.0)
		ray->tex_id = TEX_WEST;
	else if (ray->hit_side == 1 && ray->dir_y > 0.0)
		ray->tex_id = TEX_SOUTH;
	tex = &engine->textures[ray->tex_id];
	ray->wall_x = player->y + ray->perp_wall_dist * ray->dir_y;
	if (ray->hit_side == 1)
		ray->wall_x = player->x + ray->perp_wall_dist * ray->dir_x;
	ray->wall_x -= floor(ray->wall_x);
	ray->tex_x = (int)(ray->wall_x * (double)tex->width);
	if (ray->hit_side == 0 && ray->dir_x > 0.0)
		ray->tex_x = tex->width - ray->tex_x - 1;
	if (ray->hit_side == 1 && ray->dir_y < 0.0)
		ray->tex_x = tex->width - ray->tex_x - 1;
	ray->tex_step = (double)tex->height / (double)ray->line_height;
	ray->tex_pos = (ray->draw_start - WIN_H / 2
			+ ray->line_height / 2) * ray->tex_step;
}

void	raycast_draw_column(t_engine *engine, int x)
{
	t_ray		ray;
	t_texture	*tex;
	int			y;
	int			tex_y;
	int			color;

	ray_init(engine, &ray, x);
	if (!raycast_find_wall_hit(engine, &ray))
		return ;
	ray_compute_projection(&ray);
	ray_prepare_texture(engine, &ray);
	tex = &engine->textures[ray.tex_id];
	y = ray.draw_start;
	while (y <= ray.draw_end)
	{
		tex_y = (int)ray.tex_pos;
		ray.tex_pos += ray.tex_step;
		color = (int)texture_get_pixel(tex, ray.tex_x, tex_y);
		image_put_pixel(&engine->frame, x, y, color);
		y++;
	}
}
