.POSIX:
OBJ = statusbar.c modules.c
CC = cc
CFLAGS = -Os -pedantic
LIBS = -lX11

all: statusbar config.h

statusbar: statusbar.o modules.o config.h
	${CC} ${CFLAGS} -o $@ ${OBJ} ${LIBS}

statusbar.o: statusbar.c
	${CC} -c ${CFLAGS} statusbar.c

modules.o: modules.c modules.h
	${CC} -c ${CFLAGS} modules.c

debug: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -pg
	valgrind --tool=memcheck --leak-check=full ./debug

clean:
	rm -f statusbar statusbar.o modules.o

