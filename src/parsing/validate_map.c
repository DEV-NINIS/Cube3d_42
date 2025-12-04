#include "../../includes/cube3d.h"

static int is_valid_map_char(char c)
{
    return (c == '0' || c == '1' || c == ' ' ||
            c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

static int is_player_char(char c)
{
    return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

/* --------------------------------------------------------- */
/*               FLOOD FILL POUR VERIFIER MAP FERMÉE         */
/* --------------------------------------------------------- */
static int flood_fill(char **map, int h, int w, int y, int x, int **visited)
{
    if (y < 0 || x < 0 || y >= h || x >= w)
        return (0);
    if (map[y][x] == ' ')
        return (0);
    if (visited[y][x])
        return (1);
    visited[y][x] = 1;

    if (map[y][x] == '1')
        return (1);

    return (
        flood_fill(map, h, w, y - 1, x, visited) &&
        flood_fill(map, h, w, y + 1, x, visited) &&
        flood_fill(map, h, w, y, x - 1, visited) &&
        flood_fill(map, h, w, y, x + 1, visited)
    );
}

/* --------------------------------------------------------- */
/*                    NORMALISER LA MAP                       */
/* --------------------------------------------------------- */
static char **normalize_map(char **map_lines, int count, int *outW)
{
    int maxlen = 0;
    for (int i = 0; i < count; i++)
    {
        int len = ft_strlen(map_lines[i]);
        if (len > maxlen)
            maxlen = len;
    }

    char **map = malloc(sizeof(char *) * (count + 1));
    for (int i = 0; i < count; i++)
    {
        int len = ft_strlen(map_lines[i]);
        map[i] = malloc(maxlen + 1);
        ft_memset(map[i], ' ', maxlen);
        ft_memcpy(map[i], map_lines[i], len);
        map[i][maxlen] = '\0';
    }
    map[count] = NULL;
    *outW = maxlen;
    return map;
}

/* --------------------------------------------------------- */
/*                   VALIDATE MAP                            */
/* --------------------------------------------------------- */
int validate_map(char **map_lines, int count, t_cub *cub)
{
    int players = 0;

    /* 1) Check characters + player count */
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; map_lines[i][j]; j++)
        {
            if (!is_valid_map_char(map_lines[i][j]))
                return (printf("return map is -> 0 and map char is %c and index is -> %d -> %d\n", map_lines[i][j], i, j), 0);

            if (is_player_char(map_lines[i][j]))
            {
                players++;
                cub->player_x = j;
                cub->player_y = i;
                cub->player_dir = map_lines[i][j];
            }
        }
    }

    if (players != 1)
        return (printf("return map is -> 1"), 0);

    /* 2) Normaliser */
    cub->map_height = count;
    cub->map = normalize_map(map_lines, count, &cub->map_width);

    /* Remplacer player char par '0' */
    cub->map[cub->player_y][cub->player_x] = '0';

    /* 3) Flood fill */
    int **visited = malloc(sizeof(int *) * cub->map_height);
    for (int i = 0; i < cub->map_height; i++)
    {
        visited[i] = ft_calloc(cub->map_width, sizeof(int));
    }

    int result = flood_fill(
        cub->map,
        cub->map_height,
        cub->map_width,
        cub->player_y,
        cub->player_x,
        visited
    );

    for (int i = 0; i < cub->map_height; i++)
        free(visited[i]);
    free(visited);

    return (result);
}
