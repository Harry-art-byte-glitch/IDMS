#include <string>
#include <time.h>
#include <Windows.h>
#include "common.h"

#pragma warning(disable:4996)

using namespace std;

string int_to_string(int n)
{
	int m = n;
	char s[100];
	char ss[100];
	int i = 0, j = 0;
	while (m > 0)
	{
		s[i++] = m % 10 + '0';
		m /= 10;
	}
	s[i] = '\0';
	i = i - 1;
	while (i >= 0)
	{
		ss[j++] = s[i--];
	}
	ss[j] = '\0';
	return ss;
}

string float_to_string(float Num)
{
	//std::ostringstream oss;
	//oss << Num;
	//std::string str(oss.str());
	//return str;
	return std::to_string(Num);
}

string get_time()
{
	time_t curtime;
	time(&curtime);
	tm* nowtime = localtime(&curtime);
	// 输出tm结构的年月日
	string a = int_to_string(1900 + nowtime->tm_year);
	string b = int_to_string(1 + nowtime->tm_mon);
	string c = int_to_string(nowtime->tm_mday);
	string e = c + '/' + b + '/' + a + '/';
	return e;
}

string get_hour_min_sec()
{
	time_t curtime;
	time(&curtime);
	tm* nowtime = localtime(&curtime);
	// 输出tm结构的年月日
	string a = int_to_string(nowtime->tm_hour);
	string b = int_to_string(nowtime->tm_min);
	string c = int_to_string(nowtime->tm_sec);
	string e = c + '/' + b + '/' + a + '/';
	return e;
}