OBJ = statusbar.c modules.c 
CC = cc
CFLAGS = -lX11 -std=c99 -pedantic -Wall

statusbar: ${OBJ}
	${CC} -o $@ ${OBJ} $(CFLAGS)
