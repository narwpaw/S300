//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                 CEURON                                                   *//
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : WizFiCommacd.c                           *//
//*  Opis                         : Obs³uga komend modu³u WizFi              *//
//*  Autor                        : Pawe³ Narwojsz                           *//
//*  Data utrzorzenia             : 11.04.2012                               *//
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



#include "WizFi.h"
//#include "Framework.h"
//#include "core/device.h"                                                        //aduje bibliotreki dla odpowiedniego procesora
//#include "main_H.h" 
#include "string.h"
#include "WizFi.h"


//Sta³e ------------------------------------------------------------------------
    const char AT[]="AT\r";                                                     //komenda WizFi 
    const char ATI2[]="ATI2\r";                                                     //komenda WizFi 
    const char AT_VER[]="AT+VER=?\r";
    const char AT_XDUM0[]="AT+XDUM=0\r";                                                //komenda WizFi 
    const char AT_WD[]="AT+WD\r";                                               //komenda WizFi 
    const char AT_NDHCP_0[]="AT+NDHCP=0\r";                                     //komenda WizFi   
    const char AT_NDHCP_1[]="AT+NDHCP=1\r";                                     //komenda WizFi 
    const char AT_NSET[]="AT+NSET=";                                          //komenda WizFi  
    const char AT_WAUTH_1[]="AT+WAUTH=1\r";                                          //komenda WizFi  
    const char AT_WWEP1[]="AT+WWEP1=";                                          //komenda WizFi  
    const char AT_WWPA[]="AT+WWPA=";                                            //komenda WizFi  
    const char AT_WA[]="AT+WA=";                                              //komenda WizFi  
    const char AT_DNSLOOKUP[]="AT+DNSLOOKUP=";                                              //komenda WizFi  
    const char AT_NCTCP[]="AT+NCTCP=";                                              //komenda WizFi  
    const char AT_NCLOSE[]="AT+NCLOSE=";                                              //komenda WizFi  
    const char AT_NCLOSEALL[]="AT+NCLOSEALL\r";                                              //komenda WizFi  
    const char AT_WS[]="AT+WS\r";
    const char AT_CID[]="AT+CID=?\r";
    const char AT_XRESET[]="AT+XRESET\r";
 //____________________________Koniec sta³ych ___________________________________
 

    extern unsigned long long  g_counter;                                              //globalny licznik milisekundowy od w³¹czenia/restartu sterownika
    
    
    
    extern char Variables[];    //zmienne
    extern char VariablesImpulse[];
    extern char VariablesNetTmp[];
    extern short int VariablesAnal[];    //zmienne
    extern _ProgramTab gProg;
    extern _MainStage  MainStage; 
    extern _DateTime DateTime;    
    extern _TransQuality TransQuality;
    extern unsigned char ChangeStageFlag;
    extern _RF_Status RF_Status;
    extern char WiFiDebug;

//------------------------------------------------------------------------------
void WizFiCommand_AT(char *Buff_RX, char *Buff_TX,  AT91PS_USART USART, _Net *Net)
{
   if (Net->WiFi.SSID[0]==0)
   {
     Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
     Net->Status.FreezTime=g_counter+1000;
   }else{
     for (short i=0; i<US1_BUFFER_Rx_SIZE; i++) Buff_RX[i]=0; 
     Buff_TX[0]='A';
     Buff_TX[1]='T';
     Buff_TX[2]=0x0d;
     Buff_TX[3]=0x0A;
     
     Net->Status.CurrentOrder=WizFi_AT;

     AT91F_US_SendFrame(USART,Buff_TX,4,0, 0);                        //Wysyla ramke 
     Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
     
     if (WiFiDebug) SendViaUSB(Buff_TX, 4);
   }
}




char WizFiResponseCommand_AT(char *Buff_RX, char *Buff_TX, AT91PS_USART USART, _Net *Net)
{
   if (WiFiDebug) SendViaUSB(Buff_RX, strlen(Buff_RX));
   char odp=0;
   if(strstr(Buff_RX, "OK")!=NULL)
        {
          odp=1;  //ok 
          for (char i=0; i<50; i++) Buff_RX[i]=0;
          Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
          Net->Status.Error=0;
        }

   return odp;
}






//------------------------------------------------------------------------------
void WizFiCommand_AT_VER(char *Buff_RX, char *Buff_TX,  AT91PS_USART USART, _Net *Net)
{
   if (WiFiDebug) SendViaUSB((char*)AT_VER, sizeof(AT_VER)-1);
   for (short i=0; i<US1_BUFFER_Rx_SIZE; i++) Buff_RX[i]=0;
   Net->Status.CurrentOrder=WizFi_AT_VER;     
   AT91F_US_SendFrame(USART,(char*)AT_VER,sizeof(AT_VER)-1,0, 0);                        //Wysyla ramke  
   Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
   
   
}




char WizFiResponseCommand_AT_VER(char *Buff_RX, char *Buff_TX, AT91PS_USART USART, _Net *Net)
{
   if (WiFiDebug) SendViaUSB(Buff_RX, strlen(Buff_RX));
   char odp=0;
   if(strstr(Buff_RX, "OK")!=NULL)
        {
          odp=1;  //ok 
          for (char i=0; i<50; i++) Buff_RX[i]=0;
          Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
          Net->Status.Error=0;
        }
   
   return odp;
}



//------------------------------------------------------------------------------
void WizFiCommand_ATI2(char *Buff_RX, char *Buff_TX,  AT91PS_USART USART, _Net *Net)
{
   for (short i=0; i<US1_BUFFER_Rx_SIZE; i++) Buff_RX[i]=0;
   Net->Status.CurrentOrder=WizFi_ATI2;
   if (WiFiDebug) SendViaUSB((char*)ATI2, sizeof(ATI2)-1);
   AT91F_US_SendFrame(USART,(char*)ATI2,sizeof(ATI2)-1,0, 0);                        //Wysyla ramke  
   Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
   
   
}




char WizFiResponseCommand_ATI2(char *Buff_RX, char *Buff_TX, AT91PS_USART USART, _Net *Net)
{
   char odp=0;
   if (WiFiDebug) SendViaUSB(Buff_RX, strlen(Buff_RX));
   if(strstr(Buff_RX, "OK")!=NULL)
        {
          odp=1;  //ok 
          for (char i=0; i<50; i++) Buff_RX[i]=0;
          Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
          Net->Status.Error=0;
        }
   
   return odp;
}




//------------------------------------------------------------------------------
void WizFiCommand_AT_XDUM0(char *Buff_RX, char *Buff_TX,  AT91PS_USART USART, _Net *Net)
{
   for (short i=0; i<US1_BUFFER_Rx_SIZE; i++) Buff_RX[i]=0;
   Net->Status.CurrentOrder=WizFi_AT_XDUM0;    
   if (WiFiDebug) SendViaUSB((char*)AT_XDUM0, sizeof(AT_XDUM0)-1);
   AT91F_US_SendFrame(USART,(char*)AT_XDUM0,sizeof(AT_XDUM0)-1,0, 0);                        //Wysyla ramke  
   Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
   
   
   
}

char WizFiResponseCommand_AT_XDUM0(char *Buff_RX, char *Buff_TX, AT91PS_USART USART, _Net *Net)
{
   char odp=0;
   if (WiFiDebug) SendViaUSB(Buff_RX, strlen(Buff_RX));
   if(strstr(Buff_RX, "OK")!=NULL)
        {
          odp=1;  //ok 
          for (char i=0; i<50; i++) Buff_RX[i]=0;
          Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
          Net->Status.Error=0;
        }
   
   return odp;
}






//------------------------------------------------------------------------------
void WizFiCommand_AT_NCLOSEALL(char *Buff_RX, char *Buff_TX,  AT91PS_USART USART, _Net *Net)
{
   for (short i=0; i<US1_BUFFER_Rx_SIZE; i++) Buff_RX[i]=0;
   Net->Status.CurrentOrder=WizFi_AT_NCLOSEALL;  
   if (WiFiDebug) SendViaUSB((char*)AT_NCLOSEALL, sizeof(AT_NCLOSEALL)-1);   
   AT91F_US_SendFrame(USART,(char*)AT_NCLOSEALL,sizeof(AT_NCLOSEALL)-1,0, 0);                        //Wysyla ramke  
   //Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
   Net->Status.HostLink=DISCONNECTED; 
   
   
}

