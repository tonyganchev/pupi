/**
 * \file Glue functions needed to port newlib C
 */

#include <types.h>
#include <conio.h>

#include <sys/stat.h>

#include <errno.h>
#undef errno
extern int errno;

struct tms;

void _write(int fd, const void *buf, size_t cnt) {
	kputs(buf);
}

void abort() {
	__asm__ __volatile__ ("hlt");
}

/**
 _exit
 Exit a program without cleaning up files.
 If your system doesn't provide this, it is best to avoid linking with subroutines that
 require it (exit, system).
 */

/**
 * close
 * Close a file.
 *
 *
 */
int close(int file)
{
	return -1;
}

/**
 * environ
 * A pointer to a list of environment variables and their values.
 * For a minimal environment, this empty list is adequate:
 */
char *__env[1] = { 0 };
char **environ = __env;

/**
 * execve
 * Transfer control to a new process.
 *
 * Minimal implementation (for a system without processes):
 */
int execve(char *name, char **argv, char **env)
{
	errno = ENOMEM;
	return -1;
}

/**
 * fork
 * Create a new process.
 *
 * Minimal implementation (for a system without processes):
 */
int fork()
{
	errno = EAGAIN;
	return -1;
}

/**
 * fstat
 * Status of an open file.
 *
 * For consistency with other minimal implementations in these examples, all
 * files are regarded as character special devices.
 *
 * The `sys/stat.h' header file is distributed in the `include' subdirectory for
 * this C library.
 *
 * #include <sys/stat.h>
 */
int fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

/**
 * getpid
 * Process-ID;
 *
 * This is sometimes used to generate strings unlikely to conflict with other processes.
 *
 * Minimal implementation, for a system without processes:
 */

int getpid()
{
	return 1;
}

/**
 * isatty
 * Query whether output stream is a terminal.
 *
 * For consistency with the other minimal implementations, which only support
 * output to stdout, this minimal implementation is suggested:
 */
int isatty(int file)
{
	return 1;
}

/**
 * kill
 * Send a signal.
 */
int kill(int pid, int sig)
{
	errno = EINVAL;
	return (-1);
}

/**
 * link
 * Establish a new name for an existing file.
 */
int link(char *old, char *new)
{
	errno = EMLINK;
	return -1;
}

/**
 * lseek
 * Set position in a file.
 */
int lseek(int file, int ptr, int dir)
{
	return 0;
}

/**
 * open
 * Open a file.
 */
int open(const char *name, int flags, int mode)
{
	return -1;
}

/**
 * read
 * Read from a file.
 */
int read(int file, char *ptr, int len)
{
	return 0;
}

/**
 * sbrk
 * Increase program data space.
 * As malloc and related functions depend on this, it is useful to have a working implementation.
 *
 * The following suffices for a standalone system;
 * it exploits the symbol end automatically defined by the GNU linker.
 */
caddr_t sbrk(int incr)
{
	extern char end; /* Defined by the linker */
	static char *heap_end;
	char *prev_heap_end;

	if (heap_end == 0)
	{
		heap_end = &end;
	}
	prev_heap_end = heap_end;

	void* stack_p;
	__asm__ __volatile__ ("inl %%esp, %0" : "=a" (stack_p));
	if (heap_end + incr > (char *)stack_p)
	{
		_write(1, "Heap and stack collision\n", 25);
		abort();
	}

	heap_end += incr;
	return (caddr_t) prev_heap_end;
}

/**
 * stat
 * Status of a file(by name)
 */
int stat(const char *file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

/**
 * times
 * Timing information for current process.
 */
clock_t times(struct tms *buf)
{
	return -1;
}

/**
 * unlink
 * Remove a file's d irectory entry.
 */
int unlink(char *name)
{
	errno = ENOENT;
	return -1;
}

/**
 *wait
 *Wait for a child process.
 */
int wait(int *status)
{
	errno = ECHILD;
	return -1;
}

void writechar(char c) {
	kputchar(c);
}

/**
 * write
 * Write a character to a file.
 *
 * `libc' subroutines will use this system routine for output to all files,
 * including stdout---so if you need to generate any output, for example to a
 * serial port for debugging, you should make your minimal write capable of
 * doing this.
 *
 * The following minimal implementation is an incomplete example; it relies on a
 * writechar subroutine (not shown; typically, you must write this in assembler
 * from examples provided by your hardware manufacturer) to actually perform the
 * output.
 */
int write(int file, char *ptr, int len)
{
	int todo;

	for (todo = 0; todo < len; todo++)
	{
		writechar(*ptr++);
	}
	return len;
}
