/*
 
XSTEP 3.3 - Toolkit for X-Window System
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

void treedestroy(struct xtree *treeptr) {

	struct xtree *treeaux,*treenxt;

	for(treeaux=treeptr->treestk;
	    treeaux;
	    treeaux=treenxt) {

		if(treeaux->treestk) treedestroy(treeaux->treestk);
		treenxt=treeaux->next;
		free(treeaux);
	}
}

void window_delete(struct xtree *treeptr) {

	struct xtree *treeaux;

	XDestroyWindow(display,treeptr->win);
	
	if(treeptr!=winstk) {

		for(treeaux=winstk;
		    treeaux->next!=treeptr;
		    treeaux=treeaux->next);

		treeaux->next=treeptr->next;

	} else treestk=winstk=treeptr->next;

	treedestroy(treeptr);
	free(treeptr);
}

void window_close(struct xtree *treeptr) {

	struct xtree *treeaux;

	for(treeaux=treeptr;treeaux->parent;treeaux=treeaux->parent);

	treeaux->todelete=treeaux->win;
	broadcast++;
}

struct xtree *window_create(int x,int y,int w,int h,char *t) {

	XClassHint classhint;
	XWMHints *hints;
	XSizeHints *shints;
	struct xtree *winaux;

	winaux=(struct xtree *)malloc(sizeof(struct xtree));
	memset(winaux,0,sizeof(struct xtree));

	winaux->x=x;
	winaux->y=y;
	winaux->w=winaux->aw=w;
	winaux->h=winaux->ah=h;
	winaux->t=t;

	winaux->message=window_delete;
	winaux->configure=reconfigure;

	if(rootwindow==RootWindow(display,screen))
		winaux->win=XCreateSimpleWindow(display,rootwindow,x,y,w,h,0,black,gray);
	else
		winaux->win=rootwindow;

	classhint.res_name = application;
	classhint.res_class = application;
	XSetClassHint(display,winaux->win,&classhint);

	if(t) {
		XSetIconName(display,winaux->win,t);
		XSetWMProtocols(display,winaux->win,&atom,1);
		XStoreName(display,winaux->win,t);
	} else {

		if(!treestk) 	XSetTransientForHint(display,winaux->win,rootwindow);
		else 		XSetTransientForHint(display,winaux->win,treestk->win);

		XSetWMProtocols(display,winaux->win,&atom,0);
		shints=XAllocSizeHints();
		shints->x=x;
		shints->y=y;
		shints->min_width=shints->max_width=w;
		shints->min_height=shints->max_height=h;
		shints->flags=PMinSize|PMaxSize|PPosition;
		XSetWMNormalHints(display,winaux->win,shints);
		XStoreName(display,winaux->win,"");
	}
	if(!treestk) {

		XSetCommand(display,winaux->win,argv,argc);
		hints = XAllocWMHints();
		hints->window_group = winaux->win;
		hints->flags = WindowGroupHint;
		XSetWMHints(display,winaux->win,hints);
	}
        winaux->gc=XCreateGC(display,winaux->win,0,&values);
        XSetFont(display,winaux->gc,(winaux->font=defaultfont)->fid);
        XSetLineAttributes(display,winaux->gc,0,LineSolid,CapRound,JoinRound);
        XSelectInput(display,winaux->win,StructureNotifyMask);
	XDefineCursor(display,winaux->win,
	XCreateFontCursor(display,XC_left_ptr));
	XMapWindow(display,winaux->win);

	winaux->next=winstk;
	winstk=winaux;
	treestk=winaux;

	return(winaux);
}

struct xtree *toolbox_create(int x,int y,int w,int h) {

	XSetWindowAttributes attributes;
	struct xtree *treeaux;

	treeaux=(struct xtree *)malloc(sizeof(struct xtree));
	memset(treeaux,0,sizeof(struct xtree));

	treeaux->x=x;
	treeaux->y=y;
	treeaux->aw=w;
	treeaux->ah=h;

	treeaux->configure=reconfigure;

	attributes.save_under=True;
	attributes.override_redirect=True;

	treeaux->win=XCreateWindow(display,rootwindow,
		x,y,w,h,0,
		CopyFromParent,
		InputOutput,
		CopyFromParent,
		CWSaveUnder|CWOverrideRedirect,
		&attributes);

	treeaux->gc=XCreateGC(display,treeaux->win,0,&values);
	XSetFont(display,treeaux->gc,helvetica12m->fid);
	XSetLineAttributes(display,treeaux->gc,0,LineSolid,CapRound,JoinRound);
	XDefineCursor(display,treeaux->win,XCreateFontCursor(display,XC_left_ptr));
	XMapWindow(display,treeaux->win);
        XSelectInput(display,treeaux->win,StructureNotifyMask);

	treeaux->next=winstk;
	winstk=treeaux;
	treestk=treeaux;

	return(treeaux);
}
