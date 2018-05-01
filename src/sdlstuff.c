#include <stdlib.h>
#include "sdlstuff.h"

SDL_Surface *screen;

void init_sdl(int w,int h,int bps) {
	if(SDL_Init(SDL_INIT_VIDEO)) {
		printf("Error ! Cannot initialize SDL !\n%s\n",SDL_GetError());
		exit(1);
	}
	screen=SDL_SetVideoMode(w,h,bps,0);
	if(screen==NULL) {
		printf("Unable to set video mode: %s\n", SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);
}

void putpixel(SDL_Surface *surface,int x,int y,Uint32 color) {
  Uint8 *bits,bpp;
  if(x<0 || y<0 || x>=surface->w || y>=surface->h) return;
  bpp = surface->format->BytesPerPixel;
  bits = ((Uint8 *)surface->pixels)+y*surface->pitch+x*bpp;
  switch(bpp) {
    case 1:
     *((Uint8 *)(bits)) = (Uint8)color;
     break;
    case 2:
      *((Uint16 *)(bits)) = (Uint16)color;
      break;
    case 3: {
      Uint8 r, g, b;
      r = (color>>surface->format->Rshift)&0xFF;
      g = (color>>surface->format->Gshift)&0xFF;
      b = (color>>surface->format->Bshift)&0xFF;
      *((bits)+surface->format->Rshift/8) = r;
      *((bits)+surface->format->Gshift/8) = g;
      *((bits)+surface->format->Bshift/8) = b;
      }
      break;
    case 4:
      *((Uint32 *)(bits)) = (Uint32)color;
	}
}

void draw_line(SDL_Surface *screen,int x1,int y1,int x2,int y2,Uint32 
pixel) {
    Uint8 *bits,bpp;
    Uint8 r,g,b;
    int dx,dy;
    int ax,ay;
    int sx,sy;
    int x,y;

    if(x1<0) x1=0; else if(x1>screen->w) x1=screen->w;
    if(x2<0) x2=0; else if(x2>screen->w) x2=screen->w;
    if(y1<0) y1=0; else if(y1>screen->h) y1=screen->h;
    if(y2<0) y2=0; else if(y2>screen->h) y2=screen->h;

    dx=x2-x1;
    dy=y2-y1;
    ax=abs(dx) << 1;
    ay=abs(dy) << 1;
    sx=(dx>=0)?1:-1;
    sy=(dy>=0)?1:-1;
    x=x1;
    y=y1;
    bpp = screen->format->BytesPerPixel;
    if (ax > ay) {
        int d = ay - (ax >> 1);
        while (x != x2) {
                /***  DRAW PIXEL HERE ***/
                bits = ((Uint8 *)screen->pixels)+y*screen->pitch+x*bpp;
                switch(bpp) {
                  case 1:
                        *((Uint8 *)(bits)) = (Uint8)pixel;
                        break;
                  case 2:
                        *((Uint16 *)(bits)) = (Uint16)pixel;
                        break;
                  case 3: { /* Format/endian independent */
                        r = (pixel>>screen->format->Rshift)&0xFF;
                        g = (pixel>>screen->format->Gshift)&0xFF;
                        b = (pixel>>screen->format->Bshift)&0xFF;
                        *((bits)+screen->format->Rshift/8) = r;
                        *((bits)+screen->format->Gshift/8) = g;
                        *((bits)+screen->format->Bshift/8) = b;
                        }
                        break;
                  case 4:
                        *((Uint32 *)(bits)) = (Uint32)pixel;
                        break;
                }
                /*** END DRAWING PIXEL ***/
            if (d > 0 || (d == 0 && sx == 1)) {
                y += sy;
                d -= ax;
            }
            x += sx;
            d += ay;
        }
    } else {
        int d = ax - (ay >> 1);
        while (y != y2) {
                /*** DRAW PIXEL HERE ***/
        bits = ((Uint8 *)screen->pixels)+y*screen->pitch+x*bpp;
                switch(bpp) {
                  case 1:
                        *((Uint8 *)(bits)) = (Uint8)pixel;
                        break;
                  case 2:
                        *((Uint16 *)(bits)) = (Uint16)pixel;
                        break;
                  case 3: { /* Format/endian independent */
                        r = (pixel>>screen->format->Rshift)&0xFF;
                        g = (pixel>>screen->format->Gshift)&0xFF;
                        b = (pixel>>screen->format->Bshift)&0xFF;
                        *((bits)+screen->format->Rshift/8) = r;
                        *((bits)+screen->format->Gshift/8) = g;
                        *((bits)+screen->format->Bshift/8) = b;
                        }
                        break;
                  case 4:
                        *((Uint32 *)(bits)) = (Uint32)pixel;
                        break;
                }

                /*** END DRAWING PIXEL ***/

            if (d > 0 || (d == 0 && sy == 1)) {
                x += sx;
                d -= ay;
            }
            y += sy;
            d += ax;
        }
    }
    /*** DRAW PIXEL HERE ***/
    bits = ((Uint8 *)screen->pixels)+y*screen->pitch+x*bpp;
        switch(bpp) {
        case 1:
                *((Uint8 *)(bits)) = (Uint8)pixel;
                break;
          case 2:
                *((Uint16 *)(bits)) = (Uint16)pixel;
                break;
          case 3: { /* Format/endian independent */
                r = (pixel>>screen->format->Rshift)&0xFF;
                g = (pixel>>screen->format->Gshift)&0xFF;
                b = (pixel>>screen->format->Bshift)&0xFF;
                *((bits)+screen->format->Rshift/8) = r;
                *((bits)+screen->format->Gshift/8) = g;
                *((bits)+screen->format->Bshift/8) = b;
                }
                break;
          case 4:
                *((Uint32 *)(bits)) = (Uint32)pixel;
                break;
        }
    /*** END DRAWING PIXEL HERE ***/
}

void wait_for_enter() {
	SDL_Event event;
	while(1) {
		SDL_WaitEvent(&event);
		if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_RETURN) break;
	  if(event.type==SDL_JOYBUTTONDOWN) break;
	}
}
