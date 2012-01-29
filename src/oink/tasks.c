#include <types.h>
#include <video.h>
#include <string.h>
#include <descriptor.h>

#define MAX_TASK_COUNT 2

static word taskCount = 0;

//DWORD tssSize = sizeof(TSS);

volatile TSS tasks[MAX_TASK_COUNT];
static volatile byte stacks[MAX_TASK_COUNT][1024];

extern void taskJump(dword selector);
extern void taskJumpFirst();
extern dword currentTask;

void task0()
{
	//int i;
	for ( ; ; )
	{
		//cputs("\r\nentered task0\r\n");

		print_word(0, 0, 0, 0x0D);
	}
}

void task1()
{
	//int i;
	for ( ; ; )
	{
		print_word(20, 0, 1, 0x0D);
	}
}

void tasks_switch()
{
	if (taskCount == 0) {
		return;
	}
	word sel = 0;
	currentTask = (currentTask + 1) % taskCount;
	sel = (currentTask + 3) << 3;

	print_word(currentTask * 14 + 0, 24, sel, 0x0C);
	print_dword(currentTask * 14 + 5, 24, tasks[currentTask].esp, 0x0C);

	taskJump(sel);
}

void tasksInit()
{
	DESCRIPTOR desc;

	taskCount = 2u;

	TSS *pTSS = (TSS *)tasks;
	memset(pTSS, 0, sizeof(TSS) * taskCount);

	tasks[0].eax      = 0;
	tasks[0].ebx      = 0;
	tasks[0].ecx      = 0;
	tasks[0].edx      = 0;
	tasks[0].ebp      = 0;
	tasks[0].esi      = 0;
	tasks[0].edi      = 0;
	tasks[0].eflags   = 0x202;
	tasks[0].eip      = (dword)task0;
	tasks[0].cs       = 8;
	tasks[0].ds       = 0x10;
	tasks[0].ss       = 0x10;
	tasks[0].ss0      = 0x10;
	tasks[0].ss1      = 0x10;
	tasks[0].ss2      = 0x10;
	tasks[0].es       = 0x10;
	tasks[0].fs       = 0x10;
	tasks[0].gs       = 0x10;
	tasks[0].esp      = (dword)(&(stacks[0])) + 1024;
	tasks[0].esp0     = (dword)(&(stacks[0])) + 1024;
	tasks[0].esp1     = (dword)(&(stacks[0])) + 1024;
	tasks[0].esp2     = (dword)(&(stacks[0])) + 1024;
	tasks[0].backlink = 0;
	tasks[0].ldt      = 0;

	memset((byte *)stacks[0], 0, 1024);

	desc.raw.lo = 0xFFFF | ((dword)pTSS << 16);
	desc.raw.hi = 0x8900 | (((dword)pTSS >> 16) & 0xFF) | 0x9F0000;

	descriptor_set(GDT_ADDR, 3, &desc);
	
	pTSS = (TSS *)tasks + 1;

	tasks[1].eax      = 0;
	tasks[1].ebx      = 0;
	tasks[1].ecx      = 0;
	tasks[1].edx      = 0;
	tasks[1].ebp      = 0;
	tasks[1].esi      = 0;
	tasks[1].edi      = 0;
	tasks[1].eflags   = 0x202;
	tasks[1].eip      = (dword)task1;
	tasks[1].cs       = 8;
	tasks[1].ds       = 0x10;
	tasks[1].ss       = 0x10;
	tasks[1].ss0      = 0x10;
	tasks[1].ss1      = 0x10;
	tasks[1].ss2      = 0x10;
	tasks[1].es       = 0x10;
	tasks[1].fs       = 0x10;
	tasks[1].gs       = 0x10;
	tasks[1].esp      = (dword)&(stacks[1]) + 1024;
	tasks[1].esp0     = (dword)&(stacks[1]) + 1024;
	tasks[1].esp1     = (dword)&(stacks[1]) + 1024;
	tasks[1].esp2     = (dword)&(stacks[1]) + 1024;
	tasks[1].backlink = 0;
	tasks[1].ldt      = 0;

	memset((byte *)stacks[1], 0, 1024);
	
	desc.raw.lo = 0xFFFF | ((dword)pTSS << 16);
	desc.raw.hi = 0x8900 | (((dword)pTSS >> 16) & 0xFF) | 0x9F0000;
	descriptor_set(GDT_ADDR, 4, &desc);

	currentTask = taskCount - 1;
	ltr((currentTask + 3) << 3);
}
