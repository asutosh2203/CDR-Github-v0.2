#include <sys/stat.h>
#include <fcntl.h>
#include <server.h>

Utils ut;

Server::Server()
{
    // default constructor
}

/*
 *  FUNCTION NAME   : createSocket
 *
 *  DESCRIPTION     : It creates server socket and sets IP address and port number also.
 *
 *  PARAMETERS		: none
 *
 *  RETURN 		    : void
 *
 */
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
    server_addr.sin_addr.s_addr = inet_addr(ipAddr); //(const char*)ipAddr.c_str());

    cout << "[+] Server socket created Successfully" << endl;
}
/*
 *  FUNCTION NAME	: bind_listen()
 *
 *  DESCRIPTION		: It binds the server to a port using bind() system call
                      and enables server to listen to clients using listen() system call listen() system call
 *
 *  PARAMETERS		: none
 *
 *  RETURN 		    : void
 *
 */
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
} // end of blind_listen()

/*
 *  FUNCTION NAME	: acceptClient()
 *
 *  DESCRIPTION		: It creates a multi-client server enviorment by calling fork() system call
                        whenever a client gets accepted using accpet() call.
 *
 *  PARAMETERS		: none
 *
 *  RETURN 		    : void
 *
 */
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
        cout << "[+] Accepted the client " << ntohs(client_addr.sin_port) << endl;
        ut.log(INFO, "Server accepts the client", S_LOGFILE);

        if (fork() == 0)
        {
            initClient(clientSockfd);
        }
        close(clientSockfd);
    }
} // end of acceptClient()

/*
 *  FUNCTION NAME	: initClient()
 *
 *  DESCRIPTION		: It initiates client after accepting and shows
 *
 *  PARAMETERS		: none
 *
 *  RETURN 		    : void
 *
 */
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
        memset(&buf, 0, MAX_BUFF);
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
            // memset(&user, 0, MAX_BUFF);
            // bzero(&user, sizeof(User));

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
                // storing user details in database
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
            memset(buf, 0, MAX_BUFF);
            recv(newfd, buf, MAX_BUFF, 0);
            break;
        // login
        case 2:
            if (send(newfd, "login", strlen("login"), 0) < 0)
            {
                ut.log(FATAL, "send() error", S_LOGFILE);
            }

            // memset(&user, 0, sizeof(User));
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

                        if (flag == 1)
                            ut.log(FATAL, "recv() error", S_LOGFILE);
                        //
                    }

                    Operator op;
                    Customer cust;
                    bool isProcessed;
                    choice = atoi(buf);
                    switch (choice)
                    {
                    case 1: // Process CDR

                        isProcessed = processCallData(op, cust);

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

                                        if (buf[0] == '\0')
                                            break;

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
                                            if (send(newfd, "sending", strlen("sending"), 0) < 0)
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
                                        break;

                                    default:
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
                                            if (send(newfd, "sending", strlen("sending"), 0) < 0)
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
                                        break;

                                    default:
                                        break;
                                    }

                                    if (choice == 3 || buf[0] == '\0')
                                    {
                                        choice = 0;
                                        break;
                                    }
                                }
                                break;
                            case 3: // go back
                                break;

                            default:
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
            break;

        default:
            break;
        }

        if (buf[0] == '\0')
        {
            cout << "[-] Client " << ntohs(client_addr.sin_port) << " left the server." << endl;
            break;
        }
    }
} // end of initClient()

/*
 *  FUNCTION NAME	: userExists()
 *
 *  DESCRIPTION		: It checks the given username exists in the database or not,
                      A return value of 1 indicates user exists, 0 indicates user does not exists and -1 indicates database error.
 *
 *  PARAMETERS		: User &newUser
 *
 *  RETURN 		    : int
 *
 */
int Server::userExists(User &newUser)
{
    User user;
    fstream userDB;

    // make directory if not exists
    mkdir("data", 0777);
    userDB.open(USER_DB, ios::out | ios::in);

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
} // end of userExists()

/*
 *  FUNCTION NAME	: verifyLoginCreds()
 *
 *  DESCRIPTION		: It verifies the given credentials from database,
                      A return value of 1 indicates successful verification, 0 indicates unsucessful verification and -1 indicates database error.
 *
 *  PARAMETERS		: User &newUser
 *
 *  RETURN 		    : int
 *
 */
int Server::verifyLoginCreds(User &newUser)
{
    User user;
    fstream userDB;

    userDB.open(USER_DB, ios::out | ios::in);

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
} // end of verifyLoginCreds()

/*
 *  FUNCTION NAME	: processCallData()
 *
 *  DESCRIPTION		: It invokes processAndCreateFile() function for objects of both Operator and Customer classes parallely through threads,
                      and returns true if both functions return true, and returns false if any fails.
 *
 *  PARAMETERS		: User &op, Customer &cust
 *
 *  RETURN 		    : bool
 *
 */
bool Server::processCallData(Operator &op, Customer &cust)
{

    promise<bool> opPromise;
    promise<bool> custPromise;

    thread t1(&Operator::processAndCreateFile, &op, &opPromise);
    thread t2(&Customer::processAndCreateFile, &cust, &custPromise);

    if (t1.joinable())
        t1.join();

    if (t2.joinable())
        t2.join();

    future<bool> opFuture = opPromise.get_future();
    future<bool> custFuture = custPromise.get_future();

    if (opFuture.get() && custFuture.get())
    {
        return true;
    }
    return false;
} // end of processCallData()

/*
 *  FUNCTION NAME	: sendFile()
 *
 *  DESCRIPTION		: It send file to client by streaming the data 
                      A return value of 1 indicates successful transfer, 0 indicates unsuccessful transfer. 
 *
 *  PARAMETERS		: int newfd, char *filename
 *
 *  RETURN 		    : int
 *
 */
int Server::sendFile(int newfd, char *filename)
{
    char bufr[MAX_BUFF] = {'\0'};

    string line;
    ifstream file;
    file.open(filename);

    if (file.is_open())
    {
        while (!file.eof())
        {
            getline(file, line);
            strcpy(bufr, line.c_str());
            if (send(newfd, bufr, sizeof(bufr), 0) < 0)
            {
                ut.log(FATAL, "file send() error", S_LOGFILE);
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
        if (send(newfd, "openErr", strlen("openErr"), 0) < 0)
        {
            ut.log(FATAL, "Could not send file.", S_LOGFILE);
            return 0;
        }
    }
    file.close();
    return 1;
} // end of senfile()

// function to close the server socket
void Server::closeServer()
{
    close(serverSockfd);
}