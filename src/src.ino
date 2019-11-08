#include <Arduino.h>
#include "__Servo__.h"
#include "__Move__.h"
#include "__Ultrasonic__.h"
#include "__Serial__.h"
#ifndef RTOS
#define RTOS
#include <Arduino_FreeRTOS.h>
#include <event_groups.h>
#include <queue.h>
#endif
// Define States of State-Machine
#include <string.h>
#include <stdio.h>

#define DEBUG -2
#define CONFIG_DEBUG -1
#define ERROR 0
#define TRACK_LINE 1
#define TRACK_LINE_1 5
#define TRASH_CAN 4
#define PARKING_FIRST 7
#define TURN_RIGHT1 8

// maps
#define MAP1 100
#define SUBMAP1 1000
#define SUBMAP2 1001
#define SUBMAP3 1002

#define LED_BUILTIN 13
#define rightSensorPin A1 // RightSesnor
#define leftSensorPin A0 // LeftSesnor
#define INCLUDE_vTaskDelay 1
#define STEPS_FOR_90_DEGREE 6000
#define MOTOR_SPD 130

#define FIRE_PIN 6
#define BUZZER_PIN 9
bool fireSafety = true;
// Define lineTracker sensor margins
#define RR_MIN 10
#define RR_MAX 300

#define R_MIN 10
#define R_MAX 500

#define L_MIN 10
#define L_MAX 500

#define LL_MIN 10
#define LL_MAX 300

// Define colorSensor margins
#define CR_MIN 5
#define CR_MAX 50

#define CG_MIN 5
#define CG_MAX 50

#define CB_MIN 5
#define CB_MAX 50

// Define colorSensor1 margins
#define CR1_MIN 5
#define CR1_MAX 50

#define CG1_MIN 5
#define CG1_MAX 50

#define CB1_MIN 5
#define CB1_MAX 50

float alpha = 0.5;
void Loop(void *pvParameters);
__Servo__ *arm;
__Servo__ *armTwo;
__Servo__ *armOne;
__Servo__ *armRotate;
__Serial__ serial;

Servo ESC;
__Ultrasonic__ ultrasonic;
__Move__ motor(X_STP,
        Y_STP,
        Z_STP,
        A_STP,
        X_DIR,
        Y_DIR, Z_DIR,
        A_DIR,
        MOTOR_SPD);

void
setup() {
    // Define servos -> "min, max, delay, pin" parameters
    arm = new __Servo__(0, 60, 15, 5, 20);
    armOne = new __Servo__(0, 180, 30,4, 180);
    armTwo = new __Servo__(0, 168, 30, 3, 30);
    armRotate = new __Servo__(2, 132);
    xTaskCreate(
            Loop
            ,  (const portCHAR *)"Blink"   // A name just for humans
            ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
            ,  NULL
            ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
            ,  NULL );
    motor.changeDirForward();
    motor.startTask();
    Serial.begin(115200);
    Serial3.begin(115200);
    ESC.attach(FIRE_PIN); 
    ESC.writeMicroseconds(1000);
    pinMode(BUZZER_PIN, OUTPUT);
}

int lineTrackerRight = 0;
int lineTrackerRightRight = 0;
int lineTrackerLeft = 0;
int lineTrackerLeftLeft = 0;
int colorSenseRed = 0;
int colorSenseGreen = 0;
int colorSenseBlue = 0;
int colorSense1_mean = 0;
bool listen = 1;
    void
