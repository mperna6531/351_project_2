// CPSC 351 Assignment 2.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "process.h"
#include "memory.h"

const int timeMax = 100000;
int numOfProcs = 0;
long last_announcement = -1;

std::vector<Process> pl;
FrameList fl;
std::vector<Process> pq;



void print_proc_queue(std::vector<Process> pq) {
	std::cout << "\tInput queue: [";

	for (auto el : pq) {
   std::cout << el.pid;
	}
	std::cout <<  "]\n";
}

std::string get_announcement_prefix(long current_time)  {
	std::string result = "\t";

  if (last_announcement != current_time) 
		result = "t = " + std::to_string(current_time) + ", ";

	last_announcement = current_time;

	return result;
}

void enqueue_newly_arrived(long current_time) {
  for (auto proc : pl) {
	  if (proc.arrivalTime == current_time) {
			std::cout << get_announcement_prefix(current_time) <<
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
			std::cout << get_announcement_prefix(current_time)
        << "Process " << proc.pid << " completes\n";

			proc.isActive = 0;
			proc.timeDone = current_time;

			fl.free_by_pid(proc.pid);

			fl.print();
		}
	}
}

void assign_available_mem(long current_time) {
	int index, limit;
	limit = pq.size();

	// enqueue any procs that can be put into mem
	for (int i = 0; i < pq.size(); ++i) {
		Process proc = pq.at(i);

		if (fl.process_fits(proc)) {
			std::cout << get_announcement_prefix(current_time) 
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

void print_turnaround_time()  {
	float total = 0;

	for (auto proc : pl) 
		total += proc.timeDone - proc.arrivalTime;
	
  float avg = total / pl.size();
	std::cout << "Average Turnaround Time: " << avg << std::endl;
}

void get_user_input(int &mem, int &page_size, std::string &filename) {
	std::cout << "Memory: ";
  std::cin >> mem;
  std::cout << "Page size: ";
  std::cin >> page_size;
  std::ifstream ifs;

  if (!(mem % page_size == 0)) {
		std::cout << "ERROR: Memory size must be a multiple of the page!";
	  exit(1);
	}

  std::cout << "Input file: ";
  std::cin >> filename;
  ifs.open(filename);

  if (!ifs.is_open()) {
		std::cout << "ERROR: Could not open file!\n";
    exit(1);
	}

	ifs.close();
}

void assign_process_list(std::string &file_name) {
  long arrival, life_time;
  int pid, tmp, num_space;
	int counter = 0;
	int total_space = 0;
  int num_procs = 0;

	std::ifstream ifs(file_name);

  if (!ifs.is_open()) {
	  std::cout << "Error opening file!";
	  exit(1);
	}
  
  ifs >> num_procs;
	// ale space for process array
	pl = std::vector<Process>(num_procs);

	while (!ifs.eof() && counter < num_procs) {
    
		// store values for processes
		ifs >> pid >> arrival >> life_time >> num_space;

		pl[counter].pid = pid;
		pl[counter].arrivalTime = arrival;
		pl[counter].lifeTime = life_time;
			
		// get total memory requirements for process
		total_space = 0;
		for (int i = 0; i < num_space; ++i) {
			ifs >> tmp;
			total_space += tmp;
		}

		pl[counter].memReqs = total_space;
		pl[counter].isActive = 0;
		pl[counter].timeAddMemory = -1;
		pl[counter].timeDone = -1;

		++counter;
	}

	ifs.close();
}

void main_loop() {
	long current_time = 0;

	do { 
		enqueue_newly_arrived(current_time);
    terminate_completed_process(current_time);
		assign_available_mem(current_time);

		++current_time;

		if (current_time > timeMax) {
			std::cout << "DEADLOCK: max time reached\n";
			break;
		}

	} while (!(pq.empty() && fl.empty()));

	print_turnaround_time();
}

int main() {
  int mem_size = 0;
	int pg_size = 0;
	
	std::string file_path;

	get_user_input(mem_size, pg_size, file_path);
	assign_process_list(file_path);

	pq = std::vector<Process>(numOfProcs);
  int num_frames = mem_size / pg_size;
	fl = FrameList(num_frames, pg_size);

	main_loop();

	return 0;
}
