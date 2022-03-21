# ----------------- #
#     VARIABLES     #
# ----------------- #

NAME	=	ircserv

SRC_LST	=	main.cpp\
			Server.cpp\
			Client.cpp\
			Channel.cpp\
			Message.cpp\
			PING.cpp\
			CAP.cpp\
			NICK.cpp\
			PART.cpp\
			NOTICE.cpp\
			PASS.cpp\
			QUIT.cpp\
			MODE.cpp\
			USER.cpp\
			PRIVMSG.cpp\
			TOPIC.cpp\
			KICK.cpp\
			JOIN.cpp\
			LIST.cpp\
			CmdHandler.cpp\
			ACommand.cpp\
			utils.cpp
			
SRC_DIR	=	$(shell find srcs -type d)
SRCS	=	$(foreach dir, $(SRC_DIR), $(SRC_LST))

INC_DIR	=	inc/

OBJ_DIR	=	objs/
OBJS	=	$(addprefix $(OBJ_DIR), $(SRC_LST:%.cpp=%.o))

vpath %.cpp $(foreach dir, $(SRC_DIR)/, $(dir):)

# ----------------- #
#    COMPILATION    #
# ----------------- #

CC		=	clang++

FLAGS	=	-g -std=c++98 -D DEBUG -Wall -Wextra -Werror

# ----------------- #
#     FUNCTIONS     #
# ----------------- #

$(OBJ_DIR)%.o: %.cpp
				@mkdir -p $(OBJ_DIR)
				$(CC) -I $(INC_DIR) $(FLAGS) -o $@ -c $<

$(NAME):	$(OBJS)
			$(CC) -I $(INC_DIR) $(FLAGS) $(OBJS) -o $(NAME)

all:		$(NAME)

# ----------------- #
#       CLEAN       #
# ----------------- #

RM		=	rm -rf

clean:	
			$(RM) $(OBJ_DIR)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re