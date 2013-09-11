                                                                   //****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                 CEURON                     *//
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : WizFi.h                                 *//
//*  Opis                         : Obs³uga modu³u WizFi                    *//
//*  Autor                        : Pawe³ Narwojsz                           *//
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

#define VAR_SET_AMOUNT        20   //maksymalna iloœc zmiennych w zestawie do odpytania
#define WiFi_TIMEOUT          10000 //[ms] timeout wifi 
#define WiFi_CONNECT_PERIOD   500 //[ms] czas pomiêdzy standardowym odpytywaniem hosta 


#include "USART.h"



//Sta³e ------------------------------------------------------------------------
    const char AT[]="AT\r";                                                     //komenda WizFi 
    const char AT_WD[]="AT+WD\r";                                               //komenda WizFi 
    const char AT_NDHCP_1[]="AT+NDHCP=1\r";                                     //komenda WizFi   
    const char AT_WWEP1[]="AT+WWEP1=";                                          //komenda WizFi  
    const char AT_WWPA[]="AT+WWPA=";                                            //komenda WizFi  
    const char AT_WA[]="AT+WA=";                                              //komenda WizFi  
    const char AT_DNSLOOKUP[]="AT+DNSLOOKUP=";                                              //komenda WizFi  
    const char AT_NCTCP[]="AT+NCTCP=";                                              //komenda WizFi  
    const char AT_NCLOSEALL[]="AT+NCLOSEALL\r";                                              //komenda WizFi  
    const char AT_WS[]="AT+WS\r";
    
    

//____________________________Koniec sta³ych ___________________________________



typedef enum {
      WizFi_NC=0,
      WizFi_AT,
      WizFi_AT_WD,
      WizFi_AT__REP,  //powtórzenie AT
      WizFi_AT_NDHCP_1,
      WizFi_AT_WWEP1,
      WizFi_AT_WWPA,
      WizFi_AT_WA,
      WizFi_AT_DNSLOOKUP,
      WizFi_AT_NCTCP,
      WizFi_AT_NCLOSEALL,
      WizFi_AT_WS,
      WizFi_SendData,
    }_WiFiCommand;

typedef enum {
      WizFi_AT_NC=0,
      WizFi_AT_CONFIG,            //konfiguracja
      WizFi_AT_CONFIG_ERROR,      //b³ad konfiguracji
      WizFi_AT_CONNECTING,           //³¹czenie z hotspotem
      WizFi_AT_CONNECT_ERROR,     //b³¹d po³¹czenia z hotspotem
      WizFi_AT_CONNECTED,         //po³¹czono z hotspotem
    }_WiFiStat;






typedef enum {
              HTTP_NO_INIT=0,
              HTTP_NULL,                //brak rozkazów do wykoania
              HTTP_POST,                //rozkaz wys³ania danych do putgate.php
              HTTP_GET,                 //rozkaz poprania danych z  dc_xxxxxxx.csv
              HTTP_GET_Config,          //rozkaz pobrania konfiguracji zmiennych z datain_xxxxxxx.csv  (s³u¿acych potam jako bazo do HTML_POST)
    }_HTTP_Order_Type;                                                                                                                                            



//stan po³¹czenia z serwerem przez protokó³ HTTP
typedef enum {
            HTTP_HOST_DISCONNECT=0,        //nie po³¹czono / roz³aczono
            HTTP_ERROR_CONNECT,        //b³¹d przy po³¹czeniu z hostem
            HTTP_HOST_CONNECTING,            //³¹czenie z hostem
            HTTP_HOST_CONNECTED,             //po³aczony z hostem
            HTTP_SENDING_QUERY,
    }_HTTP_Status;
typedef enum {
              NO_Var=0,
              a='a',
              b='b',
              c='c',
              d='d',
            }_VarType;
              

              
//opis zmiennej
typedef struct{
                _VarType      VarType;    //typ zmeinnej
                char          VarNr;      //numer zmiennej
                short         Value;
              }_VarSet;

typedef struct{
                  _HTTP_Order_Type    OrderType;
                  _HTTP_Status        Status;
                  char                WaitingPost;
                  char                VarSetCount;
                  short int           LastForseOrderNr;       
                  _VarSet             VarSet[VAR_SET_AMOUNT];
              }_HTTP;


typedef struct{
                 char  Name[23];
                 char  Signal;    //moc sygna³u
               }_SSID;


typedef struct{
                _WiFiStat         Status;
                char              Ready;          //gotowy do sta³ej komunikacji
                char              SSID_Check;     //numer kroku szukania accespointów
                _WiFiCommand      Command;
                char              DomenaIP[15];
                char              HostChannel;                
                
              }_WiFiStatus;
                
                

              


    
    void InitWizFi(AT91PS_USART USART);
    void WizFiResponseInterpreter(char* ReciveBuff, char *SendBuff,  AT91PS_USART USART);
    void GetHTTP_Force(char *SendBuff,  AT91PS_USART USART);
    void CheckVarChangeHTTP(char *SendBuff,  AT91PS_USART USART);
    void ConnectWithHost(char *SendBuff,  AT91PS_USART USART);
    void PostHTTP_Force(char *SendBuff,  AT91PS_USART USART);
   void SendHttpQwery(char *SendBuff,  AT91PS_USART USART);
    
    
   

#endif
      