/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 19:34:44 by ael-fari          #+#    #+#             */
/*   Updated: 2025/12/15 19:34:45 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cube3d.h"

int	print_error(const char *msg)
{
	write(2, "Error\n", 6);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	return (0);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	has_cub_extension(const char *filename)
{
	int	len;

	len = ft_strlen(filename);
	if (len < 4)
		return (0);
	return (ft_strcmp(filename + (len - 4), ".cub") == 0);
}

int	check_arguments(int argc, char **argv)
{
	int	fd;

	if (argc != 2)
		return (print_error("Invalid arguments"));
	if (!has_cub_extension(argv[1]))
		return (print_error("File is not a .cub"));
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		return (print_error("Cannot open .cub file"));
	close(fd);
	return (1);
}

int	is_cub_file(const char *filename)
{
	size_t	len;

	if (!filename)
		return (0);
	len = ft_strlen(filename);
	if (len < 5)
		return (0);
	if (ft_strcmp(filename + (len - 4), ".cub") == 0)
		return (1);
	return (0);
}
