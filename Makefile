# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nchennaf <nchennaf@student.42lausanne.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/15 08:45:10 by nchennaf          #+#    #+#              #
#    Updated: 2023/04/15 08:46:05 by nchennaf         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC =		c++
#CC =		g++
#CPPFLAGS =	g++-11 -Werror -Wextra -Wall -std=c++98 -pedantic
CPPFLAGS =	-Wall -Wextra -Werror -g3 -std=c++98 -pedantic #-fsanitize=address
NAME =		ircserv
AR =		ar rcs
SRC =		main.cpp \
			Socket.cpp

HEADER =	Socket.hpp

OBJ =		${SRC:.cpp=.o}

all:		${NAME}

${OBJ}: %.o : %.cpp ${HEADER} Makefile
	@${CC} ${CPPFLAGS} -o $@ -c $<

${NAME}:	${OBJ}
			@${CC} ${CPPFLAGS} ${OBJ} -o ${NAME}
			@echo "compiling	OK"

clean:
			@rm -rf ${OBJ}
			@echo "cleaning	OK"

fclean:		clean
			@rm -rf ${NAME}

re:			fclean all

.PHONY:		all clean fclean re norm
