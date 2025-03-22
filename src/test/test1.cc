// test.cc
#include <iostream>
#include "shared_memory.h"
#include "semaphore.h"
#include "message.h"

int main() {
    SharedMemory shm;
    shm.create();
    
    Semaphore sem_write;
    sem_write.create();

    Message<std::string> msg("Hello, this is a DDS message!");
    SharedData* data = shm.getPtr();
    
    sem_write.wait();
    snprintf(data->message, MAX_MSG_SIZE, "%s", msg.serialize().c_str());
    data->new_message = true;
    
    sem_write.post();
    
    std::cout << "Publisher sent: " << data->message << std::endl;
    
    return 0;
}
