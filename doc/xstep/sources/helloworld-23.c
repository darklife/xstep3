      #include <xstep.h>
      #include <sys/types.h>
      #include <sys/ipc.h>
      #include <sys/shm.h>
      #include <X11/extensions/XShm.h>

      XImage *image;
      XShmSegmentInfo shminfo;

      int     c,d,max,depth,frame,
              *buffer24;
      char    *buffer8;
      short   *buffer16;

      void drawf(struct xtree *t) {

              int x,y,ti;
              char buffer[256];

              switch(depth) {

                      case 8:
                              for(x=0;x!=image->width;x++)
                                      for(y=0;y!=image->height;y++)
                                              buffer8[y*image->width+x]=rand();
                      break;


                      case 16:
                              for(x=0;x!=image->width;x++)
                                      for(y=0;y!=image->height;y++)
                                              buffer16[y*image->width+x]=rand();
                      break;

                      case 24:
                              for(x=0;x!=image->width;x++)
                                      for(y=0;y!=image->height;y++)
                                              buffer24[y*image->width+x]=rand();
                      break;
              }

              c++;

              XShmPutImage(display,
                      t->win,t->gc,image,
                      0,0,0,0,
                      image->width,image->height,False);

              ti=time(0);
              
              if(ti!=d) { max=c; d=ti; c=0; frame++; } 

              sprintf(buffer,"%d.%02d (%d fps)",frame,c,max);

              XSetForeground(display,t->gc,black);
              XDrawString(display,t->win,t->gc,6,t->ah-4,
                      buffer,strlen(buffer));

              XSetForeground(display,t->gc,white);
              XDrawString(display,t->win,t->gc,4,t->ah-6,
                      buffer,strlen(buffer));

              XFlush(display);
      }

      XImage *xshmget(int x,int y) {

              XImage          *image;

              image=XShmCreateImage(display,
                      DefaultVisual(display,screen),
                      depth=DefaultDepth(display,screen),
                      ZPixmap, 
                      NULL, 
                      &shminfo,
                      x,y);

              shminfo.shmid=shmget(IPC_PRIVATE, 
                      image->bytes_per_line*image->height,
                      IPC_CREAT|0777 );

              shminfo.shmaddr=image->data=(char *)shmat(shminfo.shmid,0,0);

              shminfo.readOnly = False;
              XShmAttach(display, &shminfo);

              return(image);
      }

      void xmain(int i,char **p) {

              struct xtree *w;

              defaultfont=XLoadQueryFont(display,"*helvetica*14*");

              w=window_create(0,0,320,240,"XSTEP and XSHM");
              w->broadcast=drawf;

              image=xshmget(w->aw,w->ah);

              buffer8=image->data;
              buffer16=(short *)image->data;
              buffer24=(int *)image->data;

              animate=1;
      }

