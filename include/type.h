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

/* descriptor of segment */
typedef struct s_descriptor {
	u16 limit_low;
	u16	base_low;
	u8	base_mid;
	u8 	attr1;
	u8	limit_high_attr2;
	u8	base_high;
}DESCRIPTOR;

//typedef the function pointer of interrupt handler function
typedef void (*int_handler) ();
typedef void (*task_f) ();

typedef struct s_tss {
	u32	backlink;
	u32	esp0;		/* stack pointer to use during interrupt */
	u32	ss0;		/*   "   segment  "  "    "        "     */
	u32	esp1;
	u32	ss1;
	u32	esp2;
	u32	ss2;
	u32	cr3;
	u32	eip;
	u32	flags;
	u32	eax;
	u32	ecx;
	u32	edx;
	u32	ebx;
	u32	esp;
	u32	ebp;
	u32	esi;
	u32	edi;
	u32	es;
	u32	cs;
	u32	ss;
	u32	ds;
	u32	fs;
	u32	gs;
	u32	ldt;
	u16	trap;
	u16	iobase;	/* I/O位图基址大于或等于TSS段界限，就表示没有I/O许可位图 */
	/*u8	iomap[2];*/
}TSS;

#endif