char WizFiResponseCommand_AT_NCLOSEALL0(char *Buff_RX, char *Buff_TX, AT91PS_USART USART, _Net *Net)
{
   char odp=0;
   if (WiFiDebug) SendViaUSB(Buff_RX, strlen(Buff_RX));
   if(strstr(Buff_RX, "OK")!=NULL)
        {
          odp=1;  //ok 
          for (char i=0; i<50; i++) Buff_RX[i]=0;
          Net->Status.Timeout=g_counter + FREEZ_TIME;    //przed³u¿enie /restart timeoutu
           Net->Status.HostLink=DISCONNECTED;
        }
   
   return odp;
}


 


//------------------------------------------------------------------------------
void WizFiCommand_AT_WD(char *Buff_RX, char *Buff_TX,  AT91PS_USART USART, _Net *Net)
{
   for (short i=0; i<US1_BUFFER_Rx_SIZE; i++) Buff_RX[i]=0;
   Net->Status.CurrentOrder=WizFi_AT_WD;    
   if (WiFiDebug) SendViaUSB((char*)AT_WD, sizeof(AT_WD)-1);
   AT91F_US_SendFrame(USART,(char*)AT_WD,sizeof(AT_WD)-1,0, 0);                        //Wysyla ramke  
   Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
   
   
}

char WizFiResponseCommand_AT_WD(char *Buff_RX, char *Buff_TX, AT91PS_USART USART, _Net *Net)
{
   char odp=0;
   if (WiFiDebug) SendViaUSB(Buff_RX, strlen(Buff_RX));
   if(strstr(Buff_RX, "OK")!=NULL)
        {
          odp=1;  //ok 
          for (char i=0; i<50; i++) Buff_RX[i]=0;
          Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
          Net->Status.Error=0;
        }
   
   return odp;
}




//------------------------------------------------------------------------------
void WizFiCommand_AT_NDHCP(char *Buff_RX, char *Buff_TX,  AT91PS_USART USART, _Net *Net)
{      
   for (short i=0; i<US1_BUFFER_Rx_SIZE; i++) Buff_RX[i]=0;
   Net->Status.CurrentOrder=WizFi_AT_NDHCP;  
   if (Net->WiFi.IpNotAuto)
   {  
       if (WiFiDebug) SendViaUSB((char*)AT_NDHCP_0, sizeof(AT_NDHCP_0)-1);
       AT91F_US_SendFrame(USART,(char*)AT_NDHCP_0,sizeof(AT_NDHCP_0)-1,0, 0);                        //Wysyla ramke  
       
   }else{ 
       if (WiFiDebug) SendViaUSB((char*)AT_NDHCP_1, sizeof(AT_NDHCP_1)-1);
       AT91F_US_SendFrame(USART,(char*)AT_NDHCP_1,sizeof(AT_NDHCP_1)-1,0, 0);                        //Wysyla ramke  
       
   }
   Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu 
   
}

char WizFiResponseCommand_AT_NDHCP(char *Buff_RX, char *Buff_TX, AT91PS_USART USART, _Net *Net)
{
   char odp=0;
   if (WiFiDebug) SendViaUSB(Buff_RX, strlen(Buff_RX));
   if(strstr(Buff_RX, "OK")!=NULL)
        {
          odp=1;  //ok 
          for (char i=0; i<50; i++) Buff_RX[i]=0;
          Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
          Net->Status.Error=0;
        }
   
   return odp;
}




//------------------------------------------------------------------------------
void WizFiCommand_AT_NSET(char *Buff_RX, char *Buff_TX,  AT91PS_USART USART, _Net *Net)
{      
           for (short i=0; i<US1_BUFFER_Rx_SIZE; i++) Buff_RX[i]=0;
           Net->Status.CurrentOrder=WizFi_AT_NSET;
           char BTS=0;
           for (char i=0; i<sizeof(AT_NSET); i++)  {Buff_TX[i]=AT_NSET[i];   BTS++;}
           BTS--;
           char ZN=0;
           ZN=0; while ((Net->WiFi.IP[ZN]>0) && (ZN<=14)) {Buff_TX[BTS+ZN]=Net->WiFi.IP[ZN]; ZN++;}
           BTS+=ZN;
           Buff_TX[BTS++]=',';
           ZN=0; while ((Net->WiFi.Mask[ZN]>0) && (ZN<=14)) {Buff_TX[BTS+ZN]=Net->WiFi.Mask[ZN]; ZN++;}
           BTS+=ZN;
           Buff_TX[BTS++]=',';
           ZN=0; while ((Net->WiFi.Gate[ZN]>0) && (ZN<=14)) {Buff_TX[BTS+ZN]=Net->WiFi.Gate[ZN]; ZN++;}
           
           Buff_TX[BTS+ZN++]=0x0d;
           if (WiFiDebug) SendViaUSB(Buff_TX, BTS+ZN);
           AT91F_US_SendFrame(USART,Buff_TX,BTS+ZN,0, 0);                        //Wysyla ramke  
           Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
           
           
}

char WizFiResponseCommand_AT_NSET(char *Buff_RX, char *Buff_TX, AT91PS_USART USART, _Net *Net)
{
   char odp=0;
   if (WiFiDebug) SendViaUSB(Buff_RX, strlen(Buff_RX));
   if(strstr(Buff_RX, "OK")!=NULL)
        {
          odp=1;  //ok 
          for (char i=0; i<50; i++) Buff_RX[i]=0;
          Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
          Net->Status.Error=0;
        }
   
   return odp;
}




//------------------------------------------------------------------------------
void WizFiCommand_AT_WAUTH_1(char *Buff_RX, char *Buff_TX,  AT91PS_USART USART, _Net *Net)
{
   for (short i=0; i<US1_BUFFER_Rx_SIZE; i++) Buff_RX[i]=0;
   Net->Status.CurrentOrder=WizFi_AT_WAUTH_1;    
   if (WiFiDebug) SendViaUSB((char*)AT_WAUTH_1, sizeof(AT_WAUTH_1)-1);
   AT91F_US_SendFrame(USART,(char*)AT_WAUTH_1,sizeof(AT_WAUTH_1)-1,0, 0);                        //Wysyla ramke  
   Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
}

char WizFiResponseCommand_AT_WAUTH_1(char *Buff_RX, char *Buff_TX, AT91PS_USART USART, _Net *Net)
{
   char odp=0;
   if (WiFiDebug) SendViaUSB(Buff_RX, strlen(Buff_RX));
   if(strstr(Buff_RX, "OK")!=NULL)
        {
          odp=1;  //ok 
          for (char i=0; i<50; i++) Buff_RX[i]=0;
          Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
          Net->Status.Error=0;
        }
   
   return odp;
}


//funkcja konwertuj¹ca string asci na string hex. ka¿da litera rozk³adana jest na dwie cyfry hex które s¹ zapisywane spowrotem jako ascii
void AsciToHexStr(char *AsciStr, char *HexStr)
{

   
   char L=0; //litera
   char LHex=0; //cyfra hex
   while ((AsciStr[L]>0) && (L<100))
   {
      CharToStr((AsciStr[L]>>4), &HexStr[LHex++], 1); 
      CharToStr((AsciStr[L]&0xF), &HexStr[LHex++], 1); 
      L++;
   }
   HexStr[LHex]=0;
}



