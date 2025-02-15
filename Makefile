# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cbordeau <cbordeau@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/15 08:41:31 by cbordeau          #+#    #+#              #
#    Updated: 2025/02/15 12:18:24 by cbordeau         ###   ########.fr        #
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

vtest2: all
	rm -f outfile
	@valgrind --trace-children=yes --track-fds=yes ./pipex infile \
	"sleep 2" "cat" \
	outfile 2>&1 | grep -E "errors from|heap|HEAP|open|Command|blocks" \
	| GREP_COLORS='mt=1;4;32' grep --color=always -E "HEAP SUMMARY|$$" \
	| GREP_COLORS='mt=1;4;31' grep --color=always -E "ERROR SUMMARY|$$" \
	| GREP_COLORS='mt=1;4;33' grep --color=always -E "FILE DESCRIPTORS|$$" \
	| GREP_COLORS='mt=1;4;36' grep --color=always -E "==.....==|$$" \
	| GREP_COLORS='mt=1;5;35' grep --color=always -E "All heap blocks were freed -- no leaks are possible|$$"
	@[ -r outfile ] && cat outfile || echo "outfile is missing or unreadable"
	@echo "exit code :"
	@echo $$?
.PHONY : all clean fclean re
