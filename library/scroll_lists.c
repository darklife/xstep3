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

void list_expose(struct xtree *treeptr) {
	
	int i;

	XSetForeground(display,treeptr->gc,black);
	XDrawLine(display,treeptr->win,treeptr->gc,0,0,0,treeptr->ah);
	if(*treeptr->q<(*treeptr->n)) (*treeptr->n)=*treeptr->q;
	if(*treeptr->q>((*treeptr->n)+(treeptr->ah)/16-1)) 
		(*treeptr->n)=*treeptr->q-((treeptr->ah)/16-1);

	if(*treeptr->m>=(treeptr->ah/16))
		if((treeptr->ah/16)>=(*treeptr->m-*treeptr->n))
			(*treeptr->n)=(*treeptr->m)-(treeptr->ah/16);

	for(i=0;i!=((treeptr->ah)/16)&&(i+*treeptr->n)<(*treeptr->m);i++) {

		if((*treeptr->n)+i==(*treeptr->q)) {
			XSetForeground(display,treeptr->gc,white);
			XFillRectangle(display,treeptr->win,treeptr->gc,
				1,(*treeptr->q-(*treeptr->n))*16,treeptr->aw-1,16);
			XSetForeground(display,treeptr->gc,black);
		}
		XDrawString(display,treeptr->win,treeptr->gc,
			4,16*i+12,treeptr->z[i+(*treeptr->n)],
			strlen(treeptr->z[i+(*treeptr->n)]));
	}
}

void list_broadcast(struct xtree *treeptr) {
	
	int i,c,ki;

	for(ki=0,i=0;i<*treeptr->m;i++) ki+=(strcheck(treeptr->z[i])*(i+1));
	ki=ki*(1+*treeptr->m+*treeptr->n);
	if(treeptr->k==ki&&treeptr->l==*treeptr->q) return;

	c=treeptr->r;
	if(*treeptr->q>=*treeptr->m&&*treeptr->m) *treeptr->q=*treeptr->m-1;
	if(*treeptr->q<0) *treeptr->q=0;
	if(*treeptr->q<*treeptr->n) (*treeptr->n)=*treeptr->q;
	if(*treeptr->q>(*treeptr->n+(treeptr->ah)/16-1)) 
		(*treeptr->n)=*treeptr->q-((treeptr->ah)/16-1);

	if(*treeptr->m>(treeptr->ah/16))
		if((treeptr->ah/16)>=(*treeptr->m-*treeptr->n))
			(*treeptr->n)=(*treeptr->m)-(treeptr->ah/16);

        if((*treeptr->m-*treeptr->n)<(treeptr->ah/16)) {

		XSetForeground(display,treeptr->gc,gray);
		XFillRectangle(display,treeptr->win,treeptr->gc,
			1,(*treeptr->m-*treeptr->n)*16,treeptr->aw-1,treeptr->ah);
	}
	if(*treeptr->n!=c||treeptr->k!=ki) {

		for(i=0;i!=(treeptr->ah)/16&&(i+*treeptr->n)<*treeptr->m;i++) {

			if((*treeptr->n)+i==(*treeptr->q)) {
				XSetForeground(display,treeptr->gc,white);
				XFillRectangle(display,treeptr->win,treeptr->gc,
					1,(*treeptr->q-(*treeptr->n))*16,treeptr->aw-1,16);
				XSetForeground(display,treeptr->gc,black);
			} else {
				XSetForeground(display,treeptr->gc,gray);
				XFillRectangle(display,treeptr->win,treeptr->gc,
					1,i*16,treeptr->aw-1,16);
				XSetForeground(display,treeptr->gc,black);
			}
			XDrawString(display,treeptr->win,treeptr->gc,
				4,16*i+12,treeptr->z[i+(*treeptr->n)],
				strlen(treeptr->z[i+(*treeptr->n)]));
		}
	} else {
		i=treeptr->l-*treeptr->n;
		XSetForeground(display,treeptr->gc,gray);
		XFillRectangle(display,treeptr->win,treeptr->gc,
			1,i*16,treeptr->aw-1,16);
		XSetForeground(display,treeptr->gc,black);
		XDrawString(display,treeptr->win,treeptr->gc,
			4,16*i+12,treeptr->z[i+(*treeptr->n)],
			strlen(treeptr->z[i+(*treeptr->n)]));		
		i=*treeptr->q-*treeptr->n;
		XSetForeground(display,treeptr->gc,white);
		XFillRectangle(display,treeptr->win,treeptr->gc,
			1,i*16,treeptr->aw-1,16);
		XSetForeground(display,treeptr->gc,black);
		XDrawString(display,treeptr->win,treeptr->gc,
			4,16*i+12,treeptr->z[i+(*treeptr->n)],
			strlen(treeptr->z[i+(*treeptr->n)]));
	}
	treeptr->l=(*treeptr->q);
	for(ki=0,i=0;i<*treeptr->m;i++) ki+=(strcheck(treeptr->z[i])*(i+1));
	treeptr->k=ki*(1+*treeptr->m+*treeptr->n);
	treeptr->r=*treeptr->n;
	if(treeptr->f) treeptr->f(treeptr);
}

