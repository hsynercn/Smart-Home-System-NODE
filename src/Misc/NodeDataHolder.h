#ifndef NODEDATAHOLDER_H_INCLUDED
#define NODEDATAHOLDER_H_INCLUDED

#define MAX

#include "stdint.h"
struct MyStruct
{
    uint8_t buffer;
    uint16_t maxBufferSize;
    uint16_t *lastBufferSize;

};


#endif // NODEDATAHOLDER_H_INCLUDED
