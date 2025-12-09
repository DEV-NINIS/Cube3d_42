#include "../../includes/cube3d.h"

char	**read_cub_file(const char *filename, int *line_count)
{
	int		fd;
	char	*line;
	char	**lines;
	int		count;
	int		capacity;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (NULL);
	capacity = 100;
	count = 0;
	lines = malloc(sizeof(char *) * capacity);
	if (!lines)
		return (close(fd), NULL);
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (count >= capacity - 1)
		{
			capacity *= 2;
			lines = ft_realloc(lines, sizeof(char *) * capacity / 2, 
					sizeof(char *) * capacity);
			if (!lines)
				return (close(fd), NULL);
		}
		lines[count++] = line;
		line = get_next_line(fd);
	}
	lines[count] = NULL;
	*line_count = count;
	return (close(fd), lines);
}

/*
** Libère un tableau de strings alloué par read_cub_file
** Param: array - tableau à libérer
*/
void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

/*
** Fonction realloc maison (si non disponible)
** Pour éviter d'utiliser la fonction interdite realloc()
*/
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	char	*src;
	char	*dst;
	size_t	i;

	if (!ptr)
		return (malloc(new_size));
	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	src = (char *)ptr;
	dst = (char *)new_ptr;
	i = 0;
	while (i < old_size && i < new_size)
	{
		dst[i] = src[i];
		i++;
	}
	return (free(ptr), new_ptr);
}

