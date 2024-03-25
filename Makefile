CC = clang

CFLAGS = -Os -pedantic -Wall -Wextra -Werror -Wno-deprecated-declarations -Wextra -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_POSIX_C_SOURCE=200810L
LIBS = -lX11
LDFLAGS = ${LIBS}

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
HEADER = $(wildcard *.h)

all: statusbar config.h

install:
	mv statusbar /usr/local/bin/statusbar

statusbar: ${OBJ}
	${CC} ${OBJ} ${LDFLAGS} -g -o $@

.c.o:
	${CC} -c ${CFLAGS} -g $<

debug: ${OBJ}
	${CC} -o $@ $^ ${CFLAGS} ${LDFALGS} -fsanitize=address -pg
	valgrind --tool=memcheck --leak-check=full ./debug

clean:
	rm -f statusbar *.o
