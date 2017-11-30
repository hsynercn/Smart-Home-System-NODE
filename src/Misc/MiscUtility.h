#ifndef MISCUTILITY_H
#define MISCUTILITY_H
#include "stdint.h"



void myDelay(uint32_t wait );
void readNode(int memoryLocation, struct MainNodeSave *nodeSave);
void writeNode(int memoryLocation, struct MainNodeSave *nodeSave);
uint8_t searchInSequence(uint8_t *sourceSeq, uint16_t sourceSize , uint8_t *searchSeq, uint16_t searchSize);

uint16_t readNextInt(uint16_t bufferStart , uint16_t bufferMax , uint8_t *buffer);
uint8_t readNextBoolean(uint16_t bufferStart , uint16_t bufferMax , uint8_t *buffer);
uint16_t utilUint8_tTouint16_t(uint8_t *buffer,uint16_t maxBufferSize );
uint16_t findInSequence(uint8_t *sourceSeq, uint16_t sourceSize , uint8_t *searchSeq, uint16_t searchSize);


//void utilUint16_tTouint8_t(uint16_t value,uint16_t* retSize,uint8_t* seqForm,uint16_t seqFormSize)
//uint16_t readNextString(uint16_t bufferStart , uint16_t bufferMax , uint8_t *buffer,);
#endif // MISCUTILITY_H
