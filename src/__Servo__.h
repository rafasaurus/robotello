#ifndef __servo__
#define __servo__

#include <Servo.h>

#ifndef RTOS
#define RTOS
#include <Arduino_FreeRTOS.h>
#include <event_groups.h>
#include <queue.h>
#endif

class __Servo__ {
  private:
    int min_;
    int max_;
    int delay_;
    int angle_;
    Servo servo_;
  public:
    __Servo__(int min_, int max_, int delay_, int pin_, int defaultAngle);
    __Servo__(int pin_, int defaultAngle); void open();
    void close();
    void setAngle(int angle);
    void setAngleSlowly(int angle);
};

__Servo__::__Servo__(int min_, int max_, int delay_, int pin_, int defaultAngle) {
  this->min_ = min_;
  this->max_ = max_;
  this->delay_ = delay_;
  servo_.attach(pin_);  // open = 0, close = 60
  this->setAngle(defaultAngle);

}

__Servo__::__Servo__(int pin_, int defaultAngle) {
  servo_.attach(pin_);  // open = 0, close = 60
  this->setAngle(defaultAngle);
}
void
__Servo__::open() {
  for (int angle = this->angle_; angle <= this->max_; angle += 1) {
    delay(this->delay_);
    this->servo_.write(angle);
    this->angle_ = angle;

  }
}
void
__Servo__::close () {
  for (int angle = this->angle_; angle >= this->min_; angle -= 1) {
    delay(this->delay_);
    this->servo_.write(angle);
    this->angle_ = angle;

  }
}
// for getting trash ca
void
__Servo__::setAngleSlowly (int angle) {
  if (this->angle_ > angle) {
    for (int ang = this->angle_; ang >= angle; ang -= 1) {
      delay(this->delay_);
      this->servo_.write(ang);
      this->angle_ = ang;
    }
  }
  else {
    for (int ang = this->angle_; ang <= angle; ang += 1) {
      delay(this->delay_);
      this->servo_.write(ang);
      this->angle_ = ang;
    }
  }


}
void
__Servo__::setAngle(int angle) {
  servo_.write(angle);
  this->angle_ = angle;
}


#endif 
