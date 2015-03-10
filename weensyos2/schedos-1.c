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

#ifndef SHARE
#define SHARE 		10
#endif

#ifndef PRIORITY
#define PRIORITY	6
#endif

#define ALTERNATESYNC


void
start(void)
{
	int i;
	int to_print = PRINTCHAR;
	sys_setshare(SHARE);
	sys_setpriority(PRIORITY);
	sys_yield();

	
	for (i = 0; i < RUNCOUNT; i++) {
		// Write characters to the console, yielding after each one.
		#ifndef ALTERNATESYNC
			sys_write_char(to_print);
		#endif
		#ifndef ALTERNATESYNC
			while (atomic_swap(&write_lock, 1) != 0)
				continue;

			*cursorpos++ = PRINTCHAR;
			atomic_swap(&write_lock, 0);
		#endif ALTERNATESYNC
		sys_yield();
	}

	// Exercise 2
	// now processes exit, and don't yield forever
	sys_exit(0);

	// Yield forever.
	//while (1)
	//	sys_yield();
}
