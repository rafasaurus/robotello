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

#define rightSensorPin A0 // RightSesnor
#define leftSensorPin A1 // LeftSesnor
#define INCLUDE_vTaskDelay 1
TaskHandle_t motor_task_handler;
void MyTask1( void *pvParameters );


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
    /* xTaskCreate(MyTask1, "MyTask1", 10000, NULL, 1, NULL); */
    /* vTaskStartScheduler(); */
    motor.changeState(ONE_STEP);
    motor.startTask();
}

/* Task1 with priority 1 */
static void MyTask1(void* pvParameters)
{   
    (void) pvParameters;
    while(1) {
        Serial.println(F("\nTask1\n"));
        vTaskDelay(1000000);
    }
}

void loop() {
    //nothing
}
