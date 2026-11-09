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

void xmain(int n,char *p[]) {

	dialog_create(400,180);

        defaultfont=helvetica24b;
        label_create(0,0,0,-90,"Warning, please read!",gray,up);
        defaultfont=helvetica12m;

        label_create(0,-90,0,0,"",gray,up);
        label_create(0,-(24+16),0,40,
                "This is a dialog box, click OK to close.",
                invisible,center);

        button_create( -8,-8,72,24,"OK",      window_close);	
}
