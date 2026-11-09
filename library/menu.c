/*

XSTEP 3.2 - Toolkit for X-Window System
Copyright (C) 1996,1997,1998 by Marcelo Samsoniuk

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
#include <X11/Xatom.h>
#include <unistd.h>

void menu_expose(struct xtree *treeptr) {

	int i;

	for(i=0;i!=(treeptr->ah/21);i++) {

		upbox(0,i*21,treeptr->aw,21,treeptr->l==i?white:gray,treeptr);
	        XDrawString(display,treeptr->win,treeptr->gc,
	                4,i*21+14,treeptr->z[i],strlen(treeptr->z[i]));
	}
}



void menu_buttonpress(struct xtree *treeptr) {

	/*if(treeptr->l>=0&&report.xbutton.y>=0
	   &&treeptr->l/21==report.xbutton.y/21) return;*/

	if(treeptr->l>=0&&treeptr->l<treeptr->ah) {

		upbox(0,21*(treeptr->l/21),treeptr->aw,21,gray,treeptr);
		XDrawString(display,treeptr->win,treeptr->gc,
			4,21*(treeptr->l/21)+14,
			treeptr->z[treeptr->l/21],
			strlen(treeptr->z[treeptr->l/21]));
	}

	if(report.xbutton.y>=0&&report.xbutton.y<treeptr->ah&&
	   report.xbutton.x>=0&&report.xbutton.x<treeptr->aw) {

	        upbox(0,21*(report.xbutton.y/21),treeptr->aw,21,white,treeptr);
		XDrawString(display,treeptr->win,treeptr->gc,
			4,21*(report.xbutton.y/21)+14,
			treeptr->z[report.xbutton.y/21],
			strlen(treeptr->z[report.xbutton.y/21]));

		treeptr->l=report.xbutton.y;	
			
	} else
		treeptr->l=-1;
}

void menu_buttonrelease(struct xtree *treeptr) {

        struct xtree *treeaux;
	int i;

        if(report.xbutton.y>0&&report.xbutton.y<treeptr->ah&&
           report.xbutton.x>0&&report.xbutton.x<treeptr->aw) {
        
        	treeptr->l=report.xbutton.y/21;
        
		for(i=0;i!=6;i++,usleep(1000000/20)) {

		        upbox(0,21*treeptr->l,treeptr->aw,21,i%2?gray:white,treeptr);
		        XDrawString(display,treeptr->win,treeptr->gc,
		                4,21*treeptr->l+14,
		                treeptr->z[treeptr->l],
		                strlen(treeptr->z[treeptr->l]));
			XFlush(display);
		}
		if(treeptr->t) strcpy(treeptr->t,treeptr->z[treeptr->l]); 
		broadcast++;
	
	} else {
	
		if(treeptr->t) strcpy(treeptr->t,"");
		broadcast++;
		treeptr->l=-1;
	}

	if(treeptr->f) treeptr->f(treeptr);

        XDestroyWindow(display,treeptr->win);
	XFlush(display);

        if(treeptr!=winstk) {

                for(treeaux=winstk;
                    treeaux->next!=treeptr;
                    treeaux=treeaux->next);

                treeaux->next=treeptr->next;

        } else winstk=treeptr->next;

        free(treeptr);
}

struct xtree *menu_create(int x,int y,int w,int h,char *z[],struct xtree *t,void (*f)(struct xtree *)) {

	XSetWindowAttributes attributes;
	struct xtree *treeaux;
        int rx,ry,wx,wy,mask,i;
        Window root,child;

	treeaux=(struct xtree *)malloc(sizeof(struct xtree));
	memset(treeaux,0,sizeof(struct xtree));
	if(x==0&&y==0) {
	        XQueryPointer(display,t?t->win:rootwindow,
			&root,&child,
			&rx,&ry,&wx,&wy,&mask);
		if(t) {
			x=rx-wx;
			y=ry-wy;
			for(i=0;i!=h/21;i++) 
				if(!strcmp(t->t,z[i])) {
					
					y-=i*21;
					treeaux->l=i;
					break;
				}
		} else {
			x=rx-w/2;
			y=ry-10;
		}
	}

	treeaux->x=x;
	treeaux->y=y;
	treeaux->aw=w;
	treeaux->ah=h;
	treeaux->z=z;
	treeaux->f=f;
	treeaux->t=t?t->t:0;
	treeaux->treestk=0;
	treeaux->expose=menu_expose;
	treeaux->buttonpress=menu_buttonpress;
	treeaux->buttonmotion=menu_buttonpress;
	treeaux->buttonrelease=menu_buttonrelease;

	attributes.save_under=True;
	attributes.override_redirect=True;
	treeaux->win=XCreateWindow(display,rootwindow,
		x,y,w,h,0,
		CopyFromParent,
		InputOutput,
		CopyFromParent,
		CWSaveUnder|CWOverrideRedirect,
		&attributes);

        XSelectInput(display,treeaux->win,
                ExposureMask|
                ButtonPressMask|
                ButtonReleaseMask|
                ButtonMotionMask);

	treeaux->gc=XCreateGC(display,treeaux->win,0,&values);
	XSetFont(display,treeaux->gc,helvetica12m->fid);
	XSetLineAttributes(display,treeaux->gc,0,LineSolid,CapRound,JoinRound);
	XDefineCursor(display,treeaux->win,XCreateFontCursor(display,XC_left_ptr));
	XMapWindow(display,treeaux->win);
	XGrabPointer(display,treeaux->win,False,
		ButtonPressMask|ButtonReleaseMask|ButtonMotionMask,
		GrabModeAsync,GrabModeAsync,
		None,
		None,
		CurrentTime);
	treeaux->next=winstk;
	winstk=treeaux;

	return(treeaux);
}
