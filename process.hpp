#ifndef PROCESS_HPP
#define PROCESS_HPP

class Process {
private:
	int processId;
	int life_;
	int memoryReq;
	long arriveTime;
	long load_time_;
	long time_done_;
	bool active_;

public:
  Process(int pid, int arrival_time, int life, int mem_req);
  void loadToMemory(long currTime);
  bool active();
  int get_pid() const;
  int get_life() const;
  int get_arrival() const;
  long getLoadTime() const;
  long getTimeDone() const;
  int getMemoryReqs() const;
  void end(long currTime);
  Process() {}
};

Process::Process(int pid, int arrival_time, int life, int mem_reqs) :
  processId(pid),
  life_(life),
  memoryReq(mem_reqs),
  arriveTime(arrival_time),
  active_(false) {}

long Process::getTimeDone() const {
  return time_done_;
}

int Process::getMemoryReqs() const {
  return memoryReq;
}

void Process::loadToMemory(long currTime) {
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

long Process::getLoadTime() const {
  return load_time_;
}


int Process::get_pid() const {
  return processId;
}

#endif