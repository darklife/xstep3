#include <xstep.h>
#include <time.h>

char message[256];
int reference,counter,max,checkbytime;

void drawf(struct xtree *t) {

	counter++;

	if(counter==max) window_close(t);
	
	if(checkbytime) {
	
		if(reference==time(0)) return;
		reference=time(0);

	} else if(counter%16) return;
	
	sprintf(message,"working %d of %d...",counter,max);

	downbox(0,0,t->aw,t->ah,gray,t);

	XSetForeground(display,t->gc,getcolor(0xffff,0,0));
	XFillRectangle(display,t->win,t->gc,3,3,(t->aw-6)*counter/max,t->ah-6);
	XSetForeground(display,t->gc,black);

        XDrawString(display,t->win,t->gc,
                (t->aw-XTextWidth(defaultfont,message,strlen(message)))/2,
                t->ah/2+4,message,strlen(message));

	broadcast++;
}

void bytime(struct xtree *t) {

        dialog_create(400,180);

        defaultfont=helvetica24b;
        label_create(0,0,0,-90,"Working",gray,up);
        defaultfont=helvetica12m;

       	label_create(0,-90,0,0,"",gray,up);
	box_create(8,98,-8,21,drawf,gray);

        button_create( -8,-8,72,24,"Cancel",window_close);

	counter=0;
	checkbytime=1;
}

void bycount(struct xtree *t) {

        dialog_create(400,180);

        defaultfont=helvetica24b;
        label_create(0,0,0,-90,"Working",gray,up);
        defaultfont=helvetica12m;

       	label_create(0,-90,0,0,"",gray,up);
	box_create(8,98,-8,21,drawf,gray);

        button_create( -8,-8,72,24,"Cancel",window_close);
        
        counter=0;
        checkbytime=0;
}

void xmain(int i,char **p) {

	max=1000;

	window_create(0,0,512,384,p[0]);	

	button_create( -88,-8,72,24,"by time", bytime);
	button_create(-168,-8,72,24,"by count",bycount);

	button_create( -8,-8,72,24,"OK",window_close);

	button_create( -8,-8,72,24,"OK",window_close);

	animate=1;
}
