#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <xstep.h>
#include "style.h"

static int     tempcol[3], dummy;
extern int     color[16];
extern int     min,max;
static unsigned int workcol;
static struct xtree *cwin;
static char buffer[7];

extern void color_update(int index,int new);
void col_c_up(struct xtree *t);
void col_c_new(struct xtree *t);
void col_c_old(struct xtree *t);
void accept_color(struct xtree *t);
void type_color(struct xtree *t);

void type_color(struct xtree *t)
{
  int a,b,c,d;

  d = sscanf(buffer,"%2x%2x%2x",&a,&b,&c); 
  // printf("%x %x %x %d\n",a,b,c,d); /* DEBUG */
  if(d != 3)
    {
      sprintf(buffer,"ERROR!");
    }
  else
    {
      workcol = a * 0x10000 + b * 0x100 + c;
      tempcol[0] = (workcol & 0xff0000) >> 16;
      tempcol[1] = (workcol & 0x00ff00) >> 8;
      tempcol[2] = workcol & 0x0000ff;
      sprintf(buffer,"%.6x",workcol);
    }
}

void accept_color(struct xtree *t)
{
  color_update(dummy,workcol);
  sprintf(buffer,"%.6x",workcol);
  broadcast++;
  window_close(cwin);
}

void col_change_func(struct xtree *t)
{
  static char titlebar[24];

  if(t->l >= 0) 
    {
      dummy = t->l;
      sprintf(titlebar,"Change Color #%d",t->l+1);

      workcol = color[t->l];
      tempcol[0] = (workcol & 0xff0000) >> 16;
      tempcol[1] = (workcol & 0x00ff00) >> 8;
      tempcol[2] = workcol & 0x0000ff;

      cwin = window_create(0,0,288,240,titlebar);
 
      button_create( 8,-8,72,24,"Accept",accept_color);
      button_create( -8,-8,72,24,"Cancel",window_close);


      box_create(COL_1+16,ROW_1,124,32,col_c_new,workcol);
      box_create(156,ROW_1,124,32,col_c_old,color[t->l]);

      finetune_create(COL_1,ROW_2,&tempcol[0],&min,&max);
      finetune_create(COL_1,ROW_3,&tempcol[1],&min,&max);
      finetune_create(COL_1,ROW_4,&tempcol[2],&min,&max);

      label_create(COL_1,ROW_5,COL_3-COL_1,21,"Insert hex code (6 chars.) :",invisible,left);
      edit_create(COL_3-60,ROW_5,280-(COL_3-60),21,buffer,7,type_color);

      box_create(COL_1+16,ROW_2,256,ROW_4-ROW_2+32,col_c_up,gray);

      
    }
}

void col_c_new(struct xtree *t)
{
  static char a[]="New Color";
  workcol = tempcol[0]*0x10000 + tempcol[1]*0x100 + tempcol[2];

  XSetForeground(display,t->gc,workcol);
  XFillRectangle(display,t->win,t->gc,
		 0, 0,
		 t->aw,
		 t->ah);

  XSetForeground(display,t->gc,white);
  XDrawLine(display,t->win,t->gc,t->aw-1,t->ah-1,t->aw-1,1);
  XDrawLine(display,t->win,t->gc,t->aw-1,t->ah-1,1,t->ah-1);  

  XSetForeground(display,t->gc,gray);
  XDrawLine(display,t->win,t->gc,t->aw-2,t->ah-2,t->aw-2,1);
  XDrawLine(display,t->win,t->gc,t->aw-2,t->ah-2,1,t->ah-2); 

  XSetForeground(display,t->gc,darkgray);
  XDrawLine(display,t->win,t->gc,0,0,t->aw-1,0);
  XDrawLine(display,t->win,t->gc,0,0,0,t->ah-1); 
  
  XSetForeground(display,t->gc,black);
  XDrawLine(display,t->win,t->gc,1,1,t->aw-2,1);
  XDrawLine(display,t->win,t->gc,1,1,1,t->ah-2);

  XDrawString(display,t->win,t->gc,8,20,a,strlen(a));
}

void col_c_old(struct xtree *t)
{
  static char a[]="Old Color";

  XSetForeground(display,t->gc,white);
  XDrawLine(display,t->win,t->gc,t->aw-1,t->ah-1,t->aw-1,1);
  XDrawLine(display,t->win,t->gc,t->aw-1,t->ah-1,1,t->ah-1);  

  XSetForeground(display,t->gc,gray);
  XDrawLine(display,t->win,t->gc,t->aw-2,t->ah-2,t->aw-2,1);
  XDrawLine(display,t->win,t->gc,t->aw-2,t->ah-2,1,t->ah-2); 

  XSetForeground(display,t->gc,darkgray);
  XDrawLine(display,t->win,t->gc,0,0,t->aw-1,0);
  XDrawLine(display,t->win,t->gc,0,0,0,t->ah-1); 
  
  XSetForeground(display,t->gc,black);
  XDrawLine(display,t->win,t->gc,1,1,t->aw-2,1);
  XDrawLine(display,t->win,t->gc,1,1,1,t->ah-2);

  XDrawString(display,t->win,t->gc,8,20,a,strlen(a));
}

