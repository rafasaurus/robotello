#ifndef __serial__
#define __serial__
class __Serial__ {
    private:
        String str;  
    public:
        void getMessage() {
            if (Serial.available()) {
                this->str = Serial.readString();
            }
        }
};
#endif
