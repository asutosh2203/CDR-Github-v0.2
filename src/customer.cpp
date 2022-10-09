#include <customer.h>

Customer::Customer()
{
  // default constructor
}

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
// getters functions
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

// Processing the CDR data and writing in the CB.txt file
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
      long brandMSISDN = stol(parameters[0]);
      string brand = parameters[1];
      // long brandMMC = stol(parameters[2]);     // parameters[2]=brand MMC

      long callDuration = stol(parameters[4]); // parameters[4]= (call duration)
      long dataDownload = stol(parameters[5]); // parameters[5]= (data download)
      long dataUpload = stol(parameters[6]);   // parameters[6]= (data upload)

      if (parameters[2] == parameters[8])
      {
        if (parameters[3] == "MTC")
        {
          Customer newCust(brandMSISDN, brand, callDuration, 0, 0, 0, 0, 0, 0, 0, 0, 0);
          CustomersMap[brandMSISDN] = newCust;
        }
        else if (parameters[3] == "MTO")
        {
          Customer newCust(brandMSISDN, brand, 0, callDuration, 0, 0, 0, 0, 0, 0, 0, 0);
          CustomersMap[brandMSISDN] = newCust;
        }
        else if (parameters[3] == "SMS-MT")
        {
          Customer newCust(brandMSISDN, brand, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0);
          CustomersMap[brandMSISDN] = newCust;
        }
        else if (parameters[3] == "SMS-MO")
        {
          Customer newCust(brandMSISDN, brand, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0);
          CustomersMap[brandMSISDN] = newCust;
        }
        else if (parameters[3] == "GPRS")
        {
          Customer newCust(brandMSISDN, brand, 0, 0, 0, 0, dataDownload, dataUpload, 0, 0, 0, 0);
          CustomersMap[brandMSISDN] = newCust;
        }
      }
      else
      {
        if (parameters[3] == "MTC")
        {
          Customer newCust(brandMSISDN, brand, 0, 0, callDuration, 0, 0, 0, 0, 0, 0, 0);
          CustomersMap[brandMSISDN] = newCust;
        }
        else if (parameters[3] == "MTO")
        {
          Customer newCust(brandMSISDN, brand, 0, 0, 0, callDuration, 0, 0, 0, 0, 0, 0);
          CustomersMap[brandMSISDN] = newCust;
        }
        else if (parameters[3] == "SMS-MT")
        {
          Customer newCust(brandMSISDN, brand, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0);
          CustomersMap[brandMSISDN] = newCust;
        }
        else if (parameters[3] == "SMS-MO")
        {
          Customer newCust(brandMSISDN, brand, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
          CustomersMap[brandMSISDN] = newCust;
        }
        else if (parameters[3] == "GPRS")
        {
          Customer newCust(brandMSISDN, brand, 0, 0, 0, 0, dataDownload, dataUpload, 0, 0, 0, 0);
          CustomersMap[brandMSISDN] = newCust;
        }
      }
      parameters.clear();
    }
    // closing data.cdr
    fs.close();

    // fstream file;
    // file.open("data/CB.txt", ios::out);
    // if (file)
    // {
    //   mapToFile(file);

    //   // closing CB.txt
    //   file.close();
    // }
    // else
    // {
    //   perror("CB.txt File error: ");
    //   exit(EXIT_FAILURE);
    // }
  }
  else
  {
    perror("data.cdr File error: ");
    exit(EXIT_FAILURE);
    return false;
  }

  return true;
}

bool Customer::mapToFile()
{

  fstream CB;
  CB.open("data/CB.txt", ios::out);

  if (CB)
  {
    CB << "# Customers Data Base:\n\n";
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
    // utl.log("IOSB.txt could not be opened", "logs/Interoperator.txt");
    return false;
  }

  return true;
}

// Searching MSISDN and printing data
string Customer::searchMSISDN(long brandMSISDN)
{
  string result;

  map<long, Customer>::iterator cstr;
  for (cstr = CustomersMap.begin(); cstr != CustomersMap.end(); cstr++)
  {
    Customer cstrData = cstr->second;
    if (cstrData.getMSISDN() == brandMSISDN)
    {
      result = cstrToString(cstrData);
      return result;
    }
  }

  // checking if MSISDN is invalid
  string notFound = "MSISDN doesn't exist.";
  return notFound;
}

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
}

bool Customer::processAndCreateFile()
{
  bool process = processCDR();
  bool toFile = mapToFile();

  if (process && toFile)
    return true;

  return false;
}

Customer::~Customer()
{
  // default constructor
}