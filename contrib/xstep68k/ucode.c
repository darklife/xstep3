#include "68000.h"

void movebf() {

	byte  data;
	word opcode=xwendian(*cpu.pc.w++);

	pokebyte((opcode>>3)&7,opcode&7,
		data=peekbyte((opcode>>6)&7,(opcode>>9)&7));

	cpu.status=(data?0:CCRZ)|(data&0x80?CCRN:0);
}

void movewf() {

	word  data,ms,md;
	word opcode=xwendian(*cpu.pc.w++);

	ms=(opcode>>3)&7;
	md=(opcode>>6)&7;

	if((ms<2&&md<2)||(ms>1&&md>1)) 

		pokeword(ms,opcode&7,data=peekword(md,(opcode>>9)&7));
	else 
		pokeword(ms,opcode&7,data=xwendian(peekword(md,(opcode>>9)&7)));

	if(ms<2) cpu.status=(data?0:CCRZ)|(data&0x8000?CCRN:0);
	else	 cpu.status=(data?0:CCRZ)|(data&0x80?CCRN:0);
}

void movelf() {

	long  data,ms,md;
	word opcode=xwendian(*cpu.pc.w++);

	ms=(opcode>>3)&7;
	md=(opcode>>6)&7;

	if((ms<2&&md<2)||(ms>1&&md>1)) 

		pokelong(ms,opcode&7,data=peeklong(md,(opcode>>9)&7));
	else 
		pokelong(ms,opcode&7,data=xlendian(peeklong(md,(opcode>>9)&7)));

	if(ms<2) cpu.status=(data?0:CCRZ)|(data&0x80000000?CCRN:0);
	else	 cpu.status=(data?0:CCRZ)|(data&0x80?CCRN:0);
}

void addqbf() {
	
	byte *data;
	int  newdata,olddata,opcode=xwendian(*cpu.pc.w++);

	data=byteaddress((opcode>>3)&7,opcode&7);

	olddata=*data;
	newdata=(*data+=(opcode&0xe00?(opcode>>9)&7:8));

	cpu.status=(newdata?0:CCRZ)|
		   (newdata&0x80?CCRN:0)|
		   ((unsigned)newdata<(unsigned)olddata?(CCRC|CCRX|CCRV):0);
}

void addqwf() {
	
	word *data;
	int  newdata,olddata,opcode=xwendian(*cpu.pc.w++);

	data=wordaddress((opcode>>3)&7,opcode&7);

	if((opcode&0x38)<0x18) {

		olddata=*data;
		newdata=(*data+=(opcode&0xe00?(opcode>>9)&7:8));
	} else {
	
		olddata=xlendian(*data);
		*data=xlendian(newdata=(olddata+(opcode&0xe00?(opcode>>9)&7:8)));
	}
	cpu.status=(newdata?0:CCRZ)|
		   (newdata&0x8000?CCRN:0)|
		   ((unsigned)newdata<(unsigned)olddata?(CCRC|CCRX|CCRV):0);
}

void addqlf() {
	
	long *data;
	int  newdata,olddata,opcode=xwendian(*cpu.pc.w++);

	data=longaddress((opcode>>3)&7,opcode&7);

	if((opcode&0x38)<0x18) {

		olddata=*data;
		newdata=(*data+=(opcode&0xe00?(opcode>>9)&7:8));
	} else {
	
		olddata=xlendian(*data);
		*data=xlendian(newdata=(olddata+(opcode&0xe00?(opcode>>9)&7:8)));
	}
	cpu.status=(newdata?0:CCRZ)|
		   (newdata&0x80000000?CCRN:0)|
		   ((unsigned)newdata<(unsigned)olddata?(CCRC|CCRX|CCRV):0);
}

void subqbf() {
	
	byte *data;
	int  newdata,olddata,opcode=xwendian(*cpu.pc.w++);

	data=byteaddress((opcode>>3)&7,opcode&7);

	olddata=*data;
	newdata=(*data-=(opcode&0xe00?(opcode>>9)&7:8));

	cpu.status=(newdata?0:CCRZ)|
		   (newdata&0x80?CCRN:0)|
		   ((unsigned)newdata>(unsigned)olddata?(CCRC|CCRX|CCRV):0);
}

