#include "MyDHT11Sensor.h"
#include "Arduino.h"
#include <avr/pgmspace.h>

uint8_t ident=1;


MyDHT11Sensor::MyDHT11Sensor(uint8_t pin,uint8_t sensorId1,uint8_t sensorId2)
{
    this->pin = pin;
    this->sensorId1 = sensorId1;
    this->sensorId2 = sensorId2;
	pinMode(pin,INPUT);

}

MyDHT11Sensor::~MyDHT11Sensor()
{
    //dtor
}

uint8_t MyDHT11Sensor::waitForStatus(uint8_t status,uint8_t timeOut)
{

    uint32_t timeOutLimit = micros() + (uint32_t)timeOut;
    while(digitalRead(this->pin)!=status)
    {
        if(timeOutLimit<micros())
        {
            return 0;
        }

    }
    return 1;
}

uint8_t MyDHT11Sensor::readByte()
{
    uint8_t result = B00000000;
    uint8_t cursor = B10000000;
    int8_t i = 0;
    for(i=7;i>=0;i--)
    {
        waitForStatus(HIGH,50);
        delayMicroseconds(30);
        if(digitalRead(this->pin) == HIGH)
        {
            result = result | cursor;
        }
        cursor =(cursor>>1);
        this->waitForStatus(LOW,40);
    }
    return result;
}

uint8_t MyDHT11Sensor::read()
{
    //temp: temprature pointer
    //hum: humidity pointer
    // Normal read: return 0
    // Timeout error: return 1

    delay(400);
    pinMode(pin,OUTPUT);
    digitalWrite(this->pin,LOW);//
    delay(19);
    digitalWrite(this->pin,HIGH);//
    delayMicroseconds(40);
    pinMode(pin,INPUT);

    if(this->waitForStatus(LOW, 40) == 0){return 1;}
    if(this->waitForStatus(HIGH, 90) == 0){return 1;}
    if(this->waitForStatus(LOW, 90) == 0){return 1;}

    //"8bit integral RH data + 8bit decimal RH data + 8bit integral T data + 8bit decimal T data
    this->RHint = this->readByte();
    uint8_t RHdec = this->readByte();
    this->Tint = this->readByte();
    uint8_t Tdec = this->readByte();
    uint8_t check = this->readByte();

    if( (RHint+RHdec+Tint+Tdec)!=check )
    {
        return 1;
    }

    return 0;
}

void MyDHT11Sensor::getReport(uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize )
{
    this->read();
    *lastBufferSize = snprintf_P((char*)buffer,maxBufferSize, PSTR("{\"localId\":%d,\"val\":%d},{\"localId\":%d,\"val\":%d}"),this->sensorId1,this->RHint,this->sensorId2,this->Tint );
}

uint8_t MyDHT11Sensor::getIdentity(uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize )
{
    if( (ident%2) == 1)
    {
        *lastBufferSize = snprintf_P((char*)buffer,maxBufferSize, PSTR("{\"deviceType\":\"Sensor\",\"localId\":%d,\"name\":\"DHT11 hum\",\"varType\":\"Integer\",\"unit\":\"\%c hum\"}"),this->sensorId1,'%');
        ident--;
        return 1;
    }
    else
    {
        *lastBufferSize = snprintf_P((char*)buffer,maxBufferSize, PSTR("{\"deviceType\":\"Sensor\",\"localId\":%d,\"name\":\"DHT11 temp\",\"varType\":\"Integer\",\"unit\":\"C\"}"),this->sensorId2);
        ident--;
        return 0;
    }
}
















