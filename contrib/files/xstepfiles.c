#include <xstep.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <sys/types.h>

char *files[2000],*dirs[2000],directory[256],
     info[256],program[64]="netscape",
     dest[256]="",*command;

struct mlist dlist[]={{ "Directories",dirs,200 }};
struct mlist flist[]={{ "Files",files,200 }};
struct stat stat_p;
struct plist
{
   char owner[9],group[9];
   char ur[2],uw[2],ux[2];
   char gr[2],gw[2],gx[2];
   char or[2],ow[2],ox[2];
} permissions;

int fptr,fmax,dptr,dmax,lock=0;
long int totalsize=0;

char *format_time(time_t cal_time)
{
   struct tm *time_struct;
   static char string[30];
   time_struct=localtime(&cal_time);
   strftime(string,sizeof string,"%h %e %H:%M\n",time_struct);
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

void updatedirs(void)
{
   DIR *dir_p;
   struct dirent *dir_entry_p;
   int i;
   for(i=0;i!=dmax;i++) free(dirs[i]);
   dmax=0;
   dptr=0;
   dir_p=opendir("./");
   while((dir_entry_p=readdir(dir_p)) != NULL)
   {
      if (strcmp(dir_entry_p->d_name,"."))
      {
         stat(dir_entry_p->d_name,&stat_p);
         if (S_ISDIR(stat_p.st_mode))
         {
            dirs[dmax]=(char *)malloc(strlen(dir_entry_p->d_name)+2);
            strcpy(dirs[dmax],dir_entry_p->d_name);
            strcat(dirs[dmax],"/");
            dmax++;
         }
      }
   }
   closedir(dir_p);
   qsort(dirs,dmax,sizeof *dirs,compare);
   strcpy(directory,currentdir());
}

void updatefiles(void)
{
   DIR *dir_p;
   struct dirent *dir_entry_p;
   int i;
   for(i=0;i!=fmax;i++) free(files[i]);
   fmax=0;
   fptr=0;
   totalsize=0;
   dir_p=opendir("./");
   while((dir_entry_p=readdir(dir_p)) != NULL)
   {
      stat(dir_entry_p->d_name,&stat_p);
      if (!S_ISDIR(stat_p.st_mode))
      {
         files[fmax]=(char *)malloc(strlen(dir_entry_p->d_name)+1);
         strcpy(files[fmax],dir_entry_p->d_name);
         lstat(dir_entry_p->d_name,&stat_p);
         totalsize+=stat_p.st_size;
         fmax++;
      }
   }
   closedir(dir_p);
   qsort(files,fmax,sizeof *files,compare);
}

long int filesize(void)
{
   return(stat_p.st_size);
}

char *owner(void)
{
   struct passwd *pws;
   if ((pws=getpwuid(stat_p.st_uid)) != NULL) return(pws->pw_name);
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
   if ((gps=getgrgid(stat_p.st_gid)) != NULL) return(gps->gr_name);
   else
   {
      char *gid=(char *)malloc(10);
      sprintf(gid,"%d",stat_p.st_gid);
      return(gid);
   }
}

char *modeline(void)
{
   char *line;
   line=(char *)malloc(11);
   if (S_ISLNK(stat_p.st_mode)) strcpy(line,"l");
   if (S_ISREG(stat_p.st_mode)) strcpy(line,"-");
   if (S_ISDIR(stat_p.st_mode)) strcpy(line,"d");
   if (S_ISCHR(stat_p.st_mode)) strcpy(line,"c");
   if (S_ISBLK(stat_p.st_mode)) strcpy(line,"b");
   if (S_ISFIFO(stat_p.st_mode)) strcpy(line,"f");
   if (S_ISSOCK(stat_p.st_mode)) strcpy(line,"s");
   strcat(line,(stat_p.st_mode & S_IRUSR)?"r":"-");
   strcat(line,(stat_p.st_mode & S_IWUSR)?"w":"-");
   strcat(line,(stat_p.st_mode & S_IXUSR)?"x":"-");
   strcat(line,(stat_p.st_mode & S_IRGRP)?"r":"-");
   strcat(line,(stat_p.st_mode & S_IWGRP)?"w":"-");
   strcat(line,(stat_p.st_mode & S_IXGRP)?"x":"-");
   strcat(line,(stat_p.st_mode & S_IROTH)?"r":"-");
   strcat(line,(stat_p.st_mode & S_IWOTH)?"w":"-");
   strcat(line,(stat_p.st_mode & S_IXOTH)?"x":"-");
   return(line);
   free(line);
}

char *timestr(void)
{
   return format_time(stat_p.st_mtime);
}

void finf(void)
{
   if (fmax > 0)
   {
      lstat(files[fptr],&stat_p);
      sprintf(info,"%s     %s     %s     %ld     %s",
         modeline(),owner(),group(),filesize(),timestr());
   }
   else sprintf(info,"no file selected");
   broadcast++;
}

void dinf(void)
{
   lstat(dirs[dptr],&stat_p);
   sprintf(info,"%s     %s     %s     %ld     %s",
      modeline(),owner(),group(),filesize(),timestr());
   broadcast++;
}

void update(void)
{
   updatedirs();
   updatefiles();
   sprintf(info,"%d files     %d directories     %ld bytes",
      fmax,dmax-1,totalsize);
}

void cd(struct xtree *treeptr)
{
   if (lock == 0)
   {
      chdir(dirs[dptr]);
      update();
   }
}

void runprog(struct xtree *treeptr)
{
   char *cmdline;
   cmdline=(char *)malloc(strlen(program)+strlen(files[fptr])+6);
   sprintf(cmdline,"%s \"%s\" &",program,files[fptr]);
   system(cmdline);
   free(cmdline);
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

void nothing(struct xtree *treeptr)
{
   window_close(treeptr);
   lock=0;
}

void open_w(void)
{
   if (fmax > 0)
   {
      char static *plist[7]=
      {
         "netscape","gimp","xv","gnp","xwpe",
         "guitar","x11amp"
      };
      lock=1;
      dialogbox_create(400,180);
      label_create(0,0,0,-90,"Open with ...",gray,up);
      XSetFont(display,treestk->treestk->gc,
         (treestk->treestk->font=helvetica24b)->fid);
      label_create(0,-90,0,0,"",gray,up);
      button_create(-88,-8,72,24,"Ok",runprog);
      button_create(-8,-8,72,24,"Cancel",nothing);
      popup_create(8,-48,200,21,program,plist,7*21);
      edit_create(8,-18,200,21,program,64,0);
   }
}

void edit(void)
{
   if (fmax > 0)
   {
      char *cmdline;
      char *editor="gnp";
      cmdline=(char *)malloc(strlen(editor)+strlen(files[fptr])+6);
      sprintf(cmdline,"%s \"%s\" &",editor,files[fptr]);
      system(cmdline);
      free(cmdline);
   }
}

void copy(void)
{
   if (fmax > 0)
   {
      lock=1;
      command=(char *)malloc(strlen(files[fptr])+305);
      sprintf(command,"cp \"%s\" ",files[fptr]);
      dialogbox_create(400,180);
      label_create(0,0,0,-90,"Copy to ...",gray,up);
      XSetFont(display,treestk->treestk->gc,
         (treestk->treestk->font=helvetica24b)->fid);
      label_create(0,-90,0,0,"",gray,up);
      button_create(-88,-8,72,24,"Ok",runcommand);
      button_create(-8,-8,72,24,"Cancel",cancel);
      label_create(8,-48,200,21,files[fptr],white,left);
      XSetFont(display,treestk->treestk->gc,
         (treestk->treestk->font=helvetica12m)->fid);
      edit_create(8,-18,200,21,dest,256,0);
   }
}

void move(void)
{
   if (fmax > 0)
   {
      lock=1;
      command=(char *)malloc(strlen(files[fptr])+305);
      sprintf(command,"mv \"%s\" ",files[fptr]);
      dialogbox_create(400,180);
      label_create(0,0,0,-90,"Move to ...",gray,up);
      XSetFont(display,treestk->treestk->gc,
         (treestk->treestk->font=helvetica24b)->fid);
      label_create(0,-90,0,0,"",gray,up);
      button_create(-88,-8,72,24,"Ok",runcommand);
      button_create(-8,-8,72,24,"Cancel",cancel);
      label_create(8,-48,200,21,files[fptr],white,left);
      XSetFont(display,treestk->treestk->gc,
         (treestk->treestk->font=helvetica12m)->fid);
      edit_create(8,-18,200,21,dest,256,0);
   }
}

void del(void)
{
   if (fmax > 0)
   {
      lock=1;
      command=(char *)malloc(strlen(files[fptr])+6);
      sprintf(command,"rm \"%s\"",files[fptr]);
      dialogbox_create(400,180);
      label_create(0,0,0,-90,"Delete file?",gray,up);
      XSetFont(display,treestk->treestk->gc,
         (treestk->treestk->font=helvetica24b)->fid);
      label_create(0,-90,0,0,"",gray,up);
      button_create(-88,-8,72,24,"Ok",runcommand);
      button_create(-8,-8,72,24,"Cancel",cancel);
      label_create(8,-38,200,21,files[fptr],white,left);
      XSetFont(display,treestk->treestk->gc,
         (treestk->treestk->font=helvetica12m)->fid);
   }
}

void lnk(void)
{
   if (fmax > 0)
   {
      lock=1;
      command=(char *)malloc(strlen(currentdir())+strlen(files[fptr])+308);
      sprintf(command,"ln -s \"%s/%s\" ",currentdir(),files[fptr]);
      dialogbox_create(400,180);
      label_create(0,0,0,-90,"Link to ...",gray,up);
      XSetFont(display,treestk->treestk->gc,
         (treestk->treestk->font=helvetica24b)->fid);
      label_create(0,-90,0,0,"",gray,up);
      button_create(-88,-8,72,24,"Ok",runcommand);
      button_create(-8,-8,72,24,"Cancel",cancel);
      label_create(8,-48,200,21,files[fptr],white,left);
      XSetFont(display,treestk->treestk->gc,
         (treestk->treestk->font=helvetica12m)->fid);
      edit_create(8,-18,200,21,dest,256,0);
   }
}

void perm(void)
{
   if (fmax > 0)
   {
      char l[11];
      lock=1;
      lstat(files[fptr],&stat_p);
      command=(char *)malloc(strlen(files[fptr])+1);
      strcpy(command,files[fptr]);
      strcpy(l,modeline());
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
}

void createdir(void)
{
   lock=1;
   command=(char *)malloc(305);
   strcpy(command,"mkdir ");
   dialogbox_create(400,180);
   label_create(0,0,0,-90,"Create dir ...",gray,up);
   XSetFont(display,treestk->treestk->gc,
      (treestk->treestk->font=helvetica24b)->fid);
   label_create(0,-90,0,0,"",gray,up);
   button_create(-88,-8,72,24,"Ok",runcommand);
   button_create(-8,-8,72,24,"Cancel",cancel);
   edit_create(8,-38,200,21,dest,256,0);
}

void copydir(void)
{
   if (dptr > 0)
   {
      lock=1;
      command=(char *)malloc(strlen(dirs[dptr])+308);
      sprintf(command,"cp -r \"%s\" ",dirs[dptr]);
      dialogbox_create(400,180);
      label_create(0,0,0,-90,"Copy to ...",gray,up);
      XSetFont(display,treestk->treestk->gc,
         (treestk->treestk->font=helvetica24b)->fid);
      label_create(0,-90,0,0,"",gray,up);
      button_create(-88,-8,72,24,"Ok",runcommand);
      button_create(-8,-8,72,24,"Cancel",cancel);
      label_create(8,-48,200,21,dirs[dptr],white,left);
      XSetFont(display,treestk->treestk->gc,
         (treestk->treestk->font=helvetica12m)->fid);
      edit_create(8,-18,200,21,dest,256,0);
   }
}

void movedir(void)
{
   if (dptr > 0)
   {
      lock=1;
      command=(char *)malloc(strlen(dirs[dptr])+305);
      sprintf(command,"mv \"%s\" ",dirs[dptr]);
      dialogbox_create(400,180);
      label_create(0,0,0,-90,"Move to ...",gray,up);
      XSetFont(display,treestk->treestk->gc,
         (treestk->treestk->font=helvetica24b)->fid);
      label_create(0,-90,0,0,"",gray,up);
      button_create(-88,-8,72,24,"Ok",runcommand);
      button_create(-8,-8,72,24,"Cancel",cancel);
      label_create(8,-48,200,21,dirs[dptr],white,left);
      XSetFont(display,treestk->treestk->gc,
         (treestk->treestk->font=helvetica12m)->fid);
      edit_create(8,-18,200,21,dest,256,0);
   }
}

void deldir(void)
{
   if (dptr > 0)
   {
      lock=1;
      command=(char *)malloc(strlen(dirs[dptr])+9);
      sprintf(command,"rm -r \"%s\"",dirs[dptr]);
      dialogbox_create(400,180);
      label_create(0,0,0,-90,"Delete dir? (recursive)",gray,up);
      XSetFont(display,treestk->treestk->gc,
         (treestk->treestk->font=helvetica24b)->fid);
      label_create(0,-90,0,0,"",gray,up);
      button_create(-88,-8,72,24,"Ok",runcommand);
      button_create(-8,-8,72,24,"Cancel",cancel);
      label_create(8,-38,200,21,dirs[dptr],white,left);
      XSetFont(display,treestk->treestk->gc,
         (treestk->treestk->font=helvetica12m)->fid);
   }
}

void lnkdir(void)
{
   if (dptr > 0)
   {
      lock=1;
      command=(char *)malloc(strlen(currentdir())+strlen(dirs[dptr])+308);
      sprintf(command,"ln -s \"%s/%s\" ",currentdir(),dirs[dptr]);
      dialogbox_create(400,180);
      label_create(0,0,0,-90,"Link to ...",gray,up);
      XSetFont(display,treestk->treestk->gc,
         (treestk->treestk->font=helvetica24b)->fid);
      label_create(0,-90,0,0,"",gray,up);
      button_create(-88,-8,72,24,"Ok",runcommand);
      button_create(-8,-8,72,24,"Cancel",cancel);
      label_create(8,-48,200,21,dirs[dptr],white,left);
      XSetFont(display,treestk->treestk->gc,
         (treestk->treestk->font=helvetica12m)->fid);
      edit_create(8,-18,200,21,dest,256,0);
   }
}

void permdir(void)
{
   if (dptr > 0)
   {
      char l[11];
      lock=1;
      lstat(dirs[dptr],&stat_p);
      command=(char *)malloc(strlen(dirs[dptr])+1);
      strcpy(command,dirs[dptr]);
      strcpy(l,modeline());
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
}

void dclick(struct xtree *treeptr)
{
   if ((fmax > 0) && (lock == 0))
   {
      lstat(files[fptr],&stat_p);
      if (modeline()[3] == 'x')
      {
         char *cmdline;
         cmdline=(char *)malloc(strlen(files[fptr])+5);
         sprintf(cmdline,"\"%s\" &",files[fptr]);
         system(cmdline);
      }
      else open_w();
   }
}

void menuff(struct xtree *treeptr)
{
   switch (treeptr->l)
   {
      case 0: finf(); break;
      case 1: update(); break;
      case 2: open_w(); break;
      case 3: edit(); break;
      case 4: copy(); break;
      case 5: move(); break;
      case 6: del(); break;
      case 7: lnk(); break;
      case 8: perm(); break;
      case 9: exit(0); break;
   }
}

void menudf(struct xtree *treeptr)
{
   switch (treeptr->l)
   {
      case 0: dinf(); break;
      case 1: update(); break;
      case 2: createdir(); break;
      case 3: copydir(); break;
      case 4: movedir(); break;
      case 5: deldir(); break;
      case 6: lnkdir(); break;
      case 7: permdir(); break;
      case 8: exit(0); break;
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
   if (lock == 0) menu_create(0,0,120,10*21,z,0,menuff);
}

void menud(struct xtree *treeptr)
{
   char static *z[9]=
   {
      "Dir info","Update","Create dir",
      "Copy","Move","Delete","Link",
      "Permissions","Quit"
   };
   if (lock == 0) menu_create(0,0,120,9*21,z,0,menudf);
}

void xmain(int n,char *p[])
{
   update();
   window_create(0,0,650,400,"XstepFiles");
   label_create(1,1,-1,21,directory,gray,left);
   label_create(1,-1,-1,21,info,gray,center);
   mscroll_create(1,23,250,-22,&dptr,&dmax,dlist,0,cd,menud,1);
   mscroll_create(251,23,-1,-22,&fptr,&fmax,flist,0,dclick,menuf,1);
}
