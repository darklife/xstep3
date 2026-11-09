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

#include "xstep.h"

int compose;

void drawcursor(struct xtree *treeptr,int c) {

	int s;

	if(treeptr->c>strlen(treeptr->b)) treeptr->c=strlen(treeptr->b);
	s=XTextWidth(helvetica12m,treeptr->b,treeptr->c);
	XSetForeground(display,treeptr->gc,c);
	XDrawLine(display,treeptr->win,treeptr->gc,4+s,4,4+s,16);
	if(*(treeptr->b+treeptr->c)) {
		XSetForeground(display,treeptr->gc,black);
		XDrawString(display,treeptr->win,treeptr->gc,4+s,14,treeptr->b+treeptr->c,1);
	}
}

void drawtext(struct xtree *treeptr,int c) {

	XSetForeground(display,treeptr->gc,c);
	XDrawString(display,treeptr->win,treeptr->gc,4,14,treeptr->b,strlen(treeptr->b));
	treeptr->k=strcheck(treeptr->b);
}

void edit_expose(struct xtree *treeptr) {

	if(treeptr->aw==1||treeptr->ah==1) return;
	downbox(0,0,treeptr->aw,treeptr->ah,white,treeptr);
	if(treeptr->p) drawcursor(treeptr,black);
	else drawcursor(treeptr,white);
	drawtext(treeptr,black);
}

void edit_broadcast(struct xtree *treeptr) {

	if(treeptr->k==strcheck(treeptr->b)) return;
	edit_expose(treeptr);
}

void edit_focusin(struct xtree *treeptr) {

	drawcursor(treeptr,black);
	drawtext(treeptr,black);
	treeptr->p=1;
	broadcast++;
}

void edit_focusout(struct xtree *treeptr) {

	drawcursor(treeptr,white);
	drawtext(treeptr,black);
	treeptr->p=0;
	broadcast++;
}

void edit_buttonpress(struct xtree *treeptr) {

	int i;
	char *buffer;

	switch(report.xbutton.button) {
	
		case 2:
 
			buffer=XFetchBuffer(display,&i,0);

			if(i) {

				strncpy(treeptr->b,buffer,(i<treeptr->s)?i:(treeptr->s-1));
				treeptr->b[i]='\0';
				broadcast++;
			}
			break;
		case 1:

			if((i=strlen(treeptr->b))) 
				XStoreBuffer(display,treeptr->b,i,0);
			break;
	}
	
	if(treeptr->p) drawcursor(treeptr,white);

	if(checkbox(0,0,treeptr->aw,treeptr->ah))
		for(treeptr->c=0;treeptr->c!=strlen(treeptr->b);treeptr->c++)
			if(4+XTextWidth(helvetica12m,treeptr->b,treeptr->c)>=report.xbutton.x)
				break;

	if(treeptr->p) {
		drawcursor(treeptr,black);
		drawtext(treeptr,black);
		return;
	}
	XSetInputFocus(display,treeptr->win,RevertToNone,CurrentTime);
}

