//
// Created by kolya on 05.11.2022.
//

#ifndef AUTO_GREENHOUSE_INFORMATIONSYSTEM_H
#define AUTO_GREENHOUSE_INFORMATIONSYSTEM_H

#include "Semaphore.h"
#include "Channel.h"
#include "Reader.h"

#define HIGH_TEMPERATURE 31
#define HIGH_HUMIDITY 66
#define CONSUME_AIR_TIMES 2
#define CONSUME_WATER_TIMES 2

class InformationSystem {
private:
    /** Система проветривания */
    Semaphore *airSemaphore, *airSystem, *answerAirSystem;
    Reader *airReader;

    /** Система полива */
    Semaphore *waterSemaphore, *waterSystem, *answerWaterSystem;
    Reader *waterReader;

    int temperature = 0, humidity = 0;

public:
    InformationSystem() {
        this->airSemaphore = new Semaphore("AirSensorInformSystem", false);
        this->airSystem = new Semaphore("AirSystemInformSystem", false);
        this->answerAirSystem = new Semaphore("AirSystemAnswerInformSystem", false);
        this->airReader = new Reader("AirSensor");

        this->waterSemaphore = new Semaphore("WaterSensorInformSystem", false);
        this->waterSystem = new Semaphore("WaterSystemInformSystem", false);
        this->answerWaterSystem = new Semaphore("WaterSystemAnswerInformSystem", false);
        this->waterReader = new Reader("WaterSensor");
    }
    ~InformationSystem() = default;

    void run() {
        printf("Information System for greenhouse\n\n");
        while (true) {
            printf("\nAverage temperature: ");
            if (temperature == 0) printf("indefinite\n"); else printf("%d\n", temperature);
            printf("Average humidity: ");
            if (humidity == 0) printf("indefinite\n"); else printf("%d\n", humidity);

            airSemaphore->V();
            this->temperature = this->airReader->read();
            waterSemaphore->V();
            this->humidity = this->waterReader->read();

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
};

DWORD WINAPI InformationSystemThread(LPVOID pVoid) {
    Semaphore *airSemaphore = new Semaphore("AirSensorInformSystem", false);
    Channel *airChannel = new Channel("AirSensorInformSystem");
    Semaphore *airSystem = new Semaphore("AirSystemInformSystem", false);
    Semaphore *answerAirSystem = new Semaphore("AirSystemAnswerInformSystem", false);

    Semaphore *waterSemaphore = new Semaphore("WaterSensorInformSystem", false);
    Channel *waterChannel = new Channel("WaterSensorInformSystem");
    Semaphore *waterSystem = new Semaphore("WaterSystemInformSystem", false);
    Semaphore *answerWaterSystem = new Semaphore("WaterSystemAnswerInformSystem", false);

    int temperature = 0, humidity = 0;

    while (true) {
        printf("\nTemperature: ");
        if (temperature == 0) printf("indefinite\n"); else printf("%d\n", temperature);
        printf("Humidity: ");
        if (humidity == 0) printf("indefinite\n"); else printf("%d\n", humidity);

        airSemaphore->V();
        temperature = airChannel->get();
        waterSemaphore->V();
        humidity = waterChannel->get();

        // если данные получили и оказались, что они критические - запускаем системы
        if (temperature > HIGH_TEMPERATURE)
            waterSystem->V();
        if (humidity > HIGH_HUMIDITY)
            airSystem->V();

        // смотрим, что к нам могут прийти ответы от этих систем
        if (answerAirSystem->P(500) == WAIT_OBJECT_0)
            printf("\tInitiation ventilation system is successed!\n");
        if (answerWaterSystem->P(500) == WAIT_OBJECT_0)
            printf("\tInitiation watering system is successed!\n");
    }

    return 0;
}

#endif //AUTO_GREENHOUSE_INFORMATIONSYSTEM_H
