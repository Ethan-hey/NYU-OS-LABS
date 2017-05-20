#ifndef FSCAN_H
#define FSCAN_H

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

class FSCAN : public IO {
public:
	FSCAN() : IO() {
		busy1 = false;
		busy2 = false;
	};
	~FSCAN() {};

	bool busy1, busy2;

	Request* get_from_waitlist(int current_track);
	Request* get_ongoing_req();
	bool has_wait_request();
	void set_add(Request* req, int current_time, int io_num);
	void delete_from_wait();

private:

};

#endif