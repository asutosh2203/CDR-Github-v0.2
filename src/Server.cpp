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
        ut.log("Fatal log: socket() error", "logs/ServerData.log");
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
        ut.log("Fatal log: bind() error", "logs/ServerData.log");
        perror("bind() error: ");
        exit(EXIT_FAILURE);
    }
    cout << "[+] Server bind to port" << endl;
    if (listen(serverSockfd, 7) < 0)
    {
        ut.log("Fatal log: listen() error", "logs/ServerData.log");
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
            ut.log("Fatal log: accept() error", "logs/ServerData.log");
            exit(EXIT_FAILURE);
        }
        cout << "[+]Accepted the client " << ntohs(client_addr.sin_port) << endl;
        ut.log("Info log: Server accepts the client", "logs/ServerData.log");

        if (fork() == 0)
        {
            registerLoginUser(clientSockfd);
            cout << getpid() << endl;
        }
        close(clientSockfd);
    }
}

void Server::registerLoginUser(int newfd)
{
    User *d1 = new User;
    char user_data[MAX_BUFF] = {
        '\0',
    };
    char buf[MAX_BUFF] = {
        '\0',
    };

    int flag = 0;

    fstream fs;
    string line;
    while (1)
    {
        recv(newfd, buf, MAX_BUFF, 0);
        // check whether client wants to login or register

        cout << "Server side buff: " << buf << endl;

        char *username;

        switch (atoi(buf))
        {
        // registeration
        case 1:
            if (send(newfd, "register", 9, 0) < 0)
            {
                ut.log("Fatal log: send() error", "logs/ServerData.log");
                exit(EXIT_FAILURE);
            }
            // Receieved UserID and Password
            memset(&user_data, 0, MAX_BUFF);
            if (recv(newfd, &user_data, sizeof(user_data), 0) < 0)
            {
                ut.log("Fatal log: recv() error", "logs/ServerData.log");
                exit(EXIT_FAILURE);
            }
            // Stores registered users in a file

            char user[MAX_BUFF];

            strcpy(user, user_data);

            username = strtok(user, "|");

            if (userExists(username))
            {
                if (send(newfd, "exists", strlen("exists"), 0) < 0)
                {
                    ut.log("Fatal log: send() error", "logs/ServerData.log");
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                d1->database(user_data);

                if (send(newfd, "success", strlen("success"), 0) < 0)
                {
                    ut.log("Fatal log: send() error", "logs/ServerData.log");
                    exit(EXIT_FAILURE);
                }
            }
            break;
        // login
        case 2:
            if (send(newfd, "login", strlen("login"), 0) < 0)
            {
                ut.log("Fatal log: send() error", "logs/ServerData.log");
                exit(EXIT_FAILURE);
            }
            memset(user_data, 0, sizeof(user_data));
            // Receives the login credentials
            if (recv(newfd, &user_data, sizeof(user_data), 0) < 0)
            {
                ut.log("Fatal log: recv() error", "logs/ServerData.log");
                exit(EXIT_FAILURE);
            }

            // Verify if the login credentials matches the registered credentials
            fs.open("data/registered.dat");

            if (fs.is_open())
            {
                while (!fs.eof())
                {
                    getline(fs, line);
                    if ((strcmp(user_data, line.c_str()) == 0))
                    {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 1)
                {
                    if (send(newfd, "loggedIn", strlen("loggedIn"), 0) < 0)
                    {
                        ut.log("Fatal log: send() error", "logs/ServerData.log");
                        exit(EXIT_FAILURE);
                    }

                    while (1)
                    {
                        // resetting the buffer after login successful
                        memset(&buf, 0, MAX_BUFF);

                        // receiving option from main menu
                        if (recv(newfd, buf, MAX_BUFF, 0) < 0)
                        {
                            ut.log("Fatal log: recv() error", "logs/ServerData.log");
                            // exit(EXIT_FAILURE);
                        }

                        Operator op;
                        Customer cust;
                        bool isProcessed = true;
                        switch (atoi(buf))
                        {
                        case 1:
                            // isProcessed = op.processCDR();
                            // op.mapToFile();

                            processCallData(op, cust);

                            isProcessed == true ? send(newfd, "processed", strlen("processed"), 0) : send(newfd, "notprocessed", strlen("notprocessed"), 0);
                            break;

                        case 2:
                            while (1)
                            {
                                memset(&buf, 0, MAX_BUFF);

                                // receiving option from billing info menu
                                if (recv(newfd, buf, MAX_BUFF, 0) < 0)
                                {
                                    ut.log("Fatal log: recv() error", "logs/ServerData.log");
                                    // exit(EXIT_FAILURE);
                                }

                                switch (atoi(buf))
                                {
                                case 1:
                                    while (1)
                                    {
                                        memset(&buf, 0, MAX_BUFF);

                                        // receiving option from customer billing menu
                                        if (recv(newfd, buf, MAX_BUFF, 0) < 0)
                                        {
                                            ut.log("Fatal log: recv() error", "logs/ServerData.log");
                                            // exit(EXIT_FAILURE);
                                        }
                                        string msisdnSearchResult;
                                        bool isCBgen = true;
                                        switch (atoi(buf))
                                        {
                                        case 1:
                                            if (send(newfd, "searchmsisdn", strlen("searchmsisdn"), 0) < 0)
                                            {
                                                ut.log("Fatal log: send() error", "logs/ServerData.log");
                                                // exit(EXIT_FAILURE);
                                            }

                                            memset(&buf, 0, MAX_BUFF);

                                            // receiving MSISDN to be searched
                                            if (recv(newfd, buf, MAX_BUFF, 0) < 0)
                                            {
                                                ut.log("Fatal log: recv() error", "logs/ServerData.log");
                                                // exit(EXIT_FAILURE);
                                            }

                                            cout << "MSISDN received: " << buf << endl;
                                            cust.processCDR();
                                            msisdnSearchResult = cust.searchMSISDN(stol(buf));

                                            if (send(newfd, msisdnSearchResult.c_str(), msisdnSearchResult.length(), 0) < 0)
                                            {
                                                ut.log("Fatal log: send() error", "logs/ServerData.log");
                                            }

                                            break;
                                        case 2:
                                            if (isCBgen)
                                            {
                                                cust.processCDR();
                                                cust.mapToFile();
                                                cout << "Putting all Customer bills into CB.txt" << endl;
                                                if (send(newfd, "generated", strlen("generated"), 0))
                                                {
                                                    ut.log("Fatal log: send() error", "logs/ServerData.log");
                                                    // exit(EXIT_FAILURE);
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

                                        if (atoi(buf) == 3)
                                            break;
                                    }
                                    break;

                                case 2:
                                    while (1)
                                    {
                                        memset(&buf, 0, MAX_BUFF);

                                        // receiving option from interoperator billing menu
                                        if (recv(newfd, buf, MAX_BUFF, 0) < 0)
                                        {
                                            ut.log("Fatal log: recv() error", "logs/ServerData.log");
                                            // exit(EXIT_FAILURE);
                                        }
                                        string brandSearchResult;
                                        bool isIOSBgen = true;

                                        switch (atoi(buf))
                                        {
                                        case 1:
                                            if (send(newfd, "searchbrand", strlen("searchbrand"), 0) < 0)
                                            {
                                                ut.log("Fatal log: send() error", "logs/ServerData.log");
                                                // exit(EXIT_FAILURE);
                                            }

                                            memset(&buf, 0, MAX_BUFF);

                                            // receiving brand name to be searched
                                            if (recv(newfd, buf, MAX_BUFF, 0) < 0)
                                            {
                                                ut.log("Fatal log: recv() error", "logs/ServerData.log");
                                                // exit(EXIT_FAILURE);
                                            }

                                            cout << "Brand name received: " << buf << endl;
                                            op.processCDR();
                                            brandSearchResult = op.searchBrandName(buf);

                                            if (send(newfd, brandSearchResult.c_str(), brandSearchResult.length(), 0) < 0)
                                            {
                                                ut.log("Fatal log: send() error", "logs/ServerData.log");
                                            }

                                            break;

                                        case 2:

                                            // function for sending IOSB.txt to client

                                            if (isIOSBgen)
                                            {
                                                if (send(newfd, "generated", strlen("generated"), 0))
                                                {
                                                    ut.log("Fatal log: send() error", "logs/ServerData.log");
                                                    // exit(EXIT_FAILURE);
                                                }
                                            }
                                            else
                                            {
                                                if (send(newfd, "generateErr", strlen("generateErr"), 0))
                                                {
                                                    ut.log("Fatal log: send() error", "logs/ServerData.log");
                                                    // exit(EXIT_FAILURE);
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

                                        if (atoi(buf) == 3)
                                            break;
                                    }

                                case 3:
                                    cout << "Exiting..." << endl;
                                    break;

                                default:
                                    cout << "Invalid Input in billing info" << endl;
                                    break;
                                }

                                if (atoi(buf) == 3)
                                    break;
                            }
                            break;

                        case 3:
                            break;

                        default:
                            cout << "Invalid Input in menu info" << endl;
                            break;
                        }

                        if (atoi(buf) == 3)
                            break;
                    }
                }
                else
                {
                    if (send(newfd, "failure", 8, 0) < 0)
                    {
                        ut.log("Fatal log: recv() error", "logs/ServerData.log");
                        exit(EXIT_FAILURE);
                    }
                }
            }
            // line = "";
            fs.close();
            break;

        case 3:
            exit(EXIT_SUCCESS);

        default:
            cout << "Invalid input in welcome menu" << endl;
            break;
        }
    }
}

bool Server::userExists(char *username)
{
    fstream userDB;
    string line;

    userDB.open("data/registered.dat");

    if (userDB)
    {
        while (getline(userDB, line))
        {
            if (strcmp(username, strtok((char *)line.c_str(), "|")) == 0)
                return true;
        }
    }

    return false;
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

void Server::closeServer()
{
    close(serverSockfd);
}