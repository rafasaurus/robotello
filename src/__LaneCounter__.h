#ifndef laneCounter
#define lanCounter

#include "FIFO.h"
class __LaneCounter__ {
    private:
        FIFO fifoCounter;
    public:
        __LaneCounter() {
        }
        void test() {
            char test[64] = "Hello, World!!!\nHello, World!!!\nHello, World!!!\nHello, World!!!";
            for (byte i = 0; i < sizeof(test); i++) {
                Serial.write(test[i]);
                this->fifoCounter.push(test[i]);
                delay(100);
            }
            while (this->fifoCounter.size() > 0) {
                Serial.write((char)this->fifoCounter.pop());
                delay(100);
            }
        }
};
#endif
