/* This was written by Cassio (napalm@zaz.com.br),and hacked by Tito with hints by Marcelo Samsoniuk*/
/* Licensed under GPL */

#include <xstep.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <libgen.h>
#include "Config.h"


/*for modeline()*/
#define TYPEINDEX(mode) (((mode) >> 12) & 0x0f)
#define TYPECHAR(mode)  ("0pcCd?bB-?l?s???" [TYPEINDEX(mode)])

/* to use the internal viewer applet */
#ifdef INTERNAL_VIEWER
	#define MAXLINES 1000000
	int	stackmax,
		stackptr;
	char text_buffer[1024],
		*stack[MAXLINES];
#endif

int			dirlistptr,
			dirlistmax,
			filelistptr,
			filelistmax,
			lock=0;

long int	totalsize=0;

char		directory[256],
			info[256],
			program[64],
			dest[256]="",
			*command,
			*command2,
			buffer[258],
			name[256],
			**dirs,
			**files,
			cwd[PATH_MAX];


struct xtree *wdirs,*wfiles,*waux;
/* I have elimated the labels because they sometimes
don't get redrawn correctly and I've made the lists dynamic
with the help of Marcelo Samsoniuk  */
struct mlist list[]={{"",NULL,256}};
struct mlist list2[]={{"",NULL,256}};
struct stat stat_p;
struct dirent  **namelist;
struct plist
{
char owner[9],group[9];
char ur[2],uw[2],ux[2];
char gr[2],gw[2],gx[2];
char or[2],ow[2],ox[2];
} permissions;

/* Code below for the modeline() function is from libbb mode_string.c
of the busybox project, more about it at www.busybox.net*/

/* The special bits. If set, display SMODE0/1 instead of MODE0/1 */
static const mode_t SBIT[] = {
	0, 0, S_ISUID,
	0, 0, S_ISGID,
	0, 0, S_ISVTX
};

/* The 9 mode bits to test */
static const mode_t MBIT[] = {
	S_IRUSR, S_IWUSR, S_IXUSR,
	S_IRGRP, S_IWGRP, S_IXGRP,
	S_IROTH, S_IWOTH, S_IXOTH
};

static const char MODE1[]  = "rwxrwxrwx";
static const char MODE0[]  = "---------";
static const char SMODE1[] = "..s..s..t";
static const char SMODE0[] = "..S..S..T";
/* end of busybox code*/

void list_f()
{
	int i, n;

	dirlistmax=0;
	filelistmax=0;

	getcwd(cwd,PATH_MAX);

	n=scandir(getcwd(cwd,PATH_MAX), &namelist, 0,alphasort);

	if(dirs)
	{
		for(i=0;i!=dirlistmax;i++) free(dirs[i]);
		free(dirs);
	}

	if(files)
	{
		for(i=0;i!=filelistmax;i++) free(files[i]);
		free(files);
	}

	dirs =(char **)malloc(n*sizeof(char *));
	files=(char **)malloc(n*sizeof(char *));

	if(wdirs)
		wdirs->z=dirs;
	else
		list[0].p=dirs;

	if(wfiles)
		wfiles->z=files;
	else
		list2[0].p=files;

	for( i=0; i < n ; i++)
	{
		stat(namelist[i]->d_name,&stat_p);
		/* TODO if we stat a broken softlink here it gets not displayed,
		need a workaround for this */
		if (S_ISDIR(stat_p.st_mode))
		{
			dirs[dirlistmax]=(char *)malloc(strlen(namelist[i]->d_name)+strlen("/")+2);
			strcpy(dirs[dirlistmax],namelist[i]->d_name);
			strcat(dirs[dirlistmax],"/");
			dirlistmax++;
		}
		else
		{
			files[filelistmax]=(char *)malloc(strlen(namelist[i]->d_name)+2);
			strcpy(files[filelistmax],namelist[i]->d_name);
			filelistmax++;
		}
		free(namelist[i]);
	}
	free(namelist);
	filelistptr=0;
	dirlistptr=0;
	sprintf(directory,"%s %s","Path:",cwd);
}

