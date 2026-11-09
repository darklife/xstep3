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

/* macros to port XSTEP 2.0 applications to XSTEP 3.3 */

#define font 				defaultfont
#define xsteptree 			xtree
#define xstepwin			xtree
#define make_button			button_create
#define make_window			window_create
#define make_edit(X,Y,W,H,T,B,S); 	edit_create(X,Y,W,H,B,S,0); \
				  	label_create((X)-50,Y,50,H,T, \
				  		invisible,right);
#define make_list(X,Y,W,H,L,P,M,F);	scroll_create(X,Y,W,H,P,M,L,F);
#define make_radio(X,Y,T,B);		radio_create(X,Y,100,21,T,B);
#define make_check(X,Y,T,B);		check_create(X,Y,100,21,T,B);
#define make_text(X,Y,W,H,B,F,Q);	label_create(X,Y,W,H,B,darkgray,Q); \
					if(F) { \
						label_create(0,0,1,1,"",invisible,left); \
						treestk->treestk->broadcast=F; \
						animate=1; \
					}
#define close_window			window_close

/* macros to port XSTEP 3.0 applications to XSTEP 3.3 */

void xmain(int argc,char **argv) {

	animate=1;
	xapplication(NULL);
}

void xinit(char *app) {

	application=app;
}

#define main				unused
#define xmain				xapplication
