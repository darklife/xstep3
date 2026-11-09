#include "68000.h"
 
inline unsigned word xwendian(unsigned word x) {

	return(x>>8|x<<8);
}

inline unsigned long xlendian(unsigned long x) {

	return(x>>24|((x>>8)&0xff00)|((x<<8)&0xff0000)|x<<24);
}
