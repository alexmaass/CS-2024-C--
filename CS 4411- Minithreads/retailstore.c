#include <stdlib.h>
#include <stdio.h>
#include "minithread.h"
#include "queue.h"
#include "synch.h"

typedef struct phone{
	void* prev;
	void* next;
	int serial_no;
}*phone_t;

int employee_unpack(int* arg);
int customer_getphone(int* arg);
int mainproc(int* arg);
int get_serial_no();
int phone_serial_no;
queue_t phoneQueue;
semaphore_t phoneSem;


int main(int argc, char **argv){
	phone_serial_no = 0;
	phoneSem = semaphore_create();
	semaphore_initialize(phoneSem, 0);
	minithread_system_initialize(mainproc, NULL);
}

int mainproc(int* arg)
{
	// Creates 10 employee unpacking threads
	for(int i=0; i<10; i++){
		minithread_t employee = minithread_fork(employee_unpack, NULL);
	}
	// Creates 10 customer threads.
	for(int i=0; i<10; i++){
		minithread_t customer = minithread_fork(customer_getphone, NULL);
	}
	// Once the threads have been created this thread will be used as the idle thread.
	while(1){
		minithread_yield();
	}
	return 0;
}

// The employee unpack function which unpacks phones to hand out to the customer. 
int employee_unpack()
{
	phone_t new_phone = (phone_t) malloc(sizeof(struct phone));
	new_phone -> serial_no = assign_serial_no();
	printf("Stocked: %d", new_phone->serial_no);
	queue_append(phoneQueue, new_phone);
	semaphore_V(phoneSem);
	return 0;
}

// The customer get function which prints out the phones which is handed out to the customer. 
int customer_getphone()
{
	phone_t new_phone = NULL;
	semaphore_P(phoneSem);
	queue_dequeue(phoneQueue, (void**)new_phone);
	printf("Gave out: %d", new_phone->serial_no);
	free(new_phone);
	return 0;
}

// The function used to assign a serial number. 
int assign_serial_no()
{
	return phone_serial_no++;
}