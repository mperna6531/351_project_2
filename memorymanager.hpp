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
  long curr_time_;
  long last_time_;

  std::vector<Process> pl_;
  FrameList fl_;
  std::vector<Process> pq_;

  void update_pl(int pid);
  void print_turnaround_time();
  void queue_arrival();
  void print_queue() const;
  void assign_avail_mem();
  void end_done_processes();
  std::string get_prefix();

public:
  MemoryManager(int mem, int page_size, std::string &filename);
  void simulate();
};

MemoryManager::MemoryManager(int mem, int page_size, std::string &filename) :
  curr_time_(0), last_time_(0) {
  std::ifstream ifs;	

  ifs.open(filename);

  while (!ifs.is_open()) {
    std::cout << "Error: File could not be opened." << std::endl;
    std::cout << "Please input the correct filename." << std::endl;
    std::cout << "Input file: ";
    std::cin >> filename;
    ifs.open(filename);
  }

  int num_frames = mem / page_size;
  fl_ = FrameList(num_frames, page_size);

  int num_procs;
  ifs >> num_procs;
  pl_ = std::vector<Process>(num_procs);
  
  int proc_count = 0, total_space = 0;
  int processId, arrival, lifetime, space;
  int temp;

  while (!ifs.eof() && proc_count < num_procs) {
    ifs >> processId >> arrival >> lifetime >> space;
    total_space = 0;
    for (int i = 0; i < space; ++i) {
      ifs >> temp;
      total_space += temp;
    }
    Process proc(processId, arrival, lifetime, total_space);
    pl_[proc_count++] = proc;
  }
  ifs.close();
}

void MemoryManager::queue_arrival() {
  for (auto &proc : pl_) {
    if (proc.get_arrival() == curr_time_) {
      std::cout << get_prefix() <<
        "Process " << proc.get_pid() << " arrives" << std::endl;
          
      pq_.push_back(proc);
      print_queue();

      fl_.print();
    }
  }
}

void MemoryManager::print_queue() const {
  std::cout << "\tInput queue: [";

  for (auto el : pq_) {
   std::cout << el.get_pid();
  }
  std::cout <<  "]\n";
}

std::string MemoryManager::get_prefix() {
  std::string result = "\t";

  if (last_time_ != curr_time_) 
    result = "t = " + std::to_string(curr_time_) + ", ";

  last_time_ = curr_time_;

  return result;
}

void MemoryManager::end_done_processes() {
  for (size_t i = 0; i < pl_.size(); ++i) {
    if (pl_[i].active()) {
      int time_elapsed = curr_time_ - pl_[i].get_load_time();
      if (time_elapsed >= pl_[i].get_life()) {
        std::cout << get_prefix()
          << "process " << pl_[i].get_pid() << " completes\n";

        pl_[i].end(curr_time_);
        fl_.free_by_pid(pl_[i].get_pid());
        fl_.print();
      }
    }
  }
}

void MemoryManager::update_pl(int pid) {
  for (auto &el : pl_) 
    if (el.get_pid() == pid) 
      el.load_to_memory(curr_time_);
}

void MemoryManager::assign_avail_mem() {
  for (size_t i = 0; i < pq_.size(); ++i) {
    Process proc = pq_[i];

    if (fl_.fits(proc)) {
      std::cout << get_prefix() 
        << "MM moves Process " << proc.get_pid() << " to memory\n";
      
      fl_.add_process(proc);
      update_pl(proc.get_pid());
    
      pq_.erase(pq_.begin() + i);
      print_queue();
      assign_avail_mem();
      fl_.print();		
    }
  }
}

void MemoryManager::print_turnaround_time()  {
  float total = 0;
  for (auto proc : pl_) 
    total += proc.getTimeDone() - proc.get_arrival();
  
  float avg = total / pl_.size();
  std::cout << "Average Turnaround Time: " << avg << std::endl;
}

void MemoryManager::simulate() {
  do { 
    queue_arrival();
    end_done_processes();
    assign_avail_mem();
    ++curr_time_;

    if (curr_time_ > MAX_TIME) {
      std::cout << "DEADLOCK: max time reached\n";
      exit(1);
    }
  } while (!(pq_.empty() && fl_.empty()));

  print_turnaround_time();
}

#endif