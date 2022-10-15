#include <interoperator.h>
#include <utils.h>

map<long, Operator> operatorsMap;

Utils utl;

Operator::Operator()
{
    // default constructor
}

Operator::Operator(string brandName, long MMC, long inCall, long outCall, long down, long up, long incMsg, long outMsg)
{
    this->MMC = MMC;
    this->brandName = brandName;
    this->inCallDuration = inCall;
    this->outCallDuration = outCall;
    this->downData = down;
    this->upData = up;
    this->inMsg = incMsg;
    this->outMsg = outMsg;
}

// setters functions
void Operator::setOutCallDuration(long outCallDuration) { this->outCallDuration = outCallDuration; }
void Operator::setInCallDuration(long inCallDuration) { this->inCallDuration = inCallDuration; }
void Operator::setDownloadData(long downData) { this->downData = downData; }
void Operator::setUploadData(long upData) { this->upData = upData; }
void Operator::setInMsg(long inMsg) { this->inMsg = inMsg; }
void Operator::setOutMsg(long outMsg)
{
    this->outMsg = outMsg;
}
// getters functions
long Operator::getInCallDuration() { return inCallDuration; }
long Operator::getOutCallDuration() { return outCallDuration; }
long Operator::getDownloadData() { return downData; }
long Operator::getUploadData()
{
    return upData;
}
long Operator::getInMsg() { return inMsg; }
long Operator::getOutMsg() { return outMsg; }
long Operator::getMMC() { return MMC; }
string Operator::getBrandName() { return brandName; }

/*
 *  FUNCTION NAME	: processCDR
 *
 *  DESCRIPTION		: It takes the data from data.cdr and puts it into STL containers after formatting
 *
 *  PARAMETERS		: None
 *
 *  RETURN 		    : bool
 *
 */
bool Operator::processCDR()
{
    fstream dataCDR;
    dataCDR.open("data/data.cdr", ios::in);
    string line = "";
    string token = "";
    vector<string> parameters;
    if (dataCDR)
    {
        while (getline(dataCDR, line))
        {
            // stringstream class check
            stringstream check(line);

            // Tokenizing w.r.t. space '|'
            while (getline(check, token, '|'))
            {
                parameters.push_back(token);
            }

            // stol: converts the string to long data type
            string brand = parameters[1];
            long brandMMC = stol(parameters[2]);
            long callDuration = stol(parameters[4]);
            long dataDownload = stol(parameters[5]);
            long dataUpload = stol(parameters[6]);

            // checking if a brand MMC already exists in the operators map
            if (operatorsMap.count(brandMMC))
            {
                if (parameters[3] == "MTC")
                {
                    operatorsMap[brandMMC].setInCallDuration(operatorsMap[brandMMC].getInCallDuration() + callDuration);
                }
                else if (parameters[3] == "MOC")
                {
                    operatorsMap[brandMMC].setOutCallDuration(operatorsMap[brandMMC].getOutCallDuration() + callDuration);
                }
                else if (parameters[3] == "SMS-MT")
                {
                    operatorsMap[brandMMC].setInMsg(operatorsMap[brandMMC].getInMsg() + 1);
                }
                else if (parameters[3] == "SMS-MO")
                {
                    operatorsMap[brandMMC].setOutMsg(operatorsMap[brandMMC].getOutMsg() + 1);
                }
                else
                {
                    operatorsMap[brandMMC].setDownloadData(operatorsMap[brandMMC].getDownloadData() + dataDownload);
                    operatorsMap[brandMMC].setUploadData(operatorsMap[brandMMC].getUploadData() + dataUpload);
                }
            }
            else if (parameters[3] == "MTC")
            {
                Operator newOp(brand, brandMMC, callDuration, 0, 0, 0, 0, 0);
                operatorsMap[brandMMC] = newOp;
            }
            else if (parameters[3] == "MOC")
            {
                Operator newOp(brand, brandMMC, 0, callDuration, 0, 0, 0, 0);
                operatorsMap[brandMMC] = newOp;
            }
            else if (parameters[3] == "SMS-MT")
            {
                Operator newOp(brand, brandMMC, 0, 0, 0, 0, 1, 0);
                operatorsMap[brandMMC] = newOp;
            }
            else if (parameters[3] == "SMS-MO")
            {
                Operator newOp(brand, brandMMC, 0, 0, 0, 0, 0, 1);
                operatorsMap[brandMMC] = newOp;
            }
            else
            {
                Operator newOp(brand, brandMMC, 0, 0, dataDownload, dataUpload, 0, 0);
                operatorsMap[brandMMC] = newOp;
            }

            parameters.clear();
        } // end of while loop

        // closing data.cdr
        dataCDR.close();
    } // end of if block
    else
    {
        perror("data.cdr File error: ");
        utl.log(FATAL, "data.cdr could not be opened", S_LOGFILE);
        return false;
    }

    return true;
} // end of processCDR()

