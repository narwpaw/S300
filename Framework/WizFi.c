//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                 CEURON                                                   *//
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : WizFi.c                                 *//
//*  Opis                         : Obs³uga modu³u WizFi                      *//
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

#include "WizFi.h"
#include "Framework.h"
#include "core/device.h"                                                        //aduje bibliotreki dla odpowiedniego procesora
#include "main_H.h" 
#include "string.h"
#include "WizFiCommand.h"
#include "serial_driver.h"




/*
  extern char Variables[]; 
  extern BYTE Buff_TX_1[];
  extern short int VariablesAnal[];
  extern unsigned long long  g_counter;
  long long WiFi_TimeOut_Counter;
  
  
  
        
 _WiFi WiFi;
 _WiFiStatus  WiFiStat;
 _HTTP  HTTP;
 //_SSID   g_SSID_List[20];
*/ 


extern unsigned long long  g_counter;                                              //globalny licznik milisekundowy od w³¹czenia/restartu sterownika
extern char Variables[];    //zmienne
extern short int VariablesAnal[];    //zmienne
extern _ProgramTab gProg;
extern _MainStage  MainStage; 
char WiFiDebug=0;
  //
//funkcja CheckVarChangeHTTP --------------------------------
//funkcja sprawdza czy zmieni³y siê zmienne sprawdzane przez strone web. Je¿eli tak to wyœlij zmiany
void CheckVarChangeHTTP(char *SendBuff,  AT91PS_USART USART, _Net *Net)
{

  static long long AnalogPeriod;
  
  if (Net->Status.HostLink==CONNECTED)
  {
     char Zmiana=0;
     for (char IDX=0; IDX<Net->HTTP.VarSetCount; IDX++)
     {  
       switch (Net->HTTP.NetNodeSet[IDX].NetDataType)
       {                  
          case 'b': 
               if (((Variables[Net->HTTP.NetNodeSet[IDX].VarNr/8]>>(Net->HTTP.NetNodeSet[IDX].VarNr%8))&0x1) != (Net->HTTP.NetNodeSet[IDX].Value)) {Zmiana++;}
          break;
          case 'a': 
            if (AnalogPeriod<g_counter) {Zmiana++; AnalogPeriod=g_counter+30000;}
          break;  
          case 'c': 
            if (AnalogPeriod<g_counter) {Zmiana++; AnalogPeriod=g_counter+30000;}
          break; 
          
          case 'd': 
            if (GetSpecialBinarVar(Net->HTTP.NetNodeSet[IDX].NodeNr)!=Net->HTTP.NetNodeSet[IDX].Value) Zmiana++;
          break; 
          
          case 'i': 
             if ( (PortStageRead(MainStage.StageI[Net->HTTP.NetNodeSet[IDX].NodeNr].Port, Net->HTTP.NetNodeSet[IDX].PortNr)) != (Net->HTTP.NetNodeSet[IDX].Value)) {Zmiana++;}
          break; 
          
          case 'q': 
             if ( (PortStageRead(MainStage.StageQ[Net->HTTP.NetNodeSet[IDX].NodeNr].Port, Net->HTTP.NetNodeSet[IDX].PortNr)) != (Net->HTTP.NetNodeSet[IDX].Value)) {Zmiana++;}
          break; 
          
          
       }   
     }
   
     if (Zmiana>0) 
     {
       if (Net->Status.HttpNextQueryTime>g_counter+100) Net->Status.HttpNextQueryTime=g_counter+100;  //przyœpieszenie wys³ania danych
       Net->Status.NextHttpOrder=HTTP_POST;
     }
  }

}



    
    
char *pt1;    
char *pt2;
void InitWizFi(AT91PS_USART USART, _Net *Net)
{
  //Net->WiFi.HttpQueryPeriod=5000;    //[ms] okres odpytywania hosta
  Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
     
   pt1=(char*)&Net->WiFi;
   pt2=(char*)(&AT91C_IFLASH_MEM->Config.WiFi);
  for (short i=0; i<sizeof(_WiFi); i++) pt1[i]=pt2[i];
  
  
  Net->WiFi.On=AT91C_IFLASH_MEM->Config.WiFi.On;
  Net->WiFi.HttpQueryPeriod=AT91C_IFLASH_MEM->Config.WiFi.HttpQueryPeriod;
  if ((Net->WiFi.HttpQueryPeriod==0) || (Net->WiFi.HttpQueryPeriod>60)) Net->WiFi.HttpQueryPeriod=8;              //je¿eli 0 to ustaw 8 czyli 800ms   
}   


 