//------------------------------------------------------------------------------
char WizFiCommand_AT_WWEP1(char *Buff_RX, char *Buff_TX,  AT91PS_USART USART, _Net *Net)
{
           //okreœlenie typu wpisu czy? 64bit hex, czy 64bit asci, czy 128bit ASCI  -- 128B hex nie jest obs³ugiwane   
           //5 znaków - kod asci 64b
           //13 znaków - kod asci 128b
           //10 znaków - kod hex 64 bit
           //wartoœæ inna to b³¹d lub kodowanie nie WEP
           
           char PassLen=0;
           while ((Net->WiFi.Key[PassLen]>0) && (PassLen<=15)) PassLen++;
           
           if ((PassLen!=5) && (PassLen!=13) && (PassLen!=10)) return 0;        //z³a iloœc znaków lub nie ten typ kodowania
   
           
           //wstawienie rozkazu 
           for (short i=0; i<US1_BUFFER_Rx_SIZE; i++) Buff_RX[i]=0;
          // Net->Status.CurrentOrder=WizFi_AT_WWEP1;
           char BTS=0;
           for (char i=0; i<sizeof(AT_WWEP1); i++)  {Buff_TX[i]=AT_WWEP1[i];   BTS++;}
           BTS--;
           
           
           //wstawienie klucza
           char Key[15];
           char PassN=0;
           switch (PassLen)
           {
              case 5:    
                  PassN=10;  
                  AsciToHexStr(Net->WiFi.Key, &Buff_TX[BTS]);
              break;

              case 13:    
                  PassN=26; 
                  AsciToHexStr(Net->WiFi.Key, &Buff_TX[BTS]);
              break;
              
              case 10:    
                  PassN=10;  for (char i=0; i<PassN; i++) Buff_TX[BTS+i]=Net->WiFi.Key[i];
              break;
           }
  

           
           
            
           Buff_TX[BTS+PassN]=0x0d;
           if (WiFiDebug) SendViaUSB(Buff_TX, BTS+PassN+1);
           AT91F_US_SendFrame(USART,Buff_TX,BTS+PassN+1,0, 0);                        //Wysyla ramke  
           Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
           
           
           //while ((Net->WiFi.Key[PassN]>0) && (PassN<=15)) {Buff_TX[BTS+PassN]=Net->WiFi.Key[PassN]; PassN++;}
   
          return 1;
   
}

char WizFiResponseCommand_AT_WWEP1(char *Buff_RX, char *Buff_TX, AT91PS_USART USART, _Net *Net)
{
   char odp=0;
   if (WiFiDebug) SendViaUSB(Buff_RX, strlen(Buff_RX));
   if(strstr(Buff_RX, "OK")!=NULL)
        {
          odp=1;  //ok 
          for (char i=0; i<50; i++) Buff_RX[i]=0;
          Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
          Net->Status.Error=0;
        }
   
   return odp;
}









//------------------------------------------------------------------------------
char WizFiCommand_AT_WWPA(char *Buff_RX, char *Buff_TX,  AT91PS_USART USART, _Net *Net)
{
           for (short i=0; i<US1_BUFFER_Rx_SIZE; i++) Buff_RX[i]=0;
           Net->Status.CurrentOrder=WizFi_AT_WWPA;
           char BTS=0;
           for (char i=0; i<sizeof(AT_WWPA); i++)  {Buff_TX[i]=AT_WWPA[i];   BTS++;}
           BTS--;
           char PassN=0;
           while ((Net->WiFi.Key[PassN]>0) && (PassN<=15)) {Buff_TX[BTS+PassN]=Net->WiFi.Key[PassN]; PassN++;}
           
           if (PassN<8) return 0;
           
           Buff_TX[BTS+PassN++]=0x0d;
           if (WiFiDebug) SendViaUSB(Buff_TX, BTS+PassN);
           AT91F_US_SendFrame(USART,Buff_TX,BTS+PassN,0, 0);                        //Wysyla ramke 
           Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
           return 1;
           
}

char WizFiResponseCommand_AT_WWPA(char *Buff_RX, char *Buff_TX, AT91PS_USART USART, _Net *Net)
{
   char odp=0;
   if (WiFiDebug) SendViaUSB(Buff_RX, strlen(Buff_RX));
   if(strstr(Buff_RX, "OK")!=NULL)
        {
          odp=1;  //ok 
          for (char i=0; i<50; i++) Buff_RX[i]=0;
          Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
          Net->Status.Error=0;
        }
   
   return odp;
}








//³¹czenie z accespointem ------------------------------------------------------
//------------------------------------------------------------------------------
void WizFiCommand_AT_WA(char *Buff_RX, char *Buff_TX,  AT91PS_USART USART, _Net *Net)
{
     if (Net->WiFi.SSID[0]!=0)
     {
           for (short i=0; i<US1_BUFFER_Rx_SIZE; i++) Buff_RX[i]=0;
           Net->Status.CurrentOrder=WizFi_AT_WA;

           char BTS=0;
           for (char i=0; i<sizeof(AT_WA); i++)  {Buff_TX[i]=AT_WA[i];   BTS++;}
           BTS--;
           char PassN=0;
           while ((Net->WiFi.SSID[PassN]>0) && (PassN<=15)) {Buff_TX[BTS+PassN]=Net->WiFi.SSID[PassN]; PassN++;}
           Buff_TX[BTS+PassN++]=0x0d;
           
           if (WiFiDebug) SendViaUSB(Buff_TX, BTS+PassN);
           AT91F_US_SendFrame(USART,Buff_TX,BTS+PassN,0, 0);                        //Wysyla ramke 
           Net->Status.Timeout=g_counter + AP_CONNECT_TIMEOUT;    //przed³u¿enie /restart timeoutu
           
           
     }


}

char WizFiResponseCommand_AT_WA(char *Buff_RX, char *Buff_TX, AT91PS_USART USART, _Net *Net)
{
   char odp=0;
   if (WiFiDebug) SendViaUSB(Buff_RX, strlen(Buff_RX));
   if(strstr(Buff_RX, "OK")!=NULL)
        {
          odp=1;  //ok 
          for (char i=0; i<50; i++) Buff_RX[i]=0;
          
          //Tu mo¿e byæ pobranie otrzymanego IP, Bramy, 

          //---------------
          Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
          Net->Status.Error=0;
          
          Net->Status.AccesPointAssociateTest=0xFF;           //dla testów ekranowych
        }
     
   
   return odp;
}






//Sprawdzenie czy w WiFi.Domena jest wpisana domena czy IP serwera domeny ------
// odp: 0- domena
// odp: 1 - IP
//------------------------------------------------------------------------------
char CheckExistDomenaIP(_Net *Net)
{
  char ExistIP=1;

  if ((Net->WiFi.Domena[0]<'0') && ((Net->WiFi.Domena[0]>'9'))) ExistIP=0;
  
  char Size= sizeof(Net->WiFi.Domena);
  char Znak=1;
  while ((Znak<Size) && (ExistIP==1)  && (Net->WiFi.Domena[Znak]>0) )  
  {
     if (((Net->WiFi.Domena[Znak]<'0') || (Net->WiFi.Domena[Znak]>'9')) && (Net->WiFi.Domena[Znak]!='.'))  ExistIP=0;
  
     Znak++; 
  }
  
  //je¿eli w Net->WiFi.Domena wpisany jest IP tp przepisz do  Net->Status.DomenaIP
  if (ExistIP==1)
  {
    Znak=0;
    for (char i=0; i<Size; i++) Net->Status.DomenaIP[i]=0;
    while ((Znak<Size) && (ExistIP)  && (Net->WiFi.Domena[Znak]>0))
    {
      Net->Status.DomenaIP[Znak]=Net->WiFi.Domena[Znak];
      Znak++;
    }
  }
  
  return ExistIP;
  
}






//Pobranie adresu domeny ------------------------------------------------------
//------------------------------------------------------------------------------
void WizFiCommand_AT_DNSLOOKUP(char *Buff_RX, char *Buff_TX,  AT91PS_USART USART, _Net *Net)
{
      if (Net->WiFi.Domena[0]!=0)
      {
          for (short i=0; i<US1_BUFFER_Rx_SIZE; i++) Buff_RX[i]=0;
          if (CheckExistDomenaIP(Net)==0)                                                  //sprawdzenie czy w WiFi.Domena jest wpisana domena czy IP serwera domeny
          {                                                                     //w WiFi.Domena jest wpisana domena
             Net->Status.CurrentOrder=WizFi_AT_DNSLOOKUP;
  
             char BTS=0;
             for (char i=0; i<sizeof(AT_DNSLOOKUP); i++)  {Buff_TX[i]=AT_DNSLOOKUP[i];   BTS++;}
             BTS--;
             char PassN=0;
             while ((Net->WiFi.Domena[PassN]>0) && (PassN<=15)) {Buff_TX[BTS+PassN]=Net->WiFi.Domena[PassN]; PassN++;}
             Buff_TX[BTS+PassN++]=0x0d;
         
             if (WiFiDebug) SendViaUSB(Buff_RX, BTS+PassN); 
             AT91F_US_SendFrame(USART,Buff_TX,BTS+PassN,0, 0);                        //Wysyla ramke 
             Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
             Net->Status.NextHttpOrder=HTTP_GET_Config;
             
             
          }
          else                                                                  //je¿eli w WiFi.Domena jest wpisany IP serwera domeny
          {
             Net->Status.AccesPointAssociate=CONNECTED;    
             Net->Status.CurrentOrder=WizFi_NC;     //wyczyszczenie rozkazów
             Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
          }
          
      }
}



