# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aboncine <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/03 08:35:03 by aboncine          #+#    #+#              #
#    Updated: 2023/01/04 12:19:54 by aboncine         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_bonus
SRCS = main_bonus.c checks_and_utils_bonus.c struct_init_bonus.c utils_bonus.c
OBJS = $(SRCS:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -g
%.o: %.c
	$(CC) -c $(CFLAGS) $?

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re .c.o
