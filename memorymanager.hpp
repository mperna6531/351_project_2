#ifndef MEMORYMANAGER_HPP
#define MEMORYMANAGER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <vector>

#include "process.hpp"
#include "framelist.hpp"

class MemoryManager {
private:
  const int MAX_TIME = 100000;
  long current_time_;
  long last_announcement_;

  std::vector<Process> pl_;
  FrameList fl_;
  std::vector<Process> pq_;

  void update_pl(int pid);
  void print_turnaround_time();
  void enqueue_arrivals();
  void print_process_queue() const;
  void assign_available_memory();
  void terminate_completed_processes();
	std::string get_prefix();
public:
  MemoryManager(int mem, int page_size, std::string &filename);
  void simulate();
};

MemoryManager::MemoryManager(int mem, int page_size, std::string &filename) :
  current_time_(0), last_announcement_(0) {
  std::ifstream ifs;	

  if (!(mem % page_size == 0)) {
	  std::cout << "ERROR: Memory size must be a multiple of the page!";
	  exit(1);
  }

  ifs.open(filename);
  if (!ifs.is_open()) {
	std::cout << "ERROR: Could not open file!\n";
    exit(1);
  }

	int num_frames = mem / page_size;
	fl_ = FrameList(num_frames, page_size);

  int num_procs;
	ifs >> num_procs;
	pl_ = std::vector<Process>(num_procs);
  
	int counter = 0;
	int total_space = 0;
	int pid, arrival, life_time, num_space, tmp;

	while (!ifs.eof() && counter < num_procs) {
		// store temp values for processes
		ifs >> pid >> arrival >> life_time >> num_space;
		total_space = 0;
		for (int i = 0; i < num_space; ++i) {
			ifs >> tmp;
			total_space += tmp;
		}
		Process proc(pid, arrival, life_time, total_space);
		pl_[counter++] = proc;
  }
	ifs.close();
}

void MemoryManager::enqueue_arrivals() {
  for (auto &proc : pl_) {
	  if (proc.get_arrival() == current_time_) {
			std::cout << get_prefix() <<
			  "Process " << proc.get_pid() << " arrives" << std::endl;
		      
			pq_.push_back(proc);
      print_process_queue();

      fl_.print();
		}
	}
}

void MemoryManager::print_process_queue() const {
	std::cout << "\tInput queue: [";

	for (auto el : pq_) {
   std::cout << el.get_pid();
	}
	std::cout <<  "]\n";
}

std::string MemoryManager::get_prefix() {
	std::string result = "\t";

  if (last_announcement_ != current_time_) 
		result = "t = " + std::to_string(current_time_) + ", ";

	last_announcement_ = current_time_;

	return result;
}

void MemoryManager::terminate_completed_processes() {
	// dequeue any processes that have completed their runtime
	for (size_t i = 0; i < pl_.size(); ++i) {
	  if (pl_[i].active()) {
			int time_elapsed = current_time_ - pl_[i].get_load_time();
	    if (time_elapsed >= pl_[i].get_life()) {
			  std::cout << get_prefix()
          << "process " << pl_[i].get_pid() << " completes\n";

			  pl_[i].end(current_time_);
			  fl_.free_by_pid(pl_[i].get_pid());
			  fl_.print();
		  }
	  }
  }
}

void MemoryManager::update_pl(int pid) {
  for (auto &el : pl_) 
		if (el.get_pid() == pid) 
		  el.load_to_mem(current_time_);
}

void MemoryManager::assign_available_memory() {
	for (size_t i = 0; i < pq_.size(); ++i) {
		Process proc = pq_[i];

		if (fl_.fits(proc)) {
			std::cout << get_prefix() 
				<< "MM moves Process " << proc.get_pid() << " to memory\n";
			
		  fl_.add_process(proc);
			update_pl(proc.get_pid());
		
		  pq_.erase(pq_.begin() + i);
		  print_process_queue();
			assign_available_memory();
		  fl_.print();		
	  }
	}
}

void MemoryManager::print_turnaround_time()  {
	float total = 0;
	for (auto proc : pl_) 
		total += proc.get_time_done() - proc.get_arrival();
	
  float avg = total / pl_.size();
	std::cout << "Average Turnaround Time: " << avg << std::endl;
}

void MemoryManager::simulate() {
	do { 
		enqueue_arrivals();
    terminate_completed_processes();
		assign_available_memory();
		++current_time_;

		if (current_time_ > MAX_TIME) {
			std::cout << "DEADLOCK: max time reached\n";
			break;
		}
	} while (!(pq_.empty() && fl_.empty()));

	print_turnaround_time();
}

#endif