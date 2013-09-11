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
//#include "core/trace.h"
#include "main_H.h" 
#include "string.h"

  extern char Variables[]; 
  extern BYTE Buff_TX_1[];
  extern short int VariablesAnal[];
  extern unsigned long long  g_counter;
  long long WiFi_TimeOut_Counter;
  
  
  
        
 _WiFi WiFi;
 _WiFiStatus  WiFiStat;
 _HTTP  HTTP;
 //_SSID   g_SSID_List[20];
 
void InitWizFi(AT91PS_USART USART)
{
    
  WiFi.Key[0]='0';
  WiFi.Key[1]='1';
  WiFi.Key[2]='5';
  WiFi.Key[3]='8';
  WiFi.Key[4]='0';
  WiFi.Key[5]='0';
  WiFi.Key[6]='0';
  WiFi.Key[7]='0';
  WiFi.Key[8]='0';
  WiFi.Key[9]='0';
  WiFi.Key[10]=0;
  
  
  WiFi.SSID[0]='d';
  WiFi.SSID[1]='e';
  WiFi.SSID[2]='f';
  WiFi.SSID[3]='a';
  WiFi.SSID[4]='u';
  WiFi.SSID[5]='l';
  WiFi.SSID[6]='t';
  WiFi.SSID[7]=0;

   /*
  WiFi.Domena[0]='c';
  WiFi.Domena[1]='e';
  WiFi.Domena[2]='u';
  WiFi.Domena[3]='r';
  WiFi.Domena[4]='o';
  WiFi.Domena[5]='n';
  WiFi.Domena[6]='.';
  WiFi.Domena[7]='p';
  WiFi.Domena[8]='l';
  WiFi.Domena[9]=0;
                */
  
  WiFi.Domena[0]='d';
  WiFi.Domena[1]='o';
  WiFi.Domena[2]='r';
  WiFi.Domena[3]='y';
  WiFi.Domena[4]='w';
  WiFi.Domena[5]='c';
  WiFi.Domena[6]='z';
  WiFi.Domena[7]='a';
  WiFi.Domena[8]='k';
  WiFi.Domena[9]='.';
  WiFi.Domena[10]='p';
  WiFi.Domena[11]='l';
  WiFi.Domena[12]=0;
  
  WiFi.Path[0]='/';
  WiFi.Path[1]='T'; 
  WiFi.Path[2]='e'; 
  WiFi.Path[3]='s';   
  WiFi.Path[4]='t'; 
  WiFi.Path[5]=0; 
  
  
  
  
  
  
  
   SetWiFiSuspend();
  
   Buff_TX_1[0]='A';
   Buff_TX_1[1]='T';
   Buff_TX_1[2]=0x0d;
   Buff_TX_1[3]=0x0A;
   
   WiFiStat.Status=WizFi_AT_CONFIG;
   WiFiStat.Command=WizFi_AT;
   HTTP.OrderType=HTTP_GET_Config;
   AT91F_US_SendFrame(USART,Buff_TX_1,4,0, 0);                        //Wysyla ramke  
}   



void ConnectAccespoint(AT91PS_USART USART)
{
   Buff_TX_1[0]='A';
   Buff_TX_1[1]='T';
   Buff_TX_1[2]=0x0d;
   Buff_TX_1[3]=0x0A;
   
   WiFiStat.Status=WizFi_AT_CONFIG;
   WiFiStat.Command=WizFi_AT;
   HTTP.OrderType=HTTP_NO_INIT;
   AT91F_US_SendFrame(USART,Buff_TX_1,4,0, 0);                        //Wysyla ramke  
} 


char CheckStatus(void)
{
   return  WiFiStat.Status;
}


void SetWiFiSuspend(void)
{
  WiFiStat.Ready=0;
}

void SetWiFiResume(void)
{
  WiFiStat.Ready=1;
}



