#ifndef REQUEST_H
#define REQUEST_H

class Request {
public:
	int req_start;
	int req_finish;
	int req_track;
	int req_num;
	int req_turnaround;
	int req_wait;
	int move_track;
	Request(int time, int track);
	~Request(){};
};

#endif