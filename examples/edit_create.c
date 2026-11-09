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

char buffer1[64],buffer2[64];

void function(struct xtree *t) {

	if(atof(t->b)) 
		sprintf(t->b,"US$ %.02f",atof(t->b));

	broadcast++;
}

void xmain(int n,char *p[]) {

	window_create(0,0,400,180,"edit_create");
	edit_create(8,8,200,21,buffer1,64,function);
	edit_create(8,8+24,200,21,buffer2,64,function);
	button_create(-8,-8,72,24,"close",window_close);
}
