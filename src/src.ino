#include <Arduino.h>
#include "__Servo__.h"
#include "__Move__.h"
#include "__Ultrasonic__.h"
#include "__ColorSense__.h"
#include "__LineTracker__.h"

#ifndef RTOS
#define RTOS
#include <Arduino_FreeRTOS.h>
#include <event_groups.h>
#include <queue.h>
#endif
#define LED_BUILTIN 13

#define rightSensorPin A1 // RightSesnor
#define leftSensorPin A0 // LeftSesnor
#define INCLUDE_vTaskDelay 1
/* TaskHandle_t motor_task_handler; */
void Loop(void *pvParameters);

__ColorSense__ colorSense;
// define servos -> min, max, delay, pin
__Servo__ *arm;
__Servo__ *armTwo;
__Servo__ *armOne;
__Servo__ *armRotate;

__Move__ motor(X_STP,
        Y_STP,
        Z_STP,
        A_STP,
        X_DIR,
        Y_DIR, Z_DIR,
        A_DIR,
        120);
__Ultrasonic__ ultrasonic;
// lineTracker example "whiteLineThreshold, blackLineThreshold, blackLineOffset, whiteLineOffset, pin"
__LineTracker__ lineTrackerLeft(10, 10, 100, 200, 1);
__LineTracker__ lineTrackerRight(10, 10, 100, 200, 0);

void setup() {
    arm = new __Servo__(0, 60, 15, 5, 20);
    armOne = new __Servo__(0, 180, 30,4, 180);
    armTwo = new __Servo__(0, 168, 30, 3, 30);
    armRotate = new __Servo__(2, 132);

    Serial.begin(115200);
    Serial.print("setup complete");

    xTaskCreate(
            Loop
            ,  (const portCHAR *)"Blink"   // A name just for humans
            ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
            ,  NULL
            ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
            ,  NULL );
    /* vTaskStartScheduler(); */
    motor.changeDirForward();
    motor.startTask();
}

void Loop(void *pvParameters)
{
    (void) pvParameters;
    motor.changeDirForward();
    while(1)
    {
        vTaskDelay(600 / portTICK_PERIOD_MS); // wait for one second
        /* Serial.print("R="); */
        /* Serial.print(colorSense.getRedColor()); */
        /* Serial.print(" G="); */
        /* Serial.print(colorSense.getGreenColor()); */ /* Serial.print(" B="); */
        /* Serial.print(colorSense.getBlueColor()); */
        /* Serial.print("    Left ="); */
        /* Serial.print(lineTrackerLeft.getValue()); */
        /* Serial.print("    Righr ="); */
        /* Serial.println(lineTrackerRight.getValue()); */

        // if two sensors are black
        if(lineTrackerLeft.getValue() >= 300 &&
                lineTrackerLeft.getValue() <= 1060 &&
                lineTrackerRight.getValue()  <= 1060 &&
                lineTrackerRight.getValue() >= 300 ){
            motor.changeState(ONE_STEP);
            Serial.print(1);
        }
        // if left sensor is white , right sensor is black
        if(lineTrackerLeft.getValue() >= 0 &&
                lineTrackerLeft.getValue() <= 250 &&
                lineTrackerRight.getValue() <= 1060 &&
                lineTrackerRight.getValue() >= 300 ){
            motor.changeState(ONE_STEP_ANTI_CLK_WISE);
            Serial.print(2);
        }
        // if left sensor is black , right sensor is white
        if(lineTrackerLeft.getValue() >= 300 &&
                lineTrackerLeft.getValue() <= 1060 &&
                lineTrackerRight.getValue() <= 250 &&
                lineTrackerRight.getValue() >= 0){               
            motor.changeState(ONE_STEP_CLK_WISE);
            Serial.print(3);
        }
    }
}

void trackLine(){
    if(lineTrackerLeft.getValue() >= 750 &&
            lineTrackerLeft.getValue() <= 1060 &&
            lineTrackerRight.getValue()  <= 1060 &&
            lineTrackerRight.getValue() >= 750 ){
        motor.changeState(ONE_STEP);
        Serial.println("GoToForward");
    }
    if(lineTrackerLeft.getValue() >= 20 &&
            lineTrackerLeft.getValue() <= 200 &&
            lineTrackerRight.getValue() <= 1060 &&
            lineTrackerRight.getValue() >= 750 ){
        for(int i= 0; i <= 100; i++){
            motor.changeState(ONE_STEP_ANTI_CLK_WISE);
            Serial.println("TurnRight_AntiClcWise");
        }
    }
    if(lineTrackerLeft.getValue() >= 750 &&
            lineTrackerLeft.getValue() <= 1060 &&
            lineTrackerRight.getValue() <= 200 &&
            lineTrackerRight.getValue() >= 20){
        for(int i= 0; i <= 100; i++){                 
            motor.changeState(ONE_STEP_CLK_WISE);
            Serial.println("TurnRight_ClcWise");
        }
    }
    else {
    }
    /* motor.changeState(NOTHING); */
}
void loop() {
    // do nothing
}
