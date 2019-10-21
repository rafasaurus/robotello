// Direction pin 
#define X_DIR     52 
#define Y_DIR     50
#define Z_DIR     48
#define A_DIR     46

// Step pin
#define X_STP     53
#define Y_STP     51 
#define Z_STP     49 
#define A_STP     47


// DRV8825
int delayTime=120; //Delay between each pause (uS)
int stps=6400;// Steps to move
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
        
        void n_step(int steps) {
           for (int step = 0; step <= steps; ++step) {
             one_step();
           }
        }
};

__Move__ *motor;

void setup(){
    motor = new __Move__(X_STP,
                        Y_STP,
                        Z_STP,
                        Z_STP,
                        X_DIR,
                        Y_DIR, 
                        Z_DIR,
                        A_DIR,
                        120);
}

void loop(){
    motor->changeDirForward();
    motor->n_step(6000);
    motor->changeDirBackward();
    motor->n_step(6000);
}
