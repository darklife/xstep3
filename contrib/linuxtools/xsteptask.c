/*
	Xtask - Task Manager for X
	Copyright (C) 1998 by Marcelo Samsoniuk

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
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>

char 		*ilist[6][1000],status[256],taskboxname[256];
int		cpud[100],memd[100],lodd[100];
int		cpup=0,taskboxpid,lockmenu,lcpuuse;
int		green,red,blue;

struct mlist milist[]={

	{ "task",	ilist[4], 50  },
	{ "%CPU",	ilist[0], 50  },
	{ "%MEM",	ilist[1], 50  }, 
	{ "size",	ilist[2], 50  },
	{ "user",	ilist[3], 50  },
	{ "name",	ilist[5], 100 },
};

int 	iptr,imax;

struct {

	char type[16];
	char prot[16];
	char from[32];
	char fport[16];
	char to[32];
	char tport[16];

} rule;

void quit(struct xtree *treeptr) { exit(0); }

char *getfield(char *p,int f) {

	char *q,*r;
	int i,fi;

	while(*p<=32) p++;
	for(q=p,i=0;*p>32;p++,i++);

	for(fi=0;fi!=f;fi++) {
	
		while(*p<=32) p++;
		for(q=p,i=0;*p>32;p++,i++);
	}
	r=(char *)malloc(i+1);
	strncpy(r,q,i);
	r[i]=0;
	return(r);
}

void update(struct xtree *treeptr) {

	return;
	strcpy(rule.type,ilist[0][iptr]);
	strcpy(rule.prot,ilist[1][iptr]);
	strcpy(rule.from,ilist[2][iptr]);
	strcpy(rule.fport,ilist[4][iptr]);
	strcpy(rule.to,ilist[3][iptr]);
	strcpy(rule.tport,ilist[5][iptr]);
}

int reference;

#define MAX 1024

void graphexpose(struct xtree *treeptr) {

	int i;

	downbox(0,0,treeptr->aw,treeptr->ah,black,treeptr);
	XSetForeground(display,treeptr->gc,red);
	for(i=0;i!=cpup;i++)
		XDrawLine(display,treeptr->win,treeptr->gc,
			2+i*(treeptr->aw-4)/99,102-cpud[i]/100,
			2+(i+1)*(treeptr->aw-4)/99,102-cpud[i+1]/100);
	XSetForeground(display,treeptr->gc,green);
	for(i=0;i!=cpup;i++)
		XDrawLine(display,treeptr->win,treeptr->gc,
			2+i*(treeptr->aw-4)/99,102-memd[i]/100,
			2+(i+1)*(treeptr->aw-4)/99,102-memd[i+1]/100);
/*
	XSetForeground(display,treeptr->gc,blue);
	for(i=0;i!=cpup;i++)
		XDrawLine(display,treeptr->win,treeptr->gc,
			2+i*(treeptr->aw-4)/99,102-lodd[i]/100,
			2+(i+1)*(treeptr->aw-4)/99,102-lodd[i+1]/100);
*/
	broadcast++;
	sprintf(status,
		"CPU (red): %d.%02d%% - MEM (green): %d.%02d%%",
		cpud[cpup]/100,cpud[cpup]%100,memd[cpup]/100,memd[cpup]%100);
}

