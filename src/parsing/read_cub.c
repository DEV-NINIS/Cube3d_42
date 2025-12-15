/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_cub.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 18:33:20 by ael-fari          #+#    #+#             */
/*   Updated: 2025/12/15 18:33:22 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

int	grow_lines_array(char ***lines, int *capacity)
{
	char	**new;

	new = ft_realloc(*lines, sizeof(char *) * (*capacity), sizeof(char *)
			* (*capacity * 2));
	if (!new)
		return (0);
	*lines = new;
	*capacity *= 2;
	return (1);
}

char	**read_cub_file(const char *filename, int *line_count)
{
	int		fd;
	char	**lines;
	int		capacity;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (NULL);
	capacity = 16;
	*line_count = 0;
	lines = malloc(sizeof(char *) * capacity);
	if (!lines)
		return (close(fd), NULL);
	lines[*line_count] = get_next_line(fd);
	while (lines[*line_count])
	{
		(*line_count)++;
		if (*line_count >= capacity - 1)
			if (!grow_lines_array(&lines, &capacity))
				return (close(fd), NULL);
		lines[*line_count] = get_next_line(fd);
	}
	lines[*line_count] = NULL;
	return (close(fd), lines);
}

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
