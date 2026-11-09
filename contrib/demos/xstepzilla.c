/*

XSTEP 3.2 - Toolkit for X-Window System
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

#include <xstep.h>
#include "mozilla.xpm"

#define LINEMAX 1000000

char 	*line[LINEMAX];
int 	linemax,lineptr,htmlenable=1,space=16;

void text_expose(struct xtree *t) {

	int i,x,y,fontwidth,fontheight;
	char *p,*q,c;
	XFontStruct *selectfont;
/*
	if(t->k==t->parent->aw*16777216+t->parent->ah*65536+t->y*256+t->x+1) return;
	t->k=t->parent->aw*16777216+t->parent->ah*65536+t->y*256+t->x+1;
*/
	selectfont=defaultfont;
/*
	XSetForeground(display,t->gc,gray);
	XDrawRectangle(display,t->win,t->gc,72,72,t->aw-space*2,t->ah-space*2);
	XSetForeground(display,t->gc,black);
	XDrawString(display,t->win,t->gc,
		t->aw-72-XTextWidth(selectfont,"Page 1",strlen("Page 1")),
		72-4,
		"Page 1",strlen("Page 1"));
*/
	XSetForeground(display,t->gc,black);
	y=space+20;
	x=space;

	for(c=i=0;i!=linemax;i++) {

		if(!htmlenable&&i) {
		
			x=space;
			y+=fontheight;
		}

		fontwidth=XTextWidth(selectfont," ",1);
		fontheight=14;

		q=p=line[i];
		
		while(*q) {

			for(p=q;*p&&*p<=32;p++) {
			
				if(*p=='\t') x+=fontwidth*8;
			}
			if(htmlenable) {

				if(*p=='<') {
				
					for(q=p;*q&&*q!='>';q++);
					q++;
					
					if(!strncmp("<h1",p,3)) {
				
						selectfont=helvetica24b;
						XSetFont(display,t->gc,selectfont->fid);
					}
					if(!strncmp("</h1>",p,5)) {

						x=space;
						y+=fontheight*2;					
						selectfont=defaultfont;
						XSetFont(display,t->gc,selectfont->fid);
					}
					if(!strncmp("<b>",p,3)) {
				
						selectfont=helvetica12b;
						XSetFont(display,t->gc,selectfont->fid);
					}
					if(!strncmp("</b>",p,4)) {
					
						selectfont=defaultfont;
						XSetFont(display,t->gc,selectfont->fid);
					}
	
					if(!strncmp("<a",p,2)) {
					
						XSetForeground(display,t->gc,getcolor(0,0,-1));
					}
					if(!strncmp("</a>",p,4)) {
					
						XSetForeground(display,t->gc,black);
					}
					if(!strncmp("<blockquote>",p,12)) {
					
						space+=72/4;
						x=space;
						y+=fontheight*2;
					}
					if(!strncmp("</blockquote>",p,13)) {
					
						space-=72/4;
						x=space;
						y+=fontheight*2;
					}
					if(!strncmp("<p>",p,3)) {
					
						x=space;
						y+=fontheight*2;
					}
					if(!strncmp("<br>",p,4)) {
				
						x=space;
						y+=fontheight;
					}
					if(!strncmp("<li>",p,4)) {
				
						x=space;
						y+=fontheight;
					}
					continue;
				
				} else for(q=p;*q&&*q>32&&*q!='<';q++);
			} else for(q=p;*q&&*q>32;q++);

			if(!(q-p)) continue;
			if(x+XTextWidth(selectfont,p,q-p)>t->aw-space) {
			
				x=space;
				y+=fontheight;
			}

			XDrawString(display,t->win,t->gc,
					x,
					y,
					p,
					q-p);
					
			x+=XTextWidth(selectfont,p,q-p)+fontwidth;
			c+=q-p+1;
		}

	}
}

void crash(struct xtree *t) {

        dialog_create(400,180);

        defaultfont=helvetica24b;
        label_create(0,0,0,-90,"General Protection Fault!",gray,up);
        defaultfont=helvetica12m;

        label_create(0,-90,0,0,"",gray,up);
        label_create(0,-(24+16),0,40,
                "Detected a illegal instruction in the module (null).",
                invisible,center);

        button_create( -8,-8,72,24,"OK",      window_close);
}

void xmain(int i,char *p[]) {

	FILE *f;
	int a,b;
	char buffer[1024];
	struct xtree *t;

	t=window_create(0,0,600,600,"xstepzilla");

	for(a=1;a!=i;a++) {

		if(!strcmp(p[a],"-text")) {
		
			htmlenable=0;
			continue;
		}

		f=fopen(p[a],"r");
		b=linemax;
		while(fgets(buffer,1024,f)) {

			line[linemax]=(char *)malloc(1+strlen(buffer));
			strcpy(line[linemax],buffer);
			linemax++;
		}
		fclose(f);
	}
	t=xyscroll_create(8,8+8+72,-8,0,text_expose,white,0,16*linemax);

	t=button_create(8,8,72,72,"bugzilla!",crash);
	mkpixmap(mozilla,t);
	t=button_create(8+80,8,72,72,"bugzilla!",crash);
	mkpixmap(mozilla,t);
	t=button_create(8+160,8,72,72,"bugzilla!",crash);
	mkpixmap(mozilla,t);
}
