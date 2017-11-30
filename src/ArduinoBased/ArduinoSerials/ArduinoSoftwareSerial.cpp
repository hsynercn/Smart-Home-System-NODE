#include "ArduinoSoftwareSerial.h"

#define JITTERACCAPTENCE 20

ArduinoSoftwareSerial::ArduinoSoftwareSerial(uint8_t in,uint8_t out):mySerial(in, out)
{

}

ArduinoSoftwareSerial::~ArduinoSoftwareSerial()
{
    //dtor
}

int16_t ArduinoSoftwareSerial::available()
{
     return this->mySerial.available();
}

uint8_t ArduinoSoftwareSerial::read(void)
{
    return this->mySerial.read();
}

int32_t ArduinoSoftwareSerial::print(String msg)
{

    int16_t i=0;
    while(msg[i]!='\0')
    {
        this->mySerial.print(String(msg[i]));
        i++;
    }
    return 0;
}

int32_t ArduinoSoftwareSerial::println(String msg)
{
    int16_t i=0;
    while(msg[i]!='\0')
    {
        this->mySerial.print(String(msg[i]));
        i++;
    }
    this->mySerial.println("");
    return 0;
}

uint16_t ArduinoSoftwareSerial::safePrint(uint8_t *pointer, uint16_t prmSize )
{
    uint16_t i=0;
    for(i=0;i<prmSize;i++)
    {
        this->mySerial.print((char)pointer[i]);
    }
    return i;
}

uint16_t ArduinoSoftwareSerial::safeRead(uint8_t *pointer, uint16_t prmSize )
{
    uint16_t i = 0;
    while( (this->available()) && (i<prmSize) )
    {
        pointer[i] = this->read();
        i++;
        int w=0;
        while( (!this->available()) && (w<20))
        {
            delay(100);
            w++;
        }
    }
    if(i<(prmSize-1))
        pointer[i] = '\0';
    else
        pointer[prmSize-1] = '\0';
    return i;
}


uint16_t ArduinoSoftwareSerial::safeAdvancedRead(uint8_t *pointer, uint16_t prmSize,uint8_t start,uint8_t stop )
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
                delay(10);
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
                delay(10);
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
void ArduinoSoftwareSerial::begin(uint32_t baudRate)
{
    this->mySerial.begin(baudRate);

}
void ArduinoSoftwareSerial::flush()
{
    this->mySerial.flush();
}
