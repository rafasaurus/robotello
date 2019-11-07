#ifndef __serial__
#define __serial__

// Payload id and Sensor id (from src/__Serial__.h) should match
// PayloadIds
#define R 0
#define L 1
#define RR 2
#define LL 3
#define COLORSENSE_RED 4
#define COLORSENSE_GREEN 5
#define COLORSENSE_BLUE 6
#define COLORSENSE1_RED 7
#define COLORSENSE1_GREEN 8
#define COLORSENSE1_BLUE 9
#define MAX_INPUT 32

class __Serial__ {
    private:
        String str;  
        int L_;
        int LL_;
        int R_;
        int RR_;
        int colorSenseRed_;
        int colorSenseGreen_;
        int colorSenseBlue_;
        int colorSense1Red_;
        int colorSense1Green_;
        int colorSense1Blue_;
    public:
        String getMessage() {
            if (Serial.available()) {
                this->str = Serial.readString();
                return(this->str);
            }
        }

        int get_L_sensor() {
            return this->L_;
        }
        int get_R_sensor() {
            return this->R_;
        }
        int get_LL_sensor() {
            return this->LL_;
        }
        int get_RR_sensor() {
            return this->RR_;
        }
        int colorSenseGetRedColor() {
            return this->colorSenseRed_;
        }
        int colorSenseGetGreenColor() {
            return this->colorSenseGreen_;
        }
        int colorSenseGetBlueColor() {
            return this->colorSenseBlue_;
        }
        int colorSense1GetRedColor() {
            return this->colorSense1Red_;
        }
        int colorSense1GetGreenColor() {
            return this->colorSense1Green_;
        }
        int colorSense1GetBlueColor() {
            return this->colorSense1Blue_;
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
                    if (sensorId == 0) {}
                    switch(sensorId) {
                        case L:
                            this->L_ = position;
                        case LL:
                            this->LL_ = position;
                        case R:
                            this->R_ = position;
                        case RR:
                            this->RR_ = position;
                        case COLORSENSE_RED:
                            this->colorSenseRed_ = position;
                        case COLORSENSE_GREEN:
                            this->colorSenseGreen_ = position;
                        case COLORSENSE_BLUE:
                            this->colorSenseBlue_ = position;
                        case COLORSENSE1_RED:
                            this->colorSense1Red_ = position;
                        case COLORSENSE1_GREEN:
                            this->colorSense1Green_ = position;
                        case COLORSENSE1_BLUE:
                            this->colorSense1Blue_ = position;
                    }
                }
                // Find the next sensorData in input string
                sensorData = strtok(0, "&");
            }
        }
};
#endif
