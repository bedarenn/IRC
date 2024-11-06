# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: matde-ol <matde-ol@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/02 13:24:32 by bedarenn          #+#    #+#              #
#    Updated: 2024/07/28 12:14:00 by matde-ol         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

##################################### NAME #####################################

NAME := ircserv

################################### COMPILER ###################################

CC := c++

################################# DIRECTORIES ##################################

DIR_HDRS := hdrs/
DIR_SRCS := srcs/
DIR_OBJS := objs/
DIR_LIBS := libs/

DIR := \
	$(DIR_SRCS) \
	$(DIR_HDRS)

#################################### FLAGS #####################################

CFLAGS := -Wall -Wextra -Werror
LFLAGS := -L$(DIR_LIBS)
IFLAGS := -I$(DIR_HDRS)

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
