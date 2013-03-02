/*
 * minithread.c:
 *	This file provides a few function headers for the procedures that
 *	you are required to implement for the minithread assignment.
 *
 *	EXCEPT WHERE NOTED YOUR IMPLEMENTATION MUST CONFORM TO THE
 *	NAMING AND TYPING OF THESE PROCEDURES.
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include "minithread.h"
#include "queue.h"
#include "synch.h"
#include "interrupts.h"
#include <assert.h>

//#define MTDEBUG

#ifdef MTDEBUG
void TRACE(char *format, ...) {
	char buffer[256];
	va_list args;
	va_start (args, format);
	vsprintf (buffer,format, args);
	fprintf(stderr, buffer);
	va_end (args);
}
#else
void TRACE(char *fmt, ...) {
	(void) fmt;
}
#endif

/*
 * A minithread should be defined either in this file or in a private
 * header file.  Minithreads have a stack pointer with to make procedure
 * calls, a stackbase which points to the bottom of the procedure
 * call stack, the ability to be enqueueed and dequeued, and any other state
 * that you feel they must have.
 */


typedef enum thread_state
{
   STATE_STOPPED,
   STATE_RUNNING,
   STATE_RUN_ON_DEMAND,
   STATE_DYING
} thread_state_t;

struct minithread
{
   int process_id;  //id number
   thread_state_t state;  //see enum above
   stack_pointer_t stacktop;  //stack pointer
   stack_pointer_t stackbase;  //base of stack
   int priority;
};

/* minithread functions */

minithread_t
minithread_fork(proc_t proc, arg_t arg) 
{
   minithread_t t = minithread_create(proc,arg);
   minithread_start(t);
   return t;
}

int final_proc(arg_t arg)
{
  TRACE("final_proc");
  queue_append(cleanupQueue, arg);
  runningThread->state=STATE_DYING;
  minithread_yield();

  return 0;
}

minithread_t
minithread_create(proc_t proc, arg_t arg)
{
   minithread_t t = (minithread_t) malloc(sizeof(*t));
   t->process_id = processcounter;
   processcounter++;
   t->state = STATE_STOPPED;
   t->priority = 0;
   minithread_allocate_stack(&t->stackbase, &t->stacktop);
   minithread_initialize_stack(&t->stacktop, proc, arg, final_proc, (arg_t)t);
   return t;
}

//process for cleanup thread
int cleanup(arg_t junk)
{
  while(1)
  {
    while(queue_length(cleanupQueue) > 0)
    {
     minithread_t thread_to_clean;
     queue_dequeue(cleanupQueue, (void**) &thread_to_clean);
     minithread_free_stack(thread_to_clean->stackbase);
    }
    minithread_yield();
  }
  return 0;
}


minithread_t
minithread_self()
{
   return runningThread;
}

int
minithread_id()
{
   return runningThread->process_id;
}

void
minithread_stop()
{
   runningThread->state = STATE_STOPPED;
   minithread_yield();
}

void
minithread_start(minithread_t t)
{
   t->state = STATE_RUNNING;
   queue_append(readyQueue,  t);
}

void
minithread_yield()
{
   interrupt_level_t intlevel = set_interrupt_level(DISABLED);
   minithread_t to_be_sched;
   minithread_t old_current = runningThread;
   /* NEED TO QUEUE BEFORE DEQUEUE IF WE'RE THE ONLY THREAD */
   if(runningThread->state==STATE_RUNNING)
   {
      TRACE("Descheduling %x (for later)\n", runningThread);
	   queue_append(readyQueue,  runningThread);
   }
   else
      TRACE("Descheduling %x (permanently)\n", runningThread);

	   /*This is the scheduler, which decides which level of the queue to
	     access based on a random number deciding between weighted options*/
	   queue_dequeue(readyQueue, &to_be_sched);


   TRACE("Scheduling %x\n", to_be_sched);
   runningThread = to_be_sched;
   
   minithread_switch(&old_current->stacktop, &to_be_sched->stacktop);
      
}

/*
 * Initialization.
 *
 * 	minithread_system_initialize:
 *	 This procedure should be called from your C main procedure
 *	 to turn a single threaded UNIX process into a multithreaded
 *	 program.
 *
 *	 Initialize any private data structures.
 * 	 Create the idle thread.
 *       Fork the thread which should call mainproc(mainarg)
 * 	 Start scheduling.
 *
 */



void
minithread_system_initialize(proc_t mainproc, arg_t mainarg) {
	processcounter = 1;
	runningThread= NULL;
	cleanup_thread= NULL; 
	idle_thread= NULL;



  //sgenrand((long)currentTimeMillis());
  readyQueue = queue_new();
  cleanupQueue = queue_new();
//  minithread_clock_init(clock_handler);
  
  idle_thread = (minithread_t) malloc(sizeof(*idle_thread));
  idle_thread->process_id = 0;
  idle_thread->state = STATE_RUNNING;
  idle_thread->stackbase = NULL;
  idle_thread->stacktop = NULL;
  idle_thread->priority = 3;


  cleanup_thread= minithread_create(cleanup, NULL);

  cleanup_thread->state = STATE_RUN_ON_DEMAND;
  runningThread= idle_thread;
  set_interrupt_level(ENABLED);  
  minithread_fork(mainproc, mainarg);
  
  TRACE("Init done.\nidle thread=%x\ncleanup thread=%x\n", idle_thread, cleanup_thread);

  while(1){minithread_yield();}
}

