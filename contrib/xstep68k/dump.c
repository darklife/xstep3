#include "68000.h"

inline void dump(void) {

	int i;

	printf(	"---------------------------------------------------------------\n");

	printf( "68000/%dMB (%f MIPS/%f MHz)\n",
		SPACE/1048576,
		(cpu.count/1000000.0)/((clock()-cpu.start)/CLOCKS_PER_SEC),
		((cpu.clock+cpu.count*4)/1000000.0)/((clock()-cpu.start)/CLOCKS_PER_SEC));

	printf(	"---------------------------------------------------------------\n");

	for(i=0;i!=8;i++)
		printf("D%d: %s %08x %17d\n",i,
			tobin(cpu.d[i].l,32),
			cpu.d[i].l,
			cpu.d[i].l);

	for(i=0;i!=8;i++)
		printf("A%d: %s %08x %17d\n",i,
			tobin(cpu.a[i].l,32),
			cpu.a[i].l,
			cpu.a[i].l);

		printf("PC: %s %08x %17u\n",
			tobin(cpu.pc.d-space.d,32),
			cpu.pc.d-space.d,
			cpu.pc.d-space.d);

		printf( "[KSR: -------- ---%c%c%c%c%c\n",
			cpu.status&CCRX?'X':'-',
			cpu.status&CCRN?'N':'-',
			cpu.status&CCRZ?'Z':'-',
			cpu.status&CCRV?'V':'-',
			cpu.status&CCRC?'C':'-');

	printf(	"---------------------------------------------------------------\n");

	printf( "[KNext PC: %08x/%08x:%04x (unknown)\n",
		cpu.pc.d,
		cpu.pc.d-space.d,
		xwendian((short)(*cpu.pc.w)));

	printf(	"---------------------------------------------------------------\n");			

	fflush(stdout);
}
