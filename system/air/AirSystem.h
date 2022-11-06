//
// Created by kolya on 05.11.2022.
//

#ifndef AUTO_GREENHOUSE_AIRSYSTEM_H
#define AUTO_GREENHOUSE_AIRSYSTEM_H

#include "Semaphore.h"
#include "Channel.h"

#define OVERHEAT 33
#define OVERCOOLING 18

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
        this->answerToInformSystem = new Semaphore("AirSystemInformSystem", false);
    }
    ~AirSystem() { }

    void run() {
        while (true) {
            this->semaphore->V();
            int dataFromSensor = this->channel->get();

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
            if (this->informSystem->P(500) == WAIT_OBJECT_0) {
                printf("\t[BY ORDER OF INF_SYS]System starts!\n");
                this->answerToInformSystem->V();
            }
        }
    }

    void runLikeThread() {

    }
};


#endif //AUTO_GREENHOUSE_AIRSYSTEM_H
