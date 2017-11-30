#ifndef MYFC109SENSOR_H
#define MYFC109SENSOR_H

#include "arduino.h"
class MyFC109Sensor
{
    public:
        MyFC109Sensor(uint8_t pin,uint8_t sensorId);
        virtual ~MyFC109Sensor();

        virtual ~MyMQ2Sensor();
        uint8_t read();
        void getReport(uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize );
        uint8_t getIdentity(uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize );
    protected:
    private:
};

#endif // MYFC109SENSOR_H
