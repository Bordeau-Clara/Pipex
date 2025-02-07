# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cbordeau <cbordeau@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/15 08:41:31 by cbordeau          #+#    #+#              #
#    Updated: 2025/02/07 09:18:09 by cbordeau         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
FLAGS = -Wall -Wextra -Werror -g3
NAME = pipex
HEADER = pipex.h
SRC = pipex.c utils.c get_next_line.c get_next_line_utils.c

OBJ = $(SRC:.c=.o)

all : $(NAME)

$(NAME) : $(OBJ)
	make -C Libft
	$(CC)  $(FLAGS) -o $(NAME) $(OBJ) -L. Libft/libft.a


%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

clean :
	rm -f $(OBJ)
	#rm -f $(OBJ_BONUS)
	make clean -C Libft

fclean : clean
	rm -f $(NAME)
	#rm -f $(NAME_BONUS)
	make fclean -C Libft

re : fclean all
	make re -C Libft
	rm -f outfile

test: all 
	rm -f outfile
	cat > infile
	./pipex infile \
	ls cat \
	outfile
	cat outfile
	echo $$?

vtest: all 
	rm -f outfile
	cat > infile
	valgrind --trace-children=yes --track-fds=yes ./pipex infile \
	ls cat \
	outfile 2>&1 | grep -E "errors from|heap|HEAP|open|blocks"
	cat outfile
	echo $$?

.PHONY : all clean fclean re
