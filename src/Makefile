NAME = ft_irc
SRC = Server.cpp main.cpp
OBJS = $(SRC:.cpp=.o)
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++98

all: $(NAME)

$(NAME): $(OBJS)
	@c++ $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "COMPILATION SUCCESSFULL"

%.o:%.cpp
	@c++ $(CXXFLAGS) -c -g $< -o $@

clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

run:
	make; ./$(NAME) 4242

.PHONY: all clean fclean re
