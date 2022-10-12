#include <user.h>

/*This function is used to get the user ID and Password
 from the client for registration and login*/
void User::setDetails()
{
    cout << "Enter User name: ";
    cin >> username;

    char *storedPass = getpass("Enter Password: ");
    setPassword(storedPass);
}

bool User::validateUsername(string user)
{
    int special = 0, l = user.length();
    // check length is 0 or more than 30
    if (l < 6 || l > 20)
        return 0;

    for (int i = 0; i < l; i++)
    {
        char s = user.at(i);

        // no spaces allowed
        if (s == ' ')
            return 0;

        // characters other than alphabets and numbers
        if (isalnum(s))
            continue;
        else
        {
            // periods and underscore allowed but only one
            if (s == '_' || s == '.')
            {
                special++;
                if (special > 1)
                    return 0;
            }
            else
                return 0;
        }
    }
    return 1;
}

bool User::validatePassword(char *pass)
{
    bool isValid = 0;

    // check storedPass
    if (strlen(pass) >= 6 && strlen(pass) <= 20)
        // set isValid
        isValid = 1;

    return isValid;
}

void User::registerDetails()
{
    string user;
    cout << "Enter User name: ";
    cin.ignore();
    getline(cin, user);

    while (!validateUsername(user))
    {
        cout << "INVALID USERNAME!" << endl
             << "Username must contains only {A-Z, a-z, 0-9, '.', '_'}and length must range from [6-20]." << endl;
        cout << "Enter User name: ";

        getline(cin, user);
    }

    strcpy(username, user.c_str());

    char *storedPass = getpass("Enter Password: ");

    while (!validatePassword(storedPass))
    {
        cout << "INVALID PASSWORD!" << endl
             << "Password length must range from [6-20]." << endl;
        storedPass = getpass("Enter Password: ");
    }

    setPassword(storedPass);
}

// This function is used to store the registered users data into a file
void User::toDatabase(User &newUser)
{
    fstream userDB;

    // file.write((const char*)this, sizeof(this));

    userDB.open("data/registered.dat", ios::out | ios::app);
    if (userDB)
    {
        userDB.write(reinterpret_cast<char *>(&newUser), sizeof(newUser));
    }

    else
    {
        perror("file()");
        // log("File cannot be opened");
    }

    userDB.close();
}

string User::toString()
{
    ostringstream ss;
    ss.clear();
    ss << this->getUsername() << "|" << this->getPassword();
    return ss.str();
}