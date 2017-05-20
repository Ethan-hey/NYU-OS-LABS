#include "SCAN.h"
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
#include <limits.h>

Request* SCAN::get_ongoing_req() {
	return waitlist.front();
}

bool SCAN::has_wait_request() {
	return waitlist.size() != 0;
}

void SCAN::set_add(Request* req, int current_time, int io_num) {
	req->req_num = io_num;
	req->move_track = 0;
	waitlist.push_back(req);
}

void SCAN::delete_from_wait() {
	waitlist.pop_front();
}

Request* SCAN::get_from_waitlist(int current_track) {
	if(waitlist.size() > 1) {
		list<Request*> :: iterator it;
		list<Request*> :: iterator index = waitlist.begin();
		int shortest = INT_MAX;
		Request* req;
		bool found = false;
		while(1) {
			// up
			if(check_direction()) {
				for(it = waitlist.begin(); it != waitlist.end(); it++) {
					int track = (*it)->req_track;
					int dist = track - current_track;
					if(dist >= 0 && shortest > dist) {
						shortest = dist;
						index = it;
						found = true;
					}
				}
				if(found) {
					req = *index;
					waitlist.erase(index);
					waitlist.push_front(req);
					return req;
				}
				else {
					// change direction
					change_direction();
				} 
			}
			// down
			if(!check_direction()) {
				for(it = waitlist.begin(); it != waitlist.end(); it++) {
					int track = (*it)->req_track;
					int dist = track - current_track;
					if(dist <= 0 && shortest > -dist) {
						shortest = -dist;
						index = it;
						found = true;
					}
				}
				if(found) {
					req = *index;
					waitlist.erase(index);
					waitlist.push_front(req);
					return req;
				}
				else {
					// change direction
					change_direction();
				}
			}
		}
	}
	else if(waitlist.size() == 1) {
		return waitlist.front();
	}
	else return NULL;
}