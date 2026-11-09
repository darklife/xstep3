#include "68000.h"

inline byte *byteaddress(int mode,int regs) {

	switch(mode) {
	
		case 0: 
			return(&cpu.d[regs].b);
		case 2: cpu.clock+=4;
			return(space.b+cpu.a[regs].l);
		case 3: cpu.clock+=4;
			cpu.a[regs].l++;
			return(space.b+cpu.a[regs].l-1);
		case 4: cpu.clock+=4;
			cpu.a[regs].l--;
			return(space.b+cpu.a[regs].l);
		case 5: cpu.clock+=8;
			return(space.b+cpu.a[regs].l+(*cpu.pc.w++));
		case 6: cpu.clock+=10;
			return(space.b+cpu.a[regs].l+getindex());
	}
	switch(regs) {

		case 0: cpu.clock+=8;
			return(space.b+(*cpu.pc.w++));
		case 1: cpu.clock+=12;
			return(space.b+(*cpu.pc.l++));
		case 2: cpu.clock+=8;
			return(cpu.pc.b+(*cpu.pc.w++));
		case 3: cpu.clock+=10;
			return(cpu.pc.b+getindex());
		case 4: cpu.clock+=4;
			return((char *)(cpu.pc.w++));
	}
	return(0);
}

inline word *wordaddress(int mode,int regs) {

	switch(mode) {
	
		case 0: 
			return(&cpu.d[regs].w);
		case 1: 
			return(&cpu.a[regs].w);
		case 2: cpu.clock+=4;
			return((word *)(space.b+cpu.a[regs].l));
		case 3: cpu.clock+=4;
			cpu.a[regs].l+=2;
			return((word *)(space.b+cpu.a[regs].l-2));
		case 4: cpu.clock+=4;
			cpu.a[regs].l-=2;
			return((word *)(space.b+cpu.a[regs].l));
		case 5: cpu.clock+=8;
			return((word *)(space.b+cpu.a[regs].l+(*cpu.pc.w++)));
		case 6: cpu.clock+=10;
			return((word *)(space.b+cpu.a[regs].l+getindex()));
	}
	switch(regs) {

		case 0: cpu.clock+=8;
			return((word *)(space.b+(*cpu.pc.w++)));
		case 1: cpu.clock+=12;
			return((word *)(space.b+(*cpu.pc.l++)));
		case 2: cpu.clock+=8;
			return((word *)(cpu.pc.b+(*cpu.pc.w++)));
		case 3: cpu.clock+=10;
			return((word *)(cpu.pc.b+getindex()));
		case 4: cpu.clock+=4;
			return(cpu.pc.w++);
	}
	return(0);
}

inline long *longaddress(int mode,int regs) {

	switch(mode) {
	
		case 0: 
			return(&cpu.d[regs].l);
		case 1: 
			return(&cpu.a[regs].l);
		case 2: cpu.clock+=8;
			return((long *)(space.b+cpu.a[regs].l));
		case 3: cpu.clock+=8;
			cpu.a[regs].l+=4;
			return((long *)(space.b+cpu.a[regs].l-4));
		case 4: cpu.clock+=8;
			cpu.a[regs].l-=4;
			return((long *)(space.b+cpu.a[regs].l));
		case 5: cpu.clock+=12;
			return((long *)(space.b+cpu.a[regs].l+(*cpu.pc.w++)));
		case 6: cpu.clock+=14;
			return((long *)(space.b+cpu.a[regs].l+getindex()));
	}
	switch(regs) {

		case 0: cpu.clock+=12;
			return((long *)(space.b+(*cpu.pc.w++)));
		case 1: cpu.clock+=16;
			return((long *)(space.b+(*cpu.pc.l++)));
		case 2: cpu.clock+=12;
			return((long *)(cpu.pc.b+(*cpu.pc.w++)));
		case 3: cpu.clock+=14;
			return((long *)(cpu.pc.b+getindex()));
		case 4: cpu.clock+=8;
			return(cpu.pc.l++);
	}
	return(0);
}
