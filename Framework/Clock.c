//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                 IASE Microcontroller Software Support                    *//                       
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : Clock.c                                  *//
//*  Opis                         : Plik obs³ugi zegara czasu rzeczywistego  *//
//*                                 DS1306                                   *//
//*  Autor                        : Pawe³ Narwojsz                           *//
//*  Data utrzorzenia             : 15.01.2009                               *//
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



//Konfiguracja -----------------------------------------------------------------
  
//___________________________Koniec konfiguracji________________________________




//Definicje --------------------------------------------------------------------

//___________________________Koniec definicji___________________________________



// Pliki do³¹czane -------------------------------------------------------------
     #include <intrinsics.h>
     #include "Framework.h"
     #include "core/device.h"                                                   //do³¹cza bibliotreki do odpowiedniego procesora
     #include "USART.h"
     #include "core/common.h"
     #include "core/board.h"
     #include "core/trace.h"
     #include "core/usb.h"
     #include "core/standard.h"
     #include "cdc.h"
     #include "serial_driver.h"
     #include "modbus.h"
     #include "main_H.h"


   #include "Clock.h"
   #include "spi_main.h"
#include "Application_prog.h"
//-----------------------Koniec plików do³¹czanych -----------------------------




//Sta³e ------------------------------------------------------------------------

//____________________________Koniec sta³ych ___________________________________





//Zmienne globalne -------------------------------------------------------------
  extern _DateTime DateTime;                                                       //zienna daty i czasu
  _DateTime  *pDateTime    = &DateTime;
//___________________________Koniec zmiennych globalnych________________________





//Zmienne lokalne -------------------------------------------------------------

//___________________________Koniec zmiennych lplkalnych________________________


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
  #define order_set_ChargerReg    0x91
  #define order_read_ChargerReg   0x11
  #define order_set_ComandReg     0x8F
//___________________________Koniec definicji___________________________________
  

//-----------------------------------------------------------------------------
//! Nazwa funkcji :  InitRTC  
//! Funkcja ustawieñ startowych RTC
//-----------------------------------------------------------------------------
 
void InitRTC(void)  //ustawienia startowe RTC
{

  
   char command[] = {order_set_ComandReg,0x0,0};  //0x8F ustawienie rejestru CR
   SPI_SendB_Buff(command,2,Cs_Clock);

}//_____________________________________________________________________________

    
    

//-----------------------------------------------------------------------------
//! Nazwa funkcji :  GetDateTime  
//! Funkcja pobierania daty i czasu
//-----------------------------------------------------------------------------
 
void GetDateTimeFromRTC(void)  //pobiera czas z zegara czasu rzeczywistego i wrzuca do zmiennej DateTime
{

  
   char command[] = {0x00,0x01,0x2,0x3, 0x4,0x5,0x6,0};
   SPI_SendB_Buff(command,8,Cs_Clock);
    
    
}//_____________________________________________________________________________




//-----------------------------------------------------------------------------
//! Nazwa funkcji :  SetTime  
//! Funkcja ustawiania czasu
//-----------------------------------------------------------------------------
static  char ComandCLK[10];
void SetRTC_DateTime(void)               //Ustawinie: sekundy, minuty, godziny, dnia, miesi¹ca, roku
{


  
char Sec        = ((DateTime.Second/10)<<4)       |  ((DateTime.Second%10) &0xF);
char Min        = ((DateTime.Minute/10)<<4)       |  ((DateTime.Minute%10) &0xF);
char Hour       = ((DateTime.Hour/10)<<4)         |  ((DateTime.Hour%10) &0xF);
char Day        = ((DateTime.Day/10)<<4)          |  ((DateTime.Day%10) &0xF);
char Mounth     = ((DateTime.Mounth/10)<<4)       |  ((DateTime.Mounth%10) &0xF);
char Year       = ((DateTime.Year/10)<<4)         |  ((DateTime.Year%10) &0xF);
char DayOfWeek  = ((DateTime.DayOfWeek/10)<<4)    |  ((DateTime.DayOfWeek%10) &0xF);
  
ComandCLK[0]=0x80;
ComandCLK[1]=Sec;
ComandCLK[2]=Min;
ComandCLK[3]=Hour;
ComandCLK[4]=DayOfWeek;
ComandCLK[5]=Day;
ComandCLK[6]=Mounth;
ComandCLK[7]=Year;
ComandCLK[8]=0;
//  char comand[]={0x80, Sec, Min, Hour, DayOfWeek, Day, Mounth, Year}; 
int DTS=8;
  SPI_SendB_Buff(ComandCLK,DTS,Cs_Clock); 
  
   
}//_____________________________________________________________________________





//-----------------------------------------------------------------------------
//! Nazwa funkcji :    SPI_Response_Interpretation
//! Funkcja interpretacji danych otrzymanych z lini SPI
//  Buffer_Tx - parametr funkcji zawieraj¹cy wskaŸnik do danych wys³anych odebranych
//-----------------------------------------------------------------------------
char SPI_Clock_Execute(char *Buffer_Tx, char *Buffer_Rx)                                       
{
    char res=0; 
    switch (*Buffer_Tx)
    {

      case order_set_sec:                                                       //wykonano ustawienie sekundy jako czêœæ ustawiania godziny/daty  
           for (char i=0; i<16; i++)
           {
             Buffer_Tx[i]=0;
             Buffer_Rx[i]=0;
           }
           res = 8;
      break;

      case order_read_ChargerReg:                                                       //wykonano ustawienie sekundy jako czêœæ ustawiania godziny/daty  

           res = 2;
      break;

      case order_set_ChargerReg:                                                       //wykonano ustawienie sekundy jako czêœæ ustawiania godziny/daty  

           res = 2;
      break;      
      
      
      case order_read_sec:                                                      //wykonano odczyt sekundy jako czêœæ odczytu godziny/daty  
          
        
        
         pDateTime->Second     = (Buffer_Rx[0]&0xf) +  ((Buffer_Rx[0]>>4)&0x7)*10;
         pDateTime->Minute     = (Buffer_Rx[5]&0xf) +  ((Buffer_Rx[5]>>4)&0x7)*10;
         pDateTime->Hour       = (Buffer_Rx[4]&0xf) +  ((Buffer_Rx[4]>>4)&0x3)*10;
         pDateTime->DayOfWeek  = Buffer_Rx[9];
         pDateTime->Day        = (Buffer_Rx[8]&0xf) +  ((Buffer_Rx[8]>>4)&0x3)*10;
         pDateTime->Mounth     = (Buffer_Rx[13]&0xf) +  ((Buffer_Rx[13]>>4)&0x3)*10;
         pDateTime->Year       = (Buffer_Rx[12]&0xf) +  ((Buffer_Rx[12]>>4)&0xF)*10;
     

         TRACE_INFO("%d:", pDateTime->Hour); 
         TRACE_INFO("%d:", pDateTime->Minute); 
         TRACE_INFO("%d     ", pDateTime->Second);  
         TRACE_INFO("%d-", pDateTime->Day);  
         TRACE_INFO("%d-", pDateTime->Mounth);  
         TRACE_INFO("%d    ", pDateTime->Year);  
          TRACE_INFO("\n\r");  
        
          for (char i=0; i<16; i++)
           {
             Buffer_Tx[i]=0;
             Buffer_Rx[i]=0;
           }
          
           res = 8;
      break;
      

      

      
      
    }

   return res;
}//_____________________________________________________________________________


