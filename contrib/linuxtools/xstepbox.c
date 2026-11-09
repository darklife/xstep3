// to compile:
// gcc -Wall -O3 grafico.c -o grafico -I/usr/local/include -I/usr/X11R6/include -L/usr/local/lib -lxstep -L/usr/X11R6/lib -lX11 -lm

#include <xstep.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include "construction.xpm"
#include "xterm.xpm"



#define PROC_PATH      "/proc/net/dev"	/* path do arquivo dev  (2.2.x) */
#define MAXLEN         255	/* tamanho maximo de cada linha */
#define INTERFACE      "eth0"	/* interface a monitorar        */


/* compatibilidade : */

/* kernel 2.2.x	     */


#define PROC_RX	1
#define PROC_TX	9

/* kernel 2.0.x		*/
/* obs ele fornece apenas o numero de pacotes, tem q haver a conversao automatica */
/* e o define rpa ele q eu nao sei como detectar */

// #define PROC_RX 1
// #define PROC_TX	6

#define DEBUGO 		fprintf (stderr,"+");

/* parametros pro getstat */

#define RX	0
#define TX	1


#define XSCALE 100
#define YSCALE 100

int data[2][XSCALE],check,scale=YSCALE;

char lbl_rx[256], lbl_tx[256], lbl_peak_rx [256], lbl_peak_tx[256] ; /* texto nos labels */

struct xtree *win[3];

unsigned long  get_stat (int );

char buffer[256];

void systemf(struct xtree *t) {

	signal (SIGPIPE, SIG_IGN);	        	
        
        if(!fork()) {
        
        	system(buffer);
        	exit(0);
	}
        buffer[0]=0;
        broadcast++;
        }

void configf(struct xtree *t) {
        window_create(0,0,400,300,"toolbox config");
	
	button_create(-8,-8,72,24,"OK",window_close);

        }
                        

void quickf(struct xtree *t) {

        // casca! executa o label do botao!

	signal (SIGPIPE, SIG_IGN);	        	
        
        if(!fork()) {
        
        	system(t->t);
	        exit(0);
	}
}
        

int find_line(char *buffer, char *interface) { 	/* abre o arquivo, acha a linha , copia e fecha arquivo */
	
	FILE *fp;

	
	if ((fp=fopen(PROC_PATH, "r"))==NULL) return (-1); /* erro ao abrir arquivo */
	
	/* pula 2 linhas ( header ) */
	fgets (buffer, MAXLEN, fp);
	fgets (buffer, MAXLEN, fp);
	
	while (!feof(fp)) {
		fgets (buffer, MAXLEN, fp);
		if (strstr (buffer, interface)) {
			fclose (fp);
			break;
			}
		}
	}

	
 void parse_buf (char *buf, char **args) {   /* divide a linha no estilo de argv[] */
 	
 	
 	while ( *buf != 0) {
  		  		
  		while ((*buf == ' ') || (*buf == ':')) *buf++ = 0;
 		*args++=buf;
 		while ((*buf != 0) && (*buf != ' ') && (*buf !=':')) buf++;
 		
 		}
 	*args=NULL;
}

