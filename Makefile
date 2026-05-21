CFLAGS=-Wall -Wextra -Werror -pedantic -std=c99
BUILD_DIR=build
OBJS=$(BUILD_DIR)/main.o $(BUILD_DIR)/board.o $(BUILD_DIR)/display.o $(BUILD_DIR)/state.o
SRC_DIR=src
LDFLAGS=-lncurses
TARGET=minesweepy

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

minesweepy: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

$(BUILD_DIR)/board.o: $(SRC_DIR)/board.c $(SRC_DIR)/board.h
$(BUILD_DIR)/display.o: $(SRC_DIR)/display.c $(SRC_DIR)/display.h
$(BUILD_DIR)/state.o: $(SRC_DIR)/state.c $(SRC_DIR)/state.h

all: $(TARGET)

clean:
	rm -rf ./$(BUILD_DIR)/*
	rm -f ./minesweepy