void edit_keypress(struct xtree *treeptr) {

	int a,k,i=0;
	
	if(shift)     i++;

	if(treeptr->p) {

		k=XLookupKeysym((XKeyEvent *)&report,i);

		if(compose)k+=128;

#ifdef SHOWKEYSYM
		printf("%d:%d.%d, index=%d\n",k,k/256,k%256,i);
#endif

		switch(k) {

			case keypad_enter:
				k=key_return ; 
				break;

#ifdef KEYPADBUG
			case 65439: 		k='.'; break; 
			case 65438: 		k='0'; break; 
			case 65436: 		k='1'; break;  
			case 65433: 		k='2'; break;  
			case 65435: 		k='3'; break;  
			case 65430: 		k='4'; break;  
			case 65437: 		k='5'; break; 
			case 65432: 		k='6'; break; 
			case 65429: 		k='7'; break;  
			case 65431: 		k='8'; break; 
			case 65434: 		k='9'; break;
#endif
			case key_asterisk: 	k='*'; break;  
			case key_plus: 		k='+'; break; 
			case key_comma: 	k=','; break; 
			case key_hyphen: 	k='-'; break; 
			case key_decimal: 	k='.'; break; 
			case key_solidus: 	k='/'; break; 
			case key_zero: 		k='0'; break; 
			case key_one: 		k='1'; break;  
			case key_two: 		k='2'; break;  
			case key_thre: 		k='3'; break;  
			case key_four: 		k='4'; break;  
			case key_five: 		k='5'; break; 
			case key_six: 		k='6'; break; 
			case key_seven: 	k='7'; break;  
			case key_eight: 	k='8'; break; 
			case key_nine: 		k='9'; break;
			case key_equal: 	k='='; break;
		}

		switch(k) {
					
			case key_escape:

				XSetForeground(display,treeptr->gc,white);
				XFillRectangle(display,treeptr->win,treeptr->gc,
					0+2,0+2,treeptr->aw-4,treeptr->ah-4);
				for(a=0;a!=treeptr->s;a++) *(treeptr->b+a)=0;
				treeptr->c=0;
				drawcursor(treeptr,black);
				broadcast++;
				break;

			case key_delete:

				XSetForeground(display,treeptr->gc,white);
				XFillRectangle(display,treeptr->win,treeptr->gc,
					0+2,0+2,treeptr->aw-4,treeptr->ah-4);
				if(strlen(treeptr->c+treeptr->b))
					strcpy(treeptr->b+treeptr->c,treeptr->b+treeptr->c+1);

				drawcursor(treeptr,black);
				drawtext(treeptr,black);
				break;

			case key_backspace:

				XSetForeground(display,treeptr->gc,white);
				XFillRectangle(display,treeptr->win,treeptr->gc,
					0+2,0+2,treeptr->aw-4,treeptr->ah-4);
				if(treeptr->c) {
					treeptr->c--;
					strcpy(treeptr->b+treeptr->c,treeptr->b+treeptr->c+1);
				}
				drawcursor(treeptr,black);
				drawtext(treeptr,black);
				break;

			case key_arrow_left:
					
				drawcursor(treeptr,white);
				if(treeptr->c) treeptr->c--;
				drawcursor(treeptr,black);
				break;

			case key_arrow_right: 
					
				drawcursor(treeptr,white);
				if(treeptr->c<strlen(treeptr->b)) treeptr->c++;
				drawcursor(treeptr,black); 
				break;
						
			case key_arrow_up:
			key_shift_tab:

				if(treeptr->f) treeptr->f(treeptr);
				broadcast++;

				if(treeptr->next)
					if(treeptr->next->buttonpress==edit_buttonpress)
						treeptr->next->buttonpress(treeptr->next);
				break;

			case key_tab:

				if(shift) goto key_shift_tab;

			case key_arrow_down:
			case key_return:
			
				if(treeptr->f) 
					treeptr->f(treeptr);
				broadcast++;
				if(treeptr->last)
					if(treeptr->last->buttonpress==edit_buttonpress)
						treeptr->last->buttonpress(treeptr->last);
				break;
						
			case key_shift_left:
			case key_shift_right:

				shift=1; 
				break;

			case 0xffe7:
			case 0xffe8:
			case 0xffe9:
			case 0xffea:
			case 0xff20:

				compose=1; 
				break;

			case key_capslock:
	
				capslock=!capslock; 
				break;

			case key_home:

				drawcursor(treeptr,white);
				treeptr->c=0;
				drawcursor(treeptr,black);
				break;

			case key_end:

				drawcursor(treeptr,white);
				treeptr->c=strlen(treeptr->b);
				drawcursor(treeptr,black);
				break;

			default: 
					
				if(k>=256) break;
				if(strlen(treeptr->b)==(treeptr->s-1)) break;
 				if(treeptr->c==treeptr->s-1) break;
 				if((XTextWidth(helvetica12m,treeptr->b,strlen(treeptr->b))+
				    XTextWidth(helvetica12m,(char *)&k,1))>=(treeptr->aw-8)) break;
				drawcursor(treeptr,white);
 				drawtext(treeptr,white);

				if(treeptr->c>0) 
					switch(k) {

					case 'a':
						switch(treeptr->l) {
							case '`':  k=key_agrave; 	goto compose;
							case '\'': k=key_aacute; 	goto compose;
							case '^':  k=key_acircunflex; 	goto compose;
							case '~':  k=key_atilde; 	goto compose;
							case '"':  k=key_adiaeresis; 	goto compose;
						} break;
					case 'e':
						switch(treeptr->l) {
							case '`':  k=key_egrave; 	goto compose;
							case '\'': k=key_eacute; 	goto compose;
							case '^':  k=key_ecircunflex; 	goto compose;
							case '"':  k=key_ediaeresis; 	goto compose;								
						} break;
					case 'i':
						switch(treeptr->l) {
							case '`':  k=key_igrave; 	goto compose;
							case '\'': k=key_iacute; 	goto compose;
							case '^':  k=key_icircunflex; 	goto compose;
							case '"':  k=key_idiaeresis; 	goto compose;								
						} break;
					case 'o':
						switch(treeptr->l) {
							case '`':  k=key_ograve; 	goto compose;
							case '\'': k=key_oacute; 	goto compose;
							case '^':  k=key_ocircunflex; 	goto compose;
							case '~':  k=key_otilde; 	goto compose;
							case '"':  k=key_odiaeresis; 	goto compose;
						} break;
					case 'u':
						switch(treeptr->l) {
							case '`':  k=key_ugrave; 	goto compose;
							case '\'': k=key_uacute; 	goto compose;
							case '^':  k=key_ucircunflex; 	goto compose;
							case '"':  k=key_udiaeresis; 	goto compose;
						} break;;
					case 'c':
						switch(treeptr->l) {
							case '\'': k=key_cedila; 	goto compose;
						} break;;
					case 'n':
						switch(treeptr->l) {
							case '~':  k=key_ntilde; 	goto compose;
						} break;;
					case 'y':
						switch(treeptr->l) {
							case '\'': k=key_yacute; 	goto compose;
							case '"':  k=key_ydiaeresis; 	goto compose;
						} break;
					
					compose:
						treeptr->c--; 
						if(shift||capslock)
							k=k-('a'-'A');

					default: break;
						
				}
/*
				if(shift) switch(k) {
				
					case '`': k='~'; break;
					case '1': k='!'; break;
					case '2': k='@'; break;
					case '3': k='#'; break;
					case '4': k='$'; break;
					case '5': k='%'; break;
					case '6': k='^'; break;
					case '7': k='&'; break;
					case '8': k='*'; break;
					case '9': k='('; break;
					case '0': k=')'; break;
					case '-': k='_'; break;
					case '=': k='+'; break;
					case '[': k='{'; break;
					case ']': k='}'; break;
					case ';': k=':'; break;
					case '\'':k='\"';break;
					case ',': k='<'; break;
					case '.': k='>'; break;
					case '/': k='?'; break;
					case '\\':k='|'; break;
					case ' ': k=' '; break;
				}
*/
				if((capslock)&&k>='a'&&k<='z') k=k-('a'-'A');

				if(k<128)
					for(a=strlen(treeptr->b)+1;a!=treeptr->c;a--) 
						*(treeptr->b+a)=*(treeptr->b+a-1);

				*(treeptr->b+treeptr->c)=(char)(treeptr->l=k);
				treeptr->c++;
				drawtext(treeptr,black);
				drawcursor(treeptr,black);
				break;
		}
	}
}

