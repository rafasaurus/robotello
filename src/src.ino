#include "__servo__.h"
#include "__move__.h"

__Servo__ *arm;
__Servo__ *armOne;
__Servo__ *armTwo;
__Servo__ *armRotate;
__Move__ *motor;

void setup() {
    arm = new __Servo__(0, 60, 15, 5);
    armTwo = new __Servo__(0, 168, 30, 3);
    armOne = new __Servo__(0, 180, 30,4);
    armRotate = new __Servo__(2);
    motor = new __Move__(X_STP,
                        Y_STP,
                        Z_STP,
                        A_STP,
                        X_DIR,
                        Y_DIR, 
                        Z_DIR,
                        A_DIR,
                        100);
}

void loop() {
    armRotate->setAngle(130);

    motor->changeDirForward();
    motor->n_step_smooth(6000);
    motor->changeDirRight();
    motor->n_step_smooth(6000);

    delay(1000);
}
