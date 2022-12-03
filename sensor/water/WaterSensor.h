//
// Created by kolya on 05.11.2022.
//

#ifndef AUTO_GREENHOUSE_WATERSENSOR_H
#define AUTO_GREENHOUSE_WATERSENSOR_H

#include <ctime>
#include "Producer.h"
#include "Semaphore.h"
#include "Channel.h"

#define PRODUCE_WATER_COUNT 2

class WaterSensor {
private:
    Semaphore *semaphore, *informSystemSemaphore;
    Producer *producer;

    /** Генерация значения влажности от 55% до 100% */
    static long generateValue() {
        return (rand() + 55) % 100;
    }

    static int *generateHumidities() {
        int *producedHumidity = new int[PRODUCE_WATER_COUNT];
        for (int i = 0; i < PRODUCE_WATER_COUNT; i++)
            producedHumidity[i] = generateValue();

        return producedHumidity;
    }

public:
    WaterSensor() {
        srand(time(nullptr));

        this->semaphore = new Semaphore("WaterSensor", false);
        this->informSystemSemaphore = new Semaphore("WaterSensorInformSystem", false);

        this->producer = new Producer("WaterSensor", PRODUCE_WATER_COUNT);
    }
    ~WaterSensor() = default;

    void run() {
        while (true) {
            this->semaphore->P();
            int *humidities = generateHumidities();
            int averageHumidity = 0;
            for (int i = 0; i < PRODUCE_WATER_COUNT; i++)
                averageHumidity += humidities[i];

            this->producer->produce(humidities);
            printf("[TO SYSTEM]\t Average humidity is %d\n", averageHumidity / PRODUCE_WATER_COUNT);

            // Если к нам обратилась информационная система для получения данных
            if (this->informSystemSemaphore->P(500) == WAIT_OBJECT_0) {
                int *humiditiesForInform = generateHumidities();
                int averageHumidityForInform = 0;
                for (int i = 0; i < PRODUCE_WATER_COUNT; i++)
                    averageHumidityForInform += humiditiesForInform[i];

                this->producer->produce(humiditiesForInform);
                printf("[TO INF_SYS]\t Average humidity is %d\n", averageHumidityForInform / PRODUCE_WATER_COUNT);
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
}

#endif //AUTO_GREENHOUSE_WATERSENSOR_H
