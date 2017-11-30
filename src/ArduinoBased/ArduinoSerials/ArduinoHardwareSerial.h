#ifndef ARDUINOHARDWARESERIAL_H
#define ARDUINOHARDWARESERIAL_H
class String;
#include "ISerial.h"
#include "Arduino.h"

//CONVERTED TO GENERIC TYPES

class ArduinoHardwareSerial : public ISerial
{
    public:
        ArduinoHardwareSerial();
        ~ArduinoHardwareSerial();

        int16_t available();
        uint8_t read();
        int32_t print(String);
        int32_t println(String  );

        uint16_t safePrint(uint8_t*, uint16_t );
        uint16_t safeRead(uint8_t*, uint16_t );
        uint16_t safeAdvancedRead(uint8_t*, uint16_t ,uint8_t ,uint8_t  );


        void begin(uint32_t baudRate);
        void flush(void);
    protected:
    private:

};

#endif // ARDUINOHARDWARESERIAL_H