//G³ówna funkcja WiFi
//Funkcja wywo³ywania ci¹gle w pêtli g³ównej programu
void WiFi(char *Buff_RX, char *Buff_TX,  AT91PS_USART USART, _Net *Net)
{
     if (Net->Status.Error>3)    //przekroczenie dopuszczalnej iloœci b³êdów
     {
        Net->Status.Error=0;
        Net->Status.HostLink=DISCONNECTED;
        Net->Status.AccesPointAssociate=DISCONNECTED;
        Net->Status.Timeout=0;
        Net->Status.HttpNextQueryTime=0;
        Net->Status.FreezTime=0;
     }
  
  
     //sprawdzenie Timeoutu
     if ((Net->Status.FreezTime==0) || (Net->Status.FreezTime<g_counter))
     {  
         if ((Net->Status.Timeout<g_counter) && (Net->Status.Timeout>0))
         {//Timeout przekroczony
           
            Net->Status.CurrentOrder=WizFi_NC;
            Net->Status.RepeatOrder=0; 
           
            //je¿eli utracono po³¹czenie z hostem
            if (Net->Status.AccesPointAssociate==CONNECTED)                             //je¿eli brak po³¹czenia z serwerem hosta
                {
                   Net->Status.HostLink=DISCONNECTED;
                   Net->Status.Timeout=0;
                   Net->Status.HttpNextQueryTime=0;
                   Net->Status.FreezTime=0;
                   Net->Status.Error++;
                }else{
                   Net->Status.AccesPointAssociate=DISCONNECTED;
                   Net->Status.Timeout=0;
                   Net->Status.HttpNextQueryTime=0;
                   Net->Status.FreezTime=0;
                }
         }else{
            if (Net->Status.AccesPointAssociate==DISCONNECTED)                      //je¿eli brak po³¹czenia z acces pointem to zainicjalizuj ³¹czenie
            {
                for (short i=0; i<sizeof(Buff_RX); i++) Buff_RX[i]=0;
                Net->Status.AccesPointAssociate=CONNECTING;                         //zmiana statusu RouterAssociate na ³¹czenie 
                Net->Status.HostLink=DISCONNECTED;                                  //reset flagi stanu po³¹czenie z hostem
                Net->Status.CurrentOrder=WizFi_NC;                                  //reset flagi rozkazu
                Net->Status.RepeatOrder=0;
                Net->Status.NextHttpOrder=WizFi_NC;                                 //reset flagi oczekuj¹cego rozkazu
                
                AP_Associate(Buff_RX, Buff_TX,USART,Net);                                    //inicjalizacja po³¹czenie z acces point'em                                                                                                         
            }else if (Net->Status.AccesPointAssociate==CONNECTED) 
            {
              if (Net->Status.HostLink==DISCONNECTED)                               //je¿eli brak po³¹czenia z serwerem hosta
                {
                   WizFiCommand_AT_NCTCP(Buff_RX, Buff_TX,USART, Net); 
                }else if (Net->Status.HostLink==CONNECTED)                          //je¿eli po³¹czony z serwerem hosta
                {
                   if( Net->Status.CurrentOrder==WizFi_NC)                          //je¿eli brak bierz¹cych zadañ
                   {                                                             //HTTP_CheckConnection(Buff_TX,  USART, Net);
                        HTTP_Task(Buff_RX, Buff_TX,USART, Net);                                //wykonaj kolejne zadanie HTTP
                   }
                }
            }
           
         } //koniec: if ((Net->Status.Timeout<g_counter) && (Net->Status.Timeout>0))
     } //koniec: if ((Net->Status.FreezTime==0) || (Net->Status.FreezTime<g_counter))
  
}



 extern long long FuseLastWiFiDataIn;  //zabezpieczenie przed blokad¹ DMA USART. FuseLastWiFiDataIn zawiera czas ostatniej odpowiedzi od usarta. Je¿eli WiFi jest w³¹czone a USART milczy od 30s to restartuj mikroprocesor
              


