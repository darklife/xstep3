

      #include <xstep.h>
              
      int     x,y,ix,iy,z;

      char    *message="hello world",
              colorname[64]="White",
              turbo[64]="1x";

      char    *colors[4096],*codes[4096];
                                
      struct mlist multi[]={
                   
              { "Name",       colors, 100 },   
              { "Code",       codes,  100 },
      };

      int     colorptr,colormax;

      void drawf(struct xtree *t) {

              int c;

              c=atoi(turbo);

              XSetForeground(display,t->gc,black);
              XDrawString(display,t->win,t->gc,x,y,message,strlen(message));
              
              if((x+z)>=t->aw)        ix=-1;
              if(x<=0)                ix= 1;
              if(y>=t->ah)            iy=-1;
              if((y-20)<=0)           iy= 1;

              x+=(ix*c); y+=(iy*c);

              XSetForeground(display,t->gc,getnamedcolor(colorname));
              XDrawString(display,t->win,t->gc,x,y,message,strlen(message));  
              XFlush(display);
      }

      void showcolorf(struct xtree *t) {

              if(colormax) downbox(0,0,t->aw,t->ah,getnamedcolor(colors[colorptr]),t);
      }

      void applyf(struct xtree *t) {

              if(colormax) strcpy(colorname,colors[colorptr]);
      }

      void okf(struct xtree *t) {

              if(colormax) strcpy(colorname,colors[colorptr]);
              window_close(t);
      }


      char colorbuffer[64];

      void insertf(struct xtree *t) {

              if(!*colorbuffer) return;

              colors[colormax]=(char *)malloc(1+strlen(colorbuffer));
              strcpy(colors[colormax],colorbuffer);
              sprintf(colorbuffer,"%d",getnamedcolor(colorbuffer));
              codes[colormax]=(char *)malloc(1+strlen(colorbuffer));
              strcpy(codes[colormax],colorbuffer);
              colormax++;
              
              window_close(t);
      }

      void decodemenuf(struct xtree *t) {

              switch(t->l) {
              
                      case 0: 
                              dialog_create(400,180);
                              
                              defaultfont=helvetica24b;
                              label_create(0,0,0,90,"Insert New Color",gray,up);
                              defaultfont=helvetica12m;
                              label_create(0,90,0,0,"",gray,up);

                              edit_create (8,98,200,21,colorbuffer,64,insertf);

                              button_create(-8,-8,72,24,"Insert",insertf);
                              button_create(-88,-8,72,24,"Cancel",window_close);
                              break;
                      case 1:
                              free(colors[colorptr]);
                              free(codes[colorptr]);
                              colormax--;
                              colors[colorptr]=colors[colormax];
                              codes[colorptr] =codes[colormax];
                              broadcast++;
                              break;
                      case 2:
                              applyf(t);
                              break;
                      case 3:
                              window_close(t);
                              break;
              }
      }

      void menuf(struct xtree *t) {

              static char *options[]={

                      "Insert"        ,
                      "Delete"        ,       
                      "Apply"         ,
                      "Cancel"        ,
              };

              menu_create(0,0,120,4*21,options,0,decodemenuf);
      }

      void colorsf(struct xtree *t) {

              dialog_create(400,180);
              box_create(-8,8,132,132,showcolorf,black);
              mscroll_create(8,8,-148,-40,&colorptr,&colormax,multi,0,applyf,menuf,2);
              button_create(-168,-8,72,24,"Cancel",window_close);
              button_create(-88,-8,72,24,"OK",okf);
              button_create(-8,-8,72,24,"Apply",applyf);
      }

      void xmain(int i,char **p) {

              FILE *f;
              char buffer[256],*q;
              struct xtree *w;

              f=fopen("/usr/X11R6/lib/X11/rgb.txt","r");
              
              while(fgets(buffer,256,f)) {

                      if(buffer[0]=='!') continue;
              
                      buffer[strlen(buffer)-1]=0;
                      q=buffer;

                      while(*q<33) q++;              
                      while(*q>32) q++;
                      while(*q<33) q++;
                      while(*q>32) q++;
                      while(*q<33) q++;
                      while(*q>32) q++;
                      while(*q<33) q++;

                      colors[colormax]=(char *)malloc(1+strlen(q));
                      strcpy(colors[colormax],q);

                      sprintf(buffer,"%d",getnamedcolor(q));
                      codes[colormax]=(char *)malloc(1+strlen(buffer));
                      strcpy(codes[colormax],buffer);

                      colormax++;
              }       

              fclose(f);

              window_create(0,0,640,480,p[0]);

              radio_create((8+50*0),-8,50,21,"1x",turbo);
              radio_create((8+50*1),-8,50,21,"2x",turbo);
              radio_create((8+50*2),-8,50,21,"4x",turbo);
              radio_create((8+50*3),-8,50,21,"8x",turbo);
              radio_create((8+50*4),-8,50,21,"16x",turbo);
              radio_create((8+50*5),-8,50,21,"32x",turbo);

              button_create(-8,-8,72,24,"Quit",window_close);
              button_create(-88,-8,72,24,"Colors",colorsf);

              defaultfont=helvetica24b;
              w=box_create(8,8,-8,-40,drawf,black);
              w->expose=0;
              z=XTextWidth(defaultfont,message,strlen(message));
              x=64,y=64,ix=1,iy=1; 

              defaultfont=helvetica12m;
              animate=1;
      }

