//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                                 CEURON                                   *//
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : Modbus.c                                 *//
//*  Opis                         : G³ówny plik programu                     *//
//*  Autor                        : Pawe³ Narwojsz                           *//
//*  Data utrzorzenia             : 16.01.2008                               *//
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
//
// Typowe funkcje zdefiniowane w protokole Modbus:
//    1  - Read Coil Status           - odczyt biezacego stanu grupy wyjsc cyfrowych,
//    2  - Read Input Status          - odczyt stanu grupy wejsc cyfrowych,
//    3  - Read Holding Register      - odczyt zawartosci grupy rejestrów/analogów wyjsciowych,
//    4  - Read Input Register        - odczyt zawartosci grupy rejestrów/analogów wejsciowych,
//    5  - Force Single Coil          - ustawienie stanu jednego wyjscia cyfrowego,
//    6  - Preset Single Register     - zapis do pojedynczego rejestru/analogu wyjsciowego,
//    7  - Read Exception Status      - odczyt statusu urzadzenia Slave,
//    8  - Diagnostics                - test diagnostyczny,
//    9  - Force Multiple Coils       - ustawienie stanu grupy wyjsc cyfrowych,
//    10 - Preset Multiple Register   - zapis do grupy rejestrów/analogów wyjsciowych,
//    11 - Report Slave ID            - odczyt ID jednostki Slave,
//    12 - Reset Communication Link   - resetowanie polaczenia,
//    13 - Read General Reference     - odczyt rejestrów w pamieci rozszerzonej,
//    14 - Write General Reference    - zapis do rejestrów w pamieci rozszerzonej,
//    15 - Write Multiple Coils       - grupowy zapis wyjœæ cyfrowych / bitów w rejestrach,
//    16 - Write Register             - odczyt/zapis grupy rejestrów,
//    17 - Read FIFO Queue            - odczyt kolejki FIFO.
//****************************************************************************//
//****************************************************************************//





#include "Modbus.h"



/*
_ModTaskTab ModTaskTab;



ModTaskTab.Scene[1].Order=1;
ModTaskTab.Scene[1].Q=0;
ModTaskTab.Scene[1].Port=1;
*/


//Zmienne konfiguracyjne -------------------------------------------------------     

    //MODBUS_0----------------------
     char US0_MasterMODBUS_ENABLE    = 1;                                      //1 -US0 jako MASTER (komunikacja MODBUS)
     char US1_MasterMODBUS_ENABLE    = 0;                                      //1 -US0 jako MASTER (komunikacja MODBUS) 
//______________________________________________________________________________


     


//Zmienne  ---------------------------------------------------------------------     
    _ZD_MODBUS ZD_MODBUS_0[ZD_MODBUS_Tab_Max];                                  //tablica zadañ modbus 0
    char g_Current_ZD_MODBUS_0=0;                                               //bierz¹ce zadanie modbus wed³ug tablicy ZD_MODBUS_0
    char g_ERR_ZD_MODBUS_0=0;                                                   //b³¹d zadañ MODBUS 0 
    char g_Ans_ZD_MODBUS_0=0;                                                   //1- odebrano odpowiedz na pytanie 
    BYTE Buff_TX_0[US0_BUFFER_Tx_SIZE];                                                        //bufor nadawczy USART 0 
    
    _ZD_MODBUS ZD_MODBUS_1[ZD_MODBUS_Tab_Max];                                  //tablica zadañ modbus 0
    char g_Current_ZD_MODBUS_1=0;                                               //bierz¹ce zadanie modbus wed³ug tablicy ZD_MODBUS_0
    char g_ERR_ZD_MODBUS_1=0;                                                   //b³¹d zadañ MODBUS 0 
    char g_Ans_ZD_MODBUS_1=0;                                                   //1- odebrano odpowiedz na pytanie 
    char Buff_TX_1[US1_BUFFER_Tx_SIZE];                                         //bufor nadawczy USART 0 
//______________________________________________________________________________


    
    
    
//Zmienne zewnêtrzne -----------------------------------------------------------    
    extern RAM ram;
    extern char AdressModbus;
    extern int SendModbusEnable;
    extern int g_US0_Comm_TIMEOUT;
    extern float g_US0_OneByteTxTime;
    extern int g_US0_Comm_MinCykle;
    extern _MainStage  MainStage; 
//___________________________Koniec zmiennych zewnêtrznych______________________    
    
    
//void GetBinVarComm(int adr, unsigned int Length, char *Tab );
    
    
//Sta³e ------------------------------------------------------------------------   

const BYTE   bCRC_Hi[] = {0x0,0xC1,0x81,0x40,0x1,0xC0,0x80,0x41,0x1,0xC0,
                         0x80,0x41,0x0,0xC1,0x81,0x40,0x1,0xC0,0x80,0x41,
                          0x0,0xC1,0x81,0x40,0x0,0xC1,0x81,0x40,0x1,0xC0,
                         0x80,0x41,0x1,0xC0,0x80,0x41,0x0,0xC1,0x81,0x40,
                          0x0,0xC1,0x81,0x40,0x1,0xC0,0x80,0x41,0x0,0xC1,
                         0x81,0x40,0x1,0xC0,0x80,0x41,0x1,0xC0,0x80,0x41,
                          0x0,0xC1,0x81,0x40,0x1,0xC0,0x80,0x41,0x0,0xC1,
                         0x81,0x40,0x0,0xC1,0x81,0x40,0x1,0xC0,0x80,0x41,
                          0x0,0xC1,0x81,0x40,0x1,0xC0,0x80,0x41,0x1,0xC0,
                         0x80,0x41,0x0,0xC1,0x81,0x40,0x0,0xC1,0x81,0x40,
                          0x1,0xC0,0x80,0x41,0x1,0xC0,0x80,0x41,0x0,0xC1,
                         0x81,0x40,0x1,0xC0,0x80,0x41,0x0,0xC1,0x81,0x40,
                          0x0,0xC1,0x81,0x40,0x1,0xC0,0x80,0x41,0x1,0xC0,
                         0x80,0x41,0x0,0xC1,0x81,0x40,0x0,0xC1,0x81,0x40,
                          0x1,0xC0,0x80,0x41,0x0,0xC1,0x81,0x40,0x1,0xC0,
                         0x80,0x41,0x1,0xC0,0x80,0x41,0x0,0xC1,0x81,0x40,
                          0x0,0xC1,0x81,0x40,0x1,0xC0,0x80,0x41,0x1,0xC0,
                         0x80,0x41,0x0,0xC1,0x81,0x40,0x1,0xC0,0x80,0x41,
                          0x0,0xC1,0x81,0x40,0x0,0xC1,0x81,0x40,0x1,0xC0,
                         0x80,0x41,0x0,0xC1,0x81,0x40,0x1,0xC0,0x80,0x41,
                          0x1,0xC0,0x80,0x41,0x0,0xC1,0x81,0x40,0x1,0xC0,
                         0x80,0x41,0x0,0xC1,0x81,0x40,0x0,0xC1,0x81,0x40,
                          0x1,0xC0,0x80,0x41,0x1,0xC0,0x80,0x41,0x0,0xC1,
                         0x81,0x40,0x0,0xC1,0x81,0x40,0x1,0xC0,0x80,0x41,
                          0x0,0xC1,0x81,0x40,0x1,0xC0,0x80,0x41,0x1,0xC0,
                         0x80,0x41,0x0,0xC1,0x81,0x40};

