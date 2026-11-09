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

#include "xstep.h"
#include "scroll_bitmap.h"

void upbutton_buttonrepeat(struct xtree *treeptr) {

	if(treeptr->c) {
		treeptr->c--;
		return;
	}
	if(*treeptr->q) {

		(*treeptr->q)--;
		treeptr->c=2;
		broadcast++;
	}
}

void upbutton_buttonpress(struct xtree *treeptr) {

	if(*treeptr->q) {

		(*treeptr->q)--;
		broadcast++;
		treeptr->broadcast=upbutton_buttonrepeat;
		treeptr->c=20;
	}
	downbox(0,0,treeptr->w,treeptr->h,white,treeptr);
	bitbox(4,4,9,9,arrowupbt,treeptr);
	animate++;
}

void upbutton_expose(struct xtree *treeptr) {

        upbox(0,0,treeptr->w,treeptr->h,gray,treeptr);
        bitbox(3,3,9,9,arrowupbt,treeptr);
}
                                

void upbutton_buttonrelease(struct xtree *treeptr) {

	upbutton_expose(treeptr);
	treeptr->broadcast=0;
	animate--;
}

struct xtree *upbutton_create(int x,int y,int *q,int *m) {

	int w=16,h=16; struct xtree *treeaux;

	treeaux=(struct xtree *)malloc(sizeof(struct xtree));
	memset(treeaux,0,sizeof(struct xtree));
	treeaux->x=x;
	treeaux->y=y;
	treeaux->w=w;
	treeaux->h=h;
	treeaux->q=q;
	treeaux->m=m;
	treeaux->treestk=0;
	treeaux->expose=upbutton_expose;
	treeaux->buttonpress=upbutton_buttonpress;
	treeaux->buttonrelease=upbutton_buttonrelease;
	treeaux->parent=treestk;

	treeaux->win=XCreateSimpleWindow(display,treestk->treestk->win,x,
		(y>=0)?y:treestk->treestk->ah+y-h,
		treeaux->aw=w,treeaux->ah=h,0,black,gray);

	treeaux->gc=XCreateGC(display,treeaux->win,0,&values);
	XSelectInput(display,treeaux->win,
		ExposureMask|
		ButtonPressMask|
		ButtonReleaseMask);
	XSetFont(display,treeaux->gc,helvetica12m->fid);
	XSetLineAttributes(display,treeaux->gc,0,LineSolid,CapRound,JoinRound);
	XMapWindow(display,treeaux->win);
	treeaux->next=treestk->treestk->treestk;
	treestk->treestk->treestk=treeaux;

	return(treeaux);
}

void downbutton_buttonrepeat(struct xtree *treeptr) {

	if(treeptr->c) {
		treeptr->c--;
		return;
	}
	if(*treeptr->q!=*treeptr->m-1) {

		(*treeptr->q)++;
                treeptr->c=2;
		broadcast++;
	}
}

void downbutton_buttonpress(struct xtree *treeptr) {

	if(*treeptr->m) if(*treeptr->q!=*treeptr->m-1) {

		(*treeptr->q)++;
		broadcast++;
		treeptr->broadcast=downbutton_buttonrepeat;
		treeptr->c=20;
	}
	downbox(0,0,treeptr->w,treeptr->h,white,treeptr);
	bitbox(4,4,9,9,arrowdownbt,treeptr);
	animate++;
}

void downbutton_expose(struct xtree *treeptr) {

	upbox(0,0,treeptr->w,treeptr->h,gray,treeptr);
	bitbox(3,3,9,9,arrowdownbt,treeptr);
}

void downbutton_buttonrelease(struct xtree *treeptr) {

	downbutton_expose(treeptr);
	treeptr->broadcast=0;
	animate--;
}

struct xtree *downbutton_create(int x,int y,int *q,int *m) {

	int w=16,h=16; struct xtree *treeaux;

	treeaux=(struct xtree *)malloc(sizeof(struct xtree));
	memset(treeaux,0,sizeof(struct xtree));
	treeaux->x=x;
	treeaux->y=y;
	treeaux->w=w;
	treeaux->h=h;
	treeaux->q=q;
	treeaux->m=m;
	treeaux->treestk=0;
	treeaux->expose=downbutton_expose;
	treeaux->buttonpress=downbutton_buttonpress;
	treeaux->buttonrelease=downbutton_buttonrelease;
	treeaux->parent=treestk;

