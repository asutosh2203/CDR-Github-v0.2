#include <server.h>

int main()
{
    Server s;
    s.createSocket();
    s.bind_listen();
    s.acceptClient();

    return 0;
}