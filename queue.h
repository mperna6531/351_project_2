#pragma once
#include "process.h"

struct procQueue
{
	int capacity;
	int size;
	int front;
	int rear;
	Process **elements;
};

procQueue* createProcQueue(int length)
{
	procQueue *q;

	q = malloc(sizeof(procQueue));

	q->elements = malloc(sizeof(Process) * length);
	q->size = 0;
	q->capacity = length;
	q->front = 0;
	q->rear = -1;

	return q;
}

void enqueueProc(procQueue* q, Process* proc)
{
	if (q->size == q->capacity)
	{
		printf("ERROR: queue is full!\n");

		exit(2);
	}

	q->size++;
	q->rear = q->rear + 1;

	if (q->rear == q->capacity)
	{
		q->rear = 0;
	}

	q->elements[q->rear] = proc;
}

int queueNext(procQueue* q)
{
	if (!queueNext(q))
	{
		printf("ERROR: queue is empty\n");
		exit(2);
	}

	return q->elements[q->front];
}

Process* peekQueueAtIndex(procQueue* q, int index)
{
	return q->elements[index];
}

void dequeueProc(procQueue* q)
{
	if (!queueNext(q))
	{
		printf("ERROR: queue is empty\n");
		exit(2);
	}

	q->size--;
	q->front++;

	if (q->front == q->capacity)
	{
		q->front = 0;
	}
}

int iterateQueueIndex(procQueue* q, int index) {
	return q->front + index;
}

void dequeueProcIndex(procQueue* q, int index) {
	int prev = 0;

	for (int i = 0; i < q->size; i++) {
		if (i > index) {
			q->elements[prev] = q->elements[i];
		}

		prev = i;
	}

	q->size--;
	q->rear--;
}

void printProcQueue(procQueue* q) 
{
	Process* proc;

	printf("\tInput queue: [ ");
	for (int counter = 0; counter < q->size; counter++) {
		proc = peekQueueAtIndex(q, iterateQueueIndex(q, counter));

		printf("%d ", proc->pid);
	}
	printf("]\n");
}