/*
IN USE:19.04.2016 23:45
LAST ORDER FEEDBACK
STATE INCLUDED
*/


#include <Arduino.h>
#include <ArduinoHardwareSerial.h>
#include <ArduinoSoftwareSerial.h>
#include <SoftwareSerial.h>
#include "IPin.h"
#include "ISerial.h"
#include "IMainNode.h"
#include "IControl.h"
#include "EspConnection.h"
#include "ArduinoIoPin.h"
#include "MyDHT11Sensor.h"
#include "MyMQ2Sensor.h"
#include "ArduinoMainNode.h"
#include "ArduinoBuzzer.h"
#include "MainNodeSave.h"


uint8_t introduceSensors();
uint8_t introduceControls();
uint8_t parseResponse(uint8_t *mssg , uint16_t mssgSize);

    ISerial* localTerminal= new ArduinoSoftwareSerial(8,9);
    ISerial* iSerial = new ArduinoHardwareSerial();
    IPin* iPin= new ArduinoIoPin(13,1);
    IGate* iGate = new EspConnection(iPin,iSerial);
    ISensor* tempHumSensor =new MyDHT11Sensor(12,1,2);
    ISensor* gasSensor =new MyMQ2Sensor(A5,3);
    IMainNode* iMainNode=new ArduinoMainNode();
    IControl* buzzer = new ArduinoBuzzer(5,1);


    uint8_t payload[150] = "";
    uint16_t maxPayloadSize =150;

    uint8_t responseBuffer[60]="";
    uint16_t responseSize=60;

    uint16_t lastResponseSize=0;
    uint16_t lastPayloadSize = 0;

    ISensor* allSensors[10];
    IControl* allControls[10];

    uint8_t  sensorCount=0;
    uint8_t  controlCount=0;

    uint16_t  lastOrderId=0;
    uint8_t  lastOrderState=0;

void setup()
{
    allSensors[0]=tempHumSensor;
    allSensors[1]=gasSensor;
    sensorCount=2;

    allControls[0] = buzzer;
    controlCount=1;

    localTerminal->begin(9600);
    iSerial->begin(9600);
}

void loop()
{

    localTerminal->println("BOOT-19/04/2016 23:29 INUSE");

    iSerial->println("T HARDSER");
    localTerminal->println("T SOFSER");

    struct MainNodeSave nodeSave;
    readNode(0, &nodeSave);


    uint8_t initResponded=0;
    uint8_t gateConnected=0;
    while( (gateConnected==0) && (initResponded==0) )
    {
        localTerminal->println("WIFI NOT CONNECTED");
        uint16_t read =0;
        uint8_t nextTry=0;
        while( (localTerminal->available()==0) && (nextTry<10) )
        {
            localTerminal->println("ID?");
            myDelay(1000);
            nextTry++;
        }

        if((localTerminal->available()!=0))
        {
            read= localTerminal->safeAdvancedRead(nodeSave.gateId, MAXSAVEPRMSIZE,'>','<');
            nodeSave.gateIdLen = read;
            localTerminal->print("->");
            localTerminal->safePrint(nodeSave.gateId, nodeSave.gateIdLen);
            localTerminal->print("<-");

            while(localTerminal->available()!=0)
                localTerminal->read();
        }

        nextTry=0;
        while( (localTerminal->available()==0) && (nextTry<10) )
        {
            localTerminal->println("PASS?");
            myDelay(1000);
            nextTry++;
        }
        if((localTerminal->available()!=0))
        {
            read= localTerminal->safeAdvancedRead(nodeSave.gatePass, MAXSAVEPRMSIZE,'>','<');
            nodeSave.gatePassLen = read;
            localTerminal->print("->");
            localTerminal->safePrint(nodeSave.gatePass, nodeSave.gatePassLen);
            localTerminal->print("<-");
            while(localTerminal->available()!=0)
                localTerminal->read();
        }

        nextTry=0;
        while( (localTerminal->available()==0) && (nextTry<10) )
        {
            localTerminal->println("IP?");
            myDelay(1000);
            nextTry++;
        }
        if((localTerminal->available()!=0))
        {
            read= localTerminal->safeAdvancedRead(nodeSave.serverIp, MAXSAVEPRMSIZE,'>','<');
            nodeSave.serverIpLen = read;
            localTerminal->print("->");
            localTerminal->safePrint(nodeSave.serverIp, nodeSave.serverIpLen);
            localTerminal->print("<-");

            while(localTerminal->available()!=0)
                localTerminal->read();
        }

        iGate->setIpForServer(nodeSave.serverIp, nodeSave.serverIpLen);
        gateConnected=iGate->connectGate(nodeSave.gateId, nodeSave.gateIdLen, nodeSave.gatePass, nodeSave.gatePassLen);

        payload[0] ='I';payload[1] ='P';payload[2] ='T';payload[3] ='E';payload[4] ='S';payload[5] ='T';
        uint16_t fullArea = 6;
        iGate->sendAndReceivePacket(payload,fullArea,&lastResponseSize,responseBuffer,responseSize);

        localTerminal->println("RESP:");
        localTerminal->safePrint(responseBuffer, lastResponseSize);
        localTerminal->println("");
        initResponded= parseResponse(responseBuffer, lastResponseSize);
        if(gateConnected==0)localTerminal->println("GATE NOT CONNEC");
        if(initResponded==0)localTerminal->println("NOT RESPED");
    }

    localTerminal->println("CONTED");
    writeNode(0, &nodeSave);

    introduceSensors();  //THIS ALINGMENT IS IMPORTANT FOR INIT ENDING
    introduceControls();


    uint16_t fullArea=0;
    uint8_t keepWorking=1;
    while(keepWorking)
    {
        fullArea=0;
        payload[fullArea] = '{';
        fullArea++;

        iMainNode->idStamp(lastOrderId,lastOrderState,payload+fullArea,maxPayloadSize,&lastPayloadSize,1);
        fullArea+=lastPayloadSize;

        memcpy(&payload[fullArea], "{\"s\":[", sizeof("{\"s\":["));
        fullArea += sizeof("{\"s\":[");

        for(uint8_t i=0;i<sensorCount;i++)
        {\
            allSensors[i]->getReport(payload+fullArea,maxPayloadSize,&lastPayloadSize);
            fullArea+=lastPayloadSize;
            if(i!=(sensorCount-1) )
            {
                payload[fullArea] = ',';
                fullArea++;
            }
        }

        memcpy(&payload[fullArea], "]},{\"c\":[", sizeof("]},{\"c\":["));
        fullArea += sizeof("]},{\"c\":[");

        uint16_t lastComma=0;
        for(uint8_t i=0;i<controlCount;i++)
        {
            allControls[i]->getStateReport(payload+fullArea,maxPayloadSize,&lastPayloadSize);
            fullArea+=lastPayloadSize;

            if(i!=(controlCount-1) )
            {
                payload[fullArea] = ',';
                fullArea++;
            }
        }


        memcpy(&payload[fullArea], "]}]}", sizeof("]}]}"));
        fullArea += sizeof("]}]}");

        localTerminal->safePrint(payload, fullArea);
        localTerminal->println("");

        iGate->sendAndReceivePacket(payload,fullArea,&lastResponseSize,responseBuffer,responseSize);
        myDelay(100);
        localTerminal->println("RESP:");
        localTerminal->safePrint(responseBuffer, lastResponseSize);
        localTerminal->println("");

        localTerminal->println("PARS:");
        parseResponse(responseBuffer, lastResponseSize);
        myDelay(900);
    }
}

