#include <stdio.h>

#define PI 3.14159265358979323846

#define YSTEP 18
#define XSTEP 36
#define SCALE 10

xmain() {

	float x,y,z;
	int a,b,v;

	printf("circle\n%d %d\n",
		(YSTEP+1)*(XSTEP+1),(YSTEP-2)*XSTEP+2);

	for(v=a=0;a!=YSTEP+1;a++) {
	
		y=cos(a*SCALE*PI/180.0);
	
		for(b=0;b!=XSTEP+1;b++,v++) {
	
			x=sin(SCALE*b*PI/180.0)*sin(a*SCALE*PI/180.0);
			z=cos(SCALE*b*PI/180.0)*sin(a*SCALE*PI/180.0);

			printf("%d %f %f %f\n",v+1,x,y,z);
		}
	}

	printf("gray %d",XSTEP+1);

	for(b=0;b!=XSTEP+1;b++) printf(" %d",(XSTEP+1)+XSTEP-b+1);

	putchar('\n');

        for(a=1;a!=YSTEP-1;a++) {

		for(b=0;b!=XSTEP;b++) {
                                
			printf("gray 4 %d %d %d %d\n",
				(a)*(XSTEP+1)+(b)+1,
				(a)*(XSTEP+1)+(b+1)+1,
				(a+1)*(XSTEP+1)+(b+1)+1,
				(a+1)*(XSTEP+1)+(b)+1);
		}
	}

	printf("gray %d",XSTEP+1);

	for(b=0;b!=XSTEP+1;b++) printf(" %d",YSTEP*(XSTEP+1)+b-XSTEP);

	putchar('\n');
}
