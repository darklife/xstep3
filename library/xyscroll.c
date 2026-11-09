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
#include "xyscroll.h"

void xyscroll_broadcast(struct xtree *t) {

	int dx,dy,dz=16,dw=19;

	if(!t->r) return;

	if(t->k) { t->k--; return; };
	t->k=3;

	switch(t->r) {

		case 1: t->treestk->treestk->y+=dz; break;
		case 2: t->treestk->treestk->y-=dz; break;
		case 3: t->treestk->treestk->x+=dz; break;
		case 4: t->treestk->treestk->x-=dz; break;
	}
	if(t->treestk->treestk->x+t->treestk->treestk->aw<t->treestk->aw) t->treestk->treestk->x=t->treestk->aw-t->treestk->treestk->aw;
	if(t->treestk->treestk->y+t->treestk->treestk->ah<t->treestk->ah) t->treestk->treestk->y=t->treestk->ah-t->treestk->treestk->ah;
	if(t->treestk->treestk->aw<t->treestk->aw) t->treestk->treestk->x=(t->treestk->aw-t->treestk->treestk->aw)/2;
	if(t->treestk->treestk->ah<t->treestk->ah) t->treestk->treestk->y=(t->treestk->ah-t->treestk->treestk->ah)/2;
	if(t->treestk->treestk->x>0) t->treestk->treestk->x=0;
	if(t->treestk->treestk->y>0) t->treestk->treestk->y=0;

	XMoveWindow(display,
		t->treestk->treestk->win,
		t->treestk->treestk->x,
		t->treestk->treestk->y);

	if(t->treestk->treestk->h) {

		if(t->treestk->treestk->ah > t->treestk->ah) {

			dx=((t->ah-40)*t->treestk->ah)/t->treestk->treestk->ah+1;
			dy=-((t->ah-40)*t->treestk->treestk->y)/t->treestk->treestk->ah;

			XSetFillStyle(display,t->gc,FillTiled);
			XSetTile(display,t->gc,t->pix);
			if(dy>0) XFillRectangle(display,t->win,t->gc,3,3,16,dy);
			if((t->ah-40-dy-dx)>0) XFillRectangle(display,t->win,t->gc,3,3+dy+dx,16,t->ah-40-dy-dx);
			XSetFillStyle(display,t->gc,FillSolid);
			upbox(3,3+dy,16,dx,gray,t);
			if(dx>16) bitbox(7,dx/2+dy,6,6,scrollbar_bitmap,t);
	
		} else {
	
			XSetFillStyle(display,t->gc,FillTiled);
			XSetTile(display,t->gc,t->pix);
			XFillRectangle(display,t->win,t->gc,3,3,16,t->ah-6);
			XSetFillStyle(display,t->gc,FillSolid);
		}
		dw=0;
	}

	if(t->treestk->treestk->w) {
	
		if(t->treestk->treestk->aw > t->treestk->aw) {
	
			dx=((t->aw-59+dw)*t->treestk->aw)/t->treestk->treestk->aw+1;
			dy=-((t->aw-59+dw)*t->treestk->treestk->x)/t->treestk->treestk->aw;
	
			XSetFillStyle(display,t->gc,FillTiled);
			XSetTile(display,t->gc,t->pix);
			if((dy)>0) XFillRectangle(display,t->win,t->gc,56-dw,t->ah-19,dy,16);
			if((t->aw-59-dy-dx+dw)>0) XFillRectangle(display,t->win,t->gc,56+dy+dx-dw,t->ah-19,t->aw-59-dy-dx+dw,16);
			XSetFillStyle(display,t->gc,FillSolid);
			upbox(56+dy-dw,t->ah-19,dx,16,gray,t);
			if(dx>16) bitbox(56+dx/2+dy-3-dw,t->ah-19+4,6,6,scrollbar_bitmap,t);
	
		} else {
		
			XSetFillStyle(display,t->gc,FillTiled);
			XSetTile(display,t->gc,t->pix);
			XFillRectangle(display,t->win,t->gc,22-dw,t->ah-19,t->aw-25+dw,16);
			XSetFillStyle(display,t->gc,FillSolid);
		}
	}
}

