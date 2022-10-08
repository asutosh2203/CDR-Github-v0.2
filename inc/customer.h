#include <iostream>
#include <iterator>
#include <map>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cstring>
#include <vector>
using namespace std;

class Customer
{
private:
        long MSISDN; // Unique user number
        string brandName;
        // getters, setters vairables
        long MMC;
        long inCallDurationI;
        long outCallDurationI;
        long inMsgI;
        long outMsgI;
        long inCallDurationO;
        long outCallDurationO;
        long inMsgO;
        long outMsgO;
        long downData;
        long upData;

        map<long, Customer> CustomersMap;

public:
        Customer(); // default constructor

        // parameterized constructor
        Customer(long, string, long, long, long, long, long, long, long, long, long, long);
        // setter
        //  Function name ending with "I" is for services within mobile Customer
        //  Function name ending with "O" is for services Outside mobile Customer
        void setOutCallDurationI(long); // services within mobile Customer
        void setInCallDurationI(long);
        void setDownloadData(long);
        void setUploadData(long);
        void setInMsgI(long);
        void setOutMsgI(long);
        void setInMsgO(long); // services Outside mobile Customer
        void setOutMsgO(long);
        void setOutCallDurationO(long);
        void setInCallDurationO(long);

        // getters functions
        long getInCallDurationI();
        long getOutCallDurationI();
        long getDownloadData();
        long getUploadData();
        long getInMsgI();
        long getOutMsgI();
        long getInMsgO();
        long getOutMsgO();
        long getInCallDurationO();
        long getOutCallDurationO();
        long getMSISDN();
        string getBrandName();

        void processAndCreateFile();

        string searchMSISDN(long); // For searching unique MSISDN
        string cstrToString(Customer &);

        void processCDR(); // processing and storing call data records into STL
        bool mapToFile();  // printing data from map into file
        ~Customer();
};