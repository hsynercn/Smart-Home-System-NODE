#ifndef ICONTROL_H
#define ICONTROL_H

#include "stdint.h"
class IControl
{
    public:
        virtual uint8_t getStateReport(uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize);
        virtual uint8_t acceptCommand(uint8_t *command , uint16_t commandLen);
        virtual uint8_t getIdentity(uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize );
    protected:
    private:
};

#endif // ICONTROL_H
