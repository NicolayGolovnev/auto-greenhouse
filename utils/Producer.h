//
// Created by kolya on 03.12.2022.
//

#ifndef AUTO_GREENHOUSE_PRODUCER_H
#define AUTO_GREENHOUSE_PRODUCER_H


#include "Channel.h"

class Producer {
private:
    Channel *channel;
    const char *name;
    int bufSize;

public:
    Producer(const char *name, int bufSize) {
        this->name = name;
        this->bufSize = bufSize;
        this->channel = new Channel(name);
    }

    void produce(int *data) {
        int howMuch = bufSize;
        int index = 0;
        while (howMuch) {
            channel->put(data[index++]);
            howMuch--;
        }

//        printf("Produce %d times for %s's route\n", bufSize, name);
    }

    ~Producer() = default;
};


#endif //AUTO_GREENHOUSE_PRODUCER_H
