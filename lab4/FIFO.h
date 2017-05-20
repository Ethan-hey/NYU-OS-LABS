#ifndef FIFO_H
#define FIFO_H

#include "IO.h"
#include "Request.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <unistd.h>

class FIFO : public IO {
public:
	FIFO() : IO() {};
	~FIFO() {};

	Request* get_from_waitlist(int current_track);
	Request* get_ongoing_req();
	bool has_wait_request();
	void set_add(Request* req, int current_time, int io_num);
	void delete_from_wait();

private:

};

#endif