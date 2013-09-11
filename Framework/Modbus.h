//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                 IASE Microcontroller Software Support                    *//
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : Modbus.h                                 *//
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
//****************************************************************************//



//Definicje --------------------------------------------------------------------     
    #define ZD_MODBUS_Tab_Max     20
//______________________________________________________________________________
      



#ifndef _MODBUS_H_
#define _MODBUS_H_
                                                       //aduje bibliotreki dla odpowiedniego procesora

#include "Framework.h"
#include "core/device.h"                                                        //aduje bibliotreki dla odpowiedniego procesora
#include "core/trace.h"
#include "main_H.h"
#include "ScreenMENU2.h"
#include "Application_prog.h" 

typedef union MBUS_DATA{
  float fMBus;
  int   iMBus;
  unsigned short  bMBus[4];
} MBUS;
  
#pragma pack(1)             
      //tablica zadañ modbus
      typedef struct Def_ZD_MODBUS
                {
                  char Adress;        //Adres pytanego urz¹dzenia
                  char Function;      //Funkcja
                  unsigned short Target;        //Miejsce importu danych (dla odczytu), zapisu pobrania danych (dla zapisu)
                  unsigned short StartAdr;      //Pocz¹tkowy adres danych (w urz¹dzeniu )
                  unsigned short Coil;          //Iloœæ danych do pobrania
                  unsigned short LastSend;      //Wartoœæ wysy³ana ostatnio (wyko¿ystywane od blokowania dwukrotnego zapisu dwustanów w urz¹dzeniu)
                            char Confirm;       //Potwierdzenie przyjêcia (wyko¿ystywane od blokowania dwukrotnego zapisu dwustanów w urz¹dzeniu)
                  unsigned short Error;         //B³¹d komunikacji
                }_ZD_MODBUS, *ptr_ZD_MODBUS;
#pragma pack()
      
      
                        
//MODBUS functions

unsigned short crc16(unsigned char* modbusframe,int Length);
void SetWordToBytes(unsigned short tmp, unsigned char* ptr);
unsigned char ModBus_CommS(unsigned char* modbusframe, char US_Nr);
char ModBus_MASTER_TX(char US_Nr);
unsigned char ModBus_MASTER_Return(unsigned char* modbusframe, char US_Nr);

#endif
