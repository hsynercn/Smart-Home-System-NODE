#ifndef ARDUINOMAINNODE_H
#define ARDUINOMAINNODE_H
#include <avr/pgmspace.h>
#include "stdint.h"
#include "IMainNode.h"

#define GLOBALIDNUMBER "\"1031\""
#define GLOBALIDNUMBERDEC 1031


#include "MainNodeSave.h"

class ArduinoMainNode:public IMainNode
{
    public:
        ArduinoMainNode();
        virtual ~ArduinoMainNode();
        void idStamp(uint16_t lastOrderId,uint8_t orderRes,uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize, uint8_t stampType );
        void getIdentity(uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize );
    protected:
    private:
};

#endif // ARDUINOMAINNODE_H
