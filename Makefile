PURP	= \e[35m
GREEN	= \e[32m
GREEN2	= \033[32m
RED		= \e[31m
WHITE	= \e[39m
YELLOW	= \033[33m
HIDDEN	= \e[8m
RESET	= \e[0m
BLUE	= \033[34m
CYAN	= \033[36m
MAGENTA = \033[35m

NAME	= ircserv

SRCS	= main.cpp Server.cpp Client.cpp
MAIN	= main.cpp
OBJS	= ${SRCS:.cpp=.o}

LOGO_IRC	= 	 @echo "$(CYAN) \tWelcome to : \n\
 								\tIIIIIIIIIIRRRRRRRRRRRRRRRRR           CCCCCCCCCCCCC\n\
 								\tI::::::::IR::::::::::::::::R       CCC::::::::::::C\n\
								\tI::::::::IR::::::RRRRRR:::::R    CC:::::::::::::::C\n\
								\tII::::::IIRR:::::R     R:::::R  C:::::CCCCCCCC::::C\n\
								\t  I::::I    R::::R     R:::::R C:::::C       CCCCCC\n\
								\t  I::::I    R::::R     R:::::RC:::::C              \n\
								\t  I::::I    R::::RRRRRR:::::R C:::::C              \n\
								\t  I::::I    R:::::::::::::RR  C:::::C              \n\
								\t  I::::I    R::::RRRRRR:::::R C:::::C              \n\
								\t  I::::I    R::::R     R:::::RC:::::C              \n\
								\t  I::::I    R::::R     R:::::RC:::::C              \n\
								\t  I::::I    R::::R     R:::::R C:::::C       CCCCCC\n\
								\tII::::::IIRR:::::R     R:::::R  C:::::CCCCCCCC::::C\n\
								\tI::::::::IR::::::R     R:::::R   CC:::::::::::::::C\n\
								\tI::::::::IR::::::R     R:::::R     CCC::::::::::::C\n\
								\tIIIIIIIIIIRRRRRRRR     RRRRRRR        CCCCCCCCCCCCC$(\RESET)\n\
								\t                                      By The Moires\n\
								\t  \n"

CC			= c++
CPPFLAGS	= -Wall -Wextra -Werror -g3 -std=c++98 -pedantic -fsanitize=address

AR 		= ar rc
RM 		= rm -f

.c.o:		%.o : %.cpp
					@$(CC)${CPPFLAGS}

all: 		logo ${NAME} 

logo:
			$(LOGO_IRC)


${NAME}:			${OBJS}
					@echo "$(CYAN)---- Compiling library ----"
					@echo "$(GREEN2)📚 Link paths"
					@$(CC) $(CPPFLAGS) ${OBJS} -o $(NAME)
					@printf "$(BLUE)🍵 Creating $(NAME)$(RESET)\n"
					@echo "$(CYAN)$(NAME) Compiled ! \033[39m(\033[31m๑\033[39m╹◡╹\033[31m๑\033[39m)"
					@echo ""


clean:
					@${RM} ${OBJS}
					@echo ""
					@echo "$(YELLOW)---- Cleaning library ----"
					@printf "$(BLUE)🧽 Cleaning $(NAME)$(RESET)\n"
					@echo "$(GREEN2)📚 Cleaning paths"
					@echo "$(YELLOW)$(NAME) is all clean ! $(ORANGE)(ﾉ◕ヮ◕)ﾉ$(YELLOW)*:･ﾟ✧"
					@echo ""

fclean: 	clean
					@${RM} $(NAME) $(textGen.replace)
					@printf "\r$(PURP)----- Deleting library ----\n"
					@echo 🗑 "\033[31mEverything is deleting now !"
					@printf "\r$(PURP)$(NAME) have been removed $(WHITE)¯\_$(PURP)(ツ)$(WHITE)_/¯$(RESET)\n"
					@echo ""

re:			fclean all

leak: 		all
					leaks -atExit -- ./$(NAME)

.PHONY: all clean fclean re
