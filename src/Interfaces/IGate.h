#ifndef IGATE_H
#define IGATE_H

#include "stdint.h"
class IGate
{
    public:
        virtual uint8_t connectGate(uint8_t* id, uint16_t idLen ,uint8_t* pass, uint16_t passLen);
        virtual uint8_t sendAndReceivePacket (uint8_t *payload , uint16_t payloadSize, uint16_t *lastBufferSize,uint8_t *buffer,uint16_t maxBufferSize);
        virtual void disconnectGate();
        virtual void setIpForServer(uint8_t *serverIp , uint16_t serverIpLength);
    protected:
    private:
};

#endif // IGATE_H
