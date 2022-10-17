#include <customer.h>

Utils custUtil;
map<long, Customer> CustomersMap;

Customer::Customer()
{
  // default constructor
}

// Parameterised Constructor
Customer::Customer(long msIsdn, string brandName, long inCallI, long outCallI, long inCallO, long outCallO, long down, long up, long incMsgI, long outMsgI, long incMsgO, long outMsgO)
{
  this->MSISDN = msIsdn;
  // this->MMC = MMC;
  this->brandName = brandName;
  this->inCallDurationO = inCallO;
  this->outCallDurationO = outCallO;
  this->downData = down;
  this->upData = up;
  this->inMsgO = incMsgO;
  this->outMsgO = outMsgO;
  this->inMsgI = incMsgI;
  this->outMsgI = outMsgI;
  this->inCallDurationI = inCallI;
  this->outCallDurationI = outCallI;
}
// Setters Functions
void Customer::setOutCallDurationI(long outCallDurationI) { this->outCallDurationI = outCallDurationI; }
void Customer::setInCallDurationI(long inCallDurationI) { this->inCallDurationI = inCallDurationI; }
void Customer::setDownloadData(long downData) { this->downData = downData; }
void Customer::setUploadData(long upData) { this->upData = upData; }
void Customer::setInMsgI(long inMsgI) { this->inMsgI = inMsgI; }
void Customer::setOutMsgI(long outMsgI) { this->outMsgI = outMsgI; }
void Customer::setOutCallDurationO(long outCallDurationO) { this->outCallDurationO = outCallDurationO; }
void Customer::setInCallDurationO(long inCallDurationO) { this->inCallDurationO = inCallDurationO; }
void Customer::setInMsgO(long inMsgO) { this->inMsgO = inMsgO; }
void Customer::setOutMsgO(long outMsgO) { this->outMsgO = outMsgO; }
// Getters Functions
long Customer::getInCallDurationI() { return inCallDurationI; }
long Customer::getOutCallDurationI() { return outCallDurationI; }
long Customer::getDownloadData() { return downData; }
long Customer::getUploadData() { return upData; }
long Customer::getInMsgI() { return inMsgI; }
long Customer::getOutMsgI() { return outMsgI; }
long Customer::getMSISDN() { return MSISDN; }
string Customer::getBrandName() { return brandName; }
long Customer::getInCallDurationO() { return inCallDurationO; }
long Customer::getOutCallDurationO() { return outCallDurationO; }
long Customer::getInMsgO() { return inMsgO; }
long Customer::getOutMsgO() { return outMsgO; }

/*
 *  FUNCTION NAME : processCDR
 *
 *  DESCRIPTION   : It takes the data from data.cdr and puts it into STL containers after formatting
 *
 *  PARAMETERS    : None
 *
 *  RETURN        : bool
 *
 */
