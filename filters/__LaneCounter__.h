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

#define COLORSENSE1_RED_id 7
#define COLORSENSE1_GREEN_id 8
#define COLORSENSE1_BLUE_id 9

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
    Sensor cR1_;
    Sensor cG1_;
    Sensor cB1_;
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
    int* cR1_arr;
    int* cB1_arr;
    int* cG1_arr;
    public:

    __LaneCounter__() {};
    void push(int ll,
            int l,
            int r,
            int rr,
            int redColor,
            int greenColor,
            int blueColor,
            int redColor1,
            int greenColor1,
            int blueColor1) {
        LL_.push_(ll);
        L_.push_(l); 
        R_.push_(r);
        RR_.push_(rr);
        cR_.push_(redColor);
        cB_.push_(blueColor);
        cG_.push_(greenColor);
        cR1_.push_(redColor1);
        cB1_.push_(blueColor1);
        cG1_.push_(greenColor1);
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

        send(cR1_.getMean(), COLORSENSE1_RED_id);
        send(cG1_.getMean(), COLORSENSE1_GREEN_id);
        send(cB1_.getMean(), COLORSENSE1_BLUE_id);
        // Debug
        // Serial.println();
    }
};
