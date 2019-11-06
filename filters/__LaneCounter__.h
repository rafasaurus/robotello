#include "Queue.h"
class __LaneCounter__ {
    private:
        int size;
        DataQueue<int> redQueue;
        DataQueue<int> blueQueue;
        DataQueue<int> greenQueue;
    public:
        __LaneCounter__() : redQueue(5),blueQueue(5),greenQueue(5) {};
        void printItems() {
            while(!redQueue.isEmpty()) {
                Serial.print("redQueue:");
                Serial.println(redQueue.dequeue());
            }
        }
        void push(int redColor, int greenColor, int blueColor) {
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
                greenQueue.enqueue(redColor);
            }
            else {
                greenQueue.dequeue();
                greenQueue.enqueue(redColor);
            }
            // push blue
            if (!blueQueue.isFull()) {
                blueQueue.enqueue(redColor);
            }
            else {
                blueQueue.dequeue();
                blueQueue.enqueue(redColor);
            }
        }
        void getStat() {
            Serial.print("items in redQueue cnt:");
            Serial.println(redQueue.item_count());
            Serial.print("items in greenQueue cnt:");
            Serial.println(greenQueue.item_count());
            Serial.print("items in blueQueue cnt:");
            Serial.println(blueQueue.item_count());
            int arr[5];
            redQueue.nextValue(arr);
            Serial.println("i*********************");
            for (int i = 0; i < 5; ++i) {
                Serial.println(arr[i]);
            }
        }
        void getColorSenseMean() {
        }
};

