//
// Created by kolya on 06.11.2022.
//

#ifndef AUTO_GREENHOUSE_CHANNEL_H
#define AUTO_GREENHOUSE_CHANNEL_H

#include "Semaphore.h"

class Channel {
private:
    Semaphore *free, *empty;
    HANDLE fileMem;
    void *buffer;
public:
    Channel(const char *name) {
        std::string buf = "CH_" + std::string(name);
        const char *newName = buf.c_str();

        this->free = new Semaphore(newName, true);
        this->empty = new Semaphore(newName, false);

        this->fileMem = OpenFileMappingA(
                FILE_MAP_ALL_ACCESS,
                false,
                (LPCSTR) newName
                );
        if (this->fileMem == nullptr)
            this->fileMem = CreateFileMappingA(
                    INVALID_HANDLE_VALUE,
                    nullptr,
                    PAGE_READWRITE,
                    0, 4096,
                    (LPCSTR) newName
                    );

        if (this->fileMem != nullptr)
            this->buffer = MapViewOfFile(
                    this->fileMem,
                    FILE_MAP_ALL_ACCESS,
                    0,
                    0,
                    4096
                    );
        else printf("error: FILE_MAP!\n");
    }
    ~Channel() = default;


    void put(int data) {
        free->P();
        *( (int*) buffer) = data;
        empty->V();
    }

    int get() {
        int data;
        empty->P();
        data = *( (int*) buffer);
        free->V();
        return data;
    }
};

#endif //AUTO_GREENHOUSE_CHANNEL_H