/*
 *  FUNCTION NAME	: mapToFile
 *
 *  DESCRIPTION		: It takes the data from operatorsMap and puts it into IOSB.txt after formatting
 *
 *  PARAMETERS		: None
 *
 *  RETURN 		: bool
 *
 */
bool Operator::mapToFile()
{
    fstream IOSB;
    IOSB.open("data/IOSB.txt", ios::out);

    if (IOSB)
    {
        IOSB << "=========== OPERATOR DATA BASE ===========\n\n";
        map<long, Operator>::iterator opr;
        for (opr = operatorsMap.begin(); opr != operatorsMap.end(); opr++)
        {
            Operator oprData = opr->second;
            IOSB << oprToString(oprData);
        }
        IOSB.close();
    }
    else
    {
        utl.log(FATAL, "IOSB.txt could not be opened", S_LOGFILE);
        return false;
    }

    return true;
} // end of mapToFile()

/*
 *  FUNCTION NAME	: searchBrandName
 *
 *  DESCRIPTION		: It takes a string as parameter, searches for it in the operatorsMap 
                       and returns the formatted result.
 *
 *  PARAMETERS		: string brand
 *
 *  RETURN 		: string
 *
 */
string Operator::searchBrandName(string brand)
{
    string result;
    map<long, Operator>::iterator opr;
    for (opr = operatorsMap.begin(); opr != operatorsMap.end(); opr++)
    {
        Operator oprData = opr->second;
        if (utl.toLowerCase(oprData.getBrandName()) == utl.toLowerCase(brand))
        {
            result = oprToString(oprData);
            return result;
        }
    }

    // checking if brand name is invalid
    string notFound = "Brand name doesn't exist.";
    return notFound;
} // end of searchBrandName()

/*
 *  FUNCTION NAME	: oprToString
 *
 *  DESCRIPTION		: It takes an object of Operator type, formats the data and returns it in a string
 *
 *  PARAMETERS		: Operator &oprData
 *
 *  RETURN 		: string
 *
 */
string Operator::oprToString(Operator &oprData)
{
    ostringstream ss;
    ss.clear();

    ss << "Operator Brand: " << oprData.getBrandName() << "(" << oprData.getMMC() << ")" << endl
       << "\t"
       << "Incoming Call Duration: " << oprData.getInCallDuration() << endl
       << "\t"
       << "Outgoing Call Duration: " << oprData.getOutCallDuration() << endl
       << "\t"
       << "Incoming Messages: " << oprData.getInMsg() << endl
       << "\t"
       << "Outgoing Messages: " << oprData.getOutMsg() << endl
       << "\t"
       << "GPRS usage: " << endl
       << "\t"
       << "\t"
       << "Downloaded Data: " << oprData.getDownloadData() << " MB" << endl
       << "\t"
       << "\t"
       << "Uploaded Data: " << oprData.getUploadData() << " MB" << endl
       << endl;

    return ss.str();
} // end of oprToString()

/*
 *  FUNCTION NAME	: processAndCreateFile
 *
 *  DESCRIPTION		: It invokes processCDR and mapToFile and returns true on successful processing 
                       and file creation, false otherwise.
 *
 *  PARAMETERS		: promise<bool> *isProcessed
 *
 *  RETURN 		: bool
 *
 */
bool Operator::processAndCreateFile(promise<bool> *isProcessed)
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
} //end of processAndCreateFile

Operator::~Operator()
{
    // default constructor
}
