// CPSC 351 Assignment 2.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <memory>
#include <queue>

#include "process.h"
#include "queue.h"
#include "memory.h"
#include "prototypes.h"

const int timeMax = 100000;
int numOfProcs = 0, lastAnnouncement = -1;
Process *proc_list;
std::unique_ptr<std::queue<Process>> pq;
std::unique_ptr<FrameList> fl;

void main_loop() {
	long currentTime = 0;
	while (1) {
		(currentTime);

		(currentTime);

		assignAvailableMemoryWaitingProcs(currentTime);

		currentTime++;

		if (currentTime > timeMax)
		{
			printf("DEADLOCK: max time reached\n");
			break;
		}

		if (queue->size_ == 0 && fl->empty())
			break;
		
	}

	printTurnaroundTime();
}

int main() {
	int pageSize = 0;
	int memSize = 0;

	char* filePath = malloc(100 * sizeof(char));

	getUserInput(&memSize, &pageSize, filePath);

	procList = assignProcessList(filePath);

	queue = createProcQueue(numOfProcs);

	framelist = createFrameList(memSize / pageSize, pageSize);

	mainLoop();

	return 0;
}

void enqueueNewArrivedProces(int currentTime){
	Process* proc;

	for (int i = 0; i < numOfProcs; i++) {
		proc = &procList[i];

		if (proc->arrivalTime == currentTime) {
			printf("%sProcess %d arrives\n",
				getAnnouncementPrefix(currentTime),
				proc->pid);

			enqueueProc(queue, proc);

			printProcQueue(queue);
			printFrameList(framelist);
		}
	}
}

void terminateCompletedProcs(int currentTime) 
{
	int i, time_spent_in_memory;
	Process* proc;

	// dequeue any procs that need it
	for (i = 0; i < numOfProcs; i += 1) {
		proc = &procList[i];
		time_spent_in_memory = currentTime - proc->timeAddMemory;

		if (proc->isActive && (time_spent_in_memory >= proc->lifeTime)) {
			printf("%sProcess %d completes\n",
				getAnnouncementPrefix(currentTime),
				proc->pid);

			proc->isActive = 0;
			proc->timeDone = currentTime;

			freeMemoryPID(framelist, proc->pid);

			printFrameList(framelist);
		}
	}
}

void assignAvailableMemoryWaitingProcs(int currentTime)
{
	int i, index, limit;
	Process* proc;

	limit = queue->size;

	// enqueue any procs that can be put into mem
	for (i = 0; i < limit; i += 1) {
		index = iterateQueueIndex(queue, i);
		proc = queue->elements[index];

		if (procFitMemory(framelist, proc)) {
			printf("%sMM moves Process %d to memory\n",
				getAnnouncementPrefix(currentTime),
				proc->pid);

			fitProcMemory(framelist, proc);

			proc->isActive = 1;
			proc->timeAddMemory = currentTime;

			dequeueProcIndex(queue, i);
			printProcQueue(queue);
			printFrameList(framelist);
		}
	}
}

char* getAnnouncementPrefix(int currentTime) 
{
	char* result;

	result = malloc(20 * sizeof(char));

	if (lastAnnouncement == currentTime) {
		sprintf(result, "\t");
	}
	else {
		sprintf(result, "t = %d: ", currentTime);
	}

	lastAnnouncement = currentTime;

	return result;
}

void print_turnaround_times() 
{
	float total = 0;

	for (int i = 0; i < numOfProcs; i += 1) {
		total += procList[i].timeDone - procList[i].arrivalTime;
	}

	printf("Average Turnaround Time: %2.2f\n", total / numOfProcs);
}

int multipleOneHundred(int t) 
{
	return (t % 100) == 0 ? 1 : 0;
}

int isOneTwoOrThree(int t)
{
	return (t >= 1 && t <= 3) ? 1 : 0;
}

void clearStdin(char* buf) 
{
	if (buf[strlen(buf) - 1] != '\n') {
		int ch;
		while (((ch = getchar()) != '\n') && (ch != EOF));
	}
}

int processNumericInputFromUser(const char* output, int(*func)(int))
{
	char buf[10];
	int success = 0;
	int res = 0;

	while (!success) {
		printf("%s: ", output);

		if (fgets(buf, 10, stdin) == NULL) {
			clearStdin(buf);
			printf("ERROR: You didn't enter any data!\n");

			continue;
		}

		if (sscanf(buf, "%d", &res) <= 0) {
			clearStdin(buf);
			printf("ERROR: You didn't enter a number!\n");

			continue;
		}

		if (!(success = (*func)(res))) {
			clearStdin(buf);
			printf("ERROR: That number is not a valid choice\n");
		}
	}

	return res;
}

void promptForFileName(char* res) 
{
	char buf[100];
	FILE* fp;

	while (1) {
		printf("Input file: ");

		if (fgets(buf, 100, stdin) == NULL) {
			clearStdin(buf);
			printf("ERROR: You didn't enter any data!\n");

			continue;
		}

		if (sscanf(buf, "%s", res) <= 0) {
			clearStdin(buf);
			printf("ERROR: You didn't enter a string!\n");

			continue;
		}

		if (!(fp = fopen(res, "r"))) {
			perror("ERROR: Could not open file!\n");
		}
		else {
			break;
		}
	}
}

// prompts for memory size and page size
void getUserInput(int* mem, int* page, char* filePath) 
{
	while (1) {
		*mem = processNumericInputFromUser(
			"Memory size", multipleOneHundred);

		*page = processNumericInputFromUser(
			"Page size (1: 100, 2: 200, 3: 400)", isOneTwoOrThree);

		switch (*page) {
		case 1: *page = 100; break;
		case 2: *page = 200; break;
		case 3: *page = 400; break;
		}

		if ((*mem) % (*page) == 0) {
			break;
		}

		printf("ERROR: Memory size must be a multiple of the page!");
		printf(" %d is not a multiple of %d, please retry.\n", *mem, *page);
	}

	promptForFileName(filePath);
}

// get number of processes from file
int getNumOfProcessFromFile(FILE* filePtr) {
	int num = 0;

	fscanf(filePtr, "%d", &num);

	return num;
}

// stores values processes in process array
Process* assignProcessList(const char* filePath) {
	int numSpace;
	int tmp;
	int counter = 0;
	int totalSpace = 0;
	FILE* filePtr = fopen(filePath, "r");

	numOfProcs = getNumOfProcessFromFile(filePtr);

	// ale space for process array
	Process* procList = malloc(numOfProcs * sizeof(Process));

	if (!filePtr) {
		printf("ERROR: Failed to open file %s", filePath);
		exit(1);
	}

	while (!feof(filePtr) && counter < numOfProcs) {
		// store values for processes
		fscanf(filePtr, "%d %d %d %d",
			&(procList[counter].pid),
			&(procList[counter].arrivalTime),
			&(procList[counter].lifeTime),
			&numSpace);

		// get total memory requirements for process
		totalSpace = 0;
		for (int i = 0; i < numSpace; i++) {
			fscanf(filePtr, "%d", &tmp);
			totalSpace += tmp;
		}
		procList[counter].memReqs = totalSpace;

		procList[counter].isActive = 0;
		procList[counter].timeAddMemory = -1;
		procList[counter].timeDone = -1;

		counter++;
	}

	fclose(filePtr);

	return procList;
}