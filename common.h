#pragma once
#include <string>

#pragma warning(disable:4996)

using namespace std;

struct user
{
	string name = { NULL };
	string password = { NULL };
	long long int file_max_size = 0;
	long long int file_size = 0;
	string state = { NULL };
	long long int available_space = 0;
};

string int_to_string(int n);

string float_to_string(float Num);

string get_time();

string get_hour_min_sec();