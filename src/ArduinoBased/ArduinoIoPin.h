#ifndef ARDUINOIOPIN_H
#define ARDUINOIOPIN_H

#include "IPin.h"
#include "Arduino.h"

//CONVERTED TO GENERIC TYPES

class ArduinoIoPin: public IPin
{
    public:
        ArduinoIoPin(uint8_t,uint8_t);
        virtual ~ArduinoIoPin();
        void setPinMode(uint8_t);
        void setDigitalWrite(uint8_t);

    protected:
    private:
        uint8_t pinNumber;


};

#endif // ARDUINOIOPIN_H
