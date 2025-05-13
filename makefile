CC = gcc
CFLAGS = -Wall -Wextra -g
LIBS = -lncurses

all: forca

forca: main.c forca.c hint.c forca.h hint.h
	$(CC) $(CFLAGS) main.c forca.c hint.c -o forca $(LIBS)

clean:
	rm -f forca
