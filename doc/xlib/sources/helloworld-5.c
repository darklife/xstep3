      #include        <X11/Xlib.h>
      #include        <stdio.h>
      #include        <stdlib.h>
      #include        <unistd.h>

      Display         *display;       // descritor para o servidor X remoto
      Window          window;         // descritor para janela
      GC              gc;             // descritor para contexto grafico
      XFontStruct     *font;          // descritor para uma fonte
      int             screen,         // descritor para um screen no servidor X
                      white,          // descritor para a cor branca
                      black;          // descritor para a cor preta
      XGCValues       gcvalues;       // configuracoes do contexto grafico

      #define DISPLAY "localhost:0.0" // nome do servidor X remoto
      #define FONT    "*helvetica*24*"

      int main() {

              int i;

              char *message="hello world!";

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
              
              XMapWindow(display,window);

              XSetFont(display,gc,font->fid);

              for(;;) {

                      XSetForeground(display,gc,black);

                      XFillRectangle(display,window,gc,16,16,640-32,480-32);
               
                      XSetForeground(display,gc,white);

                      XDrawString(display,window,gc,   
                              320-XTextWidth(font,message,strlen(message))/2,
                              240,
                              message,
                              strlen(message));
               
                      XFlush(display);
                      usleep(1);
              }
      }
