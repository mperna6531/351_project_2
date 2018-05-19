// CPSC 351 Assignment 2.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <memory>
#include <queue>
#include <vector>

#include "process.h"
#include "memory.h"
#include "prototypes.h"
const int timeMax = 100000;
int numOfProcs = 0, lastAnnouncement = -1;

std::vector<Process> pl;
FrameList fl;
std::vector<Process> pq;

void print_proc_queue(std::vector<Process> pq) {
	std::cout << "\tInput queue: [ ";

	for (auto el : pq) {
   std::cout << el.pid;
	}
	std::cout <<  "]\n";
}
void enqueue_newly_arrived(long current_time) {
   for (auto proc : pl) {
	    if (proc.arrivalTime == current_time) {
			  std::cout << getAnnouncementPrefix(current_time) <<
			    "Process " << proc.pid << std::endl;
		      
			  pq.push_back(proc);
        print_proc_queue(pq);
        fl.print();
		  }
    
		}
}

void terminate_completed_process(long current_time) {
	int time_spent_in_memory = 0;

	// dequeue any procs that need it
	for (auto proc : pl) {
		time_spent_in_memory = current_time - proc.timeAddMemory;

		if (proc.isActive && (time_spent_in_memory >= proc.lifeTime)) {
			std::cout << getAnnouncementPrefix(current_time)
        << "Process " << proc.pid << " completes\n";

			proc.isActive = 0;
			proc.timeDone = current_time;

			fl.free_by_pid(proc.pid);

			fl.print();
		}
	}
}

void assign_available_mem(long current_time) {
	int i, index, limit;
	limit = pq.size();

	// enqueue any procs that can be put into mem
	for (int i = 0; i < pq.size(); ++i) {
		Process proc = pq.at(i);

		if (fl.process_fits(proc)) {
			std::cout << getAnnouncementPrefix(current_time) 
        << "MM moves Process " << proc.pid << " to memory\n";

    proc.isActive = 1;
		proc.timeAddMemory = current_time;
		fl.fit_process(proc);
  
		pq.erase(pq.begin() + i);
		print_proc_queue(pq);
    fl.print();
		}
	}
}

void main_loop() {
	long current_time = 0;
	while (!pq.empty() && !fl.empty()) {

		enqueue_newly_arrived(current_time);
    terminate_completed_process(current_time);
		assign_available_mem(current_time);

		++current_time;

		if (current_time > timeMax) {
			std::cout << "DEADLOCK: max time reached\n";
			break;
		}
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