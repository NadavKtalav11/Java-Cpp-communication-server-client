#pragma once

#include <string>
#include <iostream>
#include <map>
#include <vector>

class User
{
private:
std::string userName;
std::string password;

public:
User(std::string userName, std::string password);
virtual ~User();
const std::string &getUserName() const;
const std::string &getUserPassword() const;

};