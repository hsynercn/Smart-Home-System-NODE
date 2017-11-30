#ifndef ARDUINOBASICLED_H
#define ARDUINOBASICLED_H
#include "stdint.h"
#include "IControl.h"

class ArduinoBasicLed:public IControl
{
    public:
        ArduinoBasicLed(uint8_t pinNumber, uint8_t controlId);

        uint8_t getStateReport(uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize );
        uint8_t acceptCommand(uint8_t *command , uint16_t commandLen);
        uint8_t getIdentity(uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize );
        //uint8_t isStateChanged();

        void on();
        void off();

    protected:
    private:
        uint8_t pinNumber;
        uint8_t controlId;
        uint8_t currentState;
        uint8_t oldState;

};

#endif // ARDUINOBUZZER_H
