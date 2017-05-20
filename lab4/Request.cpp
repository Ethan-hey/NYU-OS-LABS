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

using namespace std;

Request::Request(int time, int track) {
	req_start = time;
	req_track = track;
}