CXX = cc
CFLAGS = -lX11

statusbar: statusbar.c 
	$(CXX) -o statusbar statusbar.c $(CFLAGS)
