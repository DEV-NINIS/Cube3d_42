/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 18:31:48 by ael-fari          #+#    #+#             */
/*   Updated: 2025/12/15 18:31:50 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

void	free_cub(t_cub *cub)
{
	int	i;

	if (!cub)
		return ;
	i = 0;
	while (i < MAX_TEXTURES)
	{
		if (cub->texture_paths[i])
		{
			free(cub->texture_paths[i]);
			cub->texture_paths[i] = NULL;
		}
		i++;
	}
	if (cub->map)
	{
		i = -1;
		while (i < cub->map_height && ++i > -1)
			free(cub->map[i]);
		free(cub->map);
		cub->map = NULL;
		cub->map_width = 0;
		cub->map_height = 0;
	}
}
