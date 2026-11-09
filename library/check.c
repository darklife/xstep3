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

char *checkbt[]=
{
	"222222230",
	"222222301",
	"222223012",
	"302230122",
	"301301222",
	"313012222",
	"330122222",
	"301222222",
	"012222222",
};

void check_expose(struct xtree *treeptr) {

	upbox(0,2,15,15,gray,treeptr);
	XDrawString(display,treeptr->win,treeptr->gc,24,14,treeptr->t,strlen(treeptr->t));		
	if(!strcmp(treeptr->t,treeptr->b)) bitbox(3,5,9,9,checkbt,treeptr);
}

void check_broadcast(struct xtree *treeptr) {

	if(treeptr->k==strcheck(treeptr->b)) return;
	treeptr->k=strcheck(treeptr->b);
	upbox(0,2,15,15,gray,treeptr);
	if(!strcmp(treeptr->t,treeptr->b)) bitbox(3,5,9,9,checkbt,treeptr);
}

void check_buttonpress(struct xtree *treeptr) {

	if(!checkbox(0,0,treeptr->w,treeptr->h)) return;
	if(strcmp(treeptr->b,treeptr->t))  strcpy(treeptr->b,treeptr->t);
	else strcpy(treeptr->b,"");
	broadcast++;
}

struct xtree *check_create(int x,int y,int w,int h,char *t,char *b) {

	struct xtree *treeaux;

	treeaux=(struct xtree *)malloc(sizeof(struct xtree));
	memset(treeaux,0,sizeof(struct xtree));

	treeaux->x=x;
	treeaux->y=y;
	treeaux->w=w;
	treeaux->h=h;
	treeaux->t=t;
	treeaux->b=b;
	treeaux->broadcast=check_broadcast;
	treeaux->expose=check_expose;
	treeaux->buttonpress=check_buttonpress;
	treeaux->parent=treestk;

	treeaux->win=XCreateSimpleWindow(display,treestk->win,
		get_xywh(),0,black,gray);

	treeaux->gc=XCreateGC(display,treeaux->win,0,&values);

	XSelectInput(display,treeaux->win,
		ExposureMask|
		ButtonPressMask);

	XSetFont(display,treeaux->gc,defaultfont->fid);
	XSetLineAttributes(display,treeaux->gc,0,LineSolid,CapRound,JoinRound);
	XMapWindow(display,treeaux->win);
	treeaux->next=treestk->treestk;
	treestk->treestk=treeaux;

	return(treeaux);
}
