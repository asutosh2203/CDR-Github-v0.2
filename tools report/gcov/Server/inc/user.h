#pragma once
#include <iostream>
#include <cstring>
#include <sstream>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <ctype.h>

#define USER_DB "data/registered.dat"

using namespace std;

class User
{
public:
    void setDetails();
    // void registerDetails();
    bool validateUsername(string);
    bool validatePassword(char *);
    void toDatabase(User &);
    void setUsername(const char *s) { strcpy(username, s); }
    void setPassword(const char *s) { strcpy(password, s); }
    char *getUsername() { return username; }
    char *getPassword() { return password; }
    void getDetails() { cout << "Username:" << username << '\n'
                             << "Password: " << password << endl; }
    void log(string);

private:
    char username[20] = {'\0'};
    char password[20] = {'\0'};
};