/*
//Pobranie adresu domeny ------------------------------------------------------
//------------------------------------------------------------------------------
void WizFiCommand_AT_DNSLOOKUP(char *Buff_TX,  AT91PS_USART USART, _Net *Net)
{
             Net->Status.CurrentOrder=WizFi_AT_DNSLOOKUP;
  
             char BTS=0;
             for (char i=0; i<sizeof(AT_DNSLOOKUP); i++)  {Buff_TX[i]=AT_DNSLOOKUP[i];   BTS++;}
             BTS--;
             char PassN=0;
             while ((Net->WiFi.Domena[PassN]>0) && (PassN<=25)) {Buff_TX[BTS+PassN]=Net->WiFi.Domena[PassN]; PassN++;}
             Buff_TX[BTS+PassN++]=0x0d;
         
             AT91F_US_SendFrame(USART,Buff_TX,BTS+PassN,0, 0);                        //Wysyla ramke 
             Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
             Net->Status.NextHttpOrder=HTTP_GET_Config;
}
*/


char WizFiResponseCommand_AT_DNSLOOKUP(char *Buff_RX, char *Buff_TX, AT91PS_USART USART, _Net *Net)
{
   char odp=0;
   char *pTxt;
   
   pTxt=strstr(Buff_RX, "IP:");
   if (WiFiDebug) SendViaUSB(Buff_RX, strlen(Buff_RX));
   if((strstr(Buff_RX, "OK")!=NULL) && (pTxt!=NULL))
        {
          odp=1;  //ok 
          
          Net->Status.AccesPointAssociate=CONNECTED;    
          Net->Status.CurrentOrder=WizFi_NC;     //wyczyszczenie rozkazów
            
          pTxt+=3;
          char p=0;
          while ((*pTxt!=0x0d) && (p<=15))  {Net->Status.DomenaIP[p++]=*pTxt; pTxt++;}
          Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
          for (char i=0; i<50; i++) Buff_RX[i]=0;
          Net->Status.Error=0;
        }
     
   return odp;
}



               





//Rozkaz po³¹czenia z hostem----------------------------------------------------
//------------------------------------------------------------------------------
void WizFiCommand_AT_NCTCP(char *Buff_RX, char *Buff_TX,  AT91PS_USART USART, _Net *Net)
{
     if (Net->WiFi.Domena[0]!=0)
     {
           for (short i=0; i<US1_BUFFER_Rx_SIZE; i++) Buff_RX[i]=0;
           Net->Status.SendPacketTime=g_counter;
           Net->Status.HostLink=CONNECTING;
           Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
           //Net->Status.HttpNextQueryTime=g_counter+100;   //rezerwa czasu pomiêdzy odpytaniami HTTP.
           Net->Status.FreezTime=g_counter+FREEZ_TIME;
           
           Net->Status.CurrentOrder=WizFi_AT_DNSLOOKUP;
           
           char BTS=0;
           for (char i=0; i<sizeof(AT_NCTCP); i++)  {Buff_TX[i]=AT_NCTCP[i];   BTS++;}
           BTS--;
           char PN=0;
           if ((Net->WiFi.Subdomena[0]>47)&&(Net->WiFi.Subdomena[0]<123)) 
           {
             while ((Net->WiFi.Subdomena[PN]>0) && (PN<=15)) {Buff_TX[BTS+PN]=Net->WiFi.Subdomena[PN]; PN++;}
             Buff_TX[BTS+PN]='.';
           }
           PN=0;
           while ((Net->Status.DomenaIP[PN]>0) && (PN<=15)) {Buff_TX[BTS+PN]=Net->Status.DomenaIP[PN]; PN++;}
           Buff_TX[BTS+PN++]=',';
           Buff_TX[BTS+PN++]='8';
           Buff_TX[BTS+PN++]='0';
           Buff_TX[BTS+PN++]=0x0d;
          
           

            
            if (WiFiDebug) SendViaUSB(Buff_RX, BTS+PN);  
           AT91F_US_SendFrame(USART,Buff_TX,BTS+PN,0, 0);                        //Wysyla ramke  otwieraj¹c¹ po³¹czenie 
           
          
     }
}



char WizFiResponseCommand_AT_NCTCP(char *Buff_RX, char *Buff_TX, AT91PS_USART USART, _Net *Net)
{
   char odp=0;
   char *pTxt;
   
   if (WiFiDebug) SendViaUSB(Buff_RX, strlen(Buff_RX));
   pTxt=strstr(Buff_RX, "[CONNECT");
   if ((pTxt!=NULL) && (pTxt[9]!='9')&& (pTxt[9]!='A'))
        {
          odp=1;  //ok 
          pTxt+=9;
          Net->Status.HostChannel=*pTxt;
          Net->Status.CurrentOrder=WizFi_NC;     //wyczyszczenie rozkazów
          
          Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
          

     
          if ((pTxt[0]-48)==6)
          {
           char BTS=0;
           for (char i=0; i<sizeof(AT_NCLOSE); i++)  {Buff_TX[i]=AT_NCLOSE[i];   BTS++;}
           BTS--;
           Buff_TX[BTS++]='0';
           Buff_TX[BTS++]=0x0d; 
           if (WiFiDebug) SendViaUSB(Buff_RX, BTS); 
           AT91F_US_SendFrame(USART,Buff_TX,BTS,0, 0);                        //Wysyla ramke  otwieraj¹c¹ po³¹czenie 
          
          }
          if ((pTxt[0]-48)>6)
          {
           char BTS=0;
           for (char i=0; i<sizeof(AT_NCLOSE); i++)  {Buff_TX[i]=AT_NCLOSE[i];   BTS++;}
           BTS--;
           Buff_TX[BTS++]=pTxt[0]-1;
           Buff_TX[BTS++]=0x0d; 
           
           if (WiFiDebug) SendViaUSB(Buff_TX, BTS);
           AT91F_US_SendFrame(USART,Buff_TX,BTS,0, 0);                        //Wysyla ramke  otwieraj¹c¹ po³¹czenie 

          }
      
          
          for (char i=0; i<50; i++) Buff_RX[i]=0;
          
          
          Net->Status.HostLink=CONNECTED;
          Net->Status.HttpNextQueryTime=g_counter+100;
          Net->Status.FreezTime=0;
          Net->Status.Error=0;
          

          
        
          
        }else if (strstr(Buff_RX, "[ERROR")!=NULL)
        {
          if (strstr(Buff_RX, "NO CID]")!=NULL)
          {
            
            odp=0;
            
            //radykalne zabezpieczenie - tymczasowe(prawdopodobnie :-)  )
            //AT91C_BASE_RSTC->RSTC_RCR = AT91C_RSTC_PROCRST | AT91C_RSTC_PERRST | AT91C_RSTC_KEY;   //restart sterownika    
            
             if (WiFiDebug) SendViaUSB((char*)AT_XRESET, sizeof(AT_XRESET)-1);
            AT91F_US_SendFrame(USART,(char*)AT_XRESET,sizeof(AT_XRESET)-1,0, 0);                        //Wysyla ramke resetuj¹c¹ modu³ WiFi 
            
          #ifdef _ADMINISTRATOR_
                      Message_Paint(2,"Reset","WiFi",""," ","");   
          #endif            
          }else{
            odp=0;
            for (char i=0; i<50; i++) Buff_RX[i]=0;                    
            Net->Status.HostLink=DISCONNECTED;
            Net->Status.FreezTime=0;
            
            Net->Status.Error++;
          }
          
        }
          

   return odp;
}




 