//G³ówna funkcja interpretacji odpowiedzi
void WizFiResponseInterpreter(char *Buff_RX, char *Buff_TX,  AT91PS_USART USART, _Net *Net)
{
      char *pTxt;
      
      if (WiFiDebug) SendViaUSB(Buff_RX, 580);
     

      FuseLastWiFiDataIn=g_counter;
      
      if ((strstr(Buff_RX, "[DISASSOCIATED]")!=NULL) || (strstr(Buff_RX, "[ERROR]")!=NULL))
      {
   
         Net->Status.AccesPointAssociate=DISCONNECTED;
         Net->Status.HostLink=DISCONNECTED; 
         Net->Status.FreezTime=0;
         Net->Status.Timeout=0;
         Net->Status.CurrentOrder=WizFi_NC;
         
      }
      
      

      else if (Net->Status.AccesPointAssociate==CONNECTING)
      {
         AP_AssociateResponseInterpreter(Buff_RX,Buff_TX,USART, Net);           //odpowiedz dla zadania 
      }else if (Net->Status.AccesPointAssociate==CONNECTED) 
      {
          if (strstr(Buff_RX, "send() failed:")!=NULL)
          {
            //for (short i=0; i<sizeof(Buff_RX); i++) Buff_RX[i]=0;
            Net->Status.CurrentOrder=WizFi_NC;
            Net->Status.FreezTime=0;
            Net->Status.Timeout=0;
            Net->Status.NextHttpOrder=0;
            Net->Status.HostLink=DISCONNECTED; 
          }
          else if (Net->Status.HostLink==CONNECTING)                            //je¿eli ³¹czenie z hostem
          {
            WizFiResponseCommand_AT_NCTCP(Buff_RX,Buff_TX,USART, Net);          //odpowiedz dla zadania 
            
          }
          else if (Net->Status.HostLink==CONNECTED)                             //je¿eli po³¹czono z hostem
          {
             pTxt=strstr(Buff_RX, "[DISCONNECT");                               //jê¿eli porzysz³a informacja o roz³¹czeniu
             if ((pTxt!=NULL) && (Net->Status.HostLink==CONNECTED))
             {
                if(pTxt[12]==Net->Status.HostChannel) 
                 {
                   for (short i=0; i<sizeof(Buff_RX); i++) Buff_RX[i]=0; 
                   Net->Status.FreezTime=g_counter+FREEZ_TIME;
                   Net->Status.HostLink=DISCONNECTED;                           //Roz³¹czenie
                 }
              }
             
             
             if (strstr(Buff_RX, "[ERROR: INVALID INPUT]")!=NULL)        //jê¿eli porzysz³a informacja o b³edzie
              {
                  for (short i=0; i<sizeof(Buff_RX); i++) Buff_RX[i]=0;
                  Net->Status.FreezTime=g_counter+FREEZ_TIME;
                  Net->Status.HostLink=DISCONNECTED;                            //Roz³¹czenie
                  Net->Status.Error++;
              }
              else
              {
                  HTTP_QueryResponse(Buff_RX,Buff_TX,USART, Net); 
              }
                                                                                //if (Net->Status.CurrentOrder==WizFi_AT_CID)
                                                                                //{
                                                                                //   HTTP_CheckConnectionResponse(Buff_RX,Buff_TX,USART, Net);         
                                                                                //}else{
                                                                                //     HTTP_QueryResponse(Buff_RX,Buff_TX,USART, Net); 
                                                                                //}
          }
          else if (Net->Status.HostLink==DISCONNECTING)
          {
             WizFiResponseCommand_AT_NCLOSEALL0(Buff_RX,Buff_TX,USART, Net);
          }
      }                                                                         //koniec: else if (Net->Status.AccesPointAssociate==CONNECTED) 
}







