.POSIX:
OBJ = statusbar.c modules.c
CC = cc
CFLAGS = -Os -pedantic -Wall -fsanitize=address
LIBS = -lX11

all: statusbar config.h

statusbar: statusbar.o modules.o config.h
	${CC} ${CFLAGS} -o $@ ${OBJ} ${LIBS}

statusbar.o: statusbar.c
	${CC} -c ${CFLAGS} $^

modules.o: modules.c modules.h
	${CC} -c ${CFLAGS} $^

debug: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) -pg
	valgrind --tool=memcheck --leak-check=full ./debug

clean:
	rm -f statusbar statusbar.o modules.o