void xyscroll_expose(struct xtree *t) {

	int dw=19;

	if(t->treestk->treestk->h) {

		if(t->treestk->treestk->ah > t->treestk->ah) {

			upbox( 3,t->ah-36,16,16,gray,t);
			upbox( 3,t->ah-19,16,16,gray,t);

			bitbox( 6,t->ah-33,9,9,arrow_up_bitmap,t);
			bitbox( 6,t->ah-16,9,9,arrow_down_bitmap,t);
		}
		XSetForeground(display,t->gc,black);
		XDrawLine(display,t->win,t->gc,20,0,20,t->ah);
		dw=0;
	}	

	if(t->treestk->treestk->w) {

		if(t->treestk->treestk->aw > t->treestk->aw) {

			upbox(22-dw,t->ah-19,16,16,gray,t); 
			upbox(39-dw,t->ah-19,16,16,gray,t);
		
			bitbox(25-dw,t->ah-16,9,9,arrow_left_bitmap,	t);
			bitbox(42-dw,t->ah-16,9,9,arrow_right_bitmap,	t);
		}
		XSetForeground(display,t->gc,black);
		XDrawLine(display,t->win,t->gc,20-dw,t->ah-21,t->aw,t->ah-21);
	}
	t->k=0;
	t->r=5;
	xyscroll_broadcast(t);
	xyscroll_broadcast(t);
	t->r=0;
	downbox(0,0,t->aw,t->ah,invisible,t);
}

void scrollbar_configure(struct xtree *t) {

	reconfigure(t);

	t->r=7;
	xyscroll_broadcast(t);
	xyscroll_broadcast(t);
	t->r=0;
	broadcast++;
}

void xyscroll_buttonrelease(struct xtree *t) {

	int dw;
	
	if(t->treestk->treestk->h) dw=0; else dw=+19;

	if(t->r==1) {
		upbox(3,t->ah-36,16,16,gray,			t);
		bitbox(6,t->ah-33,9,9,arrow_up_bitmap,		t);
		animate--;
	}
	if(t->r==2) {
		upbox(3,t->ah-19,16,16,gray,			t);
		bitbox(6,t->ah-16,9,9,arrow_down_bitmap,	t);
		animate--;
	}
	if(t->r==3) {
		upbox(22-dw,t->ah-19,16,16,gray,		t);
		bitbox(25-dw,t->ah-16,9,9,arrow_left_bitmap,	t);
		animate--;
	}
	if(t->r==4) {
		upbox(39-dw,t->ah-19,16,16,gray,		t);
		bitbox(42-dw,t->ah-16,9,9,arrow_right_bitmap,	t);
		animate--;
	}
	t->r=0;
	t->k=0;
}

void xyscroll_buttonpress(struct xtree *t) {

	int dx,dy,dw=+19;

	if(t->treestk->treestk->h && t->treestk->treestk->ah > t->treestk->ah) {

		if(checkbox(3,t->ah-36,16,16)) {

			downbox(3,t->ah-36,16,16,white,			t);
			bitbox(6+1,t->ah-33+1,9,9,arrow_up_bitmap,	t);
			t->r=1;
			animate++;
		}
		if(checkbox(3,t->ah-19,16,16)) {

			downbox(3,t->ah-19,16,16,white,			t);
			bitbox(6+1,t->ah-16+1,9,9,arrow_down_bitmap,	t);
			t->r=2;
			animate++;
		}
		dx=((t->ah-40)*t->treestk->ah)/t->treestk->treestk->ah+1;
		dy=-((t->ah-40)*t->treestk->treestk->y)/t->treestk->treestk->ah;

		if(checkbox(3,3+dy,16,dx)) {
		
			t->r=5;
			t->l=report.xbutton.y-dy;
		}
		dw=0;
	}
	if(t->treestk->treestk->w && t->treestk->treestk->aw > t->treestk->aw) {
	
		if(checkbox(22-dw,t->ah-19,16,16)) {

			downbox(22-dw,t->ah-19,16,16,white,	t);
			bitbox(25+1-dw,t->ah-16+1,9,9,arrow_left_bitmap,	t);
			t->r=3;
			animate++;
		}
		if(checkbox(39-dw,t->ah-19+1,16,16)) {

			downbox(39-dw,t->ah-19,16,16,white,	t);
			bitbox(42+1-dw,t->ah-16+1,9,9,arrow_right_bitmap,t);
			t->r=4;
			animate++;
		}

		dx=((t->aw-59+dw)*t->treestk->aw)/t->treestk->treestk->aw+1;
		dy=-((t->aw-59+dw)*t->treestk->treestk->x)/t->treestk->treestk->aw;

		if(checkbox(56+dy-dw,t->ah-19,dx,16)) {

			t->l=report.xbutton.x-dy;
			t->r=6;
		}
	}
}

