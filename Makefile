CFLAGS:=-Wall -Wextra -pedantic -std=c99
CPPFLAGS := -MMD -MP
CC:=gcc
TARGET:=mines.out

SRC_DIR:=src
BUILD_DIR:=build

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:%.o=%.d)

LDLIBS=-lncurses

$(TARGET): $(OBJS)
	$(CC) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: clean

-include $(DEPS)
