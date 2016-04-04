#ifndef INCLUDE_TYPE_H
#define INCLUDE_TYPE_H

#define u32 unsigned int
#define u8	unsigned char
#define u16 unsigned short

/* descriptor of gate */
typedef struct s_gate
{
	u16 offset_low;
	u16 selector;
	u8 	dcount;
	u8	attr;
	u16 offset_high;
}GATE;

//typedef the function pointer of interrupt handler function
typedef void (*int_handler) ();

#endif