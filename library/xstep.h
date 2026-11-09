/*

XSTEP 3.5 - Toolkit for X-Window System
Copyright (C) 1996-2000 Marcelo Samsoniuk and many colaborators.

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

#ifndef XSTEP
#define XSTEP

#ifdef __cplusplus

	// the C++ code was fixed by CRG for 3.5
	extern "C" {
#endif

#include 	<X11/Xlib.h>
#include 	<X11/cursorfont.h>
#include 	<X11/Xutil.h>
#include	<stdio.h>
#include 	<stdlib.h>
#include	<string.h>
#include	<time.h>
#include 	<sys/time.h>

#define		EXPOSECACHESIZE 32

Display		*display;

XFontStruct	*helvetica12m,
		*helvetica12b,
		*helvetica24m,
		*helvetica24b,
		*courier12m,
		*courier12b,
		*defaultfont;

XGCValues	values;
XEvent		report;
Window		rootwindow;
Atom		atom;
XRectangle      clipmask[EXPOSECACHESIZE];

char		*displayname,
		*application,
		**argv;

int 		argc,
		screen,
		black, 
		white, 
		gray, 
		darkgray,
		shades[16],
		invisible,
		broadcast,
		capslock,
		shift,
		animate,
		exposec;

struct xtree {

	int		x,y,w,h,aw,ah,
			k,s,c,l,p,*q,r,*m,*n,todelete;
	struct timeval 	tv;

	char		*t,*b,**z;
	void		(*f)(struct xtree *);
	void            (*dc)(struct xtree *); 	        
	void            (*rc)(struct xtree *); 	        
	struct 		xtree *treestk;
	struct  	xtree *next;
	struct  	xtree *last;
	struct  	xtree *parent;
	Window		win;
	GC		gc,gcpix;
	Pixmap		pix;
	XFontStruct	*font;
	
	void	(*keypress)		(struct xtree *);
	void	(*keyrelease)		(struct xtree *);
	void	(*buttonpress)		(struct xtree *);
	void	(*buttonrelease)	(struct xtree *);
	void	(*buttonmotion)		(struct xtree *);
	void	(*expose)		(struct xtree *);
	void 	(*focusin)		(struct xtree *);
	void    (*focusout)		(struct xtree *);
	void 	(*broadcast)		(struct xtree *);
	void	(*buttonrepeat)		(struct xtree *);
	void 	(*message)		(struct xtree *);
	void 	(*configure)		(struct xtree *);

	int	wpix,hpix;

} *treestk,*winstk;

struct mlist {

	char 	*t;
	char 	**p;
	int	w;
};

struct xtree	*xyscroll_create(int,int,int,int,void (*)(struct xtree *),int,int,int);
struct xtree	*xyscroll_group (int,int,int,int,void (*)(struct xtree *),int,int,int);
struct xtree 	*window_create	(int,int,int,int,char *);
struct xtree 	*toolbox_create	(int,int,int,int);
struct xtree 	*menu_create	(int,int,int,int,char **,struct xtree *,void (*)(struct xtree *));
struct xtree 	*button_create	(int,int,int,int,char *,void (*)(struct xtree *));
struct xtree 	*popup_create	(int,int,int,int,char *,char **,int);
struct xtree 	*radio_create	(int,int,int,int,char *,char *);
struct xtree 	*check_create	(int,int,int,int,char *,char *);
struct xtree 	*edit_create	(int,int,int,int,char *,int,void (*)(struct xtree *));
struct xtree 	*label_create	(int,int,int,int,char *,int,int);
struct xtree 	*box_create	(int,int,int,int,void (*)(struct xtree *),int);
struct xtree 	*scroll_create	(int,int,int,int,int *,int *,char *[],void (*)(struct xtree *));
struct xtree 	*mscroll_create	(int,int,int,int,int *,int *,struct mlist *,void (*)(struct xtree *),void (*)(struct xtree *),void (*)(struct xtree *),int);
struct xtree 	*group_begin	(int,int,int,int,int);
void 		finetune_create	(int x, int y, int *q, int *a, int *b);
void 		group_end	(void);
void 		upbox		(int,int,int,int,int,struct xtree *);
void 		upboxsemi	(int,int,int,int,int,struct xtree *);
void 		downbox		(int,int,int,int,int,struct xtree *);
void 		window_delete	(struct xtree *);
void 		window_close	(struct xtree *);
void 		xmain		(int,char **);
int 		xscan		(struct xtree *);
void 		xbroad		(struct xtree *);
void 		bitbox		(int,int,int,int,char *[],struct xtree *);
int  		checkbox	(int,int,int,int);
int  		getcolor	(int,int,int);
int  		strcheck	(char *);
void 		reconfigure	(struct xtree *);
void 		tree		(void);
void 		mkpixmap	(char **,struct xtree *);


#define center		1
#define	left		2
#define	right		3
#define	up		4

#define		dialog_create($1,$2) window_create( 			\
			(DisplayWidth(display,screen) -($1))/2, 	\
			(DisplayHeight(display,screen)-($2))/2, 	\
			$1,$2,0);

#define		dialogbox_create($1,$2) window_create( 			\
			(DisplayWidth(display,screen) -($1))/2, 	\
			(DisplayHeight(display,screen)-($2))/2, 	\
			$1,$2,0);

#define get_xywh()	(x>=0)?x:treestk->aw+x-w, 			\
                	(y>=0)?y:treestk->ah+y-h,			\
                	treeaux->aw=(w>0)?w:treestk->aw-x+w,		\
                	treeaux->ah=(h>0)?h:treestk->ah-y+h

#define keypad_enter	65421
#define key_escape	65307
#define key_delete	65535
#define key_backspace	65288
#define key_arrow_left	65361
#define key_arrow_right	65363
#define key_arrow_up	65362
#define key_arrow_down	65364
#define key_page_up	65365
#define key_page_down	65366
#define key_tab		65289
#define key_return	65293
#define key_shift_left	65505
#define key_shift_right	65506
#define key_capslock	65509
#define key_home	65360
#define key_end		65367
#define key_asterisk	65450
#define key_plus	65451
#define key_comma	65452
#define key_hyphen	65453
#define key_decimal	65454
#define key_solidus	65455
#define key_zero	65456
#define key_one		65457
#define key_two		65458
#define key_thre	65459
#define key_four	65460
#define key_five	65461
#define key_six		65462
#define key_seven	65463
#define key_eight	65464
#define key_nine	65465
#define key_equal	65466

#define key_agrave	224
#define key_aacute	225
#define key_acircunflex	226
#define key_atilde	227
#define key_adiaeresis	228
#define key_egrave	232
#define key_eacute	233
#define key_ecircunflex	234
#define key_ediaeresis	235
#define key_igrave	236
#define key_iacute	237
#define key_icircunflex	238
#define key_idiaeresis	239
#define key_ograve	242
#define key_oacute	243
#define key_ocircunflex	244
#define key_otilde	245
#define key_odiaeresis	246
#define key_ugrave	249
#define key_uacute	250
#define key_ucircunflex	251
#define key_udiaeresis	252	
#define key_cedila	231
#define key_ntilde	241
#define key_yacute	253
#define key_ydiaeresis	255

#ifdef __cplusplus
	}
#endif
#endif
