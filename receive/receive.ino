const unsigned int MAX_INPUT = 50;

#define L 0
#define LL 1
#define R 2
#define RR 3

void setup ()
{
    Serial.begin (115200);
    Serial3.begin(115200);
} // end of setup

// here to process incoming serial data after a terminator received
void process_data (const char * data) {
    // for now just display it
    // (but you could compare it to some value, convert to an integer, etc.)
    /* Serial.println (data); */

    // Read each command pair 
    char* command = strtok(data, "&");
    while (command != 0) {
        // Split the command in two values
        char* separator = strchr(command, ':');
        if (separator != 0) {
            // Actually split the string in 2: replace ':' with 0
            *separator = 0;
            int sensorId = atoi(command);
            ++separator;
            int position = atoi(separator);
            switch(sensorId) {
                case L:
                    Serial.println("*************");
                    Serial.println(sensorId);
                    Serial.println(position);
            }
        }
        // Find the next command in input string
        command = strtok(0, "&");
    }
}  // end of process_d

void processIncomingByte (const byte inByte)
{
    static char input_line [MAX_INPUT];
    static unsigned int input_pos = 0;

    switch (inByte)
    {

        case '&':   // end of text
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

    }  // end of switch

} // end of processIncomingByte  

void loop()
{
    // if serial data available, process it
    while (Serial3.available () > 0)
        processIncomingByte (Serial3.read ());

    // do other stuff here like testing digital input (button presses) ...

}  // end of loop

