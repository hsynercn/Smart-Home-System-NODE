#include "EspConnection.h"

#define BUFSIZE 100
#define ESPWAITTIME 90
#define ESPWAITTIMEWIFI 4000
#define ESPCOMWAITQUANTUM 90
#define WIFICONTYRTIME 5
#define PACKETRESPONSEWAITTIME 5


uint8_t response[BUFSIZE]  = "RECEIVED:";

EspConnection::EspConnection(IPin *espPowerPin, ISerial *espComSer )
{
    this->espPowerPin = espPowerPin;
    this->espComSer = espComSer;
    this->espComSer->begin(9600);

}

EspConnection::~EspConnection()
{
    //dtor
}

uint8_t* EspConnection::receiveResponse(uint16_t timeOut, uint16_t *size)
{
    uint16_t wait=0;
    while((!this->espComSer->available())&&(wait<timeOut))
    {
        wait++;
        myDelay(ESPCOMWAITQUANTUM);
    }

    uint16_t byteCount = this->espComSer->safeRead(response,BUFSIZE);
    *size = byteCount;
    return response;
}

void EspConnection::safeAdvancedRead(uint16_t timeOut, uint16_t *lastSize,uint8_t *buffer,uint16_t bufferSize )
{
    //mySerial.println("DEBUG PRINT:safeAdvancedRead called");
    uint16_t wait=0;
    while((this->espComSer->available()==0)&&(wait<(timeOut*10)))
    {
        //mySerial.println("DEBUG PRINT:safeAdvancedRead waiting");
        wait++;
        myDelay(10);
    }


    uint16_t byteCount = this->espComSer->safeAdvancedRead(buffer,bufferSize,'#','$'); //NO EXTERNAL BUFFER USED!!!
    //mySerial.println("DEBUG PRINT");
    //mySerial.println((char*)buffer);
    //mySerial.println("DEBUG PRINT END");
    *lastSize = byteCount;

}


void EspConnection::sendAtCommand(uint8_t *pointer, uint16_t prmSize)
{
    this->espComSer->safePrint(pointer, prmSize);
}

void EspConnection::endAtCommand()
{
    uint8_t enter[2] = {'\r', '\n'};
    this->espComSer->safePrint(enter, 2);
}

void EspConnection::powerOn()
{

    this->espPowerPin->setDigitalWrite(1);
}
void EspConnection::powerOff()
{
    this->espPowerPin->setDigitalWrite(0);

}

uint8_t EspConnection::connectWifi(uint8_t* id, uint16_t idLen ,uint8_t* pass, uint16_t passLen)
{


    uint8_t comma[1] = {','};
    uint8_t quotation[2] = "\"";

    this->sendAtCommand( (uint8_t*)"AT+CWJAP=", 9 );

    this->sendAtCommand(quotation, 1);
    this->sendAtCommand( id , idLen );
    this->sendAtCommand(quotation, 1);


    this->sendAtCommand(comma, 1);

    this->sendAtCommand(quotation, 1);
    this->sendAtCommand( pass , passLen );
    this->sendAtCommand(quotation, 1);

    this->endAtCommand();

    myDelay(ESPWAITTIMEWIFI);

    uint16_t byteCount = 0;
    this->receiveResponse(30,&byteCount);

    if( this->searchInSequence(response,byteCount,(uint8_t*)"OK",2)==1 )
    {
        return 1;
    }
    else
        return 0;
}

uint8_t EspConnection::connectWifiWithWait(uint8_t* id, uint16_t idLen ,uint8_t* pass, uint16_t passLen)
{
    uint8_t wait = 0;
    while(wait<WIFICONTYRTIME)
    {
        if(this->connectWifi(id, idLen, pass , passLen)==1)
        {
            return 1;
        }
        myDelay(ESPWAITTIMEWIFI);
        wait++;
    }
    return 0;
}

uint8_t EspConnection::searchInSequence(uint8_t *sourceSeq, uint16_t sourceSize , uint8_t *searchSeq, uint16_t searchSize)
{
    uint16_t i;
    uint16_t j;
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

    if(found!=0)
        return 1;
    return 0;
}

uint8_t EspConnection::cipStart()
{
    //mySerial.println("DEBUG PRINT:cipStart called");
    uint16_t byteCount = 0;
    this->cipStatus();
    //myDelay(500);
    myDelay(ESPWAITTIME);
    this->receiveResponse(30,&byteCount);

    //if( this->searchInSequence(response,byteCount,(uint8_t*)"192.168.2.144",13)==1 )
    if( this->searchInSequence( response , byteCount , this->serverIp , this->serverIpLength )==1 )
        return 1;//connected

    this->sendAtCommand( (uint8_t*)"AT+CIPSTART=\"TCP\",\"" , 19 );
    this->sendAtCommand( this->serverIp , this->serverIpLength );
    this->sendAtCommand( (uint8_t*)"\",16384" , 7 );
    this->endAtCommand();

    //myDelay(500);
    myDelay(ESPWAITTIME+100); //CIPSTART TAKES LONGER THAN NORMAL COMMANDS
    this->receiveResponse(60,&byteCount);

    //myDelay(500);
    myDelay(ESPWAITTIME);
    this->cipStatus();

    this->receiveResponse(30,&byteCount);

    if( this->searchInSequence(response,byteCount, this->serverIp , this->serverIpLength)==1 )
        return 1;//connected*/
    return 0;//did not connect
}

