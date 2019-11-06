#include "Queue.h"
class __LaneCounter__ {
    private:
        int size;
        DataQueue<int> redQueue;
        DataQueue<int> blueQueue;
        DataQueue<int> GreenQueue;
    public:
        __LaneCounter__() : redQueue(5),blueQueue(5),greenQueue(5) {};
        void test() {
            for(int n=0; n<10; n++) { 
                if (!redQueue.isFull()) {
                    Serial.print("Items in queue now:");
                    Serial.println(redQueue.item_count());
                    redQueue.enqueue(n);
                }
                else {
                    Serial.println(redQueue.dequeue());
                }
            }
            Serial.println("");
            Serial.print("Size of each element:");
            Serial.println(redQueue.item_size());
            Serial.print("Items in queue now:");
            Serial.println(redQueue.item_count());
            Serial.print("Queue actual max items:");
            Serial.println(redQueue.max_queue_size());
            Serial.print("Queue actual max memory:");
            Serial.println(redQueue.max_memory_size());
        }
        void printItems() {
            while(!redQueue.isEmpty()) {
                Serial.print("redQueue:");
                Serial.println(redQueue.dequeue());
            }
        }
};

