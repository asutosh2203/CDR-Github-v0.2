#include <interoperator.h>
#include <utils.h>

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

bool Operator::processCDR()
{
    fstream dataCDR;
    dataCDR.open("data/data.cdr", ios::in);
    string line;
    string token;
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

            // parameters[3] --> call type.
            // stol: converts the string as a argument in function call.
            string brand = parameters[1];
            long brandMMC = stol(parameters[2]);     // parameters[2]=brand MMC
            long callDuration = stol(parameters[4]); // parameters[4]= (call duration)
            long dataDownload = stol(parameters[5]); // parameters[5]= (data download)
            long dataUpload = stol(parameters[6]);   // parameters[6]= (data upload)

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
                // Operator::Operator(string brandName, long MMC, long inCall, long outCall, long down, long up, long incMsg, long outMsg)
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
        }

        // closing data.cdr
        dataCDR.close();

        // if (IOSB)
        // {
        //     mapToFile(IOSB);

        //     // closing IOSB.txt
        //     IOSB.close();
        // }
        // else
        // {
        //     perror("IOSB.txt File error: ");
        //     return false;
        // }
    }
    else
    {
        perror("data.cdr File error: ");
        utl.log("data.cdr could not be opened", "logs/Interoperator.txt");
        return false;
    }

    return true;
}

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
            IOSB << "Operator Brand: " << oprData.getBrandName() << "(" << oprData.getMMC() << ")" << endl
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
        }
        IOSB.close();
    }
    else
    {
        utl.log("IOSB.txt could not be opened", "logs/Interoperator.txt");
        return false;
    }

    return true;
}

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
}

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
}

Operator::~Operator()
{
    // default constructor
}
