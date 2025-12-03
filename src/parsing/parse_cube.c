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

static int	count_parameters(char **lines, int line_count)
{
	int	i;
	int	count;
	char	*trimmed;

	i = 0;
	count = 0;
	while (i < line_count && count < 6)
	{
		if (!is_empty_line(lines[i]))
		{
			trimmed = ft_strtrim(lines[i], " \t\n\r\v\f");
			if (!trimmed)
				return (-1);
			if (ft_strncmp(trimmed, "NO ", 3) == 0
				|| ft_strncmp(trimmed, "SO ", 3) == 0
				|| ft_strncmp(trimmed, "WE ", 3) == 0
				|| ft_strncmp(trimmed, "EA ", 3) == 0
				|| ft_strncmp(trimmed, "F ", 2) == 0
				|| ft_strncmp(trimmed, "C ", 2) == 0)
				count++;
			free(trimmed);
		}
		i++;
	}
	return (count);
}

static int	is_valid_parameter_line(char *line)
{
	char	*trimmed;
	int		result;

	trimmed = ft_strtrim(line, " \t\n\r\v\f");
	if (!trimmed)
		return (0);
	result = 0;
	if (ft_strncmp(trimmed, "NO ", 3) == 0
		|| ft_strncmp(trimmed, "SO ", 3) == 0
		|| ft_strncmp(trimmed, "WE ", 3) == 0
		|| ft_strncmp(trimmed, "EA ", 3) == 0
		|| ft_strncmp(trimmed, "F ", 2) == 0
		|| ft_strncmp(trimmed, "C ", 2) == 0)
		result = 1;
	free(trimmed);
	return (result);
}

int	separate_config_and_map(char **lines, int line_count,
							char ***config, char ***map, t_separation *sep)
{
	int	i;
	int	config_end;

	i = 0;
	skip_empty_lines(lines, &i, line_count);
	if (i >= line_count)
		return (0);
	config_end = i;
	while (config_end < line_count && count_parameters(lines, config_end + 1) < 6)
	{
		if (!is_empty_line(lines[config_end])
			&& !is_valid_parameter_line(lines[config_end]))
			return (0);
		config_end++;
	}
	if (count_parameters(lines, config_end) != 6)
		return (0);
	skip_empty_lines(lines, &config_end, line_count);
	if (config_end >= line_count || is_empty_line(lines[config_end]))
		return (0);
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
	// if (!validate_configuration(sep.config_lines + sep.config_start,
	// 		sep.config_count, cub))
	// {
	// 	free_string_array(lines);
	// 	return (printf("Error\nInvalid configuration\n"), 0);
	// }
	// if (!validate_map(sep.map_lines + sep.map_start, sep.map_count, cub))
	// {
	// 	free_string_array(lines);
	// 	return (printf("Error\nInvalid map\n"), 0);
	// }
	free_string_array(lines);
	return (1);
}
