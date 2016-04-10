#include "proc.h"
#include "const.h"
#include "type.h"
#include "global.h"

/* calculate the absolute address by segment register */
PUBLIC u32 seg2phys(u16 seg)
{
	DESCRIPTOR* p_dest = &gdt[seg >> 3];

	return (p_dest->base_high << 24) | (p_dest->base_mid << 16) | (p_dest->base_low);
}

PRIVATE void init_descriptor(DESCRIPTOR * p_desc, u32 base, u32 limit, u16 attribute)
{
	p_desc->limit_low		= limit & 0x0FFFF;		// 段界限 1		(2 字节)
	p_desc->base_low		= base & 0x0FFFF;		// 段基址 1		(2 字节)
	p_desc->base_mid		= (base >> 16) & 0x0FF;		// 段基址 2		(1 字节)
	p_desc->attr1			= attribute & 0xFF;		// 属性 1
	p_desc->limit_high_attr2	= ((limit >> 16) & 0x0F) |
						(attribute >> 8) & 0xF0;// 段界限 2 + 属性 2
	p_desc->base_high		= (base >> 24) & 0x0FF;		// 段基址 3		(1 字节)
}

/* Init the descriptor in gdt for tss and ldt */
PUBLIC void init_descriptor_of_tss_and_ldt() {
	/* 填充 GDT 中 TSS 这个描述符 */
	memset(&tss, 0, sizeof(tss));
	tss.ss0		= SELECTOR_KERNEL_DS;
	
	init_descriptor(&gdt[INDEX_TSS],
			vir2phys(seg2phys(SELECTOR_KERNEL_DS), &tss),
			sizeof(tss) - 1,
			DA_386TSS);
	tss.iobase	= sizeof(tss);	/* 没有I/O许可位图 */

	init_descriptor(&gdt[INDEX_LDT_FIRST],
				vir2phys(seg2phys(SELECTOR_KERNEL_DS), proc_table[0].ldts),
				LDT_SIZE * sizeof(DESCRIPTOR) - 1,
				DA_LDT);
}