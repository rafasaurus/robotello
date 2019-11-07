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
#define COLORSENSE1_MEAN 7
#define LANE_CNT 8
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
        int colorSense1Mean_;
        int laneCnt_;
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
        int colorSense1GetMean() {
            return this->colorSense1Mean_;
        }
        int getLaneCnt() {
            return this->laneCnt_;
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
                            break;
                        case LL:
                            this->LL_ = position;
                            break;
                        case R:
                            this->R_ = position;
                            break;
                        case RR:
                            this->RR_ = position;
                            break;
                        case COLORSENSE_RED:
                            this->colorSenseRed_ = position;
                            break;
                        case COLORSENSE_GREEN:
                            this->colorSenseGreen_ = position;
                            break;
                        case COLORSENSE_BLUE:
                            this->colorSenseBlue_ = position;
                            break;
                        case COLORSENSE1_MEAN:
                            this->colorSense1Mean_ = position;
                            break;
                        case LANE_CNT:
                            this->laneCnt_ = position;
                            break;
                    }
                }
                // Find the next sensorData in input string
                sensorData = strtok(0, "&");
            }
        }
};
#endif
