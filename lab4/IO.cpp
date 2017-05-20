#include "IO.h"
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

void IO::initialize(Request* req) {
	reqlist.push_back(req);
}

bool IO::has_unload_request() {
	return reqlist.size() != 0;
}

Request* IO::get_from_reqlist() {
	return reqlist.front();
}

bool IO::check_completed(Request* req, int current_track) {
	return current_track == req->req_track;
}

void IO::set_finish(Request* req, int current_time) {
	req->req_finish = current_time;
	req->req_turnaround = req->req_finish - req->req_start;
}

bool IO::check_arriving(Request* req, int current_time) {
	return req->req_start == current_time;
}

void IO::delete_from_req() {
	reqlist.pop_front();
}

bool IO::check_ongoing() {
	return ongoing;
}

void IO::set_ongoing() {
	ongoing = true;
}

void IO::clear_ongoing() {
	ongoing = false;
}

bool IO::check_direction() {
	return dir_flag == true;
}

void IO::change_direction() {
	dir_flag = !dir_flag;
}
