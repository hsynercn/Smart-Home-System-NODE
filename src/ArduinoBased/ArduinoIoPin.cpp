#include "ArduinoIoPin.h"

#include "SoftwareSerial.h"

//CONVERTED TO GENERIC TYPES


ArduinoIoPin::ArduinoIoPin(uint8_t pinNumber,uint8_t pinMode)
{
    this->pinNumber = pinNumber;
    //pinMode((uint8_t)this->pinNumber,(uint8_t)pinMode);
    //pinMode();
    this->setPinMode(pinMode);
}

ArduinoIoPin::~ArduinoIoPin()
{
    //dtor
}

void ArduinoIoPin::setPinMode(uint8_t mode)
{
    //INPUT = 0, OUTPUT = 1, or INPUT_PULLUP = 2.
    if(mode == 0)
        pinMode(this->pinNumber, INPUT);

    if(mode == 1)
        pinMode(this->pinNumber, OUTPUT);
    if(mode == 2)
        pinMode(this->pinNumber, INPUT_PULLUP);
}

void ArduinoIoPin::setDigitalWrite(uint8_t value)
{
    //INPUT = 0, OUTPUT = 1, or INPUT_PULLUP = 2.
    if(value == 0)
        digitalWrite(this->pinNumber, LOW);
    if(value == 1)
        digitalWrite(this->pinNumber, HIGH);

}



