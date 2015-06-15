#ifndef PROJECT_H
#define PROJECT_H

typedef enum {NONE,TEXT,PAUSE,NEWLINE,BACKSPACE,CLEAR,CURSOR, MOD_TYPE} SegmentType;
typedef enum {MSEC,SEC,FRAME} TimeFormat;

typedef struct _Segment {
  char *buffer;
  int pause;
  TimeFormat pauseformat;
  SegmentType type;
  struct _Segment *next;
} Segment;

typedef struct _Line {
  char *buffer;
  Segment *segments;
  int num;
  struct _Line *next;
} Line;

typedef struct {
  Line *lines; 
} Project;

extern Project *parse_input(char *filename,unsigned char *keymap);

extern void preview_project(Project *project);
extern void output_project(Project *project,char *filename);

#endif

