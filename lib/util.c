/*
some common utils used in our codes
*/
void memory_copy(char * source, char * dest, int num_bytes) {
	// make sure num_bytes is legal
	//assert(num_bytes > 0); We can not use the lib of gcc!
	if (num_bytes <= 0) {
		print("No, the number of bytes is illegal\n");
		return;
	}

	//ensure there is no overlap between source and dest
	//assert( (source + num_bytes) <= dest || (dest + num_bytes) <= num_bytes );
	if ((source + num_bytes) > dest || (dest + num_bytes) > source) {
		print("No, the destination memory overlaps with the source destination.\n");
		return;
	}
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