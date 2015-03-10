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

#ifndef RANDSEED
#define RANDSEED	0xACE1u
#endif

unsigned short seed = RANDSEED;

int priority_rand() {
	unsigned randbit = ((seed >> 0) ^ (seed >> 2) ^ (seed >> 3) ^ (seed >> 5)) &  1;
	return (seed = (seed >> 1) | (randbit << 15));
}

void
start(void)
{
	int i;
	int to_print = PRINTCHAR;
	int priority = 0;
	// update priority
	while (!(priority = priority_rand() % NPROCS));

	sys_setshare();
	sys_setpriority(priority);
	
	for (i = 0; i < RUNCOUNT; i++) {
		// Write characters to the console, yielding after each one.
		sys_write_char(to_print);
	}

	// Exercise 2
	// now processes exit, and don't yield forever
	sys_exit(0);

	// Yield forever.
	//while (1)
	//	sys_yield();
}
