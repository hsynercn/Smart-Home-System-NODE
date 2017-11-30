#include "MyFC109Sensor.h"

MyFC109Sensor::MyFC109Sensor()
{
    //ctor
}

MyFC109Sensor::~MyFC109Sensor()
{
    //dtor
}


MyFC109Sensor::MyMQ2Sensor(uint8_t pin, uint8_t sensorId)
{
    this->pin = pin;
    this->sensorId = sensorId;

}

MyFC109Sensor::~MyMQ2Sensor()
{
    //dtor
}


uint8_t MyFC109Sensor::read()
{
    uint16_t sensorVoltage = 0;
    for(int i=0;i<10;i++)
    {
        sensorVoltage+= analogRead(this->pin);
    }
    sensorVoltage/=10;

    if(sensorVoltage>SAFEOUTPUTLIMIT)
        return 1;
    else
        return 0;

}

 void MyFC109Sensor::getReport(uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize )
 {
    if(this->read() == 1)
        *lastBufferSize = snprintf_P((char*)buffer,maxBufferSize, PSTR("{\"id\":%d,\"val\":true}"),this->sensorId );
    else
        *lastBufferSize = snprintf_P((char*)buffer,maxBufferSize, PSTR("{\"id\":%d,\"val\":false}"),this->sensorId);
 }

uint8_t MyFC109Sensor::getIdentity(uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize )
{
    *lastBufferSize = snprintf_P((char*)buffer,maxBufferSize, PSTR("{\"deviceType\":\"Sensor\",\"localId\":%d,\"name\":\"MQ2 gas\",\"varType\":\"Boolean\",\"unit\":null}"),this->sensorId);
    return 0;
}
