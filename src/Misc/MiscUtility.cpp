#include "MiscUtility.h"
#include "Arduino.h"
#include "EEPROM.h"
#include "MainNodeSave.h"
void myDelay(uint32_t wait)
{
    delay(wait);
}

void readNode(int memoryLocation, struct MainNodeSave *nodeSave)
{
    char buffer[sizeof(MainNodeSave)];

    EEPROMClass EEPROM;

    int i=0;
    for(i=memoryLocation;i<sizeof(MainNodeSave);i++)
    {
        buffer[i] = EEPROM.read(i);

    }
    memcpy ( nodeSave, &buffer[0],  sizeof(MainNodeSave) );

    if( nodeSave->gateIdLen > MAXSAVEPRMSIZE )
        nodeSave->gateIdLen = MAXSAVEPRMSIZE;

    if( nodeSave->gatePassLen > MAXSAVEPRMSIZE )
        nodeSave->gatePassLen = MAXSAVEPRMSIZE;

    if( nodeSave->serverIpLen > MAXSAVEPRMSIZE )
    nodeSave->serverIpLen = MAXSAVEPRMSIZE;
}

void writeNode(int memoryLocation, struct MainNodeSave *nodeSave)
{
    char buffer[sizeof(MainNodeSave)];
    EEPROMClass EEPROM;

    memcpy ( &buffer[0], nodeSave,  sizeof(MainNodeSave) );
    int i=0;
    for(i=memoryLocation;i<sizeof(MainNodeSave);i++)
    {
         EEPROM.write(i,buffer[i]);
    }
}

uint8_t searchInSequence(uint8_t *sourceSeq, uint16_t sourceSize , uint8_t *searchSeq, uint16_t searchSize)
{
    uint16_t i = 0;
    uint16_t j = 0;
    uint8_t found = 0;

    while( (i<sourceSize) && (found == 0) )
    {
        found = 1;
        j=0;
        while(j<searchSize)
        {
            if( searchSeq[j] != sourceSeq[i+j] )
            {
                found=0;
            }
            j++;
        }
        i++;
    }
    return found;
}

void utilUint16_tTouint8_t(uint16_t value,uint16_t* retSize,uint8_t* seqForm,uint16_t seqFormSize)
{
    uint16_t divider = 1;
    uint8_t i = 0;
    uint8_t j = 0;
    while( (value/divider) != 0 )
    {
        divider*=10;
        j++;
    }
    divider/=10;
    while( (j != 0) && (i<seqFormSize) )
    {
        seqForm[i] = '0'+value/divider;
        value = value%divider;
        divider/=10;
        i++;
        j--;
    }
    *retSize = i;
}

uint16_t utilUint8_tTouint16_t(uint8_t *buffer,uint16_t maxBufferSize )
{

    uint16_t cursor = 0;
    uint16_t number = 0;
    while( (maxBufferSize>cursor) && ((buffer[cursor]>='0') && (buffer[cursor]<='9')) )
    {
        number=number*10+(buffer[cursor]-'0');
        cursor++;
    }
    return number;

}
uint16_t findInSequence(uint8_t *sourceSeq, uint16_t sourceSize , uint8_t *searchSeq, uint16_t searchSize)
{
    uint16_t i;
    uint16_t j;
    uint8_t found = 0;

    while( (i<sourceSize) && (found == 0) )
    {
        found = 1;
        j=0;
        while( (j<searchSize) && (found == 1) )
        {
            if( searchSeq[j] != sourceSeq[i+j] )
            {
                found=0;
            }
            j++;
        }
        i++;
    }
    return i-1;
}

