
      #include <xstep.h>

      void xmain(int i,char **p) {

              window_create(0,0,640,480,p[0]);

              defaultfont=helvetica24b;

              button_create(64,64,200,100,"hello world!",0);
              button_create(280,64,200,100,"quit",window_close);
      }

