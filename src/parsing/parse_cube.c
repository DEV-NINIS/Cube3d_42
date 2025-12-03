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

int separate_config_and_map(char **lines, int line_count,
                            char ***config, char ***map, t_separation *sep)
{
    int i = 0;
    int params = 0;
    int config_end;

    // 1) Sauter les lignes vides initiales
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
	if (!validate_configuration(sep.config_lines + sep.config_start,
			sep.config_count, cub))
	{
		free_string_array(lines);
		return (printf("Error\nInvalid configuration\n"), 0);
	}
	if (!validate_map(sep.map_lines + sep.map_start, sep.map_count, cub))
	{
		free_string_array(lines);
		return (printf("Error\nInvalid map\n"), 0);
	}
	free_string_array(lines);
	return (1);
}