char *format_time(time_t cal_time)
{
	struct tm *time_struct;
	static char string[30];
	time_struct=localtime(&cal_time);
	/*strftime(string,sizeof string,"%h %e %H:%M",time_struct);
	changed this as gcc with -pedantic reports:
	warning: ISO C89 does not support the `%h' strftime format
	warning: ISO C89 does not support the `%e' strftime format*/
	strftime(string,sizeof string,"%b %d %H:%M",time_struct);
	return(string);
}

char *currentdir(void)
{
	int size=100;
	char *buffer=(char *)malloc(size);
	while (1)
	{
		char *value=getcwd(buffer,size);
		if (value != 0) return buffer;
		size*=2;
		free(buffer);
		buffer=(char *)malloc(size);
	}
}

int compare(const void *a,const void *b)
{
	return strcmp(*(char**)a,*(char**)b);
}

long int filesize(void)
{
	return(stat_p.st_size);
}

char *owner(void)
{
	struct passwd *pws;
	
	if ((pws=getpwuid(stat_p.st_uid)) != NULL)
		return(pws->pw_name);
	else
	{
		char *uid=(char *)malloc(10);
		sprintf(uid,"%d",stat_p.st_uid);
		return(uid);
	}
}

char *group(void)
{
	struct group *gps;
	
	if ((gps=getgrgid(stat_p.st_gid)) != NULL)
		return(gps->gr_name);
	else
	{
		char *gid=(char *)malloc(10);
		sprintf(gid,"%d",stat_p.st_gid);
		return(gid);
	}
}

const char *modeline(int mode)
{
	/* This is taken from busybox libbb mode_string.c,
	more about it at www.busybox.net */
	static char buf[12];

	int h;

	buf[0] = TYPECHAR(mode);
	for (h = 0; h < 9; h++)
	{
		if (mode & SBIT[h])
			buf[h + 1] = (mode & MBIT[h]) ? SMODE1[h] : SMODE0[h];
		else
			buf[h + 1] = (mode & MBIT[h]) ? MODE1[h] : MODE0[h];
	}
	return buf;
}
/* end of busybox code */

char *timestr(void)
{
	return format_time(stat_p.st_mtime);
}


void finf()
{
	if (filelistmax > 0)
	{
		lstat(files[filelistptr],&stat_p);
		sprintf(info,"%s     %s     %s     %ld     %s",
			modeline(stat_p.st_mode),owner(),group(),filesize(),timestr());
		broadcast++;
	}
}


void dinf()
{
	lstat(dirs[dirlistptr],&stat_p);
	sprintf(info,"%s     %s     %s     %ld     %s",
		modeline(stat_p.st_mode),owner(),group(),filesize(),timestr());
	broadcast++;
}

void dinfo()
{
	sprintf(info,"%d files     %d directories     %ld bytes",
		filelistmax,dirlistmax-1,totalsize);
	broadcast++;
}

void update()
{
	list_f();
	dinfo();
	broadcast++;
}

void cd(struct xtree *treeptr)
{
	if (lock == 0)
	{
		chdir(dirs[dirlistptr]);
		update();
	}
}


void runprog(struct xtree *treeptr)
{
	char *cmdline;
	cmdline=(char *)malloc(strlen(program)+strlen(files[filelistptr])+6);
	sprintf(cmdline,"%s \"./%s\" &",program,files[filelistptr]);
	#ifdef DEBUG
	printf("cmd %s\n",cmdline);/* debug */
	#endif
	system(cmdline);
	free(cmdline);
	sprintf(program," ");
	window_close(treeptr);
	lock=0;
}

void runcommand(struct xtree *treeptr)
{
	if (strlen(dest) > 0)
	{
		strcat(command,"\"");
		strcat(command,dest);
		strcat(command,"\"");
	}
	#ifdef DEBUG
	printf("%s\n",command);/*debug*/
	#endif
	system(command);
	free(command);
	window_close(treeptr);
	strcpy(dest,"");
	update();
	broadcast++;
	lock=0;
}

