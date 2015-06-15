OBJS=sdlstuff.o font.o project.o output.o video.o savetga.c animtext.o
CFLAGS=-g

all: $(OBJS) animtext

clean:
	rm -f *.o animtext

animtext: $(OBJS)
	gcc -o animtext $(OBJS) -lSDL -lpthread

