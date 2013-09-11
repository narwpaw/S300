//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                 IASE Microcontroller Software Support                    *//
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : Main_H.h                                 *//
//*  Opis                         : Plik nag³ówkowy dla pliku Main.c         *//
//*  Autor                        : Pawe³ Narwojsz                           *//
//*  Data utrzorzenia             : 5.05.2008                               *//
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


#ifndef __Main_H__
#define __Main_H__



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
     #include "RF.h"
     #include "spi_main.h"
     #include "InterruptPriority.h"
     #include "RF_Exe.h"
     #include "Clock.h"
    // #include "WizFi.h"




void Delay (unsigned long a);
void Timer_10ms(void);
void ThermostatsTimeChartExecute(void);
void Timer_1h(void);    
void SetWiFiSuspend(void);
void SendViaUSB(unsigned char *Buff, unsigned short Size);


#endif
