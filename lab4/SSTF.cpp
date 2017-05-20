#include "SSTF.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cctype>
#include <list>
#include <cmath>
#include <unistd.h>

Request* SSTF::get_ongoing_req() {
	return waitlist.front();
}

bool SSTF::has_wait_request() {
	return waitlist.size() != 0;
}

void SSTF::set_add(Request* req, int current_time, int io_num) {
	req->req_num = io_num;
	req->move_track = 0;
	waitlist.push_back(req);
}

void SSTF::delete_from_wait() {
	waitlist.pop_front();
}

Request* SSTF::get_from_waitlist(int current_track) {
	if(waitlist.size() > 1) {
		list<Request*> :: iterator it;
		list<Request*> :: iterator index = waitlist.begin();
		int shortest = abs(waitlist.front()->req_track - current_track);
		Request* req;
		for(it = waitlist.begin(); it != waitlist.end(); it++) {
			int track = (*it)->req_track;
			int dist = abs(current_track - track);
			if(shortest > dist) {
				shortest = dist;
				index = it;
			}
		}
		req = *index;
		waitlist.erase(index);
		waitlist.push_front(req);
		return req;
	}
	else if(waitlist.size() == 1) {
		return waitlist.front();
	}
	else return NULL;
}