/*

XUSER 2.0 - X User Manager for Linux
Copyright (C) 1997-99 by Marcelo Samsoniuk

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

#include <stdlib.h>
#include <unistd.h>
#include <shadow.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <time.h>
#include <xstep.h>

#define MAXUSERS 10000
#define MAXGROUPS 1000

struct passwd 	*pw;
struct spwd	*sw;
struct group	*gr,groups[MAXGROUPS];

char 	*grlist[MAXGROUPS],
	*grxlist[MAXUSERS],
	*grx2list[MAXUSERS],
	adduser[64],addgroup[64];

char *removefile[64];

struct user {

	struct passwd 	pw;
	struct spwd 	sw;

} users[MAXUSERS];

int 	pwsize,pwptr,
	grsize,grptr,
	grx2size,grx2ptr,
	grxsize,grxptr;

struct {

	char 	*login	[MAXUSERS];
	char 	*group	[MAXUSERS];
	char 	*name	[MAXUSERS];
	char 	*phone	[MAXUSERS];
} pwbuf;

struct mlist pwlist[]={

	{ "Login",		pwbuf.login,	72 },
	{ "Group",		pwbuf.group,	72 },
	{ "Phone",		pwbuf.phone,	90 },
	{ "Name",		pwbuf.name,	0 },
};

struct {

	char 	login	[32];
	char 	passwd	[32];
	char 	uid	[8];
	char 	group	[32];
	char 	name	[128];
	char 	office	[128];
	char 	nphone	[16];
	char 	ophone	[16];
	char 	home	[64];
	char 	shell	[64];
	char 	lstchg	[64];
	char	expire	[64];
	char 	min	[8];
	char 	max	[8];
	char	warn	[8];
	char	inact	[8];
	char	flag	[8];
} pwedit;

char *getstr(char *p) {

	char *q;
	
	q=(char *)malloc(1+strlen(p));
	strcpy(q,p);
	return(q);
}

char *getfield(char p[],int n,char s) {

	int i,j,k;
	char *t;

	k=strlen(p);
	for(i=j=0;i!=k&&j!=n;i++) if(p[i]==s) j++;
	for(j=i;j!=k;j++) if(p[j]==s) break;
	t=(char *)malloc(1+j-i);
	strncpy(t,p+i,j-i);
	t[j-i]=0;
	return(t);
}

void pwsavef(struct xtree *treeptr) {

	FILE *f1,*f2,*f3;
	int i,j;

	f1=fopen("/etc/passwd.new","w");
	f2=fopen("/etc/shadow.new","w");

	for(i=0;i!=pwsize;i++) {
		
		putpwent(&users[i].pw,f1);
		putspent(&users[i].sw,f2);
	}
	fclose(f1);
	fclose(f2);

	f3=fopen("/etc/group.new","w");

	for(i=0;i!=grsize;i++) {
	
		fprintf(f3,"%s:%s:%d:",
			groups[i].gr_name,groups[i].gr_passwd,
			(int)((short)groups[i].gr_gid));

		for(j=0;groups[i].gr_mem[j];j++) {

			fprintf(f3,"%s",groups[i].gr_mem[j]);
			if(groups[i].gr_mem[j+1]) 
				fprintf(f3,",");
		}
		fprintf(f3,"\n");
	}
	fclose(f3);

	/* well. this isn't really necessary, but... */

	system("mkdir /etc/backup && cp -vp /etc/passwd /etc/shadow /etc/group /etc/backup");

	/* done! */

	rename("/etc/passwd.new","/etc/passwd");
	rename("/etc/shadow.new","/etc/shadow");
	rename("/etc/group.new","/etc/group");
}

void grx2f(struct xtree *treeptr) {

	for(grx2size=0;groups[grptr].gr_mem[grx2size];grx2size++)

		grx2list[grx2size]=groups[grptr].gr_mem[grx2size];
}

