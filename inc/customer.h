#include <iostream>
#include <iterator>
#include <map>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cstring>
#include <vector>
#include <future>
using namespace std;

class Customer
{
private:
        long MSISDN; // Unique user number
        string brandName;
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
        // setters
        //  Function name ending with "I" is for services within mobile Customer
        void setOutCallDurationI(long); 
        void setInCallDurationI(long);
        void setInMsgI(long);
        void setOutMsgI(long);
        //  Function name ending with "O" is for services Outside mobile Customer
        void setOutCallDurationO(long);
        void setInCallDurationO(long);
        void setInMsgO(long); 
        void setOutMsgO(long);
        
        void setDownloadData(long);
        void setUploadData(long);
        
        // getters functions
        long getInCallDurationI();
        long getOutCallDurationI();
        long getInMsgI();
        long getOutMsgI();
        
        long getInMsgO();
        long getOutMsgO();
        long getInCallDurationO();
        long getOutCallDurationO();
        
        long getDownloadData();
        long getUploadData();
        long getMSISDN();
        string getBrandName();

        bool processAndCreateFile(promise<bool> *);

        string searchMSISDN(long); // For searching unique MSISDN
        string cstrToString(Customer &); // For converting customer obj data to string
        bool processCDR(); // processing and storing call data records into STL
        bool mapToFile();  // printing data from map into file
        ~Customer();
};