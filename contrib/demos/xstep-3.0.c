/*
XSTEP 3.0 - Toolkit for X-Window System
Copyright (C) 1996,1997 by Marcelo Samsoniuk

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

#include <math.h>
#include <unistd.h>
#include <xstep-port.h>
 
int i; 

char buffer[80];
char check[4][32];
char status[128];

int ptr1,ptx1=0,max1=26; char *list1[26]={ "Alpha","Beta","Charlie","Delta","Echo","Foxtrot","Golf","Hotel","India","Juliet","Kilo","Lima","Mike","November","Oscar","Papa","Quebec","Romeo","Sierra","Tango","Uniform","Victor","Whiskey","Xray","Yankee","Zulu", };
int ptr2,ptx2=0,max2=12; char *list2[12]={ "speak to me","breathe","on the run","time","the great gig in the sky","money","us and then","any colour you like","brain damage","eclipse","wish you were here","comfortably numb", };

void draw(struct xtree *treeptr) {

	XSetForeground(display,treeptr->gc,rand());
	XFillRectangle(display,treeptr->win,treeptr->gc,
		rand()%treeptr->w,rand()%treeptr->h,
		rand()%treeptr->w,rand()%treeptr->h);
	XFlush(display);
}

void xlink(struct xtree *treeptr) {

	char buffer1[256],buffer2[256];
	FILE *fd;
	
	fd=popen("/usr/X11R6/bin/xwininfo","r");
	while(fgets(buffer2,256,fd)) {
		if(!strncmp("xwininfo: Window id: ",buffer2,strlen("xwininfo: Window id: "))) {
			sprintf(buffer1,
				"xstep 0x%lx &",
				strtoul(buffer2+strlen("xwininfo: Window id: "),NULL,0));
			system(buffer1);
			break;
		}
	}
	pclose(fd);
}

void fx(struct xtree *treeptr) { sprintf(buffer,"%s: %s",list1[ptr1],list2[ptr2]); }

void xapplication(struct xtree *treeptr) {

	int i;

	window_create(0,0,16+16+200+200+200+200+8,16+17*24+21,"XSTEP 3.x - Product Preview");

	for(i=0;i!=15;i++) button_create(8,8+i*24,200,21,"OpenWARE Button",xapplication);

	button_create(8,8+15*24,200,21,"Link Application",xlink);
	button_create(8,8+16*24,200,21,"Exit",(void (*)(struct xtree *))exit);

	radio_create(16+200,8+0*24,200,21,"OpenWARE for Pentium",buffer);
	radio_create(16+200,8+1*24,200,21,"OpenWARE for PPRO",buffer);
	radio_create(16+200,8+2*24,200,21,"OpenWARE for ix86",buffer);
	radio_create(16+200,8+3*24,200,21,"OpenWARE for PowerPC",buffer);

	check_create(16+200,8+5*24,200,21,"OpenWARE for Alpha",check[0]);
	check_create(16+200,8+6*24,200,21,"OpenWARE for MIPS R4x00",check[1]);
	check_create(16+200,8+7*24,200,21,"OpenWARE for SPARC",check[2]);

	edit_create(16+200,8+ 9*24,200,21,buffer,52,0);
	edit_create(16+200,8+10*24,200,21,buffer,52,0);
	edit_create(16+200,8+11*24,200,21,buffer,52,0);
	edit_create(16+200,8+12*24,200,21,buffer,52,0);

	label_create(16+200,8+14*24,200,21,buffer,white,left);
	label_create(16+200,8+15*24,200,21,buffer,gray,center);
	label_create(16+200,8+16*24,200,21,buffer,darkgray,right);
	label_create(8,8+17*24,408,21,status,black,center);

	box_create(424,24+164+164,408,17*24+21-164-164-16,draw,black);

	scroll_create(424,8,200,164,&ptx1,&max1,list1,0);
	scroll_create(424,180,200,164,&ptr1,&ptx1,list1,fx);
	scroll_create(424+208,8,200,164,&ptx2,&max2,list2,0);
	scroll_create(424+208,180,200,164,&ptr2,&ptx2,list2,fx);
}

void main(int n,char *p[]) {

	xinit("");

	if(n==2) rootwindow=(Window)strtoul(p[1],NULL,0);
	else rootwindow=RootWindow(display,screen);

	xapplication(NULL);
	for(i=0;treestk;i++) {
		report.type=0;
		if(XPending(display)) {
			XNextEvent(display,&report);
			if(report.type) {
				if(!(i%10)) {
					sprintf(status,
						"%d: received event [%d] in windows [%d]",
						i,report.xany.type,
						(int)report.xany.window);
				}
				broadcast++;
			}
		} else usleep(1);
		broadcast++;
		for(xscan(treestk);broadcast;broadcast--) xbroad(treestk);
	}
}
