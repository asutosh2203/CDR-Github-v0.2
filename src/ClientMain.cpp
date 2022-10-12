#include <client.h>

Utils ut;
Client newClient;

// signal handling
void signalHandler(int sig)
{
    if (sig == SIGINT || sig == SIGTSTP)
    {
        cout << endl
             << "Program Terminated."
             << endl;
    }

    ut.log(INFO, "User left", C_LOGFILE);

    close(newClient.getClientSockfd());
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{

    // Signal Handling
    signal(SIGINT, signalHandler);
    signal(SIGTSTP, signalHandler);

    newClient.createSocket();
    newClient.clientConnect();

    int choice;
    int clientFD = newClient.getClientSockfd();

    User newUser;
    char buf[MAX_BUFF];

    while (1)
    {
        showMenu(0);

        choice = getUserChoice();

        switch (choice)
        {
        // Registration
        case 1: // ASCII value of 1
            send(clientFD, "1", 2, 0);
            memset(&buf, 0, MAX_BUFF);
            recv(clientFD, buf, sizeof(buf), 0);
            if (strcmp(buf, "register") == 0)
            {
                newUser.setDetails();
                // string str = newUser.toString();
                // send(clientFD, str.c_str(), str.length(), 0);
                send(clientFD, &newUser, sizeof(User), 0);
            }
            memset(&buf, 0, MAX_BUFF);
            recv(clientFD, buf, sizeof(buf), 0);
            if (strcmp(buf, "success") == 0)
            {
                cout << endl;
                cout << "Registration successful" << endl;
                send(clientFD, "success", strlen("success"), 0);
            }
            else if (strcmp(buf, "exists") == 0)
            {
                cout << endl;
                cout << "Username already exists.\nCreate a new username or login with your existing account." << endl;
                send(clientFD, "failure", strlen("failure"), 0);
            }
            else
            {
                cout << endl;
                cout << "Registration unsuccessful" << endl;
                send(clientFD, "failure", strlen("failure"), 0);
            }
            pressEnter();
            break;

        // login
        case 2: // ASCII value of 2
            send(clientFD, "2", 2, 0);
            memset(&buf, 0, MAX_BUFF);
            recv(clientFD, buf, sizeof(buf), 0);

            if (strcmp(buf, "login") == 0)
            {
                memset(&newUser, 0, sizeof(User));
                newUser.setDetails();
                // string str1 = newUser.toString();
                send(clientFD, &newUser, sizeof(User), 0);
                memset(&buf, 0, MAX_BUFF);
                recv(clientFD, buf, sizeof(buf), 0);
                if (strcmp(buf, "loggedIn") == 0)
                {
                    cout << "Login Successful" << endl;

                    while (1)
                    {
                        sleep(2);
                        showMenu(1);
                        choice = getUserChoice();
                        switch (choice)
                        {
                        case 1: // Process CDR
                            if (send(clientFD, "1", 2, 0) < 0)
                            {
                                cout << "Could not connect to server" << endl;
                                ut.log(FATAL, "send() error", C_LOGFILE);
                                exit(EXIT_FAILURE);
                            }
                            // op.processCDR();
                            memset(&buf, 0, MAX_BUFF);
                            recv(clientFD, buf, sizeof(buf), 0);
                            if (strcmp(buf, "processed") == 0)
                            {
                                cout << "CDR file processed successfully." << endl;
                            }
                            else
                            {
                                ut.log(INFO, "Cannot process CDR file", C_LOGFILE);
                                cout << "CDR file cannot be processed." << endl;
                            }
                            pressEnter();
                            break;
                        case 2: // Billing Info
                            if (send(clientFD, "2", 2, 0) < 0)
                            {
                                cout << "Could not connect to server" << endl;
                                ut.log(FATAL, "send() error", C_LOGFILE);
                                exit(EXIT_FAILURE);
                            }
                            while (1)
                            {
                                showMenu(2);
                                // cout << "====== BILLING INFO MENU ======" << endl;

                                choice = getUserChoice();
                                switch (choice)
                                {
                                case 1: // Customer Billing
                                    if (send(clientFD, "1", 2, 0) < 0)
                                    {
                                        cout << "Could not connect to server" << endl;
                                        ut.log(FATAL, "send() error", C_LOGFILE);
                                        exit(EXIT_FAILURE);
                                    }
                                    while (1)
                                    {
                                        // shows customer billing menu
                                        showMenu(3);

                                        choice = getUserChoice();

                                        long MSISDN = 0;

                                        switch (choice)
                                        {
                                        case 1:
                                            if (send(clientFD, "1", 2, 0) < 0)
                                            {
                                                cout << "Could not connect to server" << endl;
                                                ut.log(FATAL, "send() error", C_LOGFILE);
                                                exit(EXIT_FAILURE);
                                            }

                                            memset(&buf, 0, MAX_BUFF);

                                            if (recv(clientFD, buf, MAX_BUFF, 0) < 0)
                                            {
                                                cout << "Could not connect to server" << endl;
                                                ut.log(FATAL, "recv() error", C_LOGFILE);
                                                exit(EXIT_FAILURE);
                                            }

                                            if (strcmp(buf, "searchmsisdn") == 0)
                                            {

                                                cout << "Enter MSISDN to be searched: ";
                                                cin >> MSISDN;

                                                string sdn = to_string(MSISDN);

                                                // sending msisdn to be searched
                                                if (send(clientFD, sdn.c_str(), MAX_BUFF, 0) < 0)
                                                {
                                                    cout << "Could not connect to server" << endl;
                                                    ut.log(FATAL, "send() error", C_LOGFILE);
                                                    exit(EXIT_FAILURE);
                                                }

                                                memset(&buf, 0, MAX_BUFF);

                                                // receiving msisdn search results
                                                if (recv(clientFD, buf, MAX_BUFF, 0) < 0)
                                                {
                                                    cout << "Could not connect to server" << endl;
                                                    ut.log(FATAL, "recv() error", C_LOGFILE);
                                                    exit(EXIT_FAILURE);
                                                }

                                                cout << buf << endl;
                                            }

                                            pressEnter();

                                            // for searching by MSISDN
                                            break;

                                        case 2:
                                            if (send(clientFD, "2", 2, 0) < 0)
                                            {
                                                cout << "Could not connect to server" << endl;
                                                ut.log(FATAL, "send() error", C_LOGFILE);
                                                exit(EXIT_FAILURE);
                                            }

                                            memset(&buf, 0, MAX_BUFF);
                                            if (recv(clientFD, buf, MAX_BUFF, 0) < 0)
                                            {
                                                cout << "Could not connect to server" << endl;
                                                ut.log(FATAL, "recv() error", C_LOGFILE);
                                                exit(EXIT_FAILURE);
                                            }

                                            cout << "Sending buff: " << buf << endl;

                                            if (strcmp(buf, "sending") == 0)
                                            {

                                                if (send(clientFD, "yes", strlen("yes"), 0) < 0)
                                                {
                                                    ut.log(FATAL, "send() error", C_LOGFILE);
                                                    // exit(EXIT_FAILURE);
                                                }

                                                // downloading file
                                                int isSuccess = newClient.writeToFile(clientFD, (char *)CB_DOWNLOAD);
                                                // cout<<"END OF WRITETOFILE"<<endl;
                                                if (isSuccess == 1)
                                                {
                                                    // send to server
                                                    if (send(clientFD, "SUCCESS", strlen("SUCCESS"), 0) < 0)
                                                    {
                                                        ut.log(FATAL, "Could not connect to server", C_LOGFILE);
                                                        clientErrExit();
                                                    }
                                                    else
                                                    {
                                                        // log to file
                                                        cout << "File downloaded Successfully in " << CB_DOWNLOAD << endl;
                                                        ut.log(INFO, "File Downloaded Successfully", C_LOGFILE);
                                                    }
                                                }
                                                else
                                                {
                                                    // send to server
                                                    if (send(clientFD, "FAILED", strlen("FAILED"), 0) < 0)
                                                    {
                                                        ut.log(FATAL, "Could not connect to server", C_LOGFILE);
                                                        clientErrExit();
                                                    }
                                                    else
                                                    { // log to file
                                                        cout << "Required file could not be generated, please try again." << endl;
                                                        ut.log(INFO, "File Downloading failed", C_LOGFILE);
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                // if error receive
                                                cout << "CDR Processing Failed. Try again!" << endl;
                                            }
                                            pressEnter();
                                            break;

                                        case 3:
                                            if (send(clientFD, "3", 2, 0) < 0)
                                            {
                                                cout << "Could not connect to server" << endl;
                                                ut.log(FATAL, "send() error", C_LOGFILE);
                                                exit(EXIT_FAILURE);
                                            }
                                            cout << "Going back..." << endl;
                                            sleep(2);
                                            break;

                                        default:
                                            break;
                                        }

                                        if (choice == 3)
                                        {
                                            choice = 0;
                                            break;
                                        }
                                    }
                                    break;

                                case 2:
                                    if (send(clientFD, "2", 2, 0) < 0)
                                    {
                                        cout << "Could not connect to server" << endl;
                                        ut.log(FATAL, "send() error", C_LOGFILE);
                                        exit(EXIT_FAILURE);
                                    }
                                    while (1)
                                    {
                                        // shows interop billing menu
                                        showMenu(-1);
                                        choice = getUserChoice();
                                        char brandName[MAX_BUFF] = {'\0'};
                                        switch (choice)
                                        {
                                        case 1:
                                            if (send(clientFD, "1", 2, 0) < 0)
                                            {
                                                cout << "Could not connect to server" << endl;
                                                ut.log(FATAL, "send() error", C_LOGFILE);
                                                exit(EXIT_FAILURE);
                                            }

                                            memset(&buf, 0, MAX_BUFF);

                                            if (recv(clientFD, buf, MAX_BUFF, 0) < 0)
                                            {
                                                cout << "Could not connect to server" << endl;
                                                ut.log(FATAL, "recv() error", C_LOGFILE);
                                                exit(EXIT_FAILURE);
                                            }

                                            if (strcmp(buf, "searchbrand") == 0)
                                            {
                                                cout << "Enter brand name to be searched: ";
                                                cin >> brandName;

                                                // sending brand name to be searched
                                                if (send(clientFD, brandName, MAX_BUFF, 0) < 0)
                                                {
                                                    cout << "Could not connect to server" << endl;
                                                    ut.log(FATAL, "send() error", C_LOGFILE);
                                                    exit(EXIT_FAILURE);
                                                }

                                                // receiving brand search results
                                                if (recv(clientFD, buf, MAX_BUFF, 0) < 0)
                                                {
                                                    cout << "Could not connect to server" << endl;
                                                    ut.log(FATAL, "recv() error", C_LOGFILE);
                                                    exit(EXIT_FAILURE);
                                                }

                                                cout << buf << endl;
                                            }
                                            pressEnter();
                                            break; // Brand Search end

                                        case 2: // Download IOSB.txt

                                            if (send(clientFD, "2", 2, 0) < 0)
                                            {
                                                cout << "Could not connect to server" << endl;
                                                ut.log(FATAL, "send() error", C_LOGFILE);
                                                exit(EXIT_FAILURE);
                                            }

                                            memset(&buf, 0, MAX_BUFF);
                                            if (recv(clientFD, buf, MAX_BUFF, 0) < 0)
                                            {
                                                cout << "Could not connect to server" << endl;
                                                ut.log(FATAL, "recv() error", C_LOGFILE);
                                                exit(EXIT_FAILURE);
                                            }

                                            cout << "IOSB sending buff: " << buf << endl;

                                            if (strcmp(buf, "sending") == 0)
                                            {

                                                if (send(clientFD, "yes", strlen("yes"), 0) < 0)
                                                {
                                                    ut.log(FATAL, "send() error", C_LOGFILE);
                                                    // exit(EXIT_FAILURE);
                                                }

                                                // downloading file
                                                int isSuccess = newClient.writeToFile(clientFD, (char *)IOSB_DOWNLOAD);
                                                // cout<<"END OF WRITETOFILE"<<endl;
                                                if (isSuccess == 1)
                                                {
                                                    // send to server
                                                    if (send(clientFD, "SUCCESS", strlen("SUCCESS"), 0) < 0)
                                                    {
                                                        ut.log(FATAL, "Could not connect to server", C_LOGFILE);
                                                        clientErrExit();
                                                    }
                                                    else
                                                    {
                                                        // log to file
                                                        cout << "File downloaded Successfully in " << IOSB_DOWNLOAD << endl;
                                                        ut.log(INFO, "File Downloaded Successfully", C_LOGFILE);
                                                    }
                                                }
                                                else
                                                {
                                                    // send to server
                                                    if (send(clientFD, "FAILED", strlen("FAILED"), 0) < 0)
                                                    {
                                                        ut.log(FATAL, "Could not connect to server", C_LOGFILE);
                                                        clientErrExit();
                                                    }
                                                    else
                                                    { // log to file
                                                        cout << "Required file could not be generated, please try again." << endl;
                                                        ut.log(INFO, "File Downloading failed", C_LOGFILE);
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                // if error receive
                                                cout << "CDR Processing Failed.  Try again!" << endl;
                                            }
                                            pressEnter();
                                            break;

                                        case 3:
                                            if (send(clientFD, "3", 2, 0) < 0)
                                            {
                                                // perror("Fatal: ");
                                                cout << "Could not connect to server. Try again!" << endl;
                                                ut.log(FATAL, "Could not connect to server", C_LOGFILE);
                                                exit(EXIT_FAILURE);
                                            }
                                            cout << "Going back..." << endl;
                                            sleep(2);
                                            break;

                                        default:
                                            cout << "Invalid Input" << endl;
                                            break;
                                        }

                                        if (choice == 3)
                                        {
                                            choice = 0;
                                            break;
                                        }
                                    }
                                    break;

                                case 3:
                                    if (send(clientFD, "3", 2, 0) < 0)
                                    {
                                        cout << "Could not connect to server" << endl;
                                        ut.log(FATAL, "Could not connect to server", C_LOGFILE);
                                        exit(EXIT_FAILURE);
                                    }
                                    cout << "Going back..." << endl;
                                    sleep(2);
                                    break;

                                default:
                                    cout << "Invalid Input" << endl;
                                    sleep(2);
                                    break;
                                }

                                if (choice == 3)
                                {
                                    choice = 0;
                                    break;
                                }
                            }
                            break;

                        case 3:
                            if (send(clientFD, "3", 2, 0) < 0)
                            {
                                cout << "Could not connect to server" << endl;
                                ut.log(FATAL, "Could not connect to server", C_LOGFILE);
                                exit(EXIT_FAILURE);
                            }
                            cout << "Logged out successfully!" << endl;
                            pressEnter();
                            break;

                        default:
                            cout << "Invalid Input" << endl;
                            sleep(2);
                            break;
                        }

                        if (choice == 3)
                        {
                            choice = 0;
                            break;
                        }
                    }
                }
                else if (strcmp(buf, "failure") == 0)
                {
                    // wrong credentials
                    cout << "\nLogin Unsuccessful" << endl
                         << "Wrong username and password. Try again." << endl;
                }
                else
                {
                    // for handling db error
                    cout << "\nCannot verify user at the moment"
                         << endl
                         << "Please try again later" << endl;
                }
            }
            else
            {
                cout << "Cannot login right now, try again later" << endl;
            }
            sleep(2);
            break;
        case 3: // ASCII value of 3
            send(clientFD, "3", 2, 0);
            cout << "You've exited sucessfully!" << endl;
            sleep(2);
            exit(EXIT_SUCCESS);
        }
    }
}
