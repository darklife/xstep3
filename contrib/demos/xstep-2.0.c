/*
XSTEP 3.3 - Toolkit for X-Window System
Copyright (C) 1996,1997,1998,1999 by Marcelo Samsoniuk

Xstep 2.00 - Toolkit for Graphic Applications in X11
Copyright (C) 1996,1997 Marcelo Samsoniuk

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

	This application is the demonstration of the old XSTEP 2.0 
	changed to work with new library of XSTEP 3.3, with minimal
	modifications in the source code.

	The default xstep.h header was replaced by the xstep-port.h
	header with macros to translate the old sintax of XSTEP 2.0
	to new sintax of XSTEP 3.3 and this old application was 
	recycled to work with new widgets:

		#include <xstep.h>
		
	replaced by:
		
		#include <xstep-port.h>

	In XSTEP 2.0 the treeptr pointer to the caller widget
	in any callbacked function. In XSTEP 3.0 this was changed
	and the treeptr pointer don't exist! :)

	Of course, the 'k[*(treeptr->p)]' is a error in XSTEP 3.0
	and XSTEP 2.0, the correct code is really 'k[kptr]' in
	both versions:

		void listprock(void) { strcpy(database.name,k[*(treeptr->p)]); }
		void listprocw(void) { strcpy(database.address,w[*(treeptr->p)]); }
	
	replaced by:

		void listprock(void) { strcpy(database.name,k[kptr]); }
		void listprocw(void) { strcpy(database.address,w[wptr]); }
*/

#include <xstep-port.h>

struct {

	char 	name	[50];
	char 	address	[50];
	char 	city	[20];
	char	uf	[ 3];
	char	zip	[ 9];
	char 	country	[20];
	char 	phone	[16];
	char	fax	[16];
	char 	mail	[50];
	char 	url	[50];

} database;

char *k[14]={

	"NeXTSTEP",
	"Linux",
	"FreeBSD",
	"AIX",
	"SCO UNIX",
	"4.4 BSD",
	"Plan 9",
	"Mach",
	"Windows 95",
	"Windows NT",
	"OS/2 Warp",
	"MacOS",
	"Minix",
	"Solaris",

}; int ksize=14,kptr;

char *w[9]={

	"PowerPC",
	"Intel x86",
	"Motorola 68k",
	"Sparc",
	"MIPS Rx000",
	"DEC ALpha",	
	"HP PARISC",
	"ColdFire",
	"Motorola 88k",

}; 

int wsize=8,wptr; char calcbuffer[16];

void listprock(void) { strcpy(database.name,k[kptr]); }
void listprocw(void) { strcpy(database.address,w[wptr]); }

void calcule(void) { 

	sprintf(calcbuffer,"US$ %.02f",
		(float)(database.name[0]+
			database.address[0]+
			database.city[0])); 
}

void xmain(void) {

	make_window(16,16,608,440,		"Xstep Toolkit");
	
	make_button(396,16+0*24,180,21,		"New Register",		xmain);
	make_button(396,16+1*24,180,21,		"Save Register",	0);
	make_button(396,16+2*24,180,21,		"Search Register",	0);
	make_button(396,16+3*24,180,21,		"Print Register",	0);
	make_button(396,16+4*24,180,21,		"Refresh Register",	0);
	make_button(396,16+5*24,180,21,		"Quit Demo",		close_window);

	make_radio(396,16+7*24,"Curitiba",		database.city);
	make_radio(396,16+8*24,"S\343o Paulo",		database.city);
	make_radio(396,16+9*24,"Brasilia",		database.city);	
	make_radio(396,16+10*24,"Rio de Janeiro",	database.city);	
	make_radio(396,16+11*24,"Fortaleza",		database.city);	

	make_text(396,16+13*24,100,21,		database.city,0,1);
	make_text(396,16+14*24,100,21,		calcbuffer,calcule,2);
	
	make_text(16,16,180,21,			"Operating System",0,0);
	make_list(16,16+24,180,132,		k,&kptr,&ksize,listprock);
	make_text(200,16,180,21,		"Supported Plataforms",0,0);
	make_list(200,16+24,180,132,		w,&wptr,&wsize,listprocw);

	make_edit(80,16+ 7*24,300,21,"Name",	database.name,		50);
	make_edit(80,16+ 8*24,300,21,"Address", database.address,	50);
	make_edit(80,16+ 9*24,300,21,"City",	database.city,		20);
	make_edit(80,16+10*24,100,21,"State",	database.uf,		 3);
	make_edit(80,16+11*24,100,21,"ZIP",	database.zip,		 9);
	make_edit(80,16+12*24,100,21,"Country",	database.country,	20);
	make_edit(80,16+13*24,100,21,"Phone",	database.phone,		16);
	make_edit(80,16+14*24,100,21,"Fax",	database.fax,		16);
	make_edit(80,16+15*24,300,21,"E-Mail",	database.mail,		50);
	make_edit(80,16+16*24,300,21,"HomePage",database.url,		50);
}
