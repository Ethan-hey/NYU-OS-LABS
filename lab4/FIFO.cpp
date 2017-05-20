#include "FIFO.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cctype>
#include <list>
#include <unistd.h>

Request* FIFO::get_ongoing_req() {
	return waitlist.front();
}

bool FIFO::has_wait_request() {
	return waitlist.size() != 0;
}

void FIFO::set_add(Request* req, int current_time, int io_num) {
	req->req_num = io_num;
	req->move_track = 0;
	waitlist.push_back(req);
}

void FIFO::delete_from_wait() {
	waitlist.pop_front();
}

Request* FIFO::get_from_waitlist(int current_track) {
	if(waitlist.size() != 0) {
		Request* req = waitlist.front();
		return req;
	}
	else return NULL;
}