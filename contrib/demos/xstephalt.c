#include <xstep.h>

void rebootf(struct xtree *t) { system("/sbin/reboot"); }
void haltf(struct xtree *t) { system("/sbin/halt"); }

void xmain(int n,char **p) {

	dialog_create(400,180);

	label_create(0,0,0,90,"Warning!",gray,up);
	XSetFont(display,treestk->treestk->gc,
		(treestk->treestk->font=helvetica24b)->fid);
	label_create(0,90,0,0,"",gray,up);

        label_create(0,92,0,-(24+18),
		"You are sure to shutdown this computer?",
		invisible,center);

	button_create(-(8+80*2),-8,72,24,"Cancel",	window_close);
	button_create(-(8+80*1),-8,72,24,"Restart",	rebootf);
	button_create(-(8+80*0),-8,72,24,"Shutdown",	haltf);
}

