#pragma once
#include "/data/libco/co_routine.h"
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <functional>
#include <iostream>
#include <map>
#include <string.h>
#include <string>

using namespace std;

struct stEnv_t
{
   stCoCond_t* cond;
   char* data;
   int fd;
};

class data_io
{
public:
    data_io();
    ~data_io();

    static int readn(const int fd, void* vptr, const size_t SIZE);
    static int writen(const int fd, const void* vptr, const size_t SIZE);

};

class handler
{
public:
    handler(); 
    ~handler(); 
   
    void handle(const int fd);
    void get_handle();
    void post_handle();

private:
    std::map<int, std::function<void(const std::string&)> > get_map;
    std::map<int, std::function<void(const std::string&)> > post_map;
};
