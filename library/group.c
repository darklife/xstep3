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

void group_end(void) {

	treestk=treestk->parent;
}

struct xtree *group_begin(int x,int y,int w,int h,int c) {

	struct xtree *treeaux;

	treeaux=(struct xtree *)malloc(sizeof(struct xtree));
	memset(treeaux,0,sizeof(struct xtree));

	treeaux->x=x;
	treeaux->y=y;
	treeaux->w=w;
	treeaux->h=h;
	treeaux->c=c;
	treeaux->parent=treestk;
	treeaux->configure=reconfigure;

	treeaux->win=XCreateSimpleWindow(display,treestk->win,
		get_xywh(),0,black,c);

	treeaux->gc=XCreateGC(display,treeaux->win,0,&values);

        XSelectInput(display,treeaux->win,StructureNotifyMask|ExposureMask);
	XSetFont(display,treeaux->gc,(treeaux->font=defaultfont)->fid);
	XSetLineAttributes(display,treeaux->gc,0,LineSolid,CapRound,JoinRound);
	XMapWindow(display,treeaux->win);
	treeaux->next=treestk->treestk;
	treestk->treestk=treeaux;
	treestk=treeaux;

	return(treeaux);
}