void changeperm(struct xtree *treeptr)
{
	char *cmdline;
	cmdline=(char *)malloc(strlen(command)+18);
	sprintf(cmdline,"chmod u=%s%s%s \"%s\"",
		permissions.ur,permissions.uw,permissions.ux,command);
	system(cmdline);
	sprintf(cmdline,"chmod g=%s%s%s \"%s\"",
		permissions.gr,permissions.gw,permissions.gx,command);
	system(cmdline);
	sprintf(cmdline,"chmod o=%s%s%s \"%s\"",
		permissions.or,permissions.ow,permissions.ox,command);
	system(cmdline);
	sprintf(cmdline,"chown %s \"%s\"",permissions.owner,command);
	system(cmdline);
	sprintf(cmdline,"chgrp %s \"%s\"",permissions.group,command);
	system(cmdline);
	free(cmdline);
	free(command);
	window_close(treeptr);
	lock=0;
}

void cancel(struct xtree *treeptr)
{
	free(command);
	window_close(treeptr);
	strcpy(dest,"");
	lock=0;
}


void do_nothing(struct xtree *treeptr)
{
	window_close(treeptr);
	lock=0;
}

void error_f()
{
	if(lock==0)
	{
		lock=1;
		dialogbox_create(400,180);
		label_create(0,0,0,-90,"Error",gray,up);
		XSetFont(display,treestk->treestk->gc,
			(treestk->treestk->font=helvetica24b)->fid);
		label_create(0,-90,0,0,"",gray,up);
		defaultfont=helvetica12m;
		button_create(-8,-8,72,24,"Cancel",do_nothing);
	}
}

void open_w()
{
	#ifdef DEBUG
	printf("lock %d\n",lock); /*debug*/
	#endif
	if ((filelistmax > 0) && (lock==0))
	{
		lock=1;
		dialogbox_create(400,180);
		label_create(0,0,0,-90,"Open with ...",gray,up);
		XSetFont(display,treestk->treestk->gc,
			(treestk->treestk->font=helvetica24b)->fid);
		label_create(0,-90,0,0,"",gray,up);
		button_create(-88,-8,72,24,"Ok",runprog);
		button_create(-8,-8,72,24,"Cancel",do_nothing);
		edit_create(8,-48,382,21,program,64,0);
	}
	else
		error_f();
}

void edit()
{
	if (filelistmax > 0)
	{
		char *cmdline;

		cmdline=(char *)malloc(strlen(EDITOR)+strlen(files[filelistptr])+6);
		sprintf(cmdline,"%s \"%s\" &",EDITOR,files[filelistptr]);
		system(cmdline);
		free(cmdline);
	}
	else
		error_f();
}
#ifdef INTERNAL_VIEWER
void scrollf(struct xtree *t)
{
	strcpy(text_buffer,stack[stackptr]);
	broadcast++;
}

void view_file(char *filename)
{
	FILE *file;
	int n;

	if(!(file=fopen(filename,"r")))
	{
		printf("Cannot open file: %s\n",filename);
		/*exit(EXIT_FAILURE);*/
	}
	for( ;fgets(text_buffer,1024,file) && stackmax!=MAXLINES;stackmax++)
	{
		for(n=0;text_buffer[n];n++)
		{
			if(text_buffer[n]=='\n')
				text_buffer[n]=0;
			if(text_buffer[n]=='\t')
			{
				/* reduce tabs to two spaces */
				memmove(text_buffer+n+2,text_buffer+n+1,strlen(text_buffer+n+1));
				strncpy(text_buffer+n,"  ",2);
			}
		}
	stack[stackmax]=(char *)malloc(1+strlen(text_buffer));
	strcpy(stack[stackmax],text_buffer);
	}
	fclose(file);
}
#endif
void view()
{
	if (filelistmax > 0)
	{


#ifdef INTERNAL_VIEWER
		lock=1;
		window_create(0,0,512,512,"Viewer");
		view_file(files[filelistptr]);
		scroll_create(0,0,-1,-41,&stackptr,&stackmax,stack,scrollf);
		button_create(-(8+80*0),-8,72,24,"Ok", do_nothing);
#else
		char *cmdline;
		
		cmdline=(char *)malloc(strlen(VIEWER)+strlen(files[filelistptr])+6);
		sprintf(cmdline,"%s \"%s\" &",VIEWER,files[filelistptr]);
		system(cmdline);
		free(cmdline);
#endif
	}
	else
		error_f();
}


