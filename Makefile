#_______________________________________________________________________________
# Tools

CC = i586-elf-gcc

AS = nasm

LD = i586-elf-ld

OBJCOPY = objcopy

#_______________________________________________________________________________
# Directories

DIR_SRC = src
DIR_BOOT = $(DIR_SRC)/boot
DIR_KRN = $(DIR_SRC)/oink
DIR_OBJ = obj
DIR_BIN = bin
DIR_DEP = dep
DIR_DBG = debug

#_______________________________________________________________________________
# Files & Extensions

OINK_CC_SRCS = $(DIR_KRN)/video.c $(DIR_KRN)/ports.c $(DIR_KRN)/memsetw.c $(DIR_KRN)/intr.c \
               $(DIR_KRN)/keyb.c $(DIR_KRN)/conio.c $(DIR_KRN)/floppy.c $(DIR_KRN)/timer.c \
               $(DIR_KRN)/tasks.c $(DIR_KRN)/debug.c $(DIR_KRN)/descriptor.c $(DIR_KRN)/vesa.c \
               $(DIR_KRN)/bits.c $(DIR_KRN)/libc_glue.c
OINK_MAIN_SRC = $(DIR_KRN)/main.c
SRCS = ${OINK_CC_SRCS} ${OINK_MAIN_SRC}
DEPS = $(subst $(DIR_KRN), $(DIR_DEP), $(SRCS:.c=.d))
OINK_CC_OBJS = $(subst $(DIR_KRN), $(DIR_OBJ), $(OINK_CC_SRCS:.c=.o))
#OINK_CC_OBJS = $(OINK_CC_SRCS:.c=.o)
OBJS = ${OINK_CC_OBJS} obj/main.o obj/entry.o obj/switch.o

INCLUDES = -I$(DIR_SRC)/include -Ilibc/i586-elf/include -g

CFLAGS = -nostdlib -fno-builtin ${INCLUDES} -Wall -std=c99

LDFLAGS = -lc -Llibc/i586-elf/lib
#LDFLAGS = -lgcc -L/lib/gcc/i686-pc-cygwin/3.4.4 -Llibc/i586-elf/

LDSCRIPT = pupi.sc

#_______________________________________________________________________________
# RULES

all: build install

-include $(DEPS)

dump:
	echo $(OBJS)
	echo $(OINK_CC_SRCS)

build: bin/floppy.img

install:

${DIR_DEP}/%.${DEPEND_EXT}: $(DIR_SRC)/%.c ${DIR_DEP}/placeholder
	$(CC) $(CFLAGS) -MF"$@" -MG -MM -MP -MT"$@" -MT"obj/$*.o" "$<"
# 	${CC} -MM ${INCLUDES} $(DIR_SRC)/$*.c -MF ${DIR_DEP}/$*.${DEPEND_EXT}
# -MF  write the generated dependency rule to a file
# -MG  assume missing headers will be generated and don't stop with an error
# -MM  generate dependency rule for prerequisite, skipping system headers
# -MP  add phony target for each header to prevent errors when header is missing
# -MT  add a target to the generated dependency

%/placeholder:
	mkdir $* -p
	touch $*/placeholder

$(DIR_OBJ)/entry.o: $(DIR_KRN)/entry.asm $(DIR_OBJ)/placeholder $(DIR_DBG)/placeholder 
	$(AS) -f elf32 $(DIR_KRN)/entry.asm -o $@ -l $(DIR_DBG)/entry.lst
	
$(DIR_OBJ)/switch.o: $(DIR_KRN)/switch.asm $(DIR_OBJ)/placeholder $(DIR_DBG)/placeholder
	$(AS) -f elf32 $(DIR_KRN)/switch.asm -o $@ -l $(DIR_DBG)/switch.lst

$(DIR_OBJ)/%.o $(DIR_DBG)/%.s: $(DIR_KRN)/%.c $(DIR_OBJ)/placeholder $(DIR_DBG)/placeholder
	$(CC) $(CFLAGS) -c $(DIR_KRN)/$*.c -o $(DIR_OBJ)/$*.o
	$(CC) $(CFLAGS) -S $(DIR_KRN)/$*.c -o $(DIR_DBG)/$*.s 
	
$(DIR_OBJ)/main.o: ${OINK_MAIN_SRC} $(DIR_OBJ)/placeholder $(DIR_DBG)/placeholder
	$(CC) $(CFLAGS) -c $< -o $@ -ffreestanding
	$(CC) $(CFLAGS) -S $< -o $(DEBUG)/main.s -ffreestanding

$(DIR_BIN)/idt: $(DIR_BOOT)/idt.asm $(DIR_BIN)/placeholder
	$(AS) $(DIR_BOOT)/idt.asm -o $(DIR_BIN)/idt
$(DIR_BIN)/gdt: $(DIR_BOOT)/gdt.asm $(DIR_BIN)/placeholder
	$(AS) $(DIR_BOOT)/gdt.asm -o $(DIR_BIN)/gdt

$(DIR_OBJ)/kernel.o: ${OBJS} $(LDSCRIPT) $(DIR_OBJ)/placeholder $(DIR_DBG)/placeholder
	$(LD) -T $(LDSCRIPT) -M > debug/map.txt $(LDFLAGS)

$(DIR_BIN)/kernel.img: $(DIR_OBJ)/kernel.o $(DIR_BIN)/placeholder
	$(OBJCOPY) -R .note -R .comment -S -O binary $(DIR_OBJ)/kernel.o $@

$(DIR_BIN)/floppy.img: $(DIR_BOOT)/boot.asm $(DIR_BIN)/kernel.img $(DIR_BIN) $(DIR_BIN)/idt $(DIR_BIN)/gdt $(DIR_DBG)/placeholder
	$(AS) $(DIR_BOOT)/boot.asm -o $@ -l $(DIR_DBG)/boot.lst

clean:
	rm -fR $(DIR_DEP)
	rm -fR $(DIR_OBJ)
	rm -fR $(DIR_BIN)
	rm -fR $(DIR_DBG)
