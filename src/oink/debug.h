/*
+----------------------------------------------------------------------------+
|                                                                            |
|  debug.h - debug routines                                                  |
|                                                                            |
+----------------------------------------------------------------------------+
*/

#define BOCHS_PORT_CMD  0x8A00
#define BOCHS_PORT_DATA 0x8A01

/* Used to enable the device. */
#define BOCHS_CMD_ENABLE 0x8A00
/* Selects register 0: Memory monitoring range start address (inclusive). */
#define BOCHS_CMD_SELECT_SELECTREG0 0x8A01
/* Selects register 1: Memory monitoring range end address (exclusive). */
#define BOCHS_CMD_SELECT_SELECTREG1 0x8A02
/* Memory range monitoring as indicated by the register and clear both. */
#define BOCHS_CMD_SELECT_MEMRANGEMON 0x8A80
/* Return to Debugger Prompt */
#define BOCHS_CMD_PROMPT 0x8AE0
/* Instruction Trace Disable */
#define BOCHS_CMD_INSTRTRACEOFF 0x8AE2

/* Instruction Trace Enable */
#define BOCHS_CMD_INSTRTRACEON 0x8AE3
/* Register Trace Disable */
#define BOCHS_CMD_REGTRACEOFF 0x8AE4
/* Register Trace Enable */
#define BOCHS_CMD_REGTRACEON 0x8AE5
/* Disable the I/O interface. */
#define BOCHS_CMD_DISABLE 0x8AFF

extern BOOL debug_is_active();
extern void debug_enable();
extern void debug_disable();
extern BOOL debug_cmd(byte cmd);
