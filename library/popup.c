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
 
void popup_expose(struct xtree *treeptr) {

	upbox(0,0,treeptr->w,treeptr->h,gray,treeptr);
	upbox(treeptr->w-20,8,12,6,gray,treeptr);
	XDrawString(display,treeptr->win,treeptr->gc,
		4,14,treeptr->t,strlen(treeptr->t));
}

void popup_broadcast(struct xtree *treeptr) {

	if(treeptr->k==strcheck(treeptr->t)) return;
	treeptr->k=strcheck(treeptr->t);
	popup_expose(treeptr);
}

void popup_buttonpress(struct xtree *treeptr) {

	menu_create(0,0,treeptr->w,treeptr->c,treeptr->z,treeptr,0);
	treestk->t=treeptr->t;
}

struct xtree *popup_create(int x,int y,int w,int h,char *t,char **z,int c) {

	struct xtree *treeaux;

	treeaux=(struct xtree *)malloc(sizeof(struct xtree));
	memset(treeaux,0,sizeof(struct xtree));

	treeaux->x=x; 
	treeaux->y=y;
	treeaux->w=w;
	treeaux->h=h;
	treeaux->t=t;
	treeaux->z=z;
	treeaux->expose=popup_expose;
	treeaux->buttonpress=popup_buttonpress;
	treeaux->broadcast=popup_broadcast;
	treeaux->parent=treestk;

	/* treeaux->c=c*21; 	this line will be enabled only in 
				version 4.x to avoid more problems 
				with old source codes in versions
				3.x of XSTEP.
	*/
	treeaux->c=c;

	treeaux->win=XCreateSimpleWindow(display,treestk->win,
		(x>=0)?x:treestk->w+x-w,
		(y>=0)?y:treestk->h+y-h,
		w,h,0,black,gray);

	treeaux->gc=XCreateGC(display,treeaux->win,0,&values);
	
	XSelectInput(display,treeaux->win,
		ExposureMask|
		ButtonPressMask|
		ButtonReleaseMask);
	XSetFont(display,treeaux->gc,helvetica12m->fid);
	XSetLineAttributes(display,treeaux->gc,0,LineSolid,CapRound,JoinRound);
	XMapWindow(display,treeaux->win);
	treeaux->next=treestk->treestk;
	treestk->treestk=treeaux;

	return(treeaux);
}
