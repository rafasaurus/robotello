const unsigned int MAX_INPUT = 32;

#define L 1
#define LL 3
#define R 0
#define RR 2

void setup ()
{
    Serial.begin (115200);
    Serial3.begin(115200);
} // end of setup

void process_data (const char * data) {
    // Read each sensorData pair 
    char* sensorData = strtok(data, "&");
    while (sensorData != 0) {
        // Split the sensorData in two values
        char* separator = strchr(sensorData, ':');
        if (separator != 0) {
            // Actually split the string in 2: replace ':' with 0
            *separator = 0;
            int sensorId = atoi(sensorData);
            ++separator;
            int position = atoi(separator);
            switch(sensorId) {
                case L:
                    Serial.println("*************");
                    Serial.println(sensorId);
                    Serial.println(position);
            }
        }
        // Find the next sensorData in input string
        sensorData = strtok(0, "&");
    }
}

void processIncomingByte (const byte inByte)
{
    static char input_line [MAX_INPUT];
    static unsigned int input_pos = 0;

    switch (inByte)
    {
        case '&':   // end of text charecter
            input_line [input_pos] = 0;  // terminating null byte
            // terminator reached! process input_line here ...
            process_data (input_line);
            // reset buffer for next time
            input_pos = 0;  
            break;
        case '\r':   // discard carriage return
            break;
        default:
            // keep adding if not full ... allow for terminating null byte
            if (input_pos < (MAX_INPUT - 1))
                input_line [input_pos++] = inByte;
            break;
    }
}

void loop()
{
    // if serial data available, process it
    while (Serial3.available () > 0)
        processIncomingByte (Serial3.read ());

    // do other stuff here like testing digital input (button presses) ...

}  // end of loop

