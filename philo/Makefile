# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/10 20:43:18 by bsyvasal          #+#    #+#              #
#    Updated: 2024/03/06 15:44:56 by bsyvasal         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CC = cc
CFLAGS = -Wall -Werror -Wextra
# CFLAGS = -Wall -Werror -Wextra -fsanitize=thread -g
FILES =	philo_main \
		philo_philo \
		philo_single \
		philo_utils \
		philo_table \
		philo_clock \
		philo_exit \
		philo_initialization
		
SRCS = $(addsuffix .c, $(FILES))
OBJS = $(addsuffix .o, $(FILES))

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

clean:
	@rm -f *.o

fclean: clean
	@rm -f $(NAME)

re: clean all

.PHONY: all clean fclean re bonus
