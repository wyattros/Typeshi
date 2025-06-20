CC = gcc
CFLAGS = -Wall -Wextra -I./src/utils #-fsanitize=address 
LDFLAGS = -lSDL2 -lSDL2_image

SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = obj
UTILS_DIR = $(SRC_DIR)/utils

SRC = $(SRC_DIR)/main.c \
      $(wildcard $(UTILS_DIR)/*.c)

OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

TARGET = $(BUILD_DIR)/Typesh!

.PHONY: clean all

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)
