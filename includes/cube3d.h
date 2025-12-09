#ifndef CUBE_3D_H
#define CUBE_3D_H

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "../src/LIBFT/libft.h"
#include "../src/get_next_line/get_next_line.h"

# define MAX_TEXTURES 4

typedef struct s_separation
{
	char	**config_lines;
	int		config_start;
	int		config_count;
	char	**map_lines;
	int		map_start;
	int		map_count;
}	t_separation;

typedef struct s_cub
{
	char	*texture_paths[MAX_TEXTURES];
	int		floor_color;
	int		ceiling_color;
	char	**map;
	int		map_width;
	int		map_height;
	int		player_x;
	int		player_y;
	char	player_dir;
}	t_cub;

// Check les arguments
int     check_arguments(int argc, char **argv);
/* Fonctions de lecture */
char	**read_cub_file(const char *filename, int *line_count);
void	free_string_array(char **array);

/* Fonctions de parsing */
int		parse_cub_file(const char *filename, t_cub *cub);

int		separate_config_and_map(char **lines, int line_count,
                            char ***map, t_separation *sep);
void	free_separation(t_separation *sep);

/* Fonctions de validation */
int		validate_configuration(char **config_lines, int count, t_cub *cub);
int		validate_map(char **map_lines, int count, t_cub *cub);
int		parse_color_line(t_cub *cub, const char *line, int line_num);
int		parse_texture_line(t_cub *cub, const char *line, int line_num);
int 	check_begin_map_ok(char **lines, int line_count,
                            char ***map, t_separation *sep);
int		is_config_line(const char *line);
int		validate_configuration_color(char **config_lines, int count, t_cub *cub);

int	    is_cub_file(const char *filename);
void	print_cub_info(t_cub *cub);
void    free_cub(t_cub *cub);




// utils
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);
int     ft_strcmp(const char *s1, const char *s2);

#endif

