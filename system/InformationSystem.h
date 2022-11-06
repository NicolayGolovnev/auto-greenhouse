//
// Created by kolya on 05.11.2022.
//

#ifndef AUTO_GREENHOUSE_INFORMATIONSYSTEM_H
#define AUTO_GREENHOUSE_INFORMATIONSYSTEM_H


#include "Semaphore.h"
#include "Channel.h"

class InformationSystem {
private:
    /** Система проветривания */
    Semaphore *airSemaphore, *airSystem;
    Channel *airChannel;

    /** Система полива */
    Semaphore *waterSemaphore, *waterSystem;
    Channel *waterChannel;

public:
    InformationSystem() {
        this->airSemaphore = new Semaphore("AirSensorInformSystem", false);
        this->airChannel = new Channel("AirSensorInformSystem");
        this->airSystem = new Semaphore("AirSystemInformSystem", false);

        this->waterSemaphore = new Semaphore("WaterSensorInformSystem", false);
        this->waterChannel = new Channel("WaterSensorInformSystem");
        this->waterSystem = new Semaphore("WaterSystemInformSystem", false);
    }
    ~InformationSystem() = default;

    void run() {
        bool execute = true;
        do {

        } while(execute);
    }
};


#endif //AUTO_GREENHOUSE_INFORMATIONSYSTEM_H