void copy()
{
	#ifdef DEBUG
	printf("lock %d\n",lock); /*debug*/
	#endif
	if ((filelistmax > 0) && (lock==0))
	{
		lock=1;
		command=(char *)malloc(strlen(files[filelistptr])+305);
		sprintf(command,"cp \"%s\" ",files[filelistptr]);
		dialogbox_create(400,180);
		label_create(0,0,0,-90,"Copy to ...",gray,up);
		XSetFont(display,treestk->treestk->gc,
			(treestk->treestk->font=helvetica24b)->fid);
		label_create(0,-110,0,0,"",gray,up);
		button_create(-88,-8,72,24,"Ok",runcommand);
		button_create(-8,-8,72,24,"Cancel",cancel);
		label_create(8,-68,382,21,files[filelistptr],gray,left);
		XSetFont(display,treestk->treestk->gc,
			(treestk->treestk->font=helvetica12m)->fid);
		edit_create(8,-38,382,21,dest,256,0);
	}
	else
		error_f();
}


void move()
{
	#ifdef DEBUG
	printf("lock %d\n",lock); /*debug*/
	#endif
	if ((filelistmax > 0) && (lock==0))
	{
		lock=1;
		command=(char *)malloc(strlen(files[filelistptr])+305);
		sprintf(command,"mv \"%s\" ",files[filelistptr]);
		dialogbox_create(400,180);
		label_create(0,0,0,-90,"Move to ...",gray,up);
		XSetFont(display,treestk->treestk->gc,
			(treestk->treestk->font=helvetica24b)->fid);
		label_create(0,-110,0,0,"",gray,up);
		button_create(-88,-8,72,24,"Ok",runcommand);
		button_create(-8,-8,72,24,"Cancel",cancel);
		label_create(8,-68,382,21,files[filelistptr],gray,left);
		XSetFont(display,treestk->treestk->gc,
			(treestk->treestk->font=helvetica12m)->fid);
		edit_create(8,-38,382,21,dest,256,0);
	}
	else
		error_f();
}


void del()
{
	#ifdef DEBUG
	printf("lock %d\n",lock); /*debug*/
	#endif
	if ((filelistmax > 0) && (lock==0))
	{
		lock=1;
		command=(char *)malloc(strlen(files[filelistptr])+6);
		sprintf(command,"rm \"%s\"",files[filelistptr]);
		dialogbox_create(400,180);
		label_create(0,0,0,-90,"Delete file?",gray,up);
		XSetFont(display,treestk->treestk->gc,
			(treestk->treestk->font=helvetica24b)->fid);
		label_create(0,-90,0,0,"",gray,up);
		button_create(-88,-8,72,24,"Ok",runcommand);
		button_create(-8,-8,72,24,"Cancel",cancel);
		label_create(8,-48,382,21,files[filelistptr],white,left);
		XSetFont(display,treestk->treestk->gc,
			(treestk->treestk->font=helvetica12m)->fid);
	}
	else
		error_f();
}


void lnk()
{
	#ifdef DEBUG
	printf("lock %d\n",lock); /*debug*/
	#endif
	if ((filelistmax > 0) && (lock==0))
	{
		lock=1;
		command=(char *)malloc(strlen(currentdir())+strlen(files[filelistptr])+308);
		if(strcmp(currentdir(),"/")==0)
			sprintf(command,"ln -s \"%s%s\" ",currentdir(),files[filelistptr]);
		else
			sprintf(command,"ln -s \"%s/%s\" ",currentdir(),files[filelistptr]);
		dialogbox_create(400,180);
		label_create(0,0,0,-90,"Link to ...(symbolic)",gray,up);
		XSetFont(display,treestk->treestk->gc,
			(treestk->treestk->font=helvetica24b)->fid);
		label_create(0,-110,0,0,"",gray,up);
		button_create(-88,-8,72,24,"Ok",runcommand);
		button_create(-8,-8,72,24,"Cancel",cancel);
		label_create(8,-68,382,21,files[filelistptr],gray,left);
		XSetFont(display,treestk->treestk->gc,
			(treestk->treestk->font=helvetica12m)->fid);
		edit_create(8,-38,382,21,dest,256,0);
	}
	else
		error_f();
}

