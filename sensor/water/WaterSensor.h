//
// Created by kolya on 05.11.2022.
//

#ifndef AUTO_GREENHOUSE_WATERSENSOR_H
#define AUTO_GREENHOUSE_WATERSENSOR_H

#include <ctime>
#include "Semaphore.h"
#include "Channel.h"
#include "Writer.h"

class WaterSensor {
private:
    Semaphore *semaphore, *informSystemSemaphore;
    Writer *writer;

public:
    WaterSensor() {
        srand(time(nullptr));

        this->semaphore = new Semaphore("WaterSensor", false);
        this->informSystemSemaphore = new Semaphore("WaterSensorInformSystem", false);

        this->writer = new Writer("WaterSensor");
    }
    ~WaterSensor() = default;

    void run() {
        while (true) {
            this->semaphore->P();
            int value = this->writer->write();
            printf("[TO SYSTEM]\t Humidity is %d\n", value);

            // Если к нам обратилась информационная система для получения данных
            if (this->informSystemSemaphore->P(500) == WAIT_OBJECT_0)
                printf("[TO INF_SYS]\t Average humidity is %d\n", value);
        }
    }
};

DWORD WINAPI WaterSensorThread(LPVOID pVoid) {
    srand(time(nullptr));

    Semaphore *semaphore = new Semaphore("WaterSensor", false);
    Channel *channel = new Channel("WaterSensor");

    Semaphore *informSystem = new Semaphore("WaterSensorInformSystem", false);
    Channel *informChannel = new Channel("WaterSensorInformSystem");

    while (true) {
        semaphore->P();
        int humidity = (rand() + 55) % 100;
        channel->put(humidity);
        printf("[TO SYSTEM]\t Humidity is %d\n", humidity);

        // Если к нам обратилась информационная система для получения данных
        if (informSystem->P(500) == WAIT_OBJECT_0) {
            int humidityForInform = (rand() + 55) % 100;
            informChannel->put(humidityForInform);
            printf("[TO INF_SYS]\t Humidity is %d\n", humidityForInform);
        }
    }
}

#endif //AUTO_GREENHOUSE_WATERSENSOR_H
