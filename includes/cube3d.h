#ifndef CUBE_3D_H
# define CUBE_3D_H

# include "../src/LIBFT/libft.h"
# include "../src/get_next_line/get_next_line.h"
# include "mlx.h"
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>

# define MAX_TEXTURES 4
# define WIN_W 1024
# define WIN_H 768

# define KEY_ESC 65307
# define KEY_W 119
# define KEY_S 115
# define KEY_A 97
# define KEY_D 100
# define KEY_LEFT 65363
# define KEY_RIGHT 65361

# define MOVE_SPEED 0.05
# define ROT_SPEED 0.03
# define PLAYER_RADIUS 0.2

# define TEX_NORTH 0
# define TEX_SOUTH 1
# define TEX_WEST 2
# define TEX_EAST 3

typedef struct s_separation
{
	char		**config_lines;
	int			config_start;
	int			config_count;
	char		**map_lines;
	int			map_start;
	int			map_count;
}				t_separation;

typedef struct s_cub
{
	char		*texture_paths[MAX_TEXTURES];
	int			floor_color;
	int			ceiling_color;
	char		**map;
	int			map_width;
	int			map_height;
	int			player_x;
	int			player_y;
	char		player_dir;
}				t_cub;

// Exec
typedef struct s_image
{
	void		*handle;
	char		*addr;
	int			bpp;
	int			line_len;
	int			endian;
}				t_image;

typedef struct s_texture
{
	void		*handle;
	char		*addr;
	int			width;
	int			height;
	int			bpp;
	int			line_len;
	int			endian;
}				t_texture;

typedef struct s_player
{
	double		x;
	double		y;
	double		dir_x;
	double		dir_y;
	double		plane_x;
	double		plane_y;
}				t_player;

typedef struct s_key_state
{
	int			move_forward;
	int			move_backward;
	int			strafe_left;
	int			strafe_right;
	int			turn_left;
	int			turn_right;
}				t_key_state;

typedef struct s_engine
{
	void		*mlx;
	void		*win;
	t_image		frame;
	t_texture	textures[MAX_TEXTURES];
	t_cub		cub;
	t_player	player;
	t_key_state	keys;
}				t_engine;

typedef struct s_ray
{
	double		camera_x;
	double		dir_x;
	double		dir_y;
	int			map_x;
	int			map_y;
	double		side_dist_x;
	double		side_dist_y;
	double		delta_dist_x;
	double		delta_dist_y;
	double		perp_wall_dist;
	int			step_x;
	int			step_y;
	int			hit_side;
	int			line_height;
	int			draw_start;
	int			draw_end;
	int			tex_id;
	double		wall_x;
	int			tex_x;
	double		tex_step;
	double		tex_pos;
}				t_ray;

typedef struct s_ff
{
	char		**map;
	int			h;
	int			w;
	int			**vis;
}				t_ff;

/* ----------------------------- exec ----------------------------- */

int				engine_init(t_engine *engine, int argc, char **argv);
int				textures_load(t_engine *engine);
void			player_init(t_engine *engine);
void			engine_destroy(t_engine *engine);

int				render_frame(t_engine *engine);

int				on_key_press(int keycode, t_engine *engine);
int				on_key_release(int keycode, t_engine *engine);
void			player_update(t_engine *engine);

int				cub_is_walkable(t_cub *cub, double x, double y);
int				player_can_move_x(t_cub *cub, double new_x, double y,
					double dir_x);
int				player_can_move_y(t_cub *cub, double x, double new_y,
					double dir_y);

void			image_put_pixel(t_image *img, int x, int y, int color);
unsigned int	texture_get_pixel(t_texture *tex, int x, int y);
void			render_draw_background(t_engine *engine);
void			raycast_draw_column(t_engine *engine, int x);
int				raycast_find_wall_hit(t_engine *engine, t_ray *ray);

/* ------------------------------------ . ----------------------------------- */

// Check les arguments
int				check_arguments(int argc, char **argv);
/* Fonctions de lecture */
char			**read_cub_file(const char *filename, int *line_count);
void			free_string_array(char **array);

/* Fonctions de parsing */
int				parse_cub_file(const char *filename, t_cub *cub);

int				separate_config_and_map(char **lines, int line_count,
					char ***map, t_separation *sep);
void			free_separation(t_separation *sep);

/* Fonctions de validation */
int				validate_configuration(char **config_lines, int count,
					t_cub *cub);
int				validate_map(char **map_lines, int count, t_cub *cub);
// int		parse_color_line(t_cub *cub, const char *line, int line_num);
// int		parse_texture_line(t_cub *cub, const char *line, int line_num);
// int 	check_begin_map_ok(char **lines, int line_count,
//                             char ***map, t_separation *sep);
int				is_config_line(const char *line);

int				is_cub_file(const char *filename);
void			print_cub_info(t_cub *cub);
void			free_cub(t_cub *cub);

int				print_error(const char *msg);
int				has_cub_extension(const char *filename);
int				check_color_range(int value, int line_num);
int				validate_color_value(char *str, int *value, int line_num);
int				parse_rgb_values(char **rgb_values, int rgb[3], int line_num);
int				validate_rgb_count(char **rgb_values, int line_num);
int				parse_color_values(const char *line, int *color, int line_num);
int				check_duplicate_color(t_cub *cub, char type, int line_num);
int				process_color_line(t_cub *cub, char *clean, int line_num);
int				parse_color_line(t_cub *cub, const char *line, int line_num);
int				is_empty_line(char *line);
int				is_valid_parameter_line(char *line);
int				is_config_line(const char *line);
int				skip_empty_start(char **lines, int count);
int				read_config(char **lines, int count, int start);
int				find_map_start(char **lines, int count, int i);
int				validate_map_block(char **lines, int count, int start);
int				grow_lines_array(char ***lines, int *capacity);
int				is_tex_id(const char *id);
int				pack_rgb(int r, int g, int b);
int				read_rgb_value(char **p, int *out);
int				read_rgb_comma(char **p);
int				parse_rgb_components(char *s, int *rgb);
char			*extract_id(char *line, char *idbuf);
void			init_cub_config(t_cub *cub);
int				config_complete(int *flags);
int				parse_texture(char *id, char *after, t_cub *cub, int *flags);
int				parse_color(char *id, char *after, t_cub *cub, int *flags);
int				handle_config_line(char *line, t_cub *cub, int *flags);
int				validate_configuration(char **config_lines, int count,
					t_cub *cub);
int				is_valid_map_char(char c);
int				is_player_char(char c);
int				get_max_len(char **lines, int count);
char			**allocate_map(int count);
void			normalize_lines(char **lines, char **map, int count,
					int maxlen);
int				flood_fill(t_ff *ff, int y, int x);
int				check_map_chars_and_player(char **lines, int count, t_cub *cub);
int				normalize_map_and_check(char **lines, int count, t_cub *cub);
int				allocate_visited(t_cub *cub, int ***visited);
int				check_map_closed(t_cub *cub, int **visited);
int				check_line_chars(char *line, int y, t_cub *cub, int *players);

// utils
void			*ft_realloc(void *ptr, size_t old_size, size_t new_size);
int				ft_strcmp(const char *s1, const char *s2);
int				parse_rgb_str(char *s);
char			*skip_ws(char *s);
void			rtrim(char *s);

#endif
