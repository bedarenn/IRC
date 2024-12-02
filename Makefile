##################################### NAME #####################################

NAME := ircserv
NAME_BONUS := bot

################################### COMPILER ###################################

CC := make --no-print-directory -C

################################# DIRECTORIES ##################################

DIR_MANDA = manda/
DIR_BONUS = bonus/

#################################### RULES #####################################

all:
bonus:

$(NAME): $(DIR_MANDA)
	@$(CC) $<
	@cp $<$@ .
$(NAME_BONUS): $(DIR_BONUS)
	@$(CC) $<
	@cp $<$@ .

clean:
	@$(CC) $(DIR_MANDA) clean
	@$(CC) $(DIR_BONUS) clean

fclean:
	@$(CC) $(DIR_MANDA) fclean
	@$(CC) $(DIR_BONUS) fclean
	@rm -f $(NAME)
	@rm -f $(NAME_BONUS)

re: fclean all bonus

run: all
	@flatpak run io.github.Hexchat > /dev/null 2>&1 &
	@./ircserv 8080 pop

all: $(NAME)
bonus: $(NAME_BONUS)

#################################### PHONY #####################################
.PHONY: all debug clean fclean re

#################################### COLORS ####################################
RED = \033[0;31m
GREEN = \033[0;32m
BROWN = \033[1;33m
PURPLE = \033[0;35m
NC = \033[0m
