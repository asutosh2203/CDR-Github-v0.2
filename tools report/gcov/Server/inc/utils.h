#include <iostream>
#include <fstream>
#include <algorithm>
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
};
