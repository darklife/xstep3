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
#include <sys/time.h>
#include <unistd.h>
#include <math.h>

#include <X11/extensions/Xdbe.h>

XdbeSwapInfo 	dbinfo;
XdbeBackBuffer 	dbbuffer;

#define VECTORS (18*4)

#ifndef PI
	#define PI 	(355.0/113.0)
#endif

XPoint 	object[3][5][VECTORS];
Pixmap	buffer;
GC	gcbuffer,dbgc;

struct timeval tv,tv1,tv0;
struct timezone tz;

int 	frames,framec,
	xdir=4,ydir=4,
	tic,
	softdbf,harddbf,
	frametime=1000000/30;

char	fps[64],
	framedef[64],
	bufferdef[64],
	status[128];

int red,green,blue,color,grey[16];

void draw(struct xtree *treeptr) {

	int f,ticd;

	gettimeofday(&tv1,&tz);

	ticd=tv1.tv_usec/frametime;
	if(tic==ticd) return;
	tic=ticd;

	frames++;
	framec++;
	
	if(frames%32>=16) 
		color=15-frames%16;
	else
		color=frames%16;
	
	f=frames%5;
	memcpy(&tv0,&tv1,sizeof(tv));

	if(!tic) { 
	
		sprintf(status,"Buffer 1: %dx%d (%d fps)",
			treeptr->w,treeptr->h,
			tv1.tv_sec-tv.tv_sec?framec/(tv1.tv_sec-tv.tv_sec):0);
	}

	if(softdbf) {

		XSetForeground(display,gcbuffer,grey[color]);
		XFillRectangle(display,buffer,gcbuffer,0,0,treeptr->w,treeptr->h);

		XSetForeground(display,gcbuffer,red);
		XFillPolygon(display,buffer,gcbuffer,
			object[0][frames%5],VECTORS,Nonconvex,CoordModeOrigin);
		XSetForeground(display,gcbuffer,green);
		XFillPolygon(display,buffer,gcbuffer,
			object[1][4-(frames-1)%5],VECTORS,Nonconvex,CoordModeOrigin);
		XSetForeground(display,gcbuffer,blue);
		XFillPolygon(display,buffer,gcbuffer,
			object[2][frames%5],VECTORS,Nonconvex,CoordModeOrigin);

		XSetForeground(display,gcbuffer,white);
        	XDrawString(display,buffer,gcbuffer,
        	        4,24,status,strlen(status));

		XCopyArea(display,buffer,treeptr->win,treeptr->gc,0,0,treeptr->w,treeptr->h,0,0);
		return;
	}
	if(harddbf) {

		XSetForeground(display,dbgc,grey[color]);
		XFillRectangle(display,dbbuffer,dbgc,0,0,treeptr->w,treeptr->h);

		XSetForeground(display,dbgc,red);
		XFillPolygon(display,dbbuffer,dbgc,
			object[0][frames%5],VECTORS,Nonconvex,CoordModeOrigin);
		XSetForeground(display,dbgc,green);
		XFillPolygon(display,dbbuffer,dbgc,
			object[1][4-(frames-1)%5],VECTORS,Nonconvex,CoordModeOrigin);
		XSetForeground(display,dbgc,blue);
		XFillPolygon(display,dbbuffer,dbgc,
			object[2][frames%5],VECTORS,Nonconvex,CoordModeOrigin);

		XSetForeground(display,dbgc,white);
        	XDrawString(display,dbbuffer,dbgc,
        		4,24,status,strlen(status));
		
		XdbeSwapBuffers(display,&dbinfo,1);
		return;
	}
	XSetForeground(display,treeptr->gc,grey[color]);
	XFillRectangle(display,treeptr->win,treeptr->gc,0,0,treeptr->w,treeptr->h);

	XSetForeground(display,treeptr->gc,red);
	XFillPolygon(display,treeptr->win,treeptr->gc,
		object[0][frames%5],VECTORS,Nonconvex,CoordModeOrigin);
	XSetForeground(display,treeptr->gc,green);
	XFillPolygon(display,treeptr->win,treeptr->gc,
		object[1][4-(frames-1)%5],VECTORS,Nonconvex,CoordModeOrigin);
	XSetForeground(display,treeptr->gc,blue);
	XFillPolygon(display,treeptr->win,treeptr->gc,
		object[2][frames%5],VECTORS,Nonconvex,CoordModeOrigin);
	XSetForeground(display,treeptr->gc,white);
        XDrawString(display,treeptr->win,treeptr->gc,
        	4,24,status,strlen(status));
	XFlush(display);
}

