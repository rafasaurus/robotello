//Sender Code

char str[4];

void setup() {
    Serial.begin(115200);
}

void loop() {
    int value=1234; //this would be much more exciting if it was a sensor value
    itoa(value, str, 10); //Turn value into a character array
    Serial.write(str, 4);
}
