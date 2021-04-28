一个使用epoll和libco实现的简单web服务器

实现方案：
1. 使用epoll，io效率高。边缘触发，while读取；
2. libco实现数据handler。

为什么使用libco？

我买的服务器是单核的，这个时候单线程无阻塞是效率最高的执行方式，libco作为支持阻塞函数hook机制的非对称协程，完美适应。
如果是多核服务器，建议使用线程池加libco的形式。如果bthread这种M:N的协程也支持阻塞函数hook的机制，才是最完美的选择。
但是遗憾的是目前bthread不支持hook。

This is a simple http server realized by cpp.

Main features:
1. Use epoll to realize socket io;
2. Use libco coroutine to avoid file io block.

Usage:

step1: cd simple_cpp_web_server

step2: mkdir build && cd build && cmake .. && make -j4

step3: build directory /data/http_file_dir, and add files that can be get by client

step4: run server by ./my_http_server &

Then you can get files from your server with browser. (example: input [server_ip]:3344/index.html，default port is 3344)

To do:
1. Add config to confirm server port and file directory;
2. When qps getting higher, TCP sticky packet problem need to be solved.
