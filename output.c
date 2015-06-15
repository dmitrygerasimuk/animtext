#include <SDL/SDL.h>
#include <stdio.h>
#include "project.h"
#include "sdlstuff.h"
#include "video.h"

static void preview_line(Line *line) {
  Segment *seg=line->segments;
  TimeFormat mod_format;
  int mod_pause;
  SegmentType modifier=NONE;
  while(seg) {
    switch(seg->type) {
      case TEXT:
	if(modifier==NONE) {
	  fputs(seg->buffer,stdout);
	  fflush(stdout);
        } else {
	  int i,j,c;
	  if(mod_pause<0) c=-mod_pause; else c=1;
	  for(i=0;i<strlen(seg->buffer);) {
	    for(j=0;j<c;j++,i++)
  	      putchar(seg->buffer[i]);
	    fflush(stdout);
	    if(mod_pause<0) usleep(get_msec_per_frame()*1000);
	    else
	      switch(mod_format) {
	        case MSEC: usleep(mod_pause*1000); break;
   	        case SEC: sleep(mod_pause); break;
	        case FRAME: usleep((mod_pause*get_msec_per_frame())*1000); break;
	      }
	  }
	  modifier=NONE;
	}
	break;
      case PAUSE: 
        switch(seg->pauseformat) {
	  case MSEC: usleep(seg->pause*1000); break;
	  case SEC: sleep(seg->pause); break;
	  case FRAME: usleep((seg->pause*get_msec_per_frame())*1000); break;
	}
	break;
      case NEWLINE: putchar('\n'); break;
      case BACKSPACE: putchar('\b'); break;
      case CURSOR: break;
      default:
        if(seg->type>=MOD_TYPE) {
	  modifier=seg->type;
	  mod_pause=seg->pause;
	  mod_format=seg->pauseformat;
	}
        break;
    }
    seg=seg->next;
  }
}

static int output_line(Line *line,char *filename) {
  SDL_Event Event;
  Segment *seg=line->segments;
  TimeFormat mod_format;
  int mod_pause,frames;
  SegmentType modifier=NONE;
  while(seg) {
    /* Check out the events */
    while(SDL_PollEvent(&Event)) { 
      switch(Event.type){
        case SDL_KEYUP:
	  if(Event.key.keysym.sym==SDLK_ESCAPE) return 1;
        break;
        default:break;
      }
    }
    switch(seg->type) {
      case TEXT:
	if(modifier==NONE) {
	  vid_putstring(seg->buffer);
	  save_frames(filename,1);
        } else {
	  int i,j,c;
	  switch(mod_format) {
	    case MSEC: frames=(mod_pause/1000.0)*get_FPS(); break;
  	    case SEC: frames=mod_pause*get_FPS(); break;
	    case FRAME: frames=mod_pause; break;
	  }
	  if(mod_pause<0) {
	    frames=1;
	    c=-mod_pause;
	  } else c=1;
	  for(i=0;i<strlen(seg->buffer);) {
	    for(j=0;j<c;j++,i++)
	      vid_putchar(seg->buffer[i]);
	    save_frames(filename,frames);
	  }
	  modifier=NONE;
	}
	break;
      case PAUSE: 
        switch(seg->pauseformat) {
	  case MSEC: frames=(seg->pause/1000.0)*get_FPS(); break;
  	  case SEC: frames=seg->pause*get_FPS(); break;
	  case FRAME: frames=seg->pause; break;
	}
	save_frames(filename,frames);
	break;
      case NEWLINE: vid_putchar('\n'); break;
      case BACKSPACE: vid_putchar('\b'); break;
      case CLEAR: vid_clearscreen(); break;
      case CURSOR: vid_setcursor(seg->pause,seg->pauseformat);
      default:
        if(seg->type>=MOD_TYPE) {
	  modifier=seg->type;
	  mod_pause=seg->pause;
	  mod_format=seg->pauseformat;
	}
        break;
    }
    seg=seg->next;
  }
  return 0;
}

void preview_project(Project *project) {
  Line *line=project->lines;
  while(line) {
    preview_line(line);
    line=line->next;
  }
}

void output_project(Project *project,char *filename) {
  Line *line=project->lines;
  while(line) {
    if(output_line(line,filename)) {
      printf("Interrupted...\n");
      printf("Rendered %d frames. (%.1f seconds with %d frames per second)\n",
	get_current_frame()-1,(float)(get_current_frame()-1)/get_FPS(),get_FPS());
      return;
    }
    line=line->next;
  }
  printf("Completed !\n");
  printf("Rendered %d frames. (%.1f seconds with %d frames per second)\n",
	get_current_frame()-1,(float)(get_current_frame()-1)/get_FPS(),get_FPS());
  wait_for_enter();
}

