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

extern char *arrowupbt[],*arrowdownbt[];

struct xtree *uptune_create(int x,int y,int *q,int *m);
struct xtree *downtune_create(int x,int y,int *q,int *m);
void finetune_create(int x, int y, int *q, int *a, int *b);

/* USAGE:
   
   finetune_create(int x, int y, int *q, int *a, int,b)

   x = horizontal position of the buttons
   y = vertical position of the buttons
   q = pointer to your integer variable
   a = (pointer to the) lowest number allowed by variable *q
   b = (pointer to the) highest number allowed by variable *q

   you need to create a label or printf the variable in order to see its value
   maybe I will implement a built-in label... Allah knows! :)

*/

void finetune_create(int x, int y, int *q, int *a, int *b)
{
  uptune_create(x,y,q,b);
  downtune_create(x,y+16,q,a);
}

void uptune_buttonrepeat(struct xtree *treeptr) {

	if(treeptr->c) {
		treeptr->c--;
		return;
	}
	if(*treeptr->q!=*treeptr->m) {

		(*treeptr->q)++;
                treeptr->c=2;
		broadcast++;
	}
}

void uptune_buttonpress(struct xtree *treeptr) {

	if(*treeptr->q!=*treeptr->m) {

		(*treeptr->q)++;
		broadcast++;
		treeptr->broadcast=uptune_buttonrepeat;
		treeptr->c=20;
	}
	downbox(0,0,treeptr->w,treeptr->h,white,treeptr);
	bitbox(4,4,9,9,arrowupbt,treeptr);
	animate++;
}

void uptune_expose(struct xtree *treeptr) {

	upbox(0,0,treeptr->w,treeptr->h,gray,treeptr);
	bitbox(3,3,9,9,arrowupbt,treeptr);
}

void uptune_buttonrelease(struct xtree *treeptr) {

	uptune_expose(treeptr);
	treeptr->broadcast=0;
	animate--;
}

struct xtree *uptune_create(int x,int y,int *q,int *m) {

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
	treeaux->expose=uptune_expose;
	treeaux->buttonpress=uptune_buttonpress;
	treeaux->buttonrelease=uptune_buttonrelease;
	treeaux->parent=treestk;

	treeaux->win=XCreateSimpleWindow(display,treestk->win,x,
		(y>=0)?y:treestk->ah+y-h,
		treeaux->aw=w,treeaux->ah=h,0,black,gray);

	treeaux->gc=XCreateGC(display,treeaux->win,0,&values);
	XSelectInput(display,treeaux->win,
		ExposureMask|
		ButtonPressMask|
		ButtonReleaseMask);
	XSetFont(display,treeaux->gc,helvetica12m->fid);
	XSetLineAttributes(display,treeaux->gc,0,LineSolid,CapRound,JoinRound);
	XMapWindow(display,treeaux->win);
	treeaux->next=treestk->treestk;
	treestk->treestk=treeaux;

	return(treeaux);
}

void downtune_buttonrepeat(struct xtree *treeptr) {

	if(treeptr->c) {
		treeptr->c--;
		return;
	}
	if(*treeptr->q!=*treeptr->m) {

		(*treeptr->q)--;
		treeptr->c=2;
		broadcast++;
	}
}

void downtune_buttonpress(struct xtree *treeptr) {

	if(*treeptr->q!=*treeptr->m) {

		(*treeptr->q)--;
		broadcast++;
		treeptr->broadcast=downtune_buttonrepeat;
		treeptr->c=20;
	}
	downbox(0,0,treeptr->w,treeptr->h,white,treeptr);
	bitbox(4,4,9,9,arrowdownbt,treeptr);
	animate++;
}

void downtune_expose(struct xtree *treeptr) {

        upbox(0,0,treeptr->w,treeptr->h,gray,treeptr);
        bitbox(3,3,9,9,arrowdownbt,treeptr);
}
                                

void downtune_buttonrelease(struct xtree *treeptr) {

	downtune_expose(treeptr);
	treeptr->broadcast=0;
	animate--;
}

struct xtree *downtune_create(int x,int y,int *q,int *m) {

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
	treeaux->expose=downtune_expose;
	treeaux->buttonpress=downtune_buttonpress;
	treeaux->buttonrelease=downtune_buttonrelease;
	treeaux->parent=treestk;

	treeaux->win=XCreateSimpleWindow(display,treestk->win,x,
		(y>=0)?y:treestk->ah+y-h,
		treeaux->aw=w,treeaux->ah=h,0,black,gray);

	treeaux->gc=XCreateGC(display,treeaux->win,0,&values);
	XSelectInput(display,treeaux->win,
		ExposureMask|
		ButtonPressMask|
		ButtonReleaseMask);
	XSetFont(display,treeaux->gc,helvetica12m->fid);
	XSetLineAttributes(display,treeaux->gc,0,LineSolid,CapRound,JoinRound);
	XMapWindow(display,treeaux->win);
	treeaux->next=treestk->treestk;
	treestk->treestk=treeaux;

	return(treeaux);
}

