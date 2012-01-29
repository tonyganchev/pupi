OUTPUT(obj/kernel.o)
OUTPUT_FORMAT("elf32-i386")
ENTRY(start)
SECTIONS {
	. = 0x20000;
   	.text :	{
		obj/entry.o
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
	}
	.data :	{
		obj/entry.o
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
	}
	.bss :	{
		obj/entry.o
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
	}
}