void edit_keyrelease(struct xtree *treeptr) {

	int k;

	if(treeptr->p) {
		switch(k=XLookupKeysym((XKeyEvent *)&report,0)) {

			case key_shift_left: 
			case key_shift_right:	shift=0; break;

			case 0xffe7:
			case 0xffe8:
			case 0xffe9:
			case 0xffea:
			case 0xff20:		compose=0; break;
		}
	}
}

struct xtree *edit_create(int x,int y,int w,int h,char *b,int s,void (*f)(struct xtree *)) {

	struct xtree *treeaux;

	treeaux=(struct xtree *)malloc(sizeof(struct xtree));
	memset(treeaux,0,sizeof(struct xtree));

	treeaux->x=x;
	treeaux->y=y;
	treeaux->w=w;
	treeaux->h=h;
	treeaux->s=s;
	treeaux->b=b;
	treeaux->f=f;
	treeaux->expose=edit_expose;
	treeaux->keypress=edit_keypress;
	treeaux->keyrelease=edit_keyrelease;
	treeaux->buttonpress=edit_buttonpress;
	treeaux->focusin=edit_focusin;
	treeaux->focusout=edit_focusout;
	treeaux->broadcast=edit_broadcast;
	treeaux->parent=treestk;

	treeaux->win=XCreateSimpleWindow(display,treestk->win,
		get_xywh(),0,black,white);

	treeaux->gc=XCreateGC(display,treeaux->win,0,&values);

	XSelectInput(display,treeaux->win,
		ExposureMask|
		KeyPressMask|
		KeyReleaseMask|
		ButtonPressMask|
		FocusChangeMask);

	XSetFont(display,treeaux->gc,defaultfont->fid);

	XDefineCursor(display,treeaux->win,XCreateFontCursor(display,XC_xterm));
	XSetLineAttributes(display,treeaux->gc,0,LineSolid,CapRound,JoinRound);
	XMapWindow(display,treeaux->win);
	treeaux->next=treestk->treestk;
	if(treestk->treestk) treestk->treestk->last=treeaux;
	treestk->treestk=treeaux;

	return(treeaux);
}
