#include "__ColorSense__.h"
#include "__LineTracker__.h"
#include "__Sensors__.h"
#include "__Send__.h"

int previous_colorSense_Red = 0;
int previous_colorSense_Green = 0;
int previous_colorSense_Blue = 0;

int previous_colorSense1_Red = 0;
int previous_colorSense1_Green = 0;
int previous_colorSense1_Blue = 0;

double previous_lineTracker_L = 0;
double previous_lineTracker_LL = 0;
double previous_lineTracker_R = 0;
double previous_lineTracker_RR = 0;

double colorAlpha = 0.3;
double lineAlpha = 0.7;

// Pins are defined in __ColorSense__.h
__ColorSense__ colorSense(S0, S1, S2, S3, COLORSENSE_OUT);
__ColorSense__ colorSense1(S00, S11, S22, S33, COLORSENSE_OUT1);
__Sensors__ sensors;
void setup() {
    Serial.begin(115200);
    pinMode(A0, INPUT);
}

DataQueue<int> laneCntQueue(3);
int laneCnt;
bool isBlackLane = false;
bool isWhiteLane = false;
int blackLaneCnt = 0;
int whiteLaneCnt = 0;

void loop() {
    int l = analogRead(A1);
    int ll = analogRead(A3);
    int r = analogRead(A0);
    int rr = analogRead(A2);
    // Basic Filtering
    double current_colorSense_Red = colorAlpha * colorSense.getRedColor() + (1-colorAlpha) * previous_colorSense_Red;
    double current_colorSense_Green = colorAlpha * colorSense.getGreenColor() + (1-colorAlpha) * previous_colorSense_Green;
    double current_colorSense_Blue = colorAlpha * colorSense.getBlueColor() + (1-colorAlpha) * previous_colorSense_Blue;

    double current_colorSense1_Red = colorAlpha * colorSense1.getRedColor() + (1-colorAlpha) * previous_colorSense1_Red;
    double current_colorSense1_Green = colorAlpha * colorSense1.getGreenColor() + (1-colorAlpha) * previous_colorSense1_Green;
    double current_colorSense1_Blue = colorAlpha * colorSense1.getBlueColor() + (1-colorAlpha) * previous_colorSense1_Blue;

    double current_lineTracker_LL = (lineAlpha * ll) + (1-lineAlpha) * previous_lineTracker_LL;
    double current_lineTracker_L = (lineAlpha * l) + (1-lineAlpha) * previous_lineTracker_L;
    double current_lineTracker_R = (lineAlpha * r) + (1-lineAlpha) * previous_lineTracker_R;
    double current_lineTracker_RR = (lineAlpha * rr) + (1-lineAlpha) * previous_lineTracker_RR;
    previous_lineTracker_L = current_lineTracker_LL;
    previous_lineTracker_LL = current_lineTracker_L;
    previous_lineTracker_R = current_lineTracker_R;
    previous_lineTracker_RR = current_lineTracker_RR;

    previous_colorSense_Red = current_colorSense_Red;
    previous_colorSense_Green = current_colorSense_Green;
    previous_colorSense_Blue = current_colorSense_Blue;

    previous_colorSense1_Red = current_colorSense1_Red;
    previous_colorSense1_Green = current_colorSense1_Green;
    previous_colorSense1_Blue = current_colorSense1_Blue;

    int laneValue;
    if (!laneCntQueue.isFull()) {
        laneCntQueue.enqueue(mean(current_colorSense_Red, current_colorSense_Blue, current_colorSense_Green));
    }
    else {
        laneValue = laneCntQueue.dequeue();
        laneCntQueue.enqueue(mean(current_colorSense_Red, current_colorSense_Blue, current_colorSense_Green));
    }

    if (laneValue > 500 && !isBlackLane) {
        isBlackLane = true; 
        blackLaneCnt++;
    } 

    if (laneValue < 500) {
        isBlackLane = false; 
    }

    if (laneValue < 500 && !isWhiteLane) {
        isWhiteLane = true; 
        whiteLaneCnt++;
    } 
    if (laneValue > 500) {
        isWhiteLane = false; 
    }
    sensors.push(
            current_lineTracker_LL,
            current_lineTracker_L,
            current_lineTracker_R,
            current_lineTracker_RR,
            current_colorSense_Green,
            current_colorSense_Blue,
            current_colorSense_Red,
            blackLaneCnt);
    /*  sensors.log(); */
    sensors.sendPayload(blackLaneCnt);
    // * Don't forget to add delay
    // * cuz it won't send the payload correctly
    delay(10);
}
// քառակուսային միջին
int mean(int a, int b, int c) {
    return (a+b+c);
}
