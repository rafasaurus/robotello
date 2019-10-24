#ifndef __move__
#define __move__

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
        }

        void changeDirLeft() {
            digitalWrite(this->dir_pin_x_, 1);
            digitalWrite(this->dir_pin_y_, 1);
            digitalWrite(this->dir_pin_z_, 0);
            digitalWrite(this->dir_pin_a_, 0);
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
            digitalWrite(this->step_pin_x_, HIGH);
            digitalWrite(this->step_pin_y_, HIGH);
            delayMicroseconds(this->motorDelayTime_); 
            digitalWrite(this->step_pin_x_, LOW);
            digitalWrite(this->step_pin_y_, LOW);
            delayMicroseconds(this->motorDelayTime_); 
        }
        void one_step_anticlockwise() {
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
                one_step_clockwise();
                one_step_clockwise();
                one_step_clockwise();
            }
        }
        void changeMotorDelay(int del) {
            this->motorDelayTime_ = del;
        }
};

#endif
