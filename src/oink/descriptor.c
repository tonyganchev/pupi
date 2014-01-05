#include "types.h"
#include "string.h"
#include "descriptor.h"

//static char *gates_names[] = {
//		"DA_GATE_INVALID", "DA_GATE_AVAIL286TSS", "DA_GATE_LDT",
//		"DA_GATE_BUSY286TSS", "DA_GATE_286CALLGATE", "DA_GATE_TASKGATE",
//		"DA_GATE_286INTRGATE", "DA_GATE_286TRAPGATE", "DA_GATE_INVALID2",
//		"DA_GATE_AVAIL386TSS", "DA_GATE_RESERVED", "DA_GATE_BUSY386TSS",
//		"DA_GATE_386CALLGATE", "DA_GATE_RESERVED2", "DA_GATE_386INTRGATE",
//		"DA_GATE_386TRAPGATE", };

void ltr(SELECTOR selector)
{
	__asm__ __volatile__ ("ltr %0" : : "r" (selector));
}

SELECTOR str(void)
{
	SELECTOR selector;
	__asm__ __volatile__ ("str %0" : "=r" (selector));
	return selector;
}

void descriptor_set(const DESCRIPTOR* tbl, const DESCRIPTORINDEX idx,
		const DESCRIPTOR* desc)
{
	memcpy((DESCRIPTOR *) (tbl + idx), desc, sizeof(DESCRIPTOR));
}

void descriptor_get(const DESCRIPTOR* tbl, const word idx, DESCRIPTOR *desc)
{
	memcpy(desc, tbl + idx, sizeof(DESCRIPTOR));
}
