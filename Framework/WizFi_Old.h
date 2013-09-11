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

#define VAR_SET_AMOUNT        20   //maksymalna ilo�c zmiennych w zestawie do odpytania
#define WiFi_TIMEOUT          10000 //[ms] timeout wifi 
#define WiFi_CONNECT_PERIOD   500 //[ms] czas pomi�dzy standardowym odpytywaniem hosta 


#include "USART.h"



//Sta�e ------------------------------------------------------------------------
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
    
    

//____________________________Koniec sta�ych ___________________________________



typedef enum {
      WizFi_NC=0,
      WizFi_AT,
      WizFi_AT_WD,
      WizFi_AT__REP,  //powt�rzenie AT
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
      WizFi_AT_CONFIG_ERROR,      //b�ad konfiguracji
      WizFi_AT_CONNECTING,           //��czenie z hotspotem
      WizFi_AT_CONNECT_ERROR,     //b��d po��czenia z hotspotem
      WizFi_AT_CONNECTED,         //po��czono z hotspotem
    }_WiFiStat;






typedef enum {
              HTTP_NO_INIT=0,
              HTTP_NULL,                //brak rozkaz�w do wykoania
              HTTP_POST,                //rozkaz wys�ania danych do putgate.php
              HTTP_GET,                 //rozkaz poprania danych z  dc_xxxxxxx.csv
              HTTP_GET_Config,          //rozkaz pobrania konfiguracji zmiennych z datain_xxxxxxx.csv  (s�u�acych potam jako bazo do HTML_POST)
    }_HTTP_Order_Type;                                                                                                                                            



//stan po��czenia z serwerem przez protok� HTTP
typedef enum {
            HTTP_HOST_DISCONNECT=0,        //nie po��czono / roz�aczono
            HTTP_ERROR_CONNECT,        //b��d przy po��czeniu z hostem
            HTTP_HOST_CONNECTING,            //��czenie z hostem
            HTTP_HOST_CONNECTED,             //po�aczony z hostem
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
                 char  Signal;    //moc sygna�u
               }_SSID;


typedef struct{
                _WiFiStat         Status;
                char              Ready;          //gotowy do sta�ej komunikacji
                char              SSID_Check;     //numer kroku szukania accespoint�w
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
      