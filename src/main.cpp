#include "cpp_server.h"
#include "handler.h"


int main()
{
    auto so = my_socket();
    int sock_fd = so.get_socket(3344);
    cout << "init socket done." << endl;
    
    my_epoll ep;

    if(!ep.add_event(sock_fd))
    {
        cout << "Add socket to epoll err" << endl;
    }

    epoll_event events[MAX_EVENTS];

    while(true)
    {
        if(ep.raise_event(events) <= 0)
        {
            cout << "Raise event nothing return" << endl;
        }
        else
        {
            cout << "Raise event" << endl;
            handler H;

            H.handle(events[0].data.fd); 

        }
    }
    cout << "Over." << endl;

    return 0;
}