const BYTE  bCRC_Lo[] = { 0x0,0xC0,0xC1,0x1,0xC3,0x3,0x2,0xC2,0xC6,0x6,
                          0x7,0xC7,0x5,0xC5,0xC4,0x4,0xCC,0xC,0xD,0xCD,
                          0xF,0xCF,0xCE,0xE,0xA,0xCA,0xCB,0xB,0xC9,0x9,
                          0x8,0xC8,0xD8,0x18,0x19,0xD9,0x1B,0xDB,0xDA,0x1A,
                         0x1E,0xDE,0xDF,0x1F,0xDD,0x1D,0x1C,0xDC,0x14,0xD4,
                         0xD5,0x15,0xD7,0x17,0x16,0xD6,0xD2,0x12,0x13,0xD3,
                         0x11,0xD1,0xD0,0x10,0xF0,0x30,0x31,0xF1,0x33,0xF3,
                         0xF2,0x32,0x36,0xF6,0xF7,0x37,0xF5,0x35,0x34,0xF4,
                         0x3C,0xFC,0xFD,0x3D,0xFF,0x3F,0x3E,0xFE,0xFA,0x3A,
                         0x3B,0xFB,0x39,0xF9,0xF8,0x38,0x28,0xE8,0xE9,0x29,
                         0xEB,0x2B,0x2A,0xEA,0xEE,0x2E,0x2F,0xEF,0x2D,0xED,
                         0xEC,0x2C,0xE4,0x24,0x25,0xE5,0x27,0xE7,0xE6,0x26,
                         0x22,0xE2,0xE3,0x23,0xE1,0x21,0x20,0xE0,0xA0,0x60,
                         0x61,0xA1,0x63,0xA3,0xA2,0x62,0x66,0xA6,0xA7,0x67,
                         0xA5,0x65,0x64,0xA4,0x6C,0xAC,0xAD,0x6D,0xAF,0x6F,
                         0x6E,0xAE,0xAA,0x6A,0x6B,0xAB,0x69,0xA9,0xA8,0x68,
                         0x78,0xB8,0xB9,0x79,0xBB,0x7B,0x7A,0xBA,0xBE,0x7E,
                         0x7F,0xBF,0x7D,0xBD,0xBC,0x7C,0xB4,0x74,0x75,0xB5,
                         0x77,0xB7,0xB6,0x76,0x72,0xB2,0xB3,0x73,0xB1,0x71,
                         0x70,0xB0,0x50,0x90,0x91,0x51,0x93,0x53,0x52,0x92,
                         0x96,0x56,0x57,0x97,0x55,0x95,0x94,0x54,0x9C,0x5C,
                         0x5D,0x9D,0x5F,0x9F,0x9E,0x5E,0x5A,0x9A,0x9B,0x5B,
                         0x99,0x59,0x58,0x98,0x88,0x48,0x49,0x89,0x4B,0x8B,
                         0x8A,0x4A,0x4E,0x8E,0x8F,0x4F,0x8D,0x4D,0x4C,0x8C,
                         0x44,0x84,0x85,0x45,0x87,0x47,0x46,0x86,0x82,0x42,
                         0x43,0x83,0x41,0x81,0x80,0x40};
                         
//___________________________Koniec sta³ych ____________________________________    
    





//!-----------------------------------------------------------------------------
//!  Nazwa funkcji :    crc16
//!  Opis:              Oblicza sumê kontroln¹ CRC 16
//!-----------------------------------------------------------------------------
//!  modbusframe  - adres pierwszej cyfry
//!  Length       - zakres liczb do wyliczenia CRC
//!----------------------------------------------------------------------------- 
WORD crc16(BYTE* modbusframe,int Length)
{
    int index;
    BYTE crc_Low = 0xFF;
    BYTE crc_High = 0xFF;

    for (int i_tmp = 0; i_tmp<Length; i_tmp++)
    {
    index = crc_High ^ modbusframe[i_tmp];
    crc_High = crc_Low ^ bCRC_Hi[index] ;
    crc_Low = bCRC_Lo[index];
    }
    
    return (crc_High << 8 | crc_Low);  
}//_______________________ Koniec funkcji crc16 ________________________________
      







//!-----------------------------------------------------------------------------
//!  Nazwa funkcji :    SetWordToBytes
//!  Opis:              Funkcja konwertuje wartoœæ w WORD na 2 kolejne bajty
//!-----------------------------------------------------------------------------
//!  tmp          - liczba w formacie WORD
//!  *ptr         - wskaŸnik do pierwszego bajtu
//!----------------------------------------------------------------------------- 
void SetWordToBytes(WORD tmp, BYTE *ptr){ 
    *(ptr+1) = 0x00ff & tmp;
    *ptr = (0x00ff & tmp>>8);
}//_______________________ Koniec funkcji SetWordToBytes _______________________




