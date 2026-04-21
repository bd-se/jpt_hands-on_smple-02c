CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
LDFLAGS = -lsqlite3
SRCS = $(wildcard src/*.c)
TARGET = bin/todo

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET) src/*.o todo.db

.PHONY: all clean
