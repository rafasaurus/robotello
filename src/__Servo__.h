#ifndef __servo__
#define __servo__

#include <Servo.h>

class __Servo__ {
    private: 
        int min_;
        int max_;
        int delay_;
        int angle_;
        Servo servo_;
    public:
        __Servo__(int min_, int max_, int delay_, int pin_);
        __Servo__(int pin_);
        void open();
        void close();
        void setAngle(int angle);
};

__Servo__::__Servo__(int min_, int max_, int delay_, int pin_) {
    this->min_ = min_;
    this->max_ = max_;
    this->delay_ = delay_;
    servo_.attach(pin_);  // open = 0, close = 60
}

__Servo__::__Servo__(int pin_) {
    servo_.attach(pin_);  // open = 0, close = 60
}
void
__Servo__::open() {
    for (int angle = this->min_; angle <= this->max_; angle+=1) {
        delay(this->delay_);
        servo_.write(angle);
    }
}
void
__Servo__::close () {
    for (int angle = this->max_; angle >= this->min_; angle-=1) {
        delay(this->delay_);
        servo_.write(angle);
    }
}
void
__Servo__::setAngle(int angle) {
    servo_.write(angle);
}


#endif
