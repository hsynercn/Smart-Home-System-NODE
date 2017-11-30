#ifndef MAINNODESAVE_H_INCLUDED
#define MAINNODESAVE_H_INCLUDED
#define MAXSAVEPRMSIZE 30
#define MAXIPSIZE 15
struct MainNodeSave
{
   uint8_t  gateId[MAXSAVEPRMSIZE];
   uint16_t gateIdLen;
   uint8_t  gatePass[MAXSAVEPRMSIZE];
   uint16_t gatePassLen;
   uint8_t  serverIp[MAXSAVEPRMSIZE];
   uint16_t serverIpLen;
};

#endif // MAINNODESAVE_H_INCLUDED
