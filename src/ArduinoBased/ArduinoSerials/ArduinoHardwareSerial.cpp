#include "ArduinoHardwareSerial.h"

#define JITTERACCAPTENCE 10
#define JITTERACCAPTENCEWAIT 10

//CONVERTED TO GENERIC TYPES

ArduinoHardwareSerial::ArduinoHardwareSerial()
{
    //ctor
}

ArduinoHardwareSerial::~ArduinoHardwareSerial()
{
    //dtor
}

int16_t ArduinoHardwareSerial::available()
{
    return Serial.available();
}

void ArduinoHardwareSerial::begin(uint32_t baudRate)
{

    Serial.begin(baudRate);
}

void ArduinoHardwareSerial::flush()
{

    Serial.flush();
}
uint8_t ArduinoHardwareSerial::read()
{

    return Serial.read();

}

int32_t ArduinoHardwareSerial::print(String str)
{

    return Serial.print(str);
}

uint16_t ArduinoHardwareSerial::safePrint(uint8_t *pointer, uint16_t prmSize )
{
    uint16_t i=0;
    for(i=0;i<prmSize;i++)
    {
        Serial.print((char)pointer[i]);
    }
    return i;
}

uint16_t ArduinoHardwareSerial::safeRead(uint8_t *pointer, uint16_t prmSize )
{
    //*pointer: writes incoming stream on it
    //prmSize: max buffer size
    //return read array size
    uint16_t i = 0;
    while( (this->available()) && (i<prmSize) )
    {
        pointer[i] = this->read();
        i++;
        int w=0;
        while( (!this->available()) && (w<JITTERACCAPTENCE))//WAITS UNTIL UP TO 2 SECOND FOR SERIAL TERMINATION
        {
            delay(JITTERACCAPTENCEWAIT);
            w++;
        }
    }
    if(i<(prmSize-1))
        pointer[i] = '\0';
    else
        pointer[prmSize-1] = '\0';
    return i;
}

uint16_t ArduinoHardwareSerial::safeAdvancedRead(uint8_t *pointer, uint16_t prmSize,uint8_t start,uint8_t stop )
{
    //*pointer: writes incoming stream on it
    //prmSize: max buffer size
    //start: start recording after receiving this char
    //stop: stop recording after receiving this char
    //return read array size
    uint16_t i = 0;

    uint8_t startFound=0;
    uint8_t stopFound=0;
    uint8_t temp ;
    while( (this->available()) && (startFound == 0) )
    {
        temp=this->read();
        if(start ==temp)
        {
            startFound = 1;
        }
        else
        {
            int w=0;
            while( (!this->available()) && (w<JITTERACCAPTENCE))//WAITS UNTIL UP TO 2 SECOND FOR SERIAL TERMINATION
            {
                delay(JITTERACCAPTENCEWAIT);
                w++;
            }
        }
    }
    int w=0;
    while( ( (this->available() ) && (i<prmSize) ) && (stopFound == 0) )
    {

        temp = this->read();
        if(temp != stop)
        {
            pointer[i] = temp;
            i++;
            w=0;
            while( (!this->available()) && (w<JITTERACCAPTENCE))//WAITS UNTIL UP TO 2 SECOND FOR SERIAL TERMINATION
            {
                delay(JITTERACCAPTENCEWAIT);
                w++;
            }
        }
        else
        {
            stopFound = 1;
        }
    }
    if(i<(prmSize-1))
        pointer[i] = '\0';
    else
        pointer[prmSize-1] = '\0';
    return i;
}


int32_t ArduinoHardwareSerial::println(String str)
{

    return Serial.println(str);
}


