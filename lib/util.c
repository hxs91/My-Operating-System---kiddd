/*
some common utils used in our codes
*/
void memory_copy(char * source, char * dest, int num_bytes) {
	// make sure num_bytes is legal
	//assert(num_bytes > 0); We can not use the lib of gcc!
	if (num_bytes <= 0) {
		print("No, the number of bytes is illegal\n");
	}

	//ensure there is no overlap between source and dest
	//assert( (source + num_bytes) <= dest || (dest + num_bytes) <= num_bytes );
	if ((source + num_bytes) > dest || (dest + num_bytes) > source) {
		print("No, the destination memory overlaps with the source destination.\n");
	}
	int i;
	for (i = 0; i < num_bytes ; i++) {
		*(dest + i) = *(source + i);
	}
}

/*
from integer to string
*/
char * itoa(int num) {
	
}