//
// Created by kolya on 05.11.2022.
//

#ifndef AUTO_GREENHOUSE_INFORMATIONSYSTEM_H
#define AUTO_GREENHOUSE_INFORMATIONSYSTEM_H


#include "Semaphore.h"
#include "Channel.h"

#define HIGH_TEMPERATURE 31
#define HIGH_HUMIDITY 66

class InformationSystem {
private:
    /** Система проветривания */
    Semaphore *airSemaphore, *airSystem, *answerAirSystem;
    Channel *airChannel;

    /** Система полива */
    Semaphore *waterSemaphore, *waterSystem, *answerWaterSystem;
    Channel *waterChannel;

    int temperature = 0, humidity = 0;

public:
    InformationSystem() {
        this->airSemaphore = new Semaphore("AirSensorInformSystem", false);
        this->airChannel = new Channel("AirSensorInformSystem");
        this->airSystem = new Semaphore("AirSystemInformSystem", false);
        this->answerAirSystem = new Semaphore("AirSystemAnswerInformSystem", false);

        this->waterSemaphore = new Semaphore("WaterSensorInformSystem", false);
        this->waterChannel = new Channel("WaterSensorInformSystem");
        this->waterSystem = new Semaphore("WaterSystemInformSystem", false);
        this->answerWaterSystem = new Semaphore("WaterSystemAnswerInformSystem", false);
    }
    ~InformationSystem() = default;

    void run() {
        printf("Information System for greenhouse\n\n");
        while (true) {
            printf("\nTemperature: ");
            if (temperature == 0) printf("indefinite\n"); else printf("%d\n", temperature);
            printf("Humidity: ");
            if (humidity == 0) printf("indefinite\n"); else printf("%d\n", humidity);

            airSemaphore->V();
            this->temperature = airChannel->get();
            waterSemaphore->V();
            this->humidity = waterChannel->get();

            // если данные получили и оказались, что они критические - запускаем системы
            if (this->temperature > HIGH_TEMPERATURE)
                waterSystem->V();
            if (this->humidity > HIGH_HUMIDITY)
                airSystem->V();

            // смотрим, что к нам могут прийти ответы от этих систем
            if (this->answerAirSystem->P(500) == WAIT_OBJECT_0)
                printf("\tInitiation ventilation system is successed!\n");
            if (this->answerWaterSystem->P(500) == WAIT_OBJECT_0)
                printf("\tInitiation watering system is successed!\n");
        }
    }

    void runLikeThread() {

    }
};


#endif //AUTO_GREENHOUSE_INFORMATIONSYSTEM_H
