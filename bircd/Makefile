##################################### NAME #####################################

NAME := bircd
RM = rm -f

################################### COMPILER ###################################

CC := gcc

################################# DIRECTORIES ##################################

DIR_HDRS := hdrs/
DIR_SRCS := srcs/
DIR_OBJS := objs/
DIR_LIBS := libs/

#################################### FLAGS #####################################

CFLAGS := -Wall -Werror -g3
IFLAGS := -I$(DIR_HDRS)
LFLAGS := -L$(DIR_LIBS)

#################################### FILES #####################################

SRCS =	main.c init_env.c clean_fd.c get_opt.c x.c main_loop.c \
	init_fd.c do_select.c check_fd.c \
	srv_create.c srv_accept.c \
	client_read.c client_write.c

################################### OBJECTS ####################################

OBJS = $(addprefix $(DIR_OBJS), $(SRCS:%.c=%.o))

#################################### RULES #####################################

all: ${NAME}

$(NAME): $(OBJS)
	$(CC) $^ $(LFLAGS) -o $@

$(DIR_OBJS)%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

clean:
	${RM} -r ${DIR_OBJS} *~ #*#

fclean: clean
	${RM} ${NAME}

re: fclean all

#################################### PHONY #####################################
.PHONY: all debug clean fclean re
