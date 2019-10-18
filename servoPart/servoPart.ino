#include <Servo.h>

class __Servo__ {
    private: 
        int min_;
        int max_;
        int delay_;
        int angle_;
        Servo servo_;

    public:
        __Servo__(int min_, int max_, int delay_, int pin_) {
            this->min_ = min_;
            this->max_ = max_;
            this->delay_ = delay_;
            servo_.attach(pin_);  // open = 0, close = 60
        }

        __Servo__(int pin_) {
            servo_.attach(pin_);  // open = 0, close = 60
        }

        void open() {
            for (int angle = this->min_; angle <= this->max_; angle+=1) {
                delay(this->delay_);
                servo_.write(angle);
            }
        }

        void close () {
            for (int angle = this->max_; angle >= this->min_; angle-=1) {
                delay(this->delay_);
                servo_.write(angle);
            }
        }

        void setAngle(int angle) {
            servo_.write(angle);
        }

        ~__Servo__();
};

__Servo__ *arm;
__Servo__ *armOne;
__Servo__ *armTwo;
__Servo__ *armRotate;

void setup() {
    arm = new __Servo__(0, 60, 15, 5);
    armTwo = new __Servo__(0, 168, 30, 3);
    armOne = new __Servo__(0, 180, 30,4);
    armRotate = new __Servo__(2);
}

void loop() {
    armRotate->setAngle(130);
    delay(1000);
}
