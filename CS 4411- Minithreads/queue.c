/*
 * Generic queue implementation.
 */
#include "queue.h"
#include "minithread.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * Struct definition for queue. 
 */ 
struct queue{
	void* head;
	void* tail;
};

/*
 * Return an empty queue.
 */
queue_t queue_new() {
	queue_t qt = (queue_t) malloc(sizeof(struct queue));
	if (qt == NULL){
		return NULL;
	}
	else{
		(qt -> head) = NULL;
		(qt -> tail) = NULL;
		return qt;
	}
}

/*
 * Peek operation to look at the top of the queue without 
 * removing the head of the queue. 
 * Return 0 if successful, -1 if failed.
 */
 int queue_peek(queue_t queue, void** item){
	if (queue == NULL){
		return -1;
	}
	else{
		item = &(queue -> head);
		return 0;
	}
 }


/*
 * Prepend a void* to a queue (both specifed as parameters).  Return
 * 0 (success) or -1 (failure).
 */
int queue_prepend(queue_t queue, void* item) {
	node_struct* tmp;
	// Check to see if either the queue or item is null.
	if (item == NULL || queue == NULL){
		return -1;
	}
	// Base case: empty queue so set head to be pointing to the item.
	if ((queue -> head) == NULL){
		(queue -> head) = item;
		(queue -> tail) = item;
		return 0;
	}
	// Queue is not empty: set previous head to be next in the list after item.
	else{
		tmp = (node_struct*) (item);
		(tmp -> next) = (queue -> head);
		(tmp -> prev) = NULL;
		item = (void*) tmp;
		(queue -> head) = item;
		return 0;
	} 
	return -1;
}

/*
 * Append a void* to a queue (both specifed as parameters). Return
 * 0 (success) or -1 (failure). 
 */
int queue_append(queue_t queue, void* item) {
	node_struct* tmp;
	// Check to see if either queue or item are null.
	if (queue == NULL || item == NULL){
		return -1;
	}
	// Base Case: Queue is empty.
	if ((queue -> head) == NULL){
		(queue -> head) = item;
		(queue -> tail) = item;
		return 0;
	}
	// Queue is not empty.
	else{
		// Set the prev pointer in item to the original tail
		tmp = (node_struct*) &item;
		(tmp -> prev) = (queue -> tail);
		(tmp -> next) = NULL;
		item = (void*) tmp;
		// Set the next pointer in the original tail to item
		((node_struct*)(queue -> tail)) -> next = item;
		queue -> tail = item;
		return 0;
	}
	return -1;
}

/*
 * Dequeue and return the first void* from the queue or NULL if queue
 * is empty.  Return 0 (success) or -1 (failure).
 */
int queue_dequeue(queue_t queue, void** item) {
	node_struct* tmp;
	// Check to see if the queue is null.
	if (queue == NULL){
		return -1;
	}
	// Base case: queue is empty so return null.
	if ((queue -> head) == NULL){
		*item = NULL;
		return -1; 
	}
	// Queue is not empty. Pop off the head of the queue. 
	else{
		*item = queue -> head;
		tmp = (node_struct*) (queue -> head);
		(queue -> head) = tmp -> next;	
		return 0;
	}
	return -1;
}

/*
 * Iterate the function parameter over each element in the queue. The
 * additional void* argument is passed to the function as its first
 * argument and the queue element is the second.  Return 0 (success)
 * or -1 (failure).
 */
int queue_iterate(queue_t queue, PFany f, void* item) {
	void* tmp;
	// Check to see if the arguments are valid
	if (queue == NULL || f == NULL){
		return -1;
	}
	// Set the tmp variabe to look at the head of the queue
	tmp = (queue -> head);
	while (tmp != NULL){
		// Perform the operation on the item.
		if ( f(item, tmp) != 0){
			return -1;
		}
		tmp = (void*)(((node_struct*)tmp) -> next);
	}
	return 0;
}

/*
 * Free the queue and return 0 (success) or -1 (failure).
 */
int queue_free (queue_t queue) {
	free(queue);
	return 0;
}

/*
 * Return the number of items in the queue.
 */
int queue_length(queue_t queue) {
	unsigned int count;
	node_struct* tmp;
	// Base case: empty queue.
	count = 0;
	// Base case: 1 item in queue
	if ((queue -> head) != NULL){
		count++;
	}
	// More than 1 item in queue. 
	tmp = (node_struct*)(((node_struct*)(queue -> head)) -> next); 
	while (tmp != NULL){
		count++;
		tmp = (node_struct*)(tmp -> next); 
	}
	return count;
}


/* 
 * Delete the specified item from the given queue. 
 * Return -1 on error.
 */
int queue_delete(queue_t queue, void** item) {
	node_struct* current;
	node_struct* before;
	node_struct* after;
	// Check to see if queue and item are valid. 
	if (queue == NULL || item == NULL){
		return -1;
	}
	// If the queue is empty, it fails.
	if (queue-> head == NULL){
		return -1;
	}
	else{
		current = (node_struct*)(queue -> head);
	}
	// Iterate through the queue. 
	while ((current -> tid) != (((node_struct*)(&item)) -> tid) && current != NULL){
		current = (node_struct*)(current -> next);
	}
	// Match was found. Remove the node and link the before and after nodes.
	if (current != NULL){
		before = (node_struct*)(current -> prev);
		after = (node_struct*)(current -> next);
		before -> next = after;
		after -> prev = before;
		free((void*) current);
		return 0;
	}
	// Match not found. Error occured. 
	else{
		return -1;
	}
	
}
