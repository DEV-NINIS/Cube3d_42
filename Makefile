NAME        = cub3D

CC          = cc
CFLAGS      = -Wall -Wextra -Werror

# Dossiers
SRC_DIR     = src
PARSING_DIR = $(SRC_DIR)/parsing
EXEC_DIR    = $(SRC_DIR)/exec
GNL_DIR     = $(SRC_DIR)/get_next_line
LIBFT_PATH  = $(SRC_DIR)/LIBFT
INCLUDE_DIR = includes
MLX_PATH    = $(INCLUDE_DIR)/mlx

# Libs
LIBFT       = $(LIBFT_PATH)/libft.a
MLX         = $(MLX_PATH)/libmlx.a

# Includes
CFLAGS     += -I$(INCLUDE_DIR) -I$(MLX_PATH) -I$(LIBFT_PATH)

# Liens
LDFLAGS     = -L$(LIBFT_PATH) -lft \
              -L$(MLX_PATH) -lmlx \
              -lXext -lX11 -lm -lbsd

# Sources C
SRC_PARSING = $(filter-out $(PARSING_DIR)/maintest.c $(PARSING_DIR)/parse_colors.c, \
              $(wildcard $(PARSING_DIR)/*.c))
SRC_EXEC    = $(wildcard $(EXEC_DIR)/*.c)
SRC_GNL     = $(wildcard $(GNL_DIR)/*.c)

SRC         = $(SRC_PARSING) $(SRC_EXEC) $(SRC_GNL) $(SRC_DIR)/main.c

OBJDIR      = obj
OBJ         = $(SRC:%.c=$(OBJDIR)/%.o)

# Norm
SRC_PATH    = $(SRC_DIR)

# Couleurs
GREEN       = \033[0;32m
YELLOW      = \033[0;33m
WHITE_BOLD  = \033[1;37m
NC          = \033[0m

all: $(NAME)
	@printf "$(WHITE_BOLD)[OK] Compilation $(NAME) done$(NC)\n"

$(NAME): $(LIBFT) $(MLX) $(OBJ)
	@printf "$(GREEN)[OK] Creating executable $(NAME)$(NC)\n"
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS) -g

$(LIBFT):
	@printf "[LIBFT]\n"
	@$(MAKE) -C $(LIBFT_PATH)

# --- MiniLibX : clone + compile si besoin ---------------------------------- #

$(MLX):
	@set -e; \
	mkdir -p "$(INCLUDE_DIR)"; \
	if [ ! -d "$(MLX_PATH)" ]; then \
		printf "[MINILIBX]\n"; \
		printf "⌛ Installing minilibx...\n"; \
		git clone -q https://github.com/42Paris/minilibx-linux.git "$(MLX_PATH)"; \
		printf "  └⚙️  Compiling minilibx...\n"; \
		$(MAKE) -C "$(MLX_PATH)" > /dev/null 2>&1; \
	elif [ ! -f "$(MLX)" ]; then \
		printf "[MINILIBX]\n"; \
		printf "⚙️  Compiling minilibx...\n"; \
		$(MAKE) -C "$(MLX_PATH)" > /dev/null 2>&1; \
	fi

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@ -g

clean:
	@printf "$(YELLOW)Cleaning object files$(NC)\n"
	@rm -rf $(OBJDIR)
	@$(MAKE) -C $(LIBFT_PATH) clean || true

fclean: clean
	@printf "$(YELLOW)Removing executable $(NAME)$(NC)\n"
	@rm -f $(NAME)

re: fclean all

norm:
	@printf "📋 Checking norm...\n"
	@norminette $(INCLUDE_DIR)/cube3d.h
	@norminette $(SRC_PATH)
	@printf "✅ Check completed!\n"

MAP ?= test.cub

valgrind: $(NAME)
	@printf "$(YELLOW)[INFO] Running Valgrind on $(NAME) with $(MAP)$(NC)\n"
	@valgrind \
		--leak-check=full \
		--show-leak-kinds=all \
		--track-origins=yes \
		--track-fds=yes \
		./$(NAME) $(MAP)

.PHONY: all clean fclean re norm valgrind