void graph(struct xtree *treeptr) {

	int i,j,n,pid,k,uptime,memory,cpuuse=0;
	FILE *f;
	char buffer[MAX],buffer2[MAX],*t;
	struct dirent **namelist;
	struct passwd *pwd;

	i=time(0)/3;
	if(reference==i) return;
	reference=i;

	for(i=0;i!=imax;i++)
		for(j=0;j!=6;j++) 
			free(ilist[j][i]);

	imax=0;

	if(cpup==99) 
		for(i=0;i!=99;i++)  {
			cpud[i]=cpud[i+1];
			memd[i]=memd[i+1];
			lodd[i]=lodd[i+1];
		}
	else
		cpup++;

	cpud[cpup]=memd[cpup]=0;

	i=open("/proc/kcore",O_RDONLY);
	memory=lseek(i,0l,SEEK_END)/1024;
	close(i);

	i=open("/proc/uptime",O_RDONLY);
	read(i,buffer,256);
	uptime=(int)(atof(buffer)*100);
	close(i);

	n=scandir("/proc", &namelist, 0, alphasort);

	for(k=0;k!=n;k++) {

		pid=atoi(namelist[k]->d_name);
		if(!pid) goto skip;

		sprintf(buffer,"/proc/%d/cmdline",pid);
                j=open(buffer,O_RDONLY);
		i=read(j,buffer,256);
		close(j);
		if(i) {

			for(j=0;j<i&&j!=256;j++) if(buffer[j]==0) buffer[j]=' ';
			buffer[j]=0;
			ilist[5][imax]=(char *)malloc(i+1);
			strcpy(ilist[5][imax],buffer);
		}
		sprintf(buffer,"/proc/%d/stat",pid);
		f=fopen(buffer,"r");
		if(!f) goto skip;
		fgets(buffer,256,f);

		if(!i) ilist[5][imax]=getfield(buffer,1);

		i=atoi(t=getfield(buffer,13)); free(t);
		i+=atoi(t=getfield(buffer,14)); free(t);
		
		cpuuse+=i;

		sprintf(buffer2,"%.03f",100*(i/(uptime-atof(t=getfield(buffer,21))))); free(t);
		ilist[0][imax]=getfield(buffer2,0);		

		sprintf(buffer2,"%.03f",100.0/(memory)*(atoi(t=getfield(buffer,23))*4.0)); free(t);
		ilist[1][imax]=getfield(buffer2,0);

		sprintf(buffer2,"%d",atoi(t=getfield(buffer,23))*4); free(t);
		ilist[2][imax]=getfield(buffer2,0);
		
		ilist[3][imax]=getfield("*",0);
		ilist[4][imax]=getfield(buffer,0);
		fclose(f);

		sprintf(buffer,"/proc/%d/status",pid);
		f=fopen(buffer,"r");
		if(!f) goto skip;
        	for(j=0;j!=7;j++) fgets(buffer,256,f);
		pwd=getpwuid(atoi(t=getfield(buffer,1)));
		free(t);
		ilist[3][imax]=getfield(pwd->pw_name,0);
		fclose(f);

		cpud[cpup]+=(int)(atof(ilist[0][imax])*100);
		memd[cpup]+=(int)(atof(ilist[1][imax])*100);

		for(i=0;i!=imax;i++) {
			
			if(atof(ilist[0][imax])>atof(ilist[0][i]))
				for(j=0;j!=6;j++) {
					
					t=ilist[j][i]; 
					ilist[j][i]=ilist[j][imax]; 
					ilist[j][imax]=t; 
				}
			if(atof(ilist[0][imax])==atof(ilist[0][i])&&
			   atof(ilist[1][imax])>atof(ilist[1][i]))
			   	for(j=0;j!=6;j++) {
			   	
					t=ilist[j][i]; 
					ilist[j][i]=ilist[j][imax]; 
					ilist[j][imax]=t;
				}
		}
		imax++;
		skip:
			free(namelist[k]);
	}
	lodd[cpup]=(cpuuse-lcpuuse)*100/5;
	free(namelist);
	graphexpose(treeptr);
	lcpuuse=cpuuse;
	
}

void sigkill(struct xtree *treeptr) { 

	kill(taskboxpid,SIGKILL); 
	window_close(treeptr);
	taskboxpid=0;
}

void sigstop(struct xtree *treeptr) { 

	strcat(taskboxname," (stoped)");
	kill(taskboxpid,SIGSTOP); 
}

void sigcont(struct xtree *treeptr) { 

	kill(taskboxpid,SIGCONT); 
	window_close(treeptr); 
	taskboxpid=0;
}

void detail(struct xtree *treeptr) {

	char *t;

	if(taskboxpid) return;

	taskboxpid=atoi(ilist[4][iptr]);
	strcpy(taskboxname,t=getfield(ilist[5][iptr],0));

	free(t);

	dialogbox_create(400,180);
	
	label_create(0,0,0,-90,taskboxname,gray,up);
	XSetFont(display,treestk->treestk->gc,(treestk->treestk->font=helvetica24b)->fid);
	label_create(0,-90,0,0,"",gray,up);
	button_create(-(8+80*0),-8,72,24,"Kill",	sigkill);
	button_create(-(8+80*1),-8,72,24,"Stop",	sigstop);
	button_create(-(8+80*2),-8,72,24,"Cancel",	sigcont);
}

void unlockmenu(struct xtree *treeptr) {

	switch(treeptr->l) {
		case 1:
		case 2:
			detail(treeptr);
			break;
		case 3:
			quit(treeptr);
		default:
	}
	lockmenu=0;
}

void menu(struct xtree *treeptr) {

	static char *z[4]={
		"Continue",
		"Stop",
		"Kill",
		"Quit"
	};
	
	if(lockmenu) return;
	lockmenu=1;
	
	menu_create(0,0,120,4*21,z,0,unlockmenu);
}

void xmain(int n,char *p[]) {

	red=getcolor(0xffff,0,0); 
	green=getcolor(0,0xffff,0);
	blue=getcolor(0,0,0xffff);

	animate=1;

	window_create(0,0,512,384,"XstepTask");
	label_create(8,8,-8,21,status,darkgray,center);
	box_create(8,29,-8,104,graph,gray);
	treestk->treestk->expose=graphexpose;
	mscroll_create(8,104+29+8,-8,-(24+16),&iptr,&imax,milist,update,detail,menu,6);
	button_create(-88,-8,72,24,"Detail",detail);
	button_create(-8,-8,72,24,"Quit",quit);
}
