using namespace std;
#include <stdlib.h>
#include "../include/ConnectionHandler.h"
#include "../include/TaskKeyBoard.h"
#include "../include/TaskServer.h"
#include "../include/Game_data.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <bits/stdc++.h>
#include <condition_variable>
//#include <vector>


int main(int argc, char *argv[]){
    // TODO: implement the STOMP client
    // if (argc < 3) {
    //     std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
    //     return -1;
    // }
    while (1) {

        const short bufsize = 1024;
        std::cout << "please enter your login command\n" << std::endl;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
		std::string line(buf);
        if(line.length()==0){
            continue;
        }
        std::string frame;
        std::vector <std::string> seperated;
        std::vector <std::string> framesToReturn;
        stringstream ss(line);
        std::string word;
        int i=0;
        while (ss>>word){
            seperated.push_back(word);
            i=i+1;
        }
        if (seperated.size()!=4){
            std::cout << "please enter a leagal login command first\n" << std::endl;
            continue;;
        }
        if (seperated[0].compare("login")!=0){
            continue;
        }
        string hostAndPort = seperated.at(1);
        int pos = hostAndPort.find_first_of(':');
        std::string portString = hostAndPort.substr(pos+1);
        std::string host = hostAndPort.substr(0, pos);
        int port ;
        try {
            port = std::stoi(portString);
        }
        catch (exception p){
            std::cerr << "port is not a number" << std::endl;
            continue;;
        }
        ConnectionHandler connectionHandler(host, port);
        if (!connectionHandler.connect()) {
            std::cerr << "Could not connect to server"<< std::endl;
            continue;
        }
        //std::cerr << "you are here 63"<< std::endl;
        if (!connectionHandler.sendLine(line)) {
            std::cout << "couldnt connect please try again\n" << std::endl;
            continue;
        }
        //while (connectionHandler.getLine)
    // connect
    std::mutex mutex1;
    //std::unique_lock<mutex> lock;
    std::condition_variable conVar;
    std::condition_variable conVar2;
    bool st = false;
    bool write = false;
    TaskServer serverTask(connectionHandler, mutex1, conVar, st, write);
    TaskKeyBoard keyBoard(connectionHandler, mutex1 ,conVar,st, write);


    std::thread serverThread(&TaskServer::run, &serverTask);
    std::thread threadKeyBoard(&TaskKeyBoard::run, &keyBoard);
    
    
    serverThread.join();
    if (serverTask.shouldTerminate==true){
        keyBoard.shouldTerminate = true;
        
       // threadKeyBoard.detach();
    }
    threadKeyBoard.join();
    
    
    //serverTheread.detach(); // Once socketThread has being terminated,threadIO will be canceled
    }
}
