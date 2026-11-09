#include "68000.h"

char tobinbuffer[33];

inline char *tobin(unsigned x,int s) {

	int i;
	
	for(i=0;i!=s;i++) 
		tobinbuffer[i]=(x&(1<<((s-1)-i)))?'1':'0';

	tobinbuffer[s]=0;
	return(tobinbuffer);
}

inline long getindex(void) {

        int regs,code;

        regs=((code=*cpu.pc.w++)>>12)&7;

        return(((code&80000000)?
              cpu.a[regs].l:cpu.d[regs].l)+
              (code&0xff));
}

void init(int size) {

        int x,y;

	space.b=(char *)malloc(size);

	for(x=0;x!=65536;x++) ucode[x]=illegalf;

        for(x=0;x!=4096;x++) {

                ucode[xwendian(0x1000|x)]=movebf;
                ucode[xwendian(0x2000|x)]=movelf;
                ucode[xwendian(0x3000|x)]=movewf;
                ucode[xwendian(0x6000|x)]=bccf;
        }

        for(y=0;y!=64;y++) {

                for(x=0;x!=8;x++) {

                        ucode[xwendian(0x5000|(x<<9)|y)]=addqbf;
                        ucode[xwendian(0x5040|(x<<9)|y)]=addqwf;
                        ucode[xwendian(0x5080|(x<<9)|y)]=addqlf;
                        ucode[xwendian(0x5100|(x<<9)|y)]=subqbf;
                        ucode[xwendian(0x5140|(x<<9)|y)]=subqwf;
                        ucode[xwendian(0x5180|(x<<9)|y)]=subqlf;
                }
        }
        ucode[xwendian(0x4e72)]=stopf;

}
