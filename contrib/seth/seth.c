#include <xstep.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define DEPTH 2
#define BORDO 10
#define SCALA 10000

int color[16] = {0xff0000, 0x00ff00, 0x0000ff, 0xffffff, 0xffff00, 0x00ffff,
		 0xff00ff, 0xffcc00, 0xccff00, 0xcc6600, 0x00cc00, 0x00ff66,
		 0x00ffcc, 0x00ccff, 0x0044bb, 0x00cccc};
int history[16], depth=0, mindepth=0, maxdepth=15;
int nlati=3, minlati=3, maxlati=16;
int ratio=161, maxratio=323, minratio=0;
char string[4][16];
struct wwxys { int x,y; } xy[16] = {{200*SCALA,70*SCALA},{50*SCALA,330*SCALA},
{350*SCALA,330*SCALA}};

struct wwxys dot;

struct xtree *group, f1, f2, f3;

struct wwxys ratios[] = {
#include "seth.ci"
};


void trovavert(int w, int h)
{
  int raggio,lato,i;
  float angolo;
  struct wwxys centro;

  raggio = (w < h) ? w/2 : h/2;
  raggio -= 2 * BORDO;
  raggio *= SCALA;
  centro.x = w/2 * SCALA;
  centro.y = h/2 * SCALA;
  angolo =(float) M_PI / nlati * 2;
  lato = raggio * sin(2 * angolo);

  for(i = 0; i < nlati; i++)
    {
      xy[i].x = centro.x + raggio * cos((2*i+1)*angolo/2);
      xy[i].y = centro.y + raggio * sin((2*i+1)*angolo/2);
    }
}

void draw(struct xtree *t)
{
  register int i,j,r;
  static int old;
  int num,den;

  if(old != nlati * ratio)
    {
      old = nlati * ratio;
      XClearArea(display, t->win, 0, 0, t->w, t->h, True);
    }

  num = ratios[ratio].x;
  den = ratios[ratio].y;

  
  sprintf(string[0],"%2d",nlati);
  sprintf(string[1],"%2d",depth); 
  sprintf(string[2],"0.%.3u",1000*num/den);
  sprintf(string[3],"%d %d %d %d",nlati,depth,num,den);

  /*
  XSetForeground(display, t->gc, 0xffffff);
  XDrawString(display, t->win, t->gc, 8, t->ah-8, string, strlen(string));
  */
  /*
  XDrawLine(display, t->win, t->gc, xy[0].x, xy[0].y, xy[1].x, xy[1].y);
  XDrawLine(display, t->win, t->gc, xy[1].x, xy[1].y, xy[2].x, xy[2].y);
  XDrawLine(display, t->win, t->gc, xy[2].x, xy[2].y, xy[0].x, xy[0].y);
  */
  /*
  xy[0].x=t->aw/2 * SCALA;
  xy[0].y=20 * SCALA;
  xy[1].x=((t->aw/2)-(t->ah-60)/1.732) * SCALA;
  xy[1].y=(t->ah-40) * SCALA;
  xy[2].x=((t->aw/2)+(t->ah-60)/1.732) * SCALA;
  xy[2].y=(t->ah-40) * SCALA;
  */

  trovavert(t->aw,t->ah);

  for(i = 0; i < 1000; i++)
    {
      r = rand() % nlati;
      for(j = 15; j>0; j--) 
	history[j] = history[j-1];
      history[0] = r;
      dot.x = dot.x + (xy[r].x - dot.x) * num / den;
      dot.y = dot.y + (xy[r].y - dot.y) * num / den;
      
      XSetForeground(display, t->gc, color[history[depth]]);
      XDrawPoint(display, t->win, t->gc, dot.x/SCALA, dot.y/SCALA);
    }
}

void xmain(int n,char *p[])
{
  srand(time(NULL));

  dot.x = rand() % (400*SCALA);
  dot.y = rand() % (400*SCALA);

  window_create(0,0,488,400,"Seth");
  animate=1;
  
  label_create(-8,8,72,24,"Sides",invisible,left);
  finetune_create(-64,32,&nlati,&minlati,&maxlati);
  label_create(-8,32,56,32,string[0],white,center);

  label_create(-8,72,72,24,"Depth",invisible,left);
  finetune_create(-64,96,&depth,&mindepth,&maxdepth);
  label_create(-8,96,56,32,string[1],white,center);

  label_create(-8,136,72,24,"Ratio",invisible,left);
  finetune_create(-64,160,&ratio,&minratio,&maxratio);
  label_create(-8,160,56,32,string[2],white,center);

  button_create(-8,-8,72,24,"Chiudi",window_close);

  box_create(8,8,-88,-8,draw,black);
/*
  window_create(0,0,188,100,"Debug");
  label_create(8,8,-8,-8,string[3],white,center);
*/
}








