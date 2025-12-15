/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map_utils_1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 19:41:04 by ael-fari          #+#    #+#             */
/*   Updated: 2025/12/15 19:41:05 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

int	is_valid_map_char(char c)
{
	if (c == '\0' || c == '\n')
		return (1);
	return (c == '0' || c == '1' || c == ' ' || c == 'N' || c == 'S' || c == 'E'
		|| c == 'W');
}

int	is_player_char(char c)
{
	return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

int	get_max_len(char **lines, int count)
{
	int	i;
	int	maxlen;
	int	len;

	i = 0;
	maxlen = 0;
	while (i < count)
	{
		len = (int)ft_strlen(lines[i]);
		if (len > maxlen)
			maxlen = len;
		i++;
	}
	return (maxlen);
}

char	**allocate_map(int count)
{
	char	**map;
	int		i;

	map = (char **)malloc(sizeof(char *) * count);
	if (!map)
		return (NULL);
	i = 0;
	while (i < count)
	{
		map[i] = NULL;
		i++;
	}
	return (map);
}

void	normalize_lines(char **lines, char **map, int count, int maxlen)
{
	int	i;
	int	j;

	i = 0;
	while (i < count)
	{
		j = 0;
		while (j < maxlen)
		{
			if (j < (int)ft_strlen(lines[i]))
				map[i][j] = lines[i][j];
			else
				map[i][j] = ' ';
			j++;
		}
		map[i][maxlen] = '\0';
		i++;
	}
}
