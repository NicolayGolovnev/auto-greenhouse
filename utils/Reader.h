//
// Created by kolya on 04.12.2022.
//

#ifndef AUTO_GREENHOUSE_READER_H
#define AUTO_GREENHOUSE_READER_H


#include "FileMemMap.h"
#include "Semaphore.h"
#include "RWLockUtils.h"

class Reader {
private:
    Semaphore *e, *reader;
    FileMemMap *fileMem;

public:
    Reader(const char *name) {
        std::string bufForRight = std::string(name) + "_Right";
        const char *newNameForRight = bufForRight.c_str();
        e = new Semaphore(newNameForRight, true);

        std::string bufForReader = std::string(name) + "_Reader";
        const char *newNameForReader = bufForRight.c_str();
        reader = new Semaphore(newNameForReader, false);

        fileMem = new FileMemMap(name);
    }
    ~Reader() = default;

    int read() {
        e->P(); // ждем права на ввод
        if (:: numW > 0) {
            // если есть еще писатели - встаем в очередь на чтение
            :: waitR++;
            e->V();
            reader->P(); // ожидаем право на чтение
        }

        :: numR++;
        :: priorityRaceForReaders(nullptr, this->reader, this->e);
        int data = fileMem->get();
        e->P(); // ожидаем доступ для окончания нашей работы
        :: numR--;
        :: priorityRaceForReaders(nullptr, this->reader, this->e);

        return data;
    }
};


#endif //AUTO_GREENHOUSE_READER_H
