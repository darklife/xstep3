
      #include <xstep.h>

      void xmain(int n,char **p) {

              window_create(0,0,400,180,"resizable window");

              button_create(   8,  8, -8, -8, "center",       0);
              button_create(  16, 16, 80, 24, "top left",     0);
              button_create( -16, 16, 80, 24, "top right",    0);
              button_create(  16,-16, 80, 24, "bottom left",  0);
              button_create( -16,-16, 80, 24, "bottom right", 0);
      }

