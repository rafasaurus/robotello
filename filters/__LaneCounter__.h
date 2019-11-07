#include "Queue.h"
#define QUEUE_SIZE 10
#include "__Send__.h"

// SensorIds
#define R_id 0
#define L_id 1
#define RR_id 2
#define LL_id 3
#define COLORSENSE_RED_id 4
#define COLORSENSE_GREEN_id 5
#define COLORSENSE_BLUE_id 6

class __LaneCounter__ {
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
            int* getList() {
                return queueList;
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
    Sensor LL_;
    Sensor L_;
    Sensor R_;
    Sensor RR_;
    int* ll_arr;
    int* l_arr;
    int* r_arr;
    int* rr_arr;
    int* cR_arr;
    int* cB_arr;
    int* cG_arr;
    public:

    __LaneCounter__() {};
    void push(int ll,
            int l,
            int r,
            int rr,
            int redColor,
            int greenColor,
            int blueColor) {
        LL_.push_(ll);
        L_.push_(l); 
        R_.push_(r);
        RR_.push_(rr);
        cR_.push_(redColor);
        cB_.push_(blueColor);
        cG_.push_(greenColor);
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
        // Debug
        // Serial.println();
    }
};
