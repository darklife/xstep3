#include <xstep.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>
#include <dirent.h>
#include <unistd.h>

#define VERSION "xstep-shell 1.0b"
#define MAXLINES 1000000

int i,
	n,
	b=0,
	p=0,
	size_x=420,
	size_y=200,
	flag_big_font=0,
	start_of_button_labels=0,
	num_of_buttons=0,
	xscroll=0,
	start_of_radio_button_labels=0,
	num_of_radio_buttons=0,
	start_of_checkbox_button_labels=0,
	num_of_checkbox_buttons=0,
	start_of_query_labels=0,
	num_of_query=0,
	start_of_query_values=0,
	num_of_query_values=0,
	pos_counter=0,
	check_box_relative_position=0,
	radio_button_relative_position=0,
	query_relative_position=0,
	flag_view_file=0,
	filename_pos=0,
	print_line=0,
	stackmax,
	stackptr,
	file_selection=0,
	list_item_ptr,
    listmax,
    service_listmax;

char*title=VERSION,
	*label="",
	flagthr[64],
	flag[50][256],
	buffer[50][256],
	text_buffer[1024],
	*stack[MAXLINES],
	**list_item,
	**service_list_item,
	path[PATH_MAX];

struct	xtree *wlist_item,*waux;
struct	dirent  **namelist;
struct	stat stat_p;
struct	mlist list[]={{"",NULL,NAME_MAX+1}};
struct	xtree *wid;

void button_f(struct xtree *wid);

void help()
{
	puts("xstep-shell (C) 2001-2003 Tito Ragusa <tito-wolit@tiscali.it> under GPL");
	puts("This program will use the xstep-3.5 library (http://xstep.dhs.org) to ask");
	puts("for user input, and can be used in scripts or called from other programs.\n");
	puts("--help, -h                               This help text");
	puts("--version, -v                            Show xstep-shell's version");
	puts("--size_x <size>, -sx                     Set the window width in pixels");
	puts("--size_y <size>, -sy                     Set the window height in pixels");
	puts("--title <title>, -t                      Set the window title");
	puts("--label <label>, -l                      Show a label to the user");
	puts("--big-font,-bf                           Doubles label's font size");
	puts("--query <label 1><label n>, -q           Display one or more query boxes");
	puts("                                         with the given label");
	puts("--query-value <value 1><value n>, -qv    Set a value in the corresponding query");
	puts("                                         box ");
	puts("--checkbox <option 1><option n>, -cb     Display a checkbox with the given");
	puts("                                         options");
	puts("--radio-button <option 1><option n>,-rb  Display a radio-button with the given");
	puts("                                         options");
	puts("--x-scroll,-xs                           Add a scroll bars to radio-button,");
	puts("                                         checkbox, query box lists");
	puts("--button <label 1><label n>, -b          Add one or more buttons at the bottom");
	puts("                                         of the window");
	puts("--file-selection, -fs                    Display a file selection box");
	puts("                                         (accepts also -t)");
	puts("--view-file <file>, -vf                  View a file in a text box");
	puts("                                         (accepts -t,-pl,-b)");
	puts("--print-line, -pl                        On exit prints the line selected in");
	puts("                                         the text box");
	puts("--edit-file , -ef                        Not supported in this version");
	exit(EXIT_FAILURE);
}