void perm_common()
{
	char l[11];
	
	/* TODO  add handling of S_ISUID,S_ISGID,S_ISVTX
	as they are now displayed in modeline()*/
	
	strcpy(l,modeline(stat_p.st_mode));
	strcpy(permissions.owner,owner());
	strcpy(permissions.group,group());
	sprintf(permissions.ur,"%c",l[1]);
	sprintf(permissions.uw,"%c",l[2]);
	sprintf(permissions.ux,"%c",l[3]);
	sprintf(permissions.gr,"%c",l[4]);
	sprintf(permissions.gw,"%c",l[5]);
	sprintf(permissions.gx,"%c",l[6]);
	sprintf(permissions.or,"%c",l[7]);
	sprintf(permissions.ow,"%c",l[8]);
	sprintf(permissions.ox,"%c",l[9]);
	dialogbox_create(400,190);
	label_create(0,0,0,-100,"Permissions",gray,up);
	XSetFont(display,treestk->treestk->gc,
		(treestk->treestk->font=helvetica24b)->fid);
	label_create(0,-100,0,0,"",gray,up);
	button_create(-88,-8,72,24,"Ok",changeperm);
	button_create(-8,-8,72,24,"Cancel",cancel);
	label_create(8,-68,60,21,"User",darkgray,center);
	label_create(8,-40,60,21,"Group",darkgray,center);
	label_create(8,-11,60,21,"Others",darkgray,center);
	label_create(-120,-68,60,21,"Owner",darkgray,center);
	label_create(-120,-40,60,21,"Group",darkgray,center);
	check_create(80,-68,40,21,"r",permissions.ur);
	check_create(120,-68,40,21,"w",permissions.uw);
	check_create(160,-68,40,21,"x",permissions.ux);
	check_create(80,-40,40,21,"r",permissions.gr);
	check_create(120,-40,40,21,"w",permissions.gw);
	check_create(160,-40,40,21,"x",permissions.gx);
	check_create(80,-11,40,21,"r",permissions.or);
	check_create(120,-11,40,21,"w",permissions.ow);
	check_create(160,-11,40,21,"x",permissions.ox);
	edit_create(-8,-68,110,21,permissions.owner,9,0);
	edit_create(-8,-40,110,21,permissions.group,9,0);
}

void perm()
{
	#ifdef DEBUG
	printf("lock %d\n",lock); /*debug*/
	#endif
	if ((filelistmax > 0) && (lock==0))
	{
		lock=1;
		lstat(files[filelistptr],&stat_p);
		command=(char *)malloc(strlen(files[filelistptr])+1);
		strcpy(command,files[filelistptr]);
		perm_common();
	}
	else
		error_f();
}


void createdir()
{
	#ifdef DEBUG
	printf("lock %d\n",lock); /*debug*/
	#endif
	if(lock==0)
	{
		lock=1;
		command=(char *)malloc(8 + PATH_MAX);
		strcpy(command,"mkdir ");
		dialogbox_create(400,180);
		label_create(0,0,0,-90,"Create dir ...",gray,up);
		XSetFont(display,treestk->treestk->gc,
			(treestk->treestk->font=helvetica24b)->fid);
		label_create(0,-90,0,0,"",gray,up);
		button_create(-88,-8,72,24,"Ok",runcommand);
		button_create(-8,-8,72,24,"Cancel",cancel);
		edit_create(8,-48,382,21,dest,256,0);
	}
}


void copydir()
{
	#ifdef DEBUG
	printf("lock %d\n",lock); /*debug*/
	#endif
	if ((dirlistptr > 0)&&(lock==0))
	{
		lock=1;
		command=(char *)malloc(strlen(dirs[dirlistptr])+PATH_MAX+9);
		sprintf(command,"cp -dpR \"%s\" ",dirs[dirlistptr]);
		dialogbox_create(400,180);
		label_create(0,0,0,-90,"Copy Dir to ...(recursive) [-dpR]",gray,up);
		XSetFont(display,treestk->treestk->gc,
			(treestk->treestk->font=helvetica24b)->fid);
		label_create(0,-110,0,0,"",gray,up);
		button_create(-88,-8,72,24,"Ok",runcommand);
		button_create(-8,-8,72,24,"Cancel",cancel);
		label_create(8,-68,382,21,dirs[dirlistptr],gray,left);
		XSetFont(display,treestk->treestk->gc,
			(treestk->treestk->font=helvetica12m)->fid);
		edit_create(8,-38,382,21,dest,256,0);
	}
	else
		error_f();
}

