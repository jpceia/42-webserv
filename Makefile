# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/04 23:37:29 by jceia             #+#    #+#              #
#    Updated: 2022/03/25 22:44:25 by jpceia           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = webserv

INC_DIR     = headers
SRC_DIR     = src
OBJ_DIR     = obj

SRCS        = $(shell find $(SRC_DIR) -name "*.cpp" -type f)
OBJS        = $(SRCS:$(SRC_DIR)/%=$(OBJ_DIR)/%.o)

CXX			= clang++
RM          = rm -f

OS          = $(shell uname)

FLAGS_WARN  = -Wall -Werror -Wextra -pedantic-errors
FLAGS_INC   = -I$(INC_DIR)
FLAGS_OPT   = -O3
FLAGS_DEP   = -MD -MF $(OBJ_DIR)/$(NAME).d

ifeq ($(OS), Linux)
	FLAGS_DEBUG = -ggdb3 -DDEBUG -D_GLIBCXX_DEBUG -fsanitize=address
else
	FLAGS_DEBUG = -g -DDEBUG -fsanitize=address 
endif

CXXFLAGS    = $(FLAGS_WARN) $(FLAGS_INC) $(FLAGS_DEP) $(FLAGS_OPT) -std=c++98

# Compilation
$(OBJ_DIR)/%.o:    $(SRC_DIR)/%
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Linking
$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

all: $(NAME)

# Cleaning
clean:
	$(RM) -rf $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

# Rebuild
re: fclean all

# Debug build
debug:      CXXFLAGS = $(FLAGS_WARN) $(FLAGS_INC)  $(FLAGS_DEP) $(FLAGS_DEBUG) -std=c++98
debug:      all


.PHONY:     all clean fclean re debug
