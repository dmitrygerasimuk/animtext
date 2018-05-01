#ifndef VIDEO_H
#define VIDEO_H

#include "project.h"

extern void set_FPS(int FPS,int drymode);
extern void init_video(int w,int h,int xoff,int yoff,int scale);

extern int get_FPS(void);
extern int get_msec_per_frame(void);
extern int get_current_frame(void);

extern void vid_putstring(char *string);
extern void vid_putchar(char chr);
extern void vid_clearscreen(void);
extern void vid_setcursor(int interval,TimeFormat format);

extern void save_frames(char *filename,int count);

#endif
