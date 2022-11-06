# Программирование параллельных процессов
    Проект создан для выполнения лабораторных работ по данному курсу.
    Работа выполняется в среде CLion

## #1. Реализация системы "Теплица-автомат"
    Для запуска программы с помощью подпрограмм необходимо скомпилировать исполняемые файлы и поотдельности
    запустить все 5 программ для запуска цельной системы "Теплица-автомат".
    
Для этого необходимо в функции `main` создать экземпляр класса и вызвать метод `run()`. Например, так:
```c++
#include "AirSensor.h"

int main() {
    AirSensor *airSensor = new AirSensor();
    airSensor->run();
    
    return 0;
}
```
Либо запустить скомпилированные программы, которые лежат в `/examples`