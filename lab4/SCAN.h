#ifndef SCAN_H
#define SCAN_H

#include "IO.h"
#include "Request.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cctype>
#include <list>
#include <unistd.h>

class SCAN : public IO {
public:
	SCAN() : IO() {};
	~SCAN() {};

	Request* get_from_waitlist(int current_track);
	Request* get_ongoing_req();
	bool has_wait_request();
	void set_add(Request* req, int current_time, int io_num);
	void delete_from_wait();

private:

};

#endif