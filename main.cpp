#include "AirSystem.h"
#include "AirSensor.h"
#include "InformationSystem.h"

int main() {
//    AirSensor *airSensor = new AirSensor();
//    airSensor->run();

//    AirSystem *aeroSystem = new AirSystem();
//    aeroSystem->run();

    InformationSystem *system = new InformationSystem();
    system->run();

    return 0;
}
