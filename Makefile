CXX = cc
CFLAGS = -lX11 -std=c99 -pedantic

statusbar: statusbar.c modules.c config.h
	$(CXX) -o statusbar statusbar.c modules.c $(CFLAGS)
