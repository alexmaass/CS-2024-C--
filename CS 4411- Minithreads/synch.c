#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "synch.h"
#include "queue.h"
#include "minithread.h"

/*
 *	You must implement the procedures and types defined in this interface.
 */


/*
 * Semaphores.
 */
struct semaphore {
    queue_t blockedQueue;		// Queue of threads blocked by the semaphore
	int count;					// State of the semaphore
};

/*
 * semaphore_t semaphore_create()
 *	Allocate a new semaphore.
 */
semaphore_t semaphore_create() {
	semaphore_t sem= (semaphore_t) malloc(sizeof(struct semaphore));
	if (sem == NULL){
		return NULL;
	}
	else{
		sem -> blockedQueue = queue_new();
		sem -> count = 0;
		return sem;
	}
}

/*
 * semaphore_destroy(semaphore_t sem);
 *	Deallocate a semaphore.
 */
void semaphore_destroy(semaphore_t sem) {
	free(sem -> blockedQueue);
	sem -> count = 0;
	free(sem);
}

 
/*
 * semaphore_initialize(semaphore_t sem, int cnt)
 *	initialize the semaphore data structure pointed at by
 *	sem with an initial value cnt.
 */
void semaphore_initialize(semaphore_t sem, int cnt) {
	sem -> count = cnt;
}


/*
 * semaphore_P(semaphore_t sem)
 *	P on the sempahore. -1 on the count. 
 */
void semaphore_P(semaphore_t sem) {
	void** tmp = NULL;

	// Check to see the status of the semaphore. If status >= 0, decrement it.
	if ((sem -> count) > 0){
		(sem -> count)--;
	}
	// Status of the semaphore =< 0. Put the running thread on the blocked queue and start the next ready thread.
	else{
		queue_append((sem -> blockedQueue), runningThread);
		queue_dequeue(readyQueue, tmp);
		runningThread = (minithread_t)*tmp;
	}
	
}

/*
 * semaphore_V(semaphore_t sem)
 *	V on the sempahore. +1 on the count.
 */
void semaphore_V(semaphore_t sem) {
	void** tmp = NULL;
	(sem -> count)++;
	if ((sem -> count) >= 0){
		if ((sem -> blockedQueue) != NULL){
			queue_dequeue((sem -> blockedQueue), tmp);
			queue_append(readyQueue, tmp);
		}
	}
}
