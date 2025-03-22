
#ifndef __COMWISE__ROBOT_RPC__SHARED_MEMORY_TRANSPORT__H__
#define __COMWISE__ROBOT_RPC__SHARED_MEMORY_TRANSPORT__H__

#include "transport.h"
#include <exception>

#ifdef _WIN32

// Windows SharedMemoryTransport.h
#include <windows.h>
#include <cstring>
#include <stdexcept>

class SharedMemoryTransport : public Transport {
public:
    void publish(const Topic& topic_name, const void* data, size_t size) override {
        HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, topic_name.get_name().c_str());
        if (hMapFile == NULL) {
            //throw std::runtime_error("Could not create file mapping object");
        }

        void* pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, size);
        if (pBuf == NULL) {
            CloseHandle(hMapFile);
            //throw std::runtime_error("Could not map view of file");
        }

        std::memcpy(pBuf, data, size);

        UnmapViewOfFile(pBuf);
        CloseHandle(hMapFile);
    }

    void subscribe(const Topic& topic_name, void* data, size_t size) override {
        HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, topic_name.get_name().c_str());
        if (hMapFile == NULL) {
            throw std::runtime_error("Could not open file mapping object");
        }

        void* pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, size);
        if (pBuf == NULL) {
            CloseHandle(hMapFile);
            throw std::runtime_error("Could not map view of file");
        }

        std::memcpy(data, pBuf, size);

        UnmapViewOfFile(pBuf);
        CloseHandle(hMapFile);
    }
};

#else

// Linux SharedMemoryTransport.h
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <cstring>
#include <iostream>

class SharedMemoryTransport : public Transport {
public:
    void publish(const Topic& topic_name, const void* data, size_t size) override {
        int shmid = shmget(ftok(topic_name.get_name().c_str(), 1), size, IPC_CREAT | 0666);
        if (shmid == -1) {
            throw std::runtime_error("Failed to create shared memory");
        }

        void* shared_memory = shmat(shmid, nullptr, 0);
        if (shared_memory == (void*)-1) {
            throw std::runtime_error("Failed to attach shared memory");
        }

        std::memcpy(shared_memory, data, size);
        shmdt(shared_memory);
    }

    void subscribe(const Topic& topic_name, void* data, size_t size) override {
        int shmid = shmget(ftok(topic_name.get_name().c_str(), 1), size, 0666);
        if (shmid == -1) {
            throw std::runtime_error("Failed to get shared memory");
        }

        void* shared_memory = shmat(shmid, nullptr, 0);
        if (shared_memory == (void*)-1) {
            throw std::runtime_error("Failed to attach shared memory");
        }

        std::memcpy(data, shared_memory, size);
        shmdt(shared_memory);
    }
};

#endif // __WIN32

#endif // __COMWISE__ROBOT_RPC__SHARED_MEMORY_TRANSPORT__H__