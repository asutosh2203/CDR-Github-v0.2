#include <utils.h>
#include <sys/stat.h>
#include <fcntl.h>

// this function creates a logfile which stores all the log messages at runtime along with the time stamp
void Utils::log(const char *type, const char *msg, const char *filename)
{
    mkdir("logs", 0777);

    fstream logfile;
    logfile.open(filename, ios::app);

    time_t ltime = time(NULL);

    struct tm res;
    char TIMESTAMP[32];

    localtime_r(&ltime, &res);
    asctime_r(&res, TIMESTAMP);

    // Writing log in file with formatting
    logfile << endl
            << "~" << TIMESTAMP << " \t " << type << ": " << msg << endl;

    logfile.close();
}

string Utils::toLowerCase(string str)
{
    for_each(str.begin(), str.end(), [](char &c)
             { c = tolower(c); });

    return str;
}
