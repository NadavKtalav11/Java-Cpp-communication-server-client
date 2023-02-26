#pragma once

#include "../include/ConnectionHandler.h"
#include "../include/Game_data.h"
#include <iostream>
#include <string>
#include <vector>




// TODO: implement the STOMP protocol
class StompProtocol
{
private:
public:
virtual ~StompProtocol();
StompProtocol();
std::vector<std::string> processOut(std::string massage);
bool processIn(std::string &massage);
Game_data game_Data;
bool writeToFile(std::string , std::string);
bool compareEvents(const Event& ,const Event&);
};
