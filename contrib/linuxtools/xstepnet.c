// to compile:
// gcc -Wall -O3 grafico.c -o grafico -I/usr/local/include -I/usr/X11R6/include -L/usr/local/lib -lxstep -L/usr/X11R6/lib -lX11 -lm

#include <xstep.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define PROC_PATH      "/proc/net/dev"	/* path do arquivo dev  (2.2.x) */
#define MAXLEN         255	/* tamanho maximo de cada linha */
#define INTERFACE      "eth0"	/* interface a monitorar        */

/* compatibilidade : */

/* kernel 2.2.x	     */


#define PROC_RX	1

// p/ kernel 2.2 eh 9 esse ae embaixo!
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


#define XSCALE 50
#define YSCALE 20

int data[2][XSCALE],check,scale=YSCALE;

char lbl_rx[256], lbl_tx[256], lbl_peak_rx [256], lbl_peak_tx[256] ; /* texto nos labels */

// modifications to work w/ any interface ;)

char *interface=INTERFACE;

unsigned long  get_stat (int );

//void find_line(char *, char *);
//void parse_buf (char *, char **);

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
//	XDrawString(display,t->win,t->gc,15,13,"RX",2);

	if((rand()%scale)<data[0][XSCALE-1])

	  	XSetForeground (display, t->gc, getnamedcolor ("green"));
	else
		XSetForeground (display, t->gc, black);

	XFillArc(display,t->win,t->gc,5,15,6,6,0,360*64);
//	XDrawString(display,t->win,t->gc,15,25,"TX",2);

  	if (check == time (0)) return;
  	check = time (0);

  	// recalculo dos valores manehs.
	
  	for (i = 0; i != XSCALE - 1; i++) {

      		data[0][i] = data[0][i + 1];
      		data[1][i] = data[1][i + 1];
    	}

 	find_line(buffer, interface);
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

	
	if (rx > peak_rx) peak_rx=rx;
	if (tx > peak_tx) peak_tx=tx;
	
	
	/* imprime apenas os bytes enviados e recebidos p/ periodo */
	/* um calculo de meida seria legal tb.			   */
 
 	
 	
 //	fprintf (stderr,"Variacao: rx: %d tx: %d (escala=%d) peak_rx: %d  peak_tx: %d\n", (long) rx, (long) tx,scale, peak_rx, peak_tx);
 	
 	pre_rx=temp_rx;
 	pre_tx=temp_tx; 	
 	
  	sprintf (lbl_rx, "RX: %.03f kbps",8*rx/1024.0);
  	sprintf (lbl_tx, "TX: %.03f kbps",8*tx/1024.0);

	sprintf (lbl_peak_rx,"Rx peak: %d", peak_rx);
	sprintf (lbl_peak_tx,"Tx peak: %d", peak_tx);
		
  /*
  	for (du=0;du< tx;du++) {
  	
  	if (tx==0) {
  		XSetForeground (display, t->gc, getnamedcolor ("black"));
  		XFillArc(display,t->win,t->gc,5,5,6,6,0,360*64);
  		break;
  		}
  		
  	XSetForeground (display, t->gc, getnamedcolor ("green"));
  	XFillArc(display,t->win,t->gc,5,5,6,6,0,360*64);	  	
  	
	XSetForeground (display, t->gc, getnamedcolor ("black"));
	XFillArc(display,t->win,t->gc,5,5,6,6,0,360*64); 	
  	
       }
  
  */	
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
/*  	
	XFillArc(display,t->win,t->gc,5,5,6,6,0,360*64);
*/
	XDrawString(display,t->win,t->gc,15,13,lbl_rx,strlen(lbl_rx));
  	
  	for (i = 0; i != XSCALE - 1; i++)
    		XDrawLine (display, t->win, t->gc,
	       	1+(i * t->aw) / XSCALE,
	       	t->ah-(data[1][i] * t->ah) / scale-1,
	       	1+((i + 1) * t->aw) / XSCALE,
	       	t->ah-(data[1][i + 1] * t->ah) / scale)-1;

  	XSetForeground (display, t->gc, getnamedcolor ("green"));
/*
	XFillArc(display,t->win,t->gc,5,15,6,6,0,360*64);
*/
	XDrawString(display,t->win,t->gc,15,25,lbl_tx,strlen(lbl_rx));

	
  	for (i = 0; i != XSCALE - 1; i++) 
  		XDrawLine (display, t->win, t->gc,
	       	(i * t->aw) / XSCALE,
	       	t->ah-(data[0][i] * t->ah) / scale-1,
	       	((i + 1) * t->aw) / XSCALE,
	       	t->ah-(data[0][i + 1] * t->ah) / scale-1);



}

void  xmain (int n, char *p[]) {

  	int i;

	for(i=0;i!=n;i++) {
	
		if(!strcmp(p[i],"-i")&&i+1<n)
			interface=p[i+1];
	}

  	// values for data[?][i] must be [0,YSCALE-1]!

  	for (i = 0; i != XSCALE; i++) {
	
    		data[0][i] = 0;
	      	data[1][i] = 0;
    	
    	}
  	window_create (0, 0, 200, 200,interface);
	box_create(0,0,0,0,draw,black);

//  	box_create (8, 8, -8, -40, draw, black);
//  	button_create (-8, -8, 72, 24, "close", window_close);

//	label_create (8,-8,100, 24, lbl_rx, black, up); 
//	label_create (100,-8,100, 24, lbl_tx, black, up);
	
//	label_create (200,-8,100, 24, lbl_peak_rx, black, up); 
//	label_create (300,-8,100, 24, lbl_peak_tx, black, up);
	
  	animate = 1;
	
	}
