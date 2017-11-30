#include "ArduinoBuzzer.h"
#include "Arduino.h"
//min:31	max:65535
ArduinoBuzzer::ArduinoBuzzer(uint8_t pinNumber, uint8_t controlId)
{
    this->pinNumber = pinNumber;
    this->controlId = controlId;
}

uint8_t ArduinoBuzzer::getStateReport(uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize )
{
    *lastBufferSize = snprintf_P((char*)buffer,maxBufferSize, PSTR("{\"conId\":%d,\"st\":null}"),this->controlId);
    return 1;

}

uint8_t ArduinoBuzzer::acceptCommand(uint8_t* buffer, uint16_t bufferSize)
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
    if(proceed==1)
    switch(selectedCommand) {

        case 1 :
            this->beep();executed=1;
            break;

        case 2 :
            this->alarm();executed=1;
            break;
    }
    return executed;

}
uint8_t ArduinoBuzzer::getIdentity(uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize )
{

    *lastBufferSize = snprintf_P((char*)buffer,maxBufferSize, PSTR("{\"deviceType\":\"Control\",\"localId\":%d,\"name\":\"Buzzer\",\"states\":[],\"commands\":[{\"id\":1,\"name\":\"beep\"},{\"id\":2,\"name\":\"alarm\"}]}"),this->controlId);
    return 0;
}


void ArduinoBuzzer::beep()
{
    uint16_t beepwait=500;
    this->start();
    delay(beepwait);
    this->stop();
    delay(beepwait);
    this->start();
    delay(beepwait);
    this->stop();
    delay(beepwait);
    this->start();
    delay(beepwait);
    this->stop();
}

void ArduinoBuzzer::alarm()
{
    uint16_t beepwait=500;
    this->start();
    delay(beepwait);
    this->stop();
    delay(beepwait);
    this->start();
    delay(beepwait);
    this->stop();
    delay(beepwait);
    this->start();
    delay(beepwait);
    this->stop();
}

void ArduinoBuzzer::start()
{
    digitalWrite(this->pinNumber, HIGH);
}
void ArduinoBuzzer::stop()
{
    digitalWrite(this->pinNumber, LOW);
}