void list_buttonrepeat(struct xtree *treeptr) {

	if(report.xbutton.y>treeptr->ah) 
		if((*treeptr->q)!=(*treeptr->m-1)) 
			(*treeptr->q)++; 

	if(report.xbutton.y<0) 
		if((*treeptr->q)) 
			(*treeptr->q)--;

	list_broadcast(treeptr);
}

void list_buttonpress(struct xtree *treeptr) {

        struct timezone tz;
        struct timeval  tv;
        float doubleclick;
	int i;

	if(report.xbutton.y<0||report.xbutton.y>treeptr->ah) {
	
		if(treeptr->broadcast!=list_buttonrepeat) { 

			treeptr->broadcast=list_buttonrepeat; 
			animate++;
		}
		return;
	}

	for(i=0;i!=(treeptr->ah)/16&&i<*treeptr->m;i++)

	if(report.xbutton.y>=(i*16)&&report.xbutton.y<=(i*16+16)) {

		*treeptr->q=((*treeptr->n)+i);
		broadcast++;
		break;
	}
	switch(report.xbutton.button) {

		case 1:
		        gettimeofday(&tv,&tz);
		        doubleclick=(tv.tv_sec-treeptr->tv.tv_sec)+
		                    (tv.tv_usec-treeptr->tv.tv_usec)/
		                    1000000.0;
			memcpy(&treeptr->tv,&tv,sizeof(tv));		
		        if(doubleclick<0.25) if(treeptr->dc) treeptr->dc(treeptr);
			break;
		case 3:
			if(treeptr->rc) treeptr->rc(treeptr);
	}
}

void list_buttonrelease(struct xtree *treeptr) { 

	if(treeptr->broadcast==list_buttonrepeat) { 

		treeptr->broadcast=list_broadcast;
		animate--;
	}
}

struct xtree *list_create(int x,int y,int w,int h,int *q,int *m,int *n,char *z[],void (*f)(struct xtree *),void (*dc)(struct xtree *),void (*rc)(struct xtree *)) {

	struct xtree *treeaux;

	treeaux=(struct xtree *)malloc(sizeof(struct xtree));
	memset(treeaux,0,sizeof(struct xtree));
	treeaux->x=x;
	treeaux->y=y;
	treeaux->w=w;
	treeaux->h=h;
	treeaux->q=q;
	treeaux->m=m;
	treeaux->n=n;
	treeaux->z=z;
	treeaux->f=f;
	treeaux->dc=dc;
	treeaux->rc=rc;
	treeaux->treestk=0;
	treeaux->expose=list_expose;
	treeaux->broadcast=list_broadcast;
	treeaux->buttonpress=list_buttonpress;
	treeaux->buttonmotion=list_buttonpress;
	treeaux->buttonrelease=list_buttonrelease;
	treeaux->parent=treestk;

	treeaux->win=XCreateSimpleWindow(display,treestk->treestk->win,x,
		(y>=0)?y:treestk->treestk->ah+y-h,
		treeaux->aw=(w>=0)?w:treestk->treestk->aw-x+w,
		treeaux->ah=(h>=0)?h:treestk->treestk->ah-y+h,
		0,black,gray);

	treeaux->gc=XCreateGC(display,treeaux->win,0,&values);

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
