#include "types.h"
#include "intr.h"
#include "tasks.h"

#define SWITCH_TICKS 20

extern dword ticks;

void ih_timer()
{
	START_INTERRUPT();

	ticks++;
	if (ticks == SWITCH_TICKS)
	{
		ticks = 0;
		tasks_switch();
	}

	END_INTERRUPT();
}
