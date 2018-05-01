#include "sdlstuff.h"
#include "font.h"
#include "video.h"

static int FPS;
static int mspf;
static int cursorX,cursorY;
static int frame;
static Font *font;
static int font_scale,xoffset,yoffset;
static char *scroll_buffer;
static int buffer_len;
static int dry=0;
static int cursor=0;
static int cursor_blink=1;

#define CURSOR_CHR 219

void set_FPS(int fps,int drymode) {
  FPS=fps;
  mspf=1000/fps;
  if(drymode) dry=mspf;
}

void init_video(int w,int h,int xoff,int yoff,int scale) {
  SDL_Color palette[2];
  init_sdl(w,h,8);
  SDL_WM_SetCaption("AnimText",NULL);
  buffer_len=(h-8*scale)*screen->pitch;
  scroll_buffer=(char*)malloc(buffer_len);
  if(!scroll_buffer) {
    printf("Error ! Couldn't initialize scroll buffer !\n");
  }
  font=load_font("font.fot");
  palette[0].r=0;
  palette[0].g=0;
  palette[0].b=0;
  palette[1].r=200;
  palette[1].g=200;
  palette[1].b=200;
  SDL_SetPalette(screen, SDL_LOGPAL|SDL_PHYSPAL, palette, 0, 2);
  cursorX=0;
  cursorY=0;
  frame=0;
  font_scale=scale;
  xoffset=xoff;
  yoffset=yoff;
}

int get_FPS(void) {return FPS;}
int get_msec_per_frame(void) {return mspf;}
int get_current_frame(void) {return frame;}

static void vid_drawcursor(char update) {
  if(cursor==0) return;
  if(cursor<0) cursor_blink=1;
  else if((frame%cursor)==0) cursor_blink=!cursor_blink;
  drawChar(screen,font,cursor_blink?CURSOR_CHR:' ',cursorX+xoffset,cursorY+yoffset,1,font_scale);
  if(update) SDL_UpdateRect(screen,cursorX+xoffset,cursorY+yoffset,8*font_scale,8*font_scale);
}

void vid_clearscreen(void) {
  cursorX=0;
  cursorY=0;
  SDL_FillRect(screen,NULL,0);
  SDL_Flip(screen);
}

void vid_putstring(char *string) {
  int w,h;
  w=strlen(string)*8*font_scale;
  h=drawString(screen,font,string,cursorX+xoffset,cursorY+yoffset,1,font_scale);
  SDL_UpdateRect(screen,cursorX+xoffset,cursorY+yoffset,w+((cursor>0)?8*font_scale:0),h+8*font_scale);
  cursorX+=w;
  cursorY+=h;
  vid_drawcursor(1);
}

void vid_putchar(char chr) {
  if(chr=='\n') {
    if(cursor) {
      drawChar(screen,font,' ',cursorX+xoffset,cursorY+yoffset,1,font_scale);
      SDL_UpdateRect(screen,cursorX+xoffset,cursorY+yoffset,8*font_scale,8*font_scale);
    }
    cursorY+=8*font_scale;
    cursorX=0;
    int screenoffset=screen->h-50;
    if(cursorY>=screenoffset) {
      SDL_Rect rect;
      rect.x=0; rect.y=8*font_scale; rect.w=screen->w; rect.h=screen->h-rect.y;
      memcpy(scroll_buffer,screen->pixels+(8*font_scale)*screen->pitch,buffer_len);
      memcpy(screen->pixels,scroll_buffer,buffer_len);
      rect.h=8*font_scale;
      rect.y=screen->h-rect.h;
      cursorY=screenoffset-rect.h;
      SDL_FillRect(screen,&rect,0);
      SDL_Flip(screen);
    }
    vid_drawcursor(1);
    return;
  } else if(chr=='\b') {
    if(cursor) {
      drawChar(screen,font,' ',cursorX+xoffset,cursorY+yoffset,1,font_scale);
      SDL_UpdateRect(screen,cursorX+xoffset,cursorY+yoffset,8*font_scale,8*font_scale);
    }
    cursorX-=8*font_scale;
    if(cursorX<xoffset) cursorX=xoffset;
    vid_drawcursor(1);
    return;
  }
  drawChar(screen,font,chr,cursorX+xoffset,cursorY+yoffset,1,font_scale);
  SDL_UpdateRect(screen,cursorX+xoffset,cursorY+yoffset,8*font_scale,8*font_scale);
  cursorX+=8*font_scale;
  vid_drawcursor(0);
  SDL_UpdateRect(screen,cursorX+xoffset,cursorY+yoffset,8*font_scale*((cursor>0)+1),8*font_scale);
}

void vid_setcursor(int interval,TimeFormat format)  {
  switch(format) {
    case MSEC: cursor=(interval/1000.0)*FPS; break;
    case SEC: cursor=interval*FPS; break;
    case FRAME: cursor=interval;
  }
}

void save_frames(char *filename,int count) {
  int f;
  char outfile[256];
  if(dry) {
    for(f=0;f<count;f++) {
      frame++;
      vid_drawcursor(1);
      usleep(dry*1000);
    }
  } else {
    for(f=0;f<count;f++) {
      sprintf(outfile,filename,frame);
      frame++;
      vid_drawcursor(1);
      if(SaveTGA(screen,outfile,1)) {
        printf("SaveTGA failed ! (filename: %s)\n",outfile);
      } else printf("Saved %s\n",outfile);
    }
  }
}

