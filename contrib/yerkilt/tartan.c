#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <xstep.h>
#include "style.h"

int 		color[16], thread[16];
int             tot_thr, tot_col, rows, cols, i, j, k,
                min=0, max=255;

struct xtree *b[16], *pixwin;
	
unsigned char 	buf[16][256], text[128],
  id[16][2]={".", "+", "-", "=", "#", "@", "*", ",", ";", ":", 
	     "_", "^", "$", "?", ">", "<"};

char *pixmap[SIDE];
static char sym_flag[64];
static char data[SIDE] = "\0";

extern void save(struct xtree *t);
extern void isave(struct xtree *t);
extern void load(struct xtree *t);
extern void col_c_up(struct xtree *t);
extern void col_change_menu(struct xtree *t);
void randomize(struct xtree *t);
void color_update(int index, int new);

int hexcolor(int hex)
{
  XColor  color;

  color.red   = (hex & 0xff0000) / 0x10000;
  color.green = (hex & 0xff00  ) / 0x100;
  color.blue  = (hex & 0xff);
  XAllocColor(display,DefaultColormap(display,screen),&color);
  return(color.pixel);
}

int sum_lenght(int a)
{
  int i, i_abs, b = 0;
  for (i = sym_flag[0] ? -15 : 0 ; i < a ; i++)
    {
      i_abs = abs(i);
      b += thread[i_abs];
    }
  return b;
}


unsigned long int rancol()
{
	unsigned long int a;
	
	a = rand();
	a = a % 0xffffff;
	return a;	
}

void randomize(struct xtree *t)
{
  int i;

  for(i = 0 ; i < 16 ; i++)
    {
      color_update(i,(int) rancol());
    }
  broadcast++;
}

void color_update(int index,int new) 
{
  //static char a[]="#00";
  color[index] = new;
 
  //sprintf(a,"#%2d", index+1);
    
	// change the client side 

	b[index]->c=new;

	//  change the server side 
	XSetWindowBackground(display,
			     b[index]->win,
			     new);
	// clear the changed box (the X server
	// must readraw with the new color)
	XClearArea(display,
		 b[index]->win,
		   0,0,
		   b[index]->w,
		   b[index]->h,
		   True);

	//XSetForeground(display,b[index]->gc,black);
	//XDrawString(display,b[index]->win,b[index]->gc,4,18,a,strlen(a));

}


void engine()
{ 
  char *p = data;
  char temp[SIDE] = "\0";
  int i_abs;

  for (i = sym_flag[0] ? -15 : 0 ; i < 16 ; i++)
    {
     
      i_abs = abs(i);
       
      for(j = 0 ; j < thread[i_abs] ; j++)
	{
	  *p=id[i_abs][0];
	  p++;
	}
    }

  cols = rows = tot_thr;

  for(i = 0 ; i < tot_thr ; i++)
    {
      pixmap[i+17] = calloc(tot_thr + 1, sizeof(char));
    }
   
  sprintf(temp,"%7d%7d%11d         1",cols,rows,16);
  pixmap[0] = calloc( strlen(temp) + 1, sizeof(char));
  strcat(pixmap[0],temp);

  for ( i = 0 ; i < 16 ; i++)
    {
      sprintf(temp,"%c c #%.6x", id[i][0], color[i]);
      pixmap[i+1] = calloc(strlen(temp)+1, sizeof(char));
      strcat(pixmap[i+1],temp);
    }
  for (i = 0 ; i < rows ; i++)
    {
      for (j = 0 ; j < cols ; j++)
	{
	  sprintf(temp,"%c", (i+j)%2 ? data[j] : data[i]);
	  strcat(pixmap[i+17],temp);
	}
    }

  pixwin = window_create(0,0,480,360,"Preview");
  mkpixmap(pixmap,pixwin);
   
  XSetWindowBackgroundPixmap(display,pixwin->win,pixwin->pix);
  XClearWindow(display,pixwin->win);


  /*
  XCopyArea(display,pixwin->pix,pixwin->win,pixwin->gc,0,0,
                pixwin->wpix,pixwin->hpix,0,0);
  */

  //printf("wpix: %d, hpix : %d\n", pixwin->wpix, pixwin->hpix);
  //printf("%s\n", sym_flag);

  button_create(8,-8,72,24,"Save",isave);
  button_create(-8,-8,72,24,"Discard",window_close);
  
  /*
  for(i = 0; i < 25; i++)
    {
      printf("%s\n", pixmap[i]);
    }
  */
}

