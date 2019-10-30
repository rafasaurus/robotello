#ifndef lineTracker
#define lineTracker

class __LineTracker__ {
    private:
        int whiteLineThreshold_;
        int blackLineThreshold_;
        int whiteLineOffset_;
        int blackLineOffset_;
        int pin_;
    public:
        __LineTracker__(int whiteLineThreshold, 
                int blackLineThreshold_, 
                int whiteLineOffset_, 
                int blackLineOffset_, 
                int pin_) {
            this->whiteLineThreshold_ = whiteLineThreshold_;
            this->blackLineThreshold_ = blackLineThreshold_;
            this->whiteLineOffset_ = whiteLineOffset_;
            this->blackLineOffset_ = blackLineOffset_;
            this->pin_ = pin_;
            pinMode(pin_, INPUT);
        }
        int getSensor () {
            return analogRead(this->pin_);
        };
        int getValue () {
            if (this->getSensor() > this->whiteLineOffset_ - this -> whiteLineThreshold_ &&
                    this->getSensor() < this->whiteLineOffset_ + this->whiteLineThreshold_) {
                return 1;
            } else 
            if (this->getSensor() > this->blackLineOffset_ - this -> blackLineThreshold_ &&
                    this->getSensor() < this->blackLineOffset_ + this->blackLineThreshold_) {
                return 0;
            }
        }
};
#endif
