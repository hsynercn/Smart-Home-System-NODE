#ifndef ESPCONNECTION_H
#define ESPCONNECTION_H


#include "ISerial.h"
#include "IPin.h"
#include "IGate.h"
#include "MiscUtility.h"

class EspConnection : public IGate
{
    public:
        EspConnection(IPin *espPowerPin,ISerial *espComSer);
        virtual ~EspConnection();
        uint8_t* receiveResponse(uint16_t , uint16_t*);

        void safeAdvancedRead(uint16_t timeOut, uint16_t *lastSize,uint8_t *buffer,uint16_t bufferSize );

        void sendAtCommand(uint8_t*, uint16_t);
        void endAtCommand();
        void enterAtCommand(String);

        uint8_t connectWifi(uint8_t*, uint16_t,uint8_t*, uint16_t);
        uint8_t connectWifiWithWait(uint8_t* id, uint16_t idLen ,uint8_t* pass, uint16_t passLen);

        uint8_t searchInSequence(uint8_t* , uint16_t  , uint8_t* , uint16_t );

        uint8_t cipStart();
        void cipStatus();

        void powerOn();
        void powerOff();

        void postHTTP( uint8_t* , uint16_t  );

        void postAndReceiveHTTP(uint8_t *payload , uint16_t payloadSize, uint16_t timeOut, uint16_t *lastSize,uint8_t *buffer,uint16_t bufferSize );

        uint8_t connectGate(uint8_t* id, uint16_t idLen ,uint8_t* pass, uint16_t passLen);
        uint8_t sendAndReceivePacket (uint8_t *payload , uint16_t payloadSize, uint16_t *lastBufferSize,uint8_t *buffer,uint16_t maxBufferSize);
        void disconnectGate();
        void setIpForServer(uint8_t *serverIp , uint16_t serverIpLength);

    protected:
    private:

        //void cipSend();
        void setMode(int mode);

        void uint16_tTouint8_t(uint16_t ,uint16_t* ,uint8_t* ,uint16_t );

        uint8_t mode;
        uint8_t powerSatus;
        uint8_t resetPin;
        IPin *espPowerPin;
        ISerial *espComSer;
        uint8_t serverIp[15];
        uint16_t serverIpLength;
};

#endif // ESPCONNECTION_H
