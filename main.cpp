#include <iostream>

#include "AirSystem.h"
#include "AirSensor.h"

int main() {
//    AirSensor *airSensor = new AirSensor();
//    airSensor->run();

    AirSystem *aeroSystem = new AirSystem();
    aeroSystem->run();

    return 0;
}
