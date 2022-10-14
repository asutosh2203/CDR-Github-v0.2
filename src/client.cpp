#include <client.h>
#include <sys/stat.h>
#include <fcntl.h>

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
    system("clear");

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
        cout << "\t2. Download Customer Billing Data" << endl;
        cout << "\t3. Go Back" << endl;
    }
    // interoperator billing menu
    else
    {
        cout << "====== INTEROPERATOR BILLING MENU ======" << endl;
        cout << "\t1. Search for Brand Name" << endl;
        cout << "\t2. Download Interoperator Billing Data" << endl;
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
    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("connect() error: ");
        exit(EXIT_FAILURE);
    }
    clientUtil.log(INFO, "Connection Established", C_LOGFILE);
    cout << "\t\t\t\t WELCOME \n";
}

int Client::writeToFile(int clientFD, char *filename)
{
    // creating data/downloads
    mkdir("data", 0777);
    int dataFD = open("data", O_DIRECTORY);
    mkdirat(dataFD, "downloads", 0777);

    // FILE *fp;
    char buff[MAX_BUFF] = {'\0'};

    ofstream file;
    file.open(filename);

    if (file.is_open())
    {

        // infinite loop to recevie data from server and store it in a file on client side
        while (true)
        {
            memset(buff, 0, MAX_BUFF);

            // log and return when recv() fail
            if (recv(clientFD, buff, MAX_BUFF, 0) < 0)
            {
                clientUtil.log(FATAL, "recv() error", C_LOGFILE);
                return 0;
            }
            // Return 1 when EOF
            if (strcmp(buff, "EOF") == 0)
            {
                return 1;
            }

            if (strcmp(buff, "openErr") == 0)
            {
                return 0;
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
}

int getUserChoice()
{
    int choice;

    while (1)
    {
        cin >> choice;

        if (cin.fail())
        {
            // get rid of failure state
            cin.clear();

            // discard 'bad' character(s)
            cin.ignore(INT64_MAX, '\n');
        }

        if (choice == 1 || choice == 2 || choice == 3)
        {
            break;
        }
        else
        {
            cout << "Invalid Choice. Please choose option: ";
        }
        // cin.ignore();
    }

    // clearing cin buffer
    // cin.clear();
    return choice;
}

void clientErrExit()
{
    cout << "Could not connect to server. Try again!" << endl;
    perror("Client error ");
    exit(EXIT_FAILURE);
}

void pressEnter()
{
    cout << "Press ENTER to continue";
    // cin.ignore();
    getchar();
}

// validate user input
// 0 - validate MSISDN, 1 - validate Brand name
bool validateInput(string str, int inputType)
{
    int l = str.length();

    // validate MSISDN
    if (inputType == 0)
    {
        if (l != 7)
        {
            return false;
        }

        for (int i = 0; i < l; i++)
        {
            char s = str.at(i);

            if (s == ' ')
            {
                return false;
            }

            if (s < 48 || s > 57)
            {
                return false;
            }
        }

        return true;
    }

    // validate Brand name
    if (inputType == 1)
    {
        for (int i = 0; i < l; i++)
        {
            char s = str.at(i);
            if (s == 10)
            {
                return false;
            }
        }

        return true;
    }

    return false;
}

// default deconstructor
Client::~Client()
{
}
