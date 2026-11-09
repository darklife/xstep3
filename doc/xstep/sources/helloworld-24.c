      #include <xstep.h>
      #include <sys/types.h>
      #include <sys/ipc.h>
      #include <sys/shm.h>
      #include <X11/extensions/XShm.h>
                                
      XImage *image;
      XShmSegmentInfo shminfo;

      int     c,d,max,depth,frame,colors[128],ix,iy,zx=2,zy=2,
              *buffer24;
      char    *buffer8;
      short   *buffer16;

      #define X 320
      #define Y 240

      #define K (X*X/4+Y*Y/4)

      int f(int x,int y) {

              x+=ix-X/2;
              y+=iy-Y/2;

              return(abs(ix-iy)*x*x+abs(ix-iy)*y*y);
      }

      void drawf(struct xtree *t) {

              int x,y,ti;
              char buffer[256];

              switch(depth) {

                      case 8:
                              for(x=0;x!=image->width;x++)
                                      for(y=0;y!=image->height;y++)
                                              buffer8[y*image->width+x]=colors[abs((f(x,y)/(K/16))%128)];
                      break;


                      case 16:
                              for(x=0;x!=image->width;x++)
                                      for(y=0;y!=image->height;y++)
                                              buffer16[y*image->width+x]=f(x,y)/(K/16);
                      break;

                      case 24:
                              for(x=0;x!=image->width;x++)
                                      for(y=0;y!=image->height;y++)
                                              buffer24[y*image->width+x]=f(x,y)/(K/128);
                      break;
              }

              c++;

              XShmPutImage(display,
                      t->win,t->gc,image,
                      0,0,0,0,
                      image->width,image->height,0);

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
              
              ix+=zx;
              iy+=zy;

              if(!(rand()%10)) zx=-zx;
              if(!(rand()%10)) zy=-zy;
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

              w=window_create(0,0,X,Y,"XSTEP and XSHM");
              w->broadcast=drawf;

              image=xshmget(w->aw,w->ah);

              buffer8=image->data;
              buffer16=(short *)image->data;
              buffer24=(int *)image->data;

              animate=1;

              for(i=0;i!=64;i++) 
              	     colors[i]=getcolor(i*1024,i*1024,i*1024);
              for(;i!=128;i++)
                     colors[i]=getcolor((63-1024)-i*1024,(63-1024)-i*1024,(63-1024)-i*1024);

              srand(time(0));
      }

