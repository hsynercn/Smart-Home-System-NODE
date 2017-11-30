#ifndef ARDUINOSOFTWARESERIAL_H
#define ARDUINOSOFTWARESERIAL_H

//CONVERTED TO GENERIC TYPES
#include"ISerial.h"
#include "Arduino.h"
#include "SoftwareSerial.h"


class ArduinoSoftwareSerial: public ISerial
{
     public:
        /*ATTENTION: Arduino software constructor uses uint8_t, char type passed to constructor and converted to uint8_t
        Provides generalized varaibles in code but need a pre defined typedef list*/
        //TODO: Stabilize all varaible types to a general format
        ArduinoSoftwareSerial(uint8_t in, uint8_t out);
        ~ArduinoSoftwareSerial();

        int16_t available();
        uint8_t read();
        int32_t print(String);
        int32_t println(String);

        uint16_t safePrint(uint8_t*, uint16_t );
        uint16_t safeRead(uint8_t*, uint16_t );
        uint16_t safeAdvancedRead(uint8_t *pointer, uint16_t prmSize,uint8_t start,uint8_t stop);

        void begin(uint32_t baudRate);
        void flush(void);
    protected:
    private:
        SoftwareSerial mySerial;//pointer way didn't work this is good for now

};

#endif // ARDUINOSOFTWARESERIAL_H
