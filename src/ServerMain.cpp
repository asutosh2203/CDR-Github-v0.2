#include <server.h>

Server s;
Utils serverUtil;
// signal handling
void signalHandler(int sig)
{
    if (sig == SIGINT || sig == SIGTSTP)
    {
        serverUtil.log(INFO, "Server Closed", S_LOGFILE);
        s.closeServer();
    }

    exit(EXIT_SUCCESS);
}

int main()
{
    // Signal Handling
    signal(SIGINT, signalHandler);
    signal(SIGTSTP, signalHandler);

    s.createSocket();
    s.bind_listen();
    s.acceptClient();

    return 0;
}