void xyscroll_buttonmotion(struct xtree *t) {

	int dy,dx,dw;

	if(t->treestk->treestk->h) dw=0; else dw=19;

	if(t->r==5) {

		dy=report.xbutton.y-t->l;
		t->treestk->treestk->y=-(dy*t->treestk->treestk->ah)/(t->ah-40);
	}
	if(t->r==6) {

		dx=report.xbutton.x-t->l;
		t->treestk->treestk->x=-(dx*t->treestk->treestk->aw)/(t->aw-59+dw);
	}
	broadcast++;
}

void xyscroll_sizeconfigure(struct xtree *t) {

	if(!t->treestk->h) t->treestk->ah=t->ah;
	if(!t->treestk->w) t->treestk->aw=t->aw;

	XResizeWindow(display,t->treestk->win,t->treestk->aw,t->treestk->ah);	
}

struct xtree *xyscroll_create(int x,int y,int w,int h,void (*f)(struct xtree *),int c,int vx,int vy) {

	struct xtree *group;
	int a,b;

	group=group_begin(x,y,w,h,gray);

		group->expose		=xyscroll_expose;
		group->buttonrelease	=xyscroll_buttonrelease;
		group->buttonpress	=xyscroll_buttonpress;
		group->buttonmotion	=xyscroll_buttonmotion;
		group->broadcast	=xyscroll_broadcast;

		XSelectInput(display,group->win,
			ExposureMask|
			StructureNotifyMask|
			ButtonPressMask|
			ButtonMotionMask|
			ButtonReleaseMask);

        	group->pix=XCreatePixmap(display,group->win,16,16,DefaultDepth(display,screen));
        	group->gcpix=XCreateGC(display,group->pix,0,&values);
		group->configure=scrollbar_configure;

        	for(a=0;a!=16;a++) {
        
        	        for(b=0;b!=16;b++) {

        	                XSetForeground(display,group->gcpix,a%2==b%2?gray:darkgray);
        	                XDrawPoint(display,group->pix,group->gcpix,a,b);
        	        }
		}

		group=group_begin(vy?21:2,2,-2,vx?-21:-2,gray);
		group->configure=xyscroll_sizeconfigure;

			group=box_create(0,0,vx,vy,f,c);

		group_end();
	group_end();

	return(group);
}

struct xtree *xyscroll_group(int x,int y,int w,int h,void (*f)(struct xtree *),int c,int vx,int vy) {

	struct xtree *group;
	int a,b;

	group=group_begin(x,y,w,h,gray);

		group->expose		=xyscroll_expose;
		group->buttonrelease	=xyscroll_buttonrelease;
		group->buttonpress	=xyscroll_buttonpress;
		group->buttonmotion	=xyscroll_buttonmotion;
		group->broadcast	=xyscroll_broadcast;

		XSelectInput(display,group->win,
			ExposureMask|
			StructureNotifyMask|
			ButtonPressMask|
			ButtonMotionMask|
			ButtonReleaseMask);

        	group->pix=XCreatePixmap(display,group->win,16,16,DefaultDepth(display,screen));
        	group->gcpix=XCreateGC(display,group->pix,0,&values);
		group->configure=scrollbar_configure;

        	for(a=0;a!=16;a++) {
        
        	        for(b=0;b!=16;b++) {

        	                XSetForeground(display,group->gcpix,a%2==b%2?gray:darkgray);
        	                XDrawPoint(display,group->pix,group->gcpix,a,b);
        	        }
		}

		group=group_begin(vy?21:2,2,-2,vx?-21:-2,gray);
		group->configure=xyscroll_sizeconfigure;

			group=group_begin(0,0,vx,vy,c);
			f(group);
			group_end();

		group_end();
	group_end();

	return(group);
}

