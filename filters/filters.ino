#include "__ColorSense__.h"
#include "__LineTracker__.h"
#include "__LaneCounter__.h"
#include "__Send__.h"

int previous_colorSense_Red = 0;
int previous_colorSense_Green = 0;
int previous_colorSense_Blue = 0;

double previous_lineTracker_L = 0;
double previous_lineTracker_LL = 0;
double previous_lineTracker_R = 0;
double previous_lineTracker_RR = 0;

double colorAlpha = 0.3;
double lineAlpha = 0.7;

// Pins are defined in __ColorSense__.h
__ColorSense__ colorSense(S0, S1, S2, S3, COLORSENSE_OUT);
/* __ColorSense__ colorSense(S00, S11, S22, S33, COLORSENSE_OUT1); */
__LaneCounter__ laneCounter;
void setup() {
    Serial.begin(115200);
    pinMode(A0, INPUT);
}


void loop() {
    int l = analogRead(A1);
    int ll = analogRead(A3);
    int r = analogRead(A0);
    int rr = analogRead(A2);
    // Basic Filtering
    double current_colorSense_Red = colorAlpha * colorSense.getRedColor() + (1-colorAlpha) * previous_colorSense_Red;
    double current_colorSense_Green = colorAlpha * colorSense.getGreenColor() + (1-colorAlpha) * previous_colorSense_Green;
    double current_colorSense_Blue = colorAlpha * colorSense.getBlueColor() + (1-colorAlpha) * previous_colorSense_Blue;

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
    laneCounter.push(
            current_lineTracker_LL,
            current_lineTracker_L,
            current_lineTracker_R,
            current_lineTracker_RR,
            current_colorSense_Green,
            current_colorSense_Blue,
            current_colorSense_Red);
    /* laneCounter.log(); */
    laneCounter.sendPayload();
    // * Don't forget to add delay
    // * cuz it won't send the payload correctly
    delay(50);
}
