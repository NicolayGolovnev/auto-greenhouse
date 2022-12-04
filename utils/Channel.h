//
// Created by kolya on 06.11.2022.
//

#ifndef AUTO_GREENHOUSE_CHANNEL_H
#define AUTO_GREENHOUSE_CHANNEL_H

#include "Semaphore.h"
#include "FileMemMap.h"

class Channel {
private:
    Semaphore *free, *empty;
    FileMemMap *fileMem;
public:
    Channel(const char *name) {
        std::string bufForFree = "CH_" + std::string(name) + "_FREE";
        const char *newNameForFree = bufForFree.c_str();
        this->free = new Semaphore(newNameForFree, true);

        std::string bufForEmpty = "CH_" + std::string(name) + "_EMPTY";
        const char *newNameForEmpty = bufForEmpty.c_str();
        this->empty = new Semaphore(newNameForEmpty, false);

        fileMem = new FileMemMap(name);
    }
    ~Channel() = default;


    void put(int data) {
        free->P();
        this->fileMem->set(data);
        empty->V();
    }

    int get() {
        int data;
        empty->P();
        data = this->fileMem->get();
        free->V();
        return data;
    }
};

#endif //AUTO_GREENHOUSE_CHANNEL_H
