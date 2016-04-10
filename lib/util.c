#include "type.h"

/*
some common utils used in our codes
*/
void memory_copy(void * src, void * des, int num_bytes) {
	// make sure num_bytes is legal
	//assert(num_bytes > 0); We can not use the lib of gcc!
	u8 * source = (u8*) src;
	u8 * dest = (u8*) des;
	if (num_bytes <= 0) {
		print("No, the number of bytes is illegal\n");
		return;
	}

	//ensure there is no overlap between source and dest
	// if (  (source + num_bytes) > dest || (dest + num_bytes) > source ) {
	// 	print("No, the destination memory overlaps with the source destination.\n");
	// 	print_hex(source);
	// 	print_hex(dest);
	// 	print_hex(num_bytes);
	// 	return;
	// }
	int i;
	for (i = 0; i < num_bytes ; i++) {
		*(dest + i) = *(source + i);
	}
}

/*
from integer to string
*/
void itoa(char * dest, int num) {
	char * p = dest;
	char ch;
	int i;
	int flag = 0;
	*p++ = '0';
	*p++ = 'x';
	if (num == 0) {
		*p++ = '0';
	}
	else {
		for (i = 28; i >= 0 ; i-=4) {
			ch = (num >> i) & 0xF;
			if (flag || (ch > 0)) {
				flag = 1;
				ch += '0';
				if (ch > '9') {
					ch += 7;	//upper case
				}
				*p++ = ch;
			}
		}
	}
	*p = 0;
}

/*
function for delay
*/
void delay(int time)
{
	int i, j, k;
	for ( k=0 ; k < time; k++ ) {
		/*for(i=0;i<10000;i++){	for Virtual PC	*/
		for(i=0;i<1000;i++){/*	for Bochs	*/
			for(j=0;j<10000;j++){}
		}
	}
}

/* memset */
void memset(void * adr, int ch, int n) {
	if (adr == 0) {
		print("memset error!\n");
		return;
	}
	int i;
	u8 * p_adr = (u8 *) adr;
	u8 x = (u8) ch;
	for(i = 0; i < n ; i++) {
		*p_adr++ = x;
	}
}