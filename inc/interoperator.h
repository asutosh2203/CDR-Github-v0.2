#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string.h>
#include <bits/stdc++.h>
using namespace std;

class Operator
{
public:
    Operator(); // default constructor

    // parameterized constructor
    Operator(string, long, long, long, long, long, long, long);

    // setters functions
    void setOutCallDuration(long);
    void setInCallDuration(long);
    void setDownloadData(long);
    void setUploadData(long);
    void setInMsg(long);
    void setOutMsg(long);
    // getters functions
    long getInCallDuration();
    long getOutCallDuration();
    long getDownloadData();
    long getUploadData();
    long getInMsg();
    long getOutMsg();
    long getMMC();
    string getBrandName();

    string searchBrandName(string);
    string oprToString(Operator &);

    bool processCDR(); // processing and storing call data records into STL
    bool mapToFile();  // printing data from map into file
    ~Operator();       // destructor

private:
    string brandName; // specific brand name

    // getters, setters vairables
    long MMC;

    long inCallDuration;
    long outCallDuration;
    long inMsg;
    long outMsg;
    long downData;
    long upData;

    map<long, Operator> operatorsMap;
};