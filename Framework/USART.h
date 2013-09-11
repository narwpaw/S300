//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                                 CEURON                                   *//                       
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : USART.h                                   *//
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

#ifndef __USART__
#define __USART__


//#include "core/common.h"
//#include "core/device.h"                                                        //aduje bibliotreki dla odpowiedniego procesora
//#include "USART.h"
#include "Modbus.h"
//#include "core/board.h"




//Definicje --------------------------------------------------------------------               
    #define US0_BUFFER_Rx_SIZE        20                                        //bufor odbiorczy USART 0 (Dla RS485 Modbus)
    #define US0_BUFFER_Tx_SIZE        20                                        //bufor nadawczy USART 0  (Dla RS485 Modbus)
    #define US1_BUFFER_Rx_SIZE        580                                       //260 bufor odbiorczy USART 1 (Dla Rmodu³u Ethernet)
    #define US1_BUFFER_Tx_SIZE        580                                       //400 bufor nadawczy USART 1  (Dla Rmodu³u Ethernet)
    #define TIME_OUT_US               10                                        //TimeOut USARTa (w bitach)
    #define TIME_OUT_US1              60                                        //TimeOut USARTa (w bitach)
    #define AT91C_US_TIMEOUT      ((unsigned int) 0x1 <<  8) // (DBGU) TXEMPTY Interrupt
                                         //poziom priorytetu przerwania USART0

    #define USART232_MODE (AT91C_US_USMODE_NORMAL | \
                       AT91C_US_NBSTOP_1_BIT | \
                       AT91C_US_PAR_NONE | \
                       AT91C_US_CHRL_8_BITS | \
                       AT91C_US_CKLO )


    #define USART485_MODE (AT91C_US_USMODE_RS485 | \
                       AT91C_US_NBSTOP_1_BIT | \
                       AT91C_US_PAR_NONE | \
                       AT91C_US_CHRL_8_BITS | \
                       AT91C_US_CKLO )

    #define USART485_MODE_8_NP (AT91C_US_USMODE_RS485 | \
                        AT91C_US_NBSTOP_1_BIT | \
                       AT91C_US_PAR_ODD | \
                       AT91C_US_CHRL_8_BITS | \
                       AT91C_US_CKLO )

    #define USART485_MODE_8_PA (AT91C_US_USMODE_RS485 | \
                        AT91C_US_NBSTOP_1_BIT | \
                       AT91C_US_PAR_EVEN | \
                       AT91C_US_CHRL_8_BITS | \
                       AT91C_US_CKLO )

    #define USART485_MODE_8_BP (AT91C_US_USMODE_RS485 | \
                        AT91C_US_NBSTOP_1_BIT | \
                       AT91C_US_PAR_NONE | \
                       AT91C_US_CHRL_8_BITS | \
                       AT91C_US_CKLO )

    #define USART485_MODE_7_NP (AT91C_US_USMODE_RS485 | \
                        AT91C_US_NBSTOP_1_BIT | \
                       AT91C_US_PAR_ODD | \
                       AT91C_US_CHRL_7_BITS | \
                       AT91C_US_CKLO )

    #define USART485_MODE_7_PA (AT91C_US_USMODE_RS485 | \
                        AT91C_US_NBSTOP_1_BIT | \
                       AT91C_US_PAR_EVEN | \
                       AT91C_US_CHRL_7_BITS | \
                       AT91C_US_CKLO )

    #define USART485_MODE_7_BP (AT91C_US_USMODE_RS485 | \
                        AT91C_US_NBSTOP_1_BIT | \
                       AT91C_US_PAR_NONE | \
                       AT91C_US_CHRL_7_BITS | \
                       AT91C_US_CKLO )


//___________________________Koniec definicji___________________________________




    
    
//Prototypy Funkcji  -----------------------------------------------------------    
    void Usart0_handler(void);
    void Usart1_handler(void);
    void US0_init(void);
    void US1_init(void);
//______________________________________________________________________________
    
    
    
#endif