	treeaux->win=XCreateSimpleWindow(display,treestk->treestk->win,x,
		(y>=0)?y:treestk->treestk->ah+y-h,
		treeaux->aw=w,treeaux->ah=h,0,black,gray);

	treeaux->gc=XCreateGC(display,treeaux->win,0,&values);
	XSelectInput(display,treeaux->win,
		ExposureMask|
		ButtonPressMask|
		ButtonReleaseMask);
	XSetFont(display,treeaux->gc,helvetica12m->fid);
	XSetLineAttributes(display,treeaux->gc,0,LineSolid,CapRound,JoinRound);
	XMapWindow(display,treeaux->win);
	treeaux->next=treestk->treestk->treestk;
	treestk->treestk->treestk=treeaux;

	return(treeaux);
}

void scrollbar_broadcast(struct xtree *treeptr) {

	int sh,sy,dx=0;

	if(*treeptr->m) {

		sh=(treeptr->ah*((treeptr->ah+36)/16))/(*treeptr->m);
		if(sh<16) {
		
			dx=16-sh;
			sh=16;
		}
		sy=(*treeptr->n*treeptr->ah-(*treeptr->n*dx))/(*treeptr->m);

	} else sh=sy=0;

	if((sh+sy)==treeptr->k) return;
	treeptr->k=sh+sy;

	if(sh&&sh<treeptr->ah) {
		if(sh>4) upbox(0,sy,treeptr->aw,sh,gray,treeptr);
		else upbox(0,sy,treeptr->aw,sh,darkgray,treeptr);
		if(sh>treeptr->aw) bitbox(4,sy+(sh/2)-3,6,6,listbt,treeptr);
	} else {
		XSetFillStyle(display,treeptr->gc,FillTiled);
		XSetTile(display,treeptr->gc,treeptr->pix);

		XFillRectangle(display,treeptr->win,treeptr->gc,
			0,0,treeptr->aw,treeptr->ah);
	}

	XSetFillStyle(display,treeptr->gc,FillTiled);	        
	XSetTile(display,treeptr->gc,treeptr->pix);
	if(sy>0) XFillRectangle(display,treeptr->win,treeptr->gc,0,0,treeptr->aw,sy);
	if((treeptr->ah-(sy+sh))>0) 
		XFillRectangle(display,treeptr->win,treeptr->gc,
		0,(sy+sh),treeptr->aw,treeptr->ah-(sy+sh));
	XSetFillStyle(display,treeptr->gc,FillSolid);
}

void scrollbar_expose(struct xtree *treeptr) {

	int sh,sy,dx=0;

	if(*treeptr->m) {

		sh=(treeptr->ah*((treeptr->ah+36)/16))/(*treeptr->m);
		if(sh<16) {
		
			dx=16-sh;
			sh=16;
		}
		sy=(*treeptr->n*treeptr->ah-(*treeptr->n*dx))/(*treeptr->m);

	} else sh=sy=0;

	if(sh&&sh<treeptr->ah) {
		if(sh>4) upbox(0,sy,treeptr->aw,sh,gray,treeptr);
		else upbox(0,sy,treeptr->aw,sh,darkgray,treeptr);
		if(sh>treeptr->aw) bitbox(4,sy+(sh/2)-3,6,6,listbt,treeptr);
	} else {
		XSetFillStyle(display,treeptr->gc,FillTiled);
		XSetTile(display,treeptr->gc,treeptr->pix);

		XFillRectangle(display,treeptr->win,treeptr->gc,
			0,0,treeptr->aw,treeptr->ah);
	}

	XSetFillStyle(display,treeptr->gc,FillTiled);
	XSetTile(display,treeptr->gc,treeptr->pix);
	if(sy>0) XFillRectangle(display,treeptr->win,treeptr->gc,0,0,treeptr->aw,sy);
	if((treeptr->ah-(sy+sh))>0) 
		XFillRectangle(display,treeptr->win,treeptr->gc,
		0,(sy+sh),treeptr->aw,treeptr->ah-(sy+sh));
	XSetFillStyle(display,treeptr->gc,FillSolid);
}