/*################################### This stuff is for the file selection widget###################################*/
void list_f()
{
	listmax=0;
	service_listmax=0;

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

void file_select()
{
	list_f();
	single_click();

	window_create(0,0,450,350,title);

	wlist_item=mscroll_create(8,8,-8,-40,&list_item_ptr,&listmax,list,single_click,double_click,0,1);

	label_create(8,8,-8,21,path,white,left);
	button_create(-(8+80*0),-8,72,24,"Cancel",window_close);
	button_create(-(8+80*1),-8,72,24,"Ok",button_f);

	for(wlist_item=wlist_item->treestk;wlist_item->z!=list_item;wlist_item=wlist_item->next);
}
/*############################### End of File Selection Widget #####################################################*/

void check_missing_arg(int index, int argc ,char *nextarg, char *optname)
{
	if( index >= (argc - 1)||strncmp(nextarg,"-",1)==0)
	{
		printf("option %s needs an argument\n",optname);
		exit(EXIT_FAILURE);
	}
}

void test_int(int test, char * optname)
{
	if(!(test/1))
	{
		printf("value of option %s is not an integer or is not greater than 0\n",optname);
		exit(EXIT_FAILURE);
	}
}

int count_args(int start )
{
	int counter=0;

	while((start + counter) < argc )
	{
		if(strncmp(argv[ start + counter],"-",1)==0)
			break;
		counter++;
	}
	return counter;
}

void set_position(int relative_position)
{
	if (relative_position == 1 )
	{
		if (strlen(label)>0)
		{
			if (flag_big_font == 1)
				pos_counter = 3;
			else
				pos_counter = 2;
		}
		else
			pos_counter = 1;
		if ( xscroll == 1)
			pos_counter = 0;
	}
}

void rb_f()
{
	set_position(radio_button_relative_position);
	for(n=0;n<num_of_radio_buttons;n++)
	{
		radio_create(8,8+(pos_counter + n)*24,400,21,argv[start_of_radio_button_labels + n ],flagthr);

		/* to initialize the first radio button as active */
		/*if(n==0)
			sprintf(flagthr,"%s",argv[start_of_radio_button_labels]);*/
	}
	pos_counter+=num_of_radio_buttons;
}

void cb_f()
{
	set_position(check_box_relative_position);
	for(n=0;n<num_of_checkbox_buttons;n++)
		check_create(8,8 + (pos_counter + n)* 24,400,21,argv[start_of_checkbox_button_labels + n ],flag[n]);

	pos_counter+=num_of_checkbox_buttons;
}

void q_f()
{
	set_position(query_relative_position);
	for(n=0;n<num_of_query;n++)
	{
		label_create(8,8 + (pos_counter + n )*24,90,21,argv[start_of_query_labels + n],invisible,left);
		edit_create(104,8 + (pos_counter + n )*24,280,21,buffer[n],255,0);
		if(num_of_query_values > 0 && n < num_of_query_values)
			strcpy(buffer[n],argv[start_of_query_values + n ]);
	}
	pos_counter+=num_of_query;
}

void cb_rb_q_f()
{
	for(i=1;i<=3;i++)
	{
		if(check_box_relative_position == i )
			cb_f();
		if(radio_button_relative_position == i)
			rb_f();
		if(query_relative_position == i )
			q_f();
	}
}

void radio_button_close_f()
{
	if(num_of_radio_buttons > 1)
		printf("radio_button %s\n", flagthr);
}

void checkbox_close_f()
{
	if( num_of_checkbox_buttons > 0)
	{
		printf("checkbox ");
        for(n=0;n < num_of_checkbox_buttons; n++)
			printf("%s ", flag[n]) ;
        printf("\n");
	}
}

void query_close_f()
{
	if ( num_of_query > 0 )
	{
        for(n=0;n < num_of_query; n++)
			printf("%s : %s ;\n",argv[start_of_query_labels + n ],buffer[n]) ;
        printf("\n");
	}
}

void scrollf(struct xtree *t)
{
	strcpy(text_buffer,stack[stackptr]);
	broadcast++;
}

void vf_close_and_print()
{
	if(print_line == 1)
		printf("%s\n",text_buffer);
}

void view_file()
{
	FILE *file;
	if(!(file=fopen(argv[filename_pos],"r")))
	{
		printf("Cannot open file: %s\n",argv[filename_pos]);
		exit(EXIT_FAILURE);
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

void button_f(struct xtree *wid)
{
	/*the button label is stored in the widget struct, so
	if you have a struct xtree *wid, you can see the label in the
	address wid->t.*/
	if(file_selection==1)
		printf("%s\n",path);
	else
	{
		printf("%s\n",wid->t);
		radio_button_close_f();
		checkbox_close_f();
		query_close_f();
		vf_close_and_print();
	}
	window_close(wid);
}

void xmain(int argc ,char **argv)
{
	if(argc==1)
		help();
	/* Let's parse the command line options */
	for( i=1 ; i < argc ; i++)
	{
		if(strcmp(argv[i],"--version")==0 || strcmp(argv[i],"-v")==0)
		{
			puts(VERSION);
			exit(EXIT_SUCCESS);
		}
		
		if(strcmp(argv[i],"--help")==0 || strcmp(argv[i],"-h")==0)
			help();

		if(strcmp(argv[i],"--size_x")==0 || strcmp(argv[i],"-sx")==0)
		{
			check_missing_arg(i,argc,argv[i+1],argv[i]);
			size_x=atoi(argv[i+1]);
			test_int(size_x,argv[i]);
		}

		if(strcmp(argv[i],"--size_y")==0 || strcmp(argv[i],"-sy")==0)
		{
			check_missing_arg(i,argc,argv[i+1],argv[i]);
			size_y=atoi(argv[i+1]);
			test_int(size_y,argv[i]);
		}

		if(strcmp(argv[i],"--title")==0 || strcmp(argv[i],"-t")==0)
		{
			check_missing_arg(i,argc,argv[i+1],argv[i]);
			title=argv[i+1];
		}

		if(strcmp(argv[i],"--label")==0 || strcmp(argv[i],"-l")==0)
		{
			check_missing_arg(i,argc,argv[i+1],argv[i]);
			label=argv[i+1];
		}
		
		if(strcmp(argv[i],"--big-font")==0 || strcmp(argv[i],"-bf")==0)
			flag_big_font=1;

		if(strcmp(argv[i],"--button")==0 || strcmp(argv[i],"-b")==0)
		{
			check_missing_arg(i,argc,argv[i+1],argv[i]);
			start_of_button_labels=i+1;
			num_of_buttons=count_args(start_of_button_labels);
		}
		
		if(strcasecmp(argv[i],"--radio-button")==0 || strcasecmp(argv[i],"-rb")==0)
		{
			/* radio button needs at least two args so we check twice */
			check_missing_arg(i,argc,argv[i+1],argv[i]);
			check_missing_arg(i,argc,argv[i+2],argv[i]);
			radio_button_relative_position = ++p;
			start_of_radio_button_labels=i+1;
			num_of_radio_buttons=count_args(start_of_radio_button_labels);
		}

		if(strcasecmp(argv[i],"--checkbox")==0 || strcasecmp(argv[i],"-cb")==0)
		{
			check_missing_arg(i,argc,argv[i+1],argv[i]);
			check_box_relative_position = ++p;
			start_of_checkbox_button_labels=i+1;
			num_of_checkbox_buttons=count_args(start_of_checkbox_button_labels);
		}
		
		if(strcasecmp(argv[i],"--query")==0 || strcasecmp(argv[i],"-q")==0)
		{
			check_missing_arg(i,argc,argv[i+1],argv[i]);
			query_relative_position = ++p ;
			start_of_query_labels=i+1;
			num_of_query=count_args(start_of_query_labels);
		}
		
		if(strcasecmp(argv[i],"--query-value")==0 || strcasecmp(argv[i],"-qv")==0)
		{
			check_missing_arg(i,argc,argv[i+1],argv[i]);
			start_of_query_values=i+1;
			num_of_query_values=count_args(start_of_query_values);
		}

		if(strcasecmp(argv[i],"--x-scroll")==0 || strcasecmp(argv[i],"-xs")==0)
		{
			xscroll = 1;
			size_y = 400 ;
		}
		
		if(strcasecmp(argv[i],"--view-file")==0 || strcasecmp(argv[i],"-vf")==0)
		{
			flag_view_file = 1 ;
			size_x=size_y=512;
			check_missing_arg(i,argc,argv[i+1],argv[i]);
			filename_pos = i+1;
			lstat(argv[filename_pos],&stat_p);
			if (!S_ISREG(stat_p.st_mode))
			{
				printf("%s is not a regular file\n",argv[filename_pos]);
				exit(EXIT_FAILURE);
			}
		}
		
		if(strcasecmp(argv[i],"--print-line")==0 || strcasecmp(argv[i],"-pl")==0)
			print_line = 1;

		if(strcasecmp(argv[i],"--file-selection")==0 || strcasecmp(argv[i],"-fs")==0)
			file_selection = 1;
		
		if(strcmp(argv[i],"--edit-file")==0 || strcmp(argv[i],"-ef")==0)
		{
			printf("option --edit-file, -ef is not supported by %s\n",VERSION);
			exit(EXIT_FAILURE);
		}
	}


	/* If we want a file selection box */
	if(file_selection==1)
		file_select();
	else
	{
		/* Now let's build our window with given title and sizes */
		window_create(0,0,size_x,size_y,title);

		if(flag_view_file>0)
		{
			view_file();
			scroll_create(0,0,-1,-41,&stackptr,&stackmax,stack,scrollf);
			button_create(-(8+80*0),-8,72,24,"Ok", button_f);
			b=1;
			goto create_buttons;
		}
		/* Show a label on the window if there is one */
		if(strlen(label)>0)
		{
			/* Set the font to big if needed */
			if(flag_big_font == 1)
				defaultfont=helvetica24b;
			label_create(0,0,0,90,label, invisible,center);
			/* reset the default font */
			defaultfont=helvetica12m;
		}
		/* Create radio buttons, check boxes, edit boxes and scroll bars if needed */
		if( xscroll == 1 )
			xyscroll_group(8,60,-8,-40,cb_rb_q_f,gray,size_y,(num_of_radio_buttons+num_of_checkbox_buttons+num_of_query)*(21+4)+4);
		else
			cb_rb_q_f();
		/* Create the given buttons */
		create_buttons:
		for(n=0;n<num_of_buttons;n++)
			button_create(-(8+80*(n+b)),-8,72,24,argv[start_of_button_labels + n ], button_f);
	}
}
