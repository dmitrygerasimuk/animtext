#include "font.h"

Font *load_font(char *filename) {
  Font *font;
  FILE *fp;
  int i;
  fp=fopen(filename,"rb");
  if(!fp) {
    fprintf(stderr,"Error ! Couldn't open font file \"%s\" for reading !\n",filename);
    return NULL;
  }
  font=(Font*)malloc(sizeof(Font));
  for(i=0;i<256;i++) {
    if(fread(font->glyph[i],1,8,fp)!=8) {
      fprintf(stderr,"Error occured while reading font file !\n");
      free(font);
      fclose(fp);
      return NULL;
    }
  }
  fclose(fp);
  return font;
}
extern void close_font(Font *font) {
  free(font);
}

extern void drawChar(SDL_Surface *surface,Font *font, unsigned char chr,int x,int y,Uint32 color,int scale) {
  unsigned char i,j,temp,mask;
  int bpp = surface->format->BytesPerPixel;
  Uint32 col;
  int w,h;
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
  if(SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);
  for(i=0;i<8;i++) {
    for(h=0;h<scale;h++) {
      temp = font->glyph[chr][i];
      mask=128;
      for(j=0;j<8;j++) {
        if(temp & mask) col=color; else col=0;
	for(w=0;w<scale;w++,p+=bpp)
        switch(bpp) {
 	  case 1:
	    *p=col;
	    break;
	  case 2:
	    *(Uint16*)p=col;
	    break;
	  case 3:
	    if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
              p[0] = (col >> 16) & 0xff;
              p[1] = (col >> 8) & 0xff;
              p[2] = col & 0xff;
            } else {
              p[0] = col & 0xff;
              p[1] = (col >> 8) & 0xff;
              p[2] = (col >> 16) & 0xff;
            }
	    break;
          case 4:
            *(Uint32 *)p = col;
            break;
	}
	p-=bpp*scale;
        p+=(bpp*scale);
        mask >>= 1;
      }
      p+=surface->pitch-(bpp*8*scale);
    }
  }
  if(SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);
}

extern int drawString(SDL_Surface *surface,Font *font, unsigned char *str,int x,int y,Uint32 color,int scale) {
  int i,dx=0,dy=0;
  for(i=0;i<strlen(str);i++) {
    if(str[i]=='\n') {
      dy+=8*scale;
      dx=0;
    } else {
      drawChar(surface,font,str[i],x+dx,y+dy,color,scale);
      dx+=7*scale;
      if(dx>=surface->w) {
        dx=0;
	dy+=8*scale;
      }
    }
  }
  return dy;
}

