#include <client.h>
#include <user.h>
#include <interoperator.h>
#include <utils.h>

Utils ut;

int main(int argc, char *argv[])
{
    Operator op;
    Client newClient;
    newClient.createSocket();
    newClient.clientConnect();

    int choice;
    int clientFD = newClient.getClientSockfd();

    User newUser;
    char buf[MAX_BUFF];

    while (1)
    {
        showMenu(0);
        cin >> choice;
        switch (choice)
        {
        // Registration
        case 1:
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
        case 2:
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
                                ut.log("Fatal log: send() error", "logs/ClientData.log");
                                exit(EXIT_FAILURE);
                            }
                            op.processCDR();
                            memset(&buf, 0, MAX_BUFF);
                            recv(clientFD, buf, sizeof(buf), 0);
                            if (strcmp(buf, "processed") == 0)
                            {
                                ut.log("Info log: Processed CDR file", "logs/ClientData.log");
                                cout << "CDR file processed successfully." << endl;
                            }
                            else
                            {
                                ut.log("Error log: Cannot process CDR file", "logs/ClientData.log");
                                cout << "CDR file cannot be processed." << endl;
                            }
                            break;
                        case 2:
                            if (send(clientFD, "2", 2, 0) < 0)
                            {
                                cout << "Could not connect to server" << endl;
                                ut.log("Fatal log: send() error", "logs/ClientData.log");
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
                                        ut.log("Fatal log: send() error", "logs/ClientData.log");
                                        exit(EXIT_FAILURE);
                                    }
                                    while (1)
                                    {
                                        // shows customer billing menu
                                        showMenu(3);

                                        cin >> choice;
                                        switch (choice)
                                        {
                                        case 1:
                                            if (send(clientFD, "1", 2, 0) < 0)
                                            {
                                                cout << "Could not connect to server" << endl;
                                                ut.log("Fatal log: send() error", "logs/ClientData.log");
                                                exit(EXIT_FAILURE);
                                            }
                                            // for searching by MSISDN
                                            break;

                                        case 2:
                                            if (send(clientFD, "2", 2, 0) < 0)
                                            {
                                                cout << "Could not connect to server" << endl;
                                                ut.log("Fatal log: send() error", "logs/ClientData.log");
                                                exit(EXIT_FAILURE);
                                            }
                                            // for generating CB.txt
                                            break;

                                        case 3:
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
                                    // interoperator billing menu
                                    if (send(clientFD, "2", 2, 0) < 0)
                                    {
                                        cout << "Could not connect to server" << endl;
                                        ut.log("Fatal log: send() error", "logs/ClientData.log");
                                        exit(EXIT_FAILURE);
                                    }
                                    while (1)
                                    {
                                        // shows interop billing menu
                                        showMenu(-1);
                                        cin >> choice;
                                        char brandName[MAX_BUFF] = {'\0'};
                                        switch (choice)
                                        {
                                        case 1:
                                            if (send(clientFD, "1", 2, 0) < 0)
                                            {
                                                cout << "Could not connect to server" << endl;
                                                ut.log("Fatal log: send() error", "logs/ClientData.log");
                                                exit(EXIT_FAILURE);
                                            }

                                            memset(&buf, 0, MAX_BUFF);

                                            if (recv(clientFD, buf, MAX_BUFF, 0) < 0)
                                            {
                                                cout << "Could not connect to server" << endl;
                                                ut.log("Fatal log: recv() error", "logs/ClientData.log");
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
                                                    ut.log("Fatal log: send() error", "logs/ClientData.log");
                                                    exit(EXIT_FAILURE);
                                                }

                                                // receiving brand search results
                                                if (recv(clientFD, buf, MAX_BUFF, 0) < 0)
                                                {
                                                    cout << "Could not connect to server" << endl;
                                                    ut.log("Fatal log: recv() error", "logs/ClientData.log");
                                                    exit(EXIT_FAILURE);
                                                }

                                                cout << buf << endl;
                                            }
                                            break;

                                        case 2:
                                            if (send(clientFD, "2", 2, 0) < 0)
                                            {
                                                cout << "Could not connect to server" << endl;
                                                ut.log("Fatal log: send() error", "logs/ClientData.log");
                                                exit(EXIT_FAILURE);
                                            }

                                            memset(&buf, 0, MAX_BUFF);

                                            if (recv(clientFD, buf, sizeof(buf), 0) < 0)
                                            {
                                                cout << "Could not connect to server" << endl;
                                                ut.log("Fatal log: recv() error", "logs/ClientData.log");
                                                exit(EXIT_FAILURE);
                                            }

                                            if (strcmp(buf, "generated") == 0)
                                            {
                                                cout << "Generated IOSB.txt file." << endl;
                                            }
                                            else
                                            {
                                                cout << "Required files could not be generated, please try again." << endl;
                                            }
                                            break;

                                        case 3:
                                            if (send(clientFD, "3", 2, 0) < 0)
                                            {
                                                cout << "Could not connect to server" << endl;
                                                ut.log("Fatal log: send() error", "logs/ClientData.log");
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
                                        ut.log("Fatal log: send() error", "logs/ClientData.log");
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
                                ut.log("Fatal log: send() error", "logs/ClientData.log");
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
        case 3:
            send(clientFD, "3", 2, 0);
            cout << "You've exited sucessfully!" << endl;
            exit(EXIT_SUCCESS);

        default:
            cout << "Invalid Input" << endl;
            break;
        }
    }
}