int SSID_ListCheckTimeout;
void WiFi_SSID_ListCheck(AT91PS_USART USART)
{
   
   if ((WiFiStat.SSID_Check==1) && (g_counter>SSID_ListCheckTimeout))
   {
       WiFiStat.Command=WizFi_AT_WS;
       AT91F_US_SendFrame(USART,(char*)AT_WS,sizeof(AT_WS)-1,0, 0);                        //Wysyla ramke  
       WiFiStat.SSID_Check=0;
   }

}


void WiFi_SSID_ListCheckStart(void)
{
   WiFiStat.Ready=0;
   SSID_ListCheckTimeout=g_counter+5000;
   WiFiStat.SSID_Check=1;
}






void ConnectWithHost(char *SendBuff,  AT91PS_USART USART)
{
  //³¹czenie z hostem -------
  if (HTTP.Status!=HTTP_HOST_CONNECTING)
  {
           char BTS=0;
           for (char i=0; i<sizeof(AT_NCTCP); i++)  {SendBuff[i]=AT_NCTCP[i];   BTS++;}
           BTS--;
           char PN=0;
           while ((WiFiStat.DomenaIP[PN]>0) && (PN<=15)) {SendBuff[BTS+PN]=WiFiStat.DomenaIP[PN]; PN++;}
           SendBuff[BTS+PN++]=',';
           SendBuff[BTS+PN++]='8';
           SendBuff[BTS+PN++]='0';
           SendBuff[BTS+PN++]=0x0d;
          
           //ClearBuff(US1_Buffer);
           AT91F_US_SendFrame(USART,SendBuff,BTS+PN,0, 0);                        //Wysyla ramke 
           WiFi_TimeOut_Counter=g_counter+WiFi_TIMEOUT;
           WiFiStat.Command=WizFi_AT_NCTCP;
           HTTP.Status=HTTP_HOST_CONNECTING;
           
           
           //-------------------------- 
  } else if (WiFi_TimeOut_Counter<g_counter)
  {
        //  WiFiStat.Command=WizFi_AT_NCLOSEALL;
        //  AT91F_US_SendFrame(USART,(char*)AT_NCLOSEALL,sizeof(AT_NCLOSEALL)-1,0, 0); 
  }
}

void CheckConnectionStatus(void)
{
if (HTTP.Status==HTTP_HOST_CONNECTED)
          {
            AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; 
          }else{
            
            AT91C_BASE_PIOA->PIO_CODR =SpeakerPin; 
          } 
  
}