void EspConnection::cipStatus()
{
    uint8_t cipSts[13] = "AT+CIPSTATUS";
    this->sendAtCommand( cipSts, 12 );
    this->endAtCommand();

 }

void EspConnection::postHTTP( uint8_t *payload , uint16_t payloadSize )
{

    //mySerial.println("DEBUG PRINT:postHTTP called");
    if(payloadSize == 0){return;}
    uint16_t byteCount = 0;

    uint8_t httpHeaderPart1[13] = "POST http://";
    uint16_t httpHeaderLen1 = 12;
    uint8_t httpHeaderPart2[16] = ":16384/HTTP/1.0";
    uint16_t httpHeaderLen2 = 15;

    uint8_t contentLength[16] = "Content-Length:";
    uint16_t contentLengthHeaderLen = 15;
    uint16_t payloadLen = payloadSize;
    uint8_t payloadForm[5];
    uint16_t payloadFormSize =5;
    uint16_t payloadRetSize = 0;
    this->uint16_tTouint8_t( payloadLen,&payloadRetSize,payloadForm, payloadFormSize);

    uint16_t requestLen = payloadSize + httpHeaderLen1 + httpHeaderLen2 + payloadRetSize + contentLengthHeaderLen + 8 +this->serverIpLength ;


    uint8_t reqForm[5];
    uint16_t reqFormSize =5;
    uint16_t reqRetSize = 0;
    this->uint16_tTouint8_t( requestLen,&reqRetSize,reqForm, reqFormSize);

    /*mySerial.println("PRINTLN");
    mySerial.print((char*)reqForm[0]);
    mySerial.print((char*)reqForm[1]);
    mySerial.print((char*)reqForm[2]);
    mySerial.print((char*)reqForm[3]);
    mySerial.print((char*)reqForm[4]);
    mySerial.println("PRINTLN");*/

    uint8_t cipSend[12] = "AT+CIPSEND=";

    this->sendAtCommand( cipSend, 11 );
    this->sendAtCommand(reqForm,reqRetSize);
    this->endAtCommand();
    myDelay(ESPWAITTIME);
    this->receiveResponse(60,&byteCount);

    uint8_t wait =0;
    while( (this->searchInSequence(response,byteCount,(uint8_t*)">",1)!=1 ) && (wait<(PACKETRESPONSEWAITTIME*10)) )
    {
        myDelay(10);
        wait++;
    }
    while(this->espComSer->available() > 0)
    {
        this->espComSer->read();
    }

    this->sendAtCommand(httpHeaderPart1,httpHeaderLen1);
    this->sendAtCommand( this->serverIp , this->serverIpLength );
    this->sendAtCommand(httpHeaderPart2,httpHeaderLen2);
    this->endAtCommand();//  COMMAND ENDING

    this->sendAtCommand(contentLength,contentLengthHeaderLen);
    this->sendAtCommand(payloadForm,payloadRetSize);
    this->endAtCommand();//  COMMAND ENDING
    this->endAtCommand();//  COMMAND ENDING

    this->sendAtCommand( payload, payloadSize );
    this->endAtCommand();//  COMMAND ENDING

}

void EspConnection::postAndReceiveHTTP( uint8_t *payload , uint16_t payloadSize, uint16_t timeOut, uint16_t *lastSize, uint8_t *buffer, uint16_t bufferSize )
{

    this->cipStart();
    this->postHTTP( payload , payloadSize );
    this->safeAdvancedRead(timeOut, lastSize,buffer,bufferSize );
}

uint8_t EspConnection::connectGate(uint8_t* id, uint16_t idLen ,uint8_t* pass, uint16_t passLen)
{
    this->powerOn();
    this->powerOff();
    this->powerOn();
    myDelay(ESPWAITTIME);
    this->sendAtCommand((uint8_t*)"AT+CWMODE=1",11);
    this->endAtCommand();
    myDelay(ESPWAITTIME);
    return this->connectWifiWithWait(id,  idLen , pass,  passLen);
}
uint8_t EspConnection::sendAndReceivePacket(uint8_t *payload , uint16_t payloadSize, uint16_t *lastBufferSize,uint8_t *buffer,uint16_t maxBufferSize)
{
    this->postAndReceiveHTTP( payload ,  payloadSize,  PACKETRESPONSEWAITTIME, lastBufferSize, buffer, maxBufferSize );
    if(*lastBufferSize!=0)
        return 1;
    return 0;
}

void EspConnection::disconnectGate()
{
    this->powerOff();
}

void EspConnection::setIpForServer(uint8_t *serverIp , uint16_t serverIpLength)
{
    for(uint16_t i=0;i<serverIpLength;i++)
    {
        this->serverIp[i] = serverIp[i];
    }
    this->serverIpLength = serverIpLength;
}


void EspConnection::uint16_tTouint8_t(uint16_t value,uint16_t* retSize,uint8_t* seqForm,uint16_t seqFormSize)
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


