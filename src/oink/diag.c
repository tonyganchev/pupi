#include <diag.h>
#include <descriptor.h>

void descriptor_dump(const byte *descTable, const word descNo)
{
	DESCRIPTOR desc;
	descriptor_get(descTable, descNo, &desc);
	
	// get access
	print_byte(0, 24, desc.desc.access & DA_EXECUTABLE, 0x0F);

	if ((desc.desc.access & DA_PRESENT) != 0)
		cputs("Present\r\n");
	else
		cputs("Not Present\r\n");
	cputs("DPL:");
	print_byte(4, 1, (desc.desc.access & DA_RING3) >> 5, 0x0F);
	cursor_move(0, 2);
	if ((desc.desc.access & DA_CODEDATA) != 0)
	{
		if ((desc.desc.access & DA_EXECUTABLE) != 0)
			cputs("Code Segment\r\n");
		else
		{
			if ((desc.desc.access & DA_EXPANDDOWN) != 0)
				cputs("Stack Segment\r\n");
			else
				cputs("Data Segment\r\n");
		}
		if ((desc.desc.access & DA_WRITEABLE) != 0)
			cputs("Writable Segment\r\n");
		else
			cputs("Read-only Segment\r\n");
		if ((desc.desc.access & DA_ACCESSED) != 0)
			cputs("Accessed\r\n");
		else
			cputs("Not accessed\r\n");
	}
	else
	{
		cputs("System Segment\r\n");
		cputs(arrGates[desc.desc.access & 0x0F]);
		cputs("\r\n");
	}
}
