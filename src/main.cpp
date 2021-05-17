#include "cpp_server.h"
#include "handler.h"


int main()
{
    auto so = my_socket();
    int sock_fd = so.get_socket(3344);
    if(sock_fd < 0)
    {
        cout << "init socket err." << endl;
        return 0;
    }
    cout << "init socket done." << endl;
    
    my_epoll ep;

    if(!ep.add_event(sock_fd))
    {
        cout << "Add socket to epoll err" << endl;
    }

    epoll_event events[MAX_EVENTS];

    while(true)
    {
        int res = ep.raise_event(events);
        if(res <= 0)
        {
            cout << "Raise event nothing return" << endl;
        }
        else
        {
            cout << "Raise event" << endl;
            handler H;

            for(int i = 0; i < res; ++i)
                H.handle(events[i].data.fd); 

        }
    }
    cout << "Over." << endl;

    return 0;
}
