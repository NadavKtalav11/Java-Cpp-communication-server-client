#pragma once
//using namespace std;
#include <mutex>
#include "../include/TaskKeyBoard.h"
using std::lock_guard;
using std::unique_lock;
#include <condition_variable>


TaskKeyBoard::TaskKeyBoard(ConnectionHandler& _conn, std::mutex& lock ,   std::condition_variable& _cvread 
, bool& sT ,bool& _write )
: conn(_conn) , mutexTask(lock) , cvread(_cvread) , shouldTerminate(sT) , write(_write)
{  
}
void TaskKeyBoard::run()
{
    while (!shouldTerminate) {
       

        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
		std::string line(buf);
        int pos = line.find_first_of(' ');
        std::string commandLine = line.substr(0,pos);
        if ((commandLine.compare("login")==0)){
            std::cout << "The client is already logged in, log out before trying again" << std::endl;
            continue;
        }
        write=true;
        if (!conn.sendLine(line)) {
            continue;
        } 
        unique_lock<std::mutex> taskLock(mutexTask); 
        cvread.wait(taskLock); 
        write = false;   

  

    }
    if (shouldTerminate){
        std::cout << "disconnected" << std::endl;
    }
}
TaskKeyBoard::~TaskKeyBoard(){};
