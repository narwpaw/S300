//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                 CEURON Microcontroller Software Support                  *//                       
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : InterruptPriority.h                      *//
//*  Opis                         : Plik ustawiania proiorytetów             *//
//*  Autor                        : Pawe³ Narwojsz                           *//
//*  Data utrzorzenia             : 30.06.2010                               *//
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



//#define AT91C_AIC_PRIOR       ((unsigned int) 0x7 <<  0) // (AIC) Priority Level
#define 	PRIOR_0                 ((unsigned int) 0x0) 
#define 	PRIOR_1                 ((unsigned int) 0x1) 
#define 	PRIOR_2                 ((unsigned int) 0x2) 
#define 	PRIOR_3                 ((unsigned int) 0x3)
#define 	PRIOR_4                 ((unsigned int) 0x4) 
#define 	PRIOR_5                 ((unsigned int) 0x5) 
#define 	PRIOR_6                 ((unsigned int) 0x6) 
#define 	PRIOR_7                 ((unsigned int) 0x7) 



  /*
//dzia³a poprawni z USB 
#define 	TIMER_10US_PRIOR            PRIOR_6
#define 	TIMER_1mS_PRIOR             PRIOR_4
#define 	TIMER_10mS_PRIOR            PRIOR_3
#define 	RF_INT_PRIOR                PRIOR_6  //6
#define 	SPI_PRIOR                   PRIOR_6  //5
#define 	USB_CON_PRIOR               PRIOR_7
#define         USB_PRIOR                   PRIOR_7  

 #define         USART0_INTERRUPT_LEVEL      PRIOR_2  
   */
   
 #define 	TIMER_10US_PRIOR            PRIOR_1
#define 	TIMER_1mS_PRIOR             PRIOR_4
#define 	TIMER_10mS_PRIOR            PRIOR_3
//#define 	RF_INT_PRIOR                PRIOR_7 
#define 	PIO_INT_PRIOR               PRIOR_7 
#define 	SPI_PRIOR                   PRIOR_6  
#define 	USB_CON_PRIOR               PRIOR_5
#define         USB_PRIOR                   PRIOR_5  
#define         USART0_INTERRUPT_LEVEL      PRIOR_2       
#define         USART1_INTERRUPT_LEVEL      PRIOR_5
                                        