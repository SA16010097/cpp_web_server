#include "cpp_server.h"
#include "/data/libco/co_routine.h"

int test_libco()
{
    co_enable_hook_sys(); 
    cout << "xxxx" << endl;
    return 0;
}

my_epoll::my_epoll()
{
    epoll_fd = epoll_create(MAX_EVENTS);
    if(epoll_fd <= 0)
    {
        cout << "Create epoll err." << endl;
        exit(1);
    }
}

my_epoll::~my_epoll()
{
    close(epoll_fd);
}

bool my_epoll::add_event(const int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN|EPOLLET;
    int res = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
    if(res < 0)
    {
        cout << "Add event err." << endl;
        return false;
    }
    sockfds.insert(fd);
    return true;
}

bool my_epoll::del_event(const int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN|EPOLLET;
    int res = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &ev);
    if(res <= 0)
    {
        cout << "Del event err." << endl;
        return false;
    }
    sockfds.erase(fd);
    return true;
}

int my_epoll::raise_event(epoll_event* events)
{
    int count = 0;

    epoll_event tmp_events[MAX_EVENTS];

    int res = epoll_wait(epoll_fd, tmp_events, MAX_EVENTS, -1);
    if(res < 0)
    {
        cout << "Wait event err." << endl;
        return 0;
    }

    for(size_t i = 0; i < res; ++i)
    {
        if(sockfds.find(tmp_events[i].data.fd) != sockfds.end())
        {
            int accept_fd = accept(tmp_events[i].data.fd, NULL, NULL);
            if(accept_fd <= 0)
            {
                cout << "Accept event err." << endl;
                return 0;
            }

            struct epoll_event ev;
            ev.data.fd = accept_fd;
            ev.events = EPOLLIN|EPOLLET;
            
            int res = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, accept_fd, &ev);
            if(res < 0)
            {
                cout << "Add accept event err." << endl;
                return 0;
            }
        }
        else
        {
            count++;
            if(count <= MAX_EVENTS)
            {
                events[count - 1] = tmp_events[i]; 
            }
            else
            {
                cout << "Raise event oversize" << endl;
                return MAX_EVENTS;
            }
        }
    }
    
    return count;
}

my_socket::my_socket()
{}

my_socket::~my_socket()
{
    close(sock_fd);
}

int my_socket::get_socket(const int port)
{
    struct sockaddr_in server_in;

    bzero(&server_in,sizeof(struct sockaddr_in));
    server_in.sin_family=AF_INET;
    server_in.sin_port=htons(port);
    server_in.sin_addr.s_addr=INADDR_ANY;
    int sock_fd=socket(AF_INET,SOCK_STREAM,0);

    if(sock_fd==-1){
        cout << "Fail to socket." << endl;
        return -1;
    }

    if(fcntl(sock_fd, F_SETFL, O_NONBLOCK) < 0)
    {
        cout << "Init socket err." << endl;
        close(sock_fd);
        return -1;
    }

    if(bind(sock_fd,(struct sockaddr*)&server_in,sizeof(struct sockaddr_in))==-1){

        cout << "Fail to bind." << endl;
        close(sock_fd);
        return -1;
    }

    if(listen(sock_fd,20)==-1)
    {
        cout << "Fail to listen." << endl;
        close(sock_fd);
        return -1;  
    }
    
    return sock_fd;
}

