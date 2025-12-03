#include "../../includes/cube3d.h"


int	main(int argc, char **argv)
{
	t_cub	cub;

	if (argc != 2)
	{
		printf("Usage: %s <file.cub>\n", argv[0]);
		return (1);
	}
	if (!parse_cub_file(argv[1], &cub))
		return (1);
	printf("✅ Fichier parsé avec succès !\n");
	printf("Textures:\n");
	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		if (cub.texture_paths[i])
			printf("  Texture %d: %s\n", i, cub.texture_paths[i]);
	}
	// printf("Couleurs: F=%d, C=%d\n", cub.floor_color, cub.ceiling_color);
	// printf("Map: %dx%d\n", cub.map_width, cub.map_height);
	// Libérer la mémoire ici plus tard
	return (0);
}