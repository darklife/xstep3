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

struct xtree *group1,*group2,*group3;

void group1f(struct xtree *t) { XRaiseWindow(display,group1->win); }
void group2f(struct xtree *t) { XRaiseWindow(display,group2->win); }
void group3f(struct xtree *t) { XRaiseWindow(display,group3->win); }

void xmain(int n,char *p[]) {

	window_create(0,0,400,180,"group");

	group1=group_begin(8,8,-(16+72),-8,darkgray);
		button_create(8,8,-8,-8,"group 1",0);
	group_end();

	group2=group_begin(8,8,-(16+72),-8,darkgray);
		button_create(8,8,-8,-8,"group 2",0);
	group_end();

	group3=group_begin(8,8,-(16+72),-8,darkgray);
		button_create(8,8,-8,-8,"group 3",0);
	group_end();

	button_create(-8,8+32*0,72,24,"group 1",	group1f);
	button_create(-8,8+32*1,72,24,"group 2",	group2f);
	button_create(-8,8+32*2,72,24,"group 3",	group3f);

	button_create(-8,-8,72,24,"close",	window_close);
}
