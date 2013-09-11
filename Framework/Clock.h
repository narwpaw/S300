//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                 IASE Microcontroller Software Support                    *//                       
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : Clock.h                                  *//
//*  Opis                         : Plik nag³ówkowy Clock.c                  *//
//*  Autor                        : Pawe³ Narwojsz                           *//
//*  Data utrzorzenia             : 14.01.2009                               *//
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

#ifndef Clock_H
#define Clock_H 


//Konfiguracja -----------------------------------------------------------------

//___________________________Koniec konfiguracji________________________________




//Definicje --------------------------------------------------------------------
  #define order_read_sec      0x00
  #define order_read_min      0x01
  #define order_read_hour     0x02
  #define order_read_DayOfW   0x03
  #define order_read_day      0x04
  #define order_read_month    0x05
  #define order_read_year     0x06 
  #define order_set_sec       0x80
  #define order_set_min       0x81
  #define order_set_hour      0x82
  #define order_set_DayOfW    0x83
  #define order_set_day       0x84
  #define order_set_month     0x85
  #define order_set_year      0x86
  #define order_set_ChargerReg   0x91
  #define order_read_ChargerReg  0x11
//___________________________Koniec definicji___________________________________




/*
  typedef struct __DateTimeType
  {
    char Hour; 
    char Min;
    char Sec;
    char Day;
    char Month;
    char Year; 
    char DayOfWeek; 
    
  }DateTimeType;
*/

/*
typedef struct{                               
       char  Year;
       char  Mounth;
       char  Day;
       char  Hour;
       char  Minute;
       char  Second;
       char  DayOfWeek;
      }_DataTime;

*/  

        extern void   GetDateTimeFromRTC(void);
        extern void   SetRTC_DateTime(void);
        extern char   SPI_Clock_Execute(char *Buffer_Tx,  char *Buffer_Rx);
        extern void InitRTC(void);  //ustawienia startowe RTC
  #endif

