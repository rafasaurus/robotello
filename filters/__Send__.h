#ifndef __send__
#define __send__

inline void
send(int payload, int sensorId) {
    char payload_str[5];
    itoa(payload, payload_str, 10);
    Serial.print(sensorId);
    Serial.print(":");
    Serial.print(payload_str);
    Serial.print('&');
}

#endif
