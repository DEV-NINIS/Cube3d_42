/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_colors_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 19:36:01 by ael-fari          #+#    #+#             */
/*   Updated: 2025/12/15 19:36:02 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

int	check_color_range(int value, int line_num)
{
	if (value < 0 || value > 255)
	{
		printf("Error\nColor value %d out of range [0-255] at line %d\n", value,
			line_num);
		return (0);
	}
	return (1);
}

int	validate_color_value(char *str, int *value, int line_num)
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

int	parse_rgb_values(char **rgb_values, int rgb[3], int line_num)
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

int	validate_rgb_count(char **rgb_values, int line_num)
{
	int	count;

	count = 0;
	while (rgb_values[count] && count < 4)
		count++;
	if (count != 3)
	{
		printf("Error\nExpected 3 color values, got %d at line %d\n", count,
			line_num);
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

int	parse_color_values(const char *line, int *color, int line_num)
{
	char	*trimmed;
	char	**rgb_values;
	int		rgb[3];

	trimmed = (char *)line + 1;
	while (*trimmed && ft_isspace(*trimmed))
		trimmed++;
	if (!*trimmed)
		return (printf("Error\nMissing color values at line %d\n", line_num),
			0);
	rgb_values = ft_split(trimmed, ',');
	if (!rgb_values)
		return (printf("Error\nMemory allocation failed\n"), 0);
	if (!validate_rgb_count(rgb_values, line_num))
	{
		return (free_string_array(rgb_values), 0);
	}
	if (!parse_rgb_values(rgb_values, rgb, line_num))
	{
		return (free_string_array(rgb_values), 0);
	}
	*color = (rgb[0] << 16) | (rgb[1] << 8) | rgb[2];
	return (free_string_array(rgb_values), 1);
}
