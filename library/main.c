/*

XSTEP 3.4 - Toolkit for X-Window System
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

#include <unistd.h>
#include "xstep.h"

#ifdef TREEWINDOW

	#include "tree.c"

#endif

int main(int n,char *p[]) {

	int 	i,realtime=0;

	argv=p; argc=n;
	application=p[0];
	
	for(i=0;i!=n;i++) {

		if(!strcmp(p[i],"-display")&&(i+1)<n) if(!(display=XOpenDisplay(p[i+1]))) return(-1);
		if(!strcmp(p[i],"-wid")&&(i+1)<n) rootwindow=(Window)strtoul(p[i+1],NULL,0);
		if(!strcmp(p[i],"-rt")) realtime=1;
	}
	
	if(!display) if(!(display=XOpenDisplay(displayname))) return(-1);
	screen=DefaultScreen(display);

	helvetica12m=XLoadQueryFont(display,	"*helveti*med*-r*12*");
	helvetica24m=XLoadQueryFont(display,	"*helveti*med*-r*24*");
	helvetica12b=XLoadQueryFont(display,	"*helveti*bol*-r*12*");
	helvetica24b=XLoadQueryFont(display,	"*helveti*bol*-r*24*");
	courier12m=XLoadQueryFont(display,	"*courier*med*-r*12*");
	courier12b=XLoadQueryFont(display,	"*courier*bol*-r*12*");

	defaultfont=helvetica12m;
	
	white=getcolor(0xffff,0xffff,0xffff);
	gray=getcolor(0xaeba,0xaaaa,0xaeba);
	darkgray=getcolor(0x5144,0x5555,0x5144);
	black=getcolor(0x0000,0x0000,0x0000);
/*
	white=getcolor(0,0xffff,0xffff);
	gray=getcolor(0,0xaaaa,0xaeba);
	darkgray=getcolor(0,0x5555,0x5144);
	black=getcolor(0x0000,0x0000,0x0000);
*/
	invisible=-1;
	atom=XInternAtom(display,"WM_DELETE_WINDOW",False);

	if(!rootwindow) rootwindow=RootWindow(display,screen);

	xmain(n,p);

#ifdef TREEWINDOW

	tree();

#endif

	for(i=0;winstk;i++) {

		if((broadcast=animate)) {

			while(XPending(display)) {

				XNextEvent(display,&report);
				xscan(winstk);
			}
			if(!realtime) usleep(1);
			
		} else {

			XNextEvent(display,&report);
			xscan(winstk);
		}
		while(broadcast--) xbroad(winstk);
	}
	return(0);
}
