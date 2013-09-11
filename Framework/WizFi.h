                                                                   //****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                 CEURON                     *//
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : WizFi.h                                 *//
//*  Opis                         : Obs�uga modu�u WizFi                    *//
//*  Autor                        : Pawe� Narwojsz                           *//
//*  Data utrzorzenia             : 31.01.2012                               *//
//*  Data ostatniej modyfikacji   : --.--.----                               *//
//*--------------------------------------------------------------------------*//
//*                                                                          *//
//*--------------------------------------------------------------------------*//
//* Rejestr zmian  (data - opis):                                            *//
//*                                                                          *//
//*                               Brak zmian                                 *//
//*                                                                          *//
//*__________________________________________________________________________*//
//****************************************************************************//





#ifndef __WIZFI__
#define __WIZFI__



      #define  CRYTICAL_TIMEOUT      10000          //[ms] Timeout krytyczny 
      #define  AP_CONNECT_TIMEOUT    20000          //[ms] Timeout krytyczny przy ��czeniu z acces pointem
      #define  HTTP_QUERY_MAX_TIME   5000           //[ms] Maksymalny czas oczekiwania na odpowied� hosta na zapytanie HTTP
      #define  FREEZ_TIME            50            //[ms] Czas wstrzymania , rezerwowanie czsu. W tym czasie �adna nowa komenda nie zostanie wys�ana
      #define  VAR_SET_AMOUNT        40             //maksymalna ilo�c zmiennych w zestawie do odpytania


#include "USART.h"






typedef enum {
      WizFi_NC=0,
      WizFi_AT,
      WizFi_AT_VER,
      WizFi_ATI2,
      WizFi_AT_XDUM0,
      WizFi_AT_WD,
      WizFi_AT_NSET,
      WizFi_AT_NDHCP,
      WizFi_AT_WAUTH_1,
      WizFi_AT_WWEP1,
      WizFi_AT_WWPA,
      WizFi_AT_WA,
      WizFi_AT_DNSLOOKUP,
      WizFi_AT_NCTCP,
      WizFi_AT_NCLOSE,
      WizFi_AT_NCLOSEALL,
      WizFi_AT_WS,
      WizFi_AT_CID,
      HTTP_POST,                //rozkaz wys�ania danych do putgate.php
      HTTP_GET,                 //rozkaz poprania danych z  dc_xxxxxxx.csv
      HTTP_GET_Config,          //rozkaz pobrania konfiguracji zmiennych z datain_xxxxxxx.csv  (s�u�acych potam jako bazo do HTML_POST)
    }_WiFiCommand;



//stan po��czenia
typedef enum {
            DISCONNECTED=0,          //nie po��czono / roz�aczono
            DISCONNECTING,            //trwa roz��czanie
            CONNECTING,            //trwa ��czenie 
            CONNECTED,             //po�aczony 
    }_Connect_Status;


typedef enum {
              NO_Var=0,
              a='a',
              b='b',
              c='c',
              d='d',
              i='i',
              o='q',
              s='r',
            }_NetDataType;
              

              
//opis zmiennej/wej�cia/wyj�cia
#pragma pack(1) 
typedef struct{
                _NetDataType      NetDataType;    //typ zmeinnej
                union{
                      char          NodeNr;      //numer zmiennej lub wez�a
                      char          VarNr;      //numer zmiennej lub wez�a
                     };
                union{
                      char          PortNr;       //numer portu
                      char          RegNr;        //numer rejestru
                     };
                short         Value;
              }_NetNodeSet;
#pragma pack() 


/*
typedef struct{
                _VarType      VarType;    //typ zmeinnej
                char          VarNr;      //numer zmiennej
                short         Value;
              }_VarSet;
*/

typedef struct{
                  char                VarSetCount;
                  short int           LastForseOrderNr;       
                  _NetNodeSet         NetNodeSet[VAR_SET_AMOUNT];
              }_HTTP;


typedef struct{
                 char  Name[23];
                 char  Signal;    //moc sygna�u
               }_SSID;




typedef struct{
                _Connect_Status       AccesPointAssociate;                      //stan po��czenia modu�u z punktem dost�powym
                _Connect_Status       HostLink;                                 //stan po��czenia modu�u z hostem
                _WiFiCommand          CurrentOrder;                             //bierz�cy rozkaz
                _WiFiCommand          NextHttpOrder;                            //nast�pny rozkaz HTTP (oczekuj�cy)
                char                  AccesPointAssociateTest;                  //test po��czenia ( 0xFF - po��czenie) przy testach nale�y zerowa� 
                char                  RepeatOrder;                              //ileo�� powt�rze� rozkaz�w
                char                  DomenaIP[15];
                char                  Error;                                    //>1 b��d 
                char                  HostChannel; 
                short                 SendBytes;                                //ilo�c wys�anych bajt�w w ostatnim pakiecie
                short                 ResBytes;                                 //ilo�c odebranych bajt�w w ostatnim pakiecie
                unsigned long long    SendPacketTime;                           //(dla Sie� INFO) czas ostatnio wys�anego rozkazu
                unsigned long long    ResPacketTime;                            //(dla Sie� INFO) czas ostatnio odebranej odpowiedzi
                unsigned long long    HttpNextQueryTime;                        //[ms] czas nast�pnego po��czenia HTTP (skoja�one z g_counter)
                unsigned long long    Timeout;                                  //[ms] czas timeout'u  (skoja�one z g_counter)
                unsigned long long    FreezTime;                                //[ms] czas zamro�enia dza�ania  dla bezpiecznego dzia�ania serwera
                char                  EmitFrame;
              }_WiFiStatus;
                

                
typedef struct{
                _WiFiStatus   Status;
                _WiFi         WiFi;
                _HTTP         HTTP;
              }_Net;
              


    
    void InitWizFi(AT91PS_USART USART, _Net *Net);
    void WiFi(char *Buff_RX, char *Buff_TX,  AT91PS_USART USART, _Net *Net);
    void AP_Associate(char *Buff_RX, char *Buff_TX,  AT91PS_USART USART, _Net *Net);
    void AP_AssociateResponseInterpreter(char *Buff_RX, char *Buff_TX,  AT91PS_USART USART, _Net *Net);
    void HTTP_Task(char *Buff_RX, char *Buff_TX,  AT91PS_USART USART, _Net *Net);
    void CheckVarChangeHTTP(char *SendBuff,  AT91PS_USART USART, _Net *Net);
    
#endif
      
 