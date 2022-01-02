OBJ = statusbar.c modules.c 
CC = cc
CFLAGS = -lX11 -Os

statusbar: ${OBJ} config.h
	${CC} -o $@ ${OBJ} $(CFLAGS)
