CC = cc
CFLAGS = -Os -pedantic -Wall -Wno-deprecated-declarations -Wextra -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_POSIX_C_SOURCE=200809L
LIBS = -lX11
SRC = $(wildcard *.c)
HEADER = $(wildcard *.h)
OBJ = $(SRC:.c=.o)

all: statusbar config.h

statusbar: ${OBJ}
	${CC} ${OBJ} ${LIBS} -o $@

%.o: %.c $(HEADER)
	${CC} ${CFLAGS} -c $< -o $@

debug: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) -pg
	valgrind --tool=memcheck --leak-check=full ./debug

clean:
	rm -f statusbar *.o

