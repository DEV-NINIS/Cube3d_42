/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maintest.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 18:32:00 by ael-fari          #+#    #+#             */
/*   Updated: 2025/12/15 18:32:02 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

int	main(int argc, char **argv)
{
	t_cub	cub;
	int		i;

	i = 0;
	if (argc != 2)
	{
		printf("Usage: %s <file.cub>\n", argv[0]);
		return (1);
	}
	if (!parse_cub_file(argv[1], &cub))
		return (1);
	printf("✅ Fichier parsé avec succès !\n");
	printf("Textures:\n");
	while (i < MAX_TEXTURES)
	{
		if (cub.texture_paths[i])
			printf("  Texture %d: %s\n", i, cub.texture_paths[i]);
		i++;
	}
	printf("color->%d\n", cub.ceiling_color);
	free_cub(&cub);
	return (0);
}
