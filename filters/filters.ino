#include "__ColorSense__.h"
#include "__LineTracker__.h"
#include "__LaneCounter__.h"

char l_str[5]; // left
char ll_str[5]; // leftleft sensor
char r_str[5]; // right
char rr_str[5]; //rightright sensor
char payload_str[5];

// sensorIds
#define R 0
#define L 1
#define RR 2
#define LL 3
#define COLORSENSE_RED 4
#define COLORSENSE_GREEN 5
#define COLORSENSE_BLUE 6

int previous_colorSense_Red = 0;
int previous_colorSense_Green = 0;
int previous_colorSense_Blue = 0;

double previous_lineTracker_L;
double previous_lineTracker_LL;
double previous_lineTracker_R;
double previous_lineTracker_RR;

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
    double current_lineTracker_L = lineAlpha * l * + (1-lineAlpha) * previous_lineTracker_L;
    double current_lineTracker_LL = lineAlpha * ll * + (1-lineAlpha) * previous_lineTracker_LL;
    double current_lineTracker_R = lineAlpha * r * + (1-lineAlpha) * previous_lineTracker_R;
    double current_lineTracker_RR = lineAlpha * rr * + (1-lineAlpha) * previous_lineTracker_RR;
    previous_colorSense_Red = current_colorSense_Red;
    previous_colorSense_Green = current_colorSense_Green;
    previous_colorSense_Blue = current_colorSense_Blue;
    laneCounter.push(current_colorSense_Red,
                        current_colorSense_Green,
                        current_colorSense_Blue,
                        current_lineTracker_LL,
                        current_lineTracker_L,
                        current_lineTracker_R,
                        current_lineTracker_RR);
    Serial.print(" -----------------  ");
    Serial.print(current_lineTracker_L);
    Serial.print(" ");
    Serial.print(current_lineTracker_LL);
    Serial.print(" ");
    Serial.print(current_lineTracker_R);
    Serial.print(" ");
    Serial.println(current_lineTracker_RR);
    laneCounter.getStat();
    /* send(l, L); */
    /* send(ll, LL); */
    /* send(r, R); */
    /* send(rr, RR); */
    /* send(current_colorSense_Red, COLORSENSE_RED); */
    /* send(current_colorSense_Green, COLORSENSE_GREEN); */
    /* send(current_colorSense_Blue, COLORSENSE_BLUE); */
    delay(2000);
}
inline void
send(int payload, int sensorId) {
    itoa(payload, payload_str, 10);
    Serial.print(sensorId);
    Serial.print(":");
    Serial.print(payload_str);
    Serial.print('&');
}
