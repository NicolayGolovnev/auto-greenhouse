//
// Created by kolya on 05.11.2022.
//

#ifndef AUTO_GREENHOUSE_WATERSYSTEM_H
#define AUTO_GREENHOUSE_WATERSYSTEM_H

#include "Semaphore.h"
#include "Channel.h"
#include "Reader.h"

#define HIGH_HUMIDITY 90
#define LOW_HUMIDITY 65

class WaterSystem {
private:
    Semaphore *semaphore, *informSystem, *answerToInformSystem;
    Reader *reader;
    bool isWorking = false;

public:
    WaterSystem() {
        this->semaphore = new Semaphore("WaterSensor", false);
        this->informSystem = new Semaphore("WaterSystemInformSystem", false);
        this->answerToInformSystem = new Semaphore("WaterSystemAnswerInformSystem", false);

        this->reader = new Reader("WaterSensor");
    }
    ~WaterSystem() = default;

    void run() {
        while (true) {
            this->semaphore->V();
            int valueFromSensor = this->reader->read();

            // Обработка данных с датчика
            if (valueFromSensor < LOW_HUMIDITY) {
                this->isWorking = true;
                printf("[LOW]\t\tHumidity = %d, system starts!\n", valueFromSensor);
            }
            else if (valueFromSensor > HIGH_HUMIDITY) {
                this->isWorking = false;
                printf("[HIGH]\t\tHumidity = %d, system shutdown!\n", valueFromSensor);
            }
            else {
                if (this->isWorking)
                    printf("[NORMAL]\tHumidity = %d, system on!\n", valueFromSensor);
                else
                    printf("[NORMAL]\tHumidity = %d, system off!\n", valueFromSensor);
            }

            // Если есть запрос на включение системы из вне - включаем и сообщаем об этом
            if (this->informSystem->P(500) == WAIT_OBJECT_0) {
                printf("\t[BY ORDER OF INF_SYS]\tSystem starts!\n");
                this->answerToInformSystem->V();
            }
        }
    }
};

DWORD WINAPI WaterSystemThread(LPVOID pVoid) {
    Semaphore *semaphore = new Semaphore("WaterSensor", false);
    Channel *channel = new Channel("WaterSensor");

    Semaphore *informSystem = new Semaphore("WaterSystemInformSystem", false);
    Semaphore *answerToInformSystem = new Semaphore("WaterSystemAnswerInformSystem", false);
    bool isWorking = false;

    while (true) {
        semaphore->V();
        int dataFromSensor = channel->get();

        // Обработка данных с датчика
        if (dataFromSensor < LOW_HUMIDITY) {
            isWorking = true;
            printf("[LOW]\t\tHumidity = %d, system starts!\n", dataFromSensor);
        }
        else if (dataFromSensor > HIGH_HUMIDITY) {
            isWorking = false;
            printf("[HIGH]\t\tHumidity = %d, system shutdown!\n", dataFromSensor);
        }
        else {
            if (isWorking)
                printf("[NORMAL]\tHumidity = %d, system on!\n", dataFromSensor);
            else
                printf("[NORMAL]\tHumidity = %d, system off!\n", dataFromSensor);
        }

        // Если есть запрос на включение системы из вне - включаем и сообщаем об этом
        if (informSystem->P(500) == WAIT_OBJECT_0) {
            printf("\t[BY ORDER OF INF_SYS]\tSystem starts!\n");
            answerToInformSystem->V();
        }
    }

    return 0;
}

#endif //AUTO_GREENHOUSE_WATERSYSTEM_H
