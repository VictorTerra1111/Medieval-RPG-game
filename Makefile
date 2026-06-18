TARGET = Corpsia

SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

CC = gcc
# -I$(INC_DIR) for Raylib
CFLAGS = -Wall -Wextra -O2 -I$(INC_DIR)

LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

SRCS = $(wildcard $(SRC_DIR)/*.c)

OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Compile
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

# Compiling
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Execution
run: $(TARGET)
	./$(TARGET)

# Cleaning
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all run clean