//funkcja konwertuje napis na wartoœæ short int --------------------------------
// Txt  -wskaŸnik na ci¹g
// Cyfr -funkcja zwraca iloœæ cyfr w ci¹gu
// funkcja zwraca wartoœæ short int
signed short int StrToWord(char *String, char *Cyfr)
{
   char *Txt=String;
   char LiczbaTab[5];
   char IlCyfr=0;
   signed char Min=1;
   
   while ((((*Txt>='0') && (*Txt<='9')) || (*Txt=='-')) && (IlCyfr<5))
   {
      if (*Txt=='-')
      {
         Min=-1;
      }else{
         LiczbaTab[IlCyfr++]=*Txt-48;
      }
      *Txt++;
   }
   *Cyfr=IlCyfr;
   
   int Mnoz=1;
   signed short int Wartosc=0; 
   while (IlCyfr>0)
   {
       Wartosc+=LiczbaTab[--IlCyfr]*Mnoz;
       Mnoz*=10;
   }
   Wartosc*=Min;
   
   return Wartosc;
}//_____________________________________________________________________________


unsigned short GetSpecialAnalogVar(char ZmSp)
{
   char sc;
   
   if (ZmSp<ScreenNr)       //zmienne czujnika termostatu
   {
        sc=ZmSp;
        if (gProg.Screen[sc].Thermo.SensorNode<0) //je¿eli zmienna
           {
             return VariablesAnal[gProg.Screen[sc].Thermo.SensorReg];
           }else{
             return MainStage.StageI[gProg.Screen[sc].Thermo.SensorNode].Value[gProg.Screen[sc].Thermo.SensorReg];
           }
   }
   else if ((ZmSp>=10) && (ZmSp<10+ScreenNr))       //zmienne czujnika termostatu
   {
           sc=ZmSp-10;
           return gProg.Screen[sc].Thermo.SetTemperature;
    
   }
   return 0;
  
}


char GetSpecialBinarVar(char ZmSp)
{
   char sc;
  
   if (ZmSp<ScreenNr)       //zmienne  termostatu  AutoOn
   {
        return gProg.Screen[ZmSp].Thermo.AutoOn;
   }
   else if ((ZmSp>=10) && (ZmSp<10+ScreenNr))       //zmienne  termostatu Temp. zad w AutoOn
   {
       sc=ZmSp-10;
      char MaxTmp;
      if (gProg.Screen[sc].Thermo.Temperature[0]>gProg.Screen[sc].Thermo.Temperature[1])
      {
        MaxTmp=gProg.Screen[sc].Thermo.Temperature[0]; 
      }else{
        MaxTmp=gProg.Screen[sc].Thermo.Temperature[1];
      }
      
      if (gProg.Screen[sc].Thermo.SetTemperature==MaxTmp)
      {
       return 1;  
      }else{
       return 0;      
      }     
          
        
   }
   else if ((ZmSp>=40) && (ZmSp<40+ScreenNr))       //zmienne  termostatu Temp. zad w AutoOn
   {
      sc=ZmSp-40;
      
      char res=0;
      
        if ((gProg.Screen[sc].Thermo.ActorNode<0xFF) && (gProg.Screen[sc].Thermo.ActorNode>=0))   //je¿eli wêze³
        {
          if ((PortStageRead(MainStage.StageQ[gProg.Screen[sc].Thermo.ActorNode].Port , gProg.Screen[sc].Thermo.ActorPort))==1) res=1;
        }else{  //Je¿eli zmienna
          if(((Variables[gProg.Screen[sc].Thermo.ActorPort/8]>> (gProg.Screen[sc].Thermo.ActorPort%8))&0x1)==1) res=1;
        }
      
      
      return res;
   }
   
   return 0;
}  
  




void SetSpecialBinarVar(char ZmSp, char Val, _Net *Net)
{
   char sc;
  
   if (ZmSp<ScreenNr)       //zmienne  termostatu  AutoOn
   {
       
      if (Val) gProg.Screen[ZmSp].Thermo.AutoOn=1; else gProg.Screen[ZmSp].Thermo.AutoOn=0;
      Net->Status.NextHttpOrder=HTTP_POST;
      ThermostatsTimeChartExecute(); 
      ThermostatsExecute();
      MenuPaint();
      ExecuteSpecialThermVar();
       
   }
   else if ((ZmSp>=10) && (ZmSp<10+ScreenNr))       //zmienne  termostatu Temp. zad w AutoOn
   {
      sc=ZmSp-10;
      char MaxTmp;
      char MinTmp;
      if (gProg.Screen[sc].Thermo.Temperature[0]>gProg.Screen[sc].Thermo.Temperature[1])
      {
        MaxTmp=gProg.Screen[sc].Thermo.Temperature[0]; 
        MinTmp=gProg.Screen[sc].Thermo.Temperature[1];
      }else{
        MaxTmp=gProg.Screen[sc].Thermo.Temperature[1];
        MinTmp=gProg.Screen[sc].Thermo.Temperature[0];
      }
      
      if (Val)
      {
          gProg.Screen[sc].Thermo.SetTemperature=MaxTmp;
      }else{
          gProg.Screen[sc].Thermo.SetTemperature=MinTmp; 
      }
      Net->Status.NextHttpOrder=HTTP_POST;
      ThermostatsTimeChartExecute(); 
      ThermostatsExecute();
      MenuPaint();
      ExecuteSpecialThermVar();
   }else if ((ZmSp>=20) && (ZmSp<20+ScreenNr))       //zmienne  termostatu temp+ w trybie Auto
   {
      sc=ZmSp-20;
      if ((gProg.Screen[sc].Thermo.SetTemperature)<500) {gProg.Screen[sc].Thermo.SetTemperature++;}
      Net->Status.NextHttpOrder=HTTP_POST;
      ThermostatsTimeChartExecute(); 
      ThermostatsExecute();
      MenuPaint();
      ExecuteSpecialThermVar();
   }else if ((ZmSp>=30) && (ZmSp<30+ScreenNr))       //zmienne  termostatu temp+ w trybie Auto
   {
      sc=ZmSp-30;
      if ((gProg.Screen[sc].Thermo.SetTemperature)>0) {gProg.Screen[sc].Thermo.SetTemperature--;}
      Net->Status.NextHttpOrder=HTTP_POST;
      ThermostatsTimeChartExecute(); 
      ThermostatsExecute();
      MenuPaint();
      ExecuteSpecialThermVar();
   }

}  
  


