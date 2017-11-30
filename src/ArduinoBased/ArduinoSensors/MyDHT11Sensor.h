#ifndef MYDHT11SENSOR_H
#define MYDHT11SENSOR_H
#include "stdint.h"
#include "ISensor.h"

class MyDHT11Sensor: public ISensor
{
    public:
        MyDHT11Sensor(uint8_t pin,uint8_t sensorId1,uint8_t sensorId2);
        virtual ~MyDHT11Sensor();
        uint8_t read();
        void getReport(uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize );
        uint8_t getIdentity(uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize );

    protected:
    private:
        uint8_t pin;
        uint8_t sensorId1;
        uint8_t sensorId2;
        uint8_t waitForStatus(uint8_t status,uint8_t timeOut);
        uint8_t readByte();
        uint8_t RHint;
        uint8_t Tint;
};

#endif // MYDHT11SENSOR_H
