#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "project.h"

static int set_segment(int line,char *buffer,Segment *seg) {
  int r=0,i,len;
  char tmp[16];
  len=strlen(buffer);
  while(buffer[r]!='\\') {
    r++;
    if(r>=len) {
      printf("Error: Unterminated segment !\n");
      return 0;
    }
  }
  if(buffer[0]=='%' && buffer[1]!='%') {
    buffer++;
    switch(*buffer) {
      case 'p':
      case 't':
      case 'C':
	if(*buffer=='p')
          seg->type=PAUSE;
	else if(*buffer=='C')
	  seg->type=CURSOR;
	else
	  seg->type=MOD_TYPE;
	i=0; buffer++;
	while(*buffer=='-'||isdigit(*buffer)) {tmp[i]=*buffer; i++; buffer++;}
	tmp[i]='\0';
	seg->pause=atoi(tmp);
	switch(*buffer) {
	  case 's': seg->pauseformat=SEC; break;
	  case 'm': seg->pauseformat=MSEC; break;
	  case 'f': seg->pauseformat=FRAME; break;
	  default: printf("[%d] Warning: Unknown timeformat '%c' !\n",line,*buffer); break;
	}
	break;
      case 'n':
	seg->type=NEWLINE;
	break;
      case 'b':
	seg->type=BACKSPACE;
	break;
      case 'q':
    seg->type=BACKSLASH;
    break;
      case 'c':
	seg->type=CLEAR;
	break;
      default:
	seg->type=NONE;
	printf("[%d] Warning: Unknown segment type '%c'\n",line,*buffer);
	break;
    }
  } else {  
    seg->type=TEXT;
    seg->buffer=(char*)malloc(r+1);
    strncpy(seg->buffer,buffer,r);
    seg->buffer[r]='\0';
  }
  return r+1;
}

static Segment *parse_line(int line,char *buffer) {
  int r;
  Segment *first=NULL,*seg=NULL;
  for(r=0;r<strlen(buffer);r++) {
    if(buffer[r]=='\\') {
      if(buffer[r+1]=='\\') { r++; continue; }
      if(!seg) {
        seg=(Segment*)malloc(sizeof(Segment));
        memset(seg,0,sizeof(Segment));
	first=seg;
      } else {
	seg->next=(Segment*)malloc(sizeof(Segment));
	seg=seg->next;
        memset(seg,0,sizeof(Segment));
      }
      r+=set_segment(line,buffer+r+1,seg);
    }
  }
  return first;
}

Project *parse_input(char *filename,unsigned char *keymap) {
  FILE *fp;
  char str[512];
  Project *project;
  Line *line;
  int ln=0;
  /* Open file */
  if(strcmp(filename,"stdin")==0)
    fp=stdin;
  else
    fp=fopen(filename,"r");
  if(!fp) {
    printf("Couldn't open \"%s\" for reading !\n",filename);
    return NULL;
  }
  /* Initialize structures */
  project=(Project*)malloc(sizeof(Project));
  line=(Line*)malloc(sizeof(Line));
  memset(line,0,sizeof(Line));
  project->lines=line;
  /* Read the entire file into memory */
  while(fgets(str,sizeof(str)-1,fp)) {
    ln++;
    if(strlen(str)==0 || str[0]=='#') continue;
    line->buffer=(char*)malloc(strlen(str)+1);
    line->num=ln;
    if(keymap) {
      int p;
      for(p=0;p<strlen(str);p++) {
        if(keymap[(unsigned char)str[p]]) line->buffer[p]=keymap[(unsigned char)str[p]];
	else line->buffer[p]=str[p];
      }
    } else {
      strcpy(line->buffer,str);
    }
    line->next=(Line*)malloc(sizeof(Line));
    memset(line->next,0,sizeof(Line));
    line=line->next;
  }
  if(fp!=stdin) fclose(fp);
  /* Parse data */
  line=project->lines;
  while(line->next) {
    line->segments=parse_line(line->num,line->buffer);
    line=line->next;
  }
  free(line->next);
  line->next=NULL;
  return project;
}

