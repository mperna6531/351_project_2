#ifndef PROCESS_H
#define PROCESS_H

class Process {
private:
	int pid_;
	int life_;
	int mem_req_;
  long arrival_time_;
	long load_time_;
  long time_done_;
	bool active_;
public:
  Process(int pid, int arrival_time, int life, int mem_req);
  void load_to_mem(long current_time);
  bool active();
  int get_pid() const;
  int get_life() const;
  int get_arrival() const;
  long get_load_time() const;
  long get_time_done() const;
  int get_mem_reqs() const;
  void end(long current_time);
  Process() {}
};

Process::Process(int pid, int arrival_time, int life, int mem_reqs) :
  pid_(pid), arrival_time_(arrival_time),
  life_(life), mem_req_(mem_reqs), active_(false) {}

long Process::get_time_done() const {
  return time_done_;
}

int Process::get_mem_reqs() const {
  return mem_req_;
}

void Process::load_to_mem(long current_time) {
  load_time_ = current_time;
  active_ = true;
}

void Process::end(long current_time) {
  time_done_ = current_time;
  active_ = 0;
}

int Process::get_life() const {
  return life_;
}

bool Process::active() {
  return active_;
}

int Process::get_arrival() const {
  return arrival_time_;
}

long Process::get_load_time() const {
  return load_time_;
}


int Process::get_pid() const {
  return pid_;
}

#endif