Loop(void *pvParameters)
{
    (void) pvParameters;
    motor.changeDirForward();
    ESC.writeMicroseconds(1000);

    int map_state = MAP1;
    while(1) {
        updateSerial();
        updateSensors();
        vTaskDelay(50/portTICK_PERIOD_MS);
        switch (map_state) {
            case MAP1:
                Serial.print("************* MAP1 *****************");
                int lastLaneCnt = serial.getLaneCnt();
                //  parameters for SUBMAP1
                int subMap_state = SUBMAP1;
                bool trashCanDo = true;
                bool submap23_switcher = false; // if true submap3; else submap2
                int turnRightCnt = 0;
                // -----------------------
                // parameters for SUBMAP2
                int turnLeftCnt = 0;
                bool parkingCanDo = true;
                bool leftTurnPass = true;
                // -----------------------
                buzzerPlay(1, 50);
                while(1) {
                    updateSerial();
                    updateSensors();
                    vTaskDelay(50/portTICK_PERIOD_MS);
                    debugLineColors();
                    switch (subMap_state) {
                        case SUBMAP1:
                            /* Serial.print("************* SUBMAP1 *****************"); */
                            // check 2x laneCnt -> trashCan
                            // 2x turnRight
                            // trackLine() until some condition
                            motor.changeDirForward();
                            trackLine();
                            // ** if you want to disable trashCan()
                            // trashCanDo = false;
                            if (serial.getLaneCnt() - lastLaneCnt > 1 && trashCanDo) {
                                trashCanDo = false; 
                                trashCan();
                            }
                            // TODO
                            if (!trashCanDo) {
                                if (checkForTurnRight() == true) {
                                    motor.changeState(NOTHING);
                                    vTaskDelay(500/portTICK_PERIOD_MS);
                                    turnRightCnt++;
                                    nStepForward(3000);
                                    turnRight90();
                                    nStepForward(2000);
                                }
                            }
#ifdef SUBMAP3_
                            if (checkForTurnLeft() == true && turnRightCnt == 2) {
                                motor.changeState(NOTHING);
                                vTaskDelay(500/portTICK_PERIOD_MS);
                                nStepForward(3000);
                                turnLeft90(); 
                                nStepForward(2000);
                                subMap_state = SUBMAP3;
                                break;
                            }
#else
                            if (checkForTurnLeft() == true && turnRightCnt == 2) {
                                motor.changeState(NOTHING);
                                vTaskDelay(1000/portTICK_PERIOD_MS);
                                motor.changeDirRight();
                                nStepRight(700);
                                nStepForward(1000);
                                subMap_state = SUBMAP2;
                            }
                            break;
#endif
                            break;

                        case SUBMAP2:
                            // 2x turnLeft check
                            // after seccond turn parking
                            trackLine();
                            if (checkForTurnLeft() == true) {
                                if (turnLeftCnt == 1) {
                                    motor.changeState(NOTHING);
                                    vTaskDelay(500/portTICK_PERIOD_MS);

                                    turnLeftCnt++;
                                    nStepRight(700);
                                    nStepForward(700);
                                } else {
                                    motor.changeState(NOTHING);
                                    vTaskDelay(500/portTICK_PERIOD_MS);

                                    turnLeftCnt++;
                                    nStepForward(3000);
                                    turnLeft90();
                                    nStepForward(2000);
                                }
                                lastLaneCnt = serial.getLaneCnt();
                                buzzerPlay(turnLeftCnt, 50);
                            }
                            // TODO **
                            if (serial.getLaneCnt() - lastLaneCnt >= 3) {
                                lastLaneCnt = serial.getLaneCnt();
                            }
                            if (turnLeftCnt >= 3 && serial.getLaneCnt() - lastLaneCnt >= 1) {
                                Serial.println("------------");
                                motor.changeState(ONE_STEP);
                                buzzerPlay(serial.getLaneCnt() - lastLaneCnt, 50);
                                vTaskDelay(500/portTICK_PERIOD_MS);
                                /* nStepForward(5500);  */
                                rightParking(); 
                                motor.changeState(NOTHING);
                            }
                            break;
                        case SUBMAP3:
                            if (checkForTurnLeft() == true) {
                                buzzerPlay(10,20);
                                nStepForward(3000);
                                turnLeft90(); 
                                nStepForward(2000);
                            }
                            trackLine();
                            break;
                    }
                }
        }
    }
}

inline void
trashCan() {
    Serial.print("************* TRASH_CAN *****************");
    nStepForward(5000);
    motor.changeState(NOTHING);

    // open all the arms
    armOne->open();
    armTwo->close();//reversed servo
    arm->open();

    turnRight90();
    motor.changeState(ONE_STEP);
    nStepForward(4000);
    motor.changeState(NOTHING);

    // get the trash can and lift
    arm->close();
    armOne->liftTrashCan(50);
    armTwo->liftTrashCan(120);

    // put trash can back
    armOne->open();
    armTwo->close();
    arm->open();

    // get back
    motor.changeState(ONE_STEP);
    nStepBackward(4300);
    motor.changeState(NOTHING);

    // close the arms
    armOne->close();
    armTwo->open();

    // get back again
    //                motor.changeState(ONE_STEP);
    //                nStepBackward(300);
    //                motor.changeState(NOTHING);
    turnLeft90();
}
inline
void
nStepForward(int steps) {
    // TODO
    // it's just for temporrary bug fix
    updateSerial();
    motor.changeDirForward();
    motor.changeState(ONE_STEP); 
    while(!motor.n_step(steps)) {
        vTaskDelay(1 / portTICK_PERIOD_MS); // wait for one second
    }
}
inline
void
nStepRight(int steps) {
    // TODO
    // it's just for temporrary bug fix
    updateSerial();
    motor.changeDirRight();
    motor.changeState(ONE_STEP); 
    while(!motor.n_step(steps)) {
        vTaskDelay(1 / portTICK_PERIOD_MS); // wait for one second
    }
}

