#include "../../includes/cube3d.h"

static int	is_empty_line(char *line)
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

static void	skip_empty_lines(char **lines, int *index, int line_count)
{
	while (*index < line_count && is_empty_line(lines[*index]))
		(*index)++;
}

static int count_parameters(char **lines, int line_count)
{
    int i = 0;
    int count = 0;
    char *trimmed;
    int j;

    while (i < line_count && count < 6)
    {
        if (!is_empty_line(lines[i]))
        {
            // skip leading spaces
            trimmed = lines[i];
            j = 0;
            while (trimmed[j] == ' ' || trimmed[j] == '\t')
                j++;
            trimmed = &trimmed[j];

            if (!ft_strncmp(trimmed, "NO ", 3)
                || !ft_strncmp(trimmed, "SO ", 3)
                || !ft_strncmp(trimmed, "WE ", 3)
                || !ft_strncmp(trimmed, "EA ", 3)
                || !ft_strncmp(trimmed, "F ", 2)
                || !ft_strncmp(trimmed, "C ", 2))
            {
                count++;
            }
        }
        i++;
    }
    return (count);
}


static int is_valid_parameter_line(char *line)
{
    int i = 0;

    if (!line)
        return (0);

    // skip leading spaces
    while (line[i] == ' ' || line[i] == '\t')
        i++;

    // identifier must be followed by at least one space
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

int is_config_line(const char *line)
{
    char    *trimmed;
    char    *first_word;
    int     result;
    
    if (!line || !line[0])
        return (0);
    
    trimmed = ft_strtrim(line, " \t");
    if (!trimmed)
        return (0);
    
    // Extraire le premier mot (identifiant)
    first_word = ft_substr(trimmed, 0, 2);
    if (!first_word)
    {
        free(trimmed);
        return (0);
    }
    
    // Vérifier si c'est un identifiant de configuration valide
    result = 0;
    if (ft_strncmp(first_word, "NO", 2) == 0 || 
        ft_strncmp(first_word, "SO", 2) == 0 ||
        ft_strncmp(first_word, "WE", 2) == 0 ||
        ft_strncmp(first_word, "EA", 2) == 0 ||
        ft_strncmp(first_word, "F", 1) == 0 ||
        ft_strncmp(first_word, "C", 1) == 0)
    {
        // Vérifier que ce n'est pas suivi d'un caractère valide
        // (pour éviter de confondre "F" avec "FOREST" par exemple)
        char next_char = trimmed[ft_strlen(first_word)];
        if (next_char == ' ' || next_char == '\t' || next_char == '\0')
            result = 1;
    }
    
    free(first_word);
    free(trimmed);
    return (result);
}

int check_begin_map_ok(char **lines, int line_count,
                            char ***config, char ***map, t_separation *sep)
{
    int map_start = -1;
    int in_map = 0;
    int empty_line_in_map = 0;
    // Trouver où commence la map (après tous les paramètres)
    for (int i = 0; i < line_count; i++) {
        char *trimmed = ft_strtrim(lines[i], " \t");
        
        // Si ligne vide
        if (!trimmed || trimmed[0] == '\0') {
            free(trimmed);
            if (in_map) {
                // Ligne vide DANS la map = erreur
                empty_line_in_map = 1;
            }
            continue;
        }
        
        // Si c'est un paramètre (NO, SO, WE, EA, F, C)
        if (is_config_line(trimmed)) {
            free(trimmed);
            if (in_map) {
                // Paramètre APRES le début de la map = erreur
                return (printf("Error\nConfiguration after map start\n"), 0);
            }
            continue;
        }
        
        // Si on arrive ici, ce n'est ni vide ni paramètre → c'est la map
        if (!in_map) {
            in_map = 1;
            map_start = i;
            sep->map_start = i;
        }
        
        free(trimmed);
    }
    
    if (map_start == -1) {
        return (printf("Error\nNo map found\n"), 0);
    }
    
    if (empty_line_in_map) {
        return (printf("Error\nEmpty line inside map\n"), 0);
    }
    
    // Vérifier que la map est bien le dernier bloc
    sep->map_count = line_count - map_start;
    *map = &lines[map_start];
    return (1);
}

int separate_config_and_map(char **lines, int line_count,
                            char ***config, char ***map, t_separation *sep)
{
    int i = 0;
    int params = 0;
    int config_end;

    // 1) Sauter les lignes vides initiales
    if (!check_begin_map_ok(lines, line_count, config, map, sep))
        return (0);
    skip_empty_lines(lines, &i, line_count);

    // 2) Lire les paramètres dans n’importe quel ordre
    config_end = i;
    while (config_end < line_count && params < 6)
    {
        if (is_empty_line(lines[config_end]))
        {
            config_end++;
            continue;
        }
        if (is_valid_parameter_line(lines[config_end]))
        {
            params++;
            config_end++;
        }
        else
            break; // fin des paramètres
    }

    // 3) Vérifier qu'on a les 6 paramètres
    if (params != 6)
        return (0);

    // 4) Sauter les lignes vides entre params et map
    skip_empty_lines(lines, &config_end, line_count);

    // 5) La map doit commencer ici
    if (config_end >= line_count)
        return (0);

    // 6) Remplir la structure
    sep->config_lines = lines;
    sep->config_start = i;
    sep->config_count = config_end - i;
    sep->map_lines = lines;
    sep->map_start = config_end;
    sep->map_count = line_count - config_end;

    return (1);
}


void	free_separation(t_separation *sep)
{
	(void)sep;
}

int	parse_cub_file(const char *filename, t_cub *cub)
{
	char	**lines;
	int		line_count;
	t_separation	sep;

	// Initialisation de la structure cub (IMPORTANT)
	ft_memset(cub, 0, sizeof(t_cub));
	cub->floor_color = -1;    // -1 signifie non défini
	cub->ceiling_color = -1;  // -1 signifie non défini

	ft_memset(&sep, 0, sizeof(t_separation));
	lines = read_cub_file(filename, &line_count);
	if (!lines)
		return (printf("Error\nCould not read file: %s\n", filename), 0);
	
	if (!separate_config_and_map(lines, line_count,
			&sep.config_lines, &sep.map_lines, &sep))
	{
		free_string_array(lines);
		return (printf("Error\nInvalid file format\n"), 0);
	}
	
	// MODIFICATION : Ajout du parsing des couleurs
	if (!validate_configuration(sep.config_lines + sep.config_start,
			sep.config_count, cub))
	{
		free_string_array(lines);
		return (printf("Error\nInvalid configuration\n"), 0);
	}
    if (!validate_configuration_color(sep.config_lines + sep.config_start,
			sep.config_count, cub))
    {
        free_string_array(lines);
		return (printf("Error\nInvalid configuration color\n"), 0);
    }
	
	// AJOUT : Vérification que toutes les couleurs sont définies
	if (cub->floor_color == -1)
	{
		free_string_array(lines);
		return (printf("Error\nMissing floor color (F)\n"), 0);
	}
	
	if (cub->ceiling_color == -1)
	{
		free_string_array(lines);
		return (printf("Error\nMissing ceiling color (C)\n"), 0);
	}
	
	if (!validate_map(sep.map_lines + sep.map_start, sep.map_count, cub))
	{
		free_string_array(lines);
		return (printf("Error\nInvalid map\n"), 0);
	}
	
	free_string_array(lines);
	return (1);
}
