char str[4];

void setup() {
    Serial.begin(115200);
    pinMode(A0, INPUT);
}

void loop() {
    int value = analogRead(A0);
    itoa(value, str, 10); //Turn value into a character array
    Serial.write(str, 4);
    Serial.write('\n');
}
