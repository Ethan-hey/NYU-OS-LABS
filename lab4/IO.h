#ifndef IO_H
#define IO_H

#include "Request.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <cctype>
#include <list>
#include <unistd.h>
#include <cmath>

using namespace std;

class IO {
public:
	IO() {
		ongoing = false;
		dir_flag = true; // for SCAN algorithm, the initial direction is up
	}
	~IO() {}
	bool ongoing;
	bool dir_flag;
	list<Request*> waitlist;
	list<Request*> reqlist;
	list<Request*> waitlist2;

	virtual Request* get_from_waitlist(int current_track) = 0;
	virtual void set_add(Request* req, int current_time, int io_num) = 0;
	virtual bool has_wait_request() = 0;
	virtual void delete_from_wait() = 0;
	virtual Request* get_ongoing_req() = 0;
	void initialize(Request* req);
	bool has_unload_request();
	Request* get_from_reqlist();
	bool check_completed(Request* req, int current_track);
	void set_finish(Request* req, int current_time);
	bool check_arriving(Request*, int current_time);
	void delete_from_req();
	bool check_ongoing();
	void set_ongoing();
	void clear_ongoing();
	bool check_direction();
	void change_direction();
};

#endif