//!-----------------------------------------------------------------------------
//!  Nazwa funkcji :    ModBus_CommS
//!  Opis:              funkcja komunikacji MODBUS (slave). Interpretuje otrzymane dane i wysy³a odpowiedŸ
//!-----------------------------------------------------------------------------
//!  modbusframe  - otrzymana ramka danych
//!----------------------------------------------------------------------------- 
BYTE ModBus_CommS(BYTE* modbusframe, char US_Nr)
{
  
      AT91PS_USART AT91C_BASE_US;
      BYTE* Buff_TX;
      
      
      //wybór USARTa
      if (US_Nr) 
      {
          AT91C_BASE_US     =   AT91C_BASE_US1;
          Buff_TX           =   Buff_TX_1; 

      } 
      else 
      {
          AT91C_BASE_US     =   AT91C_BASE_US0;
          Buff_TX           =   Buff_TX_0; 

      }
      
      
  
  
  
  
  for (int i=0; i<256; i++) {Buff_TX[i]=0x0;}                                 //czyœci bufor nadawczy
  

  
  if ((modbusframe[0]) != AdressModbus) return 0;                               //wyjdz jezeli zly adres
  
//  g_transmit_ok=1;
  
  
  WORD  RamByte=(((((WORD)modbusframe[2])<<8 | ((WORD)modbusframe[3])))/8);
  WORD  RamCall=((((WORD)modbusframe[2])<<8 | ((WORD)modbusframe[3])));
  BYTE  RamBit= ((((WORD)modbusframe[2])<<8 | ((WORD)modbusframe[3]))+0) % 8;
  BYTE  ByteCount=(((((WORD)modbusframe[4])<<8 | ((WORD)modbusframe[5]))-1)/8)+1;
  BYTE  WordCount=(((WORD)modbusframe[4])<<8 | ((WORD)modbusframe[5]));
  WORD  BitCount=(((WORD)modbusframe[4])<<8) | (((WORD)modbusframe[5]));
  char  CurentByteBufTX;
  char  CurentBitBufTX;
  
  if (modbusframe[1]==1)
          {
            //Read Coil Status  (odczyt zmiennych dwustanowych DA00-DDFF)
     
                      if ((((WORD)modbusframe[6])<<8 | ((WORD)modbusframe[7])) != crc16(modbusframe,6)) return 0;   //sprawdzenie CRC
                     
                      //OdpowiedŸ ----------------------------------------------
                      
                      Buff_TX[0]=AdressModbus;                              //adres                                           
                      Buff_TX[1]=0x1;
                      Buff_TX[2]=ByteCount;    
                      
                      CurentByteBufTX=3;
                      CurentBitBufTX=0;
                      
                      for (int i=0; i<BitCount; i++)
                      {
                         Buff_TX[CurentByteBufTX] &= ~(1<<CurentBitBufTX);
                         Buff_TX[CurentByteBufTX] |=  (((ram.VarCom_Byte[RamByte]>>RamBit) & 0x1)<< CurentBitBufTX);
                         
                         if (CurentBitBufTX<7) {CurentBitBufTX++;} else {CurentBitBufTX=0; CurentByteBufTX++;}
                         if (RamBit<7) {RamBit++;} else {RamBit=0; RamByte++;}
                      }

                      SetWordToBytes(crc16(Buff_TX,ByteCount+3),&Buff_TX[ByteCount+3] );  //wstawia sumê kontroln¹
                      
                      AT91F_US_SendFrame(AT91C_BASE_US,Buff_TX, ByteCount+5,0, 0);  //Wysyla ramke
                                              
          }
   if (modbusframe[1]==2)
   {
                      
          
            ////Read discrete input  (odczyt zmiennych dwustanowych B000-B5FF)
      
                      if ((((WORD)modbusframe[6])<<8 | ((WORD)modbusframe[7])) != crc16(modbusframe,6)) return 0;   //sprawdzenie CRC
                     /*
                      //OdpowiedŸ ----------------------------------------------                      
                      Buff_TX[0]=AdressModbus;                              //adres                                           
                      Buff_TX[1]=0x2;
                      Buff_TX[2]=ByteCount;    
                      
                      CurentByteBufTX=3;
                      CurentBitBufTX=0;
                      
                      for (int i=0; i<BitCount; i++)
                      {
                         Buff_TX[CurentByteBufTX] &= ~(1<<CurentBitBufTX);
                         Buff_TX[CurentByteBufTX] |=  (((ram.BinVarRAM[RamByte]>>RamBit) & 0x1)<< CurentBitBufTX);
                         
                         if (CurentBitBufTX<7) {CurentBitBufTX++;} else {CurentBitBufTX=0; CurentByteBufTX++;}
                         if (RamBit<7) {RamBit++;} else {RamBit=0; RamByte++;}
                      }

                      SetWordToBytes(crc16(Buff_TX,ByteCount+3),&Buff_TX[ByteCount+3] );  //wstawia sumê kontroln¹
                      
                      AT91F_US_SendFrame(AT91C_BASE_US,Buff_TX, ByteCount+5,0, 0);  //Wysyla ramke
                                               
         
                      
            //Holding Register (odczyt zmiennych analogowych DA00-DDFF)
                      */
          }
   if (modbusframe[1]==3)
   {
                      if ((((WORD)modbusframe[6])<<8 | ((WORD)modbusframe[7])) != crc16(modbusframe,6)) return 0;   //sprawdzenie CRC
                     
                      //OdpowiedŸ ----------------------------------------------                      
                      Buff_TX[0]=AdressModbus;                              //adres                                           
                      Buff_TX[1]=0x3;
                      Buff_TX[2]=(WordCount*2);    
                      
                      CurentByteBufTX=3;
                      CurentBitBufTX=0;
                      
                      for (int i=0; i<WordCount; i++)
                      {

                         Buff_TX[CurentByteBufTX] = ram.VarCom_Byte[(RamCall*2)];
                         Buff_TX[CurentByteBufTX+1] = ram.VarCom_Byte[(RamCall*2)+1];
                         
                         CurentByteBufTX=CurentByteBufTX+2;
                         RamCall++;

                      }

                      SetWordToBytes(crc16(Buff_TX,(WordCount*2)+3),&Buff_TX[(WordCount*2)+3] );  //wstawia sumê kontroln¹
                      
                      AT91F_US_SendFrame(AT91C_BASE_US,Buff_TX, (WordCount*2)+5, 0, 0);  //Wysyla ramke
                                               
          
                  
              //Input Register (odczyt zmiennych analogowych A000-A5FF)      
          }
   if (modbusframe[1]==4)
   {
      /*
                      if ((((WORD)modbusframe[6])<<8 | ((WORD)modbusframe[7])) != crc16(modbusframe,6)) return 0;   //sprawdzenie CRC
                     
                      //OdpowiedŸ ----------------------------------------------                      
                      Buff_TX[0]=AdressModbus;                              //adres                                           
                      Buff_TX[1]=0x4;
                      Buff_TX[2]=(WordCount*2);    
                      
                      CurentByteBufTX=3;
                      CurentBitBufTX=0;
                      
                      for (int i=0; i<WordCount; i++)
                      {

                         Buff_TX[CurentByteBufTX]   = (((WORD)(ram.AnaVarRAM[RamCall]))>>8) & 0xFF;
                         Buff_TX[CurentByteBufTX+1] = ((WORD)(ram.AnaVarRAM[RamCall])) & 0xFF;
                         
                         CurentByteBufTX=CurentByteBufTX+2;
                         RamCall++;
                      }

                      SetWordToBytes(crc16(Buff_TX,(WordCount*2)+3),&Buff_TX[(WordCount*2)+3] );  //wstawia sumê kontroln¹
                      
                      AT91F_US_SendFrame(AT91C_BASE_US,Buff_TX, (WordCount*2)+5, 0, 0);  //Wysyla ramke
                                               
                
            //Write Coil Status  (zapis zmiennych dwustanowych - pojedynczo DA00-DDFF)
       */
          }
   if (modbusframe[1]==5)
   {
                      if ((((WORD)modbusframe[6])<<8 | ((WORD)modbusframe[7])) != crc16(modbusframe,6)) return 0;   //sprawdzenie CRC
                      
                      //if ((modbusframe[4]!=0xFF) && (modbusframe[4]!=0x00)) return 0;
                      
                      Buff_TX[CurentByteBufTX] &= ~(1<<CurentBitBufTX);
                      if (modbusframe[4])
                      {
                         ram.VarCom_Byte[RamByte] |= 0x1 << RamBit;
                      }
                      else
                      {
                         ram.VarCom_Byte[RamByte] &= ~(0x1 << RamBit);
                      }               
                      
                      //odpowiedz ----------------------------------------------
                      Buff_TX[0]=AdressModbus;                              //adres                                           
                      Buff_TX[1]=0x5;   
                      Buff_TX[2]=modbusframe[2];
                      Buff_TX[3]=modbusframe[3];
                      Buff_TX[4]=modbusframe[4];
                      Buff_TX[5]=modbusframe[5];
                                            
                      SetWordToBytes(crc16(Buff_TX,6),&Buff_TX[6] );  //wstawia sumê kontroln¹
                      
                      AT91F_US_SendFrame(AT91C_BASE_US,Buff_TX,8,0, 0);  //Wysyla ramke
                                               
         

            //Write Single Register  (zapis zmiennych analogowych - pojedynczo DA00-DDFF)
          }
   if (modbusframe[1]==6)
   {
                      if ((((WORD)modbusframe[6])<<8 | ((WORD)modbusframe[7])) != crc16(modbusframe,6)) return 0;   //sprawdzenie CRC
                      
                      Buff_TX[CurentByteBufTX] &= ~(1<<CurentBitBufTX);
                      
                      ram.VarCom_Byte[(RamCall*2)]   = modbusframe[4] ;
                      ram.VarCom_Byte[(RamCall*2)+1] = modbusframe[5] ;
                                                              
                      //odpowiedz ----------------------------------------------
                      Buff_TX[0]=AdressModbus;                              //adres                                           
                      Buff_TX[1]=0x6;   
                      Buff_TX[2]=modbusframe[2];
                      Buff_TX[3]=modbusframe[3];
                      Buff_TX[4]=modbusframe[4];
                      Buff_TX[5]=modbusframe[5];
                                            
                      SetWordToBytes(crc16(Buff_TX,6),&Buff_TX[6] );  //wstawia sumê kontroln¹
                      
                      AT91F_US_SendFrame(AT91C_BASE_US,Buff_TX,8,0, 0);  //Wysyla ramke
                                               
               
                      
            //Write Multiple Status  (zapis zmiennych dwustanowych - grupowo DA00-DDFF)
          }
   if (modbusframe[1]==15)
   {
                      if ((((WORD)modbusframe[modbusframe[6]+7])<<8 | ((WORD)modbusframe[modbusframe[6]+8])) != crc16(modbusframe,modbusframe[6]+7)) return 0;   //sprawdzenie CRC
       
                      

                      CurentByteBufTX=7;
                      CurentBitBufTX=0;
                         
                      RamByte=(((((WORD)modbusframe[2])<<8 | ((WORD)modbusframe[3]))+1)/8);
                      RamBit= ((((WORD)modbusframe[2])<<8 | ((WORD)modbusframe[3]))+1) % 8;
                      
                      for (int i=0; i<BitCount; i++)
                      { 
                        if ((modbusframe[CurentByteBufTX]>>CurentBitBufTX)&0x1)
                        {
                          ram.VarCom_Byte[RamByte] |= 0x1 << RamBit;
                        }
                        else
                        {
                          ram.VarCom_Byte[RamByte] &= ~(0x1 << RamBit);
                        }   
                       
                        if (RamBit<=6) {RamBit++;} else {RamBit=0; RamByte++;}
                        if (CurentBitBufTX<7) {CurentBitBufTX++;} else {CurentBitBufTX=0; CurentByteBufTX++;}
                        
                      }
                     
                      
                      //odpowiedz ----------------------------------------------
                      Buff_TX[0]=AdressModbus;                              //adres                                           
                      Buff_TX[1]=0xF;   
                      Buff_TX[2]=modbusframe[2];
                      Buff_TX[3]=modbusframe[3];
                      Buff_TX[4]=modbusframe[4];
                      Buff_TX[5]=modbusframe[5];
                                            
                      SetWordToBytes(crc16(Buff_TX,6),&Buff_TX[6] );  //wstawia sumê kontroln¹
                      
                      Buff_TX[8]=0;
                      
                      AT91F_US_SendFrame(AT91C_BASE_US,Buff_TX,8,0, 0);  //Wysyla ramke
                                               
                  
                                
                      
              //Write Single Register  (zapis zmiennych analogowych - pojedynczo DA00-DDFF)
          }
   if (modbusframe[1]==16)
   {
                      if ((((WORD)modbusframe[modbusframe[6]+7])<<8 | ((WORD)modbusframe[modbusframe[6]+8])) != crc16(modbusframe,modbusframe[6]+7)) return 0;   //sprawdzenie CRC

                      CurentByteBufTX=7;
                      
                      for (int i=0; i<WordCount; i++)
                      { 
                        
                        ram.VarCom_Byte[(RamCall*2)]   = modbusframe[CurentByteBufTX] ;
                        ram.VarCom_Byte[(RamCall*2)+1] = modbusframe[CurentByteBufTX+1] ;
  
                       
                        CurentByteBufTX=CurentByteBufTX+2;
                        RamCall++;
                        
                      }

                      //odpowiedz ----------------------------------------------
                      Buff_TX[0]=AdressModbus;                              //adres                                           
                      Buff_TX[1]=0x10;   
                      Buff_TX[2]=modbusframe[2];
                      Buff_TX[3]=modbusframe[3];
                      Buff_TX[4]=modbusframe[4];
                      Buff_TX[5]=modbusframe[5];
                                            
                      SetWordToBytes(crc16(Buff_TX,6),&Buff_TX[6] );  //wstawia sumê kontroln¹
                      
                      Buff_TX[8]=0;
                      
                      AT91F_US_SendFrame(AT91C_BASE_US,Buff_TX,8,0, 0);  //Wysyla ramke
                                               
              
                
          }
  
  return 1;
}//_______________________ Koniec funkcji ModBus_CommS ________________________________








