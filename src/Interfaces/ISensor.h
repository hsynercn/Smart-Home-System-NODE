#ifndef ISENSOR_H
#define ISENSOR_H


class ISensor
{
    public:
        virtual void getReport(uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize);
        virtual uint8_t getIdentity(uint8_t *buffer,uint16_t maxBufferSize, uint16_t *lastBufferSize );
    protected:
    private:
};

#endif // ISENSOR_H
