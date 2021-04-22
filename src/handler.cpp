#include "handler.h"
#include "type_def.h"
#include <fstream>
#include <cstdlib>

data_io::data_io()
{}

data_io::~data_io()
{}

static void ANSWER_POEM(const int fd) 
{
    const char *ret = "HTTP/1.1 200 OK\r\n";
    data_io::writen(fd, (void*)ret, strlen(ret));
    ret = "Content-Type:";
    data_io::writen(fd, (void*)ret, strlen(ret));
    ret = "text/html";
    data_io::writen(fd, (void*)ret, strlen(ret));
    ret = "\r\n\r\n";
    data_io::writen(fd, (void*)ret, strlen(ret));
    ret = "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\"/>床前明月光，疑是地上霜。<br>举头望明月，低头思故乡。</head></html>";
    data_io::writen(fd, (void*)ret, strlen(ret));
}

static void ANSWER_OK(std::string& FILE_TYPE, const int ACCEPT_FD, int& RES) {
    const char *ret = "HTTP/1.1 200 OK\r\n";
    data_io::writen(ACCEPT_FD, (void*)ret, strlen(ret));
    ret = "Content-Type:";
    data_io::writen(ACCEPT_FD, (void*)ret, strlen(ret));
    auto it = attr_map.find(FILE_TYPE);
    if(it != attr_map.end())
        ret = it->second.c_str();
    else
        ret = "text/html";
    data_io::writen(ACCEPT_FD, (void*)ret, strlen(ret));
    ret = "\r\n\r\n";
    data_io::writen(ACCEPT_FD, (void*)ret, strlen(ret));
}

static void ANSWER_FILE(const std::string& FILE_NAME, const int ACCEPT_FD, int& RES)    {
    int n = 0;
    char buf[MAX_LINE];
    int file_fd = open(FILE_NAME.c_str(), O_RDONLY);
    while((n = read(file_fd, buf, MAX_LINE)) > 0)
    {
        if(write(ACCEPT_FD, buf, n) < 0)
        {
            RES = -1;
            cout << "write err." << endl;
            break;
        }
    }
    close(file_fd);
    cout << "write ok" << endl;
}


static void ANSWER_ONE_FILE(const std::string& FILE_NAME, const int ACCEPT_FD, int& RES) {
    const char *ret = "HTTP/1.1 200 OK\r\n";
    data_io::writen(ACCEPT_FD, (void*)ret, strlen(ret));
    ret = "Content-Type:";
    data_io::writen(ACCEPT_FD, (void*)ret, strlen(ret));
    std::string file_attr = FILE_NAME.substr(FILE_NAME.find_last_of(".")+1);
    auto it = attr_map.find(file_attr);
    if(it != attr_map.end())
        ret = it->second.c_str();
    else
        ret = "text/html";
    data_io::writen(ACCEPT_FD, (void*)ret, strlen(ret));
    ret = "\r\n\r\n";
    data_io::writen(ACCEPT_FD, (void*)ret, strlen(ret));
    int n = 0;
    char buf[MAX_LINE];
    int file_fd = open(FILE_NAME.c_str(), O_RDONLY);
    while((n = read(file_fd, buf, MAX_LINE)) > 0)
    {
        if(write(ACCEPT_FD, buf, n) < 0)
        {
            RES = -1;
            cout << "write err." << endl;
            break;
        }
    }
    close(file_fd);
    cout << "write ok" << endl;
}

int data_io::readn(const int fd, void* vptr, const size_t SIZE)
{
    size_t num_left = SIZE;
    size_t num_read = 0;
    
    char* ptr = (char*)vptr;

    while(num_left > 0)
    {
        if((num_read = read(fd, ptr, num_left)) < 0) 
        {
            if(errno == EINTR)
            {
                num_read = 0;
            }
            else
            {
                return -1;
            }
        }
        else if(num_read < num_left)
        {
            num_left -= num_read;
            break; 
        }
       
        num_left -= num_read;
        ptr += num_read;
    }
    return SIZE - num_left;
}

int data_io::writen(const int fd, const void* vptr, const size_t SIZE)
{
    size_t num_left = SIZE;
    size_t num_write = 0;
    
    char* ptr = (char*)vptr;

    while(num_left > 0)
    {
        if((num_write = write(fd, ptr, num_left)) <= 0) 
        {
            if(num_write < 0 && errno == EINTR)
                num_write = 0; 
            else
                return -1;
        }
        
        num_left -= num_write;
        ptr += num_write;
    }
    return SIZE - num_left;
}

void* get_fun(void* input)
{
    co_enable_hook_sys();
    stEnv_t* pt = (stEnv_t*)input;
    std::string file_name(strtok(pt->data+4, " "));
    file_name = "/data/http_file_dir" + (file_name == "/"?"/index.html":file_name);
    cout << "get:" << file_name << endl;

    int res = 0;
    ANSWER_ONE_FILE(file_name, pt->fd, res);
    if(res < 0)
    {
        cout << "Write file err. file_name:" << file_name; 
    }
}

void fun1(const std::string& data)
{
    cout << data << endl;
}

handler::handler()
{
}

handler::~handler()
{}

void handler::handle(const int fd)
{
    char* data = new char[65535];
    if(data_io::readn(fd, (void*)data, 65535) >= 0)
    {
        fun1(data); 
    }

    if(strstr(data, "GET") == data)
    {
        stEnv_t* env = new stEnv_t;
        env->data = data;
        env->fd = fd;
        stCoRoutine_t* get_routine;
        co_create(&get_routine, NULL, get_fun, env);
        co_resume(get_routine);
    }
    else
    {
        cout << "unknown request." << endl; 
    }

    close(fd);

    delete []data;
}


