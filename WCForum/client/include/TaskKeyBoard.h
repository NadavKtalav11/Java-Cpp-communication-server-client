


#include <mutex>
#include "../include/ConnectionHandler.h"
#include <condition_variable>

class TaskKeyBoard
{
    private:
        ConnectionHandler& conn;
        std::condition_variable& cvread;
        std::mutex& mutexTask;

    public:
        TaskKeyBoard(ConnectionHandler& ,std::mutex& ,  std::condition_variable&,bool& , bool&);
        bool shouldTerminate;
        bool write;
        ~TaskKeyBoard();
        void run();
};