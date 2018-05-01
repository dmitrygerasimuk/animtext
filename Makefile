CC=gcc
OBJS=sdlstuff.o font.o project.o output.o video.o savetga.c animtext.o
CFLAGS=-I/opt/local/include -g
LDFLAGS=-L/opt/local/lib
all: $(OBJS) animtext

clean:
	rm -f *.o animtext

animtext: $(OBJS)
	gcc  -o animtext $(OBJS) -I/opt/local/include -L/opt/local/lib `sdl-config --cflags --libs` -lpthread

