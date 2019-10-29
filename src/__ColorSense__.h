#define S0 45
#define S1 44
#define S2 42
#define S3 43
#define sensorOut 9

class __ColorSense__ {
    private:
        int frequency = 0;
    public:
        // constructor
        __ColorSense__() {
            pinMode(S0, OUTPUT);
            pinMode(S1, OUTPUT);
            pinMode(S2, OUTPUT);
            pinMode(S3, OUTPUT);
            pinMode(sensorOut, INPUT);
            // Setting frequency-scaling to 20%
            digitalWrite(S0,HIGH);
            digitalWrite(S1,LOW);
        }
        int getRedColor() {
            // Setting red filtered photodiodes to be read
            digitalWrite(S2,LOW);
            digitalWrite(S3,LOW);
            // Reading the output frequency
            this->frequency = pulseIn(sensorOut, LOW);
            // Printing the value on the serial monitor
            // Serial.print("R= ");//printing name
            // Serial.print(this->frequency);//printing RED color frequency
            // Serial.print("  ");
            return this->frequency;
        }
        int getGreenColor() {

            // Setting Green filtered photodiodes to be read
            digitalWrite(S2,HIGH);
            digitalWrite(S3,HIGH);
            // Reading the output this->frequency
            this->frequency = pulseIn(sensorOut, LOW);
            // Printing the value on the serial monitor
            // Serial.print("G= ");//printing name
            // Serial.print(this->frequency);//printing RED color this->frequency
            // Serial.print("  ");
            return this->frequency;
        }
        int getBlueColor() {
            // Setting Blue filtered photodiodes to be read
            digitalWrite(S2,LOW);
            digitalWrite(S3,HIGH);
            // Reading the output frequency
            this->frequency = pulseIn(sensorOut, LOW);
            // Printing the value on the serial monitor
            // Serial.print("B= ");//printing name
            // Serial.print(this->frequency);//printing RED color frequency
            // Serial.println("  ");
            return this->frequency;
        }
};
