//
// Created by kolya on 05.11.2022.
//

#ifndef AUTO_GREENHOUSE_AIRSYSTEM_H
#define AUTO_GREENHOUSE_AIRSYSTEM_H

#include "Semaphore.h"
#include "Channel.h"

#define OVERHEAT 40
#define OVERCOOLING 22

class AirSystem {
private:
    Semaphore *semaphore, *informSystem, *answerToInformSystem;
    Channel *channel;
    bool isWorking = false;

public:
    AirSystem() {
        this->semaphore = new Semaphore("AirSensor", false);
        this->channel = new Channel("AirSensor");

        this->informSystem = new Semaphore("AirSystemInformSystem", false);
        this->answerToInformSystem = new Semaphore("AirSystemAnswerInformSystem", false);
    }
    ~AirSystem() = default;

    void run() {
        while (true) {
            this->semaphore->V();
            int dataFromSensor = this->channel->get();

            // Обработка данных с датчика
            if (dataFromSensor > OVERHEAT) {
                this->isWorking = true;
                printf("[OVERHEAT]\tTemperature = %d, system starts!\n", dataFromSensor);
            }
            else if (dataFromSensor < OVERCOOLING) {
                this->isWorking = false;
                printf("[OVERCOOLING]\tTemperature = %d, system shutdown!\n", dataFromSensor);
            }
            else {
                if (this->isWorking)
                    printf("[NORMAL]\tTemperature = %d, system on!\n", dataFromSensor);
                else
                    printf("[NORMAL]\tTemperature = %d, system off!\n", dataFromSensor);
            }

            // Если есть запрос на включение системы из вне - включаем и сообщаем об этом
            if (this->informSystem->P(500) == WAIT_OBJECT_0) {
                printf("\t[BY ORDER OF INF_SYS]\tSystem starts!\n");
                this->answerToInformSystem->V();
            }
        }
    }
};

DWORD WINAPI AirSystemThread(LPVOID pVoid) {
    Semaphore *semaphore = new Semaphore("AirSensor", false);
    Channel *channel = new Channel("AirSensor");

    Semaphore *informSystem = new Semaphore("AirSystemInformSystem", false);
    Semaphore *answerToInformSystem = new Semaphore("AirSystemAnswerInformSystem", false);
    bool isWorking = false;

    while (true) {
        semaphore->V();
        int dataFromSensor = channel->get();

        // Обработка данных с датчика
        if (dataFromSensor > OVERHEAT) {
            isWorking = true;
            printf("[OVERHEAT]\tTemperature = %d, system starts!\n", dataFromSensor);
        }
        else if (dataFromSensor < OVERCOOLING) {
            isWorking = false;
            printf("[OVERCOOLING]\tTemperature = %d, system shutdown!\n", dataFromSensor);
        }
        else {
            if (isWorking)
                printf("[NORMAL]\tTemperature = %d, system on!\n", dataFromSensor);
            else
                printf("[NORMAL]\tTemperature = %d, system off!\n", dataFromSensor);
        }

        // Если есть запрос на включение системы из вне - включаем и сообщаем об этом
        if (informSystem->P(500) == WAIT_OBJECT_0) {
            printf("\t[BY ORDER OF INF_SYS]\tSystem starts!\n");
            answerToInformSystem->V();
        }
    }

    return 0;
}

#endif //AUTO_GREENHOUSE_AIRSYSTEM_H
