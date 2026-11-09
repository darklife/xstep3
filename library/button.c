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
 
void button_expose(struct xtree *treeptr) {

	upbox(0,0,treeptr->aw,treeptr->ah,gray,treeptr);

        if(treeptr->pix) {

                XCopyArea(display,treeptr->pix,treeptr->win,treeptr->gc,0,0,
                        treeptr->wpix,treeptr->hpix,
                        (treeptr->aw-treeptr->wpix)/2,(treeptr->ah-treeptr->hpix-14)/2);

		XDrawString(display,treeptr->win,treeptr->gc,
			(treeptr->aw-
			XTextWidth(treeptr->font,treeptr->t,strlen(treeptr->t)))/2,
			treeptr->ah/2+4+(treeptr->hpix)/2,treeptr->t,strlen(treeptr->t));
	} else

		XDrawString(display,treeptr->win,treeptr->gc,
			(treeptr->aw-
			XTextWidth(treeptr->font,treeptr->t,strlen(treeptr->t)))/2,
			treeptr->ah/2+4,treeptr->t,strlen(treeptr->t));

}

void button_buttonpress(struct xtree *treeptr) {

	treeptr->k=1;

	downbox(0,0,treeptr->aw,treeptr->ah,white,treeptr);

        if(treeptr->pix) {

                XCopyArea(display,treeptr->pix,treeptr->win,treeptr->gc,0,0,
                        treeptr->wpix,treeptr->hpix,
                        1+(treeptr->aw-treeptr->wpix)/2,1+(treeptr->ah-treeptr->hpix-14)/2);

		XDrawString(display,treeptr->win,treeptr->gc,
			1+(treeptr->aw-
			XTextWidth(treeptr->font,treeptr->t,strlen(treeptr->t)))/2,
			1+treeptr->ah/2+4+(treeptr->hpix)/2,treeptr->t,strlen(treeptr->t));
	} else
		XDrawString(display,treeptr->win,treeptr->gc,
			1+(treeptr->aw-
			XTextWidth(treeptr->font,treeptr->t,strlen(treeptr->t)))/2,
			1+treeptr->ah/2+4,treeptr->t,strlen(treeptr->t));
		XFlush(display);
}

void button_buttonrelease(struct xtree *treeptr) {

	Window wintmp;

	if(!treeptr->k) return;
	treeptr->k=0;

	if(checkbox(0,0,treeptr->aw,treeptr->ah)) {

		XDefineCursor(display,wintmp=treestk->win,
		XCreateFontCursor(display,XC_watch));
		XFlush(display);
		if(treeptr->f) treeptr->f(treeptr);
		XDefineCursor(display,wintmp,
			XCreateFontCursor(display,XC_left_ptr));
	}
	button_expose(treeptr);
}

struct xtree *button_create(int x,int y,int w,int h,char *t,void (*f)(struct xtree *)) {

	struct xtree *treeaux;

	treeaux=(struct xtree *)malloc(sizeof(struct xtree));
	memset(treeaux,0,sizeof(struct xtree));

	treeaux->x=x; 
	treeaux->y=y;
	treeaux->w=w;
	treeaux->h=h;
	treeaux->t=t;
	treeaux->f=f;
	treeaux->expose=button_expose;
	treeaux->buttonpress=button_buttonpress;
	treeaux->buttonrelease=button_buttonrelease;
	treeaux->parent=treestk;

	treeaux->win=XCreateSimpleWindow(display,treestk->win,
		get_xywh(),0,black,gray);

	treeaux->gc=XCreateGC(display,treeaux->win,0,&values);	

	XSelectInput(display,treeaux->win,
		ExposureMask|
		ButtonPressMask|
		ButtonReleaseMask);

	XSetFont(display,treeaux->gc,(treeaux->font=defaultfont)->fid);
	XSetLineAttributes(display,treeaux->gc,0,LineSolid,CapRound,JoinRound);
	XMapWindow(display,treeaux->win);
	treeaux->next=treestk->treestk;
	treestk->treestk=treeaux;

	return(treeaux);
}
