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
 
void label_expose(struct xtree *treeptr) {

	if(treeptr->c!=invisible) {
		if((treeptr->s)!=up)
			downbox(0,0,treeptr->aw,treeptr->ah,treeptr->c,treeptr);
		else
			upboxsemi(0,0,treeptr->aw,treeptr->ah,treeptr->c,treeptr);
	} else {
	
		XSetForeground(display,treeptr->gc,gray);
		XFillRectangle(display,treeptr->win,treeptr->gc,
			0,0,treeptr->aw,treeptr->ah);
	}
	if(treeptr->c==black||treeptr->c==darkgray)
		XSetForeground(display,treeptr->gc,white); 
	else
		XSetForeground(display,treeptr->gc,black); 

	switch(abs(treeptr->s)) {

		case left:
			XDrawString(display,treeptr->win,treeptr->gc,
				4,
				treeptr->ah/2+4,
				treeptr->b,strlen(treeptr->b));
			break;

		case right:
			XDrawString(display,treeptr->win,treeptr->gc,
				(treeptr->aw-XTextWidth(treeptr->font,treeptr->b,
				strlen(treeptr->b)))-4,
				treeptr->ah/2+4,treeptr->b,strlen(treeptr->b));
			break;

		default:
			XDrawString(display,treeptr->win,treeptr->gc,
				(treeptr->aw-XTextWidth(treeptr->font,treeptr->b,
				strlen(treeptr->b)))/2,
				treeptr->ah/2+4,treeptr->b,strlen(treeptr->b));
			break;
	}
}

void label_broadcast(struct xtree *treeptr) { 

	if(treeptr->k==strcheck(treeptr->b)) return;
	label_expose(treeptr);
	treeptr->k=strcheck(treeptr->b);
}

struct xtree *label_create(int x,int y,int w,int h,char *b,int c,int s) {

	struct xtree *treeaux;

	treeaux=(struct xtree *)malloc(sizeof(struct xtree));
	memset(treeaux,0,sizeof(struct xtree));

	treeaux->x=x;
	treeaux->y=y;
	treeaux->w=w;
	treeaux->h=h;
	treeaux->b=b;
	treeaux->c=c;
	treeaux->s=s;
	treeaux->expose=label_expose;
	treeaux->broadcast=label_broadcast;
	treeaux->parent=treestk;

	treeaux->win=XCreateSimpleWindow(display,treestk->win,
		get_xywh(),0,gray,c!=invisible?c:gray);

	treeaux->gc=XCreateGC(display,treeaux->win,0,&values);
	XSelectInput(display,treeaux->win,ExposureMask);

	if(c==black||c==darkgray)
		XSetFont(display,treeaux->gc,(treeaux->font=helvetica12b)->fid);
	else
		XSetFont(display,treeaux->gc,(treeaux->font=defaultfont)->fid);

	XSetLineAttributes(display,treeaux->gc,0,LineSolid,CapRound,JoinRound);
	XMapWindow(display,treeaux->win);
	treeaux->next=treestk->treestk;
	treestk->treestk=treeaux;

	return(treeaux);
}