//Funkcja wys³ania zadania HTTP
void HTTP_Query(char *Buff_RX, char *Buff_TX,  AT91PS_USART USART, _Net *Net)
{

                
           //wysy³anie danych ------------------------------------
           
           short BTS=0;
           for (short i=0; i<sizeof(Buff_TX); i++) Buff_TX[i]=0;
           Buff_TX[BTS++]=0x1B;
           Buff_TX[BTS++]='S';
           Buff_TX[BTS++]=Net->Status.HostChannel;     //numer kana³u [przydzielony przez serwer
           
                                     
           char Send=0; 
           char txt[16]; 
           //czyszczenie bufora nadawczego
           char *PozIlByte;
           switch (Net->Status.CurrentOrder)     
           {
              case HTTP_GET_Config:              //proœba o podanie konfiguracji zestawu zmiennych
                    strcat(Buff_TX,"GET ");
                    strcat(Buff_TX,Net->WiFi.Path);
                    strcat(Buff_TX,"/dc");
                    sprintf(txt, "%d",RF_Status.RF_ID_R);
                    strcat(Buff_TX,txt);
                    strcat(Buff_TX,".csv HTTP/1.1\r\n");
                    strcat(Buff_TX,"User-Agent: CEURON\r\nHost: ");
                    if ((Net->WiFi.Subdomena[0]>47)&&(Net->WiFi.Subdomena[0]<123)) {strcat(Buff_TX,Net->WiFi.Subdomena); strcat(Buff_TX,".");}
                    strcat(Buff_TX,Net->WiFi.Domena);
                    strcat(Buff_TX,"\r\n");
                    strcat(Buff_TX,"Connection: keep-alive\r\n");
                    strcat(Buff_TX,"\r\n\r\n");
                    BTS=strlen(Buff_TX);
                    Send=1;

              break; 
              
              case HTTP_GET:              //proœba o podanie konfiguracji zestawu zmiennych
                    strcat(Buff_TX,"GET ");
                    strcat(Buff_TX,Net->WiFi.Path);
                    strcat(Buff_TX,"/do");
                    sprintf(txt, "%d",RF_Status.RF_ID_R);
                    strcat(Buff_TX,txt);
                    strcat(Buff_TX,".csv HTTP/1.1\r\n");
                    strcat(Buff_TX,"User-Agent: CEURON\r\nHost: ");
                    if ((Net->WiFi.Subdomena[0]>47)&&(Net->WiFi.Subdomena[0]<123)) {strcat(Buff_TX,Net->WiFi.Subdomena); strcat(Buff_TX,".");}
                    strcat(Buff_TX,Net->WiFi.Domena);
                    strcat(Buff_TX,"\r\n");
                    strcat(Buff_TX,"Connection: keep-alive\r\n");
                    strcat(Buff_TX,"\r\n\r\n");
                    BTS=strlen(Buff_TX);
                     Send=1;
                    
                    
              break; 
              
              case HTTP_POST:                   //wys³anie zmiennych
                    
                    //obliczenie max iloœci wysy³anych danych
                    //151B(Nag³ówek i stopka) + 45B(Path,Subdomena,Domena) + 40(10B x VAR_SET_AMOUNT lub 7B x VAR_SET_AMOUNT dla dwustanów)  = max 596B. Przy samych dwustanach 476B
                    strcat(Buff_TX,"POST ");
                    strcat(Buff_TX,Net->WiFi.Path);
                    strcat(Buff_TX,"/putgate.php HTTP/1.1\r\nUser-Agent: CEURON\r\nHost: ");
                    if ((Net->WiFi.Subdomena[0]>47)&&(Net->WiFi.Subdomena[0]<123)) {strcat(Buff_TX,Net->WiFi.Subdomena); strcat(Buff_TX,".");}
                    strcat(Buff_TX,Net->WiFi.Domena);
                    strcat(Buff_TX,"\r\nContent-Length: ");
                    char PozLength=strlen(Buff_TX);
            
                    strcat(Buff_TX,"   ");
                    strcat(Buff_TX,"\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nNodeID=");
                    int NAG=strlen(Buff_TX)-7;
                    sprintf(txt, "%d",RF_Status.RF_ID_R);
                    strcat(Buff_TX,txt);
                    
                    strcat(Buff_TX,"&Order=1&Content=;\r\n");
                    BTS=strlen(Buff_TX);
                    //oko³o 179 byte do tej pory
                    short ContentLength=34;
                    char ilLit=0;
                    
                    for (char VSN=0; VSN<Net->HTTP.VarSetCount; VSN++)
                    {
                      switch (Net->HTTP.NetNodeSet[VSN].NetDataType)
                      {
                        case 'a':  //jezeli zmienna analogowa
                              
                              Buff_TX[BTS++]='a';
                              Buff_TX[BTS++]=(Net->HTTP.NetNodeSet[VSN].VarNr/10)+48;
                              Buff_TX[BTS++]=(Net->HTTP.NetNodeSet[VSN].VarNr%10)+48;
                              Buff_TX[BTS++]=';';
                              
                              if (VariablesAnal[Net->HTTP.NetNodeSet[VSN].VarNr]==(signed short)0x8000)
                              {
                                 Buff_TX[BTS++]='B';
                                 Buff_TX[BTS++]='A';
                                 Buff_TX[BTS++]='T';
                              }else{  
                               
                                if ((VariablesAnal[Net->HTTP.NetNodeSet[VSN].VarNr])>999)
                                {
                                  Buff_TX[BTS++]='>';
                                  Buff_TX[BTS++]='M';
                                  Buff_TX[BTS++]='A';
                                  Buff_TX[BTS++]='X';
                                }
                                else if ((VariablesAnal[Net->HTTP.NetNodeSet[VSN].VarNr])<-999)
                                {
                                  Buff_TX[BTS++]='-';
                                  Buff_TX[BTS++]='M';
                                  Buff_TX[BTS++]='I';
                                  Buff_TX[BTS++]='N';
                                }else{
                                  ilLit=sIntToStr(VariablesAnal[Net->HTTP.NetNodeSet[VSN].VarNr],&Buff_TX[BTS]);
                                  BTS+=ilLit;
   
                                for (char i=ilLit; i<4; i++) {Buff_TX[BTS++]=' ';}
                                }
                              }
                              
                              Buff_TX[BTS++]=0xd;
                              Buff_TX[BTS++]=0xa;
                              ContentLength+=10;
                                
                        break;
                        
                        case 'b':  //jezeli zmienna dwustanowa
                              Buff_TX[BTS++]='b';
                              Buff_TX[BTS++]=(Net->HTTP.NetNodeSet[VSN].VarNr/10)+48;
                              Buff_TX[BTS++]=(Net->HTTP.NetNodeSet[VSN].VarNr%10)+48;
                              Buff_TX[BTS++]=';';
                              Net->HTTP.NetNodeSet[VSN].Value=((Variables[Net->HTTP.NetNodeSet[VSN].VarNr/8]>>(Net->HTTP.NetNodeSet[VSN].VarNr%8))&0x1);
                              Buff_TX[BTS++]=Net->HTTP.NetNodeSet[VSN].Value+48;
                              Buff_TX[BTS++]=0xd;
                              Buff_TX[BTS++]=0xa;
                              ContentLength+=7;
                        break;
                        
                        case 'c':  //jezeli zmienna analogowa specjalna
                              
                              Buff_TX[BTS++]='c';
                              Buff_TX[BTS++]=(Net->HTTP.NetNodeSet[VSN].VarNr/10)+48;
                              Buff_TX[BTS++]=(Net->HTTP.NetNodeSet[VSN].VarNr%10)+48;
                              Buff_TX[BTS++]=';';
                              if (Net->HTTP.NetNodeSet[VSN].VarNr==99)   //je¿eli proœpa o podanie czsu
                              {
                                Buff_TX[BTS++]=(DateTime.Hour/10)+48;
                                Buff_TX[BTS++]=(DateTime.Hour%10)+48;
                                Buff_TX[BTS++]=':';
                                Buff_TX[BTS++]=(DateTime.Minute/10)+48;
                                Buff_TX[BTS++]=(DateTime.Minute%10)+48;
                                //for (char i=ilLit; i<4; i++) {Buff_TX[BTS++]=' ';}
                                Buff_TX[BTS++]=0xd;
                                Buff_TX[BTS++]=0xa;
                                ContentLength+=11;
                              }else{
                                ilLit=sIntToStr(GetSpecialAnalogVar(Net->HTTP.NetNodeSet[VSN].VarNr),&Buff_TX[BTS]);
                                BTS+=ilLit;
                                for (char i=ilLit; i<4; i++) {Buff_TX[BTS++]=' ';}
                                Buff_TX[BTS++]=0xd;
                                Buff_TX[BTS++]=0xa;
                                ContentLength+=10;
                              }
                        break;
                        
                         case 'd':  //jezeli zmienna dwustanowa specjalna
                          
                              Buff_TX[BTS++]='d';
                              Buff_TX[BTS++]=(Net->HTTP.NetNodeSet[VSN].VarNr/10)+48;
                              Buff_TX[BTS++]=(Net->HTTP.NetNodeSet[VSN].VarNr%10)+48;
                              Buff_TX[BTS++]=';';
                              Buff_TX[BTS++]=GetSpecialBinarVar(Net->HTTP.NetNodeSet[VSN].VarNr)+48;
                              Buff_TX[BTS++]=0xd;
                              Buff_TX[BTS++]=0xa;
                              Net->HTTP.NetNodeSet[VSN].Value=GetSpecialBinarVar(Net->HTTP.NetNodeSet[VSN].VarNr);
                                
                         
                              ContentLength+=7;
                        break;
                        
                        
                        case 'i':  //jezeli wejœcie dwustanowe
                              Buff_TX[BTS++]='i';
                              Buff_TX[BTS++]=(Net->HTTP.NetNodeSet[VSN].NodeNr/10)+48;
                              Buff_TX[BTS++]=(Net->HTTP.NetNodeSet[VSN].NodeNr%10)+48;
                              Buff_TX[BTS++]=(Net->HTTP.NetNodeSet[VSN].PortNr/10)+48;
                              Buff_TX[BTS++]=(Net->HTTP.NetNodeSet[VSN].PortNr%10)+48;
                              Buff_TX[BTS++]=';';
                              Net->HTTP.NetNodeSet[VSN].Value=PortStageRead(MainStage.StageI[Net->HTTP.NetNodeSet[VSN].NodeNr].Port, Net->HTTP.NetNodeSet[VSN].PortNr); 
                              Buff_TX[BTS++]=Net->HTTP.NetNodeSet[VSN].Value+48;
                              Buff_TX[BTS++]=0xd;
                              Buff_TX[BTS++]=0xa;
                              ContentLength+=9;
                        break;
                        
                        case 'q':  //jezeli wejœcie dwustanowe
                              Buff_TX[BTS++]='q';
                              Buff_TX[BTS++]=(Net->HTTP.NetNodeSet[VSN].NodeNr/10)+48;
                              Buff_TX[BTS++]=(Net->HTTP.NetNodeSet[VSN].NodeNr%10)+48;
                              Buff_TX[BTS++]=(Net->HTTP.NetNodeSet[VSN].PortNr/10)+48;
                              Buff_TX[BTS++]=(Net->HTTP.NetNodeSet[VSN].PortNr%10)+48;
                              Buff_TX[BTS++]=';';
                              Net->HTTP.NetNodeSet[VSN].Value=PortStageRead(MainStage.StageQ[Net->HTTP.NetNodeSet[VSN].NodeNr].Port, Net->HTTP.NetNodeSet[VSN].PortNr); 
                              Buff_TX[BTS++]=Net->HTTP.NetNodeSet[VSN].Value+48;
                              Buff_TX[BTS++]=0xd;
                              Buff_TX[BTS++]=0xa;
                              ContentLength+=9;
                        break;
                        
                        case 'r':  //jezeli rejestr wejœciowy wêz³a
                              
                              Buff_TX[BTS++]='r';
                              Buff_TX[BTS++]=(Net->HTTP.NetNodeSet[VSN].NodeNr/10)+48;
                              Buff_TX[BTS++]=(Net->HTTP.NetNodeSet[VSN].NodeNr%10)+48;
                              Buff_TX[BTS++]=(Net->HTTP.NetNodeSet[VSN].RegNr/10)+48;
                              Buff_TX[BTS++]=(Net->HTTP.NetNodeSet[VSN].RegNr%10)+48;
                              Buff_TX[BTS++]=';';
                              
                              
                              if ((MainStage.StageI[Net->HTTP.NetNodeSet[VSN].NodeNr].Value[Net->HTTP.NetNodeSet[VSN].RegNr]==(signed short)0x8000) && (Net->HTTP.NetNodeSet[VSN].RegNr>0))
                              {
                                 Buff_TX[BTS++]='B';
                                 Buff_TX[BTS++]='A';
                                 Buff_TX[BTS++]='T';
                              }else{  
                               
                                if ((VariablesAnal[Net->HTTP.NetNodeSet[VSN].VarNr])>999)
                                {
                                  Buff_TX[BTS++]='>';
                                  Buff_TX[BTS++]='M';
                                  Buff_TX[BTS++]='A';
                                  Buff_TX[BTS++]='X';
                                }
                                else if ((VariablesAnal[Net->HTTP.NetNodeSet[VSN].VarNr])<-999)
                                {
                                  Buff_TX[BTS++]='-';
                                  Buff_TX[BTS++]='M';
                                  Buff_TX[BTS++]='I';
                                  Buff_TX[BTS++]='N';
                                }else{
                                  ilLit=sIntToStr(MainStage.StageI[Net->HTTP.NetNodeSet[VSN].NodeNr].Value[Net->HTTP.NetNodeSet[VSN].RegNr],&Buff_TX[BTS]);
                                  BTS+=ilLit;
   
                                for (char i=ilLit; i<4; i++) {Buff_TX[BTS++]=' ';}
                                }
                              }
                              
                              Buff_TX[BTS++]=0xd;
                              Buff_TX[BTS++]=0xa;
                              ContentLength+=12;
                                
                        break;
                        
                      }
                    }
               
                    ContentLength=BTS-NAG;
                    
                     Send=1;
                    char LengthTxt[4];
                    //char IlLiczb=CharToStr(ContentLength,LengthTxt,3);    //(char Value, char *Vstring, char ill) 
                    char IlLiczb=sprintf(LengthTxt, "%d",ContentLength);
                    
                    
                    for (char i=0; i<IlLiczb; i++)
                    {
                      Buff_TX[PozLength+i]=LengthTxt[i];
                    }
                    
          
                  Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
            break;              
          }
           
            if (Send)
            {
              
              Buff_TX[BTS++]=0x1B;
              Buff_TX[BTS++]='E';
              for (short i=0; i<US1_BUFFER_Rx_SIZE; i++) Buff_RX[i]=0;
              if (Net->Status.HostLink==CONNECTED)
              {
                  
                  Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
                  Net->Status.HttpNextQueryTime=g_counter+HTTP_QUERY_MAX_TIME;   //restart czasu pomiêdzy odpytaniami HTTP. Ustawienie Maksymalnego czasu na odpowiedŸ
                  
                  
                  AT91F_US_SendFrame(USART,Buff_TX,BTS,0, 0);                        //Wysyla ramke 
                  
                  Net->Status.SendPacketTime=g_counter;
                  Net->Status.SendBytes=BTS;
                  
              }
              
              if (WiFiDebug) 
              {
                SendViaUSB(Buff_TX, BTS);
                /*
                if (BTS>255)
                {
                 SendViaUSB(Buff_TX, 255);
                 //SendViaUSB(Buff_TX+255, BTS-255);
                }else{
                 SendViaUSB(Buff_TX, BTS); 
                }
                */
              }
            }
  
}



  




