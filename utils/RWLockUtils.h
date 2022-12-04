//
// Created by kolya on 04.12.2022.
//

#ifndef AUTO_GREENHOUSE_RWLOCKUTILS_H
#define AUTO_GREENHOUSE_RWLOCKUTILS_H

#include "Semaphore.h"
#include "FileMemMap.h"

int numW = 0, numR = 0,     // количество работающих писателей, читателей
    waitW = 0, waitR = 0;   // ожидающих писателей, читателей

/** Передача эстафеты читателю, писателю, вводу запроса */
void priorityRaceForReaders(Semaphore *w, Semaphore *r, Semaphore *e) {
    if ((numR == 0) && (numW == 0) && (waitW > 0)) {
        if (w != nullptr) {
            waitW--;
            w->V();     // разрешаем писателю доступ
        }
    }
    else if ((numW == 0) && (waitW == 0) && (waitR > 0)) {
        if (r != nullptr) {
            waitR--;
            r->V();     // разрешаем читателю доступ
        }
    } else if (e != nullptr) e->V();  // запрос ввода
}

#endif //AUTO_GREENHOUSE_RWLOCKUTILS_H