long long NextGetTime;
//funkcja inicjalizuje pobranie forsowañ zmiennych z http
void HTTP_Connect(char *SendBuff,  AT91PS_USART USART)
{
 
  
  if (WiFiStat.Ready)
   if ((WiFiStat.Status==WizFi_AT_CONNECTED) && (WiFiStat.DomenaIP[0]!=0) 
       && (((HTTP.OrderType==HTTP_NULL) && (HTTP.Status!=HTTP_HOST_CONNECTING)) || (WiFi_TimeOut_Counter<g_counter)) 
       && ((NextGetTime<g_counter) || ( HTTP.WaitingPost==1))  
       )
   { 
                
      if (HTTP.Status==HTTP_HOST_CONNECTED)  
      {
          SendHttpQwery(SendBuff,USART);
      }else if (HTTP.Status==HTTP_HOST_DISCONNECT)
      {
     
         NextGetTime=g_counter + WiFi_CONNECT_PERIOD;
     
          //³¹czenie z hostem -------
           char BTS=0;
           HTTP.OrderType=HTTP_GET;
           ConnectWithHost(SendBuff, USART);
      }  
     
     
     
   }
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









   //funkcja inicjalizuje pobranie forsowañ zmiennych z http
void PostHTTP_Force(char *SendBuff,  AT91PS_USART USART)
{
  /*
  if (WiFiStat.Ready)
   if ((WiFiStat.Status==WizFi_AT_CONNECTED) && (WiFiStat.DomenaIP[0]!=0) && ((HTTP.OrderType==HTTP_NULL) || (WiFi_TimeOut_Counter<g_counter)))
   {
          //³¹czenie z hostem -------
           HTTP.OrderType=HTTP_POST;
           ConnectWithHost(SendBuff, USART);
   }else if ((WiFiStat.Status==WizFi_AT_CONNECTED) && (WiFiStat.DomenaIP[0]!=0))
   {
       HTTP.WaitingPost=1; 
   }
  */
    HTTP.WaitingPost=1; 
}




//funkcja CheckVarChangeHTTP --------------------------------
//funkcja sprawdza czy zmieni³y siê zmienne sprawdzane przez strone web. Je¿eli tak to wyœlij zmiany
void CheckVarChangeHTTP(char *SendBuff,  AT91PS_USART USART)
{
  static long long AnalogPeriod;
  
  if (WiFiStat.Ready)
  {
     char Zmiana=0;
     for (char IDX=0; IDX<HTTP.VarSetCount; IDX++)
     {  
       switch (HTTP.VarSet[IDX].VarType)
       {                  
          case 'b': 
               if (((Variables[HTTP.VarSet[IDX].VarNr/8]>>(HTTP.VarSet[IDX].VarNr%8))&0x1) != (HTTP.VarSet[IDX].Value)) {Zmiana++;}
          break;
          case 'a': 
            if (AnalogPeriod<g_counter) {Zmiana++; AnalogPeriod=g_counter+30000;}
          break;  
       }   
     }
   
     if (Zmiana>0) PostHTTP_Force(SendBuff, USART);
  }
}






  

void SendHttpQwery(char *SendBuff,  AT91PS_USART USART)
{
  
           if (HTTP.Status==HTTP_HOST_CONNECTED)
           {  
             
             
           
           //wysy³anie danych ------------------------------------
           
           char BTS=0;
           for (short i=0; i<sizeof(SendBuff); i++) SendBuff[i]=0;
           SendBuff[BTS++]=0x1B;
           SendBuff[BTS++]='S';
           SendBuff[BTS++]=WiFiStat.HostChannel;     //numer kana³u [przydzielony przez serwer
           
            if ((HTTP.WaitingPost) && (HTTP.OrderType!=HTTP_GET_Config)) HTTP.OrderType=HTTP_POST;
           
           char Send=0; 
            
           //czyszczenie bufora nadawczego
           char *PozIlByte;
           switch (HTTP.OrderType)
           {
              case HTTP_GET_Config:              //proœba o podanie konfiguracji zestawu zmiennych
                    strcat(SendBuff,"GET /Test/dc");
                    strcat(SendBuff,"1234567");              //do zmiany (wpisaæ prawdziwe ID)
                    strcat(SendBuff,".csv HTTP/1.1\r\n");
                    strcat(SendBuff,"User-Agent: CEURON\r\nHost: ");
                    strcat(SendBuff,WiFi.Domena);
                    strcat(SendBuff,"\r\nKeep-Alive: 2\r\n");
                    strcat(SendBuff,"Connection: keep-alive\r\n");
                    strcat(SendBuff,"\r\n\r\n");
                    BTS=strlen(SendBuff);
                    Send=1;
              break; 
              
              case HTTP_GET:              //proœba o podanie konfiguracji zestawu zmiennych
               
                    strcat(SendBuff,"GET /Test/do");
                    strcat(SendBuff,"1234567");              //do zmiany (wpisaæ prawdziwe ID)
                    strcat(SendBuff,".csv HTTP/1.1\r\n");
                    //strcat(SendBuff,".html HTTP/1.1\r\n");
                    strcat(SendBuff,"User-Agent: CEURON\r\nHost: ");
                    strcat(SendBuff,WiFi.Domena);
                    strcat(SendBuff,"\r\nKeep-Alive: 2\r\n");
                    strcat(SendBuff,"Connection: keep-alive\r\n");
                    strcat(SendBuff,"\r\n\r\n");
                    BTS=strlen(SendBuff);
                     Send=1;
              break; 
              
              case HTTP_POST:                   //wys³anie zmiennych
                    strcat(SendBuff,"POST ");
                    strcat(SendBuff,WiFi.Path);
                    strcat(SendBuff,"/putgate.php HTTP/1.1\r\nUser-Agent: CEURON\r\nHost: www.");
                    strcat(SendBuff,WiFi.Domena);
                    strcat(SendBuff,"\r\nContent-Length: ");
                    char PozLength=strlen(SendBuff);
            
                    strcat(SendBuff,"   ");
                    strcat(SendBuff,"\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nNodeID=");
                    strcat(SendBuff,"1234567");
                    strcat(SendBuff,"&Order=1&Content=;\r\n");
                    BTS=strlen(SendBuff);
                    //oko³o 183 byte do tej pory
                    char ContentLength=34;
                    for (char VSN=0; VSN<HTTP.VarSetCount; VSN++)
                    {
                      switch (HTTP.VarSet[VSN].VarType)
                      {
                        case 'a':  //jezeli zmienna analogowa
                              
                              SendBuff[BTS++]='a';
                              SendBuff[BTS++]=(HTTP.VarSet[VSN].VarNr/10)+48;
                              SendBuff[BTS++]=(HTTP.VarSet[VSN].VarNr%10)+48;
                              SendBuff[BTS++]=';';
                              char ilLit=sIntToStr(VariablesAnal[HTTP.VarSet[VSN].VarNr],&SendBuff[BTS]);
                              BTS+=ilLit;
                              for (char i=ilLit; i<4; i++) {SendBuff[BTS++]=' ';}
                              SendBuff[BTS++]=0xd;
                              SendBuff[BTS++]=0xa;
                              ContentLength+=10;
                                
                        break;
                        
                        case 'b':  //jezeli zmienna analogowa
                              SendBuff[BTS++]='b';
                              SendBuff[BTS++]=(HTTP.VarSet[VSN].VarNr/10)+48;
                              SendBuff[BTS++]=(HTTP.VarSet[VSN].VarNr%10)+48;
                              SendBuff[BTS++]=';';
                              HTTP.VarSet[VSN].Value=((Variables[HTTP.VarSet[VSN].VarNr/8]>>(HTTP.VarSet[VSN].VarNr%8))&0x1);
                              SendBuff[BTS++]=HTTP.VarSet[VSN].Value+48;
                              SendBuff[BTS++]=0xd;
                              SendBuff[BTS++]=0xa;
                              ContentLength+=7;
                                
                              //SendBuff[BTS++]=
                                
                        break;

                        
                      }
                      
                      
                    }
                    
                     Send=1;
                    char LengthTxt[4];
                    char IlLiczb=CharToStr(ContentLength,LengthTxt,3);    //(char Value, char *Vstring, char ill) 
                    
                    for (char i=0; i<IlLiczb; i++)
                    {
                      SendBuff[PozLength+i]=LengthTxt[i];
                    }
                    
            
            break;              
          }
           
            if ( Send)
            {
           SendBuff[BTS++]=0x1B;
           SendBuff[BTS++]='E';


           
              HTTP.Status=HTTP_SENDING_QUERY;
              WiFiStat.Command=WizFi_SendData;
              WiFi_TimeOut_Counter=g_counter+WiFi_TIMEOUT;
              
             
              AT91F_US_SendFrame(USART,SendBuff,BTS,0, 0);                        //Wysyla ramke 
            }
           
           //---------------------------------------------------- 
           }
}



void WizFiResponseInterpreter(char* ReciveBuff, char *SendBuff,  AT91PS_USART USART)
{
  _WiFiCommand Command =WiFiStat.Command;  
  char *pTxt;
  
 //AT91C_BASE_PIOA->PIO_SODR =SpeakerPin;  
 //ButtonBipCounter=g_counter+5;                              
  
  if ((strstr(ReciveBuff, "[DISCONNECT")!=NULL) )                                    
     {
        pTxt=strstr(ReciveBuff, "[DISCONNECT");
        if (pTxt[12]==WiFiStat.HostChannel) HTTP.Status=HTTP_HOST_DISCONNECT;     
        
     }else if  (strstr(ReciveBuff, "Connection: close")!=NULL)
     {
        HTTP.Status=HTTP_HOST_DISCONNECT; 
     }
     else if (strstr(ReciveBuff, "NO CID")!=NULL)                                        
     {
            HTTP.Status=HTTP_HOST_DISCONNECT;    
     }else 
     {
          pTxt=strstr(ReciveBuff, "[CONNECT");
          if(pTxt!=NULL) 
          {  
            pTxt+=9;
            WiFiStat.HostChannel=*pTxt;
            HTTP.Status=HTTP_HOST_CONNECTED;
          } 
     }
  
  
  
  
  switch (Command)
  {
      case WizFi_AT:  
        if(strstr(ReciveBuff, "OK")!=NULL)
        {
           WiFiStat.Command=WizFi_AT_WD;
           AT91F_US_SendFrame(USART,(char*)AT_WD,sizeof(AT_WD)-1,0, 0);                        //Wysyla ramke  
           
        }else{

          AT91F_US_SendFrame(USART,(char*)AT,sizeof(AT)-1,0, 0);    
          WiFiStat.Command=WizFi_AT__REP;       
        }
      break;
      
      case WizFi_AT__REP:  
        if(strstr(ReciveBuff, "OK")!=NULL)
        {
           WiFiStat.Command=WizFi_AT_WD;
           
           AT91F_US_SendFrame(USART,(char*)AT_WD,sizeof(AT_WD)-1,0, 0);                        //Wysyla ramke  
        }else{  
         // WiFiStat.Status=WizFi_AT_CONFIG_ERROR;       
        }
      break;
      
      
      case WizFi_AT_WD:  
        if(strstr(ReciveBuff, "OK")!=NULL)
        {
           WiFiStat.Command=WizFi_AT_NDHCP_1;
           
           AT91F_US_SendFrame(USART,(char*)AT_NDHCP_1,sizeof(AT_NDHCP_1)-1,0, 0);                        //Wysyla ramke  
        }else{
         //  WiFiStat.Status=WizFi_AT_CONFIG_ERROR; 
        }
      break;
      
      
      case WizFi_AT_WS:  
        if(strstr(ReciveBuff, "BSSID")!=NULL)
        {
            pTxt=strstr(ReciveBuff, "Found");
            if (pTxt!=NULL)
            {
              char IL;
              short FoundSSID=StrToWord(pTxt+7,&IL);
              pTxt=ReciveBuff;
              for (char NR=0; NR<FoundSSID; NR++)
              {
                 //pTxt
                //g_SSID_List
              }
            }
        }
      break;
      
      
      case WizFi_AT_NDHCP_1:  
        if(strstr(ReciveBuff, "OK")!=NULL)
        {
           WiFiStat.Command=WizFi_AT_WWEP1;
           char BTS=0;
           for (char i=0; i<sizeof(AT_WWEP1); i++)  {SendBuff[i]=AT_WWEP1[i];   BTS++;}
           BTS--;
           char PassN=0;
           while ((WiFi.Key[PassN]>0) && (PassN<=15)) {SendBuff[BTS+PassN]=WiFi.Key[PassN]; PassN++;}
           SendBuff[BTS+PassN++]=0x0d;
           for (char i=0; i<30; i++)  ReciveBuff[i]=0;
           
           AT91F_US_SendFrame(USART,SendBuff,BTS+PassN,0, 0);                        //Wysyla ramke  
        }else{
          // WiFiStat.Status=WizFi_AT_CONFIG_ERROR; 
        }
      break;
      
      
      case WizFi_AT_WWEP1:  
        if(strstr(ReciveBuff, "OK")!=NULL)
        {
           WiFiStat.Command=WizFi_AT_WWPA;
           char BTS=0;
           for (char i=0; i<sizeof(AT_WWPA); i++)  {SendBuff[i]=AT_WWPA[i];   BTS++;}
           BTS--;
           char PassN=0;
           while ((WiFi.Key[PassN]>0) && (PassN<=15)) {SendBuff[BTS+PassN]=WiFi.Key[PassN]; PassN++;}
           SendBuff[BTS+PassN++]=0x0d;
           for (char i=0; i<30; i++)  ReciveBuff[i]=0;
          
           AT91F_US_SendFrame(USART,SendBuff,BTS+PassN,0, 0);                        //Wysyla ramke  
        }else{
          // WiFiStat.Status=WizFi_AT_CONFIG_ERROR; 
        }
      break;
      
      
      
      
      case WizFi_AT_WWPA:  
        if(strstr(ReciveBuff, "OK")!=NULL)
        {
          WiFiStat.Command=WizFi_NC;
           WiFiStat.Status=WizFi_AT_CONNECTING; 
           
           //³¹czenie z accespointem -------------------------------------------
           char BTS=0;
           for (char i=0; i<sizeof(AT_WA); i++)  {SendBuff[i]=AT_WA[i];   BTS++;}
           BTS--;
           char PassN=0;
           while ((WiFi.SSID[PassN]>0) && (PassN<=15)) {SendBuff[BTS+PassN]=WiFi.SSID[PassN]; PassN++;}
           SendBuff[BTS+PassN++]=0x0d;
           WiFiStat.Command=WizFi_AT_WA;
           for (char i=0; i<30; i++)  ReciveBuff[i]=0;
           
           AT91F_US_SendFrame(USART,SendBuff,BTS+PassN,0, 0);                        //Wysyla ramke  
           WiFi_TimeOut_Counter=g_counter+5*WiFi_TIMEOUT;
           //-------------------------------------------------------------------
        }else{
          // WiFiStat.Status=WizFi_AT_CONFIG_ERROR; 
        }
      break;
      
     
      
      
      
       case WizFi_AT_WA:  
        if(strstr(ReciveBuff, "IP")!=NULL)
        {
          WiFiStat.Command=WizFi_NC;
          WiFiStat.Status=WizFi_AT_CONNECTED;    
           
           //----------------------------------------------
             char BTS=0;
           for (char i=0; i<sizeof(AT_DNSLOOKUP); i++)  {SendBuff[i]=AT_DNSLOOKUP[i];   BTS++;}
           BTS--;
           char PassN=0;
           while ((WiFi.Domena[PassN]>0) && (PassN<=15)) {SendBuff[BTS+PassN]=WiFi.Domena[PassN]; PassN++;}
           SendBuff[BTS+PassN++]=0x0d;
           
           
           WiFiStat.Command=WizFi_AT_DNSLOOKUP;
           for (char i=0; i<30; i++)  ReciveBuff[i]=0;
       
           AT91F_US_SendFrame(USART,SendBuff,BTS+PassN,0, 0);                        //Wysyla ramke  
           SetWiFiResume();
           WiFi_TimeOut_Counter=g_counter+5*WiFi_TIMEOUT;
           //-----------------------------------------------
        }else if (WiFi_TimeOut_Counter<g_counter)
        {
          //³¹czenie z accespointem -------------------------------------------
           char BTS=0;
           for (char i=0; i<sizeof(AT_WA); i++)  {SendBuff[i]=AT_WA[i];   BTS++;}
           BTS--;
           char PassN=0;
           while ((WiFi.SSID[PassN]>0) && (PassN<=15)) {SendBuff[BTS+PassN]=WiFi.SSID[PassN]; PassN++;}
           SendBuff[BTS+PassN++]=0x0d;
           WiFiStat.Command=WizFi_AT_WA;
           for (char i=0; i<30; i++)  ReciveBuff[i]=0;
     
           AT91F_US_SendFrame(USART,SendBuff,BTS+PassN,0, 0);                        //Wysyla ramke  
           WiFi_TimeOut_Counter=g_counter+5*WiFi_TIMEOUT;
           //------------------------------------------------------------------- 
        }
     
      break;  
        
      //adres IP domeny  
      case WizFi_AT_DNSLOOKUP:  
        
        pTxt=strstr(ReciveBuff, "IP:");
        if(pTxt!=NULL)
        {
          
          WiFiStat.Command=WizFi_NC;
          pTxt+=3;
          char p=0;
          while ((*pTxt!=0x0d) && (p<=15))  {WiFiStat.DomenaIP[p++]=*pTxt; pTxt++;}
          
          //³¹czenie z hostem -------
          
           HTTP.OrderType=HTTP_GET_Config;
           ConnectWithHost(SendBuff, USART);

        }
      break;
      
      
      
      
      
      
            
      //odpowiedŸ po³¹czenia z hostem
      case WizFi_SendData:  
         pTxt+=20;
         pTxt=strstr(ReciveBuff, "\r\n\r\n");
         if(pTxt!=NULL)
         {  
           WiFiStat.Command=WizFi_NC;
           pTxt+=4;
           
            
            NextGetTime=g_counter + WiFi_TIMEOUT;
          
           
           if (HTTP.OrderType==HTTP_GET_Config)
           {
             
                 WiFi_TimeOut_Counter=0;
                 pTxt+=3;
                 HTTP.VarSetCount=0;    
                 for (char i=0; i<VAR_SET_AMOUNT; i++) {HTTP.VarSet[i].VarType=NO_Var;}  //wyczyszczenie tablicy 
                 //przepisanie konfiguracji do tabeli VarSet  
                  while (((pTxt[0]=='a') || (pTxt[0]=='b') || (pTxt[0]=='c') || (pTxt[0]=='d')) && (HTTP.VarSetCount<VAR_SET_AMOUNT))
                  {
                     HTTP.VarSet[HTTP.VarSetCount].VarType=pTxt[0];
                     HTTP.VarSet[HTTP.VarSetCount].VarNr=(pTxt[1]-48)*10+(pTxt[2]-48);
                     HTTP.VarSetCount++;
                     pTxt+=7;
                  }
                 HTTP.OrderType=HTTP_NULL;
                 for (char i=0; i<30; i++)  ReciveBuff[i]=0;
                 
               //  SetWiFiResume();
                 PostHTTP_Force(SendBuff, USART);
                 
              }else if (HTTP.OrderType==HTTP_GET)
              {
                  WiFi_TimeOut_Counter=0;
                  pTxt=pTxt;
                  char ZmType=0;
                  char ZmNr=0;
                  char IlCyfr;
                  short int NNW=StrToWord(pTxt, &IlCyfr);       //numer najnowszego wpisu
                  short int NW=NNW;
                  pTxt+=IlCyfr+1;
                  ZmType=*pTxt; pTxt++;
                  ZmNr=(pTxt[0]-48)*10+pTxt[1]-48;
                  pTxt+=3;
                  while (((ZmType=='a') || (ZmType=='b') || (ZmType=='c') || (ZmType=='d')) && (NW>HTTP.LastForseOrderNr) && (HTTP.LastForseOrderNr!=0))
                  {
                    switch (ZmType) 
                    {
                      case 'a':
                          VariablesAnal[ZmNr]=StrToWord(pTxt, &IlCyfr);
                          pTxt+=IlCyfr+2;
                      break;
                        
                      case 'b':
                           if (*pTxt=='1') {Variables[ZmNr/8] |= 1<<(ZmNr%8);} else{ Variables[ZmNr/8]&= ~(1<<(ZmNr%8));}
                           pTxt+=3;
                      break;  
                    }
                    NW=StrToWord(pTxt, &IlCyfr);
                    pTxt+=IlCyfr+1;
                    ZmType=*pTxt; pTxt++;
                    ZmNr=(pTxt[0]-48)*10+pTxt[1]-48;
                    pTxt+=3;
                    
                  }
                  
                  if (NNW>HTTP.LastForseOrderNr) {HTTP.LastForseOrderNr=NNW;}
                  HTTP.OrderType=HTTP_NULL;
                  
                 
                  
                  
              }else if (HTTP.OrderType==HTTP_POST)
              {
              
                  HTTP.WaitingPost=0;
                  WiFi_TimeOut_Counter=0;
                  HTTP.OrderType=HTTP_NULL;
                  
                /*  
                  if (strstr(ReciveBuff, "DISCONNECT")!=NULL)         //jezeli nie rozlaczono automatycznie to rozlacz
                  {
                  //HTTP.Status=HTTP_HOST_DISCONNECT;
                  WiFiStat.Command=WizFi_AT_NCLOSEALL;
                  for (char i=0; i<30; i++)  ReciveBuff[i]=0;
                  HTTP.WaitingPost=0;
                  AT91F_US_SendFrame(USART,(char*)AT_NCLOSEALL,sizeof(AT_NCLOSEALL)-1,0, 0); 
                  }
                 */
                  
              } 
           
           /*
           WiFiStat.Command=WizFi_AT_NCLOSEALL;
                  for (char i=0; i<30; i++)  ReciveBuff[i]=0;
                  NextGetTime=g_counter + WiFi_TIMEOUT;
                  AT91F_US_SendFrame(USART,(char*)AT_NCLOSEALL,sizeof(AT_NCLOSEALL)-1,0, 0);    
           */
             // HTTP.Status=HTTP_DISCONNECT;
              NextGetTime=g_counter + WiFi_CONNECT_PERIOD;
           
           
           HTTP.Status=HTTP_HOST_CONNECTED;
           
         }//else if (strstr(ReciveBuff, "DISCONNECT")!=NULL)                                        
         //{
         /*
             //roz³¹czenie z hostem -------------------------------
                  
                  WiFiStat.Command=WizFi_AT_NCLOSEALL;
                  for (char i=0; i<30; i++)  ReciveBuff[i]=0;
                  NextGetTime=g_counter + 500;
                  AT91F_US_SendFrame(USART,(char*)AT_NCLOSEALL,sizeof(AT_NCLOSEALL)-1,0, 0); 
           */       
         //}else{  
          // if (WiFi_TimeOut_Counter<g_counter) HTTP.OrderType=HTTP_NULL;
          
        //}
      break;
      
      
      
      case WizFi_AT_NCLOSEALL:  
             if (strstr(ReciveBuff, "OK")!=NULL)
             {
                //HTTP.Status=HTTP_HOST_DISCONNECT;
                NextGetTime=g_counter + WiFi_CONNECT_PERIOD;
             }
        
        
      break;
      
      
      //odpowiedŸ po³¹czenia z hostem
      case WizFi_AT_NCTCP:  

        pTxt=strstr(ReciveBuff, "[CONNECT");
        if (HTTP.Status==HTTP_HOST_CONNECTED)
        {
            WiFiStat.Command=WizFi_NC;
            SendHttpQwery(SendBuff,USART); 
        }
        else if(pTxt!=NULL) 
        {
////////////////////////////////////////////////////////////////////////////////
            
            WiFiStat.Command=WizFi_NC;
            pTxt+=9;
            WiFiStat.HostChannel=*pTxt;
            HTTP.Status=HTTP_HOST_CONNECTED;
            SendHttpQwery(SendBuff,USART); 
          
          
        }else if (strstr(ReciveBuff, "DISASSOCIATED"))
        {

            ConnectAccespoint(USART); 

        }else if (strstr(ReciveBuff, "ERROR: SOCKET FAILURE"))
        {
          
            
            InitWizFi(USART); 

        }else if (strstr(ReciveBuff, "ERROR: NO CID]"))
        {
           WiFiStat.Command=WizFi_AT_NCLOSEALL;
       
           AT91F_US_SendFrame(USART,(char*)AT_NCLOSEALL,sizeof(AT_NCLOSEALL)-1,0, 0); 
          
        }
          
     
      break;


      

      


   

  }     
  
  
                                            
  
     
  
  
  
}

