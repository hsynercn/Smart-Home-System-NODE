#ifndef IMAINNODE_H
#define IMAINNODE_H
#include "MainNodeSave.h"

class IMainNode
{
    public:
        virtual void idStamp(uint16_t lastOrderId,uint8_t orderRes, uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize, uint8_t stampType  );
        virtual void getIdentity(uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize );
    protected:
    private:
};

#endif // IMAINNODE_H
