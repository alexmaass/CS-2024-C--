/*   
 * Provides a clean, virtualized interface to interrupts. 
 * 
 * Windows provides a broken, purely synchronous model for signals.
 * This interface allows us to simulate a set of virtual interrupts
 * that closely match the asynchronous operation of real 
 * processor interrupts.
 *
 * This file also implements the virtual clock device.
 * 
 * YOU SHOULD NOT [NEED TO] MODIFY THIS FILE.
 */

/* get the correct version of Windows NT recognised */
#define _WIN32_WINNT 0x0400
#include <windows.h>
#include <winsock.h>

#include <stdio.h>
#include <setjmp.h>

#include "defs.h"
#include "interrupts_private.h"
#include "machineprimitives.h"

#ifdef WINCE
#define EIP context.Pc
#define ESP context.Sp
#define REG1 context.R0
#define REG2 context.R1
#else
#if defined(_M_IX86)
#define EIP context.Eip
#define ESP context.Esp
#define REG1 context.Ecx
#define REG2 context.Edx
#elif defined(_M_AMD64)
#define EIP context.Rip
#define ESP context.Rsp
#define REG1 context.Rcx
#define REG2 context.Rdx
#else
#error "Arch not supported?"
#endif
#endif

/* a global variable to maintain time */
long ticks;

typedef struct signal_queue_t signal_queue_t;
struct signal_queue_t {
  HANDLE threadid;
  CONTEXT *context;
  int intnumber;
  signal_queue_t *next;
};

/* threads which have finished servicing their interrupts */ 
static signal_queue_t *signalq = NULL;

/* return thread waits on this semaphore to do an interrupt return */
static HANDLE cleanup = NULL;

/* 
 * handle that protects the globals in this package, including
 * the queue of threads that are handling simulated interrupts,
 * as well as the main threads stack.
 */
static HANDLE mutex = NULL;

/*
 * Virtual processor interrupt level (spl). 
 * Are interrupts enabled? A new interrupt will only be taken when interrupts
 * are enabled.
 */
interrupt_level_t interrupt_level;

typedef struct interrupt_queue_t interrupt_queue_t;
struct interrupt_queue_t {
  int type; 
  interrupt_handler_t handler;
  interrupt_property_t property;
  interrupt_queue_t* next;
};

/* 
 * Dummy routines to prevent unwanted preemption: when a timer event occurs,
 * we only preempt if the minithread which is running is at an address between
 * start() and end(), which enclose all the minithread and user-supplied code.
 * In this way we protect the NT/CE-supplied libraries, which are not
 * "minithread-safe" (though they are NT/CE-thread-safe).
 */
#ifdef WINCE
extern unsigned int start(void);
extern unsigned int end(void);
#elif defined(_M_IX86)
extern unsigned long start(void);
extern unsigned long end(void);
#elif defined(_M_AMD64)
extern unsigned __int64 start(void);
extern unsigned __int64 end(void);
#endif

/* Code outside these addresses belongs to the operating system */
#ifdef WINCE
static unsigned int start_address;
static unsigned int end_address;
#elif defined(_M_IX86)
static unsigned long start_address;
static unsigned long end_address;
#elif defined(_M_AMD64)
static unsigned __int64 start_address;
static unsigned __int64 end_address;
#endif

/* Markers for loopforever */
#ifdef WINCE
static unsigned int loopforever_start_address;
static unsigned int loopforever_end_address;
#elif defined(_M_IX86)
static unsigned long loopforever_start_address;
static unsigned long loopforever_end_address;
#elif defined(_M_AMD64)
static unsigned __int64 loopforever_start_address;
static unsigned __int64 loopforever_end_address;
#endif

static HANDLE clock_thread;   /* NT thread to check for timer events */
#ifdef WINCE
HANDLE system_thread; /* need external access to system thread in wince.c. Ugly but necessary */
#else
static HANDLE system_thread;  /* NT thread for running the minithreads */
#endif
static HANDLE return_thread;  /* NT thread for restarting system thread */

static interrupt_queue_t* interrupt_queue = NULL;

static int pid;

