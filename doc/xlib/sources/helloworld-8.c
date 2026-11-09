      #include        <X11/Xlib.h>
      #include        <stdio.h>
      #include        <stdlib.h>
      #include        <unistd.h>
       
      Display         *display;               // descritor para o servidor X remoto
      Window          window;                 // descritor para janela
      GC              gc;                     // descritor para contexto grafico
      XEvent          event;                  // descritor para eventos recebidos
      XFontStruct     *font;                  // descritor para uma fonte
      int             screen,                 // descritor para um screen no servidor X
                      white,                  // descritor para a cor branca
                      black;                  // descritor para a cor preta
      XGCValues       gcvalues;               // configuracoes do contexto grafico

      #define DISPLAY "localhost:0.0"         // nome do servidor X remoto
      #define FONT    "*helvetica*24*"        // nome da fonte a ser utilizada

      int button(int x,int y,int w,int h,char *name) {

              switch(event.type) {

                      case Expose:
                      case ButtonRelease:
                      
                              XSetForeground(display,gc,white);
                              XFillRectangle(display,window,gc,x,y,w,h);
                              XSetForeground(display,gc,black);
                              XDrawRectangle(display,window,gc,x,y,w,h);
                              XDrawString(display,window,gc,
                                      x+w/2-XTextWidth(font,name,strlen(name))/2, 
                                      y+h/2+10,
                                      name,
                                      strlen(name));

                              XFlush(display);
                              break;

                      case ButtonPress:
                              
                              if((event.xbutton.x>=x&&event.xbutton.x<=(x+w-1))&&
                                 (event.xbutton.y>=y&&event.xbutton.y<=(y+h-1))) {
                                 
                                      XSetForeground(display,gc,black);
                                      XFillRectangle(display,window,gc,x,y,w,h);
                                      XSetForeground(display,gc,white);
                                      XDrawRectangle(display,window,gc,x,y,w,h);
                                      XDrawString(display,window,gc,
                                              x+w/2-XTextWidth(font,name,strlen(name))/2,
                                              y+h/2+10,
                                              name,
                                              strlen(name));
              
                                      XFlush(display);
                                      return(1);
                              }

              }
              return(0);
      }
       
      int main(int i,char **p) {

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
              gc=XCreateGC(display,window,0,&gcvalues);

              XSelectInput(display,window,ExposureMask|KeyPressMask|ButtonPressMask|ButtonReleaseMask);
              XSetFont(display,gc,font->fid);

              XMapWindow(display,window);

              for(;;) {

                      XNextEvent(display,&event);

                      button(64,64,200,100,"hello world!");
                      if(button(280,64,200,100,"quit")) return(0);
              }
      }
