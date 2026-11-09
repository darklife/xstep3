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

char text[256];
 
void createf(struct xtree *treeptr) {

	sprintf(text,"%d clicks!",atoi(text)+1);
	broadcast++;
}

void xmain(int n,char *p[]) {

	window_create(0,0,400,180,"label_create");

	defaultfont=helvetica24b;
	label_create(0,0,0,-90,text,gray,up);
	defaultfont=helvetica12m;

	label_create(0,-90,0,0,"",gray,up);
	button_create(-88,-8,72,24,"create",	createf);
	button_create( -8,-8,72,24,"close",	window_close);
}
