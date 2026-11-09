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

char 	buffer[64], 
	*list[15]={

		"Linux",
		"FreeBSD",
		"NetBSD",
		"OpenBSD", 
		"NeXTSTEP",
		"UnixWare",
		"Solaris",
		"SunOS",
		"Irix", 
		"Minix",
		"Ultrix",
		"AIX",
		"A/UX",
		"System7",
		"OpenVMS",
	};

struct mlist biglist[]={
        { "list 1",list,100  },
        { "list 2",list,100  },
        { "list 3",list,100  },
        { "list 4",list,100  },
};

int listptr,listmax=15;

void function(struct xtree *treeptr) {

	strcpy(buffer,list[listptr]);
}

void dclick(struct xtree *treeptr) {

	sprintf(buffer,
		"item %s doubleclicked!\n",
		list[listptr]);
}

void menuf(struct xtree *treeptr) {

	switch(treeptr->l) {
	
		case 9:
			exit(0);
			break;
		default:
			sprintf(buffer,
				"item %d selected!\n",
				treeptr->l);
			break;
	}
}

void menu(struct xtree *treeptr) {

        char static *z[10]={
                "Linux",
                "FreeBSD",
                "NetBSD",
                "OpenBSD",
                "NeXTSTEP",
                "UnixWare",
                "Solaris",
                "SunOS",
                "Irix",
                "Quit",
        };

        menu_create(0,0,120,10*21,z,0,menuf);
}

void xmain(int n,char *p[]) {

	window_create(0,0,400,180,"mscroll_create");

	mscroll_create(	8,8,-8,-(40+21), 	/* position and size */
			&listptr,	/* list pointer */
			&listmax,  	/* list size */
			biglist,	/* list struct */
			function,	/* list pointer function */
			dclick,		/* doubleclick function */
			menu,		/* right button menu */
			4);		/* lists inside list struct */

	label_create(8,-40,-8,21,buffer,darkgray,center);
	button_create(-8,-8,72,24,"close",window_close);
}
