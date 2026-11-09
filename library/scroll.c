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

#include "scroll.h"

void scroll_configure(struct xtree *treeptr) {

	struct xtree *treeaux;

	int w,h;

	treeptr->x=report.xconfigure.x;
	treeptr->y=report.xconfigure.y;
	treeptr->aw=report.xconfigure.width;
	treeptr->ah=report.xconfigure.height;

	for(treeaux=treeptr->treestk;
	    treeaux;
	    treeaux=treeaux->next) {

		w=(treeaux->w>=0)?treeaux->w:treeptr->aw-treeaux->x+treeaux->w;
		h=(treeaux->h>=0)?treeaux->h:treeptr->ah-treeaux->y+treeaux->h;

		XMoveResizeWindow(display,treeaux->win,
			(treeaux->x>=0)?treeaux->x:treeptr->aw+treeaux->x-treeaux->w,
			(treeaux->y>=0)?treeaux->y:treeptr->ah+treeaux->y-treeaux->h,
			treeaux->aw=w>0?w:1,
			treeaux->ah=h>0?h:1);
	}
}

void scroll_expose(struct xtree *treeptr) { 

	downbox(0,0,treeptr->aw,treeptr->ah,invisible,treeptr);
}

struct xtree *scroll_create(int x,int y,int w,int h,int *q,int *m,char *z[],void (*f)(struct xtree *)) {

	struct xtree *treeaux;

	treeaux=(struct xtree *)malloc(sizeof(struct xtree));
	memset(treeaux,0,sizeof(struct xtree));
	treeaux->x=x;
	treeaux->y=y;
	treeaux->w=w;
	treeaux->h=h;
	treeaux->q=q;
	treeaux->m=m;
	treeaux->c=*q;
	treeaux->n=&treeaux->c;
	treeaux->treestk=0;
	treeaux->expose=scroll_expose;
	treeaux->configure=scroll_configure;
	treeaux->parent=treestk;

	treeaux->win=XCreateSimpleWindow(display,treestk->win,
		(x>=0)?x:treestk->w+x-w,
		(y>=0)?y:treestk->h+y-h,
		treeaux->aw=(w>=0)?w:treestk->w-x+w,
		treeaux->ah=(h>=0)?h:treestk->h-y+h,
		0,black,gray);

	treeaux->gc=XCreateGC(display,treeaux->win,0,&values);

	XSelectInput(display,treeaux->win,
		ExposureMask|
		ButtonPressMask|
		ButtonMotionMask|
		ButtonReleaseMask|
		StructureNotifyMask);

	XSetFont(display,treeaux->gc,helvetica12m->fid);
	XSetLineAttributes(display,treeaux->gc,0,LineSolid,CapRound,JoinRound);
	XMapWindow(display,treeaux->win);
	treeaux->next=treestk->treestk;
	treestk->treestk=treeaux;

	scrollbar_create(3,3,(h>=0)?h-40:-37,q,m,treeaux->n);
	upbutton_create(3,(h>=0)?h-36:-20,q,m);
	downbutton_create(3,(h>=0)?h-19:-3,q,m);
	list_create(20,2,(w>=0)?w-22:-2,(h>=0)?h-4:-2,q,m,treeaux->n,z,f,0,0);

	return(treeaux);
}

struct xtree *mscroll_create(int x,int y,int w,int h,int *q,int *m,struct mlist *z,void (*f)(struct xtree *),void (*dc)(struct xtree *),void (*rc)(struct xtree *),int k) {

	int i,sw,lw=w;
	struct xtree *treeaux;

	y+=21;
	treeaux=(struct xtree *)malloc(sizeof(struct xtree));
	memset(treeaux,0,sizeof(struct xtree));
	treeaux->x=x;
	treeaux->y=y;
	treeaux->w=w;
	treeaux->h=h;
	treeaux->q=q;
	treeaux->m=m;
	treeaux->c=*q;
	treeaux->n=&treeaux->c;
	treeaux->treestk=0;
	treeaux->expose=scroll_expose;
	treeaux->configure=scroll_configure;
	treeaux->parent=treestk;

	treeaux->win=XCreateSimpleWindow(display,treestk->win,
		(x>=0)?x:treestk->w+x-w,
		(y>=0)?y:treestk->h+y-h,
		treeaux->aw=(w>=0)?w:treestk->w-x+w,
		treeaux->ah=(h>=0)?h:treestk->h-y+h,
		0,black,gray);
	treeaux->gc=XCreateGC(display,treeaux->win,0,&values);

	XSelectInput(display,treeaux->win,
		ExposureMask|
		ButtonPressMask|
		ButtonMotionMask|
		ButtonReleaseMask|
		StructureNotifyMask);

	XSetFont(display,treeaux->gc,helvetica12m->fid);
	XSetLineAttributes(display,treeaux->gc,0,LineSolid,CapRound,JoinRound);
	XMapWindow(display,treeaux->win);
	treeaux->next=treestk->treestk;
	treestk->treestk=treeaux;

	scrollbar_create(3,3,(h>=0)?h-40:-37,q,m,treeaux->n);
	upbutton_create(3,(h>=0)?h-36:-20,q,m);
	downbutton_create(3,(h>=0)?h-19:-3,q,m);

	if(w>0) 
		for(w=sw=i=0;i!=k;i++) {
		
			list_create(20+w,
				2,
				i!=(k-1)?z[i].w:lw-w-22,
				(h>=0)?h-4:-2,
				q,m,treeaux->n,z[i].p,f,dc,rc);
			w+=z[i].w;
		}
	else 
		for(sw=w=i=0;i!=k;i++) {
			list_create(20+sw,2,i!=(k-1)?z[i].w:-2,(h>=0)?h-4:-2,q,m,treeaux->n,z[i].p,!i?f:0,dc,rc);
			sw+=z[i].w;
		}
	w=lw;
	if(w>0) {

		for(w=sw=i=0;i!=k;i++) {
			label_create(x+20+w,
				y-21,
				i!=(k-1)?z[i].w:lw-w-20,
				21,
				z[i].t,darkgray,center);
				
			w+=z[i].w;
		}
	} else 
		for(sw=w=i=0;i!=k;i++) {
			label_create(x+20+sw,y-21,i!=(k-1)?z[i].w:lw,21,z[i].t,darkgray,center);
			sw+=z[i].w;
		}

	return(treeaux);
}
