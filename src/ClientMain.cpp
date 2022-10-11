#include <client.h>
#include <user.h>
#include <interoperator.h>
#include <utils.h>

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

    Operator op;
    // Client newClient;
    newClient.createSocket();
    newClient.clientConnect();

    int choice;
    int clientFD = newClient.getClientSockfd();

    User newUser;
    char buf[MAX_BUFF];
    // char c;
    while (1)
    {
        showMenu(0);
        // cin.ignore();

        // char c;
        // while ((c = getchar()) != 10 && c != EOF)
        // {

        char c = getchar();

        // cout << c << endl;

        if (isChoiceValid(c) == true)
        {

            switch (c)
            {
            // Registration
            case 49: // ASCII value of 1
                memset(&buf, 0, MAX_BUFF);
                send(clientFD, "1", 2, 0);
                recv(clientFD, buf, sizeof(buf), 0);
                if (strcmp(buf, "register") == 0)
                {
                    newUser.registerDetails();
                    string str = newUser.toString();
                    send(clientFD, str.c_str(), str.length(), 0);
                }
                memset(&buf, 0, MAX_BUFF);
                recv(clientFD, buf, sizeof(buf), 0);
                if (strcmp(buf, "success") == 0)
                {
                    cout << endl;
                    cout << "Registration successful" << endl;
                }
                else if (strcmp(buf, "exists") == 0)
                {
                    cout << endl;
                    cout << "Username already exists.\nCreate a new username or login with your existing account." << endl;
                }
                else
                {
                    cout << endl;
                    cout << "Registration unsuccessful" << endl;
                }
                break;

            // login
            case 50: // ASCII value of 2
                send(clientFD, "2", 2, 0);
                memset(&buf, 0, MAX_BUFF);
                recv(clientFD, buf, sizeof(buf), 0);
                if (strcmp(buf, "login") == 0)
                {
                    newUser.setdetails();
                    string str1 = newUser.toString();
                    send(clientFD, str1.c_str(), str1.length(), 0);
                    memset(&buf, 0, MAX_BUFF);
                    recv(clientFD, buf, sizeof(buf), 0);
                    if (strcmp(buf, "loggedIn") == 0)
                    {
                        cout << "Login Successful" << endl;

                        while (1)
                        {
                            showMenu(1);
                            cin >> choice;
                            switch (choice)
                            {
                            case 1:
                                if (send(clientFD, "1", 2, 0) < 0)
                                {
                                    cout << "Could not connect to server" << endl;
                                    ut.log(FATAL, "send() error", C_LOGFILE);
                                    exit(EXIT_FAILURE);
                                }
                                op.processCDR();
                                memset(&buf, 0, MAX_BUFF);
                                recv(clientFD, buf, sizeof(buf), 0);
                                if (strcmp(buf, "processed") == 0)
                                {
                                    cout << "CDR file processed successfully." << endl;
                                }
                                else
                                {
                                    ut.log(FATAL, "Cannot process CDR file", C_LOGFILE);
                                    cout << "CDR file cannot be processed." << endl;
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
                                    showMenu(2);
                                    /*
                                    cout << "====== BILLING INFO MENU ======" << endl;
                                    cout << "\t1. Customer Billing" << endl;
                                    cout << "\t2. Interoperator Billing" << endl;
                                    cout << "\t3. Exit" << endl;
                                    */
                                    cin >> choice;
                                    switch (choice)
                                    {
                                    case 1:
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

                                            cin >> choice;

                                            int MSISDN = 0;

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

                                                    cout << "Enter MSISDN to be searched: "
                                                         << endl;
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

                                                    cout << "CDR Processing Failed.  Try again!" << endl;
                                                }

                                                break;

                                            case 3:
                                                if (send(clientFD, "3", 2, 0) < 0)
                                                {
                                                    cout << "Could not connect to server" << endl;
                                                    ut.log(FATAL, "send() error", C_LOGFILE);
                                                    exit(EXIT_FAILURE);
                                                }
                                                cout << "Going back..." << endl;
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
                                            cin >> choice;
                                            char brandName[MAX_BUFF] = {'\0'};
                                            cout << "choice 322 line: " << choice << endl;
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
                                                    cout << "Enter brand name to be searched: "
                                                         << endl;
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
                                                cout << "Press ENTER to continue";
                                                cin.ignore();
                                                c = getchar();
                                                break;

                                            case 2:
                                                cout << "Case 2 before send" << endl;
                                                if (send(clientFD, "2", 2, 0) < 0)
                                                {
                                                    cout << "Could not connect to server" << endl;
                                                    ut.log(FATAL, "send() error", C_LOGFILE);
                                                    exit(EXIT_FAILURE);
                                                }
                                                cout << "Case 2 after send" << endl;
                                                memset(&buf, 0, MAX_BUFF);
                                                if (recv(clientFD, buf, MAX_BUFF, 0) < 0)
                                                {
                                                    cout << "Could not connect to server" << endl;
                                                    ut.log(FATAL, "recv() error", C_LOGFILE);
                                                    exit(EXIT_FAILURE);
                                                }

                                                if (strcmp(buf, "sending") == 0)
                                                {

                                                    if (send(clientFD, "yes", strlen("yes"), 0))
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

                                    case 3:
                                        if (send(clientFD, "3", 2, 0) < 0)
                                        {
                                            cout << "Could not connect to server" << endl;
                                            ut.log(FATAL, "Could not connect to server", C_LOGFILE);
                                            exit(EXIT_FAILURE);
                                        }
                                        cout << "Going back..." << endl;
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
                                cout << "Logged out successfully!" << endl;
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
                    }
                    if (strcmp(buf, "failure") == 0)
                    {
                        cout << "\nLogin Unsuccessful" << endl;
                    }
                }
                break;
            case 51: // ASCII value of 3
                send(clientFD, "3", 2, 0);
                cout << "You've exited sucessfully!" << endl;
                exit(EXIT_SUCCESS);
            }
        }
        else
        {
            cout << "Invalid Choice!!!!!!!";
        }
        getchar();
        // }
    }
}
