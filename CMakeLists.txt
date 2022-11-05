cmake_minimum_required(VERSION 3.23)
project(auto_greenhouse)

set(CMAKE_CXX_STANDARD 11)

add_executable(
        auto_greenhouse main.cpp 
        system/InformationSystem.cpp system/InformationSystem.h
        system/water/WaterSystem.cpp system/water/WaterSystem.h
        system/aero/AeroSystem.cpp system/aero/AeroSystem.h
        sensor/aero/AeroSensor.cpp sensor/aero/AeroSensor.h
        sensor/water/WaterSensor.cpp sensor/water/WaterSensor.h
)
