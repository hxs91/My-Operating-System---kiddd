#include "const.h"
#include "screen.h"
#include "global.h"

/*======================================================================*
                           spurious_irq
 *======================================================================*/
PUBLIC void spurious_irq(int irq)
{
	print("spurious_irq: ");
	print(irq);
	print("\n");
}

/*======================================================================*
                           put_irq_handler
 *======================================================================*/
PUBLIC void put_irq_handler(int irq, irq_handler handler)
{
	disable_irq(irq);
	irq_table[irq] = handler;
}

void init_8259A() {
	// Master 8295, ICW1
	port_byte_out(INT_M_CTL, 0x11);

	// Slave 8295, ICW1
	port_byte_out(INT_S_CTL, 0x11);

	// Master 8295, ICW2
	port_byte_out(INT_M_CTLMASK, INT_VECTOR_IRQ0);

	// Slave 8295, ICW2
	port_byte_out(INT_S_CTLMASK, INT_VECTOR_IRQ8);

	// ICW3
	port_byte_out(INT_M_CTLMASK, 0x4);
	port_byte_out(INT_S_CTLMASK, 0X2);

	// ICW4
	port_byte_out(INT_M_CTLMASK, 0x1);
	port_byte_out(INT_S_CTLMASK, 0X1);

	//OCW1
	port_byte_out(INT_M_CTLMASK, 0xFE);
	port_byte_out(INT_S_CTLMASK, 0XFF);


	int i;
	for (i = 0 ; i < NR_IRQ; i++) {
		irq_table[i] = spurious_irq;
	}
}