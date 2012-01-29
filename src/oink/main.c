#include <types.h>
#include <ports.h>
#include <video.h>
#include <conio.h>
#include <format.h>
#include <string.h>
#include <timer.h>
#include <intr.h>
#include <keyb.h>
#include <descriptor.h>
//#include "intr_def.h"

static void ih_div_zero()
{
	START_INTERRUPT();
	cputs("DIVISION BY ZERO\r\n");
	__asm__ __volatile__("hlt");
	END_INTERRUPT();
}

static void ih_debugger()
{
	START_INTERRUPT();
	cputs("DEBUGGER\r\n");
	__asm__ __volatile__("hlt");
	END_INTERRUPT();
}

static void ih_nmi()
{
	START_INTERRUPT();
	cputs("NMI\r\n");
	__asm__ __volatile__("hlt");
	END_INTERRUPT();
}

static void ih_breakpoint()
{
	START_INTERRUPT();
	cputs("BREAKPOINT\r\n");
	__asm__ __volatile__("hlt");
	END_INTERRUPT();
}

static void ih_overflow()
{
	START_INTERRUPT();
	cputs("OVERFLOW\r\n");
	__asm__ __volatile__("hlt");
	END_INTERRUPT();
}

static void ih_bounds()
{
	START_INTERRUPT();
	cputs("BOUNDS\r\n");
	__asm__ __volatile__("hlt");
	END_INTERRUPT();
}

static void ih_invalid_opcode()
{
	START_INTERRUPT();
	cputs("INVALID OPCODE\r\n");
	__asm__ __volatile__("hlt");
	END_INTERRUPT();
}

static void ih_coproc_na()
{
	START_INTERRUPT();
	cputs("COPROCESSOR N/A\r\n");
	__asm__ __volatile__("hlt");
	END_INTERRUPT();
}

static void ih_df()
{
	START_INTERRUPT();
	cputs("DOUBLE FAULT\r\n");
	__asm__ __volatile__("hlt");
	END_INTERRUPT();
}

static void ih_coproc_seg_overrun()
{
	START_INTERRUPT();
	cputs("COPROCESSOR SEGMENT OVERRUN\r\n");
	__asm__ __volatile__("hlt");
	END_INTERRUPT();
}

static void ih_invalid_tss()
{
	START_INTERRUPT();
	cputs("INVALID TSS\r\n");
	__asm__ __volatile__("hlt");
	END_INTERRUPT();
}

static void ih_seg_not_present()
{
	START_INTERRUPT();
	cputs("SEGMENT NOT PRESENT\r\n");
	__asm__ __volatile__("hlt");
	END_INTERRUPT();
}

static void ih_sf()
{
	START_INTERRUPT();
	cputs("STACK FAULT\r\n");
	__asm__ __volatile__("hlt");
	END_INTERRUPT();
}

static void ih_gpf()
{
	START_INTERRUPT();
	char_attr_set(0x0C);
	cputs("GENERAL PROTECTION FAULT\r\n");
	__asm__ __volatile__("hlt");
	END_INTERRUPT();
}

static void ih_pf()
{
	START_INTERRUPT();
	cputs("PAGE FAULT\r\n");
	__asm__ __volatile__("hlt");
	END_INTERRUPT();
}

static void ih_empty()
{
	START_INTERRUPT();
	cputs("UNHANDLED EXCEPTION\r\n");
	//__asm__ __volatile__("hlt");
	END_INTERRUPT();
}

static void ih_math_fault()
{
	START_INTERRUPT();
	cputs("MATH FAULT\r\n");
	__asm__ __volatile__("hlt");
	END_INTERRUPT();
}

static void ih_align_check()
{
	START_INTERRUPT();
	cputs("ALIGNMENT CHECK\r\n");
	__asm__ __volatile__("hlt");
	END_INTERRUPT();
}

static void ih_machine_check()
{
	START_INTERRUPT();
	cputs("MACHINE CHECK\r\n");
	__asm__ __volatile__("hlt");
	END_INTERRUPT();
}

static void ih_simd_float_ex()
{
	START_INTERRUPT();
	cputs("SIMD FLOATING-POINT EXCEPTION\r\n");
	__asm__ __volatile__("hlt");
	END_INTERRUPT();
}

//extern void timerInterruptAsm();

static void intr_init()
{
	byte i;
	for (i = 0u; i <= 48u; i++)
		intr_set(i, ih_empty, 8, DA_KRNINTR);

	intr_set(0x00, ih_div_zero, 8, DA_KRNINTR);
	intr_set(0x01, ih_debugger, 8, DA_KRNINTR);
	intr_set(0x02, ih_nmi, 8, DA_KRNINTR);
	intr_set(0x03, ih_breakpoint, 8, DA_KRNINTR);
	intr_set(0x04, ih_overflow, 8, DA_KRNINTR);
	intr_set(0x05, ih_bounds, 8, DA_KRNINTR);
	intr_set(0x06, ih_invalid_opcode, 8, DA_KRNINTR);
	intr_set(0x07, ih_coproc_na, 8, DA_KRNINTR);
	intr_set(0x08, ih_df, 8, DA_KRNINTR);
	intr_set(0x09, ih_coproc_seg_overrun, 8, DA_KRNINTR);
	intr_set(0x0A, ih_invalid_tss, 8, DA_KRNINTR);
	intr_set(0x0B, ih_seg_not_present, 8, DA_KRNINTR);
	intr_set(0x0C, ih_sf, 8, DA_KRNINTR);
	intr_set(0x0D, ih_gpf, 8, DA_KRNINTR);
	intr_set(0x0E, ih_pf, 8, DA_KRNINTR);
	intr_set(0x0F, ih_empty, 8, DA_KRNINTR);
	intr_set(0x10, ih_math_fault, 8, DA_KRNINTR);
	intr_set(0x11, ih_align_check, 8, DA_KRNINTR);
	intr_set(0x12, ih_machine_check, 8, DA_KRNINTR);
	intr_set(0x13, ih_simd_float_ex, 8, DA_KRNINTR);
	intr_set(0x20, ih_timer, 8, DA_KRNINTR);
	intr_set(0x21, ih_keyboard, 8, DA_KRNINTR);
}

extern word taskCount;

int main()
{
	//asm("hlt");
	intr_disable();
	intr_init();

	//tasksInit();
	intr_enable();

	cputs("ssss\n");
//
	char buffer[20];
	strcmp("PESHO", "PESHO");
//	ltoa(strcmp("PESHO", "PESHO"), buffer);
//
//	cputs(buffer);

	for (;;)
		;

	return 0;
}

int __main()
{
	main();
	return 0;
}

