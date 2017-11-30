#ifndef IPIN_H
#define IPIN_H


#include "stdint.h"

class IPin
{
    public:
        virtual void setPinMode(uint8_t) = 0;
        virtual void setDigitalWrite(uint8_t) = 0;
    protected:
    private:
};

#endif // IPIN_H
