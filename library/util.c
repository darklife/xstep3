/*

XSTEP 3.4 - Toolkit for X-Window System
Copyright (C) 1996,1997,1998,1999 by Marcelo Samsoniuk

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

Please read the COPYING and README file!!!

*/

#include "xstep.h"

int getcolor(int red,int green, int blue) {

	XColor  color;

	color.red=red;
	color.green=green;
	color.blue=blue;
	XAllocColor(display,DefaultColormap(display,screen),&color);
	return(color.pixel);
}

int getnamedcolor(char *name) {

        XColor  color;

        XAllocNamedColor(display,
                DefaultColormap(display,screen),
                name,
                &color,&color);

        return(color.pixel);
}

void upbox(int x0,int y0,int w,int h,int c,struct xtree *treeptr) {

	int x1,y1;

	x1=x0+w-1;
	y1=y0+h-1;

	if(c!=invisible) {
		XSetForeground(display,treeptr->gc,c);
		XFillRectangle(display,treeptr->win,treeptr->gc,x0+2,y0+2,w-4,h-4);
	}
	XSetForeground(display,treeptr->gc,white);
	XDrawLine(display,treeptr->win,treeptr->gc,x0,y0,x1,y0);
	XDrawLine(display,treeptr->win,treeptr->gc,x0,y0,x0,y1);
	XSetForeground(display,treeptr->gc,gray);
	XDrawLine(display,treeptr->win,treeptr->gc,x0+1,y0+1,x1-1,y0+1);
	XDrawLine(display,treeptr->win,treeptr->gc,x0+1,y0+1,x0+1,y1-1);
	XSetForeground(display,treeptr->gc,darkgray);
	XDrawLine(display,treeptr->win,treeptr->gc,x1-1,y1-1,x1-1,y0+1);
	XDrawLine(display,treeptr->win,treeptr->gc,x1-1,y1-1,x0+1,y1-1);
	XSetForeground(display,treeptr->gc,black);
	XDrawLine(display,treeptr->win,treeptr->gc,x1,y1,x1,y0);
	XDrawLine(display,treeptr->win,treeptr->gc,x1,y1,x0,y1);
}

void upboxsemi(int x0,int y0,int w,int h,int c,struct xtree *treeptr) {

	int x1,y1;

	x1=x0+w-1;
	y1=y0+h-1;

	if(c!=invisible) {
		XSetForeground(display,treeptr->gc,c);
		XFillRectangle(display,treeptr->win,treeptr->gc,x0+2,y0+2,w-4,h-4);
	}
	XSetForeground(display,treeptr->gc,white);
	XDrawLine(display,treeptr->win,treeptr->gc,x0,y0,x1,y0);
	XSetForeground(display,treeptr->gc,darkgray);
	XDrawLine(display,treeptr->win,treeptr->gc,x1,y1,x0,y1);
}

void downbox(int x0,int y0,int w,int h,int c,struct xtree *treeptr) {

	int x1,y1;

	x1=x0+w-1;
	y1=y0+h-1;

	if(c!=invisible) {
		XSetForeground(display,treeptr->gc,c);
		XFillRectangle(display,treeptr->win,treeptr->gc,x0+2,y0+2,w-4,h-4);
	}
	XSetForeground(display,treeptr->gc,white);
	XDrawLine(display,treeptr->win,treeptr->gc,x1,y1,x1,y0);
	XDrawLine(display,treeptr->win,treeptr->gc,x1,y1,x0,y1);
	XSetForeground(display,treeptr->gc,gray);
	XDrawLine(display,treeptr->win,treeptr->gc,x1-1,y1-1,x1-1,y0+1);
	XDrawLine(display,treeptr->win,treeptr->gc,x1-1,y1-1,x0+1,y1-1);
	XSetForeground(display,treeptr->gc,darkgray);
	XDrawLine(display,treeptr->win,treeptr->gc,x0,y0,x1,y0);
	XDrawLine(display,treeptr->win,treeptr->gc,x0,y0,x0,y1);
	XSetForeground(display,treeptr->gc,black);
	XDrawLine(display,treeptr->win,treeptr->gc,x0+1,y0+1,x1-1,y0+1);
	XDrawLine(display,treeptr->win,treeptr->gc,x0+1,y0+1,x0+1,y1-1);
}

