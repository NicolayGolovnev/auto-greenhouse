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
        std::string bufForFree = "CH_" + std::string(name) + "_FREE";
        const char *newNameForFree = bufForFree.c_str();
        this->free = new Semaphore(newNameForFree, true);

        std::string bufForEmpty = "CH_" + std::string(name) + "_EMPTY";
        const char *newNameForEmpty = bufForEmpty.c_str();
        this->empty = new Semaphore(newNameForEmpty, false);

        std::string bufForFileMem = "FILEMEM_" + std::string(name);
        const char *newNameForFileMem = bufForFileMem.c_str();
        this->fileMem = OpenFileMappingA(
                FILE_MAP_ALL_ACCESS,
                false,
                (LPCSTR) newNameForFileMem
                );
        if (this->fileMem == nullptr)
            this->fileMem = CreateFileMappingA(
                    INVALID_HANDLE_VALUE,
                    nullptr,
                    PAGE_READWRITE,
                    0, 4096,
                    (LPCSTR) newNameForFileMem
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
