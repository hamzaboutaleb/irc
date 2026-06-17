CXX := c++
CXXFLAGS := -std=c++98 -Wall -Wextra -Werror -Iinclude

BUILD_DIR := build
OBJS_DIR := $(BUILD_DIR)/objs

NAME := irc

SRCS := $(wildcard src/*.cpp src/**/*.cpp)
OBJS := $(SRCS:src/%.cpp=$(OBJS_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

$(OBJS_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJS_DIR)

fclean: clean
	@rm -rf $(BUILD_DIR)

re: fclean all

.PHONY: all clean fclean re