void subqwf() {
	
	word *data;
	int  newdata,olddata,opcode=xwendian(*cpu.pc.w++);

	data=wordaddress((opcode>>3)&7,opcode&7);

	if((opcode&0x38)<0x18) {

		olddata=*data;
		newdata=(*data-=(opcode&0xe00?(opcode>>9)&7:8));
	} else {
	
		olddata=xlendian(*data);
		*data=xlendian(newdata=(olddata+(opcode&0xe00?(opcode>>9)&7:8)));
	}
	cpu.status=(newdata?0:CCRZ)|
		   (newdata&0x8000?CCRN:0)|
		   ((unsigned)newdata>(unsigned)olddata?(CCRC|CCRX|CCRV):0);
}

void subqlf() {
	
	long *data;
	int  newdata,olddata,opcode=xwendian(*cpu.pc.w++);

	data=longaddress((opcode>>3)&7,opcode&7);

	if((opcode&0x38)<0x18) {

		olddata=*data;
		newdata=(*data-=(opcode&0xe00?(opcode>>9)&7:8));
	} else {
	
		olddata=xlendian(*data);
		*data=xlendian(newdata=(olddata+(opcode&0xe00?(opcode>>9)&7:8)));
	}
	cpu.status=(newdata?0:CCRZ)|
		   (newdata&0x80000000?CCRN:0)|
		   ((unsigned)newdata>(unsigned)olddata?(CCRC|CCRX|CCRV):0);
}

void illegalf() {

	word opcode=xwendian(*cpu.pc.w);

	dump();
	printf("illegal opcode %04x (%s), system halted.\n\n",
		opcode,tobin(opcode,16));
	exit(-1);
}

void stopf() {

	dump();
	printf("system halted.\n\n");
	exit(-1);
}

void bccf() {

	word opcode=xwendian(*cpu.pc.w);

	cpu.clock+=2;

	switch(opcode&0xf00) {
	
		case 0x000: 
			break; // 1			BRA
		case 0x100: 
			break; // 0
		case 0x200: 
			if(!(cpu.status&CCRC)&&!(cpu.status&CCRZ)) break;
			cpu.pc.w++; 
			return; // !C&!Z		BHI
		case 0x300: 
			if((cpu.status&CCRC)||(cpu.status&CCRZ)) break;
			cpu.pc.w++; 
			return; // C|Z			BLS
		case 0x400: 
			if(!(cpu.status&CCRC)) break;		
			cpu.pc.w++; 
			return; // !C			BCC
		case 0x500: 
			if((cpu.status&CCRC)) break;		
			cpu.pc.w++; 
			return; // C			BCS
		case 0x600: 
			if(!(cpu.status&CCRZ)) break;		
			cpu.pc.w++; 
			return; // !Z			BNE
		case 0x700:
			if((cpu.status&CCRZ)) break;		 
			cpu.pc.w++; 
			return; // Z			BEQ
		case 0x800: 
			if((cpu.status&CCRV)) break;
			cpu.pc.w++; 
			return; // V			BVC
		case 0x900: 
			if(!(cpu.status&CCRV)) break;		
			cpu.pc.w++; 
			return; // !V			BVS
		case 0xa00: 
			if(!(cpu.status&CCRN)) break;		
			cpu.pc.w++; 
			return; // !N			BPL
		case 0xb00: 
			if((cpu.status&CCRN)) break;		
			cpu.pc.w++; 
			return; // N			BMI
		case 0xc00: 
			if(((cpu.status&CCRN)&&(cpu.status&CCRV))||
			  (!(cpu.status&CCRN)&&!(cpu.status&CCRV))) break;
			cpu.pc.w++; 
			return; // N&V|!N&!V		BGE
		case 0xd00: 
			if((cpu.status&CCRC)||(cpu.status&CCRZ)) break;
			cpu.pc.w++; 
			return; // C|Z			BLT
		case 0xe00: 
			if(((cpu.status&CCRN)&&(cpu.status&CCRV)&&!(cpu.status&CCRZ))||
			  (!(cpu.status&CCRN)&&!(cpu.status&CCRV)&&!(cpu.status&CCRZ))) break;
			cpu.pc.w++; 
			return; // N&V&!Z|!N&!V&!Z	BGT
		case 0xf00: 
			if((cpu.status&CCRZ)||
			   ((cpu.status&CCRN)&&!(cpu.status&CCRV))||
			   ((cpu.status&CCRV)&&!(cpu.status&CCRN))) break;
			cpu.pc.w++; 
			return; // Z|N&!V|!N&V		BLE
	} 
	
	if((char)opcode) {

		cpu.pc.b+=(char)opcode;
		return;
	}
	
	cpu.clock+=4;
	cpu.pc.b+=xwendian(*cpu.pc.w);
}