/* is the main thread ready to be moved back to previous state by the
   interrupt return assist thread ? */
static int readytoreturn = 0;

interrupt_level_t set_interrupt_level(interrupt_level_t newlevel) {
  if (DEBUG)
    kprintf("Set interrupt level to %d.\n", newlevel);
  return swap(&interrupt_level, newlevel);
}

#ifdef WINCE
unsigned int loopforever_start(void) {  
#elif defined(_M_IX86)
unsigned long loopforever_start(void) {  
#elif defined(_M_AMD64)
unsigned __int64 loopforever_start(void) {  
#else
#error "Arch not supported?"
#endif
  jmp_buf buf;

  setjmp(buf);
#ifdef WINCE
  /* for ARM processor PC is in position 11 in jmp_buf */
  return buf[10];
#elif defined(_M_IX86)
  /* for x86 processor PC is in position 6 in jmp_buf */
  return buf[5];
#elif defined(_M_AMD64)
  /* except on x86_64 */
  return buf[5].Part[0];
#endif
}

void loopforever() {
  for(;;)
    ;
  /* NOT REACHED */
  exit(1);
}

#ifdef WINCE
unsigned int loopforever_end(void) {  
#elif defined(_M_IX86)
unsigned long loopforever_end(void) {  
#elif defined(_M_AMD64)
unsigned __int64 loopforever_end(void) {  
#else
#error "Arch not supported?"
#endif
  jmp_buf buf;

  setjmp(buf);
#ifdef WINCE
  /* for ARM processor PC is in position 11 in jmp_buf */
  return buf[10];
#elif defined(_M_IX86)
  /* for x86 processor PC is in position 6 in jmp_buf */
  return buf[5];
#elif defined(_M_AMD64)
  /* except on x86_64 */
  return buf[5].Part[0];
#endif
}

/* run the user's supplied interrupt handler and clean up the state after it:
   we wake up the return thread and then loop indefinitely; it overwrites our
   execution context to set us back to where we were before the interrupt
   if the argument "arg" is NULL, it's a clock interrupt; otherwise, it's a
   pointer to a network_interrupt_arg_t, which contains information about the
   packet which has arrived.
   
   disables interrupts before passing control to the return thread.
   */
void __fastcall receive_interrupt(CONTEXT* context, int type, void* arg) {
  signal_queue_t* sq;
  interrupt_queue_t* interrupt_info;

  readytoreturn = FALSE;

  if (DEBUG)
    kprintf("SYS:Running user interrupt handler, context = 0x%x, arg = 0x%x.\n",
	   context, arg);
  
  /* find the appropriate handler */
  interrupt_info = interrupt_queue;
  while (interrupt_info!=NULL && interrupt_info->type!=type)
    interrupt_info = interrupt_info->next;

  if (interrupt_info == NULL) {
    /* we couldn't find the interrupt with type "type" so we crask the
       sistem.
    */
    kprintf("An interrupt of the unregistered type %d was received. Crashing.\n ",
	   type);
    exit(-1);
  } else {
    /* now, call the appropriate interrupt handler */
    if (DEBUG)
      kprintf("SYS:interrupt of type %d.\n", type);
    if (interrupt_info->handler != NULL)
      interrupt_info->handler(arg); 
  }
  
  WaitOnObject(mutex);
  assert(signalq == NULL);

  sq = (signal_queue_t *) malloc(sizeof(signal_queue_t));
  sq->context = context;
  sq->threadid = system_thread;

  sq->next = signalq;
  signalq = sq;

  if (interrupt_level == ENABLED) {
    if (DEBUG)
      kprintf("SYS:disabling interrupts in handler.\n");
    interrupt_level = DISABLED;
  }  
  ReleaseMutex(mutex);

  /* 
   * This is correct but not elegant (it's a kluge forced on us by Windows).
   * We need an atomic way to wake up the
   * assist thread and go to sleep ourselves. One could use SignalAndWait
   * here, but SignalAndWait is not supported on CE 
   */
  ReleaseSemaphore(cleanup, 1, NULL); /* tell the return thread to run */

  readytoreturn = TRUE;

  /* if (DEBUG) kprintf("SYS:Looping forever.\n"); */
  loopforever();
}

/* this thread assists with the returns from simulated interrupts
   
   ideally, the interrupthandler would just assume the state right before the
   interrupt, but this is not possible given the x86 and nt, so the interrupt
   handling thread has to suspend itself, and let some other thread manipulate
   its state while it is suspended. 
   
   when this thread runs, interrupts will be disabled, so we reenable them
   once it is finished.
   */
DWORD WINAPI interrupt_return_assist(LPVOID ptr) {
  signal_queue_t *sq;
  int scount, rcount;
  int success = FALSE;
  CONTEXT context;

  if (DEBUG)
    kprintf("IRA:starting ...\n");

  for(;;) {
    /* wait politely until we are needed */
    WaitOnObject(cleanup);

    if (DEBUG)
      kprintf("IRA:woken up ...\n");
	
    WaitOnObject(mutex);

    {
      success = FALSE;
      sq = signalq;
      signalq = signalq->next;
      assert(signalq == NULL);

      /* wait for the system thread to enter a "safe" state */
      while(readytoreturn != TRUE)
	SwitchToThread();

      while(success != TRUE) {

	scount = SuspendThread(sq->threadid);
	memset(&context, 0, sizeof(CONTEXT));

#ifdef WINCE
	context.ContextFlags = CONTEXT_FULL;
#else
	context.ContextFlags = 
	  CONTEXT_FULL | CONTEXT_FLOATING_POINT | CONTEXT_DEBUG_REGISTERS;
#endif

	AbortOnError(GetThreadContext(system_thread, &context));

	if (DEBUG)
	  kprintf("IRA:system thread is at 0x%x.\n", EIP);
	if((EIP >= loopforever_start_address && 
	    EIP <= loopforever_end_address)) {
	  if (DEBUG)
	    kprintf("IRA:enabling interrupts.\n");
	  interrupt_level = ENABLED;

	  AbortOnError(SetThreadContext(sq->threadid, sq->context));

	  rcount = ResumeThread(sq->threadid);

	  if (DEBUG)
	    kprintf("IRA:Interrupt return assist scount %d rcount %d\n", 
		   scount, rcount);

	  assert(rcount >= scount + 1);
	  success = TRUE;
	} 
	else {
	  ResumeThread(system_thread);
	  ReleaseMutex(mutex);
	  SwitchToThread();
	  WaitOnObject(mutex);
	}
      }
    } 
    ReleaseMutex(mutex);
    free(sq);
  }

  /* never reached */
  return 0;
}

/* 
 * Send an interrupt to the system thread: adjust its stack to call
 * the appropriate interrupt handler with the specified argument. the
 * "type" argument identifies interrupt-specific processing which must
 * be done, in particular, what we should do if interrupts are
 * disabled or the system thread is in a non-preemptable state
 * (e.g. executing a system library function). clock interrupts are
 * dropped, network interrupts are deferred. this function replaces
 * code which used to be common to clock_poll and network_poll.
*/
void send_interrupt(int type, void* arg) {
  CONTEXT context;
  int safe_to_proceed = 0;
  int drop_interrupt = 0;
  interrupt_queue_t* interrupt_info = NULL;

  for (;;) {
    WaitOnObject(mutex);

    /* need to protect this code: we only activate the interrupt if interrupts
       are actually enabled, but we also need to ensure they are not disabled
       after we test -- so we suspend the system thread first.
       */
    SuspendThread(system_thread);
    memset(&context, 0, sizeof(CONTEXT));
#ifdef WINCE
    context.ContextFlags = CONTEXT_FULL;
#else
    context.ContextFlags = 
      CONTEXT_FULL | CONTEXT_FLOATING_POINT | CONTEXT_DEBUG_REGISTERS;
#endif
    /* Warning: a printf here makes the system lock */

    AbortOnError(GetThreadContext(system_thread, &context));

    /* find interrupt description in the interrupt queue */
    interrupt_info = interrupt_queue;
    while (interrupt_info!=NULL && interrupt_info->type!=type)
      interrupt_info = interrupt_info->next;
    
    if (interrupt_info == NULL) {
      /* 
       * we couldn't find the interrupt with type "type" so we crash the
       * sistem.
      */

      kprintf("An interrupt of the unregistered type %d was received.\n",
	     type);
      AbortOnCondition(1,"Crashing.");
    } else {
      /*
       * interrupt-type-specific processing: can we interrupt now? If we 
       * ended up interrupting the process at a time when it is in some non-minithread-safe
       * Windows library, then defer or drop the interrupt.
       */
      switch (interrupt_info->property){
      case INTERRUPT_DROP:
	if (interrupt_level == DISABLED
	    || (EIP < start_address)
	    || (EIP > end_address)) {
	  drop_interrupt = 1;
	}
	else
	  safe_to_proceed = 1;
	break;
      case INTERRUPT_DEFER:
	if (interrupt_level == ENABLED 
	    && (EIP >= start_address)
	    && (EIP <= end_address)) {
	  interrupt_level = DISABLED;
	  safe_to_proceed = 1;
	}
	break;
      default:
	break;
      }
    } 
     
    if (safe_to_proceed == 1)
      break;
    else {
      ResumeThread(system_thread);
      ReleaseMutex(mutex);
      if (DEBUG) {
	switch (interrupt_info->property) {
	case INTERRUPT_DROP:
	  kprintf("Interrupt of type %d dropped, eip = 0x%x.\n", 
		 interrupt_info->type, EIP);
	  break;
	case INTERRUPT_DEFER:
	  kprintf("Interrupt of type %d deffered, eip = 0x%x.\n", 
		 interrupt_info->type, EIP);
	  break;
	}
      }
    }

    if (drop_interrupt == 1)
      return;
    else
      SwitchToThread();
  }

  /* now fix the system thread's stack so it runs run_user_handler */
  {
    int stack;
    int oldpc = 0;

    if (DEBUG) {
      kprintf("Interrupts are enabled, system thread pc = 0x%x\n", EIP);
      kprintf("Adjusting system thread context ...\n");
    }

    /* set the interrupt number */
    /* arg->intnumber = ++intnumber; */

    oldpc = EIP;
    stack = ESP;	/* Esp == extended stack pointer */

    /* safe to do a printf because other thread is stunned in user code */
    if (DEBUG)
      kprintf("Suspended system thread, adjusting stack, sp = 0x%x\n", stack);

    stack -= (sizeof(CONTEXT) + 64); /* 64 is slop */
    memcpy((int *) stack, &context, sizeof(CONTEXT));

    EIP = (int) ((void *) receive_interrupt);
    REG1 = stack; /*pointer to context*/    
    REG2 = (int) type; /*type, second argument */
#ifndef WINCE
    /* for x86 put arg pointer on the stack since only two 
       parameters can be passed in registers.
    */
    stack-=sizeof(void*);
    *((int*)stack)=(int) arg;
    stack-=sizeof(void*); 
#else
    /* for ARM put the third argument in R2 */
    context.R2 = (int) arg;
#endif
    ESP = stack;
    
    AbortOnError(SetThreadContext(system_thread, &context));

    AbortOnError(GetThreadContext(system_thread, &context));
    
    ResumeThread(system_thread);
  }
  ReleaseMutex(mutex);
}

/* procedure to poll the event queue for timer events, run by the clock
   thread; on a timer event, call "send_interrupt()" to run the system thread's
   clock handler routine 
*/
DWORD WINAPI clock_poll(LPVOID arg) {
#ifdef WINCE
  Sleep(PERIOD/1000); /* sleep requires time in milliseconds */
  send_interrupt(CLOCK_INTERRUPT_TYPE, NULL);
#else
  LARGE_INTEGER i;
  HANDLE timer; 
  /* HANDLE thread = GetCurrentThread(); */
  char name[64];

  sprintf(name, "timer %d", pid);
  timer = CreateWaitableTimer(NULL, TRUE, name);
  assert(timer != NULL);

  for (;;) {
    i.QuadPart = -PERIOD*10; /* NT timer values are in hundreds of nanoseconds */
    AbortOnError(SetWaitableTimer(timer, &i, 0, NULL, NULL, FALSE));

    if (WaitForSingleObject(timer, INFINITE) == WAIT_OBJECT_0) {
      if (DEBUG)
	kprintf("CLK: clock tick.\n");
      send_interrupt(CLOCK_INTERRUPT_TYPE, NULL);
    }
  }
#endif
  /* never reached */
  return 0;

}

/*
 * Setup the interval timer and install user interrupt handler.  After this
 * routine is called, and after you call set_interrupt_level(ENABLED), clock
 * interrupts will begin to be sent.  They will call the handler
 * function h specified by the caller.
 */
void minithread_clock_init(interrupt_handler_t clock_handler)
{
#ifndef WINCE
  HANDLE process;
#endif
  DWORD id;
  char name[64];
 
  if (clock_handler == NULL) {
    kprintf("Must provide an interrupt handler, interrupts not started.\n");
    return;
  }

  kprintf("Starting clock interrupts.\n");

  /* set values for *start_address and *stop_address */
  start_address = start();
  end_address = end();
  loopforever_start_address = loopforever_start();
  loopforever_end_address = loopforever_end();
  
  if (DEBUG){
    kprintf("start_address=%x\tend_address=%x\n",
	    start_address, end_address);
    kprintf("loop_start_address=%x\tloop_end_address=%x\n",
	    loopforever_start_address, loopforever_end_address);
  }
  pid = GetCurrentProcessId();

  /* set up the signal queue and semaphores */
  sprintf(name, "interrupt mutex %d", pid);
  mutex = CreateMutex(NULL, FALSE, name);

  /* use this semaphore as a condition variable */
  sprintf(name, "interrupt return semaphore %d", pid);
  cleanup = CreateSemaphore(NULL, 0, 10, name);

  interrupt_level = DISABLED;

  register_interrupt(CLOCK_INTERRUPT_TYPE, clock_handler, INTERRUPT_DROP);

#ifndef WINCE
  /* overcome an NT "feature" -- GetCurrentThread() returns a "pseudohandle" to
     the executing thread, not the real handle, so we need to use 
     DuplicateHandle() to get the real handle.
     */
  process = GetCurrentProcess();
  AbortOnError(DuplicateHandle(process, GetCurrentThread(), process, 
		      &system_thread, THREAD_ALL_ACCESS, FALSE, 0));

#else 
  /* system_thread already set properly in WinMain */
#endif
  
  /* create clock and return threads, but discard ids */
  clock_thread = CreateThread(NULL, 0, clock_poll, NULL, 0, &id); 
  assert(clock_thread != NULL);

  return_thread = CreateThread(NULL, 0, interrupt_return_assist, NULL, 0, &id);
  assert(return_thread != NULL);
}

int register_interrupt(int type, interrupt_handler_t handler, 
		       interrupt_property_t property){
  interrupt_queue_t* new_interrupt, *interrupt_info;
  int error=0;
  interrupt_level_t old_interrupt_level;

  kprintf("Registering interrupt of type %d.\n",type);

  /* disable interrupts not to have surprises */
  old_interrupt_level = set_interrupt_level(DISABLED);
  
  /* look for an interrupt of the desired type */
  interrupt_info = interrupt_queue;
  while (interrupt_info!=NULL && interrupt_info->type!=type)
    interrupt_info = interrupt_info->next;
  
  if (interrupt_info != NULL) {
    /* interrupt already exists, return error */
    error=-1;
    kprintf("An interrupt of this type already registered.\n");
  } else {
    new_interrupt = (interrupt_queue_t*) malloc(sizeof(interrupt_queue_t));
    new_interrupt->type = type;
    new_interrupt->handler = handler;
    new_interrupt->property = property;

    /* insert it in the queue */
    new_interrupt->next = interrupt_queue;
    interrupt_queue = new_interrupt;
  }

  /* put interrupts in their previous state */
  (void)set_interrupt_level(old_interrupt_level);

  return error;
}


