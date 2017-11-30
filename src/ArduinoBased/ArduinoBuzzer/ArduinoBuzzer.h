#ifndef ARDUINOBUZZER_H
#define ARDUINOBUZZER_H
#include "stdint.h"
#include "IControl.h"

class ArduinoBuzzer:public IControl
{
    public:
        ArduinoBuzzer(uint8_t pinNumber, uint8_t controlId);
        //virtual ~ArduinoBuzzer();
        void playNote(uint32_t freq);

        uint8_t getStateReport(uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize );
        uint8_t acceptCommand(uint8_t *command , uint16_t commandLen);
        uint8_t getIdentity(uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize );
        //uint8_t isStateChanged(void);

        void beep();
        void alarm();
        void start();
        void stop();
    protected:
    private:
        uint8_t pinNumber;
        uint8_t controlId;
};

#endif // ARDUINOBUZZER_H
