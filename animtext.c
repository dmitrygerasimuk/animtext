#include <stdio.h>
#include <string.h>

#include "sdlstuff.h"
#include "font.h"
#include "project.h"

static void print_help(void);
static int parse_keymap(char *string,unsigned char *map);

int main(int argc,char *argv[]) {
  Project *project;
  unsigned char keymap[256];
  int use_keymap=0;
  char *input=NULL,*output=NULL;
  char preview=0;
  int drymode=0;
  int r,xoff=0,yoff=0,scale=1,width=720,height=576;
  memset(keymap,0,256);
  /* Parse arguments */
  for(r=1;r<argc;r++) {
    if(strcmp(argv[r],"--help")==0) {print_help(); return 0;}
    else if(strcmp(argv[r],"-i")==0||strcmp(argv[r],"--input")==0) {
      if(r+1>=argc) {
        printf("Please specify the input file !\n");
	return 1;
      }
      input=argv[r+1]; r++;
    } else if(strcmp(argv[r],"-o")==0||strcmp(argv[r],"--output")==0) {
      if(r+1>=argc) {
        printf("Please specify the output file !\n");
	return 1;
      }
      output=argv[r+1]; r++;
    } else if(strcmp(argv[r],"-p")==0||strcmp(argv[r],"--preview")==0) preview=1;
    else if(strcmp(argv[r],"-w")==0||strcmp(argv[r],"--width")==0) {
      if(r+1>=argc) {
        printf("Please specify the width !\n");
	return 1;
      }
      width=atoi(argv[r+1]); r++;
    } else if(strcmp(argv[r],"-h")==0||strcmp(argv[r],"--height")==0) {
      if(r+1>=argc) {
        printf("Please specify the height !\n");
	return 1;
      }
      height=atoi(argv[r+1]); r++;
    } else if(strcmp(argv[r],"-x")==0||strcmp(argv[r],"--xoffset")==0) {
      if(r+1>=argc) {
        printf("Please specify the X offset !\n");
	return 1;
      }
      xoff=atoi(argv[r+1]); r++;
    } else if(strcmp(argv[r],"-y")==0||strcmp(argv[r],"--yoffset")==0) {
      if(r+1>=argc) {
        printf("Please specify the Y offset !\n");
	return 1;
      }
      yoff=atoi(argv[r+1]); r++;
    } else if(strcmp(argv[r],"-s")==0||strcmp(argv[r],"--scale")==0) {
      if(r+1>=argc) {
        printf("Please specify the font scale !\n");
	return 1;
      }
      scale=atoi(argv[r+1]); r++;
    } else if(strcmp(argv[r],"--dry")==0) {
      drymode=1;
    } else if(strcmp(argv[r],"--keymap")==0) {
      if(r+1>=argc) {
        printf("Please specify the keymap !\n");
	printf("Keymap format is <src>,<targ>,<src>,<targ>...\n");
	return 1;
      }
      r++;
      use_keymap=parse_keymap(argv[r],keymap);
    } else printf("Unknown option \"%s\"\n",argv[r]);
  }
  if(!input) {
    printf("No input file specified. Using stdin.\n");
    input="stdin";
  }
  if(!output) {
    char bltin_output[256];
    strcpy(bltin_output,input);
    strcat(bltin_output,"%.4d.tga");
    output=bltin_output;
    printf("No output file specified. Using %s.\n",output);
  }
  /* Initialize */
  set_FPS(25,drymode);
  /* Handle data */
  project=parse_input(input,use_keymap?keymap:NULL);
  if(preview) {
    preview_project(project);
  } else {
    init_video(width,height,xoff,yoff,scale);
    output_project(project,output);
  }
  return 0;
}

static void print_help(void) {
  printf("animtext\nUsage:\n");
  printf("\t--help\t\t\t\tShow this message\n");
  printf("\t-i,--input <file>\t\tInput file\n");
  printf("\t-o,--output <file%%.3d.tga>\tOutput files\n");
  printf("\t-f,--frames <frames>\t\tOutput only a subset of frames\n");
  printf("\t-p,--preview\t\t\tPreview on the console\n");
  printf("\t-w,--width <width>\t\tWidth\n");
  printf("\t-h,--height <height\t\tHeight\n");
  printf("\t-x,--xoffset\t\t\tX offset\n");
  printf("\t-y,--yoffset\t\t\tY offset\n");
  printf("\t-s,--scale <scale>\t\tFont scale\n");
  printf("\t--dry\t\t\t\tDon't output files\n");
  printf("\t--keymap <map>\t\t\tCharacter translation table\n");
  printf("\n\n");
  printf("A word about character translation.\n");
  printf("The character translation table is meant for fixing\n");
  printf("locale problems. Syntax is <source>,<dest>,<source>,<dest>,...\n");
  printf("Source and destination characters in hexadesimal format.\n");
}

static int parse_keymap(char *string,unsigned char *map) {
  char *ptr;
  int value,pos=0;
  while(*string) {
    pos++;
    value=strtol(string,&ptr,16);
    if(*ptr!=',') {
      printf("Warning: keymap parse error at value %d !\n",pos);
      return 0;
    }
    string=ptr+1;
    if(value<0||value>255) {
      printf("Warning: keymap value %d (%d) is out of bounds.\n",value,pos);
      return 0;
    }
    map[value]=strtol(string,&ptr,16);
    string=ptr;
    if(*string==',') string++;
  }
  return 1;
}

