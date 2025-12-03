#include "../../includes/cube3d.h"

static int print_error(const char *msg)
{
    write(2, "Error\n", 6);
    write(2, msg, ft_strlen(msg));
    write(2, "\n", 1);
    return (0);
}

int ft_strcmp(const char *s1, const char *s2)
{
    int i = 0;

    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

static int has_cub_extension(const char *filename)
{
    int len = ft_strlen(filename);
    if (len < 4)
        return (0);
    return (ft_strcmp(filename + (len - 4), ".cub") == 0);
}

// ---------------------------------------------------------
// Fonction principale demandée
// ---------------------------------------------------------
int check_arguments(int argc, char **argv)
{
    int fd;

    // Vérifier le nombre d'arguments
    if (argc != 2)
        return print_error("Invalid arguments");

    // Vérifier extension du fichier
    if (!has_cub_extension(argv[1]))
        return print_error("File is not a .cub");

    // Vérifier si le fichier peut être ouvert
    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        return print_error("Cannot open .cub file");

    close(fd);
    return (1); // OK ✔
}
