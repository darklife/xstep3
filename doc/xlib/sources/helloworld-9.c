      #include        <X11/Xlib.h>
      #include        <stdio.h>
      #include        <stdlib.h>
      #include        <unistd.h>
       
      Display         *display;               // descritor para o servidor X remoto
      Window          window;                 // descritor para janela
      XEvent          event;                  // descritor para eventos recebidos
      XFontStruct     *font;                  // descritor para uma fonte
      int             screen,                 // descritor para um screen no servidor X
                      white,                  // descritor para a cor branca
                      black;                  // descritor para a cor preta   
      XGCValues       gcvalues;               // configuracoes do contexto grafico

      struct Widget {

              int             w,h;
              char            *name;

              Window          window;         // descritor para janela
              GC              gc;             // descritor para contexto grafico

              void            (*expose)(struct Widget *);
              void            (*buttonpress)(struct Widget *);
              void            (*buttonrelease)(struct Widget *);
              void            (*onclick)(struct Widget *);
       
              struct Widget   *next;

      } *wid;                    

      #define DISPLAY "localhost:0.0"         // nome do servidor X remoto
      #define FONT    "*helvetica*24*"        // nome da fonte a ser utilizada

      void button_expose(struct Widget *t) {

              XSetForeground(display,t->gc,white);
              XFillRectangle(display,t->window,t->gc,0,0,t->w-1,t->h-1);
              XSetForeground(display,t->gc,black);
              XDrawRectangle(display,t->window,t->gc,0,0,t->w-1,t->h-1);
              XDrawString(display,t->window,t->gc,
                      t->w/2-XTextWidth(font,t->name,strlen(t->name))/2,
                      t->h/2+10,
                      t->name,  
                      strlen(t->name));

                      XFlush(display);
      }
       
      void button_buttonpress(struct Widget *t) {
                                 
              XSetForeground(display,t->gc,black);
              XFillRectangle(display,t->window,t->gc,0,0,t->w-1,t->h-1);
              XSetForeground(display,t->gc,white);
              XDrawRectangle(display,t->window,t->gc,0,0,t->w-1,t->h-1);
              XDrawString(display,t->window,t->gc,
                      t->w/2-XTextWidth(font,t->name,strlen(t->name))/2,
                      t->h/2+10,
                      t->name,  
                      strlen(t->name));
              
              XFlush(display);

              if(t->onclick) t->onclick(t);
      }

      void button_create(int x,int y,int w,int h,char *name,void (*onclick)(struct Widget *)) {
       
              struct Widget *aux;

              aux=(struct Widget *)malloc(sizeof(struct Widget));
              aux->window=XCreateSimpleWindow(display,window,x,y,w,h,0,black,white);
              aux->gc=XCreateGC(display,aux->window,0,&gcvalues);

              XSelectInput(display,aux->window,ExposureMask|ButtonPressMask|ButtonReleaseMask);
              XSetFont(display,aux->gc,font->fid);
              XMapWindow(display,aux->window);
              
              aux->w=w;
              aux->h=h;
              aux->name=name;
              aux->onclick=onclick;
              aux->buttonpress=button_buttonpress;
              aux->expose=button_expose;
              aux->buttonrelease=button_expose;   

              aux->next=wid;
              wid=aux;
      }

      void quitf(struct Widget *t) {
       
              exit(0);
      }

      int main(int i,char **p) {

              struct Widget *aux;

              display=XOpenDisplay(DISPLAY);

              if(!display) {
              
                      printf("can't open display %s\n",DISPLAY);
                      return;
              }
               
              screen=DefaultScreen(display);
              white=WhitePixel(display,screen);
              black=BlackPixel(display,screen);
              font=XLoadQueryFont(display,FONT);

              if(!font) {

                      printf("can't open font %s\n",FONT);
                      return;
              }

              window=XCreateSimpleWindow(display,RootWindow(display,screen),16,16,640,480,0,black,white);

              XMapWindow(display,window);

              button_create( 64,64,200,100,"hello world!",0);
              button_create(280,64,200,100,"quit",quitf);   

              for(;;) {

                      XNextEvent(display,&event);
               
                      for(aux=wid;aux;aux=aux->next) {

                              if(event.xany.window==aux->window) {
                      
                                      switch(event.type) {
                         
                                              case Expose:
                                                      aux->expose(aux);
                                                      break;
               
                                              case ButtonPress:
                                                      aux->buttonpress(aux); 
                                                      break;
                                         
                                              case ButtonRelease:
                                                      aux->buttonrelease(aux);
                                                      break;
                                      }
                              }
                      }
              }
      }
       