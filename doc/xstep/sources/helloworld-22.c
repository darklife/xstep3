
      #include <xstep.h>
      #include <sys/time.h>
      #include <unistd.h>
      #include <math.h>

      #include <X11/extensions/Xdbe.h>

      XdbeSwapInfo    dbinfo;
      XdbeBackBuffer  dbbuffer;
      GC              dbgc;

      #define VECTORS (18*4)

      #ifndef PI
              #define PI      (355.0/113.0)
      #endif

      XPoint  object[3][5][VECTORS];


      struct timeval tv,tv1,tv0;
      struct timezone tz;

      int     frames,framec,
              xdir=4,ydir=4,
              tic,
              softdbf,harddbf,
              frametime=1000000/30;

      char    fps[64],
              framedef[64],
              bufferdef[64],
              status[128];

      int red,green,blue,color;

      void draw(struct xtree *t) {

              int f,ticd;

              gettimeofday(&tv1,&tz);

              ticd=tv1.tv_usec/frametime;
              if(tic==ticd) return;
              tic=ticd;

              frames++;
              framec++;
              
              if(frames%32>=16) 
                      color=15-frames%16;
              else
                      color=frames%16;
              
              f=frames%5;
              memcpy(&tv0,&tv1,sizeof(tv));

              if(!tic) { 
              
                      sprintf(status,"%s: %dx%d (%ld fps)",
                              harddbf?"DoubleBuffer":"WindowBuffer",
                              t->aw,t->ah,
                              tv1.tv_sec-tv.tv_sec?framec/(tv1.tv_sec-tv.tv_sec):0);
              }

              if(harddbf) {

                      XSetForeground(display,dbgc,red);
                      XFillPolygon(display,dbbuffer,dbgc,
                              object[0][frames%5],VECTORS,Nonconvex,CoordModeOrigin);
                      XSetForeground(display,dbgc,green);
                      XFillPolygon(display,dbbuffer,dbgc,
                              object[1][4-(frames-1)%5],VECTORS,Nonconvex,CoordModeOrigin);
                      XSetForeground(display,dbgc,blue);
                      XFillPolygon(display,dbbuffer,dbgc,
                              object[2][frames%5],VECTORS,Nonconvex,CoordModeOrigin);

                      XSetForeground(display,dbgc,black);
                      XDrawString(display,dbbuffer,dbgc,
                              4,24,status,strlen(status));
                      
                      XdbeSwapBuffers(display,&dbinfo,1);
                      return;
              }

              XClearWindow(display,t->win);

              XSetForeground(display,t->gc,red);
              XFillPolygon(display,t->win,t->gc,
                      object[0][frames%5],VECTORS,Nonconvex,CoordModeOrigin);
              XSetForeground(display,t->gc,green);
              XFillPolygon(display,t->win,t->gc,
                      object[1][4-(frames-1)%5],VECTORS,Nonconvex,CoordModeOrigin);
              XSetForeground(display,t->gc,blue);
              XFillPolygon(display,t->win,t->gc,
                      object[2][frames%5],VECTORS,Nonconvex,CoordModeOrigin);
              XSetForeground(display,t->gc,black);
              XDrawString(display,t->win,t->gc,
                      4,24,status,strlen(status));
              XFlush(display);
      }

      void control_apply(struct xtree *t) {

              frametime=1000000/atoi(framedef);

              harddbf=0;
              if(!strcmp(bufferdef,"Double Buffered")) harddbf=1;
              
              gettimeofday(&tv,&tz);
              framec=0;
      }

      void control_ok(struct xtree *t) {

              control_apply(t);
              window_close(t);
      }

      void control(struct xtree *t) {

              dialogbox_create(400,180);

              strcpy(bufferdef,"Single Buffered");
              if(harddbf) strcpy(bufferdef,"Double Buffered");

              switch(atoi(framedef)) {
                      case 15: strcpy(framedef,"15 frames/s"); break;
                      case 30:
                      default: strcpy(framedef,"30 frames/s"); break;
                      case 25: strcpy(framedef,"25 frames/s"); break;
              }

              label_create(0,0,0,90,"",gray,up);

              radio_create(8,8+0*24,200-8,21,"15 frames/s",framedef);
              radio_create(8,8+1*24,200-8,21,"30 frames/s",framedef);
              radio_create(8,8+2*24,200-8,21,"60 frames/s",framedef);

              label_create(0,-90,0,0,"",gray,up);

              check_create(8,-8,200,21,"Double Buffered",bufferdef);

              button_create(-168,-8,72,24,"Ok",control_ok);
              button_create(-88,-8,72,24,"Cancel",window_close);
              button_create(-8,-8,72,24,"Apply",control_apply);
      }

      void xmain(int n,char *p[]) {

              int i,j,f,g;
              struct xtree *w;

              animate=1;
              for(g=0;g!=3;g++)
              for(j=0;j!=18;j++) 
              for(f=0;f!=5;f++) {

                      i=j*20+f*4;
                      object[g][f][j*4+0].x=(int)(310*g+210+cos(i*PI/180)*140.0); 
                      object[g][f][j*4+0].y=(int)(210+sin(i*PI/180)*140.0);
                      i+=10;
                      object[g][f][j*4+1].x=(int)(310*g+210+cos(i*PI/180)*140.0); 
                      object[g][f][j*4+1].y=(int)(210+sin(i*PI/180)*140.0);
                      i+=2;
                      object[g][f][j*4+2].x=(int)(310*g+210+cos(i*PI/180)*170.0); 
                      object[g][f][j*4+2].y=(int)(210+sin(i*PI/180)*170.0);
                      i+=6;
                      object[g][f][j*4+3].x=(int)(310*g+210+cos(i*PI/180)*170.0); 
                      object[g][f][j*4+3].y=(int)(210+sin(i*PI/180)*170.0);
              }

              red=getnamedcolor("DarkRed");
              green=getnamedcolor("DarkGreen");
              blue=getnamedcolor("DarkBlue");

              w=window_create(16,16,512,384,"XStep2D Animation");

              XSetFont(display,w->gc,helvetica24b->fid);

              w->broadcast=draw;

              dbbuffer=XdbeAllocateBackBufferName(display,w->win,XdbeBackground);
              dbgc=XCreateGC(display,dbbuffer,0,&values);
              XSetFont(display,dbgc,helvetica24b->fid);       

              dbinfo.swap_window=treestk->win;
              dbinfo.swap_action=XdbeBackground;

              gettimeofday(&tv,&tz);
              gettimeofday(&tv0,&tz);

              button_create(-88,-8,72,24,"Control",control);
              button_create(-8,-8,72,24,"Quit",window_close);
      }