uint8_t parseResponse(uint8_t *mssg , uint16_t mssgSize )
{

    //findInSequence(uint8_t *sourceSeq, uint16_t sourceSize , uint8_t *searchSeq, uint16_t searchSize);
    //responseBuffer,responseSize
    uint16_t processedPart = 0;//;lastResponseSize;
    uint16_t startPoint =0;
    uint16_t stopPoint =0;
    uint8_t responded = searchInSequence(mssg, mssgSize , (uint8_t*)"RECEIVED",8 );

    while( (processedPart<lastResponseSize) && (responseBuffer[processedPart]!='{'))
    {
        processedPart++;
    }
    if(responseBuffer[processedPart]!='{')
    {
            return 2;// MESSAGE RECEIVED, IT DOES NOT CONTAINS COMMAND
    }

    startPoint = processedPart;

    while( (processedPart<lastResponseSize) && (responseBuffer[processedPart]!='}'))
    {
        processedPart++;
    }
    if(responseBuffer[processedPart]!='}')
    {
            return 3;// MESSAGE RECEIVED, COMMAND DOES NOT END, FAULTY COMMAND
    }
    stopPoint = processedPart;
    uint16_t parameterSeek = startPoint;
    uint16_t parameters[3];
    uint16_t detecetdParameterCount=0;
    uint16_t expectedParameterCount = 3;

    while( (parameterSeek<stopPoint) && (detecetdParameterCount<expectedParameterCount)  )
    {
        while( ( (responseBuffer[parameterSeek]>'9') || (responseBuffer[parameterSeek]<'0') ) && (parameterSeek<stopPoint) )
        {
            parameterSeek++;
        }
        if(parameterSeek<stopPoint)
        {
            parameters[detecetdParameterCount] = 0 ;
            parameters[detecetdParameterCount] = utilUint8_tTouint16_t(&responseBuffer[parameterSeek],4 );//MAX PARAM LEN 4 CHAR

            detecetdParameterCount++;
            while( ( responseBuffer[parameterSeek]!=',' ) && (parameterSeek<stopPoint) )
            {
                parameterSeek++;
            }
        }
    }
    uint16_t OREDERID = parameters[0];
    uint16_t DEVICEID = parameters[1];
    uint16_t SUBDEVID = parameters[2];
    lastOrderId = OREDERID;
    while( (responseBuffer[parameterSeek]!=',') && (parameterSeek<stopPoint) )
    {
        parameterSeek++;
    }

    if(parameterSeek<stopPoint)
    {
        if(GLOBALIDNUMBERDEC == DEVICEID)
        {
            localTerminal->println("MIN");
            if( (SUBDEVID-1)<controlCount)
                localTerminal->println("VAL EXE:");
                lastOrderState = allControls[SUBDEVID-1]->acceptCommand(&responseBuffer[parameterSeek],stopPoint-parameterSeek);
                return 4;
        }
        else
        {
            localTerminal->println("FUC");
        }
    }
    return responded;
}

