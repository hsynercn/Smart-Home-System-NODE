#include "ArduinoBasicLed.h"
#include "Arduino.h"

ArduinoBasicLed::ArduinoBasicLed(uint8_t pinNumber, uint8_t controlId)
{
    this->pinNumber = pinNumber;
    this->controlId = controlId;
    this->currentState =1;
    this->oldState =1;

}

uint8_t ArduinoBasicLed::getStateReport(uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize )
{
    if(this->currentState==1)
    {
        *lastBufferSize = snprintf_P((char*)buffer,maxBufferSize, PSTR("{\"conId\":%d,\"st\":1}"),this->controlId);
        return 1;
    }

    if(this->currentState==2)
    {
       *lastBufferSize = snprintf_P((char*)buffer,maxBufferSize, PSTR("{\"conId\":%d,\"st\":2}"),this->controlId);
       return 1;
    }

    return 0;
}

uint8_t ArduinoBasicLed::acceptCommand(uint8_t* buffer, uint16_t bufferSize)
{
    uint16_t bufferCursor = 0;
    uint8_t proceed = 0;
    uint8_t selectedCommand=0;
    uint8_t state=0;
    uint8_t executed=0;

    while( (bufferCursor<bufferSize) && (proceed == 0) )
    {
        if( ( buffer[bufferCursor] >= '0' ) && ( buffer[bufferCursor] <= '9' ) )

        {
            selectedCommand=buffer[bufferCursor]-'0';
            if(selectedCommand==0)
            {
                while( (bufferCursor<bufferSize) && (buffer[bufferCursor]!=',') )
                {
                    bufferCursor++;
                }
                bufferCursor++;
                state = buffer[bufferCursor]-'0';
            }
            proceed=1;
        }
        else
            bufferCursor++;
    }
    this->oldState = this->currentState;
    if(proceed==1)
    switch(selectedCommand) {

        case 0 :
            if(state==1){this->currentState =1;this->off();executed=1;}
            if(state==2){this->currentState =2;this->on();executed=1;}
            break;
        case 1 :
            this->on();executed=1;
            break;

        case 2 :
            this->off();executed=1;
            break;
    }

    return executed;
}
uint8_t ArduinoBasicLed::getIdentity(uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize )
{
    *lastBufferSize = snprintf_P((char*)buffer,maxBufferSize, PSTR("{\"deviceType\":\"Control\",\"localId\":%d,\"name\":\"Led\",\"states\":[{\"n\":\"off\",\"i\":1},{\"n\":\"on\",\"i\":2}],\"commands\":[{\"id\":1,\"name\":\"on\"},{\"id\":2,\"name\":\"off\"}]}"),this->controlId);
    return 0;
}

void ArduinoBasicLed::on()
{
    digitalWrite(this->pinNumber, HIGH);
}
void ArduinoBasicLed::off()
{
    digitalWrite(this->pinNumber, LOW);
}
