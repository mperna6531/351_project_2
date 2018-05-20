#pragma once

#include <iostream>
#include <vector>

#include "process.h"

class Frame {
private:
  bool assigned_;
  int proc_assign_;
  int page_num_;
public:
  int page_num() { return page_num_; }
  Frame (bool assigned, int proc, int page) : 
    assigned_(assigned), 
	proc_assign_(proc), 
	page_num_(page) {}
  Frame() : assigned_(false) {}
  int process_assigned() { return proc_assign_; }
  bool assigned() { return assigned_; }
  void assign(int pid, int page_num) {
    assigned_ = true;
    proc_assign_ = pid;
    page_num_ = page_num;
    }
  void free() { 
    assigned_ = false;
    int proc_assign_ = 0;
    int page_num_ = 0;
  }
};

class FrameList {
private:
  using FrameVector = std::vector<Frame>;
  FrameVector frames_;
  int num_frames_;
  int page_size_;
public:
  void print();
  bool process_fits(Process proc);
  void fit_process(Process proc);
  FrameList(int num_frames, int pg_size) :
    num_frames_(num_frames), 
	page_size_(pg_size),
	frames_(pg_size) {}
  FrameList() {};
  void free_by_pid(int pid);
  bool empty();
};

bool FrameList::process_fits(Process proc) {
int free_frames = 0;

  for (auto frame : frames_) 
    if (!frame.assigned()) 
			++free_frames;
    
  return ((free_frames * page_size_) >= proc.memReqs);
}

void FrameList::fit_process(Process proc) {
	int remaining_mem = proc.memReqs;
  int current_page = 1;
  
  for (auto &frame : frames_) {
		if (!frame.assigned()) {
		  frame = Frame(true, proc.pid, current_page++);
			remaining_mem -= page_size_;
		}
  }
}

void FrameList::print() {
	bool free_block = false;
  int start_free = 1;
  int begin;
  int end;
	std::cout << "\tMemory map:\n";
 
	for (int i = 0; i < frames_.size(); ++i) {
		if (!free_block && !frames_[i].assigned()) {
			free_block = true;
		} else if (free_block && frames_[i].assigned()) {
			free_block = false;
      begin = start_free * page_size_;
      end = i * page_size_ - 1;
			std::cout << "\t\t" << begin << "-" << end << ": Free frames\n";
		}

		if (frames_[i].assigned()) {
      begin = i * page_size_;
      end = i * page_size_ - 1;
			std::cout << "\t\t" << begin << "-" << end << 
        ": Process: " << frames_[i].process_assigned() << 
        " Page: " << frames_[i].page_num() << std::endl;
		}
	}
}

bool FrameList::empty() {
	for (auto frame : frames_)
    if (frame.assigned())
      return false;
  return true;
}

void FrameList::free_by_pid(int pid) {
	for (auto frame : frames_) 
		if (frame.process_assigned() == pid)
		  frame.free();
}