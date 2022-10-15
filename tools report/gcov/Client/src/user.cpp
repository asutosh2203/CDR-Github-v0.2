#include <user.h>

/*
 *  FUNCTION NAME	: validateUsername
 *
 *  DESCRIPTION		: It validates the username provided during register or login
 *
 *  PARAMETERS		: string user
 *
 *  RETURN 		: bool
 *
 */
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
} // end of validateUsername

/*
 *  FUNCTION NAME	: validatePassword
 *
 *  DESCRIPTION		: It validates the password provided during register or login
 *
 *  PARAMETERS		: char *pass
 *
 *  RETURN 		: bool
 *
 */
bool User::validatePassword(char *pass)
{
    bool isValid = false;

    // check storedPass
    if (strlen(pass) >= 6 && strlen(pass) <= 20)
        // set isValid
        isValid = true;

    return isValid;
}

/*
 *  FUNCTION NAME	: setDetails
 *
 *  DESCRIPTION		: It sets the username and password of the current user.
 *
 *  PARAMETERS		: None
 *
 *  RETURN 		    : void
 *
 */
void User::setDetails()
{
    string user;
    cout << "Enter User name: ";
    cin.ignore();
    getline(cin, user);

    while (!validateUsername(user))
    {
        cout << "INVALID USERNAME!" << endl
             << "Username must contains only {A-Z, a-z, 0-9, '.', '_'} and length must range from [6-20]." << endl;
        cout << "Enter User name: ";

        getline(cin, user);
    }

    setUsername(user.c_str());

    char *storedPass = getpass("Enter Password: ");

    while (!validatePassword(storedPass))
    {
        cout << "INVALID PASSWORD!" << endl
             << "Password length must range from [6-20]." << endl;
        storedPass = getpass("Enter Password: ");
    }

    setPassword((const char *)storedPass);
}

/*
 *  FUNCTION NAME	: toDatabase
 *
 *  DESCRIPTION		: It take an object of User type as parameter and stores it in users database.
 *
 *  PARAMETERS		: User &newUser
 *
 *  RETURN 		: void
 *
 */
void User::toDatabase(User &newUser)
{
    fstream userDB;

    // file.write((const char*)this, sizeof(this));

    userDB.open(USER_DB, ios::out | ios::app);
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
