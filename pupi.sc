OUTPUT(obj/kernel.o)
OUTPUT_FORMAT("elf32-i386")

INPUT(obj/entry.o
		obj/bits.o
		obj/intr.o
		obj/descriptor.o
		obj/debug.o
		obj/video.o
		obj/conio.o
		obj/memsetw.o
		obj/main.o
		obj/ports.o
		obj/keyb.o
		obj/floppy.o
		obj/timer.o
		obj/tasks.o
		obj/vesa.o
		obj/libc_glue.o
)

ENTRY(start)

SECTIONS {
	. = 0x20000;
   	.text : { *(.text) }
	.data : {
		*(.data)
	}
	. = 0x50000;
	end = .;
	.bss : { *(.bss) }
}