void scrollbar_buttonmotion(struct xtree *treeptr) {

	int sh,sy,dx=0;

	if(*treeptr->m) {

		sh=(treeptr->ah*((treeptr->ah+36)/16))/(*treeptr->m);
		if(sh<16) {
		
			dx=16-sh;
			sh=16;
		}
		sy=(*treeptr->n*treeptr->ah-(*treeptr->n*dx))/(*treeptr->m);
	} else sh=sy=0;

	/* the code below must be modified to use sh/sy! */

	*treeptr->n=(*treeptr->m*(report.xbutton.y-treeptr->l))/(treeptr->ah);
	if(*treeptr->n<0) *treeptr->n=0;
	if(*treeptr->n>(*treeptr->m-((treeptr->ah+36)/16))) 
		*treeptr->n=*treeptr->m-(treeptr->ah+36)/16;
	if(*treeptr->q<*treeptr->n) *treeptr->q=*treeptr->n;
	if(*treeptr->q>=(*treeptr->n+(treeptr->ah+36)/16))
		*treeptr->q=*treeptr->n+(treeptr->ah+36)/16-1;
	broadcast++;
}

void scrollbar_buttonrelease(struct xtree *treeptr) {

	treeptr->buttonmotion=0;
}

void scrollbar_buttonpress(struct xtree *treeptr) {

	int sh,sy,dx=0;

	if(*treeptr->m&&*treeptr->m>(treeptr->ah+36)/16) {

		sh=(treeptr->ah*((treeptr->ah+36)/16))/(*treeptr->m);
		if(sh<16) {
		
			dx=16-sh;
			sh=16;
		}
		sy=(*treeptr->n*treeptr->ah-(*treeptr->n*dx))/(*treeptr->m);
	} else return;

	if(report.xbutton.y<sy) return;
	if(report.xbutton.y>(sy+sh-1)) return;
	treeptr->l=report.xbutton.y-sy;
	treeptr->buttonmotion=scrollbar_buttonmotion;
	broadcast++;
}

struct xtree *scrollbar_create(int x,int y,int h,int *q,int *m,int *n,void (*dc)(struct xtree *)) {

	int w=16,a,b; struct xtree *treeaux;

	treeaux=(struct xtree *)malloc(sizeof(struct xtree));
	memset(treeaux,0,sizeof(struct xtree));
	treeaux->x=x;
	treeaux->y=y;
	treeaux->w=w;
	treeaux->h=h;
	treeaux->q=q;
	treeaux->m=m;
	treeaux->n=n;
	treeaux->treestk=0;
	treeaux->broadcast=scrollbar_broadcast;
	treeaux->parent=treestk;

	treeaux->expose=scrollbar_expose;
	treeaux->buttonrelease=scrollbar_buttonrelease;
	treeaux->buttonpress=scrollbar_buttonpress;

	treeaux->win=XCreateSimpleWindow(display,treestk->treestk->win,
		x,
		y,
		treeaux->aw=w,
		treeaux->ah=(h>=0)?h:treestk->treestk->ah-y+h,
		0,black,gray);

	treeaux->gc=XCreateGC(display,treeaux->win,0,&values);

	treeaux->pix=XCreatePixmap(display,treeaux->win,
		16,16,DefaultDepth(display,screen));
	treeaux->gcpix=XCreateGC(display,treeaux->pix,0,&values);

	for(a=0;a!=16;a++)
		for(b=0;b!=16;b++) {

			XSetForeground(display,treeaux->gcpix,a%2==b%2?gray:darkgray);
			XDrawPoint(display,treeaux->pix,treeaux->gcpix,a,b);
		}

	XSelectInput(display,treeaux->win,
		ExposureMask|
		ButtonPressMask|
		ButtonMotionMask|
		ButtonReleaseMask);
	XSetFont(display,treeaux->gc,helvetica12m->fid);
	XSetLineAttributes(display,treeaux->gc,0,LineSolid,CapRound,JoinRound);
	XMapWindow(display,treeaux->win);
	treeaux->next=treestk->treestk->treestk;
	treestk->treestk->treestk=treeaux;

	return(treeaux);
}