void col_c_up(struct xtree *t)
{
  static char a[]="Red : 0x00", b[]="Green : 0x00", c[]="Blue : 0x00";
  sprintf(a,"Red : %#.2X",tempcol[0]);
  sprintf(b,"Green : %#.2X",tempcol[1]);
  sprintf(c,"Blue : %#.2X",tempcol[2]);

  XSetForeground(display,t->gc,tempcol[0]*0x10000);
  XFillRectangle(display,t->win,t->gc,
		 0, 0,
		 tempcol[0],
		 32); 
  XSetForeground(display,t->gc,tempcol[1]*0x100);
  XFillRectangle(display,t->win,t->gc,
		 0, 42,
		 tempcol[1],
		 32); 
  XSetForeground(display,t->gc,tempcol[2]);
  XFillRectangle(display,t->win,t->gc,
		 0, 84,
		 tempcol[2],
		 32); 
  XSetForeground(display,t->gc,gray);
  XFillRectangle(display,t->win,t->gc,
		 tempcol[0], 0,
		 256,
		 32);
  XFillRectangle(display,t->win,t->gc,
		 tempcol[1], 42,
		 256,
		 32);
  XFillRectangle(display,t->win,t->gc,
		 tempcol[2], 84,
		 256,
		 32);

  XSetForeground(display,t->gc,white);
  XDrawString(display,t->win,t->gc,8,20,a,strlen(a));
  XDrawString(display,t->win,t->gc,8,62,b,strlen(b));
  XDrawString(display,t->win,t->gc,8,104,c,strlen(c));

  XDrawLine(display,t->win,t->gc,t->aw-1,31,t->aw-1,1);
  XDrawLine(display,t->win,t->gc,t->aw-1,31,1,31);  
  XDrawLine(display,t->win,t->gc,t->aw-1,31+42,t->aw-1,1+42);
  XDrawLine(display,t->win,t->gc,t->aw-1,31+42,1,31+42);  
  XDrawLine(display,t->win,t->gc,t->aw-1,31+84,t->aw-1,1+84);
  XDrawLine(display,t->win,t->gc,t->aw-1,31+84,1,31+84); 
  
XSetForeground(display,t->gc,gray);
  XDrawLine(display,t->win,t->gc,t->aw-2,30,t->aw-2,1);
  XDrawLine(display,t->win,t->gc,t->aw-2,30,1,30); 
  XDrawLine(display,t->win,t->gc,t->aw-2,30+42,t->aw-2,1+42);
  XDrawLine(display,t->win,t->gc,t->aw-2,30+42,1,30+42);  
  XDrawLine(display,t->win,t->gc,t->aw-2,30+84,t->aw-2,1+84);
  XDrawLine(display,t->win,t->gc,t->aw-2,30+84,1,30+84); 
  
  XSetForeground(display,t->gc,darkgray);
  XDrawLine(display,t->win,t->gc,0,0,t->aw-1,0);
  XDrawLine(display,t->win,t->gc,0,0,0,31); 
  XDrawLine(display,t->win,t->gc,0,0+42,t->aw-1,0+42);
  XDrawLine(display,t->win,t->gc,0,0+42,0,31+42); 
  XDrawLine(display,t->win,t->gc,0,0+84,t->aw-1,0+84);
  XDrawLine(display,t->win,t->gc,0,0+84,0,31+84); 
  
  XSetForeground(display,t->gc,black);
  XDrawLine(display,t->win,t->gc,1,1,t->aw-2,1);
  XDrawLine(display,t->win,t->gc,1,1,1,30);  
  XDrawLine(display,t->win,t->gc,1,1+42,t->aw-2,1+42);
  XDrawLine(display,t->win,t->gc,1,1+42,1,30+42);
  XDrawLine(display,t->win,t->gc,1,1+84,t->aw-2,1+84);
  XDrawLine(display,t->win,t->gc,1,1+84,1,30+84);
}

void col_change_menu(struct xtree *t)
{  
static char  *list[16]={

    "Color  1", "Color  2", "Color  3", "Color  4", "Color  5", "Color  6", 
    "Color  7", "Color  8", "Color  9", "Color 10", "Color 11", "Color 12",
    "Color 13", "Color 14", "Color 15", "Color 16"};

 menu_create(0,0,120,21*16,list,0,col_change_func);           
}
