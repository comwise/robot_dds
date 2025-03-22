// semaphore.h
#ifndef __COMWISE__ROBOT_RPC__SEMAPHORE__H__
#define __COMWISE__ROBOT_RPC__SEMAPHORE__H__

#include <string>
#include <iostream>
#include <cstring>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#endif

#define SEM_WRITE_NAME "/dds_write_sem"

#ifdef _WIN32
class Semaphore {
public:
    Semaphore() : sem_handle(nullptr) {}

    void create() {
        sem_handle = CreateSemaphore(NULL, 1, 1, SEM_WRITE_NAME);
        if (sem_handle == NULL) {
            throw std::runtime_error("Failed to create semaphore.");
        }
    }

    void wait() {
        WaitForSingleObject(sem_handle, INFINITE);
    }

    void post() {
        ReleaseSemaphore(sem_handle, 1, NULL);
    }

    void close() {
        CloseHandle(sem_handle);
    }

private:
    HANDLE sem_handle;
};

#else

class Semaphore {
public:
    Semaphore() : sem(nullptr) {}

    void create() {
        sem = sem_open(SEM_WRITE_NAME, O_CREAT, 0666, 1);
        if (sem == SEM_FAILED) {
            throw std::runtime_error("Failed to create semaphore.");
        }
    }

    void wait() {
        sem_wait(sem);
    }

    void post() {
        sem_post(sem);
    }

    void close() {
        sem_close(sem);
    }

private:
    sem_t* sem;
};

#endif // _WIN32

#endif // __COMWISE__ROBOT_RPC__SEMAPHORE__H__
