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
        int* cR_arr;
        int* cB_arr;
        int* cG_arr;
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
            cR_arr = colorSenseRed.getList();
            cB_arr = colorSenseBlue.getList();
            cG_arr = colorSenseGreen.getList();
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
                Serial.print(rr_arr[i]);
                Serial.print(" cR:");
                Serial.print(rr_arr[i]);
                Serial.print(" cB:");
                Serial.print(rr_arr[i]);
                Serial.print(" cG:");
                Serial.println(rr_arr[i]);
            }
        }
};