void movedir()
{
	#ifdef DEBUG
	printf("lock %d\n",lock); /*debug*/
	#endif
	if ((dirlistptr > 0)&&(lock==0))
	{
		lock=1;
		command=(char *)malloc(strlen(dirs[dirlistptr])+PATH_MAX+6);
		sprintf(command,"mv \"%s\" ",dirs[dirlistptr]);
		dialogbox_create(400,180);
		label_create(0,0,0,-90,"Move Dir to ...",gray,up);
		XSetFont(display,treestk->treestk->gc,
			(treestk->treestk->font=helvetica24b)->fid);
		label_create(0,-110,0,0,"",gray,up);
		button_create(-88,-8,72,24,"Ok",runcommand);
		button_create(-8,-8,72,24,"Cancel",cancel);
		label_create(8,-68,382,21,dirs[dirlistptr],gray,left);
		XSetFont(display,treestk->treestk->gc,
			(treestk->treestk->font=helvetica12m)->fid);
		edit_create(8,-38,382,21,dest,256,0);
	}
	else
		error_f();
}


void deldir()
{
	#ifdef DEBUG
	printf("lock %d\n",lock);					/*debug*/
	printf("%s\n",dirs[dirlistptr]);			/*debug*/
	#endif
	if ((dirlistptr > 0)&&(lock==0))
	{
		lock=1;
		command=(char *)malloc(strlen(dirs[dirlistptr])+9);
		/* we do this  to strip last / from filename as this confuses lstat */
		snprintf(command,strlen(dirs[dirlistptr]),"%s",dirs[dirlistptr]);
		lstat(command,&stat_p);
		#ifdef DEBUG
		printf("now file name is: %s and ",command);	/*debug*/
		#endif
		/* it is not necessary to free and malloc or to calloc *command
		as the value contained in it in the next steps get fully overwritten */
		if(S_ISLNK(stat_p.st_mode))
		{
			/* to handle a link to dir conveniently */
			sprintf(command,"rm ");
			/* strlen(dirs[dirlistptr])-1 is to strip last / from filename*/
			strncat(command,dirs[dirlistptr],strlen(dirs[dirlistptr])-1);
			#ifdef DEBUG
			printf("S_ISLNK is true\n");	/*debug*/
			#endif
		}
		else
		{
			sprintf(command,"rm -r \"%s\"",dirs[dirlistptr]);
			#ifdef DEBUG
			printf("S_ISDIR is true\n");	/*debug*/
			#endif
		}
		dialogbox_create(400,180);
		label_create(0,0,0,-90,"Delete Dir?",gray,up);
		XSetFont(display,treestk->treestk->gc,
			(treestk->treestk->font=helvetica24b)->fid);
		label_create(0,-90,0,0,"",gray,up);
		button_create(-88,-8,72,24,"Ok",runcommand);
		button_create(-8,-8,72,24,"Cancel",cancel);
		label_create(8,-48,382,21,dirs[dirlistptr],white,left);
		XSetFont(display,treestk->treestk->gc,
			(treestk->treestk->font=helvetica12m)->fid);
	}
	else
		error_f();
}


void lnkdir()
{
	#ifdef DEBUG
	printf("lock %d\n",lock); /*debug*/
	#endif
	if ((dirlistptr > 0)&&(lock==0))
	{
		lock=1;
		command=(char *)malloc(strlen(currentdir())+strlen(dirs[dirlistptr])+308);
		if(strcmp(currentdir(),"/")==0)
			sprintf(command,"ln -s \"%s%s\" ",currentdir(),dirs[dirlistptr]);
		else
			sprintf(command,"ln -s \"%s/%s\" ",currentdir(),dirs[dirlistptr]);
		dialogbox_create(400,180);
		label_create(0,0,0,-90,"Link Dir to ...(symbolic)",gray,up);
		XSetFont(display,treestk->treestk->gc,
			(treestk->treestk->font=helvetica24b)->fid);
		label_create(0,-110,0,0,"",gray,up);
		button_create(-88,-8,72,24,"Ok",runcommand);
		button_create(-8,-8,72,24,"Cancel",cancel);
		label_create(8,-68,382,21,dirs[dirlistptr],gray,left);
		XSetFont(display,treestk->treestk->gc,
			(treestk->treestk->font=helvetica12m)->fid);
		edit_create(8,-38,382,21,dest,256,0);
	}
	else
		error_f();
}


