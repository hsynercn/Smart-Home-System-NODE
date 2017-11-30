#ifndef ISERIAL_H
#define ISERIAL_H

#include "stdint.h"

class String;

class ISerial
{
    public:
        virtual int16_t available() = 0;
        virtual uint8_t read(void) = 0;
        virtual int32_t print(String) = 0;
        virtual int32_t println(String) = 0;
        virtual uint16_t safePrint(uint8_t*, uint16_t );
        virtual uint16_t safeRead(uint8_t*, uint16_t );
        virtual uint16_t safeAdvancedRead(uint8_t*, uint16_t ,uint8_t ,uint8_t);
        virtual void begin(uint32_t) = 0;
        virtual void flush(void) = 0;
    protected:
    private:
};

#endif // ISERIAL_H









