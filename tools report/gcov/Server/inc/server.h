#pragma once

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <bits/stdc++.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <user.h>
#include <interoperator.h>
#include <customer.h>
#include <utils.h>

#define MAX_BUFF 4096

using namespace std;
class Server
{
public:
        Server();
        void setIPAddr(const char *ipAddr)
        {
                this->ipAddr = ipAddr;
        }
        void setPort(int port)
        {
                this->port = port;
        }

        void createSocket();
        void bind_listen();
        void acceptClient();
        void initClient(int);

        int userExists(User &);
        int verifyLoginCreds(User &);

        bool processCallData(Operator &, Customer &);

        int sendFile(int, char *);

        void log(string);

        void closeServer();
        struct sockaddr_in getClientAddr() { return client_addr; }
        socklen_t getClientAddrLen() { return len; }

private:
        int serverSockfd, clientSockfd;
        int port;
        const char *ipAddr;
        struct sockaddr_in server_addr;
        struct sockaddr_in client_addr;
        socklen_t len;
};
