/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cube.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 18:32:31 by ael-fari          #+#    #+#             */
/*   Updated: 2025/12/15 18:32:33 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

int	is_empty_line(char *line)
{
	int	i;

	if (!line)
		return (1);
	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n'
			&& line[i] != '\r' && line[i] != '\v' && line[i] != '\f')
			return (0);
		i++;
	}
	return (1);
}

int	is_valid_parameter_line(char *line)
{
	int	i;

	i = 0;
	if (!line)
		return (0);
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	if (!ft_strncmp(&line[i], "NO ", 3))
		return (1);
	if (!ft_strncmp(&line[i], "SO ", 3))
		return (1);
	if (!ft_strncmp(&line[i], "WE ", 3))
		return (1);
	if (!ft_strncmp(&line[i], "EA ", 3))
		return (1);
	if (!ft_strncmp(&line[i], "F ", 2))
		return (1);
	if (!ft_strncmp(&line[i], "C ", 2))
		return (1);
	return (0);
}

int	is_config_line(const char *line)
{
	return (is_valid_parameter_line((char *)line));
}

int	skip_empty_start(char **lines, int count)
{
	int	i;

	i = 0;
	while (i < count && is_empty_line(lines[i]))
		i++;
	return (i);
}
