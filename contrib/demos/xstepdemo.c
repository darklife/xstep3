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

#include <math.h>
#include <unistd.h>
#include <xstep.h>

int i;

char buffer[80],buffer2[80],buffer3[80];
char check[4][32];
char status[128];

int ptr1,ptx1=0,max1=26; 

char *list1[26]={ "Alpha","Beta","Charlie","Delta","Echo",
     "Foxtrot","Golf","Hotel","India","Juliet","Kilo",
     "Lima","Mike","November","Oscar","Papa","Quebec",
     "Romeo","Sierra","Tango","Uniform","Victor","Whiskey",
     "Xray","Yankee","Zulu", };

struct mlist mlist1[]={
	{ "list 1",list1,100  },
	{ "list 2",list1,100  },
	{ "list 3",list1,100  },
	{ "list 4",list1,100  },
};

int ptr2,ptx2=0,max2=12; 

char *list2[12]={ "speak to me","breathe","on the run","time",
     "the great gig in the sky","money","us and then",
     "any colour you like","brain damage","eclipse",
     "wish you were here","comfortably numb", };

void draw(struct xtree *treeptr) {

	downbox(0,0,treeptr->aw,treeptr->ah,invisible,treeptr);
	XSetForeground(display,treeptr->gc,black);
	XDrawLine(display,treeptr->win,treeptr->gc,
		8,8,treeptr->aw-8,treeptr->ah-8);
	XDrawLine(display,treeptr->win,treeptr->gc,
		8,treeptr->ah-8,treeptr->aw-8,8);
}

void xlink(struct xtree *treeptr) {

        char buffer1[256],buffer2[256];
        FILE *fd;

        fd=popen("/usr/X11R6/bin/xwininfo","r");
        
        while(fgets(buffer2,256,fd)) {
                if(!strncmp("xwininfo: Window id: ",
                   buffer2,strlen("xwininfo: Window id: "))) {
                        sprintf(buffer1,
                                "xdemo -wid 0x%lx &",
                                strtoul(buffer2+
                                        strlen("xwininfo: Window id: "),
                                        NULL,0));
                        system(buffer1);
                        break;
                }
        }
        pclose(fd);
}

void fx(struct xtree *treeptr) { 

	sprintf(buffer,"%s: %s",
	        list1[ptr1],
	        list2[ptr2]); 

}

void xapplication4(struct xtree *treeptr) {

	window_close(treeptr);
//	treeptr->parent->delete=treeptr->parent->win;
	broadcast++;
	window_create(48,48,400,400,"Resizable Window");
	box_create(8,8,-8,-8,draw,gray);
}

void xapplication3(struct xtree *treeptr) {



	dialogbox_create(400,180);
	label_create(0,0,0,-90,"Continue ?",gray,up);
	XSetFont(display,treestk->treestk->gc,(treestk->treestk->font=helvetica24b)->fid);
	label_create(0,-90,0,0,"",gray,up);
	button_create(-(8+80*0),-8,72,24,"OK",xapplication4);
	button_create(-(8+80*1),-8,72,24,"Cancel",window_close);
}

void xapplication2(struct xtree *treeptr) {

	int i;

	window_create(32,32,16+16+200+200+200+200+8,16+17*24+21,
		"Unresizable Window");
		
	for(i=0;i!=14;i++) button_create(8,8+i*24,200,21,"Resizable Graphic",xapplication3);

	button_create(8,8+15*24,200,21,"Link Window",xlink);
	button_create(8,8+16*24,200,21,"Exit Applications",(void (*)(struct xtree *))exit);

	radio_create(16+200,8+0*24,200,21,"Single Buffered Window",buffer);
	radio_create(16+200,8+1*24,200,21,"Double buffered Window",buffer);
	check_create(16+200,8+3*24,200,21,"Enable GLX",check[0]);
	check_create(16+200,8+4*24,200,21,"Enable XIE",check[1]);
	popup_create(16+200,8+5*24,200,21,buffer2,list1,max1*21);
	popup_create(16+200,8+6*24,200,21,buffer3,list2,max2*21);

	edit_create(16+200,8+ 9*24,200,21,buffer,52,0);
	edit_create(16+200,8+10*24,200,21,buffer,52,0);
	edit_create(16+200,8+11*24,200,21,buffer,52,0);
	edit_create(16+200,8+12*24,200,21,buffer,52,0);

	label_create(16+200,8+14*24,200,21,buffer,white,left);
	label_create(16+200,8+15*24,200,21,buffer,gray,center);
	label_create(16+200,8+16*24,200,21,buffer,darkgray,right);
	label_create(8,8+17*24,408,21,status,black,center);

	scroll_create(424,8,200,164,&ptx1,&max1,list1,0);
	scroll_create(424,180,200,164,&ptr1,&ptx1,list1,fx);
	scroll_create(424+208,8,200,164,&ptx2,&max2,list2,0);
	scroll_create(424+208,180,200,164,&ptr2,&ptx2,list2,fx);
}

void menuf(struct xtree *treeptr) {

        switch(treeptr->l) {
                case 0:
			sprintf(buffer,"Linux selected");
			break;
                default:
			sprintf(buffer,"Other unix selected");
			break;
        }		
}


void menu(struct xtree *treeptr) {

        char static *z[10]={                                             
                "Linux",                                                 
                "FreeBSD",                                               
                "NetBSD",                                                
                "OpenBSD",                                               
                "NeXTSTEP",                                              
                "UnixWare",                                              
                "Solaris",                                               
                "SunOS",                                                 
                "Irix",                                                  
                "Minix",                                                 
        };

        menu_create(0,0,120,10*21,z,0,menuf);
}

void xmain(int n,char *p[]) {
                                                               
	window_create(16,16,600,600,"Main Resizable Window");
	
	button_create(8+128*0,8,120,21,"Link Application",xlink);
	button_create(8+128*1,8,120,21,"Unresizable Window",xapplication2);
	button_create(-8,-8,200,21,"Exit Applications",(void (*)(struct xtree *))exit); 

	edit_create(8,21+16,-8,21,buffer,52,0);

	scroll_create(8,21+16+29,200,200,&ptx1,&max1,list1,0);
	scroll_create(216,21+16+29,-8,200,&ptr1,&ptx1,list1,fx);
	mscroll_create(8,21+16+29+208,-8,-(21+16),&ptr1,&ptx1,mlist1,fx,xapplication2,menu,4);
}
