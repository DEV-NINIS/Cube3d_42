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
    first_word = ft_substr(trimmed, 0, 2);
    if (!first_word)
        return (free(trimmed), 0);
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
    return (free(trimmed), free(first_word), result);
}

int check_begin_map_ok(char **lines, int line_count,
                            char ***map, t_separation *sep)
{
    int map_start = -1;
    int in_map = 0;
    int empty_line_in_map = 0;
    // Trouver où commence la map (après tous les paramètres)
    for (int i = 0; i < line_count; i++) {
        char *trimmed = ft_strtrim(lines[i], " \t");
        if (!trimmed || trimmed[0] == '\0')
        {
            free(trimmed);
            if (in_map)
                empty_line_in_map = 1;
            continue;
        }
        if (is_config_line(trimmed))
        {
            free(trimmed);
            if (in_map)
                return (printf("Error\nConfiguration after map start\n"), 0);
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
    if (map_start == -1 || empty_line_in_map)
        return (printf("Error\nmap invalid\n"), 0);
    // Vérifier que la map est bien le dernier bloc
    sep->map_count = line_count - map_start;
    *map = &lines[map_start];
    return (1);
}

int separate_config_and_map(char **lines, int line_count,
                            char ***map, t_separation *sep)
{
    int i = 0;
    int params = 0;
    int config_end;

    if (!check_begin_map_ok(lines, line_count, map, sep))
        return (0);
    skip_empty_lines(lines, &i, line_count);
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
            break;
    }
    if (params != 6)
        return (0);
    skip_empty_lines(lines, &config_end, line_count);
    if (config_end >= line_count)
        return (0);
    sep->config_lines = lines;
    sep->config_start = i;
    sep->config_count = config_end - i;
    sep->map_lines = lines;
    sep->map_start = config_end;
    sep->map_count = line_count - config_end;
    return (1);
}

int	parse_cub_file(const char *filename, t_cub *cub)
{
	char	**lines;
	int		line_count;
	t_separation	sep;

    if (!is_cub_file(filename))
            return (printf("Error\nInvalid file \n"), 0);
	ft_memset(cub, 0, sizeof(t_cub));
	cub->floor_color = -1;
	cub->ceiling_color = -1;
	ft_memset(&sep, 0, sizeof(t_separation));
	lines = read_cub_file(filename, &line_count);
	if (!lines || !separate_config_and_map(lines, line_count,
			&sep.map_lines, &sep))
		return (free_string_array(lines), printf("Error\nInvalid file format or cannot read it\n"), 0);
	if (!validate_configuration(sep.config_lines + sep.config_start,
			sep.config_count, cub)
        || !validate_configuration_color(sep.config_lines + sep.config_start,
			sep.config_count, cub))
		return (free_string_array(lines), printf("Error\nInvalid configuration\n"), 0);
	if (cub->floor_color == -1 || cub->ceiling_color == -1)
		return (printf("Color Error\n"), 0);
	if (!validate_map(sep.map_lines + sep.map_start, sep.map_count, cub))
		return (free_string_array(lines), printf("Error\nInvalid map\n"), 0);
	return (free_string_array(lines), print_cub_info(cub), 0);
}
