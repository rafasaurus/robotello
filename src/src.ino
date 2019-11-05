/* #define Serial Serial1 */
#include <Arduino.h>
#include "__Servo__.h"
#include "__Move__.h"
#include "__Ultrasonic__.h"
#include "__ColorSense__.h"
#include "__LineTracker__.h"
#include "__Serial__.h"
#include "__LaneCounter__.h"
#ifndef RTOS
#define RTOS
#include <Arduino_FreeRTOS.h>
#include <event_groups.h>
#include <queue.h>
#endif
// Define States of State-Machine
#define ERROR 0
#define TRACK_LINE 1
#define TRACK_LINE_1 5
#define ROTATE_RIGHT_90 2
#define ROTATE_LEFT_90 3
#define TRASH_CAN 4
#define PARKING_FIRST 7
#define TURN_RIGHT1 8

#define LED_BUILTIN 13
#define rightSensorPin A1 // RightSesnor
#define leftSensorPin A0 // LeftSesnor
#define INCLUDE_vTaskDelay 1
#define STEPS_FOR_90_DEGREE 6400

float alpha = 0.5;
void Loop(void *pvParameters);
__Servo__ *arm;
__Servo__ *armTwo;
__Servo__ *armOne;
__Servo__ *armRotate;
__Serial__ serial;

// lineTracker example "whiteLineThreshold, blackLineThreshold, blackLineOffset, whiteLineOffset, pin"
__LineTracker__ lineTrackerLeft(10, 10, 100, 200, 1);
__LineTracker__ lineTrackerRight(10, 10, 100, 200, 0);
__LineTracker__ lineTrackerRightRight(10, 10, 100, 200, 2);

// Pins are defined in __ColorSense__.h
__ColorSense__ colorSense(S0, S1, S2, S3, COLORSENSE_OUT);
__ColorSense__ colorSense1(S00, S11, S22, S33, COLORSENSE_OUT1);

__Ultrasonic__ ultrasonic;
__Move__ motor(X_STP,
        Y_STP,
        Z_STP,
        A_STP,
        X_DIR,
        Y_DIR, Z_DIR,
        A_DIR,
        150);
__LaneCounter__ laneCounter;

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
}

    void
