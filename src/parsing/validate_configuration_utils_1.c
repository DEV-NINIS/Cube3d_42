/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_configuration_utils_1.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 19:39:17 by ael-fari          #+#    #+#             */
/*   Updated: 2025/12/15 19:39:19 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

char	*skip_ws(char *s)
{
	while (s && *s && (unsigned char)*s <= ' ')
		s++;
	return (s);
}

void	rtrim(char *s)
{
	int	i;

	i = (int)ft_strlen(s) - 1;
	while (i >= 0 && (unsigned char)s[i] <= ' ')
		s[i--] = '\0';
}

int	is_tex_id(const char *id)
{
	return (!ft_strcmp(id, "NO") || !ft_strcmp(id, "SO") || !ft_strcmp(id, "WE")
		|| !ft_strcmp(id, "EA"));
}

int	pack_rgb(int r, int g, int b)
{
	return (((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF));
}

int	read_rgb_value(char **p, int *out)
{
	int	val;

	*p = skip_ws(*p);
	if (!ft_isdigit(**p))
		return (0);
	val = ft_atoi(*p);
	if (val < 0 || val > 255)
		return (0);
	while (ft_isdigit(**p))
		(*p)++;
	*out = val;
	return (1);
}
