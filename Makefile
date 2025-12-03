# Compilateur et options
CXX = cc
CXXFLAGS = -Wall -Wextra -O2
LDFLAGS = -L "./src/LIBFT" -lft
NAME_LIBFT = src/LIBFT


# Nom de l'exécutable
TARGET = cube3d

# Fichier source
SRC = src/parsing/check_args.c \
	src/parsing/maintest.c \
	src/parsing/read_cub.c \
	src/parsing/parse_cube.c \
	src/get_next_line/get_next_line_utils.c \
	src/get_next_line/get_next_line.c
	
OBJ = $(SRC:.c=.o)

# Règle par défaut
all: compile_libft $(TARGET)

compile_libft:
	$(MAKE) -C $(NAME_LIBFT)

# Compilation de l'exécutable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compilation des fichiers objets
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyage des fichiers générés
clean:
	$(MAKE) -C $(NAME_LIBFT) clean
	rm -f $(OBJ)

fclean:
	$(MAKE) -C $(NAME_LIBFT) fclean
	rm -f $(OBJ) $(TARGET)

re: fclean all


# Exécution du programme
run: $(TARGET)
	./$(TARGET)

# Exécution avec valgrind (détection de fuites mémoire)
valgrind-run: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)


# Affiche l'aide
help:
	@echo "Commandes disponibles:"
	@echo "  make all           - Compile le programme (défaut)"
	@echo "  make clean         - Nettoie les fichiers générés"
	@echo "  make run           - Compile et exécute"
	@echo "  make valgrind-run  - Exécute avec valgrind"
	@echo "  make help          - Affiche cette aide"

.PHONY: all clean fclean re run valgrind-run help