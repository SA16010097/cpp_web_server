#pragma once
#include<string>
#include<string.h>
#include<map>
#include <fcntl.h>
#include <iostream>

using namespace std;

#define MAX_LINE 1024

std::map<std::string, std::string> attr_map = {{"gif", "image/gif"},{"jpeg", "image/jpeg"},{"jpg","image/jpeg"},{"png","image/png"}};


