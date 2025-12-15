/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_configuration_utils_2.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 19:39:08 by ael-fari          #+#    #+#             */
/*   Updated: 2025/12/15 19:39:10 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

int	read_rgb_comma(char **p)
{
	*p = skip_ws(*p);
	if (**p != ',')
		return (0);
	(*p)++;
	return (1);
}

int	parse_rgb_components(char *s, int *rgb)
{
	char	*p;

	p = s;
	if (!read_rgb_value(&p, &rgb[0]))
		return (0);
	if (!read_rgb_comma(&p))
		return (0);
	if (!read_rgb_value(&p, &rgb[1]))
		return (0);
	if (!read_rgb_comma(&p))
		return (0);
	if (!read_rgb_value(&p, &rgb[2]))
		return (0);
	p = skip_ws(p);
	if (*p != '\0')
		return (0);
	return (1);
}

int	parse_rgb_str(char *s)
{
	int	rgb[3];

	if (!s)
		return (-1);
	if (!parse_rgb_components(s, rgb))
		return (-1);
	return (pack_rgb(rgb[0], rgb[1], rgb[2]));
}

char	*extract_id(char *line, char *idbuf)
{
	char	*p;
	int		i;

	p = skip_ws(line);
	i = 0;
	while (*p && !ft_isspace((unsigned char)*p) && i < 2)
		idbuf[i++] = *p++;
	idbuf[i] = '\0';
	return (p);
}

void	init_cub_config(t_cub *cub)
{
	int	i;

	i = 0;
	while (i < MAX_TEXTURES)
	{
		cub->texture_paths[i] = NULL;
		i++;
	}
	cub->floor_color = -1;
	cub->ceiling_color = -1;
}
