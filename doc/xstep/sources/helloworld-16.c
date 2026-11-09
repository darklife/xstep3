
      #include <xstep.h>

      int     x,y,ix,iy,z;
      char    *message="hello world";

      void drawf(struct xtree *t) {

              XSetForeground(display,t->gc,black);
              XDrawString(display,t->win,t->gc,x,y,message,strlen(message));
              
              if((x+z)>=t->aw)        ix=-1;
              if(x<=0)                ix= 1;
              if(y>=t->ah)            iy=-1;
              if((y-20)<=0)           iy= 1;

              x+=ix; y+=iy;

              XSetForeground(display,t->gc,white);
              XDrawString(display,t->win,t->gc,x,y,message,strlen(message));
       
              XFlush(display);
      }

      void startf(struct xtree *t) { animate=1; }
      void stopf (struct xtree *t) { animate=0; }

      void xmain(int i,char **p) {

              window_create(0,0,640,480,p[0]);

              button_create(-8,-8,72,24,"quit",window_close);
              button_create(-88,-8,72,24,"start",startf);
              button_create(-168,-8,72,24,"stop",stopf);  

              defaultfont=helvetica24b;
              box_create(8,8,-8,-40,drawf,black);

              x=64,y=64,ix=1,iy=1;
              z=XTextWidth(defaultfont,message,strlen(message));
      }

