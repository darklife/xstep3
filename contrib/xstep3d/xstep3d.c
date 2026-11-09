#include <xstep.h>
#include <X11/extensions/Xdbe.h>

#include <stdio.h>
#include <math.h>

#define MAGIC 1.74532925199e-2

XdbeSwapInfo    dbinfo;
XdbeBackBuffer  dbwin;
GC              dbgc;

float lightx=10000,lighty=10000,lightz=-10000;

struct object {

	char name[64];

	int vc,pc;

	struct vector { 
		float 	x,y,z,rx,ry,rz; 
	}  **v;

	struct polygon { 

		int color,idc,id[256],visible;
		float x,y,z,angle;

	} **p;

} obj;

int 	x,y,z,framerate=1000000/30,tic,ticd,frame,poly;
float	scale=10,zhorizon=10,fsin[360],fcos[360];

int shadow[128];

struct timeval 	tv;
struct timezone tz;

char drawtype[64],status[64],scales[16]="1.0";

/*
	this rotation function isn't really the better 
	way to rotate 3D vectors, but is fast and work for my
	purpose. for a more elabored application, a real 3D
	rotation function must be used:
*/

void rotate(int x,int y,int z) {

	int j;
	float xsin,xcos,ysin,ycos,zsin,zcos,ax,ay,bx,bz,cz,cy;

        zsin=fsin[z%360];
        zcos=fcos[z%360];

        ysin=fsin[y%360];
        ycos=fcos[y%360];

        xsin=fsin[x%360];
        xcos=fcos[x%360];

	for(j=0;j!=obj.vc;j++) {

		ax=(obj.v[j]->x*zcos-obj.v[j]->y*zsin);
		ay=(obj.v[j]->y*zcos+obj.v[j]->x*zsin);
	
		bx=(ax*ycos-obj.v[j]->z*ysin);
		bz=(obj.v[j]->z*ycos+ax*ysin);

		cz=(bz*xcos-ay*xsin);
		cy=(ay*xcos+bz*xsin);

		obj.v[j]->rx=bx;
		obj.v[j]->ry=cy;
		obj.v[j]->rz=cz;
	}
}

/*
	the X Window System can't make texturized polygons 
	without a extension like GLX, then i just draw flat polygons
	in the correct order, sorted by a Z-buffer value:
*/

void quicksort(struct polygon **p,int l,int r) {

        int 	i,j;
        float 	vi;

	struct polygon *ptmp;

        if(r>l) {

                vi=p[r]->z; 
                i=l-1; 
                j=r;
                
                while(1) {

                        while(p[++i]->z<vi&&i!=r);
                        while(p[--j]->z>vi&&j!=l);
                        
                        if(i>=j) break;

                        ptmp=p[i]; p[i]=p[j]; p[j]=ptmp;
                }

                ptmp=p[i]; p[i]=p[r]; p[r]=ptmp;
                quicksort(p,l,i-1);
                quicksort(p,i+1,r);
        }
}

/*
	this function calculate and draw a new X frame:
*/

