// parse_colors.c
#include "../../includes/cube3d.h"

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_colors.c                                     :+:      :+:    :+:   */
/*                                                        +:+ +:+         +:+     */
/*   By: <yourname> <email>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by <yourname>       #+#    #+#             */
/*   Updated: 2024/01/01 00:00:00 by <yourname>      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

static int	check_color_range(int value, int line_num)
{
	if (value < 0 || value > 255)
	{
		printf("Error\nColor value %d out of range [0-255] at line %d\n",
			value, line_num);
		return (0);
	}
	return (1);
}

static int	validate_color_value(char *str, int *value, int line_num)
{
	char	*clean;

	clean = ft_strtrim(str, " \t");
	if (!clean || !ft_isnumeric(clean))
	{
		if (clean)
			free(clean);
		printf("Error\nInvalid color value '%s' at line %d\n", str, line_num);
		return (0);
	}
	*value = ft_atoi(clean);
	free(clean);
	return (check_color_range(*value, line_num));
}

static int	parse_rgb_values(char **rgb_values, int rgb[3], int line_num)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		if (!validate_color_value(rgb_values[i], &rgb[i], line_num))
			return (0);
		i++;
	}
	return (1);
}

static int	validate_rgb_count(char **rgb_values, int line_num)
{
	int	count;

	count = 0;
	while (rgb_values[count] && count < 4)
		count++;
	if (count != 3)
	{
		printf("Error\nExpected 3 color values, got %d at line %d\n",
			count, line_num);
		return (0);
	}
	if (rgb_values[3] && ft_strlen(rgb_values[3]) > 0)
	{
		printf("Error\nExtra elements after color values at line %d\n",
			line_num);
		return (0);
	}
	return (1);
}

static int	parse_color_values(const char *line, int *color, int line_num)
{
	char	*trimmed;
	char	**rgb_values;
	int		rgb[3];

	trimmed = (char *)line + 1;
	while (*trimmed && ft_isspace(*trimmed))
		trimmed++;
	if (!*trimmed)
		return (printf("Error\nMissing color values at line %d\n", line_num), 0);
	rgb_values = ft_split(trimmed, ',');
	if (!rgb_values)
		return (printf("Error\nMemory allocation failed\n"), 0);
	if (!validate_rgb_count(rgb_values, line_num))
	{
		free_string_array(rgb_values);
		return (0);
	}
	if (!parse_rgb_values(rgb_values, rgb, line_num))
	{
		free_string_array(rgb_values);
		return (0);
	}
	*color = (rgb[0] << 16) | (rgb[1] << 8) | rgb[2];
	free_string_array(rgb_values);
	return (1);
}

static int	check_duplicate_color(t_cub *cub, char type, int line_num)
{
	if ((type == 'F' && cub->floor_color != -1)
		|| (type == 'C' && cub->ceiling_color != -1))
	{
		printf("Error\nDuplicate color definition at line %d\n", line_num);
		return (0);
	}
	return (1);
}

static int	process_color_line(t_cub *cub, char *clean, int line_num)
{
	char	type;
	int		success;

	type = clean[0];
	if (!check_duplicate_color(cub, type, line_num))
		return (0);
	if (type == 'F')
		success = parse_color_values(clean, &cub->floor_color, line_num);
	else
		success = parse_color_values(clean, &cub->ceiling_color, line_num);
	return (success);
}

int	parse_color_line(t_cub *cub, const char *line, int line_num)
{
	char	trimmed_line[256];
	char	*clean;
	int		success;

	ft_strlcpy(trimmed_line, line, sizeof(trimmed_line));
	clean = ft_strtrim(trimmed_line, " \t\n\r");
	if (!clean || ft_strlen(clean) == 0)
	{
		if (clean)
			free(clean);
		return (0);
	}
	if (clean[0] != 'F' && clean[0] != 'C')
	{
		free(clean);
		return (0);
	}
	success = process_color_line(cub, clean, line_num);
	free(clean);
	return (success);
}

static int	process_config_line(char *trimmed, t_cub *cub,
				int *have_f, int *have_c, int line_num)
{
	char	type;
	int		success;

	type = trimmed[0];
	if ((type == 'F' && *have_f) || (type == 'C' && *have_c))
	{
		printf("Error\nDuplicate %s color at line %d\n",
			type == 'F' ? "floor" : "ceiling", line_num);
		return (0);
	}
	if (type == 'F')
		success = parse_color_values(trimmed, &cub->floor_color, line_num);
	else
		success = parse_color_values(trimmed, &cub->ceiling_color, line_num);
	if (!success)
		return (0);
	if (type == 'F')
		*have_f = 1;
	else
		*have_c = 1;
	return (1);
}

static int	check_missing_colors(int have_f, int have_c)
{
	if (!have_f)
		return (printf("Error\nMissing floor color (F)\n"), 0);
	if (!have_c)
		return (printf("Error\nMissing ceiling color (C)\n"), 0);
	return (1);
}

int	validate_configuration_color(char **config_lines, int count, t_cub *cub)
{
	int	have_f;
	int	have_c;
	int	line_num;
	int	i;

	have_f = 0;
	have_c = 0;
	line_num = 1;
	cub->floor_color = -1;
	cub->ceiling_color = -1;
	i = -1;
	while (++i < count)
	{
		char	*trimmed;

		trimmed = ft_strtrim(config_lines[i], " \t");
		if (!trimmed || trimmed[0] == '\0')
		{
			if (trimmed)
				free(trimmed);
			line_num++;
			continue ;
		}
		if (trimmed[0] == 'F' || trimmed[0] == 'C')
		{
			if (!process_config_line(trimmed, cub, &have_f, &have_c, line_num))
			{
				free(trimmed);
				return (0);
			}
		}
		free(trimmed);
		line_num++;
	}
	return (check_missing_colors(have_f, have_c));
}