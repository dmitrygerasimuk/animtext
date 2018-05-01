#ifndef FONT_H
#define FONT_H

#include <SDL/SDL.h>

typedef struct {
  unsigned char glyph[256][8];
} Font;

extern Font *load_font(char *filename);
extern void close_font(Font *font);

extern void drawChar(SDL_Surface *surface, Font *font,unsigned char chr,int x,int y,Uint32 color,int scale);
extern int drawString(SDL_Surface *surface,Font *font, unsigned char *str,int x,int y,Uint32 color,int scale);

#endif