void idle(struct xtree *t) {

	int j,k;
	float zscale,ax,ay,az,dx,dy,dz,m,ml,mn;
	XPoint draw[256];

	struct vector l,n,a,b;

	/*
		this code enable you to control the max frame rate:
	*/

	gettimeofday(&tv,&tz);
	ticd=tv.tv_usec/framerate;

	if(tic==ticd) return;

	/*
		this code rotate all vectors in the object:
	*/

	rotate(x+=3,y+=2,z+=1);
//	rotate(x=360+30,y++,z=0);

	/*
		this sequence will calculate the reflected
		light of all rotated polygons in the 3D space:
	*/

	for(j=0;j!=obj.pc;j++) {

		obj.p[j]->x=0;
		obj.p[j]->y=0;
		obj.p[j]->z=0;

		for(k=0;k!=obj.p[j]->idc;k++) {

			obj.p[j]->x+=obj.v[obj.p[j]->id[k]]->rx;
			obj.p[j]->y+=obj.v[obj.p[j]->id[k]]->ry;
			obj.p[j]->z+=obj.v[obj.p[j]->id[k]]->rz;
		}

		// VC = S[V(n)]/n

		obj.p[j]->x/=obj.p[j]->idc;
		obj.p[j]->y/=obj.p[j]->idc;
		obj.p[j]->z/=obj.p[j]->idc;

		// light vector = VL - V[0]

		l.x=lightx-obj.v[obj.p[j]->id[0]]->rx;
		l.y=lighty-obj.v[obj.p[j]->id[0]]->ry;
		l.z=lightz-obj.v[obj.p[j]->id[0]]->rz;

		// polygon normal vector = AxB
		//       | ux uy uz |
		// AxB = | ax ay az | = (aybz-azby)ux+(azbx-axbz)uy+(axby-aybx)uz
		//       | bx by bz |

		a.x=obj.v[obj.p[j]->id[1]]->rx-obj.v[obj.p[j]->id[0]]->rx;
		a.y=obj.v[obj.p[j]->id[1]]->ry-obj.v[obj.p[j]->id[0]]->ry;
		a.z=obj.v[obj.p[j]->id[1]]->rz-obj.v[obj.p[j]->id[0]]->rz;

		b.x=obj.v[obj.p[j]->id[2]]->rx-obj.v[obj.p[j]->id[0]]->rx;
		b.y=obj.v[obj.p[j]->id[2]]->ry-obj.v[obj.p[j]->id[0]]->ry;
		b.z=obj.v[obj.p[j]->id[2]]->rz-obj.v[obj.p[j]->id[0]]->rz;

		n.x=a.y*b.z-a.z*b.y;
		n.y=a.z*b.x-a.x*b.z;
		n.z=a.x*b.y-a.y*b.x;

		// light angle = L.N/|L||N|

		ml=sqrt(l.x*l.x+l.y*l.y+l.z*l.z);
		mn=sqrt(n.x*n.x+n.y*n.y+n.z*n.z);

		obj.p[j]->color=((l.x*n.x+l.y*n.y+l.z*n.z)/(ml*mn)+1)/2*127;

		// visibility angle = C.N/|C||N|
		// C = [0,0,-1000000], |C|=|cz|, cx=0, cy=0
		
		obj.p[j]->angle=-n.z/mn;
		
		if(obj.p[j]->angle<0) obj.p[j]->visible=False;
		else obj.p[j]->visible=True;

		poly++;
	}

	/*
		polygons must be sorted by the Z-buffer value:
	*/

	quicksort(obj.p,0,obj.pc-1);

	/*
		all polygons are drawed:
	*/

	for(j=0;j!=obj.pc;j++) {



//		printf("p=%d, ",j);

		for(k=0;k!=obj.p[j]->idc;k++) {

//			obj.v[obj.p[j]->id[k]]->x
//			obj.v[obj.p[j]->id[k]]->y
//			obj.v[obj.p[j]->id[k]]->z

			if(obj.v[obj.p[j]->id[k]]->rz>zhorizon) 
				zhorizon=obj.v[obj.p[j]->id[k]]->rz;

			zscale=scale*(1+0.5*obj.v[obj.p[j]->id[k]]->rz/zhorizon);

			draw[k].x=obj.v[obj.p[j]->id[k]]->rx*zscale+t->aw/2;
			draw[k].y=obj.v[obj.p[j]->id[k]]->ry*zscale+t->ah/2;

//			printf("%d ",1+obj.p[j]->id[k]);
		}
//		putchar('\n');

		/*
			only visible polygons are drawed: in this code,
			any polygons have only one side (the other side ins't
			visible), this will minimize the client/server X
			communication:
		*/


		if(*drawtype) {

			if(!obj.p[j]->visible) continue;
		
			XSetForeground(display,dbgc,shadow[obj.p[j]->color]);
			XFillPolygon(display,dbwin,dbgc,draw,obj.p[j]->idc,Nonconvex,CoordModeOrigin);

		} else {

			for(k=0;k!=obj.p[j]->idc;k++) {

				XSetForeground(display,dbgc,shadow[obj.p[j]->color]);
				XDrawLine(display,dbwin,dbgc,
					draw[k].x,
					draw[k].y,
					draw[k?k-1:k+obj.p[j]->idc-1].x,
					draw[k?k-1:k+obj.p[j]->idc-1].y);
			}
		}
	}

	/*
		frames are drawed in a internal buffer, then moved to
		display. this operation is called 'double buffering' and
		eliminate the drawing flicker:
	*/

	XdbeSwapBuffers(display,&dbinfo,1);
	XFlush(display);

	frame++;

	if(ticd<tic) {
	
		sprintf(status,"%dx%d pixels, %d frames/s, %d polygons/s, scale=%f\n",
			t->aw,
			t->ah,
			frame,
			poly,
			zscale/10);
			
		frame=poly=0;
		
		scale=atof(scales)*10;
	}

	tic=ticd;
}

