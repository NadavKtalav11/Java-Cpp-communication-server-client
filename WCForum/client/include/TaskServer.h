#include "../include/ConnectionHandler.h"
#include <mutex>
#include <condition_variable>
class TaskServer
{
    private:
    ConnectionHandler& conn;
    std::condition_variable& conVarWriting;
    std::mutex& mutexTask ;

    public:
        TaskServer(ConnectionHandler& ,std::mutex&, std::condition_variable& ,bool& , bool&);
        bool shouldTerminate;
        bool write;
        void run();
};