void updatef(struct xtree *t)
{
  register int i;

  tot_thr = thread[0] + (sym_flag[0] ? 2 : 1) * ( thread[1] + thread[2] +  thread[3] + thread[4] + thread[5] + thread[6] + thread[7] + thread[8] + thread[9] + thread[10] + thread[11] + thread[12] + thread[13] + thread[14] + thread[15]);

  for(i = 0 ; i < 16 ; i++)
    {
      sprintf(buf[i],"#%d: %d", i+1, thread[i]);
    }
  sprintf(text,"%d",tot_thr);    

}

void draw(struct xtree *t)
{
  int i, i_abs;

      if(tot_thr == 0) 
	{   
	  XSetForeground(display,t->gc,gray);
	  XFillRectangle(display,t->win,t->gc,0,0,t->aw,t->ah);
	  return;
	}
      else
	{
	
	  for(i = sym_flag[0] ? -15 : 0 ; i < 16 ; i++)
	    {
	      i_abs = abs(i);

	      XSetForeground(display,t->gc,color[i_abs]);
	      XFillRectangle(display,t->win,t->gc,
			     t->aw * sum_lenght(i) / tot_thr, 0,
			     t->aw * thread[i_abs] / tot_thr,
		             t->ah);
	    }
	}
  
}


void xmain(int n,char *p[])
{
  struct xtree *win;
 
  char   *rgb_colors[4096], buf1[256] ,*q;
  FILE   *rgb;

  int colormax = 0;

  rgb = fopen("/usr/X11R6/lib/X11/rgb.txt","r");

  while(fgets(buf1,256,rgb))
    {

      if(buf1[0]=='!') continue;

      buf1[strlen(buf1)-1]=0;
      q = buf1;
      
      while(*q<65) q++;
      rgb_colors[colormax] = (char *) calloc(1+strlen(q),sizeof(char));
      strcpy(rgb_colors[colormax],q);
      colormax++;

    }       

  srand(time(NULL));
 
  for(k = 0; k < 16; k++) color[k]=rancol();



  win=window_create(0,0,COL_4+106,400,"YerKilt v0.2");
  win->broadcast=updatef;

  finetune_create(COL_1,ROW_1,&thread[0],&min,&max);
  finetune_create(COL_1,ROW_2,&thread[1],&min,&max);
  finetune_create(COL_1,ROW_3,&thread[2],&min,&max);
  finetune_create(COL_1,ROW_4,&thread[3],&min,&max);
  finetune_create(COL_2,ROW_1,&thread[4],&min,&max);
  finetune_create(COL_2,ROW_2,&thread[5],&min,&max);
  finetune_create(COL_2,ROW_3,&thread[6],&min,&max);
  finetune_create(COL_2,ROW_4,&thread[7],&min,&max);
  finetune_create(COL_3,ROW_1,&thread[8],&min,&max);
  finetune_create(COL_3,ROW_2,&thread[9],&min,&max);
  finetune_create(COL_3,ROW_3,&thread[10],&min,&max);
  finetune_create(COL_3,ROW_4,&thread[11],&min,&max);
  finetune_create(COL_4,ROW_1,&thread[12],&min,&max);
  finetune_create(COL_4,ROW_2,&thread[13],&min,&max);
  finetune_create(COL_4,ROW_3,&thread[14],&min,&max);
  finetune_create(COL_4,ROW_4,&thread[15],&min,&max);
 
  /*
  downbox(COL_1+16,ROW_1,32,32,color[0],win);
  downbox(COL_1+16,ROW_2,32,32,color[1],win);
  downbox(COL_1+16,ROW_3,32,32,color[2],win);
  downbox(COL_1+16,ROW_4,32,32,color[3],win);
  downbox(COL_2+16,ROW_1,32,32,color[4],win);
  downbox(COL_2+16,ROW_2,32,32,color[5],win);
  downbox(COL_2+16,ROW_3,32,32,color[6],win);
  downbox(COL_2+16,ROW_4,32,32,color[7],win);
  downbox(COL_3+16,ROW_1,32,32,color[8],win);
  downbox(COL_3+16,ROW_2,32,32,color[9],win);
  downbox(COL_3+16,ROW_3,32,32,color[10],win);
  downbox(COL_3+16,ROW_4,32,32,color[11],win);
  downbox(COL_4+16,ROW_1,32,32,color[12],win);
  downbox(COL_4+16,ROW_2,32,32,color[13],win);
  downbox(COL_4+16,ROW_3,32,32,color[14],win);
  downbox(COL_4+16,ROW_4,32,32,color[15],win);
  */

  b[0]=box_create(COL_1+16,ROW_1,82,32,NULL,color[0]);
  b[1]=box_create(COL_1+16,ROW_2,82,32,NULL,color[1]);
  b[2]=box_create(COL_1+16,ROW_3,82,32,NULL,color[2]);
  b[3]=box_create(COL_1+16,ROW_4,82,32,NULL,color[3]);
  b[4]=box_create(COL_2+16,ROW_1,82,32,NULL,color[4]);
  b[5]=box_create(COL_2+16,ROW_2,82,32,NULL,color[5]);
  b[6]=box_create(COL_2+16,ROW_3,82,32,NULL,color[6]);
  b[7]=box_create(COL_2+16,ROW_4,82,32,NULL,color[7]);
  b[8]=box_create(COL_3+16,ROW_1,82,32,NULL,color[8]);
  b[9]=box_create(COL_3+16,ROW_2,82,32,NULL,color[9]);
  b[10]=box_create(COL_3+16,ROW_3,82,32,NULL,color[10]);
  b[11]=box_create(COL_3+16,ROW_4,82,32,NULL,color[11]);
  b[12]=box_create(COL_4+16,ROW_1,82,32,NULL,color[12]);
  b[13]=box_create(COL_4+16,ROW_2,82,32,NULL,color[13]);
  b[14]=box_create(COL_4+16,ROW_3,82,32,NULL,color[14]);
  b[15]=box_create(COL_4+16,ROW_4,82,32,NULL,color[15]);

  label_create(COL_1+29,ROW_1+6,56,20,buf[0],white,left);
  label_create(COL_1+29,ROW_2+6,56,20,buf[1],white,left);
  label_create(COL_1+29,ROW_3+6,56,20,buf[2],white,left);
  label_create(COL_1+29,ROW_4+6,56,20,buf[3],white,left);
  label_create(COL_2+29,ROW_1+6,56,20,buf[4],white,left);
  label_create(COL_2+29,ROW_2+6,56,20,buf[5],white,left);
  label_create(COL_2+29,ROW_3+6,56,20,buf[6],white,left);
  label_create(COL_2+29,ROW_4+6,56,20,buf[7],white,left);
  label_create(COL_3+29,ROW_1+6,56,20,buf[8],white,left);
  label_create(COL_3+29,ROW_2+6,56,20,buf[9],white,left);
  label_create(COL_3+29,ROW_3+6,56,20,buf[10],white,left);  
  label_create(COL_3+29,ROW_4+6,56,20,buf[11],white,left);
  label_create(COL_4+29,ROW_1+6,56,20,buf[12],white,left);
  label_create(COL_4+29,ROW_2+6,56,20,buf[13],white,left);
  label_create(COL_4+29,ROW_3+6,56,20,buf[14],white,left);
  label_create(COL_4+29,ROW_4+6,56,20,buf[15],white,left);

  check_create(8,-56,80,24,"Symmetry",sym_flag);

  label_create(8,-8,42,24,"Size :",invisible,left);
  label_create(50,-8,32,24,text,invisible,up);
  label_create(82,-8,24,24,"x",invisible,up);
  label_create(114,-8,32,24,text,invisible,up);
  label_create(114,-8,32,24,text,invisible,up);
  label_create(146,-8,42,24,"pixels",invisible,left);

  box_create(COL_1,ROW_5,-8,32,NULL,black);
  box_create(COL_1+4,ROW_5+4,-12,24,draw,gray);

  button_create(8,-90-72,120,24,"Load Pattern",load);
  button_create(8,-90-48,120,24,"Save Pattern",save);
  button_create(8,-90-24,120,24,"Random Colors",randomize);
  button_create(8,-90,120,24,"Change Colors",col_change_menu);

  button_create( -80,-8,72,24,"Generate",engine);
  button_create( -8,-8,72,24,"Quit",window_close);

}











