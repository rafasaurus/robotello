#ifndef __colorsense__
#define __colorsense__
// First Sensor pins
#define S0 45
#define S1 44
#define S2 42
#define S3 43
#define COLORSENSE_OUT 9
// Seccond Sensor pins
#define S00 24
#define S11 26
#define S22 28
#define S33 30
#define COLORSENSE_OUT1 8

class __ColorSense__ {
    private:
        int frequency = 0;
        int s0;
        int s1;
        int s2;
        int s3;
        int sensorOut;
    public:
        __ColorSense__(int s0, int s1, int s2, int s3, int sensorOut) {
            this->s0 = s0;
            this->s1 = s1;
            this->s2 = s2;
            this->s3 = s3;
            this->sensorOut = sensorOut;
            pinMode(s0, OUTPUT);
            pinMode(s1, OUTPUT);
            pinMode(s2, OUTPUT);
            pinMode(s3, OUTPUT);
            pinMode(sensorOut, INPUT);
            // Setting frequency-scaling to 20%
            digitalWrite(s0,HIGH);
            digitalWrite(s1,LOW);
        }
        int getRedColor() {
            digitalWrite(s2,LOW);
            digitalWrite(s3,LOW);
            this->frequency = pulseIn(this->sensorOut, LOW);
            return this->frequency;
        }
        int getGreenColor() {
            digitalWrite(s2,HIGH);
            digitalWrite(s3,HIGH);
            this->frequency = pulseIn(this->sensorOut, LOW);
            return this->frequency;
        }
        int getBlueColor() {
            digitalWrite(s2,LOW);
            digitalWrite(s3,HIGH);
            this->frequency = pulseIn(this->sensorOut, LOW);
            return this->frequency;
        }
};
#endif
