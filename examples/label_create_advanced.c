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

char 	text[256];
int	timestamp;
 
void function(struct xtree *t) {

	if(timestamp==time(0)) return;
	
	timestamp=time(0);

	strcpy(text,ctime((time_t  *)&timestamp));
	broadcast++;
}

void xmain(int n,char *p[]) {

	animate=1;

	window_create(0,0,400,16+14*24+21+40-3,"label_create_advanced");
	treestk->broadcast=function;

	label_create(8,8+0*24,-8,21,text,invisible,	left);
	label_create(8,8+1*24,-8,21,text,white,		left);
	label_create(8,8+2*24,-8,21,text,gray,		left);
	label_create(8,8+3*24,-8,21,text,darkgray,	left);
	label_create(8,8+4*24,-8,21,text,black,		left);

	label_create(8,8+5*24,-8,21,text,invisible,	center);
	label_create(8,8+6*24,-8,21,text,white,		center);
	label_create(8,8+7*24,-8,21,text,gray,		center);
	label_create(8,8+8*24,-8,21,text,darkgray,	center);
	label_create(8,8+9*24,-8,21,text,black,		center);

	label_create(8,8+10*24,-8,21,text,invisible,	right);
	label_create(8,8+11*24,-8,21,text,white,	right);
	label_create(8,8+12*24,-8,21,text,gray,		right);
	label_create(8,8+13*24,-8,21,text,darkgray,	right);
	label_create(8,8+14*24,-8,21,text,black,	right);

	button_create(-8,-8,72,24,"close",window_close);
}
