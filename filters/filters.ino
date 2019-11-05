char l_str[4]; // left
char ll_str[4]; // leftleft sensor
char r_str[4]; // right
char rr_str[4]; //rightright sensor

// sensorIds
#define L 0
#define LL 1
#define R 2
#define RR 3
void setup() {
    Serial.begin(115200);
    pinMode(A0, INPUT);
}

void loop() {
    int l = analogRead(A1);
    int ll = analogRead(A3);
    int r = analogRead(A0);
    int rr = analogRead(A2);

    //Turn values into a character arrays
    itoa(l, l_str, 10);
    itoa(ll, ll_str, 10);
    itoa(r, r_str, 10);
    itoa(rr, rr_str, 10);

    Serial.print(L);
    Serial.print(":");
    Serial.print(l_str);
    Serial.print('&');

    Serial.print(LL);
    Serial.print(":");
    Serial.print(ll_str);
    Serial.print('&');

    Serial.print(R);
    Serial.print(":");
    Serial.print(r_str);
    Serial.print('&');

    Serial.print(RR);
    Serial.print(":");
    Serial.print(rr_str);
    Serial.print('&');
}
