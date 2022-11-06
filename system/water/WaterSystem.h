//
// Created by kolya on 05.11.2022.
//

#ifndef AUTO_GREENHOUSE_WATERSYSTEM_H
#define AUTO_GREENHOUSE_WATERSYSTEM_H


#include "Semaphore.h"
#include "Channel.h"

#define HIGH_HUMIDITY 90
#define LOW_HUMIDITY 65

class WaterSystem {
private:
    Semaphore *semaphore, *informSystem, *answerToInformSystem;
    Channel *channel;
    bool isWorking = false;

public:
    WaterSystem() {
        this->semaphore = new Semaphore("WaterSensor", false);
        this->channel = new Channel("WaterSensor");

        this->informSystem = new Semaphore("WaterSystemInformSystem", false);
        this->answerToInformSystem = new Semaphore("WaterSystemAnswerInformSystem", false);
    }
    ~WaterSystem() = default;

    void run() {
        while (true) {
            this->semaphore->V();
            int dataFromSensor = this->channel->get();

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
            if (this->informSystem->P(500) == WAIT_OBJECT_0) {
                printf("\t[BY ORDER OF INF_SYS]\tSystem starts!\n");
                this->answerToInformSystem->V();
            }
        }
    }

    void runLikeThread() {

    }
};


#endif //AUTO_GREENHOUSE_WATERSYSTEM_H