//funkcja ³¹czenia z acces point'em
void AP_Associate(char *Buff_RX, char *Buff_TX,  AT91PS_USART USART, _Net *Net)
{
      Net->Status.AccesPointAssociate=CONNECTING;                          //zmiana statusu RouterAssociate na ³¹czenie 
      
      
      switch (Net->Status.CurrentOrder)
      {
        case WizFi_NC:           WizFiCommand_AT(Buff_RX, Buff_TX,USART, Net);   break;           //kolejny rozkaz
        case WizFi_AT:           WizFiCommand_AT(Buff_RX, Buff_TX,USART, Net);   break;           //kolejny rozkaz
        case WizFi_AT_VER:       WizFiCommand_AT_VER(Buff_RX, Buff_TX,USART, Net);   break;           //wyœlij rozkaz rozkaz
        case WizFi_ATI2:         WizFiCommand_ATI2(Buff_RX, Buff_TX,USART, Net);   break;           //wyœlij rozkaz rozkaz
        case WizFi_AT_XDUM0:     WizFiCommand_AT_XDUM0(Buff_RX, Buff_TX,USART, Net);   break;           //wyœlij rozkaz rozkaz
        case WizFi_AT_WD:        WizFiCommand_AT_WD(Buff_RX, Buff_TX,USART, Net);   break;           //wyœlij rozkaz rozkaz
        case WizFi_AT_NSET:      WizFiCommand_AT_NSET(Buff_RX, Buff_TX,USART, Net);   break;           //wyœlij rozkaz rozkaz
        case WizFi_AT_NDHCP:     WizFiCommand_AT_NDHCP(Buff_RX, Buff_TX,USART, Net);   break;           //wyœlij rozkaz rozkaz
        case WizFi_AT_WAUTH_1:   WizFiCommand_AT_WAUTH_1(Buff_RX, Buff_TX,USART, Net);   break;           //wyœlij rozkaz rozkaz
        case WizFi_AT_WWEP1:     if (WizFiCommand_AT_WWEP1(Buff_RX, Buff_TX,USART, Net))  {break;}           //wyœlij rozkaz rozkaz
        case WizFi_AT_WWPA:      if (WizFiCommand_AT_WWPA(Buff_RX, Buff_TX,USART, Net))  {break;}            //wyœlij rozkaz rozkaz
        case WizFi_AT_WA:        WizFiCommand_AT_WA(Buff_RX, Buff_TX,USART, Net);   break;           //wyœlij rozkaz rozkaz
        case WizFi_AT_DNSLOOKUP: WizFiCommand_AT_DNSLOOKUP(Buff_RX, Buff_TX,USART, Net);   break;           //wyœlij rozkaz rozkaz
      }
      
      
}

          