inline
void
nStepBackward(int steps) {
    motor.changeDirBackward();
    motor.changeState(ONE_STEP); 
    while(!motor.n_step(steps)) {
        vTaskDelay(1 / portTICK_PERIOD_MS); // wait for one second
    }
}

inline
void
turnRight90() {
    motor.changeDirRight();
    motor.changeState(ONE_STEP); 
    while(!motor.n_step(STEPS_FOR_90_DEGREE)) {
        vTaskDelay(1 / portTICK_PERIOD_MS); // wait for one second
    }
}

void
turnLeft90() {
    motor.changeDirLeft();
    motor.changeState(ONE_STEP); 
    while(!motor.n_step(STEPS_FOR_90_DEGREE)) {
        vTaskDelay(1 / portTICK_PERIOD_MS); // wait for one second
    }
    motor.changeState(NOTHING);
}

void
trackLine(){
    // if two sensors are black
    if(lineTrackerLeft >= 300 &&
            lineTrackerLeft <= 1060 &&
            lineTrackerRight  <= 1060 &&
            lineTrackerRight >= 300 ) {

        motor.changeState(ONE_STEP);
    }
    //     if(lineTrackerLeft.getValue() >= 0 &&
    //            lineTrackerLeft.getValue() <= 250 &&
    //            lineTrackerRight.getValue()  <= 250 &&
    //            lineTrackerRight.getValue() >= 0 ) {
    //        
    //        motor.changeState(ONE_STEP);
    //        Serial.print(1);
    //    }
    // if left sensor is white , right sensor is black
    if(lineTrackerLeft >= 0 &&
            lineTrackerLeft <= 250 &&
            lineTrackerRight <= 1060 &&
            lineTrackerRight >= 300 ) {
        motor.changeState(ONE_STEP_ANTI_CLK_WISE);
    }
    // if left sensor is black , right sensor is white
    if(lineTrackerLeft >= 300 &&
            lineTrackerLeft <= 1060 &&
            lineTrackerRight <= 250 &&
            lineTrackerRight >= 0) {
        motor.changeState(ONE_STEP_CLK_WISE);
    }
}

inline void
debugLineColors() {
    Serial.print(" ");
    Serial.print(lineTrackerRightRight);
    Serial.print(" ");
    Serial.print(lineTrackerRight);
    Serial.print(" ");
    Serial.print(lineTrackerLeft);
    Serial.print(" ");
    Serial.print(lineTrackerLeftLeft);

    Serial.print(" ");
    Serial.print(colorSenseRed);
    Serial.print(" ");
    Serial.print(colorSenseGreen);
    Serial.print(" ");
    Serial.print(colorSenseBlue);

    Serial.print(" ");
    Serial.print(colorSense1_mean);
    Serial.println(" ");
}

inline void
rightParking(){
    motor.changeState(ONE_STEP);
    nStepForward(3500);
    motor.changeState(NOTHING);
    motor.changeDirLeft();
    motor.changeState(ONE_STEP); 
    while(!motor.n_step(STEPS_FOR_90_DEGREE)) {
        vTaskDelay(1 / portTICK_PERIOD_MS); // wait for one second
    }
    motor.changeState(ONE_STEP);
    nStepBackward(9000);
    motor.changeState(NOTHING);
    vTaskDelay(5000);
    nStepForward(9000);
    turnLeft90();
}

inline bool
inRange (int value, int min, int max) {
    if (value >= min && value <= max)
        return true;
    else 
        return false;
}

