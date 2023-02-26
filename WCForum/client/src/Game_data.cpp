#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include "../include/Game_data.h"



Game_data::Game_data(): subscriptionIdCounter(1), receiptIdCounter(1),topics_subscriptionId(),reciept_game(), reciept_command(),activeUser(), goalAndPossession()
{
}
Game_data::~Game_data(){}

std::string Game_data::getNewSubscriptionId() {
    int i =subscriptionIdCounter;
    std::string id;
    id = std::to_string(i);
    subscriptionIdCounter++;
    return id;
}


std::string Game_data::getNewreceiptId(){
    std::string id = std::to_string(receiptIdCounter);
    receiptIdCounter++;
    return id;
    
}

/*std::string Game_data::getSubscriptionIdFromMap(std::string topic){
    for (auto it = topics_subscriptionId.begin();it != subscriptionId_topics.end() ; ++it){
        if (it->second==topic){
            return it->second;
        }
    }
    return "";  
}*/

std::string Game_data::getActiveUser(){
   
    return activeUser;
    
}

void Game_data::setActiveUser(std::string& username){
   
    activeUser = username+ "";
    
}
