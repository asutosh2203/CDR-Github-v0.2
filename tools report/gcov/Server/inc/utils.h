#pragma once

#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <vector>
using namespace std;

// Log type macros
#define FATAL "Fatal log"
#define INFO "Info log"
#define WARN "Warning log"
#define DEBUG "Debug log"

#define C_LOGFILE "logs/ClientData.log"
#define S_LOGFILE "logs/ServerData.log"

class Utils
{
public:
    //      type,       msg,        filename
    void log(const char *, const char *, const char *);

    // converts a string to lowercase
    string toLowerCase(string);

    // check if a given string is a numeric string or not
    bool isNumber(string str);

    // Function to split string `str` using a given delimiter
    vector<string> split(const string &str, char delim);

    // Function to validate an IP address
    bool validateIP(string ip);
};
