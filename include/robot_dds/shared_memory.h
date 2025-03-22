// shared_memory.h
#ifndef __COMWISE__ROBOT_RPC__SHARED_MEMORY__H__
#define __COMWISE__ROBOT_RPC__SHARED_MEMORY__H__

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

#define SHM_NAME "/dds_shm"
#define SEM_WRITE_NAME "/dds_write_sem"
#define SEM_READ_NAME "/dds_read_sem"
#define MAX_MSG_SIZE 1024

struct SharedData {
    char message[MAX_MSG_SIZE];
    bool new_message;
};

#ifdef _WIN32
class SharedMemory {
public:
    SharedMemory() : shm_ptr(nullptr), hMapFile(NULL) {}

    void create() {
        hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(SharedData), SHM_NAME);
        if (hMapFile == NULL) {
            throw std::runtime_error("Failed to create file mapping.");
        }

        shm_ptr = (SharedData*) MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SharedData));
        if (shm_ptr == NULL) {
            CloseHandle(hMapFile);
            throw std::runtime_error("Failed to map view of file.");
        }
    }

    void open() {
        hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, SHM_NAME);
        if (hMapFile == NULL) {
            throw std::runtime_error("Failed to open file mapping.");
        }

        shm_ptr = (SharedData*) MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SharedData));
        if (shm_ptr == NULL) {
            CloseHandle(hMapFile);
            throw std::runtime_error("Failed to map view of file.");
        }
    }

    void close() {
        UnmapViewOfFile(shm_ptr);
        CloseHandle(hMapFile);
    }

    SharedData* getPtr() {
        return shm_ptr;
    }

private:
    SharedData* shm_ptr;
    HANDLE hMapFile;
};

#else
class SharedMemory {
public:
    SharedMemory() : shm_fd(-1), shm_ptr(nullptr) {}

    void create() {
        shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
        if (shm_fd == -1) {
            throw std::runtime_error("Failed to create shared memory.");
        }
        ftruncate(shm_fd, sizeof(SharedData));
        shm_ptr = (SharedData*) mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if (shm_ptr == MAP_FAILED) {
            close(shm_fd);
            throw std::runtime_error("Failed to map shared memory.");
        }
    }

    void open() {
        shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
        if (shm_fd == -1) {
            throw std::runtime_error("Failed to open shared memory.");
        }
        shm_ptr = (SharedData*) mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if (shm_ptr == MAP_FAILED) {
            close(shm_fd);
            throw std::runtime_error("Failed to map shared memory.");
        }
    }

    void close() {
        munmap(shm_ptr, sizeof(SharedData));
        ::close(shm_fd);
    }

    SharedData* getPtr() {
        return shm_ptr;
    }

private:
    int shm_fd;
    SharedData* shm_ptr;
};
#endif

#endif // __COMWISE__ROBOT_RPC__SHARED_MEMORY__H__