void pwlistf(struct xtree *treeptr) {

	int i,j;
	char *t;

	strcpy(pwedit.login,	users[pwptr].pw.pw_name);
	strcpy(pwedit.passwd,	users[pwptr].sw.sp_pwdp);
	strcpy(pwedit.group,	pwbuf.group[pwptr]);
	strcpy(pwedit.name,	t=getfield(users[pwptr].pw.pw_gecos,0,',')); free(t);
	strcpy(pwedit.office,	t=getfield(users[pwptr].pw.pw_gecos,1,',')); free(t);
	strcpy(pwedit.nphone,	t=getfield(users[pwptr].pw.pw_gecos,2,',')); free(t);
	strcpy(pwedit.ophone,	t=getfield(users[pwptr].pw.pw_gecos,3,',')); free(t);
	strcpy(pwedit.home,	users[pwptr].pw.pw_dir);
	strcpy(pwedit.shell,	users[pwptr].pw.pw_shell);

	sprintf(pwedit.uid,	"%d",users[pwptr].pw.pw_uid);

	if((i=users[pwptr].sw.sp_lstchg)==-1) i=0; if(i) sprintf(pwedit.lstchg,	"%d",i); else strcpy(pwedit.lstchg,	"");
	if((i=users[pwptr].sw.sp_expire)==-1) i=0; if(i) sprintf(pwedit.expire,	"%d",i); else strcpy(pwedit.expire,	"");
	if((i=users[pwptr].sw.sp_min)	==-1) i=0; if(i) sprintf(pwedit.min,	"%d",i); else strcpy(pwedit.min,	"");
	if((i=users[pwptr].sw.sp_max)	==-1) i=0; if(i) sprintf(pwedit.max,	"%d",i); else strcpy(pwedit.max,	"");
	if((i=users[pwptr].sw.sp_warn)	==-1) i=0; if(i) sprintf(pwedit.warn,	"%d",i); else strcpy(pwedit.warn,	"");
	if((i=users[pwptr].sw.sp_inact)	==-1) i=0; if(i) sprintf(pwedit.inact,	"%d",i); else strcpy(pwedit.inact,	"");
	if((i=users[pwptr].sw.sp_flag)	==-1) i=0; if(i) sprintf(pwedit.flag,	"%d",i); else strcpy(pwedit.flag,	"");

        for(grxsize=i=0;i!=grsize;i++) 
                for(j=0;groups[i].gr_mem[j];j++) 
			if(!strcmp(groups[i].gr_mem[j],pwedit.login)) 
				grxlist[grxsize++]=getstr(groups[i].gr_name);
}

void adduseringroupf(struct xtree *treeptr) {

	int a,n;
	char **t;

	if((!*adduser)||(!*addgroup)) return;

	for(a=0;a!=grsize;a++) 
	if(!strcmp(addgroup,groups[a].gr_name)) {

		for(n=0;groups[a].gr_mem[n];n++);

		t=(char **)malloc((n+2)*sizeof(char *));

		for(n=0;groups[a].gr_mem[n];n++)
			t[n]=groups[a].gr_mem[n];
		t[n]=getstr(adduser);
		t[n+1]=0;
		free(groups[a].gr_mem);
		groups[a].gr_mem=t;
		window_close(treeptr);
		pwlistf(treeptr);
		broadcast++;
		break;
	}
}

void adduserf(struct xtree *treeptr) {

	strcpy(adduser,"");
	strcpy(addgroup,grlist[grptr]);

        dialogbox_create(400,180);

        label_create(0,0,0,90,"Add user in group",gray,up);
        XSetFont(display,treestk->treestk->gc,(treestk->treestk->font=helvetica24b)->fid);
        label_create(0,90,0,0,"",gray,up);
        label_create(0,98,60,21,"User",invisible,right);
	popup_create(68,98,100,21,adduser,		pwbuf.login,pwsize*21);
        button_create(-(8+80*1),-8,72,24,"Cancel",	window_close);
        button_create(-(8+80*0),-8,72,24,"OK",		adduseringroupf);
}

void greditf(struct xtree *treeptr) {

	grx2f(treeptr);

	window_create(48,48,512,384,"Group Manager");

	label_create(8,8,244,21,"Groups",darkgray,center);
	label_create(260,8,244,21,"Users",darkgray,center);

	scroll_create(8,29,244,-200,&grptr,&grsize,grlist,grx2f);
	scroll_create(260,29,244,-200,&grx2ptr,&grx2size,grx2list,0);

	button_create(-(8+80*3),-8,72,24,"New",		0);
	button_create(-(8+80*2),-8,72,24,"Delete",	0);
	button_create(-(8+80*1),-8,72,24,"Add user",	adduserf);
	button_create(-(8+80*0),-8,72,24,"Close",	window_close);
}



