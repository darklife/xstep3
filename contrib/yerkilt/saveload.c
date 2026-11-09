#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <xstep.h>
#include "style.h"

static FILE *fp, *sfp, *lfp;	
static char filename[256], imagename[256];
extern char data[SIDE], *pixmap[SIDE];
extern int color[16], thread[16], tot_thr;
static struct xtree *lwin, *swin, *iwin;
extern struct xtree *pixwin;
static int i, nc, nb;

void file_exists();
void save_image(struct xtree *t);
void save(struct xtree *t);
void isave(struct xtree *t);
void savef(struct xtree *t);		
void load(struct xtree *t);
void loadf(struct xtree *t);
extern void color_update(int index,int new);

void load(struct xtree *t)
{
  lwin = window_create(0,0,320,240,"Load File");
  label_create(COL_1,ROW_1,200,24,"Please insert filename :",invisible,left);
  edit_create(COL_1,ROW_2,200,21,filename,251,loadf);
  button_create(-8,-8,72,24,"Cancel",window_close);
}

void save(struct xtree *t)
{
  swin = window_create(0,0,320,240,"Save File");
  label_create(COL_1,ROW_1,200,24,"Please insert filename :",invisible,left);
  edit_create(COL_1,ROW_2,200,21,filename,251,savef);
  button_create(-8,-8,72,24,"Cancel",window_close);
}

void savef(struct xtree *t)
{
  char check[256];

  window_close(swin);

  sprintf(check,"%s",filename + strlen(filename) - 4);

  //printf("%s<",filename + strlen(filename) - 4); /* DEBUG */

  if(strcmp(check,".mac") != 0 || strlen(filename) < 4)
    {
      strcat(filename,".mac");
    }

  sfp = fopen(filename, "w");
  
  for(i = 0 ; i < 16 ; i++)
    {
      fprintf(sfp,"%.6x\t%d\n",color[i],thread[i]);
    }
  fclose(sfp);
}
    
void loadf(struct xtree *t)
{

  if((lfp = fopen(filename, "r"))) file_exists();
  else 
    {
      strcat(filename,".mac");
      if((lfp = fopen(filename, "r"))) file_exists();
      else
	{
	  dialog_create(320,240);
	  defaultfont=helvetica24b;
	  label_create(0,0,0,-90,"404 - File Not Found",getnamedcolor("yellow"),up);
	  defaultfont=helvetica12m;
	  button_create(-8,-8,72,24,"Aye!",window_close);
	}
    }
}

void save_image(struct xtree *t)
{
  int i, cols, rows;
  cols = rows = tot_thr;

  window_close(iwin);

  strcat(imagename,".xpm");

  fp = fopen(imagename, "w");
  fprintf(fp,"/* XPM */\nstatic char *tartan[] = {\n/* width height num_colors chars_per_pixel */\n\"%7d%7d%11d               1\",\n",cols,rows,16);
  fprintf(fp,"/* colors */\n");
 
  for ( i = 1 ; i < rows + 17 ; i++)
    {
      fprintf(fp, "\"%s\",\n", pixmap[i]);
    }
  fprintf(fp,"};\nSIMPLE  =                    T\nBITPIX  =                    8\nNAXIS   =                    2\nNAXIS1  =%21d\nNAXIS2  =%21d\nHISTORY Written by YerKilt v0.2\nEND",cols,rows); 
  fclose(fp);

  window_close(pixwin);
}

void file_exists()
{
  window_close(lwin);
  for(i = 0 ; i < 16 ; ++i)
    {
      fscanf(lfp,"%x%d%*c",&nc,&nb);
      color_update(i,nc);
      thread[i] = (nb > 255) ? 255 : nb;
    }
  fclose(lfp);
  broadcast++;
}

void isave(struct xtree *t)
{
  iwin = window_create(0,0,320,240,"Save Image as XPM");
  label_create(COL_1,ROW_1,200,24,"Please insert filename :",invisible,left);
  edit_create(COL_1,ROW_2,200,21,imagename,251,save_image);
  button_create(-8,-8,72,24,"Cancel",window_close);
}