void permdir()
{
	#ifdef DEBUG
	printf("lock %d\n",lock); /*debug*/
	#endif
	if ((dirlistptr > 0)&&(lock==0))
	{
		lock=1;
		lstat(dirs[dirlistptr],&stat_p);
		command=(char *)malloc(strlen(dirs[dirlistptr])+1);
		strcpy(command,dirs[dirlistptr]);
		perm_common();
	}
	else
		error_f();
}

void dclick(struct xtree *treeptr)
{
	#ifdef DEBUG
	printf("lock %d\n",lock); /*debug*/
	#endif
	if ((filelistmax > 0) && (lock == 0))
	{
		lstat(files[filelistptr],&stat_p);
		if (modeline(stat_p.st_mode)[3] == 'x')
		{
			char *cmdline;
			cmdline=(char *)malloc(strlen(files[filelistptr])+14);/*12*/
			sprintf(cmdline,"%s \"./%s\" &",TERM_EXE,files[filelistptr]);
			if(!fork())
			{
				system(cmdline);
				exit(0);
			}
			free(cmdline);
		}
		else
			open_w();
	}
}

void open_shell()
{
	if(!fork())
	{
		system(TERMINAL);
		exit(0);
	}
}

void systemf(struct xtree *t)
{
	if(!fork())
	{
		system(buffer);
		exit(0);
	}
	buffer[0]=0;
	broadcast++;
}

void run()
{
	#ifdef DEBUG
	printf("lock %d\n",lock); /*debug*/
	#endif
	window_create(0,0,400,68,"Xrun");
	edit_create(5,5,130*3,21,buffer,256,systemf);
	button_create(-(5+80*0),-8,72,24,"Cancel",window_close);
	button_create(-(8+80*1),-8,72,24,"Run",systemf);
}

#ifdef RIGHT_CLICK
void menuff(struct xtree *treeptr)
{
	switch (treeptr->l)
	{
		case 0: finf();   break;
		case 1: update(); break;
		case 2: open_w(); break;
		case 3: edit();   break;
		case 4: copy();   break;
		case 5: move();   break;
		case 6: del();    break;
		case 7: lnk();    break;
		case 8: perm();   break;
		case 9: exit(0);  break;
	}
}

void menudf(struct xtree *treeptr)
{
	switch (treeptr->l)
	{
		case 0: dinf();      break;
		case 1: update();    break;
		case 2: createdir(); break;
		case 3: copydir();   break;
		case 4: movedir();   break;
		case 5: deldir();    break;
		case 6: lnkdir();    break;
		case 7: permdir();   break;
		case 8: exit(0);     break;
	}
}

void menuf(struct xtree *treeptr)
{
	char static *z[10]=
	{
		"File info","Update","Open with...",
		"Edit","Copy","Move","Delete",
		"Link","Permissions","Quit"
	};
	
	if (lock == 0)
		menu_create(0,0,120,8*21,z,0,menuff);
}

void menud(struct xtree *treeptr)
{
	char static *z[9]=
	{
		"Dir info","Update","Create dir",
		"Copy","Move","Delete","Link",
		"Permissions","Quit"
	};
	
	if (lock == 0)
		menu_create(0,0,120,9*21,z,0,menudf);
}
#endif

/* main ********************************************************* main */

