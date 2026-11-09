
      #include <xstep.h>

      void xmain(int i,char **p) {

              window_create(0,0,640,480,p[0]);
              label_create(0,0,640,480,"hello world!",invisible,center);
              XSetFont(display,treestk->treestk->gc,(treestk->treestk->font=
                      XLoadQueryFont(display,"-*-garamond-*-*-*-*-*-720-*-*-*-*-*-*"))->fid);
      }

