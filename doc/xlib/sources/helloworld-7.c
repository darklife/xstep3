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

      int main(int i,char **p) {

              char *message="hello world!";   // mensagem a ser impressa

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
              
                      switch(event.type) {

                              case Expose:
                              
                                      printf("%s: exposed area (%dx%d+%d+%d)\n",
                                              p[0],
                                              event.xexpose.width,
                                              event.xexpose.height,
                                              event.xexpose.x,
                                              event.xexpose.y);
                              
                                      XSetForeground(display,gc,black);
                                      XFillRectangle(display,window,gc,16,16,640-32,480-32);
                                      XSetForeground(display,gc,white);
                                      XDrawString(display,window,gc,   
                                              320-XTextWidth(font,message,strlen(message))/2,
                                              240,
                                              message,
                                              strlen(message));

                                      XFlush(display);
                                      break;

                              case KeyPress:
                              
                                      i=XLookupKeysym((XKeyEvent *)&event,0);
                              
                                      printf("%s: pressed key #%d (%c)\n",
                                              p[0],
                                              i,i>=32&&i<128?i:'*');
                                      break;
                                            
                              case ButtonPress:
                              
                                      printf("%s: pressed mouse button in (%d,%d)\n",
                                              p[0],
                                              event.xbutton.x,
                                              event.xbutton.y);
                                      break;

                              case ButtonRelease:
                              
                                      printf("%s: released mouse button in (%d,%d)\n",
                                              p[0],
                                              event.xbutton.x,
                                              event.xbutton.y);
                                      break;
                      }
              }
      }

