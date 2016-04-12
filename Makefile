#####################
# kiddd os makefile #
#####################


#FLAGS
CFLAG = -I include/



C_SOURCES = $(wildcard drivers/*.c lib/*.c kernel/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h include/*.h lib/*.h)

# TODO: Make source dep on all header files.

# Create a list of object files to build, simple by replacing 
# the '.c' extension of filenames in C_SOURCES with '.o'
OBJ = ${C_SOURCES:.c=.o}

all: os-image
	
run:
	bochs

#$< is the first dependancy and $@ is the target
#a.img: os-image
	#dd if=$< of=$@ bs=1320 count=1 conv=notrunc

#$^ is the whole dependancy
os-image: boot/boot_sect.bin kernel.bin
	cat $^ > os-image

kernel.bin: kernel/kernel_entry.o kernel/interrupt_func.o ${OBJ}
	ld -o $@ -melf_i386 -Ttext 0x1000 $^ --oformat binary

%.o: %.c ${HEADERS}
	gcc -ffreestanding -m32 -c $< ${CFLAG} -o $@

#Assemble the kernel_entry
%.o: %.asm
	nasm $< -f elf -I './kernel/' -o $@

%.bin:  %.asm
	nasm $< -f bin -I './boot/' -o $@

clean:
	rm -fr *.bin *.o os-image a.img
	rm -fr kernel/*.o boot/*.bin drivers/*.o lib/*.o