bool Customer::processCDR()
{
  fstream fs;
  fs.open("data/data.cdr", ios::in);
  string line;
  string token;
  vector<string> parameters;
  if (fs)
  {
    while (getline(fs, line))
    {
      // stringstream class check
      stringstream check(line);

      // Tokenizing w.r.t. space '|'
      while (getline(check, token, '|'))
      {
        parameters.push_back(token);
      }

      // parameters[3] --> call type.
      // stol: converts the string as a argument in function call.
      long custMSISDN = stol(parameters[0]);
      string brand = parameters[1];

      long callDuration = stol(parameters[4]);
      long dataDownload = stol(parameters[5]);
      long dataUpload = stol(parameters[6]);

      // checking if the connection occurs within the operator
      if (parameters[2] == parameters[8])
      {
        if (parameters[3] == "MTC")
        {
          Customer newCust(custMSISDN, brand, callDuration, 0, 0, 0, 0, 0, 0, 0, 0, 0);
          CustomersMap[custMSISDN] = newCust;
        }
        else if (parameters[3] == "MTO")
        {
          Customer newCust(custMSISDN, brand, 0, callDuration, 0, 0, 0, 0, 0, 0, 0, 0);
          CustomersMap[custMSISDN] = newCust;
        }
        else if (parameters[3] == "SMS-MT")
        {
          Customer newCust(custMSISDN, brand, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0);
          CustomersMap[custMSISDN] = newCust;
        }
        else if (parameters[3] == "SMS-MO")
        {
          Customer newCust(custMSISDN, brand, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0);
          CustomersMap[custMSISDN] = newCust;
        }
        else if (parameters[3] == "GPRS")
        {
          Customer newCust(custMSISDN, brand, 0, 0, 0, 0, dataDownload, dataUpload, 0, 0, 0, 0);
          CustomersMap[custMSISDN] = newCust;
        }
      }
      // for outside operators
      else
      {
        if (parameters[3] == "MTC")
        {
          Customer newCust(custMSISDN, brand, 0, 0, callDuration, 0, 0, 0, 0, 0, 0, 0);
          CustomersMap[custMSISDN] = newCust;
        }
        else if (parameters[3] == "MTO")
        {
          Customer newCust(custMSISDN, brand, 0, 0, 0, callDuration, 0, 0, 0, 0, 0, 0);
          CustomersMap[custMSISDN] = newCust;
        }
        else if (parameters[3] == "SMS-MT")
        {
          Customer newCust(custMSISDN, brand, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0);
          CustomersMap[custMSISDN] = newCust;
        }
        else if (parameters[3] == "SMS-MO")
        {
          Customer newCust(custMSISDN, brand, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
          CustomersMap[custMSISDN] = newCust;
        }
        else if (parameters[3] == "GPRS")
        {
          Customer newCust(custMSISDN, brand, 0, 0, 0, 0, dataDownload, dataUpload, 0, 0, 0, 0);
          CustomersMap[custMSISDN] = newCust;
        }
      }
      parameters.clear();
    } // end of while loop

    // closing data.cdr
    fs.close();
  } // end of if block
  else
  {
    perror("data.cdr File error: ");
    exit(EXIT_FAILURE);
    return false;
  }

  return true;
} // end of processCDR()

/*
 *  FUNCTION NAME	: mapToFile
 *
 *  DESCRIPTION		: It takes the data from operatorsMap and puts it into CB.txt after formatting
 *
 *  PARAMETERS		: None
 *
 *  RETURN 		    : bool
 *
 */
bool Customer::mapToFile()
{
  fstream CB;
  CB.open("data/CB.txt", ios::out);

  if (CB)
  {
    CB << "=========== CUSTOMER DATA BASE ===========\n\n";
    map<long, Customer>::iterator cstr;
    for (cstr = CustomersMap.begin(); cstr != CustomersMap.end(); cstr++)
    {
      Customer cstrData = cstr->second;
      CB << cstrToString(cstrData);
    }

    CB.close();
  }
  else
  {
    custUtil.log(FATAL, "CB.txt could not be opened", S_LOGFILE);
    return false;
  }

  return true;
} // end of mapToFile()

/*
 *  FUNCTION NAME	: searchMSISDN
 *
 *  DESCRIPTION		: It takes a MSISDN as parameter, searches for it in the customersMap
                    and returns the formatted result.
 *
 *  PARAMETERS		: string custMSISDN
 *
 *  RETURN 		    : long
 *
 */
string Customer::searchMSISDN(long custMSISDN)
{
  string result;

  map<long, Customer>::iterator cstr;
  // for (cstr = CustomersMap.begin(); cstr != CustomersMap.end(); cstr++)
  // {
  //   Customer cstrData = cstr->second;
  //   if (cstrData.getMSISDN() == custMSISDN)
  //   {
  //     result = cstrToString(cstrData);
  //     return result;
  //   }
  // }

  if (CustomersMap.find(custMSISDN) != CustomersMap.end())
  {
    result = cstrToString(CustomersMap[custMSISDN]);
    return result;
  }

  // checking if MSISDN is invalid
  string notFound = "MSISDN doesn't exist.";
  return notFound;
} // end of searchMSISDN

/*
 *  FUNCTION NAME	: cstrToString
 *
 *  DESCRIPTION		: It takes an object of Customer type, formats the data and returns it in a string.
 *
 *  PARAMETERS		: Customer cstrData
 *
 *  RETURN 		    : string
 *
 */
string Customer::cstrToString(Customer &cstrData)
{
  ostringstream ss;
  ss.clear();

  ss << "Customers ID: " << cstrData.getMSISDN() << "(" << cstrData.getBrandName() << ")" << endl
     << "\t"
     << "* Service within the mobile Customer *" << endl
     << "\t"
     << "Incoming voice call duration:" << cstrData.getInCallDurationI() << endl
     << "\t"
     << "Outgoing voice call duration:" << cstrData.getOutCallDurationI() << endl
     << "\t"
     << "Incoming SMS messages:" << cstrData.getInMsgI() << endl
     << "\t"
     << "Outgoing SMS messages:" << cstrData.getOutMsgI() << endl
     << "\t"
     << "* Service outside the mobile Customer *" << endl
     << "\t"
     << "Incoming voice call duration:" << cstrData.getInCallDurationO() << endl
     << "\t"
     << "Outgoing voice call duration:" << cstrData.getOutCallDurationO() << endl
     << "\t"
     << "Incoming SMS messages:" << cstrData.getInMsgO() << endl
     << "\t"
     << "Outgoing SMS messages:" << cstrData.getOutMsgO() << endl
     << "\t"
     << "* Internet use *" << endl
     << "\t"
     << "MB downloaded:" << cstrData.getDownloadData() << " | MB uploaded:" << cstrData.getUploadData() << endl
     << endl;

  return ss.str();
} // end of cstrToString()

/*
 *  FUNCTION NAME	: processAndCreateFile
 *
 *  DESCRIPTION		: It invokes processCDR and mapToFile and returns true on successful processing
                    and file creation, false otherwise.
 *
 *  PARAMETERS		: promise<bool> *isProcessed
 *
 *  RETURN 		    : bool
 *
 */
bool Customer::processAndCreateFile(promise<bool> *isProcessed)
{
  bool process = processCDR();
  bool toFile = mapToFile();

  if (process && toFile)
  {
    isProcessed->set_value(true);
    return true;
  }

  isProcessed->set_value(false);
  return false;
} // end of processAndCreateFile

Customer::~Customer()
{
  // default constructor
}