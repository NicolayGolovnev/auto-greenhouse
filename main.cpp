#include "AirSystem.h"
#include "AirSensor.h"
#include "WaterSensor.h"
#include "WaterSystem.h"
#include "InformationSystem.h"

void runThreads() {
    AirSensor *airSensor = new AirSensor();
    AirSystem *airSystem = new AirSystem();
    WaterSensor *waterSensor = new WaterSensor();
    WaterSystem *waterSystem = new WaterSystem();
    InformationSystem *system = new InformationSystem();

    HANDLE threadProducer[5];
    DWORD dwNetThreadId[5];

    threadProducer[0] = CreateThread(nullptr, 0, AirSensorThread, (void*) nullptr, 0, &dwNetThreadId[0]);
    threadProducer[1] = CreateThread(nullptr, 0, AirSystemThread, (void*) nullptr, 0, &dwNetThreadId[1]);
    threadProducer[2] = CreateThread(nullptr, 0, WaterSensorThread, (void*) nullptr, 0, &dwNetThreadId[2]);
    threadProducer[3] = CreateThread(nullptr, 0, WaterSystemThread, (void*) nullptr, 0, &dwNetThreadId[3]);
    threadProducer[4] = CreateThread(nullptr, 0, InformationSystemThread, (void*) nullptr, 0, &dwNetThreadId[4]);

    WaitForMultipleObjects(5, threadProducer, true, INFINITE);
}

int main() {
    /** Запуск системы теплицы в виде потоков */
    runThreads();

    /** Межпроцессорное взаимодействие */
//    AirSensor *airSensor = new AirSensor();
//    airSensor->run();

//    AirSystem *aeroSystem = new AirSystem();
//    aeroSystem->run();

//    WaterSensor *waterSensor = new WaterSensor();
//    waterSensor->run();

//    WaterSystem *waterSystem = new WaterSystem();
//    waterSystem->run();

//    InformationSystem *system = new InformationSystem();
//    system->run();

    return 0;
}
