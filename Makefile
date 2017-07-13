# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jcamhi <jcamhi@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/09/06 15:15:02 by jcamhi            #+#    #+#              #
#    Updated: 2017/07/12 20:31:48 by jcamhi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_NAME_NM = nm.c

SRC_NAME_OT = otool.c

OBJ_PATH_NM = ./objnm/
OBJ_PATH_OT = ./objot/

INC_PATH = ./includes ./libsrcs/libbinary/includes

SRC_PATH_NM = ./srcs/nm/
SRC_PATH_OT = ./srcs/otool/

NAME_NM = nm
NAME_OT = otool


CC = gcc

CFLAGS = -Werror -Wextra -Wall -g

LFLAGS = -lft -lftprintf -lbinary

LIB_DIR=./lib

OBJ_NAME = $(SRC_NAME:.c=.o)

SRC_NM = $(addprefix $(SRC_PATH_NM),$(SRC_NAME_NM))
OBJ_NM = $(addprefix $(OBJ_PATH_NM),$(OBJ_NAME_NM))
OBJ_NAME_NM = $(SRC_NAME_NM:.c=.o)

SRC_OT = $(addprefix $(SRC_PATH_OT),$(SRC_NAME_OT))
OBJ_OT = $(addprefix $(OBJ_PATH_OT),$(OBJ_NAME_OT))
OBJ_NAME_OT = $(SRC_NAME_OT:.c=.o)

INC = $(addprefix -I,$(INC_PATH))


all : $(NAME_NM) $(NAME_OT) $(SYM_NAME)

$(NAME_NM) : $(OBJ_NM)
	@mkdir -p $(LIB_DIR)
	@make -C libsrcs/libft 2> /dev/null
	@make -C libsrcs/ft_printf 2> /dev/null
	@make -C libsrcs/libbinary 2> /dev/null
	$(CC) $(CFLAGS) $^ -L $(LIB_DIR) $(LFLAGS) -o $@

$(NAME_OT) : $(OBJ_OT)
	@mkdir -p $(LIB_DIR)
	@make -C libsrcs/libft 2>&1 > /dev/null
	@make -C libsrcs/ft_printf 2>&1 > /dev/null
	@make -C libsrcs/libbinary 2>&1 > /dev/null
	$(CC) $(CFLAGS) $^ -L $(LIB_DIR) $(LFLAGS) -o $@

$(OBJ_PATH_NM)%.o: $(SRC_PATH_NM)%.c
	@mkdir -p $(OBJ_PATH_NM)
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

$(OBJ_PATH_OT)%.o: $(SRC_PATH_OT)%.c
	@mkdir -p $(OBJ_PATH_OT)
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

clean:
	rm -fv $(OBJ_NM)
	rm -fv $(OBJ_OT)
	rmdir -p $(OBJ_PATH_NM) 2> /dev/null || true
	rmdir -p $(OBJ_PATH_OT) 2> /dev/null || true
	make -C libsrcs/libft clean
	make -C libsrcs/ft_printf clean
	make -C libsrcs/libbinary clean

fclean: clean
	rm -fv $(NAME_NM)
	rm -fv $(NAME_OT)
	make -C libsrcs/libft fclean
	make -C libsrcs/ft_printf fclean
	make -C libsrcs/libbinary	 fclean
	@rmdir lib 2> /dev/null || true

re: fclean all

leaks: all
	 valgrind --leak-check=full ./$(NAME_TEST)

leaks-le-pen: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME_TEST)

.PHONY : all clean fclean re