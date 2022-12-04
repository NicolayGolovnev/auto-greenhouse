//
// Created by kolya on 04.12.2022.
//

#ifndef AUTO_GREENHOUSE_WRITER_H
#define AUTO_GREENHOUSE_WRITER_H


#include "Semaphore.h"
#include "FileMemMap.h"
#include "RWLockUtils.h"

class Writer {
private:
    Semaphore *writer, *e;
    FileMemMap *fileMem;
    const char* nameSystem; // только 2 системы в нашем случае - AirSensor, WaterSensor

    /** Генерация значения температуры от 11 до 50 */
    long generateTemperature() {
        return (rand() + 11) % 50;
    }

    /** Генерация значения влажности от 55% до 100% */
    long generateHumidity() {
        return (rand() + 55) % 100;
    }

public:
    Writer(const char *name) {
        this->nameSystem = name;

        std::string bufForRight = std::string(name) + "_Right";
        const char *newNameForRight = bufForRight.c_str();
        e = new Semaphore(newNameForRight, true);

        std::string bufForWriter = std::string(name) + "_Writer";
        const char *newNameForWriter = bufForRight.c_str();
        writer = new Semaphore(newNameForWriter, false);

        fileMem = new FileMemMap(name);
    }
    ~Writer() = default;

    int write() {
        e->P(); // ждем права на ввод
        if (:: numR > 0 || :: numW > 0) {
            // уже есть писатель - встаем в очередь
            e->V();
            writer->P(); // ожидаем право на запись
        }

        :: numW++;
        :: priorityRaceForReaders(this->writer, nullptr, this->e);
        int generatedValue = 0; // для того, чтобы передать писателю данные для вывода
        if (std::string(this->nameSystem) == "AirSensor") {
            generatedValue = this->generateTemperature();
            this->fileMem->set(generatedValue);
        }
        else if (std::string(this->nameSystem) == "WaterSensor") {
            generatedValue = this->generateHumidity();
            this->fileMem->set(generatedValue);
        }
        e->P(); // ожидаем доступ для окончания нашей работы
        :: numW--;
        :: priorityRaceForReaders(this->writer, nullptr, this->e);

        return generatedValue;
    }
};


#endif //AUTO_GREENHOUSE_WRITER_H
