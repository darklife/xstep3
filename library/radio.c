/*

XSTEP 3.3 - Toolkit for X-Window System
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

void radio_broadcast(struct xtree *treeptr) {

	if(treeptr->k==strcheck(treeptr->b)) return;
	treeptr->k=strcheck(treeptr->b);

	if(!strcmp(treeptr->t,treeptr->b)) 
		XSetForeground(display,treeptr->gc,white);
	else 
		XSetForeground(display,treeptr->gc,gray);

	XFillArc(display,treeptr->win,treeptr->gc,2,4,12,12,0,23040);
	XSetForeground(display,treeptr->gc,white);
	XDrawArc(display,treeptr->win,treeptr->gc,0,2,15,15,14400,11520);
	XSetForeground(display,treeptr->gc,gray);
	XDrawArc(display,treeptr->win,treeptr->gc,1,3,13,13,14400,11520);
	XSetForeground(display,treeptr->gc,darkgray);
	XDrawArc(display,treeptr->win,treeptr->gc,0,2,15,15,2880,11520);
	XSetForeground(display,treeptr->gc,black);
	XDrawArc(display,treeptr->win,treeptr->gc,1,3,13,13,2880,11520);

	XFlush(display);
}

void radio_expose(struct xtree *treeptr) {

	if(!strcmp(treeptr->t,treeptr->b)) 
		XSetForeground(display,treeptr->gc,white);
	else 
		XSetForeground(display,treeptr->gc,gray);

	XFillArc(display,treeptr->win,treeptr->gc,2,4,12,12,0,23040);
	XSetForeground(display,treeptr->gc,white);
	XDrawArc(display,treeptr->win,treeptr->gc,0,2,15,15,14400,11520);
	XSetForeground(display,treeptr->gc,gray);
	XDrawArc(display,treeptr->win,treeptr->gc,1,3,13,13,14400,11520);
	XSetForeground(display,treeptr->gc,darkgray);
	XDrawArc(display,treeptr->win,treeptr->gc,0,2,15,15,2880,11520);
	XSetForeground(display,treeptr->gc,black);
	XDrawArc(display,treeptr->win,treeptr->gc,1,3,13,13,2880,11520);

	XDrawString(display,treeptr->win,treeptr->gc,
		24,14,treeptr->t,strlen(treeptr->t));
}

void radio_buttonpress(struct xtree *treeptr) {

	if(!checkbox(0,0,treeptr->w,treeptr->h)) return;
	strcpy(treeptr->b,treeptr->t);
	broadcast++;
}

struct xtree *radio_create(int x,int y,int w,int h,char *t,char *b) {

	struct xtree *treeaux;

	treeaux=(struct xtree *)malloc(sizeof(struct xtree));
	memset(treeaux,0,sizeof(struct xtree));

	treeaux->x=x;
	treeaux->y=y;
	treeaux->w=w;
	treeaux->h=h;
	treeaux->t=t;
	treeaux->b=b;
	treeaux->broadcast=radio_broadcast;
	treeaux->expose=radio_expose;
	treeaux->buttonpress=radio_buttonpress;
	treeaux->parent=treestk;

	treeaux->win=XCreateSimpleWindow(display,treestk->win,
		get_xywh(),0,black,gray);

	treeaux->gc=XCreateGC(display,treeaux->win,0,&values);

	XSelectInput(display,treeaux->win,
		ExposureMask|
		ButtonPressMask);

	XSetFont(display,treeaux->gc,defaultfont->fid);
	XSetLineAttributes(display,treeaux->gc,0,LineSolid,CapRound,JoinRound);
	XMapWindow(display,treeaux->win);
	treeaux->next=treestk->treestk;
	treestk->treestk=treeaux;

	return(treeaux);
}
