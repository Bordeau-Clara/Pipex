# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cbordeau <cbordeau@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/15 08:41:31 by cbordeau          #+#    #+#              #
#    Updated: 2025/01/17 15:23:31 by cbordeau         ###   LAUSANNE.ch        #
#                                                                              #
# **************************************************************************** #

CC = cc
FLAGS = -Wall -Wextra -Werror -g3
NAME = pipex
#NAME_BONUS = checker
HEADER = pipex.h
SRC = pipex.c

#SRC_BONUS = 

OBJ = $(SRC:.c=.o)
#OBJ_BONUS = $(SRC_BONUS:.c=.o)

all : libft $(NAME)

libft : 
	make -C Libft

$(NAME) : $(OBJ)
	$(CC)  $(FLAGS) -o $(NAME) $(OBJ) -L. Libft/libft.a

#bonus : libft $(NAME_BONUS)

#$(NAME_BONUS) : $(OBJ_BONUS)
#	$(CC) $(FLAGS) -o $(NAME_BONUS) $(OBJ_BONUS) -L. Libft/libft.a

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

.PHONY : all clean fclean re