int checkbox(int x0,int y0,int w,int h) {

	int x1,y1;

	x1=x0+w-1;
	y1=y0+h-1;

	if(report.xbutton.x<x0) 		return(0);
	if(report.xbutton.y<y0) 		return(0);
	if(report.xbutton.x>x1) 		return(0);
	if(report.xbutton.y>y1) 		return(0);
	return(1);
}

int checkexpose(int x0,int y0,int w,int h) {

	int x1,y1,x2,y2;

	x1=x0+w-1;
	y1=y0+h-1;

	if((x2=report.xexpose.x)>x1) 		return(0);
	if((y2=report.xexpose.y)>y1) 		return(0);
	if((x2+report.xexpose.width -1)<x0) 	return(0);
	if((y2+report.xexpose.height-1)<y0) 	return(0);
	return(1);
}

int strcheck(char *p) {

        int i,k;

        k=(int)p;

        for(i=0;p[i];i++) {
                k+=(p[i]^i);
#ifdef DEBUG
                printf("checksum of %s: %x in p[%d]='%c'\n",p,k,i,p[i]);
#endif
        }
        return(k);
}

void bitbox(int x0,int y0,int w,int h,char *c[],struct xtree *treeptr) {

	int x,y;

	for(y=0;y!=h;y++) {
		for(x=0;x!=w;x++) {
			switch(*(c[y]+x)) {
				case '0': XSetForeground(display,treeptr->gc,black); break;
				case '1': XSetForeground(display,treeptr->gc,darkgray); break;
				case '2': continue;
				case '3': XSetForeground(display,treeptr->gc,white); break;
			}
			XDrawPoint(display,treeptr->win,treeptr->gc,x0+x,y0+y);
		}
	}
}

void reconfigure(struct xtree *treeptr) {

        struct xtree *treeaux;

        int w,h;

        treeptr->x=report.xconfigure.x;
        treeptr->y=report.xconfigure.y;
        treeptr->aw=report.xconfigure.width;
        treeptr->ah=report.xconfigure.height;

        for(treeaux=treeptr->treestk;
            treeaux;
            treeaux=treeaux->next) {

                w=(treeaux->w>0)?treeaux->w:treeptr->aw-treeaux->x+treeaux->w;
                h=(treeaux->h>0)?treeaux->h:treeptr->ah-treeaux->y+treeaux->h;

                XMoveResizeWindow(display,treeaux->win,
                        (treeaux->x>=0)?treeaux->x:treeptr->aw+treeaux->x-treeaux->w,
                        (treeaux->y>=0)?treeaux->y:treeptr->ah+treeaux->y-treeaux->h,
                        treeaux->aw=w /*w>20?w:21*/,
                        treeaux->ah=h /*h>20?h:21*/);
        }
}


void mkpixmap(char *xpm[],struct xtree *t) {

        int c,z,i,j,color[65536];
        char name[64];
        unsigned char index;

        sscanf(xpm[0],"%d %d %d %d",&t->wpix,&t->hpix,&c,&z);

        t->pix=XCreatePixmap(display,t->win,t->wpix,t->hpix,DefaultDepth(display,screen));
        t->gcpix=XCreateGC(display,t->pix,0,&values);
        XSetLineAttributes(display,t->gcpix,0,LineSolid,CapRound,JoinRound);

        XSetForeground(display,t->gcpix,gray);
        XFillRectangle(display,t->pix,t->gcpix,0,0,t->wpix,t->hpix);

        if(z!=1) {

                printf("pixmap packet format %d unsuported, go home! :)\n",z);
                return;
        }

        for(i=0;i!=c;i++) {

                sscanf(xpm[i+1],"%c %c %s",&index,(char *)&z,name);
                color[index]=getnamedcolor(name);

                if(!strcmp(name,"None")) color[index]=-1;
        }

        for(j=0;j!=t->hpix;j++) {

                for(i=0;i!=t->wpix;i++) {

                        if(color[(unsigned)*(xpm[j+c+1]+i)]!=-1) {

                                if(color[(unsigned)*(xpm[j+c+1]+i)]!=z) {

                                        XSetForeground(display,t->gcpix,
                                                color[(unsigned)*(xpm[j+c+1]+i)]);

                                        z=color[(unsigned)*(xpm[j+c+1]+i)];
                                }
                                XDrawPoint(display,t->pix,t->gcpix,i,j);
                        }
                }
        }
}
