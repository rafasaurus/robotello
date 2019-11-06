#include "Queue.h"
class __LaneCounter__ {
    class Sensor {
        public:
            Sensor():queue(5) {}
            DataQueue<int> queue;
            int queueList[5] = {0}; 
            void push(int data) {
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
            
    };
    private:
        Sensor colorSenseRed;
        Sensor colorSenseGreen;
        Sensor colorSenseBlue;
        Sensor LL;
        Sensor L;
        Sensor R;
        Sensor RR;
        int* ll_arr;
        int* l_arr;
        int* r_arr;
        int* rr_arr;
    public:
        // init emptry lists and constructors
        __LaneCounter__() {};
        void push(int ll,
                int l,
                int r,
                int rr,
                int redColor,
                int greenColor,
                int blueColor) {
            colorSenseRed.push(redColor);
            colorSenseBlue.push(blueColor);
            colorSenseGreen.push(greenColor);
            LL.push(ll);
            L.push(l); 
            R.push(r);
            RR.push(rr);
        }

        void getLists() {
            ll_arr = LL.getList();
            l_arr = L.getList();
            r_arr = R.getList();
            rr_arr = RR.getList();
        }
        void log() {
            getLists();
            Serial.println("i*********************");
            for (int i = 0; i < 5; ++i) {
                Serial.print("ll:");
                Serial.print(ll_arr[i]);
                Serial.print(" l:");
                Serial.print(l_arr[i]);
                Serial.print(" r:");
                Serial.print(r_arr[i]);
                Serial.print(" rr:");
                Serial.println(rr_arr[i]);
            }
            // Serial.print("items in redQueue cnt:");
            // Serial.println(redQueue.item_count());
            // Serial.print("items in greenQueue cnt:");
            // Serial.println(greenQueue.item_count());
            // Serial.print("items in blueQueue cnt:");
            // Serial.println(blueQueue.item_count());
            // Serial.println("i*********************");
            // for (int i = 0; i < 5; ++i) {
            //     Serial.print("red:");
            //     Serial.print(this->redQueueList[i]);
            //     Serial.print(" green:");
            //     Serial.print(this->greenQueueList[i]);
            //     Serial.print(" blue:");
            //     Serial.print(this->blueQueueList[i]);
            //     Serial.print(" L:");
            //     Serial.print(this->lineTrackerLeftLeftQueueList[i]);
            //     Serial.print(" LL:");
            //     Serial.print(this->lineTrackerLeftQueueList[i]);
            //     Serial.print(" R:");
            //     Serial.print(this->lineTrackerRightQueueList[i]);
            //     Serial.print(" RR:");
            //     Serial.println(this->lineTrackerRightRightQueueList[i]);
            // }
        }
        void getColorSenseMean() {
        }
};

