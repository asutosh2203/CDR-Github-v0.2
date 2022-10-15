#include <server.h>

Server s;
Utils serverUtil;

/*
 *  FUNCTION NAME	: signalHandler
 *
 *  DESCRIPTION		: It handles SIGINT and SIGTSTP signals
 *
 *  PARAMETERS		: int sig
 *
 *  RETURN 		: void
 *
 */
void signalHandler(int sig)
{
    if (sig == SIGINT || sig == SIGTSTP)
    {
        serverUtil.log(INFO, "Server Closed", S_LOGFILE);
        s.closeServer();
    }

    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{

    try
    {
        if (argc == 3)
        {
            int port = atoi(argv[2]);

            if (!serverUtil.validateIP(argv[1]))
            {
                throw("Invalid IP Address");
            }
            if (port > 0)
            {
                s.setIPAddr(argv[1]);
                s.setPort(port);
                // Signal Handling
                signal(SIGINT, signalHandler);
                signal(SIGTSTP, signalHandler);

                s.createSocket();
                s.bind_listen();
                s.acceptClient();
            }
            else
            {
                throw("Invalid PORT Number");
            }
        }
        else
        {
            throw "Invalid number of arguments";
        }
    }
    catch (const char *err)
    {
        cerr << err << endl;
    }

    return 0;
}