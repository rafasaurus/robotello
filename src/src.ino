#include <Arduino.h>
#include "__Servo__.h"
#include "__Move__.h"
#include "__Ultrasonic__.h"

#define rightSensorPin A0 // RightSesnor
#define leftSensorPin A1 // LeftSesnor

__Servo__ arm(0, 60, 15, 5);
__Servo__ armTwo(0, 168, 30, 3);
__Servo__ armOne(0, 180, 30,4);
__Servo__ armRotate(2);
__Move__ motor(X_STP,
        Y_STP,
        Z_STP,
        A_STP,
        X_DIR,
        Y_DIR, Z_DIR,
        A_DIR,
        100);
__Ultrasonic__ ultrasonic;
void setup() {
    Serial.begin(115200);
    Serial.print("setup complete");
    motor.changeDirForward();
}

void loop() {
    if (ultrasonic.getDistance() > 250) {
        motor.one_step();
    }
}
