#include <client.h>

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
    sleep(2);
    system("clear");
    // main menu
    if (menuType == 0)
    {
        cout << "Do you want to register or login?" << endl;
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

// read and write function with the server
void Client::clientreadwrite(char *str)
{
    char buffer[MAX_BUFF] = {
        '\0',
    };
    // read(sockfd, buffer, MAX_BUFF);
    recv(sockfd, buffer, MAX_BUFF, 0);

    cout << buffer << endl;
    memset(buffer, 0, sizeof(buffer));

    strcpy(buffer, (const char *)str);

    // write(sockfd, buffer, strlen(buffer));
    send(sockfd, buffer, strlen(buffer), 0);
    cout << "Data : \'" << buffer << "\' sent to server" << endl;
    close(sockfd);
}

// default deconstructor
Client::~Client()
{
}