# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/26 20:42:03 by lpolizzi          #+#    #+#              #
#    Updated: 2024/12/26 20:42:04 by lpolizzi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# VARIABLES
NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -O3 -g3

# SOURCES
SRCS = main.c utils.c #simulation.c init.c error_handling.c
# OBJECTS
OBJS = $(SRCS:.c=.o)

# RULES
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
