# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bsyvasal <bsyvasal@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/10 20:43:18 by bsyvasal          #+#    #+#              #
#    Updated: 2024/01/10 20:46:35 by bsyvasal         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philosophers
CC = cc
CFLAGS = -Wall -Werror -Wextra
SRC = 	philosophers \
		utils
		
SRCS = $(addsuffix .c, $(SRC))
OBJS = $(addsuffix .o, $(SRC))

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

clean:
	@rm -f *.o

fclean: clean
	@rm -f $(NAME)

re: clean all

.PHONY: all clean fclean re bonus