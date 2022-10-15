#include <utils.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
 *  FUNCTION NAME	: processAndCreateFile
 *
 *  DESCRIPTION		: It logs a message and it's type along with the time stamp in given filename.
 *
 *  PARAMETERS		: const char* type, const char* msg, const char* filename
 *
 *  RETURN 		: void
 *
 */
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

// converts a given string into lowercase
// Returns a string
string Utils::toLowerCase(string str)
{
    for_each(str.begin(), str.end(), [](char &c)
             { c = tolower(c); });

    return str;
}

bool Utils::isNumber(string str)
{
    int len = str.length();
    for (int i = 0; i < len; i++)
    {
        char c = str.at(i);

        if (c < 48 || c > 57)
        {
            return false;
        }
    }

    return true;
}

/*
 *  FUNCTION NAME	: split
 *
 *  DESCRIPTION		: It splits a given string on the basis of delimiter
 *
 *  PARAMETERS		: const string &str, char delim
 *
 *  RETURN 		: vector<string>
 *
 */
vector<string> Utils::split(const string &str, char delim)
{
    auto i = 0;
    vector<string> tokens;

    auto pos = str.find(delim);

    while (pos < str.length())
    {
        tokens.push_back(str.substr(i, pos - i));
        i = ++pos;
        pos = str.find(delim, pos);
    }

    tokens.push_back(str.substr(i, str.length()));

    return tokens;
}

/*
 *  FUNCTION NAME	: validateIP
 *
 *  DESCRIPTION		: It validates the IP according to required conditions
 *
 *  PARAMETERS		: string ip
 *
 *  RETURN 		: bool
 *
 */
bool Utils::validateIP(string ip)
{
    // split the string into tokens
    vector<string> ipTokens = split(ip, '.');

    // if the token size is not equal to four
    if (ipTokens.size() != 4)
    {
        return false;
    }

    // validate each token
    for (string ipToken : ipTokens)
    {
        // verify that the string is a number or not, and the numbers
        // are in the valid range

        if (!isNumber(ipToken) || stoi(ipToken) > 255 || stoi(ipToken) < 0)
        {
            return false;
        }
    }

    return true;
}