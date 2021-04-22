#simple_http_server_by_cpp

This is a simple http server realized by cpp.

Usage:
step1: cd simple_cpp_web_server

step2: mkdir build && cd build && cmake .. && make -j4

step3: build directory /data/http_file_dir, and add files that can be get by client

step4: run server by ./my_http_server &

Then you can get files from your server by browser. (example: input [server_ip]:3344/index.html，default ip is 3344)

to do:
1. add config to give server port and file directory;
2. when qps higher, TCP sticky packet problem need to be solved.