//!-----------------------------------------------------------------------------
//!  Nazwa funkcji :    ModBus_MASTER_TX_US0
//!  Opis:              Funkcja realizacji zadañ MODBUS wed³ug zadañ z tabeli ZD_MODBUS_0
//!-----------------------------------------------------------------------------
//!  
//!----------------------------------------------------------------------------- 
char nr_tmp=0;
char ModBus_MASTER_TX(char US_Nr)
{
      char il_ptl=0;
      AT91PS_USART AT91C_BASE_US;
      ptr_ZD_MODBUS ZD_MODBUS;
      char* Current_ZD_MODBUS;
      BYTE* Buff_TX;

      //wybór USARTa
      if (US_Nr) 
      {
          AT91C_BASE_US     =   AT91C_BASE_US1;
          ZD_MODBUS         =   ZD_MODBUS_1;
          Current_ZD_MODBUS =   &g_Current_ZD_MODBUS_1;
          Buff_TX           =   Buff_TX_1; 
      }   
      else 
      {
          AT91C_BASE_US     =   AT91C_BASE_US0;
          ZD_MODBUS         =   ZD_MODBUS_0;
          Current_ZD_MODBUS =   &g_Current_ZD_MODBUS_0;
          Buff_TX           =   Buff_TX_0; 
      }
     






      nr_tmp=US_Nr;
      
      Skok:
          
        
            if (il_ptl < ZD_MODBUS_Tab_Max) {il_ptl++;} else {return 0;}
            
            if ((ZD_MODBUS[(*Current_ZD_MODBUS)+1].Adress > 0) && ((*Current_ZD_MODBUS)+1 < ZD_MODBUS_Tab_Max) )
            {
              *Current_ZD_MODBUS=(*Current_ZD_MODBUS)+1;
              
                  ZD_MODBUS[0].Error=0x0;
                  ZD_MODBUS[1].Error=0x0;
                  ZD_MODBUS[2].Error=0x0;
                  ZD_MODBUS[3].Error=0x0;
              
            }
            else
            {
              *Current_ZD_MODBUS=0;
            } 
            
            /*
            if (ZD_MODBUS[*Current_ZD_MODBUS].Adress == 0) 
            {
              *ERR_ZD_MODBUS=1;
              goto Skok;
            }
              

            
            if  (ZD_MODBUS[*Current_ZD_MODBUS].Error > 50)
             {
               ZD_MODBUS[*Current_ZD_MODBUS].Error++;
               if  (ZD_MODBUS[*Current_ZD_MODBUS].Error > 5000+(*Current_ZD_MODBUS)*10)
               {
                  ZD_MODBUS[*Current_ZD_MODBUS].Error=0;
        
               }
               else
               {
                 goto Skok;
                 
               }
             }
             Delay(1);
            
            */
            
         char liB15 = (((ZD_MODBUS[*Current_ZD_MODBUS].Coil)-1) / 8)+1;  
            
 //*** Funkcje uproszczone dostosowane do systemu. Nie zawieraj¹ wszystkich mo¿liwoœci modbus
            
            

         
         
            
            
             //Write Multiple Status  (zapis rejestru)
             if ((ZD_MODBUS[*Current_ZD_MODBUS].Function)==0xf)
            {
                          
              
                      Buff_TX[0]  = ZD_MODBUS[*Current_ZD_MODBUS].Adress;                        //Adres
                      Buff_TX[1]  = ZD_MODBUS[*Current_ZD_MODBUS].Function;                      //Funkcja                                                      
                      SetWordToBytes(ZD_MODBUS[*Current_ZD_MODBUS].Target,&Buff_TX[2] );         //Adres zmiennej (w urz¹dzeniu)   
                      SetWordToBytes(ZD_MODBUS[*Current_ZD_MODBUS].Coil,&Buff_TX[4] );                                             //il przesy³anych bajtów
                      Buff_TX[6]=liB15; 
                      Buff_TX[7]=(MainStage.StageQ[ZD_MODBUS[*Current_ZD_MODBUS].StartAdr].Value[ZD_MODBUS[*Current_ZD_MODBUS].Target]) & 0xFF; 
                      Buff_TX[8]=((MainStage.StageQ[ZD_MODBUS[*Current_ZD_MODBUS].StartAdr].Value[ZD_MODBUS[*Current_ZD_MODBUS].Target])>>8) & 0xFF; 
                      
                      SetWordToBytes(crc16(Buff_TX,liB15+7),&Buff_TX[7+liB15] );                    //wstawia sumê kontroln¹                     
                      Buff_TX[9+liB15]=0;
                      AT91F_US_SendFrame(AT91C_BASE_US,Buff_TX,9+liB15,0, 0);                        //Wysyla ramke        
                      g_US0_Comm_MinCykle =  (int)((g_US0_OneByteTxTime * (9+liB15)) +100);   
            }
          
            
            
            
            
            
            
 //*** Funkcje pe³ne MODBUS         
            
           
   /*         
            
            
           
            //Read Coil Status  (odczyt biezacego stanu grupy wyjsc cyfrowych)
             if ((ZD_MODBUS[*Current_ZD_MODBUS].Function)==1)
             {
                      Buff_TX[0]  = ZD_MODBUS[*Current_ZD_MODBUS].Adress;                        //Adres
                      Buff_TX[1]  = ZD_MODBUS[*Current_ZD_MODBUS].Function;                      //Funkcja                                                      
                      SetWordToBytes(ZD_MODBUS[*Current_ZD_MODBUS].StartAdr,&Buff_TX[2] );       //Pocz¹tkowy adres danych (w urz¹dzeniu)
                      SetWordToBytes(ZD_MODBUS[*Current_ZD_MODBUS].Coil,&Buff_TX[4] );           //Iloœæ danych do pobrania
                      SetWordToBytes(crc16(Buff_TX,6),&Buff_TX[6] );                             //wstawia sumê kontroln¹
                      
                      Buff_TX[8]=0;
                      Buff_TX[9]=0;
                      Buff_TX[10]=0;
                      
                      AT91F_US_SendFrame(AT91C_BASE_US,Buff_TX,8,0, 0);        //Wysyla ramke
                      
                      g_US0_Comm_MinCykle =  (int)((g_US0_OneByteTxTime * 8) +1);    
             }
                      
                  
            //Read discrete input  (odczyt stanu grupy wejsc cyfrowych)
             if ((ZD_MODBUS[*Current_ZD_MODBUS].Function)==2)
            {
                      Buff_TX[0]  = ZD_MODBUS[*Current_ZD_MODBUS].Adress;                        //Adres
                      Buff_TX[1]  = ZD_MODBUS[*Current_ZD_MODBUS].Function;                      //Funkcja                                                      
                      SetWordToBytes(ZD_MODBUS[*Current_ZD_MODBUS].StartAdr,&Buff_TX[2] );       //Pocz¹tkowy adres danych (w urz¹dzeniu)
                      SetWordToBytes(ZD_MODBUS[*Current_ZD_MODBUS].Coil,&Buff_TX[4] );           //Iloœæ danych do pobrania
                      SetWordToBytes(crc16(Buff_TX,6),&Buff_TX[6] );                                //wstawia sumê kontroln¹
                      
                      Buff_TX[8]=0;
                      
                      AT91F_US_SendFrame(AT91C_BASE_US,Buff_TX,8,0, 0);        //Wysyla ramke
                      g_US0_Comm_MinCykle =  (int)((g_US0_OneByteTxTime * 8) +10);   
            }   
                      
            //Holding Register (odczyt zawartosci grupy rejestrów/analogów wyjsciowych)
             if ((ZD_MODBUS[*Current_ZD_MODBUS].Function)==3)
            {
                      Buff_TX[0]  = ZD_MODBUS[*Current_ZD_MODBUS].Adress;                        //Adres
                      Buff_TX[1]  = ZD_MODBUS[*Current_ZD_MODBUS].Function;                      //Funkcja                                                      
                      SetWordToBytes(ZD_MODBUS[*Current_ZD_MODBUS].StartAdr,&Buff_TX[2] );       //Pocz¹tkowy adres danych (w urz¹dzeniu)
                      SetWordToBytes(ZD_MODBUS[*Current_ZD_MODBUS].Coil,&Buff_TX[4] );           //Iloœæ danych do pobrania
                      SetWordToBytes(crc16(Buff_TX,6),&Buff_TX[6] );                                //wstawia sumê kontroln¹
                      
                      Buff_TX[8]=0;
                      
                      AT91F_US_SendFrame(AT91C_BASE_US,Buff_TX,8,0, 0);        //Wysyla ramke
                      g_US0_Comm_MinCykle =  (int)((g_US0_OneByteTxTime * 8) +1);   
            } 
                      
              //Input Register (odczyt zawartosci grupy rejestrów/analogów wejsciowych)      
             if ((ZD_MODBUS[*Current_ZD_MODBUS].Function)==4)
            {
                      Buff_TX[0]  = ZD_MODBUS[*Current_ZD_MODBUS].Adress;                        //Adres
                      Buff_TX[1]  = ZD_MODBUS[*Current_ZD_MODBUS].Function;                      //Funkcja                                                      
                      SetWordToBytes(ZD_MODBUS[*Current_ZD_MODBUS].StartAdr,&Buff_TX[2] );       //Pocz¹tkowy adres danych (w urz¹dzeniu)
                      SetWordToBytes(ZD_MODBUS[*Current_ZD_MODBUS].Coil,&Buff_TX[4] );           //Iloœæ danych do pobrania
                      SetWordToBytes(crc16(Buff_TX,6),&Buff_TX[6] );                                //wstawia sumê kontroln¹
                      
                      Buff_TX[8]=0;
                      
                      AT91F_US_SendFrame(AT91C_BASE_US,Buff_TX,8,0, 0);        //Wysyla ramke
                      g_US0_Comm_MinCykle =  (int)((g_US0_OneByteTxTime * 8) +1);   
            }            
                      
            //Write Coil Status  (ustawienie stanu jednego wyjscia cyfrowego)
             if ((ZD_MODBUS[*Current_ZD_MODBUS].Function)==5)
            {
                      TmpWalue = (ram.VarCom_Word[ZD_MODBUS[*Current_ZD_MODBUS].StartAdr]) & 0x1;
                                            
                      if ((TmpWalue==ZD_MODBUS[*Current_ZD_MODBUS].LastSend) && (ZD_MODBUS[*Current_ZD_MODBUS].Confirm==1) )     return 0;
                      ZD_MODBUS[*Current_ZD_MODBUS].Confirm=0;
                      ZD_MODBUS[*Current_ZD_MODBUS].LastSend=TmpWalue;
                        
                      GetBinVarPackComm(ZD_MODBUS[*Current_ZD_MODBUS].StartAdr, 1, &Buff_TX[4]);
                     
                      Buff_TX[0]  = ZD_MODBUS[*Current_ZD_MODBUS].Adress;                        //Adres
                      Buff_TX[1]  = ZD_MODBUS[*Current_ZD_MODBUS].Function;                      //Funkcja                                                      
                      SetWordToBytes(ZD_MODBUS[*Current_ZD_MODBUS].Target,&Buff_TX[2] );         //Adres zmiennej (w urz¹dzeniu)                          
                      
                      
                      SetWordToBytes(crc16(Buff_TX,6),&Buff_TX[6] );                              //wstawia sumê kontroln¹                     
                      Buff_TX[8]=0;
                      AT91F_US_SendFrame(AT91C_BASE_US,Buff_TX,8,0, 0);                           //Wysyla ramke        
                      g_US0_Comm_MinCykle =  (int)((g_US0_OneByteTxTime * 8) +1)+100;   
            }

            //Write Single Register  (zapis do pojedynczego rejestru/analogu wyjsciowego)
            if ((ZD_MODBUS[*Current_ZD_MODBUS].Function)==6)
            { 
                     
              
              
              
                      Buff_TX[0]  = ZD_MODBUS[*Current_ZD_MODBUS].Adress;                        //Adres
                      Buff_TX[1]  = ZD_MODBUS[*Current_ZD_MODBUS].Function;                      //Funkcja                                                      
                      SetWordToBytes(ZD_MODBUS[*Current_ZD_MODBUS].Target,&Buff_TX[2] );         //Adres zmiennej (w urz¹dzeniu)                         
                      
                      SetWordToBytes(ram.VarCom_Byte[(ZD_MODBUS[*Current_ZD_MODBUS].StartAdr*2)],&Buff_TX[4] ); 
                      
                      SetWordToBytes(crc16(Buff_TX,6),&Buff_TX[6] );                                //wstawia sumê kontroln¹                     
                      
                      Buff_TX[8]=0;
                      AT91F_US_SendFrame(AT91C_BASE_US,Buff_TX,8,0, 0);        //Wysyla ramke        
                      g_US0_Comm_MinCykle =  (int)((g_US0_OneByteTxTime * 8) +1);   
            }
                      
            //Write Multiple Status  (zapis zmiennych dwustanowych - grupowo DA00-DDFF)
             if ((ZD_MODBUS[*Current_ZD_MODBUS].Function)==0xf)
            {
                          
              
                      Buff_TX[0]  = ZD_MODBUS[*Current_ZD_MODBUS].Adress;                        //Adres
                      Buff_TX[1]  = ZD_MODBUS[*Current_ZD_MODBUS].Function;                      //Funkcja                                                      
                      SetWordToBytes(ZD_MODBUS[*Current_ZD_MODBUS].Target,&Buff_TX[2] );         //Adres zmiennej (w urz¹dzeniu)   
                      SetWordToBytes(ZD_MODBUS[*Current_ZD_MODBUS].Coil,&Buff_TX[4] );                                             //il przesy³anych bajtów
                      Buff_TX[6]=liB15;
                      //GetBinVarPackComm(ZD_MODBUS[*Current_ZD_MODBUS].StartAdr, (ZD_MODBUS[*Current_ZD_MODBUS].Coil), &Buff_TX[7]);  //void GetBinVarPackComm(WORD adr, WORD Length, char *Tab )
                      GetRegPackComm(ZD_MODBUS[*Current_ZD_MODBUS].StartAdr, ZD_MODBUS[*Current_ZD_MODBUS].Target,  (ZD_MODBUS[*Current_ZD_MODBUS].Coil), &Buff_TX[7]);//(WORD Adr, char Reg, char LengthBit, unsigned char* Tab )
                      
                      //Buff_TX[7]  =0x01;
                      
                      SetWordToBytes(crc16(Buff_TX,liB15+7),&Buff_TX[7+liB15] );                    //wstawia sumê kontroln¹                     
                      Buff_TX[9+liB15]=0;
                      AT91F_US_SendFrame(AT91C_BASE_US,Buff_TX,9+liB15,0, 0);                        //Wysyla ramke        
                      g_US0_Comm_MinCykle =  (int)((g_US0_OneByteTxTime * (9+liB15)) +100);   
            }
                                
                      
              //Write Single Register  (zapis zmiennych analogowych - grupowo DA00-DDFF)
             if ((ZD_MODBUS[*Current_ZD_MODBUS].Function)==0x10)
            {
              
                      Buff_TX[0]  = ZD_MODBUS[*Current_ZD_MODBUS].Adress;                        //Adres
                      Buff_TX[1]  = ZD_MODBUS[*Current_ZD_MODBUS].Function;                      //Funkcja                                                      
                      SetWordToBytes(ZD_MODBUS[*Current_ZD_MODBUS].Target,&Buff_TX[2] );         //Adres zmiennej (w urz¹dzeniu)   
                      SetWordToBytes(ZD_MODBUS[*Current_ZD_MODBUS].Coil,&Buff_TX[4] );           //il przesy³anych komórek
                      Buff_TX[6]  =(ZD_MODBUS[*Current_ZD_MODBUS].Coil) *2;                      //il przesy³anych bajtów     
                      for(int i = 0; i < ZD_MODBUS[*Current_ZD_MODBUS].Coil; ++i)
                      {
                        Buff_TX[7+(i*2)]=ram.VarCom_Byte[((ZD_MODBUS[*Current_ZD_MODBUS].StartAdr)*2)+(i*2)];
                        Buff_TX[8+(i*2)]=ram.VarCom_Byte[((ZD_MODBUS[*Current_ZD_MODBUS].StartAdr)*2)+(i*2)+1];
                      }
                      
                      SetWordToBytes(crc16(Buff_TX,(ZD_MODBUS[*Current_ZD_MODBUS].Coil *2 )+7),&Buff_TX[7+(ZD_MODBUS[*Current_ZD_MODBUS].Coil *2 )] );                    //wstawia sumê kontroln¹                     
                      Buff_TX[9+(ZD_MODBUS[*Current_ZD_MODBUS].Coil *2 )]=0;
                      AT91F_US_SendFrame(AT91C_BASE_US,Buff_TX,9+(ZD_MODBUS[*Current_ZD_MODBUS].Coil *2 ),0, 0);                        //Wysyla ramke        
                      g_US0_Comm_MinCykle =  (int)((g_US0_OneByteTxTime * (9+(ZD_MODBUS[*Current_ZD_MODBUS].Coil *2 ))) +10);   
                                               
            }    
                
          
               
*/

       return 1;
}//_______________________ Koniec funkcji ModBus_MASTER_TX_US0 _______________________



                                



