#include "../../includes/cube3d.h"

static int print_error(const char *msg)
{
    write(2, "Error\n", 6);
    write(2, msg, ft_strlen(msg));
    write(2, "\n", 1);
    return (0);
}

int ft_strcmp(const char *s1, const char *s2)
{
    int i = 0;

    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

static int has_cub_extension(const char *filename)
{
    int len = ft_strlen(filename);
    if (len < 4)
        return (0);
    return (ft_strcmp(filename + (len - 4), ".cub") == 0);
}

// ---------------------------------------------------------
// Fonction principale demandée
// ---------------------------------------------------------
int check_arguments(int argc, char **argv)
{
    int fd;

    // Vérifier le nombre d'arguments
    if (argc != 2)
        return print_error("Invalid arguments");

    // Vérifier extension du fichier
    if (!has_cub_extension(argv[1]))
        return print_error("File is not a .cub");

    // Vérifier si le fichier peut être ouvert
    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        return print_error("Cannot open .cub file");

    close(fd);
    return (1); // OK ✔
}

int    is_cub_file(const char *filename)
{
    size_t  len;

    if (!filename)
        return (0);
    len = ft_strlen(filename);
    if (len < 5) // au moins 4 char + ".cub"
        return (0);
    if (ft_strcmp(filename + (len - 4), ".cub") == 0)
        return (1);
    return (0);
}

void    free_cub(t_cub *cub)
{
    int i;

    if (!cub)
        return;

    /* Libérer les textures */
    for (i = 0; i < MAX_TEXTURES; i++)
    {
        if (cub->texture_paths[i])
        {
            free(cub->texture_paths[i]);
            cub->texture_paths[i] = NULL;
        }
    }

    /* Libérer la map */
    if (cub->map)
    {
        i = 0;
        while (cub->map[i])
        {
            free(cub->map[i]);
            i++;
        }
        free(cub->map);
        cub->map = NULL;
    }
}


void	print_cub_info(t_cub *cub)
{
	if (!cub)
	{
		printf("Error: cub is NULL\n");
		return;
	}

	printf("\n╔════════════════════════════════════════╗\n");
	printf("║          CUB3D CONFIGURATION          ║\n");
	printf("╚════════════════════════════════════════╝\n\n");

	// 1. TEXTURES
	printf("📁 TEXTURES:\n");
	printf("  ┌──────────────────────────────────┐\n");
	const char *tex_names[4] = {"NO", "SO", "WE", "EA"};
	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		if (cub->texture_paths[i])
			printf("  │ %-2s: %-30s │\n", tex_names[i], cub->texture_paths[i]);
		else
			printf("  │ %-2s: %-30s │\n", tex_names[i], "(not set)");
	}
	printf("  └──────────────────────────────────┘\n\n");

	// 2. COULEURS
	printf("🎨 COLORS:\n");
	printf("  ┌──────────────────────────────────┐\n");
	
	// Extraire les composants RGB du floor
	int fr = (cub->floor_color >> 16) & 0xFF;
	int fg = (cub->floor_color >> 8) & 0xFF;
	int fb = cub->floor_color & 0xFF;
	
	// Extraire les composants RGB du ceiling
	int cr = (cub->ceiling_color >> 16) & 0xFF;
	int cg = (cub->ceiling_color >> 8) & 0xFF;
	int cb = cub->ceiling_color & 0xFF;
	
	printf("  │ Floor  : RGB(%3d, %3d, %3d) = 0x%06X │\n", fr, fg, fb, cub->floor_color);
	printf("  │ Ceiling: RGB(%3d, %3d, %3d) = 0x%06X │\n", cr, cg, cb, cub->ceiling_color);
	printf("  └──────────────────────────────────┘\n\n");

	// 3. INFORMATIONS DE LA MAP
	printf("🗺️  MAP INFO:\n");
	printf("  ┌──────────────────────────────────┐\n");
	printf("  │ Size      : %3d x %-3d (W x H)      │\n", 
		   cub->map_width, cub->map_height);
	printf("  │ Player    : (%d, %d)                │\n", 
		   cub->player_x, cub->player_y);
	printf("  │ Direction : %c                      │\n", cub->player_dir);
	printf("  └──────────────────────────────────┘\n\n");

	// 4. LA MAP ELLE-MÊME (si elle existe)
	if (cub->map && cub->map_height > 0)
	{
		printf("🧱 MAP LAYOUT:\n");
		printf("  ");
		for (int x = 0; x < cub->map_width + 2; x++)
			printf("─");
		printf("\n");
		
		for (int y = 0; y < cub->map_height; y++)
		{
			printf("  │");
			for (int x = 0; x < cub->map_width; x++)
			{
				char c = cub->map[y][x];
				// Mettre en valeur le joueur
				if (y == cub->player_y && x == cub->player_x)
					printf("\033[1;32m%c\033[0m", cub->player_dir);
				else if (c == '1')
					printf("\033[1;37m█\033[0m");  // Murs en blanc
				else if (c == '0')
					printf(" ");  // Espaces vides
				else if (c == ' ')
					printf("·");  // Hors-map
				else
					printf("%c", c);
			}
			printf("│\n");
		}
		
		printf("  ");
		for (int x = 0; x < cub->map_width + 2; x++)
			printf("─");
		printf("\n\n");
	}
	else
	{
		printf("⚠️  No map loaded\n\n");
	}

	printf("══════════════════════════════════════════\n");
}