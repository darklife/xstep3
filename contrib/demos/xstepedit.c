/*
 
XSTEP 3.2.1 - Toolkit for X-Window System
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

--

Application Notes:

- This is the code for a small text editor for X and
  demonstrate how intercept events with XSTEP (with treestk).

- The edition of text is limited to 100000 lines. All
  tabs are converted to spaces to edition and reconverted
  to tabs to save (of course, eight spaces are one tab :)

- All original 'files' are renamed to 'files~'

- By default, this editor have support for international 
  characters, just like any XSTEP application.

- You can edit from stdin and save to your stdout! any
  like:
  
	  zcat /var/log/messages.*.gz | 
	  grep sendmail | 
	  xedit > messages.sendmail.txt'

- You can concatenate many files, edit and save to stdout! :))

*/

#define MAXLINES 1000000

#include <xstep.h>

char	 *stack[MAXLINES],buffer[1024],*filename;
int	 stackmax,stackptr,modify;

void scrollf(struct xtree *t) {

	strcpy(buffer,stack[stackptr]);
	broadcast++;
}

void updatef(void) {

	free(stack[stackptr]);
	stack[stackptr]=(char *)malloc(1+strlen(buffer));
	strcpy(stack[stackptr],buffer);
	broadcast++;
}

void keyboardf(struct xtree *t) {

	int k,i;
	void closewindowf(struct xtree *t);
	
	k=XLookupKeysym((XKeyEvent *)&report,0);

	switch(k) {

		case key_escape:
			closewindowf(t);
			break;

		case key_page_down:
		
			updatef();
			stackptr+=25;
			if(stackptr>=stackmax) stackmax?stackptr=stackmax-1:0;
			broadcast++;
			break;

		case key_page_up:

			updatef();
			stackptr-=25;
			if(stackptr<0) stackptr=0;
			broadcast++;
			break;
	
		case key_arrow_up:

			updatef();
			if(stackptr) stackptr--;
			broadcast++;
			break;

		case key_arrow_down:

			updatef();
			if(stackptr!=stackmax) stackptr++;
			broadcast++;
			break;

		case key_delete:
		
			if(!strlen(buffer)&&stackmax) {
			
				free(stack[stackptr]);
				
				for(	i=stackptr;
					i!=stackmax&&i!=MAXLINES;
					i++) 
						stack[i]=stack[i+1];
				stackmax--;
				broadcast++;
				modify=1;
			}
			break;

		case key_return:

			updatef();

			for(	i=stackmax;
				i!=stackptr&&i!=MAXLINES;
				i--) 
					stack[i+1]=stack[i];
			stackmax++;

			stackptr++;

			stack[stackptr]=(char *)malloc(1);
			strcpy(stack[stackptr],"");

			broadcast++;
			modify=1;
			break;
		
		default:
		
			printf("keysym %d (%c)\n",k,(k>=32)?(char)k:'*');
			modify=1;
	}
	edit_keypress(t);
}

void quitf(struct xtree *t) {

	exit(0);
}

void savef(struct xtree *t) {

	FILE *f;
	int i,j;

	if(filename) {

		sprintf(buffer,"%s~",filename);
		rename(filename,buffer);
		f=fopen(filename,"w");

	} else f=stdout;
	
	for(i=0;i!=stackmax;i++) {

		for(j=0;*(stack[i]+j);j++)
		
			if(!strncmp("        ",stack[i]+j,8)) {
			
				putc('\t',f);
				j+=7;
				
			} else putc(*(stack[i]+j),f);

		putc('\n',f);
	}
	fclose(f);
	exit(0);
}

void closewindowf(struct xtree *t) {

	if(!modify) exit(0);

	dialogbox_create(400,180);

	label_create(0,0,0,90,"Warning!",gray,up);
	XSetFont(display,treestk->treestk->gc,
		(treestk->treestk->font=helvetica24b)->fid);
	label_create(0,90,0,0,"",gray,up);

	label_create(0,92,0,-(24+18),
		"Save this file before exit?",
		invisible,center);

	button_create(-(8+80*1),-8,72,24,"Yes",	savef);
	button_create(-(8+80*0),-8,72,24,"No",	 quitf);
}

void xmain(int c,char **p) {

	FILE *f=stdin;
	int i,n;

	if(c==2) filename=p[1];

	for(i=1;i!=c;i++) {
	
		f=fopen(p[1],"r");

		for( ;fgets(buffer,1024,f)&&stackmax!=MAXLINES;stackmax++) {

			for(n=0;buffer[n];n++) {
			
				if(buffer[n]=='\n') buffer[n]=0;
				if(buffer[n]=='\t') {

					memmove(buffer+n+8,buffer+n+1,strlen(buffer+n+1));
					strncpy(buffer+n,"        ",8);
				}
			}
			stack[stackmax]=(char *)malloc(1+strlen(buffer));
			strcpy(stack[stackmax],buffer);
		}
		fclose(f);
	}

	window_create(16,16,512,512,"xedit");

	/*
		treestk is a pointer to the last window in the stack 
		of windows and treestk->message by default is a pointer
		to function window_close(). you can intercept a 
		window_close() and run any function before this window
		is closed! :))	
	*/

	treestk->message=closewindowf;

	scroll_create(0,0,-1,-21,&stackptr,&stackmax,stack,scrollf);
	edit_create(0,-1,-1,21,buffer,1024,0);

	/*
		treestk is a pointer to the last window in the stack
		of windows and treestk->treestk is a pointer to the
		last widget of the last window :) treestk->treestk
		is like treestk and have many pointers to functions.
		
		treestk->treestk->keypress, for example, is by default
		a pointer to edit_keypress() in editbox widgets. you
		can intercept the keypress events and run any function
		before edit_keypress() :))
	*/

	treestk->treestk->keypress=keyboardf;

/*	this code don't work in many window managers! :)*/
/*	
	XSetInputFocus(display,
		treestk->treestk->win,
		RevertToNone,CurrentTime);	
*/
}
