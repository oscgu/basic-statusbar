CC = clang
CFLAGS = -Os -pedantic -Wall -Wno-deprecated-declarations -Wextra -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_POSIX_C_SOURCE=200810L
LIBS = -lX11
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
HEADER = $(wildcard *.h)

all: statusbar config.h

install:
	mv statusbar /usr/local/bin/statusbar

statusbar: ${OBJ}
	${CC} ${OBJ} ${LIBS} -o $@

%.o: %.c $(HEADER)
	${CC} ${CFLAGS} -c $< -o $@

debug: ${OBJ}
	${CC} -o $@ $^ ${CFLAGS} ${LIBS} -pg
	valgrind --tool=memcheck --leak-check=full ./debug

clean:
	rm -f statusbar *.o
