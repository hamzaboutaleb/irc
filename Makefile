CXX := c++
CXXFLAGS := -std=c++98 -Wall -Wextra -Werror -Iinclude

BUILD_DIR := build
OBJS_DIR := $(BUILD_DIR)/objs
BIN_DIR := $(BUILD_DIR)/bin

NAME := irc

SRCS := $(wildcard srcs/*.cpp srcs/**/*.cpp)
OBJS := $(SRCS:srcs/%.cpp=$(OBJS_DIR)/%.o)
TARGET := $(BIN_DIR)/$(NAME)

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

$(OBJS_DIR)/%.o: srcs/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJS_DIR)

fclean: clean
	@rm -rf $(BUILD_DIR)

re: fclean all

.PHONY: all clean fclean re