//!-----------------------------------------------------------------------------
//!  Nazwa funkcji :    ModBus_MASTER_Return
//!  Opis:              Funkcja konwertuje wartoœæ w WORD na 2 kolejne bajty
//!-----------------------------------------------------------------------------
//!  modbusframe  - otrzymana ramka danych
//!----------------------------------------------------------------------------- 

BYTE ModBus_MASTER_Return(BYTE* modbusframe, char US_Nr)
{
  //WORD  RamByte=(((((WORD)modbusframe[2])<<8 | ((WORD)modbusframe[3])))/8);
  //WORD  RamCall=((((WORD)modbusframe[2])<<8 | ((WORD)modbusframe[3])));
  //BYTE  RamBit= ((((WORD)modbusframe[2])<<8 | ((WORD)modbusframe[3]))+0) % 8;
  //BYTE  ByteCount=(((((WORD)modbusframe[4])<<8 | ((WORD)modbusframe[5]))-1)/8)+1;
  //BYTE  WordCount=(((WORD)modbusframe[4])<<8 | ((WORD)modbusframe[5]));
  //WORD  BitCount = 0;
  

  char  CurentByteBufTX = 0;
  char  CurentBitBufTX = 0;
  WORD ByteCount =0;
  WORD WordCount =0;
  WORD BitCount = 0;
  unsigned short RamByteTMP;

  WORD RamBit = 0;
  
      ptr_ZD_MODBUS ZD_MODBUS;
      char* Current_ZD_MODBUS;
      //char* ERR_ZD_MODBUS;
      //BYTE* Buff_TX;
      
    
      //wybór USARTa
      if (US_Nr) 
      {
          ZD_MODBUS         =   ZD_MODBUS_1;
          Current_ZD_MODBUS =   &g_Current_ZD_MODBUS_1;
         // ERR_ZD_MODBUS     =   &g_ERR_ZD_MODBUS_1;
         // Buff_TX           =   Buff_TX_1; 
      }   
      else 
      {
          ZD_MODBUS         =   ZD_MODBUS_0;
          Current_ZD_MODBUS =   &g_Current_ZD_MODBUS_0;
          //ERR_ZD_MODBUS     =   &g_ERR_ZD_MODBUS_0;
          //Buff_TX           =   Buff_TX_0; 
      }
        
  
    g_Ans_ZD_MODBUS_0 = g_US0_Comm_TIMEOUT+1; 
     
    
    if ((modbusframe[0]) != ZD_MODBUS[*Current_ZD_MODBUS].Adress) 
      {
          ZD_MODBUS[*Current_ZD_MODBUS].Error++;
          //TRACE_INFO("\n\r ODP : Blad adresu \n\r");
          return 0;                                                                   //wyjdz jezeli zly adres 
      }


  

      
            
            //Response to question: Read Coil Status  (odczyt zmiennych dwustanowych do DA00-DDFF)
    
            if(modbusframe[1]==1)
            {
                      ByteCount = ((WORD)modbusframe[2]);
                      BitCount =  ((WORD)modbusframe[2]) * 8;
                      if ((((WORD)modbusframe[ByteCount+3])<<8 | ((WORD)modbusframe[ByteCount+4])) != crc16(modbusframe,ByteCount+3)) return 0;   //sprawdzenie CRC

                      CurentByteBufTX=3;
                      CurentBitBufTX=0;

                      RamByteTMP = ZD_MODBUS[*Current_ZD_MODBUS].Target;
                      RamBit  = 0;
                      
                      for (int i=0; i<BitCount; i++)
                      { 
                        if ((modbusframe[CurentByteBufTX]>>CurentBitBufTX)&0x1)
                        {
                          ram.VarCom_Word[RamByteTMP] |= 0x1 << RamBit;
                        }
                        else
                        {
                          ram.VarCom_Word[RamByteTMP] &= ~(0x1 << RamBit);
                        }   
                       
                        if (RamBit<=6) {RamBit++;} else {RamBit=0; RamByteTMP++;}
                        if (CurentBitBufTX<7) {CurentBitBufTX++;} else {CurentBitBufTX=0; CurentByteBufTX++;}
                        
                      }
             
                      ZD_MODBUS[*Current_ZD_MODBUS].Error=0;
                      ZD_MODBUS[*Current_ZD_MODBUS].Confirm=1;
                      
            }
                      
                  
             //Response to question: Read discrete input  (odczyt zmiennych dyskretnych B000-B5FF)
            if(modbusframe[1]==2)
            {
                             ByteCount = ((WORD)modbusframe[2]);
                      BitCount =  ((WORD)modbusframe[2])*8;
                      if ((((WORD)modbusframe[ByteCount+3])<<8 | ((WORD)modbusframe[ByteCount+4])) != crc16(modbusframe,ByteCount+3)) return 0;   //sprawdzenie CRC

                      CurentByteBufTX=3;
                      CurentBitBufTX=0;

                      
                      RamByteTMP = ZD_MODBUS[*Current_ZD_MODBUS].Target/8;
                      RamBit  = 0;
                      
                      for (int i=0; i<BitCount; i++)
                      { 
                        if ((modbusframe[CurentByteBufTX]>>CurentBitBufTX)&0x1)
                        {
                          ram.VarCom_Word[RamByteTMP] |= 0x1 << RamBit;
                        }
                        else
                        {
                          ram.VarCom_Word[RamByteTMP] &= ~(0x1 << RamBit);
                        }   
                       
                        if (RamBit<=6) {RamBit++;} else {RamBit=0; RamByteTMP++;}
                        if (CurentBitBufTX<7) {CurentBitBufTX++;} else {CurentBitBufTX=0; CurentByteBufTX++;}
                        
                      }
             
                      ZD_MODBUS[*Current_ZD_MODBUS].Error=0;
                      ZD_MODBUS[*Current_ZD_MODBUS].Confirm=1;
              
            }     
                      
            //Response to question: Holding Register (odczyt zmiennych analogowych DA00-DDFF)
            if(modbusframe[1]==3)
            {
                
                      
                      //BitCount =  ((WORD)modbusframe[2]) * 8;
                      ByteCount = (modbusframe[2]);        
              
                      if ((((WORD)modbusframe[ByteCount+3])<<8 | ((WORD)modbusframe[ByteCount+4])) != crc16(modbusframe,ByteCount+3)) return 0;   //sprawdzenie CRC
                
                      WordCount=ByteCount/2;
                      
                      CurentByteBufTX=3;
                      
                      RamByteTMP = 0;
                      
                      RamByteTMP = (ZD_MODBUS[*Current_ZD_MODBUS].Target)*2;
                      RamBit  = 0;
                      
                      for (WORD i=0; i<WordCount; i++)
                      { 
                        ram.VarCom_Byte[RamByteTMP+1]   = modbusframe[CurentByteBufTX];
                        ram.VarCom_Byte[RamByteTMP] = modbusframe[CurentByteBufTX+1];
                        
                        CurentByteBufTX +=2;
                        RamByteTMP      +=2;
                      }            
                      
                      ZD_MODBUS[*Current_ZD_MODBUS].Error=0;
                      ZD_MODBUS[*Current_ZD_MODBUS].Confirm=1;
                      
            }
                      
            //Response to question: Input Register (odczyt analogów wejœciowych A000-A5FF)      
           if(modbusframe[1]==4)
            {

                      //BitCount =  ((WORD)modbusframe[2]) * 8;
                      ByteCount = (modbusframe[2]);        
              
                      if ((((WORD)modbusframe[ByteCount+3])<<8 | ((WORD)modbusframe[ByteCount+4])) != crc16(modbusframe,ByteCount+3)) return 0;   //sprawdzenie CRC
                
                      WordCount=ByteCount/2;
                      
                      CurentByteBufTX=3;
                      
                      RamByteTMP = 0;
                      
                      RamByteTMP = (ZD_MODBUS[*Current_ZD_MODBUS].Target)*2;
                      RamBit  = 0;
                      
                      for (WORD i=0; i<WordCount; i++)
                      { 
                        ram.VarCom_Byte[RamByteTMP+1]   = modbusframe[CurentByteBufTX];
                        ram.VarCom_Byte[RamByteTMP] = modbusframe[CurentByteBufTX+1];
                        
                        CurentByteBufTX +=2;
                        RamByteTMP      +=2;
                      }       
                      
                      ZD_MODBUS[*Current_ZD_MODBUS].Error=0;
                      ZD_MODBUS[*Current_ZD_MODBUS].Confirm=1;
                      
            }
                      
            //Response to question: Read Coil Status  (odczyt zmiennych dwustanowych do DA00-DDFF)
            if(modbusframe[1]==5)
            {
              
                  if ((((WORD)modbusframe[6])<<8 | ((WORD)modbusframe[7])) != crc16(modbusframe,6)) return 0;   //sprawdzenie CRC
                  if (ZD_MODBUS[*Current_ZD_MODBUS].Target!=(((WORD)modbusframe[2])<<8 | ((WORD)modbusframe[3]))) return 0; //sprawdzenie poprawnoœci adresu

                  ZD_MODBUS[*Current_ZD_MODBUS].Error=0;
                  ZD_MODBUS[*Current_ZD_MODBUS].Confirm=1;
                  
              
            }
            //Response to question: Write Single Register  (zapis zmiennych analogowych - pojedynczo DA00-DDFF)
            if(modbusframe[1]==6)
            {
              
            }
                      
            //Response to question: Write Multiple Status  (zapis zmiennych dwustanowych - grupowo DA00-DDFF)
            if(modbusframe[1]==15)
            {
                       
                    ZD_MODBUS[*Current_ZD_MODBUS].Confirm=1;
                    ZD_MODBUS[*Current_ZD_MODBUS].Error=0;
              
            } 
              
            //Response to question: Write Single Register  (zapis zmiennych analogowych - pojedynczo DA00-DDFF)
            if(modbusframe[1]==16)
            {
 
                    ZD_MODBUS[*Current_ZD_MODBUS].Confirm=1;
              
              
                
          }
    return 1; 
    
}//_________________ Koniec funkcji ModBus_MASTER_Return  __________________


