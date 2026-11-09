
      #include <xstep.h>

      int     x,y,ix,iy,z;

      char    *message="hello world",
              colorbuffer[64],
              colorname[64]="Blue",
              turbo[64];

      char    *colors[]={
              "Cyan",
              "Yellow",
              "Magenta",
              "Blue",
              "Red",
              "Green" 
      };

      void drawf(struct xtree *t) {

              int c;

              if(*turbo) c=10; else c=1;

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

      void updatef(struct xtree *t) {

              if(*colorbuffer)
                      strcpy(colorname,colorbuffer);
              else
                      strcpy(colorname,"White");
      }

      void optionsf(struct xtree *t) {

              dialog_create(400,180);

              defaultfont=helvetica24b;
              label_create(0,0,0,90,"Hello World Options!",gray,up);
              defaultfont=helvetica12m;

              label_create(0,90,0,0,"",gray,up);

              popup_create(8,98,200,21,colorname,colors,6*21);        
              edit_create (8,98+24,200,21,colorbuffer,64,updatef);

              check_create(8,-8,100,21,"Accelerated!",        turbo);
              button_create(-8,-8,72,24,"OK",window_close);   
      }

      void xmain(int i,char **p) {

              window_create(0,0,640,480,p[0]);

              button_create(-8,-8,72,24,"Quit",window_close);
              button_create(-88,-8,72,24,"Options",optionsf);

              defaultfont=helvetica24b;
              (box_create(8,8,-8,-40,drawf,black))->expose=0;
              z=XTextWidth(defaultfont,message,strlen(message));
              x=64,y=64,ix=1,iy=1; 

              defaultfont=helvetica12m;
              animate=1;
      }
