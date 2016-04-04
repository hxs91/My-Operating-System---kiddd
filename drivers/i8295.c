#include "const.h"

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
	port_byte_out(INT_M_CTLMASK, 0xFD);
	port_byte_out(INT_S_CTLMASK, 0XFF);
}