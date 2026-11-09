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

#include <xstep.h>

void draw(struct xtree *t) {

	if(!animate) return;

        XSetForeground(display,t->gc,rand()%256);
        XFillRectangle(display,t->win,t->gc,
                t->aw/2+rand()%t->aw-rand()%t->aw,
                t->ah/2+rand()%t->ah-rand()%t->ah,
                rand()%t->aw/4,rand()%t->ah/4);
}

void startf(struct xtree *t) { animate=1; }
void stopf (struct xtree *t) { animate=0; }

void xmain(int n,char *p[]) {

	struct xtree *win;

	win=window_create(0,0,400,180,"animation");
        win->broadcast=draw;

	button_create(-(2*80+8),-8,72,24,"start",	startf);
	button_create(-(1*80+8),-8,72,24,"stop",	stopf);
	button_create(-(0*80+8),-8,72,24,"close",	window_close);
}
