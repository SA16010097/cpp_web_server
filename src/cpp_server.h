#pragma once

#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <string>
#include <cstring>
#include <map>
#include <vector>
#include <set>
#include <stdlib.h>
#include <fcntl.h>
#include "iostream"
#include <functional>

using namespace std;

const uint32_t MAX_EVENTS = 1024;

//int init(int port);

class my_epoll
{
public:
    my_epoll();
    virtual ~my_epoll();
    bool add_event(const int fd);
    bool del_event(const int fd);
    int raise_event(epoll_event* events);
private:
    int epoll_fd;
    std::set<int> sockfds;
};

class my_socket
{
public:
    my_socket();
    ~my_socket();
    int get_socket(const int port);
    bool recv_data(std::string& data);
private:
    int sock_fd;
};