void control_apply(struct xtree *treeptr) {

	frametime=1000000/atoi(framedef);	

	softdbf=harddbf=0;
	if(!strcmp(bufferdef,"Double Buffered (pixmap)")) softdbf=1;
	if(!strcmp(bufferdef,"Double Buffered (DBE)")) harddbf=1;
	
	gettimeofday(&tv,&tz);
	framec=0;
}

void control_ok(struct xtree *treeptr) {

	control_apply(treeptr);
	window_close(treeptr);
}

void control(struct xtree *treeptr) {

	dialogbox_create(400,180);

	strcpy(bufferdef,"Single Buffered");
	if(softdbf) strcpy(bufferdef,"Double Buffered (pixmap)");
	if(harddbf) strcpy(bufferdef,"Double Buffered (DBE)");

	switch(atoi(framedef)) {
		case 15: strcpy(framedef,"15 frames/s"); break;
		case 30:
		default: strcpy(framedef,"30 frames/s"); break;
		case 25: strcpy(framedef,"25 frames/s"); break;
	}

	label_create(0,0,0,90,"",gray,up);

	radio_create(8,8+0*24,200-8,21,"15 frames/s",framedef);
	radio_create(8,8+1*24,200-8,21,"25 frames/s",framedef);
	radio_create(8,8+2*24,200-8,21,"30 frames/s",framedef);

	radio_create(200,8+0*24,200-8,21,"Single Buffered",bufferdef);
	radio_create(200,8+1*24,200-8,21,"Double Buffered (pixmap)",bufferdef);
	radio_create(200,8+2*24,200-8,21,"Double Buffered (DBE)",bufferdef);

	label_create(0,-90,0,0,"",gray,up);

	button_create(-168,-8,72,24,"Ok",control_ok);
	button_create(-88,-8,72,24,"Cancel",window_close);
	button_create(-8,-8,72,24,"Apply",control_apply);
}

void xmain(int n,char *p[]) {

	int i,j,f,g;

	animate=1;
	for(g=0;g!=3;g++)
	for(j=0;j!=18;j++) 
	for(f=0;f!=5;f++) {

		i=j*20+f*4;
		object[g][f][j*4+0].x=(int)(310*g+210+cos(i*PI/180)*140.0); 
		object[g][f][j*4+0].y=(int)(210+sin(i*PI/180)*140.0);
		i+=10;
		object[g][f][j*4+1].x=(int)(310*g+210+cos(i*PI/180)*140.0); 
		object[g][f][j*4+1].y=(int)(210+sin(i*PI/180)*140.0);
		i+=2;
                object[g][f][j*4+2].x=(int)(310*g+210+cos(i*PI/180)*170.0); 
		object[g][f][j*4+2].y=(int)(210+sin(i*PI/180)*170.0);
		i+=6;
		object[g][f][j*4+3].x=(int)(310*g+210+cos(i*PI/180)*170.0); 
		object[g][f][j*4+3].y=(int)(210+sin(i*PI/180)*170.0);
	}

	red=getcolor(0xffff,0,0);
	green=getcolor(0,0xffff,0);
	blue=getcolor(0,0,0xffff);

	for(i=0;i!=16;i++) grey[i]=getcolor(i*2000,i*2000,i*2000);

	window_create(16,16,512,384,"XStep2D Animation");

        treestk->gc=XCreateGC(display,treestk->win,0,&values);
        XSetFont(display,treestk->gc,helvetica24b->fid);	
        XSetLineAttributes(display,treestk->gc,0,LineSolid,CapRound,JoinRound);
	treestk->broadcast=draw;

       	buffer=XCreatePixmap(display,treestk->win,
       		DisplayWidth(display,screen),
       		DisplayHeight(display,screen),
       		DefaultDepth(display,screen));

	gcbuffer=XCreateGC(display,buffer,0,&values);
        XSetFont(display,gcbuffer,helvetica24b->fid);	
	dbbuffer=XdbeAllocateBackBufferName(display,treestk->win,XdbeBackground);
	dbgc=XCreateGC(display,dbbuffer,0,&values);
        XSetFont(display,dbgc,helvetica24b->fid);	
	dbinfo.swap_window=treestk->win;
	dbinfo.swap_action=XdbeBackground;

	gettimeofday(&tv,&tz);
	gettimeofday(&tv0,&tz);

	button_create(-88,-8,72,24,"Control",control);
	button_create(-8,-8,72,24,"Quit",window_close);
}
