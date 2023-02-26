#include "../include/User.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>


User::User(std::string userName, std::string password)
    : userName(userName), password(password)
{
}

User::~User()
{
}

const std::string &User::getUserName() const
{
    return this->userName;
}

const std::string &User::getUserPassword() const
{
    return this->password;
}