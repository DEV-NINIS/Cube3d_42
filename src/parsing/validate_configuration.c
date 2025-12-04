#include "../../includes/cube3d.h"

static char *skip_ws(char *s)
{
    while (s && *s && (unsigned char)*s <= ' ')
        s++;
    return s;
}

/* trim trailing spaces in place */
static void rtrim(char *s)
{
    int i = (int)ft_strlen(s) - 1;
    while (i >= 0 && (unsigned char)s[i] <= ' ')
        s[i--] = '\0';
}

/* compare identifier (NO,SO,WE,EA,F,C) - case sensitive */
static int is_tex_id(const char *id)
{
    return (!ft_strcmp(id, "NO") || !ft_strcmp(id, "SO")
         || !ft_strcmp(id, "WE") || !ft_strcmp(id, "EA"));
}

/* pack rgb into int */
static int pack_rgb(int r, int g, int b)
{
    return ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF);
}

/* parse rgb from a string like "  220 , 100 , 0  " */
/* returns -1 on error */
static int parse_rgb_str(char *s)
{
    long v;
    char *p = s;
    int comps[3];
    for (int i = 0; i < 3; ++i)
    {
        p = skip_ws(p);
        if (!*p) return -1;
        /* parse integer */
        v = ft_atoi(p);
        if (v < 0 || v > 255) return -1;
        comps[i] = (int)v;
        /* advance p past digits we just consumed */
        while (*p && isdigit((unsigned char)*p)) p++;
        p = skip_ws(p);
        if (i < 2)
        {
            if (*p != ',') return -1;
            p++; /* skip comma */
        }
    }
    /* after third component allow only spaces */
    p = skip_ws(p);
    if (*p != '\0') return -1;
    return pack_rgb(comps[0], comps[1], comps[2]);
}

/* extract next token (identifier) from line: copy to idbuf (size >=3). returns pointer after token */
static char *extract_id(char *line, char *idbuf)
{
    char *p = skip_ws(line);
    int i = 0;
    while (*p && !isspace((unsigned char)*p) && i < 2)
        idbuf[i++] = *p++;
    idbuf[i] = '\0';
    return p;
}

/* --------------------------------------------------------- */
/* validate_configuration implementation                    */
/* --------------------------------------------------------- */
int validate_configuration(char **config_lines, int count, t_cub *cub)
{
    int have_no = 0, have_so = 0, have_we = 0, have_ea = 0;
    int have_f = 0, have_c = 0;

    /* init cub fields to safe defaults */
    for (int i = 0; i < MAX_TEXTURES; ++i)
        cub->texture_paths[i] = NULL;
    cub->floor_color = -1;
    cub->ceiling_color = -1;

    for (int i = 0; i < count; ++i)
    {
        char *line = config_lines[i];
        if (!line) continue;
        char *p = skip_ws(line);
        if (*p == '\0') continue; /* skip blank lines (shouldn't appear here but safe) */

        /* extract identifier */
        char id[3] = {'\0','\0','\0'};
        char *after = extract_id(p, id);
        after = skip_ws(after);
        if (id[0] == '\0') return (printf("return is -> 0\n"), 0); /* invalid line */

        /* TEXTURE LINES: NO, SO, WE, EA */
        if (is_tex_id(id))
        {
            /* rest of line is the path (no extra tokens allowed) */
            if (*after == '\0') return (printf("return is -> 1\n"), 0); /* path missing */
            /* copy rest and trim trailing spaces */
            char *path = ft_strdup(after);
            if (!path) return (printf("return is -> 1\n"), 0);
            rtrim(path);
            if (path[0] == '\0') { free(path); return (printf("return is -> 2\n"), 0); }

            /* forbid spaces inside path (simpler and safer) */
            for (char *q = path; *q; ++q)
            {
                if ((unsigned char)*q <= ' ') { free(path); return (printf("return is -> 3\n"), 0); }
            }

            /* check extension .xpm */
            int L = (int)ft_strlen(path);
            if (L < 4 || ft_strcmp(path + (L - 4), ".xpm") != 0)
            {
                free(path);
                return (printf("return is -> 4\n"), 0);
            }
            /* check file readable */
            if (access(path, R_OK) != 0)
            {
                printf("path is -> %s\n", path);
                /* fichier manquant ou non lisible -> considérer comme erreur de configuration */
                free(path);
                return (printf("return is -> 5\n"), 0);
            }

            if (!ft_strcmp(id, "NO"))
            {
                if (cub->texture_paths[0]) { free(path); return (printf("return is -> 6\n"), 0); }
                cub->texture_paths[0] = path; have_no = 1;
            }
            else if (!ft_strcmp(id, "SO"))
            {
                if (cub->texture_paths[1]) { free(path); return (printf("return is -> 7\n"), 0); }
                cub->texture_paths[1] = path; have_so = 1;
            }
            else if (!ft_strcmp(id, "WE"))
            {
                if (cub->texture_paths[2]) { free(path); return (printf("return is -> 8\n"), 0); }
                cub->texture_paths[2] = path; have_we = 1;
            }
            else /* EA */
            {
                if (cub->texture_paths[3]) { free(path); return (printf("return is -> 9\n"), 0); }
                cub->texture_paths[3] = path; have_ea = 1;
            }

            if (have_no && have_so && have_we && have_ea && have_f && have_c)
                return 1;
        }
        /* FLOOR / CEILING: F / C */
        else if (!ft_strcmp(id, "F") || !ft_strcmp(id, "C"))
        {
            if (*after == '\0') return (printf("return is -> 0\n"), 0); /* missing rgb */
            char *rgbstr = ft_strdup(after);
            if (!rgbstr) return (printf("return is -> 10\n"), 0);
            rtrim(rgbstr);
            int color = parse_rgb_str(rgbstr);
            free(rgbstr);
            if (color < 0) return (printf("return is -> 11\n"), 0);

            if (!ft_strcmp(id, "F"))
            {
                if (have_f) return (printf("return is -> 12\n"), 0);
                cub->floor_color = color;
                have_f = 1;
            }
            else
            {
                if (have_c) return (printf("return is -> 13\n"), 0);
                cub->ceiling_color = color;
                have_c = 1;
            }
            if (have_no && have_so && have_we && have_ea && have_f && have_c)
                return 1;
        }
        else
        {
            /* unknown identifier -> invalid configuration */
            return (printf("return is -> 14\n"), 0);
        }
    }

    /* must have all six */
    if (!(have_no && have_so && have_we && have_ea && have_f && have_c))
        return (printf("return is -> 0\n"), 0);

    return 1;
}

