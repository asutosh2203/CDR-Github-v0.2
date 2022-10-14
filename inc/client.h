#pragma once

#include <stdio.h>
#include <iostream>
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
private:
    int sockfd;
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

    int writeToFile(int, char *);
    ~Client();
};

void clientErrExit();
void showMenu(int menuType);
int getUserChoice();
void pressEnter();
bool validateInput(string, int);