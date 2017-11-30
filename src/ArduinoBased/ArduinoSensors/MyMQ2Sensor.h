#ifndef MYMQ2SENSOR_H
#define MYMQ2SENSOR_H
#include "stdint.h"
#include "ISensor.h"

class MyMQ2Sensor: public ISensor
{
    public:
        MyMQ2Sensor(uint8_t pin,uint8_t sensorId);
        virtual ~MyMQ2Sensor();
        uint8_t read();
        void getReport(uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize );
        uint8_t getIdentity(uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize );
    protected:
    private:
        uint8_t sensorId;
        uint8_t pin;

};

#endif // MYMQ2SENSOR_H
