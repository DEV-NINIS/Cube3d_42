/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map_utils_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 19:40:52 by ael-fari          #+#    #+#             */
/*   Updated: 2025/12/15 19:40:55 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

char	**normalize_map(char **lines, int count, int *out_h, int *out_w)
{
	int		maxlen;
	char	**map;
	int		i;

	maxlen = get_max_len(lines, count);
	*out_h = count;
	*out_w = maxlen;
	map = allocate_map(count);
	if (!map)
		return (NULL);
	i = 0;
	while (i < count)
	{
		map[i] = (char *)malloc(sizeof(char) * (maxlen + 1));
		if (!map[i])
		{
			while (--i >= 0)
				free(map[i]);
			free(map);
			return (NULL);
		}
		i++;
	}
	normalize_lines(lines, map, count, maxlen);
	return (map);
}

int	flood_fill(t_ff *ff, int y, int x)
{
	if (y < 0 || y >= ff->h || x < 0 || x >= ff->w)
		return (0);
	if (ff->map[y][x] == ' ')
		return (0);
	if (ff->vis[y][x])
		return (1);
	if (ff->map[y][x] == '1')
	{
		ff->vis[y][x] = 1;
		return (1);
	}
	ff->vis[y][x] = 1;
	if (!flood_fill(ff, y - 1, x))
		return (0);
	if (!flood_fill(ff, y + 1, x))
		return (0);
	if (!flood_fill(ff, y, x - 1))
		return (0);
	if (!flood_fill(ff, y, x + 1))
		return (0);
	return (1);
}

int	normalize_map_and_check(char **lines, int count, t_cub *cub)
{
	cub->map = normalize_map(lines, count, &cub->map_height, &cub->map_width);
	if (!cub->map)
		return (0);
	if (cub->player_y < 0 || cub->player_y >= cub->map_height
		|| cub->player_x < 0 || cub->player_x >= cub->map_width)
		return (0);
	cub->map[cub->player_y][cub->player_x] = '0';
	return (1);
}

int	allocate_visited(t_cub *cub, int ***visited)
{
	int	i;
	int	k;

	*visited = (int **)malloc(sizeof(int *) * cub->map_height);
	if (!*visited)
		return (0);
	i = 0;
	while (i < cub->map_height)
	{
		(*visited)[i] = (int *)ft_calloc(cub->map_width, sizeof(int));
		if (!(*visited)[i])
		{
			k = 0;
			while (k < i)
				free((*visited)[k++]);
			free(*visited);
			return (0);
		}
		i++;
	}
	return (1);
}