void quitf(struct xtree *t) {

	exit(0);
}

char anint[64]="Pause";

void aninf(struct xtree *t) {

	if(!strcmp(anint,"Pause")) {
	
		strcpy(anint,"Continue");
		animate=0;

	} else {
	
		strcpy(anint,"Pause");
		animate=1;

	}
	
	broadcast++;
}

void xmain(int i,char **p) {

	int 	j,k,v;
	char 	buffer[4096],*m,*n;
	struct xtree *t;
	FILE *f;

	char static *list[9]={
	
		"0.0625",
		"0.125",
		"0.25",
		"0.5",
		"1",
		"2",
		"4",
		"8",
		"16",
	};

/*
	this code read a ACM object:
*/

	f=fopen(p[i-1],"r");

	if(f==NULL||i==1) exit(-1);
	
	fscanf(f,"%s %d %d",
		obj.name,&obj.vc,&obj.pc);
		
	printf("%s: reading object [%s] w/ %d vectors and %d polygons...",
		p[1],obj.name,obj.vc,obj.pc);
	
	fflush(stdout);
	
	obj.v=(struct vector **)malloc(obj.vc*sizeof(struct vector **));
	
	for(j=0;j!=obj.vc;j++) {
		
		obj.v[j]=(struct vector *)malloc(sizeof(struct vector));
			
		fscanf(f,"%d %f %f %f",
			&k,&obj.v[j]->x,&obj.v[j]->y,&obj.v[j]->z);

//		printf("%d %f %f %f\n",k,obj.v[j]->x,obj.v[j]->y,obj.v[j]->z);
	}	

	obj.p=(struct polygon **)malloc(obj.pc*sizeof(struct polygon **));

	fgets(buffer,4096,f);

	for(j=0;j!=obj.pc;j++) {

		obj.p[j]=(struct polygon *)malloc(sizeof(struct polygon));

		fgets(buffer,4096,f);

		if(*buffer!='(') {

			for(m=buffer;*m>' ';m++);
			*m++='\0';
			n=buffer;
			
		} else {

			for(m=buffer;*m>' ';m++);
			*m++='\0';
			n=buffer+1;
			for(m=buffer;*m!=')';m++);
			*m++='\0';		
		}

		obj.p[j]->color=getnamedcolor(n);
		
		while(*m<=' '&&*m) m++;
		
		v=atoi(m);
		
		while(*m>' ') m++;
		while(*m<=' '&&*m) m++;

		for(k=0;*m;k++) {
			
			obj.p[j]->id[k]=atoi(m)-1;
			while(*m>' ') m++;
			while(*m<=' '&&*m) m++;
		}
		obj.p[j]->idc=k;
		
		if(v!=k) {
		
			printf("error in vector %d: inconsistent object, exiting\n",j+1);
			return;
		}	
//		printf("%d %s %d",j+1,n,k);
//		for(i=0;i!=k;i++) printf(" %d",obj.p[j]->id[i]+1);		
//		printf("\n");
	}

	printf("done.\n\n");

	fclose(f);

	window_create(0,0,800,600,p[1]);

	t=box_create(0,0,0,0,idle,getcolor(0,0,0x4000));

        dbwin=XdbeAllocateBackBufferName(display,t->win,XdbeBackground);
        dbgc=XCreateGC(display,dbwin,0,&values);
        dbinfo.swap_window=t->win;
        dbinfo.swap_action=XdbeBackground;

	window_create(0,0,800,24+8,"Xstep3D ToolBox");

	check_create(4,4,72,24,"filled",drawtype);
	popup_create(8+72,4,72,24,scales,list,9*21);
	edit_create(8+144,4,72,24,scales,16,0);
	label_create(8+216,4,-(144+8),24,status,invisible,center);

	button_create(-76,-4,72,24,anint,aninf);
	button_create(-4,-4,72,24,"Quit",quitf);

	animate=1;

	for(j=0;j!=360;j++) {
	
		fsin[j]=sin(j*MAGIC);
		fcos[j]=cos(j*MAGIC);
	}
	
	for(j=0;j!=128;j++)
		shadow[j]=getcolor(j*512,0,0);
}