void pwdeletef(struct xtree *treeptr) {

	int i;

	free(users[pwptr].pw.pw_name);
	free(users[pwptr].pw.pw_passwd);
	free(users[pwptr].pw.pw_gecos);
	free(users[pwptr].pw.pw_dir);
	free(users[pwptr].pw.pw_shell);
	free(users[pwptr].sw.sp_pwdp);

	free(pwbuf.group[pwptr]);
	free(pwbuf.name [pwptr]);
	free(pwbuf.phone[pwptr]);

	for(i=pwptr;i!=pwsize-1;i++) {

		users[i]=users[i+1];
	
		pwbuf.login[i]=pwbuf.login[i+1];
		pwbuf.group[i]=pwbuf.group[i+1];
		pwbuf.name [i]=pwbuf.name [i+1];
		pwbuf.phone[i]=pwbuf.phone[i+1];
	}
	pwsize--;
	broadcast++;
	window_close(treeptr);
}

void pwdelconf(struct xtree *treeptr) {

        dialogbox_create(400,180);

        label_create(0,0,0,90,"Warning!",gray,up);
        XSetFont(display,treestk->treestk->gc,(treestk->treestk->font=helvetica24b)->fid);
        label_create(0,90,0,0,"",gray,up);
        label_create(0,92,0,-(24+18),"You are sure to remove this user from your computer?",invisible,center);
        button_create(-(8+80*1),-8,72,24,"Cancel",	window_close);
        button_create(-(8+80*0),-8,72,24,"OK",		pwdeletef);
}

void addgroupf(struct xtree *treeptr) {

	strcpy(adduser,pwbuf.login[pwptr]);
	strcpy(addgroup,"");

        dialogbox_create(400,180);

        label_create(0,0,0,90,"Add user in group",gray,up);
        XSetFont(display,treestk->treestk->gc,(treestk->treestk->font=helvetica24b)->fid);
        label_create(0,90,0,0,"",gray,up);
        label_create(0,98,60,21,"Group",invisible,right);
	popup_create(68,98,100,21,addgroup,		grlist,grsize*21);
        button_create(-(8+80*1),-8,72,24,"Cancel",	window_close);
        button_create(-(8+80*0),-8,72,24,"OK",		adduseringroupf);
}

void pweditf(struct xtree *treeptr) {

	int x,y,k=-200;

	window_create(32,32,512,384,"User Manager");		

	label_create(8,8,244,21,"Users",darkgray,center);
	label_create(260,8,244,21,"Groups",darkgray,center);
	y+=21;
	scroll_create(8,29,244,k,&pwptr,&pwsize,pwbuf.login,pwlistf);
	scroll_create(260,29,244,k,&grxptr,&grxsize,grxlist,0);

	x=8; y=k+28;
	label_create(-108,y,100,21,	"Default Group",invisible,right);
	popup_create(-8,y,100,21,	pwedit.group,	grlist,grsize*21);
	label_create(x,y,60,21,		"Login",	invisible,right); y+=29;
	label_create(x,y,60,21,		"Name",		invisible,right);
	label_create(-108,y,60,21,	"Phone",	invisible,right); y+=24;
	label_create(x,y,60,21,		"Office",	invisible,right);
	label_create(-108,y,60,21,	"Phone",	invisible,right); y+=24;
	label_create(x,y,60,21,		"Home",		invisible,right); x+=248;
	label_create(x,y,60,21,		"Shell",	invisible,right); y+=24; x-=248;
	
	label_create(x,y,60,21,		"Changed",	invisible,right); x+=124;
	label_create(x,y,60,21,		"Expire",	invisible,right); x+=124;
	label_create(x,y,60,21,		"Warning",	invisible,right); x+=124;
	label_create(x,y,60,21,		"Inactive",	invisible,right); x-=372; y+=24;
	label_create(x,y,60,21,		"Minimum",	invisible,right); x+=124;
	label_create(x,y,60,21,		"Maximum",	invisible,right); x+=124;
	label_create(x,y,60,21,		"Flags",	invisible,right); x+=124; 
	label_create(x,y,60,21,		"User ID",	invisible,right); x-=372; y+=24;

	x+=60; y=k+28;
	edit_create(x,y,100,21,		pwedit.login,	32,0); y+=29;
	edit_create(x,y,-160,21,	pwedit.name,	128,0);
	edit_create(-8,y,100,21,	pwedit.nphone,	16,0); 	y+=24;
	edit_create(x,y,-160,21,	pwedit.office,	128,0);
	edit_create(-8,y,100,21,	pwedit.ophone,	16,0);  y+=24;
	edit_create(x,y,188,21,		pwedit.home,	128,0); x+=248;
	edit_create(x,y,188,21,		pwedit.shell,	128,0); y+=24; x-=248;

	edit_create(x,y,64,21,		pwedit.lstchg,	64,0); 	x+=124;
	edit_create(x,y,64,21,		pwedit.expire,	64,0); 	x+=124;
	edit_create(x,y,64,21,		pwedit.warn,	8,0); 	x+=124;
	edit_create(x,y,64,21,		pwedit.inact,	8,0); 	x-=372; y+=24;
	edit_create(x,y,64,21,		pwedit.min,	8,0); 	x+=124;
	edit_create(x,y,64,21,		pwedit.max,	8,0); 	x+=124;
	edit_create(x,y,64,21,		pwedit.flag,	8,0); 	x+=124;
	edit_create(x,y,64,21,		pwedit.uid,	8,0);   x-=372; y+=24;

	button_create(-(8+80*3),-8,72,24,"New",		0);
	button_create(-(8+80*2),-8,72,24,"Delete",	pwdelconf);
	button_create(-(8+80*1),-8,72,24,"Add group",	addgroupf);
	button_create(-(8+80*0),-8,72,24,"Close",	window_close);
}



