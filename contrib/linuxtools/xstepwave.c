#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <xstep.h>

int 		color[16],
		dsp,
		frametime=1000000/16,
		tic,
		frame,
		count,
		sec;

unsigned char 	data[16][500];

void draw(struct xtree *t) {

	int i,j;
	struct timeval 	tv;
	struct timezone tz;
	char ref[64];

	gettimeofday(&tv,&tz);

	i=tv.tv_usec/frametime;

	if(tic==i) return;
	if(!read(dsp,data[0],500)) return;

	tic=i;
        
	XClearWindow(display,t->win);

	for(j=16-1;j!=-1;j--) {

		XSetForeground(display,t->gc,color[j]);

		for(i=0;i!=256-1;i++) {

			XDrawLine(display,t->win,t->gc,
				16+j*16+i,64+i+data[j][i*2]/2-j*4,
				16+j*16+i+1,64+i+data[j][(i+1)*2]/2-j*4);
		}
	}

	if(tv.tv_sec!=sec) {

		if(frame!=16)
			fprintf(stderr,"%d: lost %d frames\n",count,16-frame);
	
		sec=tv.tv_sec;
		count++;
		frame=0;
	}

	sprintf(ref,"frame %d.%02d",count,frame++);

	XSetForeground(display,t->gc,white);

	XDrawString(display,t->win,t->gc,
		8,16,ref,strlen(ref));

	XFlush(display);

	for(j=16-1;j!=0;j--) 
		for(i=0;i!=500;i++)
			data[j][i]=data[j-1][i];
}

void xmain(int i,char **p) {

	dsp=open("/dev/dsp",O_RDONLY|O_NONBLOCK);

	if(dsp==-1) {
	
		dialogbox_create(400,180);
	
	        defaultfont=helvetica24b;
	        label_create(0,0,0,-90,"Fatal Error!",gray,up);
	        defaultfont=helvetica12m;

	        label_create(0,-90,0,0,"",gray,up);
	        label_create(0,-(24+16),0,40,
	                "Can't open /dev/dsp device!",
	                invisible,center);

	        button_create( -8,-8,72,24,"OK",      window_close);

		return;
	}

	for(i=1;i!=16;i++) color[i]=getcolor(0,0xA000-i*(0x6000/16),0);

	color[0]=getnamedcolor("yellow");

	window_create(0,0,620,420,"xwave3d");
	box_create(0,0,0,0,draw,black);

	button_create(-8,-8,72,24,"Done",window_close);

	animate=1;
}
