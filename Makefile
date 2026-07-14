CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -D_POSIX_C_SOURCE=200112L
TARGET = MyShell

SRC_DIR = src
BUILD_DIR = build

SRCS = $(SRC_DIR)/SH.c \
       $(SRC_DIR)/prompt.c \
       $(SRC_DIR)/builtin.c \
       $(SRC_DIR)/job.c \
       $(SRC_DIR)/exstr.c

OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: clean