#include "schedos-app.h"
#include "x86sync.h"

/*****************************************************************************
 * schedos-1
 *
 *   This tiny application prints red "1"s to the console.
 *   It yields the CPU to the kernel after each "1" using the sys_yield()
 *   system call.  This lets the kernel (schedos-kern.c) pick another
 *   application to run, if it wants.
 *
 *   The other schedos-* processes simply #include this file after defining
 *   PRINTCHAR appropriately.
 *
 *****************************************************************************/

#ifndef PRINTCHAR
#define PRINTCHAR	('1' | 0x0C00)
#endif


#ifndef PRIORITY
#define PRIORITY 6
#endif

#ifndef SHARE
#define SHARE 10
#endif

void
start(void)
{
	int i;
	//int to_print = PRINTCHAR;
	//int priority = 0;

	//while (!(priority = generate_priority() % NPROCS)); //priority > 0
	sys_priority(PRIORITY);
	//sys_share(SHARE);
	sys_share();
	sys_yield();

	for (i = 0; i < RUNCOUNT; i++) {
		// Write characters to the console, yielding after each one.
		//*cursorpos++ = PRINTCHAR;
		//sys_yield();
		sys_write_char(PRINTCHAR);
		sys_yield();
	}

	// Exercise 2
	// now processes exit, and don't yield forever
	sys_exit(0);
	// Yield forever.
	//while (1)
	//	sys_yield();
}
