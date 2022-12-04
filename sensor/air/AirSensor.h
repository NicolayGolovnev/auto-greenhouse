//
// Created by kolya on 05.11.2022.
//

#ifndef AUTO_GREENHOUSE_AIRSENSOR_H
#define AUTO_GREENHOUSE_AIRSENSOR_H

#include <ctime>
#include "Semaphore.h"
#include "Channel.h"
#include "Writer.h"

class AirSensor {
private:
    Semaphore *semaphore, *informSystemSemaphore;
    Writer *writer;

public:
    AirSensor() {
        srand(time(nullptr));

        this->semaphore = new Semaphore("AirSensor", false);
        this->informSystemSemaphore = new Semaphore("AirSensorInformSystem", false);

        this->writer = new Writer("AirSensor");
    }
    ~AirSensor() = default;

    void run() {
        while (true) {
            this->semaphore->P();
            int value = this->writer->write();
            printf("[TO SYSTEM]\t Temperature is %d\n", value);

            // Если к нам обратилась информационная система для получения данных
            if (this->informSystemSemaphore->P(500) == WAIT_OBJECT_0)
                printf("[TO INF_SYS]\t Temperature is %d\n", value);
        }
    }
};

DWORD WINAPI AirSensorThread(LPVOID pVoid) {
    srand(time(nullptr));

    Semaphore *semaphore = new Semaphore("AirSensor", false);
    Channel *channel = new Channel("AirSensor");

    Semaphore *informSystem = new Semaphore("AirSensorInformSystem", false);
    Channel *informChannel = new Channel("AirSensorInformSystem");

    while (true) {
        semaphore->P();
        int temperature = (rand() + 11) % 50;
        channel->put(temperature);
        printf("[TO SYSTEM]\t Temperature is %d\n", temperature);

        // Если к нам обратилась информационная система для получения данных
        if (informSystem->P(500) == WAIT_OBJECT_0) {
            int temperatureForInform = (rand() + 11) % 50;
            informChannel->put(temperatureForInform);
            printf("[TO INF_SYS]\t Temperature is %d\n", temperatureForInform);
        }
    }
}

#endif //AUTO_GREENHOUSE_AIRSENSOR_H
