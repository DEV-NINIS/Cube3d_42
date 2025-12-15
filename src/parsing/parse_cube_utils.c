/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cube_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 19:37:53 by ael-fari          #+#    #+#             */
/*   Updated: 2025/12/15 19:37:56 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

int	read_config(char **lines, int count, int start)
{
	int	i;
	int	params;

	i = start;
	params = 0;
	while (i < count && params < 6)
	{
		if (is_empty_line(lines[i]))
			i++;
		else if (is_valid_parameter_line(lines[i]))
		{
			params++;
			i++;
		}
		else
			break ;
	}
	if (params != 6)
		return (-1);
	return (i);
}

int	find_map_start(char **lines, int count, int i)
{
	while (i < count && is_empty_line(lines[i]))
		i++;
	if (i >= count)
		return (-1);
	return (i);
}

int	validate_map_block(char **lines, int count, int start)
{
	int	i;
	int	empty;

	i = start;
	empty = 0;
	while (i < count)
	{
		if (is_empty_line(lines[i]))
			empty = 1;
		else if (is_valid_parameter_line(lines[i]))
			return (printf("Error\nConfiguration after map start\n"), 0);
		else if (empty)
			return (printf("Error\nEmpty line inside map\n"), 0);
		i++;
	}
	return (1);
}

int	separate_config_and_map(char **lines, int count, char ***map,
		t_separation *sep)
{
	int	i;
	int	config_start;
	int	map_start;

	i = skip_empty_start(lines, count);
	config_start = i;
	i = read_config(lines, count, i);
	if (i == -1)
		return (printf("Error\nInvalid configuration\n"), 0);
	map_start = find_map_start(lines, count, i);
	if (map_start == -1)
		return (printf("Error\nMissing map\n"), 0);
	if (!validate_map_block(lines, count, map_start))
		return (0);
	sep->config_lines = lines;
	sep->config_start = config_start;
	sep->config_count = map_start - config_start;
	sep->map_lines = &lines[map_start];
	sep->map_start = 0;
	sep->map_count = count - map_start;
	*map = sep->map_lines;
	return (1);
}

int	parse_cub_file(const char *filename, t_cub *cub)
{
	char			**lines;
	int				line_count;
	t_separation	sep;

	if (!is_cub_file(filename))
		return (printf("Error\nInvalid file\n"), 0);
	ft_memset(cub, 0, sizeof(t_cub));
	cub->floor_color = -1;
	cub->ceiling_color = -1;
	ft_memset(&sep, 0, sizeof(t_separation));
	lines = read_cub_file(filename, &line_count);
	if (!lines || !separate_config_and_map(lines, line_count, &sep.map_lines,
			&sep))
		return (printf("Error\nInvalid file format or cannot read it\n"),
			free_string_array(lines), 0);
	if (!validate_configuration(sep.config_lines + sep.config_start,
			sep.config_count, cub))
		return (free_string_array(lines),
			printf("Error\nInvalid configuration\n"), 0);
	if (cub->floor_color == -1 || cub->ceiling_color == -1)
		return (free_string_array(lines), printf("Error\nColor Error\n"), 0);
	if (!validate_map(sep.map_lines, sep.map_count, cub))
		return (free_string_array(lines), printf("Error\nInvalid map\n"), 0);
	return (free_string_array(lines), 1);
}
