#ifndef __INTERRUPTS_PUBLIC_H_
#define __INTERRUPTS_PUBLIC_H_
/*
 * Provides a clean, virtualized interface to interrupts. 
 * 
 * We provide a virtual processor, which acts much like a
 * modern, real processor - it responds to interrupts, and
 * takes them on the current stack. 
 * 
 * Initially, when the system starts up, interrupts are disabled.
 * Calling minithreads_clock_init will start the clock device and
 * enable interrupts.
 *
 * YOU SHOULD NOT [NEED TO] MODIFY THIS FILE.
 */

#include "defs.h"


/*
 * PERIOD is the clock period in microseconds.  It is the interval at
 * which clock ticks will be sent
 */
#define SECOND 1000000
#define MILLISECOND 1000

/*define PERIOD */
#ifdef WINCE
/* Windows CE definitions */
#define PERIOD (100*MILLISECOND)
#else /* Windows NT definitions */
#define PERIOD (50*MILLISECOND)
#endif

/* a global variable to maintain time */
extern long ticks;

typedef void (*interrupt_handler_t)(void* );

/*
 * Virtual processor interrupt level (spl). 
 * Are interrupts enabled? A new interrupt will only be taken when interrupts
 * are enabled.
 */
typedef int interrupt_level_t;
extern interrupt_level_t interrupt_level; 

#define ENABLED 1
#define DISABLED 0

/*
 * Set the interrupt level to newlevel, return the old interrupt level
 * 
 * You should generally make changes to the interrupt level in a set/restore
 * pair. Be careful about restoring the interrupt level. Your 
 * protected code may be have been called with interrupts already 
 * disabled, in which case blindly reenabling interrupts will cause 
 * synchronization problems. Rather than downgrading the interrupts 
 * to a particular level without reference to the old value, you should 
 * generally use a set-and-restore scheme, as follows:

     interrupt_level_t l;
	   ...
	   l = set_interrupt_level(DISABLED);
	   ... [protected code]
	   set_interrupt_level(l);

 * this way, you are protected against nested interrupt downgrades (i.e. 
 * function A disables interrupts and calls B, which also disables them. If
 * B enables them, instead of setting the interrupt_level to its old value, 
 * interrupts will be enabled when B terminates, when A expected them to be
 * disabled. 
 *
 * the exception to this is when you're disabling interrupts before a call
 * to minithread_switch: the minithread switch code resets the interrupt
 * level to ENABLED itself.
 *
 * Note that you should minimize the amount of time interrupts are disabled
 * in order to reduce the impact on the real-time performance of your system.
 */
extern interrupt_level_t set_interrupt_level(interrupt_level_t newlevel);

/*
 * minithread_clock_init installs your clock interrupt service routine
 * h.  h will be called every PERIOD microseconds (defined above).
 * interrupts are disabled after minithread_clock_init finishes.
 * After you enable interrupts then your handler will be called
 * automatically on every clock tick.
 */
extern void minithread_clock_init(interrupt_handler_t clock_handler);

#endif  __INTERRUPTS_PUBLIC_H_
