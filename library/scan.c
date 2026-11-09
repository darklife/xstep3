/*
 
XSTEP 3.4 - Toolkit for X-Window System
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

#ifdef SHOWEVENT
	#include "debug.c"
#endif

void xbroad(struct xtree *treeptr) {

	if(!treeptr) return;

        if(treeptr->todelete==treeptr->win) {

                window_delete(treeptr);
                return;
        }

	if(treeptr->broadcast) treeptr->broadcast(treeptr);

	xbroad(treeptr->treestk);
	xbroad(treeptr->next);
}

int xscan(struct xtree *treeptr) {

	if(!treeptr) return(0);

	if(report.xany.window==treeptr->win) {

#ifdef SHOWEVENT

		if(report.type) {
			printf("event %d:%s to %d(%s): ",
				(int)report.type,
				geteventname(report.type),
				(int)treeptr->win,
				treeptr->t);
			fflush(stdout);
		}

#endif
		switch(report.type) {

			case KeyPress: 		
				if(treeptr->keypress) 
					treeptr->keypress(treeptr); 
				break;
					
			case KeyRelease: 
				if(treeptr->keyrelease) 
					treeptr->keyrelease(treeptr); 
				break;
				
			case ButtonPress: 
				if(treeptr->buttonpress) 
					treeptr->buttonpress(treeptr); 
				break;
				
			case ButtonRelease: 
				if(treeptr->buttonrelease) 
					treeptr->buttonrelease(treeptr); 
				break;
				
			case MotionNotify: 
				if(treeptr->buttonmotion) 
					treeptr->buttonmotion(treeptr); 
				break;

			case FocusIn: 
				if(treeptr->focusin) 
					treeptr->focusin(treeptr); 
				break;

			case FocusOut: 
				if(treeptr->focusout) 
					treeptr->focusout(treeptr); 
				break;

			case Expose:
				
				if(treeptr->expose) {
					
					clipmask[exposec].x=report.xexpose.x;
					clipmask[exposec].y=report.xexpose.y;
					clipmask[exposec].width=report.xexpose.width;
					clipmask[exposec].height=report.xexpose.height;
												
					if(!report.xexpose.count) {
#ifdef SHOWFLUSHEXPOSE

						if(exposec) printf("%d: flushed %d exposes\n",
							report.xany.window,
							exposec+1);

#endif
						XSetClipRectangles(display,treeptr->gc,
							0,0,clipmask,exposec+1,Unsorted);
						
						exposec=0;
						treeptr->expose(treeptr);

						clipmask[0].x=0;
						clipmask[0].y=0;
						clipmask[0].width=treeptr->aw;
						clipmask[0].height=treeptr->ah;
						XSetClipRectangles(display,treeptr->gc,
							0,0,clipmask,1,Unsorted);
						break;
					} else {
#ifdef SHOWCACHEEXPOSE
						printf("%d: cached %d exposes, %d left\n",
							report.xany.window,
							exposec,report.xexpose.count);
#endif								
						exposec++;
						if(exposec==EXPOSECACHESIZE) {
							printf("%lx: expose crash: too many exposes! (left %d)\n",
								report.xany.window,
								report.xexpose.count);
							exposec=0;
						}
					}
				}
				break;

			case ConfigureNotify:
				
				if(treeptr->configure)
					treeptr->configure(treeptr);
				break;
					
			case ClientMessage:

				if(treeptr->message)
					treeptr->message(treeptr);
				break;
		}
#ifdef SHOWEVENT

		if(report.type) printf("done\n");

#endif
		return(1);
	}
	if(xscan(treeptr->treestk)) return(1);
	if(xscan(treeptr->next)) return(1);
	
	return(0);
}
