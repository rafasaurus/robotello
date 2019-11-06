#include <Arduino.h>
#include "__Servo__.h"
#include "__Move__.h"
#include "__Ultrasonic__.h"
#include "__Serial__.h"
#include "__LaneCounter__.h"
#ifndef RTOS
#define RTOS
#include <Arduino_FreeRTOS.h>
#include <event_groups.h>
#include <queue.h>
#endif
// Define States of State-Machine
#include <string.h>
#include <stdio.h>

#define CONFIG_DEBUG -1
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
    Serial3.begin(115200);
}

int lineTrackerRight = 0;
int lineTrackerRightRight = 0;
int lineTrackerLeft = 0;
int lineTrackerLeftLeft = 0;
int colorSenseRed = 0;
int colorSenseGreen = 0;
int colorSenseBlue = 0;

void
Loop(void *pvParameters)
{
    (void) pvParameters;
    motor.changeDirForward();
    int state = TRACK_LINE;
    // Filter parameters
    while(1) {
        // State-Machine
        switch (state) {
#ifdef CONFIG_DEBUG
            case CONFIG_DEBUG:
                Serial.print("************* DEBUG *****************");
                while (Serial3.available () > 0)
                    serial.processIncomingByte(Serial3.read());
                updateSensors();
                debugLineColors();
                break;
#endif
            case TRACK_LINE:
#ifdef CONFIG_DEBUG
                Serial.print("************* TRACK_LINE *****************");
                debugLineColors();
#endif
                while (Serial3.available () > 0)
                    serial.processIncomingByte(Serial3.read());
                updateSensors();
                motor.changeDirForward();
                trackLine();
                vTaskDelay(50/portTICK_PERIOD_MS);
                if(lineTrackerRightRight <= 100 &&
                        lineTrackerRightRight >= 5 &&
                        inRange(lineTrackerRight, 5, 250) &&
                        inRange(colorSenseRed, 5, 50) &&
                        inRange(colorSenseGreen, 5, 50) &&
                        inRange(colorSenseBlue, 5, 50)) {
                    state = TURN_RIGHT1;
                    motor.changeState(NOTHING);
                    // For debugging
                    vTaskDelay(1000/portTICK_PERIOD_MS);;
                    break;
                }
                break;
            case ROTATE_RIGHT_90:
#ifdef CONFIG_DEBUG
                Serial.print("************* ROTATE_RIGHT_90 *****************");
#endif
                turnRight90();

                break;
            case ROTATE_LEFT_90:
#ifdef CONFIG_DEBUG
                Serial.print("************* ROTATE_LEFT_90 *****************");
#endif
                turnLeft90();
                break;
            case ERROR:
#ifdef CONFIG_DEBUG
                Serial.print("************* ERROR *****************");
#endif
                break;
            case TRASH_CAN:
#ifdef CONFIG_DEBUG
                Serial.print("************* TRASH_CAN *****************");
#endif
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
#ifdef CONFIG_DEBUG
                Serial.print("************* TRACK_LINE_1 *****************");
#endif
                motor.changeDirForward();
                trackLine();
                vTaskDelay(150/portTICK_PERIOD_MS);
                break;

            case PARKING_FIRST:
#ifdef CONFIG_DEBUG
                Serial.print("************* PARKING_FIRST *****************");
#endif
                rightParking();
                state = ERROR;
                break;

            case TURN_RIGHT1:
#ifdef CONFIG_DEBUG
                Serial.print("************* TURN_RIGHT1 *****************");
#endif
                nStepForward(4000);
                vTaskDelay(10/portTICK_PERIOD_MS);
                turnRight90();
                nStepForward(2000);
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
    Serial.println(colorSenseBlue);
}

void
rightParking(){
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

inline void
updateSensors() {
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
void
loop() {
    // All code is handled by RTOS tasks
}
