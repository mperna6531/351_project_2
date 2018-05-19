#pragma once
#include "process.h"
#include <vector>
#include <iostream>

class ProcessQueue
{
private:
	int capacity_;
	int size_;
	int front_;
	int rear_;
  
  std::vector<Process> elements_;
public:
  int rear() { return rear_; }
  int size() { return size_; }
  int front() { return front_;}
  void enqueue(Process proc);
  ProcessQueue(int length);
};

ProcessQueue::ProcessQueue(int length) : 
  capacity_(length), 
  size_(0), 
  front_(0), 
  rear_(-1) {}

void ProcessQueue::enqueue(Process proc) {
  if (size_ == capacity_) {
		std::cout << "ERROR: queue is full!\n";
      exit(1);
	}

	++size_;

	if(++rear_ == capacity_)
    rear_ = 0;

	elements_.insert(elements_.begin() + rear_, proc);
}

int ProcessQueue::queue_next() {
  if 
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