inline void
updateSensors() {
    // get the payload from filters/__LaneCounter__.h
    if (same(serial.get_R_sensor(),
                serial.get_RR_sensor(),
                serial.get_L_sensor(),
                serial.get_LL_sensor(),
                serial.colorSenseGetRedColor(),
                serial.colorSenseGetGreenColor(),
                serial.colorSenseGetBlueColor())) {
        lineTrackerRight = serial.get_R_sensor();
        lineTrackerRightRight = serial.get_RR_sensor();;
        lineTrackerLeft = serial.get_L_sensor();
        lineTrackerLeftLeft = serial.get_LL_sensor();

        colorSenseRed = serial.colorSenseGetRedColor();
        colorSenseGreen = serial.colorSenseGetGreenColor();
        colorSenseBlue = serial.colorSenseGetBlueColor();

        colorSense1_mean = serial.colorSense1GetMean();
    }
}

inline bool
same(int a, int b, int c, int d, int e, int f, int g) {
    int counter = 0;
    if (a == b)
        ++counter;
    if (a == f)
        ++counter;
    if (a == g)
        ++counter;
    if (b == c)
        ++counter;
    if (b == d)
        ++counter;
    if (b == e)
        ++counter;
    if (c == d)
        ++counter;
    if (d == e)
        ++counter;
    if (e == a)
        ++counter;
    if (e == c)
        ++counter;
    if (d == a)
        ++counter;
    if (c == e)
        ++counter;
    if (c == g)
        ++counter;
    if (c == f)
        ++counter;
    if (d == f)
        ++counter;
    if (d == g)
        ++counter;
    if (f == g)
        ++counter;
    if (e == g)
        ++counter;
    if (e == f)
        ++counter;
    if (counter >=3) {
        return false;
    }
    return true;
}

inline void
updateSerial() {
    while (Serial3.available () > 0)
        serial.processIncomingByte(Serial3.read());
}
inline void
estinguishFire() { // init
    fireSafety = false;
    ESC.writeMicroseconds(1000);
    vTaskDelay(2000/portTICK_PERIOD_MS);
    ESC.writeMicroseconds(1300);
    vTaskDelay(2000/portTICK_PERIOD_MS);
    ESC.writeMicroseconds(1000);
    fireSafety = true;
}
inline void
buzzerPlay(int n, int delay) {
    for (int i = 0; i < n; i++) {
        digitalWrite(BUZZER_PIN, 1);
        vTaskDelay(delay/portTICK_PERIOD_MS);
        digitalWrite(BUZZER_PIN, 0);
        vTaskDelay(delay/portTICK_PERIOD_MS);
    }
}
inline bool
checkForTurnRight() {
    if (inRange(lineTrackerRightRight, RR_MIN, RR_MAX) &&
            inRange(lineTrackerRight, R_MIN, R_MAX) &&
            inRange(lineTrackerLeft, L_MIN, L_MAX) &&
            inRange(colorSenseRed, CR_MIN, CR_MAX) &&
            inRange(colorSenseGreen, CG_MIN, CG_MAX) &&
            inRange(colorSenseBlue, CB_MIN, CB_MAX))
        return true;
    else
        return false;
}
inline bool
checkForTurnRightCrossRoad() {
    if (inRange(lineTrackerLeftLeft, LL_MIN, LL_MAX) &&
            inRange(lineTrackerLeft, L_MIN, L_MAX) &&
            inRange(lineTrackerRight, R_MIN, R_MAX) &&
            inRange(lineTrackerRightRight, RR_MIN, RR_MAX) &&
            inRange(colorSenseRed, CR_MIN, CR_MAX) &&
            inRange(colorSenseGreen, CG_MIN, CG_MAX) &&
            inRange(colorSenseBlue, CB_MIN, CB_MAX))
        return true;
    else
        return false;
}
inline bool
checkForTurnLeft() {
    if (inRange(lineTrackerLeftLeft, LL_MIN, LL_MAX) &&
            inRange(lineTrackerLeft, L_MIN, L_MAX) &&
            inRange(lineTrackerRight, R_MIN, R_MAX) &&
            inRange(colorSenseRed, CR_MIN, CR_MAX) &&
            inRange(colorSenseGreen, CG_MIN, CG_MAX) &&
            inRange(colorSenseBlue, CB_MIN, CB_MAX))
        return true;
    else
        return false;
}
void
loop() {
    // All code is handled by RTOS tasks
}
