/*

XFPROT
Copyright (C) 2002-2003 by Tito Ragusa

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
#include <limits.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include "config.h"

int 	lock=0,
    	reset=0,
    	show=0,
    	y=260,
    	z=430,
    	list_item_ptr,
    	listmax,
    	service_listmax,
		stackmax,
		stackptr;

char 	label[64]="Options",
     	path[PATH_MAX+1],
     	flag_scanmode[64]="Report only",
     	flag_scanmode_bak[64],
     	flag_disinf[64]="",
     	flag_rename[64]="",
     	flag_delete[64]="",
     	flag_file_types[64]="Standard file extensions",
     	flag_archives[64]="",
     	flag_comp_exe[64]="",
     	flag_nosub[64]="",
     	flag_collect[64]="",
     	flag_list[64]="",
     	flag_ai[64]="",
     	flag_old[64]="",
     	flag_follow[64]="",
     	flag_append[64]="",
      	flag_heur[64]="Use heuristics",
     	flag_macro[64]="Scan for macro virus",
     	flag_macro_action[64]="Remove infected macros from docs",
		report_file[PATH_MAX+1],
		**list_item,
		**service_list_item,
		*command,
		text_buffer[SIZE],
		*stack[MAXLINES];

struct	xtree *wlist_item,*waux;
struct	dirent  **namelist;
struct	stat stat_p;
struct	mlist list[]={{"",NULL,NAME_MAX+1}};
struct	xtree *win;


/*################################### This stuff is for the file selection widget ##################################*/

void list_f()
{
	int i, n;

	listmax=service_listmax=0;

	getcwd(path,PATH_MAX);

	n=scandir(getcwd(path,PATH_MAX), &namelist, 0,alphasort);

	if(list_item)
	{
		for(i=0;i!=listmax;i++)
			free(list_item[i]);
		free(list_item);
	}

	if(service_list_item)
	{
		for(i=0;i!=service_listmax;i++)
			free(service_list_item[i]);
		free(service_list_item);
	}

	list_item =(char **)malloc(n*sizeof(char *));
	service_list_item=(char **)malloc(n*sizeof(char *));

	if(wlist_item)
		wlist_item->z=list_item;
        else
		list[0].p=list_item;

	for( i=0; i < n ; i++)
	{
		stat(namelist[i]->d_name,&stat_p);
		if (S_ISDIR(stat_p.st_mode))
		{
			list_item[listmax]=(char *)malloc(strlen(namelist[i]->d_name)+strlen("/")+2);
			strcpy(list_item[listmax],namelist[i]->d_name);
			strcat(list_item[listmax],"/");
			listmax++;
		}
		else
		{
			service_list_item[service_listmax]=(char *)malloc(strlen(namelist[i]->d_name)+2);
			strcpy(service_list_item[service_listmax],namelist[i]->d_name);
			service_listmax++;
		}
		free(namelist[i]);
	}
	free(namelist);

	for(i=0; i< service_listmax; i++)
	{
		list_item[listmax]=(char *)malloc(strlen(service_list_item[i])+2);
		strcpy(list_item[listmax],service_list_item[i]);
		listmax++;
	}
	list_item_ptr=0;
}

void double_click()
{
      chdir(list_item[list_item_ptr]);
      list_f();
}


void single_click()
{
	if(list_item_ptr > 1)
	{
		getcwd(path,PATH_MAX);
		if(strcmp("/",path)==0)
			sprintf(path,"%s%s",path,list_item[list_item_ptr]);
		else
			sprintf(path,"%s/%s",path,list_item[list_item_ptr]);
		broadcast++;
	}
}

void main_window_close_f(struct xtree *treeptr)
{
	/* Closes the main window if no file selection or other locking child window is open */
	if(lock==0)
		window_close(treeptr);
}

void child_window_close_f(struct xtree *treeptr)
{
	/* Closes the file selection or other child window and resets the lock */
	lock=0;
	window_close(treeptr);
	treestk->message=main_window_close_f;
}

void file_select()
{
/* Runs the file selection widget if there is not one already running */

	if(lock==0)
	{
		lock=1;

		sprintf(flag_scanmode_bak,"%s",flag_scanmode);

		list_f();
		single_click();

		window_create(0,0,450,350,"Select path to scan:");

		treestk->message=child_window_close_f;

		wlist_item=mscroll_create(8,8,-8,-40,&list_item_ptr,&listmax,list,single_click,double_click,0,1);

		label_create(8,8,-8,21,path,white,left);
		defaultfont=helvetica12b;
		button_create(-(8+80*0),-8,72,24,"Cancel",child_window_close_f);
		button_create(-(8+80*1),-8,72,24,"Ok",child_window_close_f);
		defaultfont=helvetica12m;

		for(wlist_item=wlist_item->treestk;wlist_item->z!=list_item;wlist_item=wlist_item->next);
	}
}
/*############################### End of File Selection Widget #####################################################*/

