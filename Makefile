CFLAGS=-Wall -Wextra -Werror -pedantic -std=c99
OBJS=main.o board.o display.o
VPATH=src
LDFLAGS=-lncurses

mine: $(OBJS)
	cc -o mine $(OBJS) $(LDFLAGS)

board.o: board.c board.h
display.o: display.c display.h

clean:
	rm -f ./*.o
