#include "ArduinoMainNode.h"
#include "Arduino.h"
#include <avr/pgmspace.h>


ArduinoMainNode::ArduinoMainNode( )
{

}

ArduinoMainNode::~ArduinoMainNode()
{

}


void ArduinoMainNode::idStamp(uint16_t lastOrderId,uint8_t orderRes,uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize, uint8_t stampType )
{
    if(stampType==0)
        *lastBufferSize = snprintf_P((char*)buffer,maxBufferSize, PSTR("\"oId\":%d,\"oRs\":%d,\"mainId\":%s,\"mssgTyp\":\"init\",\"data\":["),lastOrderId,orderRes,GLOBALIDNUMBER);
    if(stampType==1)
        *lastBufferSize = snprintf_P((char*)buffer,maxBufferSize, PSTR("\"oId\":%d,\"oRs\":%d,\"mainId\":%s,\"mssgTyp\":\"reg\",\"data\":["),lastOrderId,orderRes,GLOBALIDNUMBER);
    if(stampType==2)
        *lastBufferSize = snprintf_P((char*)buffer,maxBufferSize, PSTR("\"oId\":%d,\"oRs\":%d,\"mainId\":%s,\"mssgTyp\":\"initEnd\",\"data\":["),lastOrderId,orderRes,GLOBALIDNUMBER);

}
void ArduinoMainNode::getIdentity(uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize )
{
    *lastBufferSize = snprintf_P((char*)buffer,maxBufferSize, PSTR("{\"deviceType\":\"MainNode\",\"mainId\":\"%s\""),GLOBALIDNUMBER);
}


