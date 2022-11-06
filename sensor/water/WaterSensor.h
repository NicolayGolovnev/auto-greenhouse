//
// Created by kolya on 05.11.2022.
//

#ifndef AUTO_GREENHOUSE_WATERSENSOR_H
#define AUTO_GREENHOUSE_WATERSENSOR_H


#include "Semaphore.h"
#include "Channel.h"

class WaterSensor {
private:
    Semaphore *semaphore, *informSystem;
    Channel *channel, *informChannel;

    /** Генерация значения влажности от 55% до 100% */
    long generateValue() {
        return (rand() + 55) % 100;
    }
public:
    WaterSensor() {
        srand(time(nullptr));

        this->semaphore = new Semaphore("WaterSensor", false);
        this->channel = new Channel("WaterSensor");

        this->informSystem = new Semaphore("WaterSensorInformSystem", false);
        this->informChannel = new Channel("WaterSensorInformSystem");
    }
    ~WaterSensor() = default;

    void run() {
        while (true) {
            this->semaphore->P();
            int humidity = generateValue();
            this->channel->put(humidity);
            printf("[TO SYSTEM]\t Humidity is %d\n", humidity);

            // Если к нам обратилась информационная система для получения данных
            if (this->informSystem->P(500) == WAIT_OBJECT_0) {
                int humidityForInform = generateValue();
                this->informChannel->put(humidityForInform);
                printf("[TO INF_SYS]\t Humidity is %d\n", humidityForInform);
            }
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

    return 0;
}

#endif //AUTO_GREENHOUSE_WATERSENSOR_H
