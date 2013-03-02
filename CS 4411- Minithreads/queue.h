/*
 * Generic queue manipulation functions  
 */
#ifndef __QUEUE_H__
#define __QUEUE_H__

/*
 * PFany is a pointer to a function that can take two void* arguments
 * and return an integer.
 */
typedef int (*PFany)(void*, void*);

/*
 * queue_t is a pointer to an internally maintained data structure.
 * Clients of this package do not need to know how queues are
 * represented.  They see and manipulate only queue_t's. 
 */
typedef struct queue* queue_t;

/*
 * Global queue variables.
 */
queue_t readyQueue;
queue_t cleanupQueue;

/*
 * node_struct in an intemediary struct used to traverse the linked
 * list implementation of the queue. It allows access to the prev
 * and tail fields to find out what node comes before and follows
 * the current node.
 */
 typedef struct{
	void* prev;
	void* next;
	int tid;
 } node_struct;

/*
 * Return an empty queue. On error should return NULL.
 */
extern queue_t queue_new();

/*
 * Prepend a void* to a queue (both specifed as parameters).  Return
 * 0 (success) or -1 (failure).
 */
extern int queue_prepend(queue_t, void*);

/*
 * Appends a void* to a queue (both specifed as parameters).  Return
 * 0 (success) or -1 (failure).
 */
extern int queue_append(queue_t, void*);

/*
 * Dequeue and return the first void* from the queue. Return 0
 * (success) and first item if queue is nonempty, or -1 (failure) and
 * NULL if queue is empty.
 */
extern int queue_dequeue(queue_t, void**);

/*
 * Iterate the function parameter over each element in the queue.  The
 * additional void* argument is passed to the function as its first
 * argument and the queue element is the second.  
 * Return 0 (success) or -1 (failure).
 */
extern int queue_iterate(queue_t, PFany, void*);

/* 
 * Free the queue and return 0 (success) or -1 (failure).
 */
extern int queue_free (queue_t);

/*
 * Return the number of items in the queue.
 */
extern int queue_length(queue_t queue);

/* 
 * Delete the specified item from the given queue. 
 * Return -1 on error.
 */
extern int queue_delete(queue_t queue, void** item);

/*
 * Peek operation to look at the top of the queue without 
 * removing the head of the queue. 
 */
extern int queue_peek(queue_t queue, void** item);

#endif __QUEUE_H__
