#include <client.h>
#include <utils.h>

Utils clientUtil;

// default constructor
Client::Client()
{
    port = 8080;
    strcpy(ipAddr, "127.0.0.1");
}

// parameterized constructore
Client::Client(int port, char *ipAddr)
{
    this->port = port;
    strcpy(this->ipAddr, ipAddr);
}

void showMenu(int menuType)
{
    // sleep(2);
    // system("clear");
    //  main menu
    if (menuType == 0)
    {
        cout << "\nDo you want to register or login?" << endl;
        cout << "\t1. Register" << endl;
        cout << "\t2. Login" << endl;
        cout << "\t3. Exit" << endl;
    }
    // services menu
    else if (menuType == 1)
    {
        cout << "====== SERVICES ======" << endl;
        cout << "\t1. Process CDR" << endl;
        cout << "\t2. Billing Info" << endl;
        cout << "\t3. Logout" << endl;
    }
    // billing info menu
    else if (menuType == 2)
    {
        cout << "====== BILLING INFO MENU ======" << endl;
        cout << "\t1. Customer Billing" << endl;
        cout << "\t2. Interoperator Billing" << endl;
        cout << "\t3. Go Back" << endl;
    }
    // customer billing menu
    else if (menuType == 3)
    {
        cout << "====== CUSTOMER BILLING MENU ======" << endl;
        cout << "\t1. Search for MSISDN" << endl;
        cout << "\t2. Get the all the content of the processed CDR for Customer Billing" << endl;
        cout << "\t3. Go Back" << endl;
    }
    // interoperator billing menu
    else
    {
        cout << "====== INTEROPERATOR BILLING MENU ======" << endl;
        cout << "\t1. Search for Brand Name/Operator ID" << endl;
        cout << "\t2. Get the all the content of the processed CDR for Interoperator Settlement Billing" << endl;
        cout << "\t3. Go Back" << endl;
    }
    cout << "Choose your option: ";
}

// creates client socket
void Client::createSocket()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        perror("socket() error: ");
        exit(EXIT_FAILURE);
    }
    cout << "[+]Client socket created successfully" << endl;

    bzero(&serverAddr, sizeof(SA_IN));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr((const char *)ipAddr);
}

// connects client to the server
void Client::clientConnect()
{
    ret = connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (ret < 0)
    {
        perror("connect() error: ");
        exit(EXIT_FAILURE);
    }
    cout << "[+]Client connect to the server" << endl;
}

int Client::writeToFile(int clientFD, char *filename)
{
    // FILE *fp;
    char buff[MAX_BUFF] = {'\0'};
    int res = 0;

    ofstream file;
    file.open(filename);

    if (file.is_open())
    {
        // infinite loop to recevie data from server and store it in a file on client side
        while (true)
        {
            memset(buff, 0, MAX_BUFF);
            
            // log and return when recv() fail
            if (recv(clientFD, buff, sizeof(buff), 0)<0)
            {
                clientUtil.log("Fatal log: ", "recv() error", "logs/ServerData.log");
                return 0;
            }
            // Return 1 when EOF
            if (strcmp(buff, "EOF") == 0)
            {
                return 1;
            }

            // writing in file
            file << buff << endl;
        }
    }
    else
    {
        return 0;
    }
    // closing filestream object
    file.close();

    return 1;
    // fp = fopen(filename, "w");

    // if (fp == NULL)
    // {
    //     perror("file open error: ");
    // }
    // else
    // {
    //     while (1)
    //     {
    //         if (recv(clientFD, buff, MAX_BUFF, 0) < 0)
    //         {
    //             clientUtil.log("Fatal log: recv() error", "logs/ClientData.log");
    //             break;
    //         }

    //         cout << "Recv in writetoFile: " << buff << endl;

    //         // fprintf(fp, "%s", buff);
    //         fs << buff;

    //         memset(buff, 0, MAX_BUFF);
    //     }
}
bool isChoiceValid(char c)
{
    cout<<c<<endl;
    cout<<(int)c<<endl;
    if (c == 49 || c == 50 || c == 51)
        return true;

    return false;
}
void clientErrExit()
{
    cout << "Could not connect to server. Try again!" << endl;
    exit(EXIT_FAILURE);
}

// default deconstructor
Client::~Client()
{
}