void xmain(int n,char *p[])
{
	#ifdef DEBUG
	printf("argc %d\n",argc);					/*debug*/
	printf("argv0 %s\n",argv[0]);				/*debug*/
	printf("argv1 %s\n",argv[1]);				/*debug*/
	printf("basename %s\n",basename(argv[0]));	/*debug*/
	printf("lock %d\n",lock); 					/*debug*/
	#endif

	/* This to start only the xrun applet */
	if(strcmp(basename(argv[0]),"xrun")==0)
		run();
#ifdef INTERNAL_VIEWER
	else if(strcmp(basename(argv[0]),"xviewer")==0 && argc == 2)
	{
		window_create(0,0,512,512,"xviewer");
		view_file(argv[1]);
		scroll_create(0,0,-1,-41,&stackptr,&stackmax,stack,scrollf);
		button_create(-(8+80*0),-8,72,24,"Ok", do_nothing);
	}
#endif
	else
	{
		animate=1;
		update();
		window_create(0,0,728,500,LABEL);
		/* path box */
		label_create(1,1,-1,21,directory,white,left);

		/* Dirs and Files scroll-boxes */
		#ifdef RIGHT_CLICK
		wdirs=mscroll_create(1,23,312,-100,&dirlistptr,&dirlistmax,list,dinf,cd,menud,1);
		treestk->treestk->expose=update;
		wfiles=mscroll_create(313,23,-1,-100,&filelistptr,&filelistmax,list2,finf,dclick,menuf,1);
		treestk->treestk->expose=update;
		#else
		wdirs=mscroll_create(1,23,312,-100,&dirlistptr,&dirlistmax,list,dinf,cd,0,1);
		treestk->treestk->expose=update;
		wfiles=mscroll_create(313,23,-1,-100,&filelistptr,&filelistmax,list2,finf,dclick,0,1);
		treestk->treestk->expose=update;
		#endif

		/* These are needed because sometimes the labels are not redrawn        */
		/* correctly if put in struct dlist and struct flist of mscroll_create. */
		/* For this to work they have to be placed after mscroll_create         */
		label_create(1,23,312,21,"DIRECTORIES",darkgray,center);
		label_create(313,23,-1,21,"FILES",darkgray,center);

		/* Status bar */
		label_create(1,-78,-1,21,info,white,center);

		/* Buttons row 1  *******************************************  row 1    */

		defaultfont=helvetica12b;                  /* Bold font for the buttons */

		/* We don't need a QUIT button because the WindowManager already provide
		this function */

		button_create(-(8+80*0),-38,72,24,"Refresh",update);		/* files  1  */
		button_create(-(8+80*1),-38,72,24,"Open with",open_w); 		/* files  2  */
		button_create(-(8+80*2),-38,72,24,"View",view);				/* files  3  */
		button_create(-(8+80*3),-38,72,24,"Edit",edit);	 			/* files  4  */
		button_create(-(8+80*4),-38,72,24,"File Perm",perm);		/* files  5  */
		button_create((8+80*0),-38,72,24,"Dir Perm",permdir);		/*  dir  1   */
		button_create((8+80*1),-38,72,24,"Shell here",open_shell);	/*  dir  2   */
		button_create((8+80*2),-38,72,24,"Create Dir",createdir);	/*  dir  3   */
		button_create((8+80*3),-38,72,24,"CWD info",dinfo);			/*  dir  4   */

		/* Buttons row 2  *******************************************  row 2     */

		button_create(-(8+80*0),-8,72,24,"Run",run);				/* files  6  */
		button_create(-(8+80*1),-8,72,24,"Delete",del);				/* files  7  */
		button_create(-(8+80*2),-8,72,24,"Link",lnk);				/* files  8  */
		button_create(-(8+80*3),-8,72,24,"Move",move);				/* files  9  */
		button_create(-(8+80*4),-8,72,24,"Copy",copy);				/* files 10  */
		button_create((8+80*0),-8,72,24,"Copy Dir",copydir);		/*  dir 5    */
		button_create((8+80*1),-8,72,24,"Move Dir",movedir);		/*  dir 6    */
		button_create((8+80*2),-8,72,24,"Link Dir",lnkdir);			/*  dir 7    */
		button_create((8+80*3),-8,72,24,"Del Dir",deldir);			/*  dir 8    */
		defaultfont=helvetica12m;          /* Resetting the font to the standard */

		/* Some magic stuff from Marcelo Samsoniuk to make dirlist and filelist dynamic
		........I'll not even try to understand this ;-) */
		for(wdirs=wdirs->treestk;wdirs->z!=dirs;wdirs=wdirs->next);
		for(wfiles=wfiles->treestk;wfiles->z!=files;wfiles=wfiles->next);
	}
}

