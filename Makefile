CC = cc

CFLAGS = -Os -std=c99 -pedantic -Wall -Wextra -Werror -Wno-deprecated-declarations -Wextra -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_POSIX_C_SOURCE=200810L
LIBS = -lX11
LDFLAGS = ${LIBS}

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
HEADER = $(wildcard *.h)

all: statusbar

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.h

statusbar: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

install:
	mv statusbar /usr/local/bin/statusbar

debug: ${OBJ}
	${CC} -o $@ $^ ${CFLAGS} ${LDFALGS} -fsanitize=address -pg
	valgrind --tool=memcheck --leak-check=full ./debug

clean:
	rm -f statusbar ${OBJ}

.PHONY: all clean install debug
