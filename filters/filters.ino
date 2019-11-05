#include "__ColorSense__.h"
#include "__LineTracker__.h"

char l_str[4]; // left
char ll_str[4]; // leftleft sensor
char r_str[4]; // right
char rr_str[4]; //rightright sensor
char payload_str[4];

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

float alpha = 0.7;
// Pins are defined in __ColorSense__.h
/* __ColorSense__ colorSense(S0, S1, S2, S3, COLORSENSE_OUT); */
__ColorSense__ colorSense(S00, S11, S22, S33, COLORSENSE_OUT1);
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
    float current_colorSense_Red = alpha * colorSense.getRedColor() + (1-alpha) * previous_colorSense_Red;
    float current_colorSense_Green = alpha * colorSense.getGreenColor() + (1-alpha) * previous_colorSense_Green;
    float current_colorSense_Blue = alpha * colorSense.getBlueColor() + (1-alpha) * previous_colorSense_Blue;

    previous_colorSense_Red = current_colorSense_Red;
    previous_colorSense_Green = current_colorSense_Green;
    previous_colorSense_Blue = current_colorSense_Blue;

    send(l, L);
    send(ll, LL);
    send(r, R);
    send(rr, RR);
    send(current_colorSense_Red, COLORSENSE_RED);
    send(current_colorSense_Green, COLORSENSE_GREEN);
    send(current_colorSense_Blue, COLORSENSE_BLUE);

    /* //Turn values into a character arrays */
    /* itoa(l, l_str, 10); */
    /* itoa(ll, ll_str, 10); */
    /* itoa(r, r_str, 10); */
    /* itoa(rr, rr_str, 10); */
    /*  */
    /* Serial.print(L); */
    /* Serial.print(":"); */
    /* Serial.print(l_str); */
    /* Serial.print('&'); */
    /*  */
    /* Serial.print(LL); */
    /* Serial.print(":"); */
    /* Serial.print(ll_str); */
    /* Serial.print('&'); */
    /*  */
    /* Serial.print(R); */
    /* Serial.print(":"); */
    /* Serial.print(r_str); */
    /* Serial.print('&'); */
    /*  */
    /* Serial.print(RR); */
    /* Serial.print(":"); */
    /* Serial.print(rr_str); */
    /* Serial.print('&'); */
}
inline void
send(int payload, int sensorId) {
    itoa(payload, payload_str, 10);
    Serial.print(sensorId);
    Serial.print(":");
    Serial.print(payload_str);
    Serial.print('&');
}
