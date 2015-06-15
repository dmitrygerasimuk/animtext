#ifndef SDLSTUFF_H
#define SDLSTUFF_H

#include <SDL/SDL.h>

extern SDL_Surface *screen;

extern void init_sdl(int w,int h,int bps);

extern void putpixel(SDL_Surface *surface, int x,int y,Uint32 pixel);
extern void draw_line(SDL_Surface *surface,int x1,int y1,int x2,int y2,Uint32 pixel);

extern void wait_for_enter();
#endif
