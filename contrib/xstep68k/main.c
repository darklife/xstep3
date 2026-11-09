#include <xstep.h>
#include "68000.h"

char rdata[8][42],raddr[8][42],rpc[42],rssr[18],status[80];

void exec68k(struct xtree *t) {

	int i;

	do {

		ucode[*cpu.pc.i]();
		ucode[*cpu.pc.i]();
		ucode[*cpu.pc.i]();
		ucode[*cpu.pc.i]();
		ucode[*cpu.pc.i]();
		ucode[*cpu.pc.i]();
		ucode[*cpu.pc.i]();
		ucode[*cpu.pc.i]();
		ucode[*cpu.pc.i]();
		ucode[*cpu.pc.i]();
		ucode[*cpu.pc.i]();
		ucode[*cpu.pc.i]();
		ucode[*cpu.pc.i]();
		ucode[*cpu.pc.i]();
		ucode[*cpu.pc.i]();
		ucode[*cpu.pc.i]();
		
	} while((cpu.count+=16)&0xffff);
	
	for(i=0;i!=8;i++) {

		sprintf(rdata[i],"%s %08X",
			tobin(cpu.d[i].l,32),
			cpu.d[i].l);
			
		sprintf(raddr[i],"%s %08X",
			tobin(cpu.a[i].l,32),
			cpu.a[i].l);
	}

	sprintf(rpc,"%s %08X",
		tobin(cpu.pc.d-space.d,32),
		cpu.pc.d-space.d);

	sprintf(rssr,"-------- ---%c%c%c%c%c",
		cpu.status&CCRX?'X':'-',
		cpu.status&CCRN?'N':'-',
		cpu.status&CCRZ?'Z':'-',
		cpu.status&CCRV?'V':'-',
		cpu.status&CCRC?'C':'-');

	sprintf(status, "68000/%dMB (%f MIPS/%f MHz)",
		SPACE/1048576,
		(cpu.count/1000000.0)/((clock()-cpu.start)/CLOCKS_PER_SEC),
		((cpu.clock+cpu.count*4)/1000000.0)/((clock()-cpu.start)/CLOCKS_PER_SEC));
}

void xmain(int n,char **p) {

	char buffer[256],*q,*pcode;
	int i;
	FILE *f;
	static char *sdata[8]={ "D0","D1","D2","D3","D4","D5","D6","D7" };
	static char *saddr[8]={ "A0","A1","A2","A3","A4","A5","A6","A7" };
	struct xtree *win;

	if(n!=2) {
	
		printf("usage: 68000 image.68k\n\n");
		return;
	}

	f=fopen(p[1],"r");
	
	init(SPACE);

	pcode=space.b;

	while(fgets(buffer,256,f)) {
	
		for(i=0;buffer[i];i++) if(buffer[i]=='#') buffer[i]=0;

		q=buffer;
	
		if(*q>32) {
		
			for(i=0,q=buffer;*q>32;q++) {
			
				i*=16;
			
				if(*q<'a') i+=(*q-'0');
				else       i+=(*q-'a'+10);
			}
			pcode=space.b+i;
		}
		for( ;*q<=32&&*q;q++);
		for( ;*q>32;q++) {
		
			if(*q<'a') i=*q-'0';
			else       i=*q-'a'+10;

			q++;
			i*=16;

			if(*q<'a') i+=(*q-'0');
			else       i+=(*q-'a'+10);
			
			*pcode++=(char)i;
		}
	}

	fclose(f);
	
	cpu.a[7].l=xlendian(space.l[0]);
	cpu.pc.b=space.b+xlendian(space.l[1]);
		
	cpu.start=clock();

	win = window_create(0,0,640,480,"68000 simulator");
	win->broadcast=exec68k;

	label_create(8,8,332,21,status,darkgray,center);

	for(i=0;i!=8;i++) {
	
		label_create( 8,32+i*24    ,32 ,21,sdata[i],darkgray,right);
		label_create( 8,32+(8+i)*24,32 ,21,saddr[i],darkgray,right);
		label_create(40,32+i*24    ,300,21,rdata[i],gray,left);
		label_create(40,32+(8+i)*24,300,21,raddr[i],gray,left);
	}

	label_create( 8,32+(8+i)*24,32 ,21,"PC",darkgray,right);
	label_create(40,32+(8+i)*24,300,21,rpc,gray,left);
	i++;

	label_create( 8,32+(8+i)*24,32 ,21,"SSR",darkgray,right);
	label_create(40,32+(8+i)*24,300,21,rssr,gray,left);

	button_create(-8,-8,72,24,"Quit",window_close);

	animate++;
}


