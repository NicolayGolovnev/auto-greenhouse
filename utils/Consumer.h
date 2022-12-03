//
// Created by kolya on 03.12.2022.
//

#ifndef AUTO_GREENHOUSE_CONSUMER_H
#define AUTO_GREENHOUSE_CONSUMER_H

#include "Channel.h"

class Consumer {
private:
    Channel *channel;
    char const *name;
    int bufSize;
public:
    Consumer(const char *name, int bufSize) {
        this->channel = new Channel(name);
        this->name = name;
        this->bufSize = bufSize;
    }

    int consume() {
        int sumOfData = 0;
        int howMuch = bufSize;
        while (howMuch) {
            sumOfData += channel->get();
            howMuch--;
        }

//        printf("\tConsume %d times on %s route\n", bufSize, name);
        return sumOfData / bufSize;
    }

    ~Consumer() = default;
};

#endif //AUTO_GREENHOUSE_CONSUMER_H
