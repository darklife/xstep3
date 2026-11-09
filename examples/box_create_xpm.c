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

#include <xstep.h>
#include "mozilla.xpm"

void draw(struct xtree *treeptr) {

	XCopyArea(display,treeptr->pix,treeptr->win,treeptr->gc,0,0,
        	treeptr->wpix,treeptr->hpix,
		(treeptr->aw-treeptr->wpix)/2,(treeptr->ah-treeptr->hpix-14)/2);
}

void xmain(int n,char *p[]) {

	struct xtree *t;

	window_create(0,0,400,180,"box_create_xpm");

	t=box_create(8,8,64,64,draw,gray);
	mkpixmap(mozilla,t);

	button_create(-8,-8,72,24,"close",window_close);
}
