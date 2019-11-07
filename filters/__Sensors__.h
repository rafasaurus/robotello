#include "Queue.h"
#define QUEUE_SIZE 7
#include "__Send__.h"

// SensorIds
#define R_id 0
#define L_id 1
#define RR_id 2
#define LL_id 3

#define COLORSENSE_RED_id 4
#define COLORSENSE_GREEN_id 5
#define COLORSENSE_BLUE_id 6

#define COLORSENSE1_MEAN 7

class __Sensors__ {
    class Sensor {
        public:
            // Init emptry queue and constructor
            Sensor():queue(QUEUE_SIZE) {}
            DataQueue<int> queue;
            int queueList[QUEUE_SIZE] = {0}; 
            void push_(int data) {
                if (!queue.isFull()) {
                    queue.enqueue(data);
                }
                else {
                    queue.dequeue();
                    queue.enqueue(data);
                }
                queue.nextValue(queueList);
            }
            int getMean() {
                double mean = 0;
                for (int i = 0; i < QUEUE_SIZE; i++) {
                    mean += queueList[i];
                }
                return mean/QUEUE_SIZE;
            }

    };
    private:
    Sensor cR_;
    Sensor cG_;
    Sensor cB_;
    Sensor cS1_mean_;
    Sensor LL_;
    Sensor L_;
    Sensor R_;
    Sensor RR_;
    public:

    __Sensors__() {};
    void push(int ll,
            int l,
            int r,
            int rr,
            int redColor,
            int greenColor,
            int blueColor,
            int colorSense1_mean) {
        LL_.push_(ll);
        L_.push_(l); 
        R_.push_(r);
        RR_.push_(rr);
        cR_.push_(redColor);
        cB_.push_(blueColor);
        cG_.push_(greenColor);
        cS1_mean_.push_(colorSense1_mean);
    }
    void sendPayload() {
        // lineTrackers payload
        send(L_.getMean(), L_id);
        send(LL_.getMean(), LL_id);
        send(R_.getMean(), R_id);
        send(RR_.getMean(), RR_id);
        // colorSensor payload
        send(cR_.getMean(), COLORSENSE_RED_id);
        send(cG_.getMean(), COLORSENSE_GREEN_id);
        send(cB_.getMean(), COLORSENSE_BLUE_id);

        send(cS1_mean_.getMean(), COLORSENSE1_MEAN);
        // Debug
        Serial.println();
    }
};
