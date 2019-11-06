#include "Queue.h"
class __LaneCounter__ {
    private:
        int size;
        DataQueue<int> redQueue;
        DataQueue<int> greenQueue;
        DataQueue<int> blueQueue;

        int redQueueList[5] = {0};
        int greenQueueList[5] = {0};
        int blueQueueList[5] = {0};

        DataQueue<int> lineTrackerLeftLeftQueue;
        DataQueue<int> lineTrackerRightQueue;
        DataQueue<int> lineTrackerLeftQueue;
        DataQueue<int> lineTrackerRightRightQueue;

        int lineTrackerLeftLeftQueueList[5] = {0};
        int lineTrackerRightQueueList[5] = {0};
        int lineTrackerLeftQueueList[5] = {0};
        int lineTrackerRightRightQueueList[5] = {0};
    public:
        // init emptry lists and constructors
        __LaneCounter__():
            lineTrackerLeftLeftQueue(5),
            lineTrackerLeftQueue(5),
            lineTrackerRightQueue(5),
            lineTrackerRightRightQueue(5),
            redQueue(5),
            blueQueue(5),
            greenQueue(5) {};

        void printItems() {
            while(!redQueue.isEmpty()) {
                Serial.print("redQueue:");
                Serial.println(redQueue.dequeue());
            }
        }
        void push(int redColor,
                    int greenColor,
                    int blueColor,
                    int lineTrackerLeftLeft,
                    int lineTrackerLeft,
                    int lineTrackerRight,
                    int lineTrackerRightRight) {
            // push red
            if (!redQueue.isFull()) {
                redQueue.enqueue(redColor);
            }
            else {
                redQueue.dequeue();
                redQueue.enqueue(redColor);
            }
            // push red
            if (!greenQueue.isFull()) {
                greenQueue.enqueue(greenColor);
            }
            else {
                greenQueue.dequeue();
                greenQueue.enqueue(greenColor);
            }
            // push blue
            if (!blueQueue.isFull()) {
                blueQueue.enqueue(blueColor);
            }
            else {
                blueQueue.dequeue();
                blueQueue.enqueue(blueColor);
            }
            redQueue.nextValue(redQueueList);
            greenQueue.nextValue(greenQueueList);
            blueQueue.nextValue(blueQueueList);
            //  push LeftLeft
            if (!lineTrackerLeftLeftQueue.isFull()) {
                lineTrackerLeftLeftQueue.enqueue(lineTrackerLeftLeft);
            }
            else {
                lineTrackerLeftLeftQueue.dequeue();
                lineTrackerLeftLeftQueue.enqueue(lineTrackerLeftLeft);
            }
            //  push Left
            if (!lineTrackerLeftQueue.isFull()) {
                lineTrackerLeftQueue.enqueue(lineTrackerLeft);
            }
            else {
                lineTrackerLeftQueue.dequeue();
                lineTrackerLeftQueue.enqueue(lineTrackerLeft);
            }
            //  push Right
            if (!lineTrackerRightQueue.isFull()) {
                lineTrackerRightQueue.enqueue(lineTrackerRight);
            }
            else {
                lineTrackerRightQueue.dequeue();
                lineTrackerRightQueue.enqueue(lineTrackerRight);
            }
            //  push RightRight
            if (!lineTrackerRightRightQueue.isFull()) {
                lineTrackerRightRightQueue.enqueue(lineTrackerRightRight);
            }
            else {
                lineTrackerRightRightQueue.dequeue();
                lineTrackerRightRightQueue.enqueue(lineTrackerRightRight);
            }
        }
        void getStat() {
            Serial.print("items in redQueue cnt:");
            Serial.println(redQueue.item_count());
            Serial.print("items in greenQueue cnt:");
            Serial.println(greenQueue.item_count());
            Serial.print("items in blueQueue cnt:");
            Serial.println(blueQueue.item_count());
            Serial.println("i*********************");
            for (int i = 0; i < 5; ++i) {
                Serial.print("red:");
                Serial.print(this->redQueueList[i]);
                Serial.print(" green:");
                Serial.print(this->greenQueueList[i]);
                Serial.print(" blue:");
                Serial.print(this->blueQueueList[i]);
                Serial.print(" L:");
                Serial.print(this->lineTrackerLeftLeftQueueList[i]);
                Serial.print(" LL:");
                Serial.print(this->lineTrackerLeftQueueList[i]);
                Serial.print(" R:");
                Serial.print(this->lineTrackerRightQueueList[i]);
                Serial.print(" RR:");
                Serial.println(this->lineTrackerRightRightQueueList[i]);
            }
        }
        void getColorSenseMean() {
        }
};

