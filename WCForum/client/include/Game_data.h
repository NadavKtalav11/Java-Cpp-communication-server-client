#pragma once

#include <string>
#include <iostream>
#include <map>
#include <vector>

#include "../include/event.h"

class Game_data
{
private:
    int subscriptionIdCounter;
    int receiptIdCounter;
    std::string name; 
    std::string activeUser;
    std::string lockedsub;
   
    


public:
Game_data();
~Game_data();
std::string getNewSubscriptionId();
std::string getNewreceiptId();
std::string getSubscriptionIdFromMap(std::string topic);
std::string getActiveUser();
void setActiveUser(std::string&);

bool compareEvents(Event , Event);
std::map<std::pair<std::string, std::string>, std::vector<Event> > userAndTopicForEvent;
std::map<std::pair<std::string, std::string>, std::map<std::string , std::string>> goalAndPossession;
std::map<std::string, std::string > topics_subscriptionId;
std::map<std::string, std::string > reciept_command;
std::map<std::string, std::string > reciept_game;
};

