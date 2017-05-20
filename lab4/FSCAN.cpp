#include "FSCAN.h"
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

bool FSCAN::has_wait_request() {
	if(busy1) {
		if(waitlist.size() == 0) {
			return waitlist2.size() != 0;
		}
		else return true;
	}
	else if(busy2) {
		if(waitlist2.size() == 0) {
			return waitlist.size() != 0;
		}
		else return true;
	}
	else return false;
}

void FSCAN::set_add(Request* req, int current_time, int io_num) {
	req->req_num = io_num;
	req->move_track = 0;
	if(!busy1 && !busy2) {
		waitlist.push_back(req);
		busy1 = true;
	}
	else if(busy1) {
		waitlist2.push_back(req);
	}
	else {
		waitlist.push_back(req);
	}
}

void FSCAN::delete_from_wait() {
	if(busy1) {
		waitlist.pop_front();
		if(waitlist.size() == 0) {
			busy1 = false;
			if(waitlist2.size() != 0) busy2 = true;
			dir_flag = true; // when switching queue, scan up first
		}
	}
	else {
		waitlist2.pop_front();
		if(waitlist2.size() == 0) {
			if(waitlist.size() != 0) busy1 = true;
			busy2 = false;
			dir_flag = true; // when switching queue, scan up first
		}
	}
}

Request* FSCAN::get_ongoing_req() {
	if(busy1) {
		return waitlist.front();
	}
	else return waitlist2.front();
}

Request* FSCAN::get_from_waitlist(int current_track) {
	if(!busy1 && !busy2) {
		busy1 = true;
	}
	if(busy1) {
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
		else {
			return NULL;
		}
	}
	else {
		if(waitlist2.size() > 1) {
			list<Request*> :: iterator it;
			list<Request*> :: iterator index = waitlist.begin();
			int shortest = INT_MAX;
			Request* req;
			bool found = false;
			while(1) {
			// up
				if(check_direction()) {
					for(it = waitlist2.begin(); it != waitlist2.end(); it++) {
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
						waitlist2.erase(index);
						waitlist2.push_front(req);
						return req;
					}
					else {
					// change direction
						change_direction();
					} 
				}
				// down
				if(!check_direction()) {
					for(it = waitlist2.begin(); it != waitlist2.end(); it++) {
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
						waitlist2.erase(index);
						waitlist2.push_front(req);
						return req;
					}
					else {
					// change direction
						change_direction();
					}
				}
			}
		}
		else if(waitlist2.size() == 1) {
			return waitlist2.front();
		}
		else return NULL;
	}
}