char HTTP_QueryResponse(char *Buff_RX, char *Buff_TX, AT91PS_USART USART, _Net *Net)
{

         char *pTxt;
    
         //if (WiFiDebug) SendViaUSB(Buff_RX, 580); 
         
         if ((Buff_RX[3]=='H') && (Buff_RX[4]=='T')&& (Buff_RX[5]=='T') && (Buff_RX[6]=='P')  ) //(strstr(Buff_RX, "HTTP")!=NULL)
         {

               Net->Status.ResPacketTime=g_counter;
              
               Net->Status.HttpNextQueryTime=g_counter+(Net->WiFi.HttpQueryPeriod*100); 
               pTxt=strstr(Buff_RX, "\r\n\r\n");
               if ((pTxt!=NULL) && (strstr(Buff_RX, "OK")!=NULL))
               {  
                 pTxt+=4;
                 switch (Net->Status.CurrentOrder)     
                       {
                          case HTTP_GET_Config:
                               pTxt+=3;
                               Net->HTTP.VarSetCount=0;    
                               for (char i=0; i<VAR_SET_AMOUNT; i++) {Net->HTTP.NetNodeSet[i].NetDataType=NO_Var;}  //wyczyszczenie tablicy 
                               //przepisanie konfiguracji do tabeli VarSet  
                                while (((pTxt[0]=='a') || (pTxt[0]=='b') || (pTxt[0]=='c') || (pTxt[0]=='d') || (pTxt[0]=='i') || (pTxt[0]=='q') || (pTxt[0]=='r')) && (Net->HTTP.VarSetCount<VAR_SET_AMOUNT))
                                {
                                   if ((pTxt[0]=='i') || (pTxt[0]=='q') || (pTxt[0]=='r'))  //je¿eli wejœcie lub wyjœcie wêz³a lub sensor
                                   {
                                     Net->HTTP.NetNodeSet[Net->HTTP.VarSetCount].NetDataType=pTxt[0];
                                     Net->HTTP.NetNodeSet[Net->HTTP.VarSetCount].NodeNr=(pTxt[1]-48)*10+(pTxt[2]-48);
                                     Net->HTTP.NetNodeSet[Net->HTTP.VarSetCount].PortNr=(pTxt[3]-48)*10+(pTxt[4]-48);
                                     Net->HTTP.VarSetCount++;
                                     pTxt+=9;
                                   }else{
                                     Net->HTTP.NetNodeSet[Net->HTTP.VarSetCount].NetDataType=pTxt[0];
                                     Net->HTTP.NetNodeSet[Net->HTTP.VarSetCount].VarNr=(pTxt[1]-48)*10+(pTxt[2]-48);
                                     Net->HTTP.VarSetCount++;
                                     pTxt+=7;
                                   }
                                }

                               for (char i=0; i<20; i++) Buff_RX[i]=0;
                               Net->Status.NextHttpOrder=HTTP_POST;
                               Net->Status.CurrentOrder=WizFi_NC;     //wyczyszczenie rozkazów
                               Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
                               Net->Status.HttpNextQueryTime=g_counter+(Net->WiFi.HttpQueryPeriod*100);   //restart czasu pomiêdzy odpytaniami HTTP.
                               
                          break; 
                          
                          case HTTP_GET: 
                     
                                pTxt=pTxt;
                                char ZmType=0;
                                char ZmNr=0;      //numer zmiennej lub wêz³a
                                char PortNr=0;
                                char IlCyfr;
                                short int NNW=StrToWord(pTxt, &IlCyfr);         //numer najnowszego wpisu
                                short int NW=NNW;
                                pTxt+=IlCyfr+1;
                                ZmType=*pTxt; pTxt++;
                                ZmNr=(pTxt[0]-48)*10+pTxt[1]-48;
                                PortNr=(pTxt[2]-48)*10+pTxt[3]-48;
                                pTxt+=3;
                                while (((ZmType=='a') || (ZmType=='b') || (ZmType=='c') || (ZmType=='d') || (ZmType=='e') || (ZmType=='q')) && (NW>Net->HTTP.LastForseOrderNr) && (Net->HTTP.LastForseOrderNr!=0) )
                                {
                                  switch (ZmType) 
                                  {
                                    case 'a': ////zmienne analogowe
                                        VariablesAnal[ZmNr]=StrToWord(pTxt, &IlCyfr);
                                        pTxt+=IlCyfr+2;
                                    break;
                                      
                                    case 'b':  //zmienne dwustanowe 
                                         
                                      if ((*pTxt=='1'))
                                       {
                                         Variables[ZmNr/8] |= 1<<(ZmNr%8);
                                       }
                                      else if ((*pTxt=='2'))
                                       {
                                         Variables[ZmNr/8] |= 1<<(ZmNr%8);
                                         VariablesImpulse[ZmNr/8] |= 1<<(ZmNr%8);
                                       }
                                       else
                                       { 
                                         Variables[ZmNr/8]&= ~(1<<(ZmNr%8));
                                       }
                                         pTxt+=3;
                                    break;  
                                    
                                    case 'c': //zmienne specjalne analogowe
             
                                        pTxt+=IlCyfr+2;
                                    break;
                                      
                                    case 'd': //zmienne specjalne dwustanowe
                                         if (*pTxt=='0') SetSpecialBinarVar(ZmNr, 0, Net); else SetSpecialBinarVar(ZmNr, 1, Net);   
                                         pTxt+=3;
                                    break;
                                    
                                    case 'e': //konfiguracja
                                      if (ZmNr==1)    //serwer ¿¹da pobrania nowej konfiguracji 
                                        {
                                          if (*pTxt=='0') Net->Status.NextHttpOrder=HTTP_GET_Config;
                                        }                               
                                         pTxt+=3;
                                    break;
                                    
                                    case 'q':  //zmienne dwustanowe 
                                      pTxt+=2;   
                                      if ((*pTxt=='1'))
                                       {
                                          PortStageSet((&(MainStage.StageQ[ZmNr].Port)),PortNr,1);
                                       } 
                                      else if ((*pTxt=='2'))
                                       {
                                          PortStageSet((&(MainStage.StageQ[ZmNr].Port)),PortNr,1);
                                          PortStageSet((&(MainStage.StageQImpulse[ZmNr])),PortNr,1);   //ustawienie flagi impuls
                                       }
                                       else
                                       { 
                                          PortStageSet((&(MainStage.StageQ[ZmNr].Port)),PortNr,0);
                                       }
                                       if (TransQuality.Mensur[ZmNr-1]>0)  MainStage.SendStageChange[ZmNr]=ChangeStageFlag;
                                       pTxt+=3;
                                    break;  
                                  }
                                  
                                  
                                  NW=StrToWord(pTxt, &IlCyfr);
                                  pTxt+=IlCyfr+1;
                                  ZmType=*pTxt; pTxt++;
                                  ZmNr=(pTxt[0]-48)*10+pTxt[1]-48;
                                  PortNr=(pTxt[2]-48)*10+pTxt[2]-48;
                                  pTxt+=3;
                                
                              }
                              
                              if (NNW>Net->HTTP.LastForseOrderNr) 
                              {
                                Net->HTTP.LastForseOrderNr=NNW;
                              }else if ((NNW==0) && (Net->HTTP.LastForseOrderNr>0))
                              {
                                Net->HTTP.LastForseOrderNr=0;
                                Net->Status.NextHttpOrder=HTTP_GET_Config;
                              }
                             
                              for (char i=0; i<20; i++) Buff_RX[i]=0;
                              Net->Status.CurrentOrder=WizFi_NC;     //wyczyszczenie rozkazów
                              Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
                              Net->Status.HttpNextQueryTime=g_counter+(Net->WiFi.HttpQueryPeriod*100);   //restart czasu pomiêdzy odpytaniami HTTP.
                          break; 
                          
                          case HTTP_POST:

                              for (char i=0; i<20; i++) Buff_RX[i]=0;
                              Net->Status.CurrentOrder=WizFi_NC;     //wyczyszczenie rozkazów
                              Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
                              Net->Status.HttpNextQueryTime=g_counter+(Net->WiFi.HttpQueryPeriod*100);   //restart czasu pomiêdzy odpytaniami HTTP.
                         break;
                         
                         
                     } //koniec switch (Net->Status.CurrentOrder)
                 
                  
                    //Zg³oszone zamkniête po³¹czenie
                 
                    if ((strstr(Buff_RX, "Connection: close")!=NULL) /*&& (Net->Status.HostLink==CONNECTED)*/) 
                    {
                      Net->Status.FreezTime=g_counter+FREEZ_TIME;
                       Net->Status.HostLink=DISCONNECTING;                             //Roz³¹czenie
                    }
                
                 
                 
                 Net->Status.ResBytes=(int)pTxt -(int)Buff_RX;
               } //koniec if(pTxt!=NULL)
               

         
         } 
         
      
  
}



