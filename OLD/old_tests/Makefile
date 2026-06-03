NAME := ircserv

CXX := c++

CXXFLAGS := -Wall -Wextra -Werror -g #-std=c++98

SRC := main.cpp Commands.cpp

OBJ := $(SRC:.cpp=.o)

RM := rm -f

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re