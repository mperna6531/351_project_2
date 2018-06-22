#ifndef FRAME_LIST_HPP
#define FRAME_LIST_HPP

#include <iostream>
#include <vector>

#include "process.hpp"
#include "frame.hpp"

class FrameList {
private:
  using FrameVector = std::vector<Frame>;
  int num_frames_;
  int page_size_;
  FrameVector frames_;
public:
  void print();
  bool fits(Process proc);
  void add_process(Process proc);
  FrameList(int num_frames, int pg_size) :
    num_frames_(num_frames), 
	page_size_(pg_size),
	frames_(FrameVector(num_frames)) {}
  FrameList() {};
  void free_by_pid(int pid);
  bool empty();
};

bool FrameList::fits(Process proc) {
int free_frames = 0;

  for (auto frame : frames_) 
    if (!frame.assigned()) 
			++free_frames; 
    
  return ((free_frames * page_size_) >= proc.getMemoryReqs());
}

void FrameList::add_process(Process proc) {
	int remaining_mem = proc.getMemoryReqs();
  int current_page = 1;
  
  for (auto &frame : frames_) {
		if (!frame.assigned() && remaining_mem > 0) {
		  frame = Frame(true, proc.get_pid(), current_page++);
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
 
	for (size_t i = 0; i < frames_.size(); ++i) {
		if (!free_block && !frames_[i].assigned()) {
			free_block = true;
      start_free = i;
		} else if (free_block && (frames_[i].assigned() || (i == (frames_.size() - 1)))) {
			free_block = false;
      begin = start_free * page_size_;
      end = i * page_size_ - 1;
			std::cout << "\t\t" << begin << "-" << end << ": Free frame(s)\n";
		}

		if (frames_[i].assigned()) {
      begin = i * page_size_;
      end = (i + 1) * page_size_ - 1;
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
	for (auto &frame : frames_) 
		if (frame.process_assigned() == pid)
		  frame.free();
}

#endif