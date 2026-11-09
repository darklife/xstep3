#ifndef D68000
#define D68000
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SPACE   16777216

#define long	int
#define word	short
#define byte	char

#define CCRX 16
#define CCRN 8
#define CCRZ 4
#define CCRV 2
#define CCRC 1

struct {

	union {
	
		byte	b;
		word	w;
		long	l;
		
	} d[8];
	
	union {
	
		word	w;
		long	l;

	} a[8],usp,ssp;


	union {

		long	d;
	
		byte	*b;
		word	*w;
		long	*l;
		unsigned word *i;
	
	} pc;

	long 	clock,count,start,status;

} cpu;

union {

		long	d;

		byte	*b;
		word	*w;
		long	*l;	

} space;

void (*ucode[65536])();

unsigned word xwendian(unsigned word);
unsigned long xlendian(unsigned long);

word *wordaddress(int,int);
byte *byteaddress(int,int);
long *longaddress(int,int);
char *tobin(unsigned,int);
void init(int);
long getindex();
byte peekbyte(int,int);
word peekword(int,int);
long peeklong(int,int);
void pokebyte(int,int,byte);
void pokeword(int,int,word);
void pokelong(int,int,long);
void dump(void);

void illegalf();
void movebf();
void movelf();
void movewf();
void bccf();
void addqbf();
void addqwf();
void addqlf();
void subqbf();
void subqwf();
void subqlf();
void stopf();


#endif