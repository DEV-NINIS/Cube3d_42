// parse_colors.c
#include "../../includes/cube3d.h"

static int	parse_color_values(const char *line, int *color, int line_num)
{
	char	*trimmed;
	char	**rgb_values;
	int		count;
	int		rgb[3];
	int		i;

	// Sauter l'identifiant (F ou C) et les espaces
	trimmed = (char *)line + 1;
	while (*trimmed && ft_isspace(*trimmed))
		trimmed++;
	
	// Vérifier qu'il reste du contenu
	if (!*trimmed)
		return (printf("Error\nMissing color values at line %d\n", line_num), 0);
	
	// Séparer par les virgules
	rgb_values = ft_split(trimmed, ',');
	if (!rgb_values)
		return (printf("Error\nMemory allocation failed\n"), 0);
	
	// Compter les valeurs
	count = 0;
	while (rgb_values[count] && count < 4)
		count++;
	
	// Vérifier exactement 3 valeurs
	if (count != 3)
	{
		free_string_array(rgb_values);
		return (printf("Error\nExpected 3 color values, got %d at line %d\n", 
				count, line_num), 0);
	}
	
	// Parser chaque valeur
	i = -1;
	while (++i < 3)
	{
		char	*clean = ft_strtrim(rgb_values[i], " \t");
		if (!clean || !ft_isnumeric(clean))
		{
			free_string_array(rgb_values);
			if (clean) free(clean);
			return (printf("Error\nInvalid color value '%s' at line %d\n", 
					rgb_values[i], line_num), 0);
		}
		
		rgb[i] = ft_atoi(clean);
		free(clean);
		
		if (rgb[i] < 0 || rgb[i] > 255)
		{
			free_string_array(rgb_values);
			return (printf("Error\nColor value %d out of range [0-255] at line %d\n", 
					rgb[i], line_num), 0);
		}
	}
	
	// Vérifier qu'il n'y a rien après la dernière valeur
	if (rgb_values[3] != NULL && ft_strlen(rgb_values[3]) > 0)
	{
		free_string_array(rgb_values);
		return (printf("Error\nExtra elements after color values at line %d\n", 
				line_num), 0);
	}
	
	// Convertir en valeur unique 0xRRGGBB
	*color = (rgb[0] << 16) | (rgb[1] << 8) | rgb[2];
	
	free_string_array(rgb_values);
	return (1);
}

int	parse_color_line(t_cub *cub, const char *line, int line_num)
{
	char	trimmed_line[256];
	char	*clean;
	char	type;
	
	// Copier et nettoyer la ligne
	ft_strlcpy(trimmed_line, line, sizeof(trimmed_line));
	clean = ft_strtrim(trimmed_line, " \t\n\r");
	
	if (!clean || ft_strlen(clean) == 0)
	{
		if (clean) free(clean);
		return (0);
	}
	
	// Vérifier si c'est une ligne de couleur
	type = clean[0];
	if (type != 'F' && type != 'C')
	{
		free(clean);
		return (0);
	}
	
	// Vérifier doublon
	if ((type == 'F' && cub->floor_color != -1) ||
		(type == 'C' && cub->ceiling_color != -1))
	{
		free(clean);
		return (printf("Error\nDuplicate color definition at line %d\n", line_num), 0);
	}
	
	// Parser les valeurs de couleur
	int success;
	if (type == 'F')
		success = parse_color_values(clean, &cub->floor_color, line_num);
	else
		success = parse_color_values(clean, &cub->ceiling_color, line_num);
	
	free(clean);
	return (success);
}


int validate_configuration_color(char **config_lines, int count, t_cub *cub)
{
    int have_f = 0, have_c = 0;
    int line_num = 1; // Numéro de ligne dans le fichier original
    
    cub->floor_color = -1;
    cub->ceiling_color = -1;
    
    for (int i = 0; i < count; ++i)
    {
        char *trimmed = ft_strtrim(config_lines[i], " \t");
        if (!trimmed || trimmed[0] == '\0')
        {
            if (trimmed) free(trimmed);
            line_num++;
            continue;
        }
        
        // Vérifier si c'est F ou C
        if (trimmed[0] == 'F' || trimmed[0] == 'C')
        {
            char type = trimmed[0];
            
            // Vérifier doublon
            if ((type == 'F' && have_f) || (type == 'C' && have_c))
            {
                free(trimmed);
                return (printf("Error\nDuplicate %s color at line %d\n", 
                        type == 'F' ? "floor" : "ceiling", line_num), 0);
            }
            
            // Parser la couleur
            int success;
            if (type == 'F')
                success = parse_color_values(trimmed, &cub->floor_color, line_num);
            else
                success = parse_color_values(trimmed, &cub->ceiling_color, line_num);
            
            if (!success)
            {
                free(trimmed);
                return 0; // parse_color_values a déjà affiché l'erreur
            }
            
            if (type == 'F') have_f = 1;
            else have_c = 1;
        }
        
        free(trimmed);
        line_num++;
    }
    
    // Vérification finale
    if (!have_f)
        return (printf("Error\nMissing floor color (F)\n"), 0);
    if (!have_c)
        return (printf("Error\nMissing ceiling color (C)\n"), 0);
    
    return 1;
}