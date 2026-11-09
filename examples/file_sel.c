#include <xstep.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

struct xtree *wdirs,*wfiles,*waux;

int   dirlistptr, dirlistmax, filelistptr, filelistmax;
char  **dirs;
char  **files;
char  cwd[PATH_MAX];
char  filename[NAME_MAX+1];

struct dirent  **namelist;
struct stat stat_p;
struct mlist list[]={{"",NULL,256}};
struct mlist list2[]={{"",NULL,256}};

void list_f()
{
	int i, n;

	dirlistmax=0;
	filelistmax=0;

	getcwd(cwd,PATH_MAX);

	n=scandir(getcwd(cwd,PATH_MAX), &namelist, 0,alphasort);

	if(dirs)  { 
	
		for(i=0;i!=dirlistmax;i++) free(dirs[i]);	
		free(dirs);
	}
	
	if(files) {
	
		for(i=0;i!=filelistmax;i++) free(files[i]);
		free(files);
	}

	dirs =(char **)malloc(n*sizeof(char *));
	files=(char **)malloc(n*sizeof(char *));

	if(wdirs)  wdirs->z=dirs;
	else 	   list[0].p=dirs;

	if(wfiles) wfiles->z=files;
	else 	   list2[0].p=files;

	for( i=0; i < n ; i++)
	{
		stat(namelist[i]->d_name,&stat_p);
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
}

void dir_double_click()
{
      chdir(dirs[dirlistptr]);
      list_f();
}


void file_click()
{
sprintf(filename,"%s",files[filelistptr]);
broadcast++;
}

void ok_f(struct xtree *treeptr)
{
if(strcmp("/",cwd)==0)
	printf("%s%s\n",cwd,filename);
else
	printf("%s/%s\n",cwd,filename);
window_close(treeptr);
}

void xmain(int n, char *p[]){
list_f();
file_click();
window_create(0,0,500,400,"File Selection Box");

wdirs=mscroll_create(8,8,-8,-(40+21),&dirlistptr,&dirlistmax,list,0,dir_double_click,0,1);
wfiles=mscroll_create(250,8,-8,-(40+21),&filelistptr,&filelistmax,list2,file_click,0,0,1);

edit_create(8,8,-8,21,cwd,PATH_MAX,0);
edit_create(8,-40,-8,21,filename,NAME_MAX+1,0);
button_create(-(8+80*0),-8,72,24,"Cancel",window_close);
button_create(-(8+80*1),-8,72,24,"Ok",ok_f);

for(wdirs=wdirs->treestk;wdirs->z!=dirs;wdirs=wdirs->next);
for(wfiles=wfiles->treestk;wfiles->z!=files;wfiles=wfiles->next);
}