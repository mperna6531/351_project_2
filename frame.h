class Frame {
private:
  bool assigned_;
  int proc_assign_;
  int page_num_;
public:
  Frame(bool assigned, int proc, int page);
  Frame();
  int page_num();
  int process_assigned();
  bool assigned(); 
  void assign(int pid, int page_num);
  void free();
};

Frame::Frame(bool assigned, int proc, int page) : 
  assigned_(assigned), 
	proc_assign_(proc), 
	page_num_(page) {}

Frame::Frame() : assigned_(false) {}

void Frame::assign(int pid, int page_num) {
  assigned_ = true;
  proc_assign_ = pid;
  page_num_ = page_num;
}

void Frame::free() { 
  assigned_ = false;
  int proc_assign_ = 0;
  int page_num_ = 0;
}

int Frame::process_assigned() { return proc_assign_; }

int Frame::page_num() { return page_num_; }

bool Frame::assigned() { return assigned_; }