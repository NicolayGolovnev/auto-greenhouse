//
// Created by kolya on 05.11.2022.
//

#ifndef AUTO_GREENHOUSE_AIRSENSOR_H
#define AUTO_GREENHOUSE_AIRSENSOR_H

#include <ctime>
#include "Producer.h"
#include "Semaphore.h"
#include "Channel.h"

#define PRODUCE_TEMPERATURE_COUNT 2

class AirSensor {
private:
    Semaphore *semaphore, *informSystemSemaphore;
    Producer *producer;

    /** Генерация значения температуры от 11 до 50 */
    static long generateValue() {
        return (rand() + 11) % 50;
    }

    int *generateTemperatures() {
        int *producedTemperature = new int[PRODUCE_TEMPERATURE_COUNT];
        for (int i = 0; i < PRODUCE_TEMPERATURE_COUNT; i++)
            producedTemperature[i] = generateValue();

        return producedTemperature;
    }

public:
    AirSensor() {
        srand(time(nullptr));

        this->semaphore = new Semaphore("AirSensor", false);
        this->informSystemSemaphore = new Semaphore("AirSensorInformSystem", false);

        this->producer = new Producer("AirSensor", PRODUCE_TEMPERATURE_COUNT);
    }
    ~AirSensor() = default;

    void run() {
        while (true) {
            this->semaphore->P();
            int *temperatures = generateTemperatures();
            int averageTemperature = 0;
            for (int i = 0; i < PRODUCE_TEMPERATURE_COUNT; i++)
                averageTemperature += temperatures[i];

            this->producer->produce(temperatures);
            printf("[TO SYSTEM]\t Average temperature is %d\n", averageTemperature / PRODUCE_TEMPERATURE_COUNT);

            // Если к нам обратилась информационная система для получения данных
            if (this->informSystemSemaphore->P(500) == WAIT_OBJECT_0) {
                int *temperaturesForInform = generateTemperatures();
                int averageTemperatureForInform = 0;
                for (int i = 0; i < PRODUCE_TEMPERATURE_COUNT; i++)
                    averageTemperatureForInform += temperaturesForInform[i];

                this->producer->produce(temperaturesForInform);
                printf("[TO INF_SYS]\t Average temperature is %d\n", averageTemperatureForInform / PRODUCE_TEMPERATURE_COUNT);
            }
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
