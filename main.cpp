// CPSC 351 Assignment 2.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "process.h"
#include "memory.h"

const int MAX_TIME = 100000;

long last_announcement = -1;

std::vector<Process> pl;
FrameList fl;
std::vector<Process> pq;

void print_proc_queue(std::vector<Process> pq) {
	std::cout << "\tInput queue: [";

	for (auto el : pq) {
   std::cout << el.get_pid();
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
  for (auto &proc : pl) {
	  if (proc.get_arrival() == current_time) {
			std::cout << get_announcement_prefix(current_time) <<
			  "Process " << proc.get_pid() << " arrives" << std::endl;
		      
			pq.push_back(proc);
      print_proc_queue(pq);

      fl.print();
		}
	}
}

void terminate_completed_process(long current_time) {
	// dequeue any procs that need it
	for (int i = 0; i < pl.size(); ++i) {

	  if (pl[i].active()) {
			int time_elapsed = current_time - pl[i].get_load_time();
	    if (time_elapsed >= pl[i].get_life()) {
			  std::cout << get_announcement_prefix(current_time)
          << "process " << pl[i].get_pid() << " completes\n";

			  pl[i].end(current_time);
			  fl.free_by_pid(pl[i].get_pid());
			  fl.print();
		  }
	  }
  }
}

void update_pl(int pid, int current_time) {
  for (auto &el : pl) 
		if (el.get_pid() == pid) 
		  el.load_to_mem(current_time);
}

void assign_available_mem(long current_time) {
	for (int i = 0; i < pq.size(); ++i) {
		Process proc = pq[i];

		if (fl.process_fits(proc)) {
			std::cout << get_announcement_prefix(current_time) 
				<< "MM moves Process " << proc.get_pid() << " to memory\n";
			
		  fl.fit_process(proc);
      
			update_pl(proc.get_pid(), current_time);
		
		  pq.erase(pq.begin() + i);
		  print_proc_queue(pq);
			assign_available_mem(current_time);
		  fl.print();		
	  }
	}
}

void print_turnaround_time()  {
	float total = 0;

	for (auto proc : pl) 
		total += proc.get_time_done() - proc.get_arrival();
	
  float avg = total / pl.size();
	std::cout << "Average Turnaround Time: " << avg << std::endl;
}

void main_loop() {
	long current_time = 0;

	do { 
		enqueue_newly_arrived(current_time);
    terminate_completed_process(current_time);
		assign_available_mem(current_time);

		++current_time;

		if (current_time > MAX_TIME) {
			std::cout << "DEADLOCK: max time reached\n";
			break;
		}
  bool test = fl.empty();
	bool test_pq = pq.empty();

	} while (!(pq.empty() && fl.empty()));

	print_turnaround_time();
}

int main() {

	int memSize = 0;
	int pgSize = 0;
	
	//User input
	std::string fileInput;
	
	std::cout << "Memory size: ";
	std::cin >> memSize;
	std::cout << "Page size: ";
	std::cin >> pgSize;
	std::ifstream ifs;

	while (!(memSize % pgSize == 0)) {
		std::cout << "Error: Memory size must be a multiple of page size." << std::endl;
		std::cout << "Please re-enter memory size and page size." << std::endl;
		std::cout << "Memory size: ";
		std::cin >> memSize;
		std::cout << "Page size: ";
		std::cin >> pgSize;
	}

	std::cout << "Input file: ";
	std::cin >> fileInput;
	ifs.open(fileInput);

	while (!ifs.is_open()) {
		std::cout << "Error: File could not be opened." << std::endl;
		std::cout << "Please input the correct filename." << std::endl;
		std::cout << "Input file: ";
		std::cin >> fileInput;
		ifs.open(fileInput);
	}

	//Process file input
	long arrival, life_time;
	int pid, tmp, num_space;
	int counter = 0;
	int total_space = 0;
	int num_procs = 0;
  
	ifs >> num_procs;
	pl = std::vector<Process>(num_procs);

	while (!ifs.eof() && counter < num_procs) {
    
		// store temp values for processes
		ifs >> pid >> arrival >> life_time >> num_space;
			
		total_space = 0;
		for (int i = 0; i < num_space; ++i) {
			ifs >> tmp;
			total_space += tmp;
		}

		Process proc(pid, arrival, life_time, total_space);
		pl[counter++] = proc;
	}

	ifs.close();

	int x = memSize / pgSize;
	fl = FrameList(x, pgSize);

	main_loop();

	return 0;
}