Loop(void *pvParameters)
{
    (void) pvParameters;
    motor.changeDirForward();
    int state = TRACK_LINE;
    // Filter parameters
    int previous_colorSense1_Red = 0;
    int previous_colorSense1_Green = 0;
    int previous_colorSense1_Blue = 0;

    while(1) {
        /* if (serial.getMessage() == "asdf") { */
        /*     state = ERROR; */
        /* }; */
        // State-Machine
        Serial.print("sensor:");
        Serial.println(lineTrackerRightRight.getSensor());
        Serial.print("state:");
        Serial.println(state);
        switch (state) {
            case TRACK_LINE:
                trackLine();
                vTaskDelay(50/portTICK_PERIOD_MS);
                /* debugColorSense(); */

                // Basic Filtering
                /* float current_colorSense1_Red = alpha * colorSense1.getRedColor() + (1-alpha) * previous_colorSense1_Red; */
                /* float current_colorSense1_Green = alpha * colorSense1.getGreenColor() + (1-alpha) * previous_colorSense1_Green; */
                /* float current_colorSense1_Blue = alpha * colorSense1.getBlueColor() + (1-alpha) * previous_colorSense1_Blue; */
                /*  */
                /* previous_colorSense1_Red = current_colorSense1_Red; */
                /* previous_colorSense1_Green = current_colorSense1_Green; */
                /* previous_colorSense1_Blue = current_colorSense1_Blue; */
                /*  */
                /* Serial.print("R="); */
                /* Serial.print(current_colorSense1_Red); */
                /* Serial.print(" "); */
                /* Serial.print(current_colorSense1_Green);  */
                /* Serial.print(" "); */
                /* Serial.println(current_colorSense1_Blue); */
                // -------------------------------
                /* if (inRange(colorSense1.getRedColor(), 100, 150) && */
                /*         inRange(colorSense1.getGreenColor(), 100, 150) && */
                /*         inRange(colorSense1.getBlueColor(),80, 120)) { */
                /*     state = TRASH_CAN; */
                /*     break; */
                /* } */
                // Test the colorSensor
                /* if (inRange(colorSense.getRedColor(),20,60) && */
                /*         inRange(colorSense.getGreenColor(), 10, 50) && */
                /*         inRange(colorSense.getBlueColor(), 10, 40)) { */
                /*     //                    state = TRASH_CAN; */
                /*     Serial.print("is white"); */
                /* } */
                /* else{ */
                /*     Serial.print("is Black"); */
                /* } */

                // if left sensor is black , right sensor is white
                if(lineTrackerRightRight.getSensor() <= 100 &&
                        lineTrackerRightRight.getSensor() >= 0 &&
                        inRange(colorSense.getRedColor(),20,60) &&
                        inRange(colorSense.getGreenColor(), 10, 50) &&
                        inRange(colorSense.getBlueColor(), 10, 40)) {
                    state = TURN_RIGHT1;
                    break;
                }
                break;
            case ROTATE_RIGHT_90:
                turnRight90();
                break;
            case ROTATE_LEFT_90:
                turnLeft90();
                break;
            case ERROR:
                Serial.println("trash_can");
                break;
            case TRASH_CAN:
                nStepForward(2000);
                motor.changeState(NOTHING);

                // open all the arms
                armOne->open();
                armTwo->close();//reversed servo
                arm->open();

                turnRight90();
                motor.changeState(ONE_STEP);
                nStepForward(2000);
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
                nStepBackward(2300);
                motor.changeState(NOTHING);

                // close the arms
                armOne->close();
                armTwo->open();

                // get back again
                //                motor.changeState(ONE_STEP);
                //                nStepBackward(300);
                //                motor.changeState(NOTHING); 
                turnLeft90();

                state = TRACK_LINE_1;
                break;
            case TRACK_LINE_1:
                motor.changeDirForward();
                trackLine();
                vTaskDelay(150/portTICK_PERIOD_MS);

            case PARKING_FIRST:
                rightParking();
                state = ERROR;
                break;


            case TURN_RIGHT1:
                nStepForward(4500);
                vTaskDelay(10/portTICK_PERIOD_MS);
                turnRight90();
                motor.changeState(NOTHING);
                state = TRACK_LINE;
                break;
        }
    }
}

inline
void
nStepForward(int steps) {
    motor.changeDirForward();
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
    if(lineTrackerLeft.getValue() >= 300 &&
            lineTrackerLeft.getValue() <= 1060 &&
            lineTrackerRight.getValue()  <= 1060 &&
            lineTrackerRight.getValue() >= 300 ) {

        motor.changeState(ONE_STEP);
    }
    // if left sensor is white , right sensor is black
    if(lineTrackerLeft.getValue() >= 0 &&
            lineTrackerLeft.getValue() <= 250 &&
            lineTrackerRight.getValue() <= 1060 &&
            lineTrackerRight.getValue() >= 300 ) {
        motor.changeState(ONE_STEP_ANTI_CLK_WISE);
    }
    // if left sensor is black , right sensor is white
    if(lineTrackerLeft.getValue() >= 300 &&
            lineTrackerLeft.getValue() <= 1060 &&
            lineTrackerRight.getValue() <= 250 &&
            lineTrackerRight.getValue() >= 0) {
        motor.changeState(ONE_STEP_CLK_WISE);
    }
}

inline void
debugColorSense() {
    Serial.print("R=");
    Serial.print(colorSense1.getRedColor());
    Serial.print(" G=");
    Serial.print(colorSense1.getGreenColor()); 
    Serial.print(" B=");
    Serial.println(colorSense1.getBlueColor());
}

void rightParking(){
    motor.changeState(ONE_STEP);
    nStepForward(2500);
    motor.changeState(NOTHING);
    motor.changeDirLeft();
    motor.changeState(ONE_STEP); 
    while(!motor.n_step(STEPS_FOR_90_DEGREE)) {
        vTaskDelay(1 / portTICK_PERIOD_MS); // wait for one second
    }
    motor.changeState(ONE_STEP);
    nStepBackward(9000);
    motor.changeState(NOTHING);
}

inline bool
inRange (int value, int min, int max) {
    if (value >= min && value <= max)
        return true;
    else 
        return false;
}
/* inline int */
/* filter (in */
void
loop() {
    // All code is handled by RTOS tasks
}
