###############################################################################

CC = i586-elf-gcc

AS = nasm

LD = i586-elf-ld

OBJCOPY = objcopy

OINK_CC_SRCS = oink/video.c oink/ports.c oink/memsetw.c oink/intr.c \
               oink/keyb.c oink/conio.c oink/floppy.c oink/timer.c \
               oink/tasks.c oink/debug.c oink/descriptor.c oink/vesa.c \
               oink/bits.c oink/libc_glue.c

OINK_MAIN_SRC = oink/main.c

SRCS = ${OINK_CC_SRCS} ${OINK_MAIN_SRC}
DEPS = $(subst oink, $(DEPEND_DIR), $(SRCS:.c=.d))

DEPEND_DIR = dep

DEPEND_EXT = d

OINK_CC_OBJS = $(subst oink, obj, $(OINK_CC_SRCS:.c=.o))

OBJS = ${OINK_CC_OBJS} obj/main.o obj/entry.o obj/switch.o

INCLUDES = -Iinclude -Ilibc/i586-elf/include -g
#INCLUDES = -I include

CFLAGS = -nostdlib -fno-builtin ${INCLUDES} -Wall -std=c99

LDFLAGS = -lc -Llibc/i586-elf/lib
#LDFLAGS = -lgcc -L/lib/gcc/i686-pc-cygwin/3.4.4 -Llibc/i586-elf/

###############################################################################

all: build install

include $(DEPS)

build: bin/floppy.img

install:

${DEPEND_DIR}/%.${DEPEND_EXT}: oink/%.c ${DEPEND_DIR}/placeholder
	$(CC) $(CFLAGS) -MF"$@" -MG -MM -MP -MT"$@" -MT"obj/$*.o" "$<"
# 	${CC} -MM ${INCLUDES} oink/$*.c -MF ${DEPEND_DIR}/$*.${DEPEND_EXT}
# -MF  write the generated dependency rule to a file
# -MG  assume missing headers will be generated and don't stop with an error
# -MM  generate dependency rule for prerequisite, skipping system headers
# -MP  add phony target for each header to prevent errors when header is missing
# -MT  add a target to the generated dependency

%/placeholder:
	mkdir $* -p
	echo > $*/placeholder

obj/entry.o: oink/entry.asm obj/placeholder debug/placeholder 
	$(AS) -f elf32 oink/entry.asm -o $@ -l debug/entry.lst
	
obj/switch.o: oink/switch.asm obj/placeholder debug/placeholder
	$(AS) -f elf32 oink/switch.asm -o $@ -l debug/switch.lst

obj/%.o debug/%.s: oink/%.c obj/placeholder debug/placeholder
	$(CC) $(CFLAGS) -c oink/$*.c -o obj/$*.o
	$(CC) $(CFLAGS) -S oink/$*.c -o debug/$*.s 
	
obj/main.o: ${OINK_MAIN_SRC} obj/placeholder debug/placeholder
	$(CC) $(CFLAGS) -c $< -o $@ -ffreestanding
	$(CC) $(CFLAGS) -S $< -o debug/main.s -ffreestanding

bin/idt: boot/idt.asm bin/placeholder
	$(AS) boot/idt.asm -o bin/idt
bin/gdt: boot/gdt.asm bin/placeholder
	$(AS) boot/gdt.asm -o bin/gdt

obj/kernel.o: ${OBJS} pupi.sc obj/placeholder debug/placeholder
	$(LD) -T pupi.sc -M > debug/map.txt $(LDFLAGS)

bin/kernel.img: obj/kernel.o bin/placeholder
	$(OBJCOPY) -R .note -R .comment -S -O binary obj/kernel.o $@

bin/floppy.img: boot/boot.asm bin/kernel.img bin bin/idt bin/gdt debug/placeholder
	$(AS) boot/boot.asm -o $@ -l debug/boot.lst

clean:
	rm -fR dep
	rm -fR obj
	rm -fR bin
	rm -fR debug

###############################################################################