void draw (struct xtree *t) {

	int i,max;
 	char *args[100];
 	char buffer[MAXLEN+10000];
 	static unsigned long pre_rx=0, pre_tx=0, temp_rx, temp_tx, peak_rx=0, peak_tx=0;
 	unsigned long rx,tx, du;
 	static int flag=0; 

	if((rand()%scale)<data[1][XSCALE-1])

	  	XSetForeground (display, t->gc, getnamedcolor ("red"));
	else
		XSetForeground (display, t->gc, black);
  	
	XFillArc(display,t->win,t->gc,5,5,6,6,0,360*64);

	if((rand()%scale)<data[0][XSCALE-1])

	  	XSetForeground (display, t->gc, getnamedcolor ("green"));
	else
		XSetForeground (display, t->gc, black);

	XFillArc(display,t->win,t->gc,5,15,6,6,0,360*64);

  	if (check == time (0)) return;
  	check = time (0);

  	// recalculo dos valores manehs.

	
  	for (i = 0; i != XSCALE - 1; i++) {

      		data[0][i] = data[0][i + 1];
      		data[1][i] = data[1][i + 1];
    	}

 	find_line(buffer, INTERFACE);
 	parse_buf (buffer, args);
 
 	/* note a reentrancia. */
 	
 	if (!flag) {

 		pre_rx=(unsigned)atof(args[PROC_RX]);
 		pre_tx=(unsigned)atof(args[PROC_TX]);
 		flag=1;
 	}
 		
 	temp_rx=(unsigned)atof(args[PROC_RX]);
 	temp_tx=(unsigned)atof(args[PROC_TX]);

 	rx=((unsigned)atof(args[PROC_RX])) - pre_rx;
 	tx=((unsigned)atof(args[PROC_TX])) - pre_tx;

	/* imprime apenas os bytes enviados e recebidos p/ periodo */
	/* um calculo de meida seria legal tb.			   */
 	
 	// fprintf (stderr,"Variacao: rx: %d tx: %d (escala=%d) peak_rx: %d  peak_tx: %d\n", (long) rx, (long) tx,scale, peak_rx, peak_tx);
 	
 	pre_rx=temp_rx;
 	pre_tx=temp_tx; 	
 	
  	sprintf (lbl_rx, "Rx: %d", rx);
  	sprintf (lbl_tx, "Tx: %d", tx);

	sprintf (lbl_peak_rx,"Rx peak: %d", peak_rx);
	sprintf (lbl_peak_tx,"Tx peak: %d", peak_tx);

  	data[0][XSCALE - 1] = tx;
  	data[1][XSCALE - 1] = rx;

	// autoscaling! :)

	for(max=i=0;i!=XSCALE-1;i++) {
	
		if(max<data[0][i]) max=data[0][i];
		if(max<data[1][i]) max=data[1][i];
	}
	
	if(max>scale) while(max>scale) scale*=2;
	if(max<scale/2) while(max<scale/2) scale/=2;

	if(scale<YSCALE) scale=YSCALE;
		
  	XClearWindow (display, t->win);

  	XSetForeground (display, t->gc, getnamedcolor ("red"));

	XDrawString(display,t->win,t->gc,15,13,"RX",2);
  	
  	for (i = 0; i != XSCALE - 1; i++)
    		XDrawLine (display, t->win, t->gc,
	       	1+(i * t->aw) / XSCALE,
	       	t->ah-(data[1][i] * t->ah) / scale-1,
	       	1+((i + 1) * t->aw) / XSCALE,
	       	t->ah-(data[1][i + 1] * t->ah) / scale)-1;


        XSetForeground (display, t->gc, getnamedcolor ("green"));
        

	XDrawString(display,t->win,t->gc,15,25,"TX",2);

	
  	for (i = 0; i != XSCALE - 1; i++) 
  		XDrawLine (display, t->win, t->gc,
	       	(i * t->aw) / XSCALE,
	       	t->ah-(data[0][i] * t->ah) / scale-1,
	       	((i + 1) * t->aw) / XSCALE,
	       	t->ah-(data[0][i + 1] * t->ah) / scale-1);

}

void hidef(struct xtree *);

void showf(struct xtree *t) {

	int i;

	XMoveResizeWindow(display,win[0]->win,16+64*4+20+4,16+24,64,64);
	XMoveResizeWindow(display,win[1]->win,16,16,64*3,21);
	XMoveResizeWindow(display,win[2]->win,16,16+24,64*4+20,64);

	t->f=hidef;
}

void hidef(struct xtree *t) {

	int i;

	XMoveResizeWindow(display,win[0]->win,16,16+24,20,64);
	XMoveResizeWindow(display,win[1]->win,16,16+24,20,64);
	XMoveResizeWindow(display,win[2]->win,16,16+24,20,64);
	t->f=showf;
}

void exitf(struct xtree *t) {

	exit(0);
}

void  xmain (int n, char *p[]) {

  	int i;
  	struct xtree *t;

  	// values for data[?][i] must be [0,YSCALE-1]!

  	for (i = 0; i != XSCALE; i++) {
	
    		data[0][i] = 0;
	      	data[1][i] = 0;
    	
    	}

	win[0]=toolbox_create(16+64*4+20+4,16+24,64,64);
  	box_create (0, 0, 64, 64,draw,black);
	
	win[1]=toolbox_create(16,16,(73*3) + 20,21);
	edit_create(0,0,72*3,21,buffer,256,systemf);
	label_create(-1,0,25, 21, "Run", black, up);

	win[2]=t=toolbox_create(16,16+24,64*4+20,64);
//	mkpixmap(mozilla,t);
//	XSetWindowBackgroundPixmap(display,t->win,t->pix);
//	XClearWindow(display,t->win);

	button_create(0,0,20,64,"<>",hidef);
	mkpixmap(construction_xpm,button_create(21,0,64,64,"config",configf));  
        mkpixmap(xterm_xpm,button_create(21+64,0,64,64,"xterm",quickf));  
        mkpixmap(xterm_xpm,button_create(21+(2*64),0,64,64,"xterm",quickf));
	mkpixmap(construction_xpm,button_create(21+(3*64),0,64,64,"close",exitf));
	
  	animate = 1;
	
	}
