
#include "../include/TaskServer.h"
#include <condition_variable>
using std::lock_guard;
using std::unique_lock;
#include <mutex>

TaskServer::TaskServer(ConnectionHandler& _conn, std::mutex& mutex1, std::condition_variable& cvWrite  
, bool& _shouldTerminate , bool& _write )
: conn(_conn) , mutexTask(mutex1) ,conVarWriting(cvWrite) ,shouldTerminate(_shouldTerminate) , write(_write) 
{  
}



void TaskServer::run(){
    while(!shouldTerminate){

        std::string answer;
        // Get back an answer: by using the expected number of bytes (len bytes + newline delimiter)
        // We could also use: connectionHandler.getline(answer) and then get the answer without the newline char at the end
        
        
        if (!conn.getLine(answer)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }
        while (write){}
        unique_lock<std::mutex> uLock(mutexTask);
        if (conn.stompP->processIn(answer)){
            std::cout << "Disconnected\n" << std::endl;
            shouldTerminate = true;
        }
        
        conVarWriting.notify_all();
        


    }
}

