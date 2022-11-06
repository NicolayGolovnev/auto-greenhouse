//
// Created by kolya on 05.11.2022.
//

#ifndef AUTO_GREENHOUSE_AIRSENSOR_H
#define AUTO_GREENHOUSE_AIRSENSOR_H

#include "Semaphore.h"
#include "Channel.h"

class AirSensor {
private:
    Semaphore *semaphore, *informSystem;
    Channel *channel, *informChannel;

    /** Генерация значения температуры от 11 до 50 */
    long generateValue() {
        return (rand() + 11) % 50;
    }

public:
    AirSensor() {
        srand(time(nullptr));

        this->semaphore = new Semaphore("AirSensor", false);
        this->channel = new Channel("AirSensor");

        this->informSystem = new Semaphore("AirSensorInformSystem", false);
        this->informChannel = new Channel("AirSensorInformSystem");
    }
    ~AirSensor() = default;

    void run() {
        while (true) {
            this->semaphore->P();
            int temperature = generateValue();
            this->channel->put(temperature);
            printf("[TO SYSTEM]\t Temperature is %d\n", temperature);

            // Если к нам обратилась информационная система для получения данных
            if (this->informSystem->P(500) == WAIT_OBJECT_0) {
                int temperatureForInform = generateValue();
                this->informChannel->put(temperatureForInform);
                printf("[TO INF_SYS]\t Temperature is %d\n", temperatureForInform);
            }
        }
    }

    void runLikeThread() {

    }
};


#endif //AUTO_GREENHOUSE_AIRSENSOR_H
