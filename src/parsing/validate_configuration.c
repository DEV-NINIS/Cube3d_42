/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_configuration.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 18:33:35 by ael-fari          #+#    #+#             */
/*   Updated: 2025/12/15 18:33:36 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

int	config_complete(int *flags)
{
	return (flags[0] && flags[1] && flags[2] && flags[3] && flags[4]
		&& flags[5]);
}

int	parse_texture(char *id, char *after, t_cub *cub, int *flags)
{
	char	*path;
	int		idx;
	int		len;

	if (*after == '\0')
		return (0);
	path = ft_strdup(after);
	if (!path)
		return (0);
	rtrim(path);
	len = ft_strlen(path);
	if (len < 4 || ft_strcmp(path + len - 4, ".xpm") != 0)
		return (free(path), 0);
	if (access(path, R_OK) != 0)
		return (free(path), 0);
	idx = (id[0] == 'N') * 0 + (id[0] == 'S') * 1 + (id[0] == 'W') * 2
		+ (id[0] == 'E') * 3;
	if (cub->texture_paths[idx])
		return (free(path), 0);
	cub->texture_paths[idx] = path;
	flags[idx] = 1;
	return (1);
}

int	parse_color(char *id, char *after, t_cub *cub, int *flags)
{
	int	color;
	int	idx;

	if (*after == '\0')
		return (0);
	color = parse_rgb_str(after);
	if (color < 0)
		return (0);
	if (id[0] == 'F')
		idx = 4;
	else
		idx = 5;
	if (flags[idx])
		return (0);
	if (id[0] == 'F')
		cub->floor_color = color;
	else
		cub->ceiling_color = color;
	flags[idx] = 1;
	return (1);
}

int	handle_config_line(char *line, t_cub *cub, int *flags)
{
	char	id[3];
	char	*p;
	char	*after;

	p = skip_ws(line);
	if (*p == '\0')
		return (1);
	after = extract_id(p, id);
	if (!id[0])
		return (0);
	after = skip_ws(after);
	if (is_tex_id(id))
		return (parse_texture(id, after, cub, flags));
	if (!ft_strcmp(id, "F") || !ft_strcmp(id, "C"))
		return (parse_color(id, after, cub, flags));
	return (0);
}

int	validate_configuration(char **config_lines, int count, t_cub *cub)
{
	int	i;
	int	flags[6];

	i = 0;
	while (i < 6)
		flags[i++] = 0;
	init_cub_config(cub);
	i = 0;
	while (i < count)
	{
		if (!handle_config_line(config_lines[i], cub, flags))
			return (0);
		if (config_complete(flags))
			return (1);
		i++;
	}
	if (!config_complete(flags))
		return (0);
	return (1);
}
