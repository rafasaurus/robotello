#ifndef __ultrasonic__ 
#define __ultrasonic__

#define trigPin 33 // Trigger pin
#define echoPin 31 // Echo pin

class __Ultrasonic__ {
    public:
        __Ultrasonic__();
        ~__Ultrasonic__();
        int getDistance();
};
__Ultrasonic__::__Ultrasonic__() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

int
__Ultrasonic__::getDistance() {
    long duration, cm, inches;
    // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
    // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Read the signal from the sensor: a HIGH pulse whose
    //      duration is the time (in microseconds) from the sending
    //      of the ping to the reception of its echo off of an object.
    duration = pulseIn(echoPin, HIGH);

    // Convert the time into a distance
    cm = (duration/2) / 29.1; // Divide by 29.1 or multiply by 0.0343
    inches = (duration/2) / 74; // Divide by 74 or multiply by 0.0135
    return cm * 10;
}

#endif
