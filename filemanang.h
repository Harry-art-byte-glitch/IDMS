#pragma once
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <direct.h>
#include <fstream>
#include "common.h"

#pragma warning(disable:4996)

using namespace std;

long long int size_file(const char* name);

void mkfolder(string name, string username);