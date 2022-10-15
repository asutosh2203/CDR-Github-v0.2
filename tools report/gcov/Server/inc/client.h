#pragma once

#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstring>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <user.h>
#include <utils.h>

#define IOSB_DOWNLOAD "data/downloads/IOSB.txt"
#define CB_DOWNLOAD "data/downloads/CB.txt"

using namespace std;
#define MAX_BUFF 4096
typedef struct sockaddr_in SA_IN;

class Client
{
public:
    Client();

    void setIPAddr(const char *ipAddr)
    {
        this->ipAddr = ipAddr;
    }
    void setPort(int port)
    {
        this->port = port;
    }

    void createSocket();
    void clientConnect();
    int getClientSockfd() { return sockfd; }

    int writeToFile(int, char *);
    ~Client();

private:
    int sockfd;
    int port;
    const char* ipAddr;
    struct sockaddr_in serverAddr;
    socklen_t len;
};

void clientErrExit();
void showMenu(int);
int getUserChoice();
void pressEnter();
bool validateInput(string, int);