void HTTP_CheckConnection(char *Buff_TX, AT91PS_USART USART, _Net *Net)
{
   Net->Status.FreezTime=g_counter+FREEZ_TIME; //rezerwowanie czasu
   Net->Status.CurrentOrder=WizFi_AT_CID;     
   AT91F_US_SendFrame(USART,(char*)AT_CID,sizeof(AT_CID)-1,0, 0);                        //Wysyla ramke  
   Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
   
}

char HTTP_CheckConnectionResponse(char *Buff_RX, char *Buff_TX, AT91PS_USART USART, _Net *Net)
{
   char odp=0;


        if ((strstr(Buff_RX, "[ERROR")!=NULL) || (strstr(Buff_RX, "No valid Cids")!=NULL))    //((Buff_RX, "[ERROR")!=NULL) ||       strstr(Buff_RX, "No valid Cids")!=NULL)
        {
          for (char i=0; i<50; i++) Buff_RX[i]=0;
          Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
          Net->Status.FreezTime=0;
          Net->Status.HostLink=DISCONNECTED;
          Net->Status.CurrentOrder=WizFi_NC;
        }else if(strstr(Buff_RX, "[OK")!=NULL)
        {
          odp=1;  //ok 
          for (char i=0; i<50; i++) Buff_RX[i]=0;
          Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
          Net->Status.CurrentOrder=WizFi_NC;
          HTTP_Task(Buff_RX, Buff_TX,USART, Net);                                //wykonaj kolejne zadanie HTTP
          Net->Status.FreezTime=0;
        }
          
  
   return odp;
}