/*############################### Internal Text file viewer Widget #################################################*/

void scrollf(struct xtree *t)
{
	strcpy(text_buffer,stack[stackptr]);
	broadcast++;
}

void view_file(char * filename)
{
	int n;
	FILE *file;
	
	stackmax=0;

	if(!(file=fopen(filename,"r")))
		printf("Cannot open file: %s\n",filename);
	else
	{
		for( ;fgets(text_buffer,SIZE,file) && stackmax!=MAXLINES;stackmax++)
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
}

void vf_window()
{
	window_create(0,0,512,512,VERSION);
	scroll_create(0,0,-1,-41,&stackptr,&stackmax,stack,scrollf);
	button_create(-(8+80*0),-8,72,24,"Ok", child_window_close_f);
}

/*############################### End   of Text file viewer Widget #################################################*/

/*############################### Fprot-info, Virus-list, Update, Test & Reprot button functions ###################*/

void run(char *command_name)
{
	command=(char *)malloc(strlen(TERMINAL)+strlen(XFPROT_DIR)+strlen(command_name)+strlen("  &")+1);
	sprintf(command,"%s %s%s &",TERMINAL,XFPROT_DIR,command_name);
	#ifdef DEBUG
	printf("%s\n",command);
	#endif
	system(command);
	free(command);
}
void fprot_virinfo()
{
	run(VIRINFO_CMD);
}

void fprot_virlist()
{
	run(VIRLIST_CMD);
}

void fprot_update()
{
	run(UPDATE_CMD);
}

void fprot_test()
{
	run(VIRTEST_CMD);
}

void view_report()
{
	if(lock==0)
	{
		lock=1;
		vf_window();
		view_file(report_file);
	}
}

/*###############################End of Fprot-info, Virus-list, Update, Test & Report buttons functions ############*/

/*############################### Scan button function  ############################################################*/

void add_to_command(char *option)
{
	command=(char*)realloc(command,strlen(option)+1+strlen(command));
	strcat(command,option);
	#ifdef DEBUG
	printf("%s\n",command);
	#endif
}

void scan_f()
{
	command=(char *)malloc(strlen(TERMINAL)+strlen(XFPROT_DIR)+strlen(SCAN_CMD)+strlen(path)+1+2);

	sprintf(command,"%s %s%s %s",TERMINAL,XFPROT_DIR,SCAN_CMD,path);

	/* Let's parse the options*/
	if(strcmp(flag_scanmode,"Report only")!=0)
	{
		if(strcmp(flag_disinf,"Disinfect")==0)
			add_to_command(" -disinf");
		if(strcmp(flag_rename,"Rename")==0)
			add_to_command(" -rename");
		if(strcmp(flag_delete,"Delete")==0)
			add_to_command(" -delete");
		if(strcmp(flag_scanmode,"Automatic")==0)
			add_to_command(" -auto");
	}
	if(strcmp(flag_file_types,"Attempt to identify files")==0)
		add_to_command(" -type");
	if(strcmp(flag_file_types,"Standard file extensions")==0)
		add_to_command(" -ext");
	if(strcmp(flag_file_types,"Scan all files")==0)
		add_to_command(" -dumb");
	if(strcmp(flag_archives,"Scan inside archives")==0)
		add_to_command(" -archive");
	if(strcmp(flag_comp_exe,"Scan compressed executables")==0)
		add_to_command(" -packed");
	if(strcmp(flag_nosub,"Do not scan subdirectories")==0)
		add_to_command(" -nosub");
	if(strcmp(flag_collect,"Scan for other various malware")==0)
		add_to_command(" -collect");
	if(strcmp(flag_list,"List all scanned files")==0)
  		add_to_command(" -list");
	if(strcmp(flag_ai,"Enable neural network virus detection")==0)
		add_to_command(" -ai");
	if(strcmp(flag_old,"Do not complain when using outdated DEF files")==0)
		add_to_command(" -old");
	if(strcmp(flag_follow,"Follow symbolic links")==0)
		add_to_command(" -follow");
	/* I prefer to create always a report file so no flag checking here */
		add_to_command(" -report=");
		add_to_command(report_file);
	/* -wrap switch is not supported, if you want to edit the logs use a text editor */
	if(strcmp(flag_append,"Append to existing report file")==0)
		add_to_command(" -append");
	if(strcmp(flag_heur,"Do not use heuristics")==0)
		add_to_command(" -noheur");
	if(strcmp(flag_heur,"Use only heuristics")==0)
		add_to_command(" -onlyheur");
	if(strcmp(flag_macro,"Do not scan for macro virus")==0)
		add_to_command(" -nomacro");
	if(strcmp(flag_macro,"Scan only for macro virus")==0)
		add_to_command(" -onlymacro");
	if(strcmp(flag_scanmode,"Report only")!=0 && strcmp(flag_macro," -nomacro")!=0)
	{
		if(strcmp(flag_macro_action,"Remove infected macros from docs")==0)
			add_to_command(" -saferemove");
		if(strcmp(flag_macro_action,"Remove all macros from infected docs")==0)
			add_to_command(" -removenew");
		if(strcmp(flag_macro_action,"Remove all macros from all docs")==0)
			add_to_command(" -removeall");
	}
	add_to_command(" &");
	#ifdef DEBUG
	printf("%s\n",command);
	#endif
	system(command);
	free(command);
}

/*############################## End of Scan button function  ######################################################*/

/*############################# About dialog box function ##########################################################*/

void read_license(struct xtree *treeptr)
{
	char *filename;
	filename=(char *)malloc(strlen(XFPROT_DIR)+strlen(LICENSE_FILE)+1);
	sprintf(filename,"%s%s",XFPROT_DIR,LICENSE_FILE);
	window_close(treeptr);
	vf_window();
	view_file(filename);
	free(filename);
}

void about_f()
{
	if(lock==0)
	{
		lock=1;

		sprintf(flag_scanmode_bak,"%s",flag_scanmode);

		dialog_create(450,200);
		defaultfont=helvetica24b;
		label_create(1,1,-1,70,VERSION,gray,up);
		defaultfont=helvetica12m;
		label_create(8,8+3*21,-8,21,COPYRIGHT,invisible,center);
		label_create(8,8+4*21,-8,21,HOMEPAGE,invisible,center);
		label_create(8,8+5*21,-8,21,WARRANTY_0,invisible,center);
		label_create(8,8+6*21,-8,21,WARRANTY_1,invisible,center);
		defaultfont=helvetica12b;
		button_create( -(8+80*0),-8,72,24,"OK",child_window_close_f);
		button_create( 8,-8,72*2,24,"READ GPL LICENSE",read_license);
		defaultfont=helvetica12m;
	}
}
/*############################## End of About dialog box function ###############################################*/

/*############################## Toggle  function for the main window ###########################################*/
void toggle_f()
{
	if(lock==0)
	{
		if(strcmp(flag_scanmode,"Interactive")==0 ||strcmp(flag_scanmode,"Automatic")==0  )
		{
		/* It's better to reset the flags to default values
		here to avoid leftovers from previous toggles */
			if(reset==0)
			{
				sprintf(flag_disinf,"%s","Disinfect");
				sprintf(flag_rename,"%s","");
				sprintf(flag_delete,"%s","");
				reset=1;
			}
			defaultfont=helvetica12b;
			label_create(8,8+3*21,-300,21,"Action:",invisible,left);
			defaultfont=helvetica12m;
			check_create(8,8+4*21,200,21,"Disinfect",flag_disinf);
			check_create(8,8+5*21,200,21,"Rename",flag_rename);
			check_create(8,8+6*21,200,21,"Delete",flag_delete);

			label_create(100,8+4*21,200,21,"It is  possible  to specify",invisible,left);
			label_create(100,8+5*21,200,21,"combinations of switches",invisible,left);
		}
		else
		{
			reset=0;
			box_create (0, 8+3*21, 300, 84,0,gray);
		}
	}
	/* freeze flag_scanmode here until a child win is opened so we avoid
	the possibility to change scan mode without contestual toggling */
	else
		sprintf(flag_scanmode,"%s",flag_scanmode_bak);
}
/*##############################End of Toggle  function for the main window #####################################*/

/*############################## Show-Hide  function for the main window ###########################################*/

void resize_f(struct xtree *t)
{
	if(show==0)
	{
		XResizeWindow(display,win->win,640,z);
		sprintf(label,"%s","Hide");
		show=1;
	}
	else
	{
		XResizeWindow(display,win->win,640,y);
		sprintf(label,"%s","Options");
		show=0;
	}
}

/*##############################End of Show-Hide  function for the main window #####################################*/

/*##################################### Main #######################################################################*/
void xmain(int n,char *p[])
{
	list_f();
	single_click();
	win=window_create(0,0,640,y,VERSION);
	win->broadcast=toggle_f;
	/*
	treestk is a pointer to the last window in the stack
	of windows and treestk->message by default is a pointer
	to function window_close(). you can intercept a
	window_close() and run any function before this window
	is closed!
	*/
	treestk->message=main_window_close_f;

	defaultfont=helvetica12b;
	label_create(8,8+0*21,-8,21,"Path to scan:",invisible,left);
	defaultfont=helvetica12m;
	edit_create(95,8,-90,21,path,PATH_MAX,0);
	defaultfont=helvetica12b;
	button_create(-(8+80*0),7+0*21,72,24,"Select",file_select);
	defaultfont=helvetica12m;

	defaultfont=helvetica12b;
	label_create(8,8+2*21,-200,21,"Scan mode:",invisible,left);
	defaultfont=helvetica12m;
	radio_create(90,8+2*21,200,21,"Report only",flag_scanmode);
	radio_create(190,8+2*21,400,21,"Interactive",flag_scanmode);
	radio_create(290,8+2*21,400,21,"Automatic",flag_scanmode);

	defaultfont=helvetica12b;
	label_create(380,8+2*21,-8,21,"File types:",invisible,left);
	defaultfont=helvetica12m;
	radio_create(380,8+3*21,400,21,"Standard file extensions",flag_file_types);
	radio_create(380,8+4*21,400,21,"Attempt to identify files",flag_file_types);
	radio_create(380,8+5*21,400,21,"Scan all files",flag_file_types);

	defaultfont=helvetica12b;
	label_create(6,8+7*21,-8,21,"Report to file:",invisible,left);
	defaultfont=helvetica12m;
	/* Try to detect the home directory of the user else use default*/
	if(getenv("HOME")==NULL)
			sprintf(report_file,"%s","f-prot.log");
	else
			sprintf(report_file,"%s/%s",getenv("HOME"),"f-prot.log");
	edit_create(95,8+7*21,278,21,report_file,PATH_MAX,0);
	check_create(8,8+9*21,400,21,"Append to existing report file",flag_append);

	check_create(8,8+12*21,400,21,"Scan inside archives",flag_archives);
	check_create(8,8+13*21,400,21,"Scan compressed executables",flag_comp_exe);
	check_create(8,8+14*21,400,21,"Do not scan subdirectories",flag_nosub);
	check_create(8,8+15*21,400,21,"Scan for other various malware",flag_collect);
	check_create(8,8+16*21,400,21,"List all scanned files",flag_list);
	check_create(8,8+17*21,400,21,"Enable neural network virus detection",flag_ai);
	check_create(8,8+18*21,400,21,"Follow symbolic links",flag_follow);
	check_create(8,8+19*21,400,21,"Do not complain when using outdated DEF files",flag_old);

	defaultfont=helvetica12b;
	label_create(380,8+6*21,-8,21,"Macro virus:",invisible,left);
	defaultfont=helvetica12m;
	radio_create(380,8+7*21,400,21,"Scan for macro virus",flag_macro);
	radio_create(380,8+8*21,400,21,"Do not scan for macro virus",flag_macro);
	radio_create(380,8+9*21,400,21,"Scan only for macro virus",flag_macro);

	defaultfont=helvetica12b;
	label_create(380,8+12*21,-8,21,"Heuristics:",invisible,left);
	defaultfont=helvetica12m;
	radio_create(380,8+13*21,400,21,"Use heuristics",flag_heur);
	radio_create(380,8+14*21,400,21,"Do not use heuristics",flag_heur);
	radio_create(380,8+15*21,400,21,"Use only heuristics",flag_heur);

	defaultfont=helvetica12b;
	label_create(380,8+16*21,-8,21,"Macro virus options:",invisible,left);
	defaultfont=helvetica12m;
	radio_create(380,8+17*21,400,21,"Remove infected macros from docs",flag_macro_action);
	radio_create(380,8+18*21,400,21,"Remove all macros from infected docs",flag_macro_action);
	radio_create(380,8+19*21,400,21,"Remove all macros from all docs",flag_macro_action);

	defaultfont=helvetica12b;
	button_create(-(8+70*0),11*21,68,24,"Exit",main_window_close_f);
	button_create(-(8+70*1),11*21,68,24,"F-prot Info",fprot_virinfo);
	button_create(-(8+(70*2)),11*21,68,24,"Virus List",fprot_virlist);
	button_create(-(8+(70*3)),11*21,68,24,"Test",fprot_test);
	button_create(-(8+(70*4)),11*21,68,24,"Update",fprot_update);
	button_create(-(8+(70*5)),11*21,68,24,"Report",view_report);
	button_create(-(8+(70*6)),11*21,68,24,label,resize_f);
	button_create(-(8+(70*7)),11*21,68,24,"Scan",scan_f);
	button_create(6,11*21,66,24,"About",about_f);
	defaultfont=helvetica12m;
}
/*####################################### End of Main ##############################################################*/


