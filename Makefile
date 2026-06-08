NAME = ircserv
CXX = c++
CXX_FLAGS = -g -Wall -Wextra -Werror -std=c++98

# Directories
SRC_DIR = src
OBJ_DIR = obj

# SRC_Functions
SRC_FILES = User.cpp main.cpp Server.cpp Commands.cpp Channel.cpp
SRC_FUNC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))

SRC = $(SRC_FUNC) 
OBJ = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.cpp=.o)))

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXX_FLAGS) -o $(NAME) $(OBJ)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXX_FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re
