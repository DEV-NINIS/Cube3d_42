/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_colors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 19:48:25 by ael-fari          #+#    #+#             */
/*   Updated: 2025/12/15 19:48:30 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

int	check_duplicate_color(t_cub *cub, char type, int line_num)
{
	if ((type == 'F' && cub->floor_color != -1) || (type == 'C'
			&& cub->ceiling_color != -1))
	{
		printf("Error\nDuplicate color definition at line %d\n", line_num);
		return (0);
	}
	return (1);
}

int	process_color_line(t_cub *cub, char *clean, int line_num)
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
