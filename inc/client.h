#pragma once

#include <iostream>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <string>  // std::string
#include <stdlib.h>

using namespace std;
#define MAX_BUFF 4096
typedef struct sockaddr_in SA_IN;

class Client
{
private:
    int sockfd;
    int ret;
    int port;
    char ipAddr[20];
    struct sockaddr_in serverAddr;
    socklen_t len;

public:
    Client();
    Client(int, char *);
    void createSocket();
    void clientConnect();
    int getClientSockfd() { return sockfd; }
    void clientreadwrite(char *);

    ~Client();
};

void showMenu(int menuType);