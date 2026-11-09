#include "68000.h"

inline void pokebyte(int mode,int regs,byte data) {

	switch(mode) {
	
		case 0: 
			cpu.d[regs].b=data; return;
		case 2: cpu.clock+=4;
			*(space.b+cpu.a[regs].l)=data; return;
		case 3: cpu.clock+=4;
			cpu.a[regs].l++;
			*(space.b+cpu.a[regs].l-1)=data; return;
		case 4: cpu.clock+=4;
			cpu.a[regs].l--;
			*(space.b+cpu.a[regs].l)=data; return;
		case 5: cpu.clock+=8;
			*(space.b+cpu.a[regs].l+(*cpu.pc.w++))=data; return;
		case 6: cpu.clock+=10;
			*(space.b+cpu.a[regs].l+getindex())=data; return;
		case 7:
			switch(regs) {

				case 0: cpu.clock+=8;
					*(space.b+(*cpu.pc.w++))=data; return;
				case 1: cpu.clock+=12;
					*(space.b+(*cpu.pc.l++))=data; return;
				case 2: cpu.clock+=8;
					*(cpu.pc.b+(*cpu.pc.w++))=data; return;
				case 3: cpu.clock+=10;
					*(cpu.pc.b+getindex())=data; return;
			}
	}
}

inline void pokeword(int mode,int regs,word data) {

	switch(mode) {
	
		case 0: 
			cpu.d[regs].w=data; return;
		case 1: 
			cpu.a[regs].w=data; return;
		case 2: cpu.clock+=4;
			*((word *)(space.b+cpu.a[regs].l))=data; return;
		case 3: cpu.clock+=4;
			cpu.a[regs].l+=2;
			*((word *)(space.b+cpu.a[regs].l-2))=data; return;
		case 4: cpu.clock+=4;
			cpu.a[regs].l-=2;
			*((word *)(space.b+cpu.a[regs].l))=data; return;
		case 5: cpu.clock+=8;
			*((word *)(space.b+cpu.a[regs].l+(*cpu.pc.w++)))=data; return;
		case 6: cpu.clock+=10;
			*((word *)(space.b+cpu.a[regs].l+getindex()))=data; return;
		case 7:
	
			switch(regs) {
		
				case 0: cpu.clock+=8;
					*((word *)(space.b+(*cpu.pc.w++)))=data; return;
				case 1: cpu.clock+=12;
					*((word *)(space.b+(*cpu.pc.l++)))=data; return;
				case 2: cpu.clock+=8;
					*((word *)(cpu.pc.b+(*cpu.pc.w++)))=data; return;
				case 3: cpu.clock+=10;
					*((word *)(cpu.pc.b+getindex()))=data; return;
			}
	}
}

inline void pokelong(int mode,int regs,long data) {

	switch(mode) {
	
		case 0: 
			cpu.d[regs].l=data; return;
		case 1: 
			cpu.a[regs].l=data; return;
		case 2: cpu.clock+=8;
			*((long *)(space.b+cpu.a[regs].l))=data; return;
		case 3: cpu.clock+=8;
			cpu.a[regs].l+=4;
			*((long *)(space.b+cpu.a[regs].l-4))=data; return;
		case 4: cpu.clock+=8;
			cpu.a[regs].l-=4;
			*((long *)(space.b+cpu.a[regs].l))=data; return;
		case 5: cpu.clock+=12;
			*((long *)(space.b+cpu.a[regs].l+(*cpu.pc.w++)))=data; return;
		case 6: cpu.clock+=14;
			*((long *)(space.b+cpu.a[regs].l+getindex()))=data; return;
	}
	switch(regs) {

		case 0: cpu.clock+=12;
			*((long *)(space.b+(*cpu.pc.w++)))=data; return;
		case 1: cpu.clock+=16;
			*((long *)(space.b+(*cpu.pc.l++)))=data; return;
		case 2: cpu.clock+=12;
			*((long *)(cpu.pc.b+(*cpu.pc.w++)))=data; return;
		case 3: cpu.clock+=14;
			*((long *)(cpu.pc.b+getindex()))=data; return;
	}
}
