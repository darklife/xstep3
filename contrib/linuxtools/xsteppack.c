#include <xstep.h>

FILE 	*f;
char 	working[1024];
int 	lock;

char 	*pklist[10000];
int	pkmax,pkptr;

void startf(struct xtree *t) {

	animate=1;
}

void stopf(struct xtree *t) {

	*working=0;
	lock=animate=0;
	window_close(t);
	pclose(f);
	t->broadcast=0;
	f=0;
}

void installpack(struct xtree *t) {

	char package[1024];

	if(!animate) return;
	XFlush(display);

	if(!f) {

		sprintf(working,"preparing to install %s, please wait...",pklist[pkptr]);
		sprintf(package,"installpack %s",pklist[pkptr]);
		f=popen(package,"r");
		return;
	}

	if(!fgets(working,1024,f)) {

		*working=0;
		window_close(t);
		lock=animate=0;
		f=0;
		pclose(f);
		t->broadcast=0;
		return;
	}
}

void installf(struct xtree *t) {

	struct xtree *win;

	if(lock) return;
	lock=1;
	animate=0;

	dialog_create(400,180);

        defaultfont=helvetica24b;
        label_create(0,0,0,-90,"Install Package",gray,up);
        defaultfont=helvetica12m;

        win=label_create(0,-90,0,0,"",gray,up);
        win->broadcast=installpack;

	sprintf(working,"Install package %s ?",pklist[pkptr]);
        label_create(8,-(24+16),0,40,
                working,
                invisible,left);

        button_create(-88,-8,72,24,"Install",startf);
        button_create( -8,-8,72,24,"Cancel",stopf);
}

void removepack(struct xtree *t) {

	char package[1024];

	if(!animate) return;
	XFlush(display);

	if(!f) {

		sprintf(working,"preparing to remove %s, please wait...",pklist[pkptr]);
		sprintf(package,"removepack %s",pklist[pkptr]);
		f=popen(package,"r");
		return;
	} 

	if(!fgets(working,1024,f)) {

		*working=0;
		lock=animate=0;
		window_close(t);
		pclose(f);
		t->broadcast=0;
		f=0;
		return;
	}
	
}

void removef(struct xtree *t) {

	struct xtree *win;

	if(lock) return;
	lock=1;
	animate=0;


	dialog_create(400,180);

        defaultfont=helvetica24b;
        label_create(0,0,0,-90,"Remove Package",gray,up);
        defaultfont=helvetica12m;

        win=label_create(0,-90,0,0,"",gray,up);
        win->broadcast=removepack;

	sprintf(working,"Remove package %s ?",pklist[pkptr]);        
        label_create(8,-(24+16),0,40,
                working,
                invisible,left);

        button_create(-88,-8,72,24,"Remove",startf);
        button_create( -8,-8,72,24,"Cancel",stopf);
}

void scanpack(struct xtree *t) {

	char path[1024];

	if(!animate) return;
	XFlush(display);

	if(!f) {
		if(argc==2)
			sprintf(path,"find %s -name '*.tgz'",argv[1]);
		else
			sprintf(path,"locate '.tgz' '.tar.gz'");

		f=popen(path,"r");
		return;
	}

	if(!fgets(working,1024,f)) {

		window_create(16,16,600,600,"Xstep Package Manager");

		label_create(8,8,-8,21,"Packages",darkgray,center);
		scroll_create(8,8+21,-8,-40,&pkptr,&pkmax,pklist,0);

		button_create(-(8+80*0),-8,72,24,"Quit",window_close);
		button_create(-(8+80*1),-8,72,24,"Install",installf);
		button_create(-(8+80*2),-8,72,24,"Remove",removef);
		
		*working=0;
		window_close(t);
		lock=animate=0;
		pclose(f);
		t->broadcast=0;
		f=0;
		return;
	}
	
	pklist[pkmax]=(char *)malloc(1+strlen(working));
	strcpy(pklist[pkmax],working);
	pkmax++;
}

void xmain(int i,char **p) {

	struct xtree *win;

	dialog_create(400,180);

        defaultfont=helvetica24b;
        label_create(0,0,0,-90,"Scanning for TGZ packages...",gray,up);
        defaultfont=helvetica12m;

        win=label_create(0,-90,0,0,"",gray,up);
        win->broadcast=scanpack;
	animate=1;

        label_create(8,-(24+16),0,40,
                working,
                invisible,left);

	button_create(-8,-8,72,24,"Cancel",window_close);
}
