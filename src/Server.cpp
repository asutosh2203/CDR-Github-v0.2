#include <server.h>

Utils ut;

Server::Server()
{
    port = 8080;
}
// Creation of server socket using socket() system call.
void Server::createSocket()
{
    serverSockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSockfd < 0)
    {
        ut.log(FATAL, "socket() error", S_LOGFILE);
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //(const char*)ipAddr.c_str());

    cout << "[+] Server socket created Successfully" << endl;
}

// Bind the server to a port using bind() system call
// and listening to clients using listen() system call
void Server::bind_listen()
{
    if (bind(serverSockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        ut.log(FATAL, "bind() error", S_LOGFILE);
        perror("bind() error: ");
        exit(EXIT_FAILURE);
    }
    cout << "[+] Server bind to port" << endl;
    if (listen(serverSockfd, 7) < 0)
    {
        ut.log(FATAL, "listen() error", S_LOGFILE);
        exit(EXIT_FAILURE);
    }
    cout << "[+] Server listening to the clients" << endl;
}

void Server::acceptClient()
{
    len = sizeof(client_addr);
    while (true)
    {
        clientSockfd = accept(serverSockfd, (struct sockaddr *)&client_addr, &len);
        if (clientSockfd < 0)
        {
            ut.log(FATAL, "accept() error", S_LOGFILE);
        }
        cout << "[+]Accepted the client " << ntohs(client_addr.sin_port) << endl;
        ut.log(INFO, "Server accepts the client", S_LOGFILE);

        if (fork() == 0)
        {
            initClient(clientSockfd);
        }

        // initClient(clientSockfd);

        close(clientSockfd);
    }
}

void Server::initClient(int newfd)
{
    User user;

    char buf[MAX_BUFF] = {
        '\0',
    };

    int flag = 0;

    // fstream fs;
    string line;
    // MAIN MENU:
    while (1)
    {
        // receiving choice for main menu from clien side
        recv(newfd, buf, MAX_BUFF, 0);
        // connvert buffer value to int
        int choice = atoi(buf);
        int isExists = -1;
        switch (choice)
        {
        // Resgistration
        case 1:
            if (send(newfd, "register", strlen("register"), 0) < 0)
            {
                ut.log(FATAL, "send() error", S_LOGFILE);
            }

            // Receieving user UserID and Password in user object
            memset(&user, 0, MAX_BUFF);
            
            if (recv(newfd, &user, sizeof(User), 0) < 0)
            {
                ut.log(FATAL, "recv() error", S_LOGFILE);
            }
           
            isExists = userExists(user);
            if (isExists == 1)
            {
                if (send(newfd, "exists", strlen("exists"), 0) < 0)
                {
                    ut.log(FATAL, "send() error", S_LOGFILE);
                }
            }
            else if (isExists == 0)
            {
                user.toDatabase(user);

                if (send(newfd, "success", strlen("success"), 0) < 0)
                {
                    ut.log(FATAL, "send() error", S_LOGFILE);
                }
            }
            else
            {
                if (send(newfd, "failed", strlen("failed"), 0) < 0)
                {
                    ut.log(FATAL, "send() error", S_LOGFILE);
                }
            }
            break;
        // login
        case 2:
            if (send(newfd, "login", strlen("login"), 0) < 0)
            {
                ut.log(FATAL, "send() error", S_LOGFILE);
            }

            memset(&user, 0, sizeof(User));
            // Receives the login credentials
            if (recv(newfd, &user, sizeof(User), 0) < 0)
            {
                ut.log(FATAL, "recv() error", S_LOGFILE);
            }

            // Verify if the login credentials matches the registered credentials
            flag = verifyLoginCreds(user);

            if (flag == 1)
            {
                if (send(newfd, "loggedIn", strlen("loggedIn"), 0) < 0)
                {
                    ut.log(FATAL, "send() error", S_LOGFILE);
                }

                while (1)
                {
                    // resetting the buffer after login successful
                    memset(&buf, 0, MAX_BUFF);

                    // receiving option from main menu
                    if (recv(newfd, buf, MAX_BUFF, 0) < 0)
                    {
                        ut.log(FATAL, "recv() error", S_LOGFILE);
                        //
                    }

                    Operator op;
                    Customer cust;
                    bool isProcessed = true;
                    choice = atoi(buf);
                    switch (choice)
                    {
                    case 1: // Process CDR
                        // isProcessed = op.processCDR();
                        // op.mapToFile();

                        processCallData(op, cust);

                        isProcessed == true ? send(newfd, "processed", strlen("processed"), 0) : send(newfd, "notprocessed", strlen("notprocessed"), 0);
                        break;

                    case 2: // Billing info
                        while (1)
                        {
                            memset(&buf, 0, MAX_BUFF);

                            // receiving option from billing info menu
                            if (recv(newfd, buf, MAX_BUFF, 0) < 0)
                            {
                                ut.log(FATAL, "recv() error", S_LOGFILE);
                                //
                            }
                            choice = atoi(buf);
                            switch (choice)
                            {
                            case 1: // Customer billing
                                while (1)
                                {
                                    memset(&buf, 0, MAX_BUFF);

                                    // receiving option from customer billing menu
                                    if (recv(newfd, buf, MAX_BUFF, 0) < 0)
                                    {
                                        ut.log(FATAL, "recv() error", S_LOGFILE);
                                        //
                                    }
                                    string msisdnSearchResult;

                                    choice = atoi(buf);
                                    switch (choice)
                                    {
                                    case 1:
                                        if (send(newfd, "searchmsisdn", strlen("searchmsisdn"), 0) < 0)
                                        {
                                            ut.log(FATAL, "send() error", S_LOGFILE);
                                            //
                                        }

                                        memset(&buf, 0, MAX_BUFF);

                                        // receiving MSISDN to be searched
                                        if (recv(newfd, buf, MAX_BUFF, 0) < 0)
                                        {
                                            ut.log(FATAL, "recv() error", S_LOGFILE);
                                            //
                                        }

                                        cout << "MSISDN received: " << buf << endl;
                                        cust.processCDR();
                                        msisdnSearchResult = cust.searchMSISDN(stol(buf));

                                        if (send(newfd, msisdnSearchResult.c_str(), msisdnSearchResult.length(), 0) < 0)
                                        {
                                            ut.log(FATAL, "send() error", S_LOGFILE);
                                        }

                                        break;
                                    case 2:
                                        if (cust.processCDR() && cust.mapToFile())
                                        {

                                            // sending file to client side
                                            if (send(newfd, "sending", strlen("sending"), 0))
                                            {
                                                ut.log(FATAL, "send() error", S_LOGFILE);
                                            }

                                            memset(&buf, 0, MAX_BUFF);

                                            if (recv(newfd, buf, MAX_BUFF, 0) < 0)
                                            {
                                                ut.log(FATAL, "recv() error", S_LOGFILE);
                                            }

                                            if (strcmp(buf, "yes") == 0)
                                            {

                                                if (sendFile(newfd, (char *)"data/CB.txt") == 1)
                                                {
                                                    ut.log(INFO, "File sent succesffuly.", S_LOGFILE);
                                                }
                                                else
                                                {
                                                    ut.log(FATAL, "File not accessible.", S_LOGFILE);
                                                }
                                            }

                                            // Receiving acknowledgement from client
                                            memset(&buf, 0, MAX_BUFF);
                                            if (recv(newfd, buf, MAX_BUFF, 0) < 0)
                                            {
                                                ut.log(FATAL, "recv() error", S_LOGFILE);
                                                break;
                                            }
                                            // Logging ack in logs

                                            if (strcmp(buf, "SUCCESS") == 0)
                                            {
                                                // cout << "Anknowledgment received: " << buf << endl;
                                                ut.log(INFO, "Client Received the file successfully", S_LOGFILE);
                                            }
                                            else
                                            {
                                                ut.log(INFO, "Client does not received the file", S_LOGFILE);
                                            }
                                        }
                                        else
                                        {
                                            if (send(newfd, "error", strlen("error"), 0))
                                            {
                                                ut.log(FATAL, "send() error", S_LOGFILE);
                                            }
                                        }

                                        break;

                                    case 3:
                                        cout << "Exiting" << endl;
                                        break;

                                    default:
                                        cout << "Invalid choice in CB menu" << endl;
                                        break;
                                    }

                                    if (choice == 3 || buf[0] == '\0')
                                    {
                                        choice = 0;
                                        break;
                                    }
                                }
                                break;

                            case 2: // interoperator billing
                                while (1)
                                {

                                    cout << "IB MENU START: " << endl;

                                    memset(&buf, 0, MAX_BUFF);

                                    // receiving option from interoperator billing menu
                                    if (recv(newfd, buf, MAX_BUFF, 0) < 0)
                                    {
                                        ut.log(FATAL, "recv() error", S_LOGFILE);
                                    }

                                    string brandSearchResult;
                                    choice = atoi(buf);

                                    switch (choice)
                                    {
                                    case 1:
                                        if (send(newfd, "searchbrand", strlen("searchbrand"), 0) < 0)
                                        {
                                            ut.log(FATAL, "send() error", S_LOGFILE);
                                            //
                                        }

                                        memset(&buf, 0, MAX_BUFF);

                                        // receiving brand name to be searched
                                        if (recv(newfd, buf, MAX_BUFF, 0) < 0)
                                        {
                                            ut.log(FATAL, "recv() error", S_LOGFILE);
                                            //
                                        }

                                        cout << "Brand name to be searched: " << buf << endl;
                                        op.processCDR();
                                        brandSearchResult = op.searchBrandName(buf);

                                        if (send(newfd, brandSearchResult.c_str(), brandSearchResult.length(), 0) < 0)
                                        {
                                            ut.log(FATAL, "send() error", S_LOGFILE);
                                        }

                                        break;

                                    case 2:

                                        // function for sending IOSB.txt to client

                                        if (op.processCDR() && op.mapToFile())
                                        {
                                            // sending file to client side
                                            if (send(newfd, "sending", strlen("sending"), 0))
                                            {
                                                ut.log(FATAL, "send() error", S_LOGFILE);
                                            }

                                            memset(&buf, 0, MAX_BUFF);

                                            if (recv(newfd, buf, MAX_BUFF, 0) < 0)
                                            {
                                                ut.log(FATAL, "recv() error", S_LOGFILE);
                                            }

                                            if (strcmp(buf, "yes") == 0)
                                            {
                                                cout << "in if containing sendfile" << endl;
                                                if (sendFile(newfd, (char *)"data/IOSB.txt") == 1)
                                                {
                                                    ut.log(INFO, "File sent succesffuly.", S_LOGFILE);
                                                }
                                                else
                                                {
                                                    ut.log(FATAL, "File not accessible.", S_LOGFILE);
                                                }
                                            }

                                            // Receiving acknowledgement from client
                                            memset(&buf, 0, MAX_BUFF);
                                            if (recv(newfd, buf, MAX_BUFF, 0) < 0)
                                            {
                                                ut.log(FATAL, "recv() error", S_LOGFILE);
                                                break;
                                            }
                                            // Logging ack in logs

                                            if (strcmp(buf, "SUCCESS") == 0)
                                            {
                                                // cout << "Anknowledgment received: " << buf << endl;
                                                ut.log(INFO, "Client Received the file successfully", S_LOGFILE);
                                            }
                                            else
                                            {
                                                ut.log(INFO, "Client does not received the file", S_LOGFILE);
                                            }
                                        }
                                        else
                                        {
                                            if (send(newfd, "error", strlen("error"), 0))
                                            {
                                                ut.log(FATAL, "send() error", S_LOGFILE);
                                            }
                                        }

                                        break;

                                    case 3:
                                        cout << "Exiting..." << endl;
                                        break;

                                    default:
                                        cout << "Invalid Input in IOSB menu" << endl;
                                        break;
                                    }

                                    if (choice == 3 || buf[0] == '\0')
                                    {
                                        choice = 0;
                                        break;
                                    }
                                }

                            case 3: // go back
                                cout << "Exiting..." << endl;
                                break;

                            default:
                                cout << "Invalid Input in billing info" << endl;
                                break;
                            }

                            if (choice == 3 || buf[0] == '\0')
                            {
                                choice = 0;
                                break;
                            }
                        }
                        break;

                    case 3: // Logout
                        break;

                    default:
                        cout << "Invalid Input in Main menu" << endl;
                        break;
                    }
                    if (choice == 3 || buf[0] == '\0')
                    {
                        choice = 0;
                        break;
                    }
                }
            }
            else if (flag == 0)
            {
                if (send(newfd, "failure", 8, 0) < 0)
                {
                    ut.log(FATAL, "send() error", S_LOGFILE);
                }
            }
            else
            {
                if (send(newfd, "DBerror", 8, 0) < 0)
                {
                    ut.log(FATAL, "send() error", S_LOGFILE);
                }
            }
            break;

        case 3:
            exit(EXIT_SUCCESS);

        default:
            cout << "Invalid input in welcome menu" << endl;
            break;
        }

        if (buf[0] == '\0')
            break;
    }
}

// return -1 on database error, 1 on user exists, 0 on user not exists
int Server::userExists(User &newUser)
{
    User user;
    fstream userDB;

    userDB.open("data/registered.dat", ios::out | ios::in);

    if (userDB)
    {
        while (!userDB.eof())
        {

            userDB.read((char *)&user, sizeof(user));

            if (strcmp(newUser.getUsername(), user.getUsername()) == 0)
            {
                return 1;
            }
        }
    }
    else
    {
        return -1;
    }

    return 0;
}

// return 1 on successful login, 0 on unsuccessful and -1 on DB error
int Server::verifyLoginCreds(User &newUser)
{
    User user;
    fstream userDB;

    userDB.open("data/registered.dat", ios::out | ios::in);

    if (userDB)
    {
        while (!userDB.eof())
        {

            userDB.read((char *)&user, sizeof(user));

            if ((strcmp(newUser.getUsername(), user.getUsername()) == 0) && (strcmp(newUser.getPassword(), user.getPassword()) == 0))
            {
                return 1;
            }
        }
    }
    else
    {
        return -1;
    }

    return 0;
}

void Server::processCallData(Operator &op, Customer &cust)
{
    thread t1(&Operator::processAndCreateFile, &op);
    thread t2(&Customer::processAndCreateFile, &cust);

    if (t1.joinable())
        t1.join();

    if (t2.joinable())
        t2.join();
}

int Server::sendFile(int newfd, char *filename)
{
    char bufr[MAX_BUFF] = {'\0'};

    if (recv(newfd, bufr, MAX_BUFF, 0) < 0)
    {
        ut.log("Fatal log: ", "recv() error", "logs/ServerData.log");
        return 0;
    }

    string line;
    ifstream file;
    file.open(filename);

    if (file.is_open() && (strcmp(bufr, "openSuccess") == 0))
    {
        while (!file.eof())
        {
            getline(file, line);
            strcpy(bufr, line.c_str());
            if (send(newfd, bufr, sizeof(bufr), 0) < 0)
            {
                ut.log(FATAL, "bufr send() error", S_LOGFILE);
                return 0;
            }

            memset(bufr, 0, MAX_BUFF);
        }
        // Sending EOF msg to client
        if (send(newfd, "EOF", strlen("EOF"), 0) < 0)
        {
            ut.log(FATAL, "EOF send() error", S_LOGFILE);
            return 0;
        }
    }
    else
    {
        ut.log(FATAL, "Could not send file.", S_LOGFILE);
        return 0;
    }
    file.close();
    return 1;
}

void Server::closeServer()
{
    close(serverSockfd);
}