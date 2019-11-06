#include "Queue.h"
#define QUEUE_SIZE 20
class __LaneCounter__ {
    class Sensor {
        public:
            Sensor():queue(QUEUE_SIZE) {}
            DataQueue<int> queue;
            int queueList[QUEUE_SIZE] = {0}; 
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
            int getMean() {
                double mean = 0;
                for (int i = 0; i < QUEUE_SIZE; i++) {
                    mean += queueList[i];
                }
                return mean/QUEUE_SIZE;
            }
            
    };
    private:
        Sensor cR;
        Sensor cG;
        Sensor cB;
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
            cR.push(redColor);
            cB.push(blueColor);
            cG.push(greenColor);
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
            cR_arr = cR.getList();
            cB_arr = cB.getList();
            cG_arr = cG.getList();
        }
        void log() {
            // getLists();
            // Serial.println("i*********************");
            // for (int i = 0; i < QUEUE_SIZE; ++i) {
                Serial.print(" ");
                Serial.print(cR.getMean());
                Serial.print(" ");
                Serial.print(cG.getMean());
                Serial.print(" ");
                Serial.print(cB.getMean());
                Serial.print(" ");
                Serial.print(LL.getMean());
                Serial.print(" ");
                Serial.print(L.getMean());
                Serial.print(" ");
                Serial.print(R.getMean());
                Serial.print(" ");
                Serial.println(RR.getMean());
            // }
        }
};