void xmain(int n,char *p[]) {

	for(pwsize=0;(pw=getpwent());pwsize++) {

		printf("debug: [%s]: ",pw->pw_name);
		fflush(stdout);
		
		users[pwsize].pw.pw_name	=getstr(pw->pw_name);
		users[pwsize].pw.pw_passwd	=getstr(pw->pw_passwd);
		users[pwsize].pw.pw_uid		=pw->pw_uid;
		users[pwsize].pw.pw_gid		=pw->pw_gid;
		users[pwsize].pw.pw_gecos	=getstr(pw->pw_gecos);
		users[pwsize].pw.pw_dir		=getstr(pw->pw_dir);
		users[pwsize].pw.pw_shell	=getstr(pw->pw_shell);

		if(!(sw=getspnam(pw->pw_name))) { 

			printf("*** shadow error ***\n"); 
			return; 
		}
		if(!(gr=getgrgid(pw->pw_gid)))  { 
		
			printf("*** group error ***\n"); 
			return; 
		}

		users[pwsize].sw.sp_namp	=users[pwsize].pw.pw_name;
		users[pwsize].sw.sp_pwdp	=getstr(sw->sp_pwdp);
		users[pwsize].sw.sp_lstchg	=sw->sp_lstchg;
		users[pwsize].sw.sp_min		=sw->sp_min;
	 	users[pwsize].sw.sp_max		=sw->sp_max;
		users[pwsize].sw.sp_warn	=sw->sp_warn;
		users[pwsize].sw.sp_inact	=sw->sp_inact;
		users[pwsize].sw.sp_expire	=sw->sp_expire;
		users[pwsize].sw.sp_flag	=sw->sp_flag;
		pwbuf.login[pwsize]		=users[pwsize].pw.pw_name;
		pwbuf.group[pwsize]		=getstr(gr->gr_name);
		pwbuf.name [pwsize]		=getfield(pw->pw_gecos,0,',');
		pwbuf.phone[pwsize]		=getfield(pw->pw_gecos,2,',');
		
		printf("shadow and group ok, done.\n");
	}

	for(grsize=0;(gr=getgrent());grsize++) {
	
		groups[grsize].gr_name	=getstr(gr->gr_name);
		groups[grsize].gr_passwd=getstr(gr->gr_passwd);
		groups[grsize].gr_gid	=gr->gr_gid;

		for(n=0;gr->gr_mem[n];n++);
		groups[grsize].gr_mem=(char **)malloc((n+1)*sizeof(char *));

		for(n=0;gr->gr_mem[n];n++)		
			groups[grsize].gr_mem[n]=getstr(gr->gr_mem[n]);

		groups[grsize].gr_mem[n]=0;
		grlist[grsize]=groups[grsize].gr_name;
	}
			
	window_create(16,16,512,384,"User and Groups");

	pwlistf(treestk);
	mscroll_create(8,8,-8,-40,&pwptr,&pwsize,pwlist,pwlistf,pweditf,0,4);

	button_create(-(8+80*4),-8,72,24,"Advanced",	0);
	button_create(-(8+80*3),-8,72,24,"Groups",	greditf);
	button_create(-(8+80*2),-8,72,24,"Users",	pweditf);
	button_create(-(8+80*1),-8,72,24,"Save",	pwsavef);
	button_create(-(8+80*0),-8,72,24,"Quit",	(void (*)(struct xtree *))exit);
}
