/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 18:33:45 by ael-fari          #+#    #+#             */
/*   Updated: 2025/12/15 18:33:46 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

int	check_map_closed(t_cub *cub, int **visited)
{
	t_ff	ff;
	int		i;

	ff.map = cub->map;
	ff.h = cub->map_height;
	ff.w = cub->map_width;
	ff.vis = visited;
	if (!flood_fill(&ff, cub->player_y, cub->player_x))
	{
		i = 0;
		while (i < cub->map_height)
			free(visited[i++]);
		free(visited);
		return (0);
	}
	i = 0;
	while (i < cub->map_height)
		free(visited[i++]);
	free(visited);
	return (1);
}

int	validate_map(char **lines, int count, t_cub *cub)
{
	int	**visited;

	if (!check_map_chars_and_player(lines, count, cub))
		return (printf("Error\nInvalid number of players\n"), 0);
	if (!normalize_map_and_check(lines, count, cub))
		return (printf("Error\nPlayer position out of bounds\n"), 0);
	if (!allocate_visited(cub, &visited))
		return (0);
	if (!check_map_closed(cub, visited))
		return (0);
	return (1);
}

int	check_line_chars(char *line, int y, t_cub *cub, int *players)
{
	int	x;
	int	valid;

	x = 0;
	while (line[x])
	{
		valid = is_valid_map_char(line[x]);
		if (!valid)
			return (0);
		if (is_player_char(line[x]))
		{
			cub->player_x = x;
			cub->player_y = y;
			cub->player_dir = line[x];
			(*players)++;
		}
		x++;
	}
	return (1);
}

int	check_map_chars_and_player(char **lines, int count, t_cub *cub)
{
	int	i;
	int	players;
	int	valid;

	i = 0;
	players = 0;
	while (i < count)
	{
		valid = check_line_chars(lines[i], i, cub, &players);
		if (!valid)
			return (0);
		i++;
	}
	if (players != 1)
		return (0);
	return (1);
}
