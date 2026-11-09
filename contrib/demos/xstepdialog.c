#include <xstep.h>

char label[50][256],flag[50][256];

void f(struct xtree *t) {

	int i;
	
	for(i=0;i!=50;i++) {

		sprintf(label[i],"option %d",i);
		check_create(8,8+i*21,200,21,label[i],flag[i]);
	}
}

void xmain(int i,char **p) {

	window_create(0,0,400,400,"x/y scroll");

	xyscroll_group(8,8,-8,-40,f,gray,0,50*21+16);
	button_create(-8,-8,72,24,"Quit",window_close);
}
