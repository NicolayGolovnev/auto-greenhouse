cmake_minimum_required(VERSION 3.15)
project(auto_greenhouse)

set(CMAKE_CXX_STANDARD 11)

include_directories(sensor/air sensor/water system system/water system/air utils)

add_executable(
        auto_greenhouse main.cpp
        system/InformationSystem.h
        system/water/WaterSystem.h
        system/air/AirSystem.h
        sensor/air/AirSensor.h
        sensor/water/WaterSensor.h
        utils/Channel.h utils/Semaphore.h
        utils/Consumer.h utils/Producer.h
        utils/Reader.h utils/Writer.h
        utils/RWLockUtils.h utils/FileMemMap.h
)