void AP_AssociateResponseInterpreter(char *Buff_RX, char *Buff_TX,  AT91PS_USART USART, _Net *Net)
{
      Net->Status.FreezTime=g_counter+FREEZ_TIME;
      char Odp=0;
      switch (Net->Status.CurrentOrder)
      {
        case WizFi_AT:            Odp=WizFiResponseCommand_AT(Buff_RX,Buff_TX,USART, Net);   break;        //odpowiedz dla zadania 
        case WizFi_AT_VER:        Odp=WizFiResponseCommand_AT_VER(Buff_RX,Buff_TX,USART, Net);   break;        //odpowiedz dla zadania 
        case WizFi_ATI2:          Odp=WizFiResponseCommand_ATI2(Buff_RX,Buff_TX,USART, Net);   break;        //odpowiedz dla zadania 
        case WizFi_AT_XDUM0:      Odp=WizFiResponseCommand_AT_XDUM0(Buff_RX,Buff_TX,USART, Net);   break;        //odpowiedz dla zadania 
        case WizFi_AT_WD:         Odp=WizFiResponseCommand_AT_WD(Buff_RX,Buff_TX,USART, Net);   break;     //odpowiedz dla zadania 
        case WizFi_AT_NSET:       Odp=WizFiResponseCommand_AT_NSET(Buff_RX,Buff_TX,USART, Net);   break;     //odpowiedz dla zadania 
        case WizFi_AT_NDHCP:      Odp=WizFiResponseCommand_AT_NDHCP(Buff_RX,Buff_TX,USART, Net);   break;     //odpowiedz dla zadania 
        case WizFi_AT_WAUTH_1:    Odp=WizFiResponseCommand_AT_WAUTH_1(Buff_RX,Buff_TX,USART, Net);   break;     //odpowiedz dla zadania 
        case WizFi_AT_WWEP1:      Odp=WizFiResponseCommand_AT_WWEP1(Buff_RX,Buff_TX,USART, Net);   break;     //odpowiedz dla zadania 
        case WizFi_AT_WWPA:       Odp=WizFiResponseCommand_AT_WWPA(Buff_RX,Buff_TX,USART, Net);   break;     //odpowiedz dla zadania 
        case WizFi_AT_WA:         Odp=WizFiResponseCommand_AT_WA(Buff_RX,Buff_TX,USART, Net);   break;     //odpowiedz dla zadania 
        case WizFi_AT_DNSLOOKUP:  Odp=WizFiResponseCommand_AT_DNSLOOKUP(Buff_RX,Buff_TX,USART, Net);   break;     //odpowiedz dla zadania 
      }
         
      
      //je¿eli poprawna odpowiedŸ 
      if (Odp==1)
      {
        Net->Status.RepeatOrder=0;
        if ((Net->Status.CurrentOrder<WizFi_AT_DNSLOOKUP) && (Net->Status.CurrentOrder!=WizFi_NC))
        {
           
           Net->Status.CurrentOrder++;
           
           if ((Net->WiFi.IpNotAuto==0) &&  (Net->Status.CurrentOrder==WizFi_AT_NSET))
            { 
              Net->Status.CurrentOrder++;
            }
           
           if (Net->Status.CurrentOrder==WizFi_AT_WWEP1)
           {
             char PassLen=0;
             while ((Net->WiFi.Key[PassLen]>0) && (PassLen<=15)) PassLen++;
             if ((PassLen!=5) && (PassLen!=10) && (PassLen!=13)) Net->Status.CurrentOrder++;
             
           }
           
           
           AP_Associate(Buff_RX, Buff_TX,  USART, Net);
           
        }
      }else{
        if(strstr(Buff_RX, "[ERROR")!=NULL)
        {
            if (Net->Status.RepeatOrder<10)      //powtórz rozkaz
            {
               Net->Status.RepeatOrder++;
               AP_Associate(Buff_RX, Buff_TX,  USART, Net);
               Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
               Net->Status.FreezTime=g_counter+FREEZ_TIME;
            }else{
               //ponowne po³¹czenie
               Net->Status.FreezTime=g_counter+FREEZ_TIME;
               Net->Status.Timeout=g_counter + CRYTICAL_TIMEOUT;    //przed³u¿enie /restart timeoutu
               Net->Status.CurrentOrder=WizFi_NC;
               Net->Status.NextHttpOrder=WizFi_NC;
               Net->Status.RepeatOrder=0;  
               Net->Status.HostLink=DISCONNECTED; 
               Net->Status.AccesPointAssociate=DISCONNECTED;
            }
          
        }
      }  
      
}
            





//funkcja wykonywania zadañ HTTP
void HTTP_Task(char *Buff_RX, char *Buff_TX,  AT91PS_USART USART, _Net *Net)
{
    if ((Net->Status.HttpNextQueryTime<=g_counter) || (Net->Status.HttpNextQueryTime==0))   //je¿eli spe³niony wymagany czas pomiêdzy odpytywaniami
    {
        if (Net->Status.NextHttpOrder!=WizFi_NC)                                    //je¿eli jest oczekuj¹ce zadanie
        {
            Net->Status.CurrentOrder=Net->Status.NextHttpOrder;
            Net->Status.NextHttpOrder=WizFi_NC;
        }else{
            Net->Status.CurrentOrder=HTTP_GET;
        }
        
        HTTP_Query(Buff_RX, Buff_TX,  USART, Net);  
        
        
    }
    
}

