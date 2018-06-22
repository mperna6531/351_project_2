#ifndef PROCESS_HPP
#define PROCESS_HPP

class Process {
private:
	int pid_;
	int life_;
	int mem_req_;
	long arriveTime;
	long load_time_;
	long time_done_;
	bool active_;

public:
  Process(int pid, int arrival_time, int life, int mem_req);
  void load_to_memory(long currTime);
  bool active();
  int get_pid() const;
  int get_life() const;
  int get_arrival() const;
  long get_load_time() const;
  long getTimeDone() const;
  int getmem_req_s() const;
  void end(long currTime);
  Process() {}
};

Process::Process(int pid, int arrival_time, int life, int mem_reqs) :
  pid_(pid),
  life_(life),
  mem_req_(mem_reqs),
  arriveTime(arrival_time),
  active_(false) {}

long Process::getTimeDone() const {
  return time_done_;
}

int Process::getmem_req_s() const {
  return mem_req_;
}

void Process::load_to_memory(long currTime) {
  load_time_ = currTime;
  active_ = true;
}

void Process::end(long currTime) {
  time_done_ = currTime;
  active_ = 0;
}

int Process::get_life() const {
  return life_;
}

bool Process::active() {
  return active_;
}

int Process::get_arrival() const {
  return arriveTime;
}

long Process::get_load_time() const {
  return load_time_;
}


int Process::get_pid() const {
  return pid_;
}

#endif