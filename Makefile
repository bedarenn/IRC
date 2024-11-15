##################################### NAME #####################################

NAME := ircserv

################################### COMPILER ###################################

CC := c++

################################# DIRECTORIES ##################################

DIR_SRCS := srcs/
DIR_HDRS := hdrs/
DIR_OBJS := objs/
DIR_LIBS := libs/

#################################### FLAGS #####################################

CFLAGS := -Wall -Wextra #-std=c++98 #-Werror
IFLAGS := -I$(DIR_HDRS)
LFLAGS := -L$(DIR_LIBS)

#################################### FILES #####################################

include sources.mk

################################### OBJECTS ####################################

OBJS = $(addprefix $(DIR_OBJS), $(SRCS:%.cpp=%.o))

#################################### RULES #####################################

all:
debug:

$(NAME): $(OBJS)
	@printf "$(GREEN)compile $@                                         $(NC)\n"
	@$(CC) $^ $(LFLAGS) -o $@
	@flatpak run io.github.Hexchat > /dev/null 2>&1 &
	@./ircserv 8080 pop

$(DIR_OBJS)%.o: $(DIR_SRCS)%.cpp
	@mkdir -p $(@D)
	@printf "$(BROWN)compile $(notdir $<)                              $(NC) \r"
	@$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

clean:
	@printf "$(RED)clean objs$(NC)\n"
	@rm -rf $(DIR_OBJS)

fclean: clean
	@printf "$(RED)clean $(NAME)$(NC)\n"
	@rm -f $(NAME)

re: fclean all

all:	$(NAME)
debug:	CFLAGS += -g
debug:	$(NAME)

#################################### PHONY #####################################
.PHONY: all debug clean fclean re

#################################### COLORS ####################################
RED = \033[0;31m
GREEN = \033[0;32m
BROWN = \033[1;33m
PURPLE = \033[0;35m
NC = \033[0m
