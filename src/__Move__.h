#ifndef __move__
#define __move__

#ifndef RTOS
#define RTOS
#include <Arduino_FreeRTOS.h>
#include <event_groups.h>
#include <queue.h>
#endif

// direction pins
#define X_DIR     52 
#define Y_DIR     50
#define Z_DIR     48
#define A_DIR     46

// step pins
#define X_STP     53
#define Y_STP     51 
#define Z_STP     49 
#define A_STP     47
#define ONE_STEP 0
#define ONE_STEP_CLK_WISE 1
#define ONE_STEP_ANTI_CLK_WISE 2
#define NOTHING 3
 
class __Move__ {
    private:
        uint8_t step_pin_x_;
        uint8_t step_pin_y_;
        uint8_t step_pin_z_;
        uint8_t step_pin_a_;
        uint8_t dir_pin_x_;
        uint8_t dir_pin_y_;
        uint8_t dir_pin_z_;
        uint8_t dir_pin_a_;
        int motorDelayTime_;
        int state_;
    public:
        __Move__(int step_pin_x_, 
                int step_pin_y_, 
                int step_pin_z_, 
                int step_pin_a_,
                int dir_pin_x_,
                int dir_pin_y_,
                int dir_pin_z_,
                int dir_pin_a_,
                int motorDelayTime_) {
            this->step_pin_x_ = step_pin_x_;
            this->step_pin_y_ = step_pin_y_;
            this->step_pin_z_ = step_pin_z_;
            this->step_pin_a_ = step_pin_a_;
            this->dir_pin_x_ = dir_pin_x_;
            this->dir_pin_y_ = dir_pin_y_;
            this->dir_pin_z_ = dir_pin_z_;
            this->dir_pin_a_ = dir_pin_a_;
            this->motorDelayTime_ = motorDelayTime_;

            pinMode(this->dir_pin_x_, OUTPUT);
            pinMode(this->dir_pin_y_, OUTPUT);
            pinMode(this->dir_pin_z_, OUTPUT);
            pinMode(this->dir_pin_a_, OUTPUT);
            pinMode(this->step_pin_x_, OUTPUT);
            pinMode(this->step_pin_y_, OUTPUT);
            pinMode(this->step_pin_z_, OUTPUT);
            pinMode(this->step_pin_a_, OUTPUT);

        }
        void changeDirForward() {
            digitalWrite(this->dir_pin_x_, 1);
            digitalWrite(this->dir_pin_y_, 1);
            digitalWrite(this->dir_pin_z_, 1);
            digitalWrite(this->dir_pin_a_, 1);
        }

        void changeDirBackward() {
            digitalWrite(this->dir_pin_x_, 0);
            digitalWrite(this->dir_pin_y_, 0);
            digitalWrite(this->dir_pin_z_, 0);
            digitalWrite(this->dir_pin_a_, 0);
        }

        void changeDirRight() {
            digitalWrite(this->dir_pin_x_, 0);
            digitalWrite(this->dir_pin_y_, 0);
            digitalWrite(this->dir_pin_z_, 1);
            digitalWrite(this->dir_pin_a_, 1);
            delayMicroseconds(this->motorDelayTime_);

        }

        void changeDirLeft() {
            digitalWrite(this->dir_pin_x_, 1);
            digitalWrite(this->dir_pin_y_, 1);
            digitalWrite(this->dir_pin_z_, 0);
            digitalWrite(this->dir_pin_a_, 0);
            delayMicroseconds(this->motorDelayTime_);
        }

        void goLeft() {
            digitalWrite(this->dir_pin_x_, 0);
            digitalWrite(this->dir_pin_y_, 1);
            digitalWrite(this->dir_pin_z_, 1);
            digitalWrite(this->dir_pin_a_, 0);
        }
        void one_step() {
            digitalWrite(this->step_pin_x_, HIGH);
            digitalWrite(this->step_pin_y_, HIGH);
            digitalWrite(this->step_pin_z_, HIGH);
            digitalWrite(this->step_pin_a_, HIGH);
            delayMicroseconds(this->motorDelayTime_);
            digitalWrite(this->step_pin_x_, LOW);
            digitalWrite(this->step_pin_y_, LOW);
            digitalWrite(this->step_pin_z_, LOW);
            digitalWrite(this->step_pin_a_, LOW);
            delayMicroseconds(this->motorDelayTime_);
        }
        // TODO 
        // with parameter delayDifference 
        // smooth start, smooth stop
        void one_step_clockwise() {
            // this->changeDirForward();
            digitalWrite(this->step_pin_x_, HIGH);
            digitalWrite(this->step_pin_y_, HIGH);
            delayMicroseconds(this->motorDelayTime_); 
            digitalWrite(this->step_pin_x_, LOW);
            digitalWrite(this->step_pin_y_, LOW);
            delayMicroseconds(this->motorDelayTime_); 
        }
        void one_step_anticlockwise() {
            // this->changeDirForward();
            digitalWrite(this->step_pin_z_, HIGH);
            digitalWrite(this->step_pin_a_, HIGH);
            delayMicroseconds(this->motorDelayTime_); 
            digitalWrite(this->step_pin_z_, LOW);
            digitalWrite(this->step_pin_a_, LOW);
            delayMicroseconds(this->motorDelayTime_); 
        }
        void n_step(int steps) {
            for (int step = 0; step <= steps; ++step) {
                one_step();
            }
        }
        void changeMotorDelay(int del) {
            this->motorDelayTime_ = del;
        }
        void changeState(int state) {
            this->state_ = state;
        }
        void update() {
            switch (this->state_) {
                case ONE_STEP: // one_step
                    one_step();
                    break;
                case ONE_STEP_CLK_WISE: // one_step_clockwise
                    one_step_clockwise();
                    break;
                case ONE_STEP_ANTI_CLK_WISE:
                    one_step_anticlockwise();
                    break;
                case NOTHING:
                    vTaskDelay(1/portTICK_PERIOD_MS);
                    break;
            }
        }
        void startTask(){
            xTaskCreate(this->startTaskImpl, "Task", 2048, this, 3, NULL);
        }
    private:
        void task(){
            while(1){
                this->update();
            }
        }
        static void startTaskImpl(void* _this){
            static_cast<__Move__*>(_this)->task();
        }
};

#endif
