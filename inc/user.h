#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <ctype.h>
#include <bits/stdc++.h>
#include <ctype.h>

using namespace std;

class User
{
private:
    char username[20] = {'\0'};
    char password[20] = {'\0'};

public:
    void setdetails();
    void registerDetails();
    bool validateUsername(string);
    bool validatePassword(char *);
    void database(char *);
    void setUID(char *s) { strcpy(username, s); }
    void setPassword(char *s) { strcpy(password, s); }
    char *getUID() { return username; }
    char *getPassword() { return password; }
    void getdetails() { cout << username << '\n'
                             << password << endl; }
    string toString();
    void log(string);
};
