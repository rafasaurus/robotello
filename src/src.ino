#include <Arduino.h>
#include "__Servo__.h"
#include "__Move__.h"
#include "__Ultrasonic__.h"
#include "__ColorSense__.h"

#ifndef RTOS
#define RTOS
#include <Arduino_FreeRTOS.h>
#include <event_groups.h>
#include <queue.h>
#endif
#define LED_BUILTIN 13

#define rightSensorPin A0 // RightSesnor
#define leftSensorPin A1 // LeftSesnor
#define INCLUDE_vTaskDelay 1
/* TaskHandle_t motor_task_handler; */
void Loop(void *pvParameters);

__ColorSense__ colorSense;
/* __Servo__ arm(0, 60, 15, 5); */
/* __Servo__ armTwo(0, 168, 30, 3); */
/* __Servo__ armOne(0, 180, 30,4); */
/* __Servo__ armRotate(2); */

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
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
    Serial.print("setup complete");
    Serial.print("setup complete");
    Serial.print("setup complete");
    Serial.print("setup complete");
    Serial.print("setup complete");
    Serial.print("setup complete");
    /* motor.changeDirForward(); */
    /* arm.open(); */
    pinMode(13, OUTPUT);
    xTaskCreate(
            Loop
            ,  (const portCHAR *)"Blink"   // A name just for humans
            ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
            ,  NULL
            ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
            ,  NULL );
    /* vTaskStartScheduler(); */
    motor.changeState(ONE_STEP);
    motor.startTask();
}

/* Task1 with priority 1 */
void Loop(void *pvParameters)  // This is a task.
{
    (void) pvParameters;
    while(1) // A Task shall never return or exit.
    {
        Serial.print("\nALAZAN");
        digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
        vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
        motor.changeState(ONE_STEP);
        Serial.print("alazan");
        digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
        vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
        motor.changeState(NOTHING);
    }
}

void loop() {
    //nothing
}