uint8_t introduceSensors()
{
    uint16_t fullArea=0;
    uint8_t responded =0;

    for(uint8_t i=0;i<sensorCount;i++)
    {
        fullArea=0;
        payload[fullArea] = '{';
        fullArea++;

        iMainNode->idStamp(lastOrderId,lastOrderState,payload+fullArea,maxPayloadSize,&lastPayloadSize,0);//INIT type packets

        if(i!=(sensorCount-1) )
            iMainNode->idStamp(lastOrderId,lastOrderState,payload+fullArea,maxPayloadSize,&lastPayloadSize,0);//INIT type packets
        else
            if(controlCount==0)
                iMainNode->idStamp(lastOrderId,lastOrderState,payload+fullArea,maxPayloadSize,&lastPayloadSize,2);//INITEND type packets
            else
                iMainNode->idStamp(lastOrderId,lastOrderState,payload+fullArea,maxPayloadSize,&lastPayloadSize,0);//INIT type packets

        fullArea+=lastPayloadSize;


        if(allSensors[i]->getIdentity(payload+fullArea,maxPayloadSize,&lastPayloadSize) !=0)
        {
            i--;
        }
        fullArea+=lastPayloadSize;

        payload[fullArea] = ']';
        fullArea++;

        payload[fullArea] = '}';
        fullArea++;

        localTerminal->safePrint(payload, fullArea);
        localTerminal->println("");

        iGate->sendAndReceivePacket(payload,fullArea,&lastResponseSize,responseBuffer,responseSize);
        myDelay(100);

        localTerminal->println("RESP:");
        localTerminal->safePrint(responseBuffer, lastResponseSize);
        localTerminal->println("");


        responded = parseResponse(responseBuffer, lastResponseSize);
        if(responded==0)
        {
            localTerminal->println("NOT RESP");
        }

        while(responded==0)
        {
            localTerminal->safePrint(payload, fullArea);
            iGate->sendAndReceivePacket(payload,fullArea,&lastResponseSize,responseBuffer,responseSize);
            myDelay(100);

            localTerminal->println("RESP:");
            localTerminal->safePrint(responseBuffer, lastResponseSize);
            localTerminal->println("");

            responded = parseResponse(responseBuffer, lastResponseSize);
            if(responded==0)
            {
                localTerminal->println("NOT RESP");
            }
        }
    }
    return 1;
}

uint8_t introduceControls()
{
    uint16_t fullArea=0;
    uint8_t responded =0;

    for(uint8_t i=0;i<controlCount;i++)
    {
        fullArea=0;
        payload[fullArea] = '{';
        fullArea++;

        if(i!=(controlCount-1) )
            iMainNode->idStamp(lastOrderId,lastOrderState,payload+fullArea,maxPayloadSize,&lastPayloadSize,0);//INIT type packets
        else
            iMainNode->idStamp(lastOrderId,lastOrderState,payload+fullArea,maxPayloadSize,&lastPayloadSize,2);//INIT type packets

        fullArea+=lastPayloadSize;

        if(allControls[i]->getIdentity(payload+fullArea,maxPayloadSize,&lastPayloadSize) !=0)
        {
            i--;
        }
        fullArea+=lastPayloadSize;

        payload[fullArea] = ']';
        fullArea++;

        payload[fullArea] = '}';
        fullArea++;

        localTerminal->safePrint(payload, fullArea);
        localTerminal->println("");

        iGate->sendAndReceivePacket(payload,fullArea,&lastResponseSize,responseBuffer,responseSize);
        myDelay(100);


        localTerminal->println("RESP:");
        localTerminal->safePrint(responseBuffer, lastResponseSize);
        localTerminal->println("");

        responded = parseResponse(responseBuffer, lastResponseSize);
        if(responded==0)
            {
                localTerminal->println("NOT RESP");
            }

        while(responded==0)
        {
            iGate->sendAndReceivePacket(payload,fullArea,&lastResponseSize,responseBuffer,responseSize);
            myDelay(100);
            localTerminal->println("RESP:");
            localTerminal->safePrint(responseBuffer, lastResponseSize);
            localTerminal->println("");
            responded = parseResponse(responseBuffer, lastResponseSize);
            if(responded==0)
            {
                localTerminal->println("NOT RESP");
            }
        }
    }
    return 1;
}


