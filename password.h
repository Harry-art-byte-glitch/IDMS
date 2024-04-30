#pragma once
#include <iostream>
#include<cryptlib.h>
#include "common.h"

#include "randpool.h"
#include "rsa.h"
#include "hex.h"
#include "files.h"
#include "config.h"
#include "stdcpp.h"
#include "modes.h"
#include"base64.h"
#include "filemanang.h"

using namespace std;
using namespace CryptoPP;

#pragma comment(lib, "cryptlib.lib")

string encrypt(const std::string& str_in, const std::string& key, const std::string& iv);

string decrypt(const std::string& str_in, const std::string& key, const std::string& iv);