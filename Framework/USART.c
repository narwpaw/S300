//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                                 CEURON                                   *//
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : USART.c                                  *//
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



//Zmienne konfiguracyjne -------------------------------------------------------     

    //US0----------------------
      int  g_US0_BAUDRATE             = 19200;                                   //prêdkoœæ transmisji US0
      int  g_US0_Comm_TIMEOUT         = 12;                                      //(w ms) Czas oczekiwania na odpowiedŸ urz¹dzenia odpytywanego poprzez MODBUS
      char q_US0_Comm_Mode            = 3;
      
    //US1----------------------
      int g_US1_BAUDRATE             = 115200;                                   //prêdkoœæ transmisji US0
      int g_US1_Comm_TIMEOUT         = 100;                                      //(w ms) Czas oczekiwania na odpowiedŸ urz¹dzenia odpytywanego poprzez MODBUS  
//___________________________Koniec zmiennych lokalnych________________________





//------------------------------------------------------------------------------
//      Includes
//------------------------------------------------------------------------------


#include "USART.h"
#include "WizFi.h"


//Zmienne lokalne -------------------------------------------------------------
    char SendModbusEnable = 1;                                                  //komunikacja z zewnêtrznym urz¹dzeniem poprzez MODBUS RS485 
    
//___________________________Koniec zmiennych lokalnych________________________




//Zmienne globalne -------------------------------------------------------------
    char   g_US0_Connect_OK = 0;                                                //1 - trwa komunikacja poprzez US0
    char   g_US1_Connect_OK = 0;                                                //1 - trwa komunikacja poprzez US1
    char   g_ETH0_Connect_OK = 0;                                               //1 - trwa komunikacja poprzez ETH0
    char   g_ETH1_Connect_OK = 0;                                               //1 - trwa komunikacja poprzez ETH0
    int    g_US0_Comm_MinCykle = 10;                                            //Czas pomiêdzy kolejnymi pytaniami (przy poprawnych odpowiedziach) US0
    float  g_US0_OneByteTxTime = 5;                                             //Czas trwania jednego bajtu (w ms) US0
    int    g_US1_Comm_MinCykle = 1;                                            //Czas pomiêdzy kolejnymi pytaniami (przy poprawnych odpowiedziach) US1
    float  g_US1_OneByteTxTime = 5;                                             //Czas trwania jednego bajtu (w ms) US1
    unsigned char  US0_Buffer[US0_BUFFER_Rx_SIZE];                                  //bufor odbiorczy USART 0
    unsigned char  US1_Buffer[US1_BUFFER_Rx_SIZE];                                  //bufor odbiorczy USART 0
    
    
//___________________________Koniec zmiennych globalnych________________________
    
    

//Zmienne zewnêtrzne -----------------------------------------------------------    
    extern char US0_MasterMODBUS_ENABLE;
    extern char US1_MasterMODBUS_ENABLE;
    extern BYTE Buff_TX_1[];
    extern _Net NET; 
//___________________________Koniec zmiennych zewnêtrznych______________________    
 

    
    
    
    
//-----------------------------------------------------------------------------
//!  Nazwa funkcji :    US0_init
//!  Funkcja inicjalizacji USART 0
//-----------------------------------------------------------------------------
//! 
//-----------------------------------------------------------------------------
void US0_init(void)
{

  
  
        // Configure USART
      AT91F_US0_CfgPMC();
      AT91F_PDC_DisableRx(AT91C_BASE_PDC_US0);
      AT91F_PDC_DisableTx(AT91C_BASE_PDC_US0);
     
      AT91F_US_Configure(AT91C_BASE_US0, AT91C_MASTER_CLOCK,
                       USART485_MODE, g_US0_BAUDRATE, 0);
  
      AT91C_BASE_US0->US_CR = AT91C_US_STTTO;

      AT91F_US_EnableIt(AT91C_BASE_US0,AT91C_US_RXBUFF | AT91C_US_TIMEOUT );     //uruchomienie/ustawienie przerwania od przepe³nienia bufora oraz od time-out

      // konfiguracja przerqwania USART0 
      AT91F_AIC_ConfigureIt ( AT91C_BASE_AIC, AT91C_ID_US0, USART0_INTERRUPT_LEVEL, AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL, Usart0_handler); 
        
      AT91F_US_EnableRx(AT91C_BASE_US0);
      AT91F_US_EnableTx(AT91C_BASE_US0);

   
      AT91F_US0_CfgPIO();                                                       //konfiguracja pinów
      
      unsigned int xxx= AT91F_US_ReceiveFrame (                                 //Ustawienie bufora odbiorczego
	AT91C_BASE_US0,
	US0_Buffer,
	US0_BUFFER_Rx_SIZE,
	0,
	0 );

      AT91C_BASE_US0->US_RTOR = TIME_OUT_US;                                    //ustawienie czasu time-out (w bitach)
      AT91C_BASE_US0->US_CR = AT91C_US_STTTO;                                   //restart time-out

      //wy³¹czone przerwanie bo koliduje z  innymi przerwaniami 
      /*      
      AT91F_AIC_EnableIt (AT91C_BASE_AIC, AT91C_ID_US0);                        //uruchomienie przerwania od USARTA
      */
      g_US0_OneByteTxTime  = 1000/ ((float) (g_US0_BAUDRATE /8));
      
}//_______________________ Koniec funkcji US0_init ____________________________ 






//-----------------------------------------------------------------------------
//! Nazwa funkcji :    Usart0_handler
//! Funkcja obs³ugi przerwania USART 0
//-----------------------------------------------------------------------------
//  
//----------------------------------------------------------------------------- 
void Usart0_handler(void)                        //timer isr 
{ 
  
  //char CheckNoZero;                          //sprawdza czy tabela odczytów TP nie ma zer
    unsigned int dummy = AT91C_BASE_US0->US_CSR;                  //interrupts    

    AT91PS_USART USART_pt = AT91C_BASE_US0; 
    unsigned int status; 
    
    
    //* get Usart status register 
    status = USART_pt->US_CSR; 
  
     
    if ( status & AT91C_US_TIMEOUT)       //wystapil timeout lub odebrano dane
      {

        AT91C_BASE_US0->US_CR = AT91C_US_STTTO;
        AT91C_BASE_US0->US_RCR = 0;

        unsigned int xxx= AT91F_US_ReceiveFrame (
	USART_pt,
	US0_Buffer,
	US0_BUFFER_Rx_SIZE,
	0,
	0 );
        
        SendModbusEnable=1;


      }
   
        status = USART_pt->US_CSR; 
    

 
  
}//_______________________ Koniec funkcji Usart0_handler _________________________
 






//-----------------------------------------------------------------------------
//!  Nazwa funkcji :    US1_init
//!  Funkcja inicjalizacji USART 1
//-----------------------------------------------------------------------------
//! 
//-----------------------------------------------------------------------------
void US1_init(void)
{
      // Configure USART
      AT91F_US1_CfgPMC();
      AT91F_PDC_DisableRx(AT91C_BASE_PDC_US1);
      AT91F_PDC_DisableTx(AT91C_BASE_PDC_US1);
      
      AT91F_US_Configure(AT91C_BASE_US1, AT91C_MASTER_CLOCK,
                       USART232_MODE, g_US1_BAUDRATE, 0);
    
      AT91C_BASE_US1->US_CR = AT91C_US_STTTO;

      AT91F_US_EnableIt(AT91C_BASE_US1,AT91C_US_RXBUFF | AT91C_US_TIMEOUT );     //uruchomienie/ustawienie przerwania od przepe³nienia bufora oraz od time-out

      // konfiguracja przerqwania USART0 
      AT91F_AIC_ConfigureIt ( AT91C_BASE_AIC, AT91C_ID_US1, USART1_INTERRUPT_LEVEL, AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL, Usart1_handler); 
        
      AT91F_US_EnableRx(AT91C_BASE_US1);
      AT91F_US_EnableTx(AT91C_BASE_US1);

      
      AT91F_US1_CfgPIO();                                                       //konfiguracja pinów
      
      unsigned int xxx= AT91F_US_ReceiveFrame (                                 //Ustawienie bufora odbiorczego
	AT91C_BASE_US1,
	US1_Buffer,
	US1_BUFFER_Rx_SIZE,
	0,
	0 );

      AT91C_BASE_US1->US_RTOR = TIME_OUT_US1;                                    //ustawienie czasu time-out (w bitach)
      AT91C_BASE_US1->US_CR = AT91C_US_STTTO;                                   //restart time-out
      
      AT91F_AIC_EnableIt (AT91C_BASE_AIC, AT91C_ID_US1);                        //uruchomienie przerwania od USARTA
      
      g_US1_OneByteTxTime  = 1000/ ((float) (g_US1_BAUDRATE /8));
}//_______________________ Koniec funkcji US1_init ____________________________ 






//-----------------------------------------------------------------------------
//! Nazwa funkcji :    Usart1_handler 
//! Funkcja obs³ugi przerwania USART 1
//-----------------------------------------------------------------------------
//  
//----------------------------------------------------------------------------- 
void Usart1_handler(void)                        //timer isr 
{ 
  
  
  //char CheckNoZero;                          //sprawdza czy tabela odczytów TP nie ma zer
    unsigned int dummy = AT91C_BASE_US1->US_CSR;                  //interrupts    

    AT91PS_USART USART_pt = AT91C_BASE_US1; 
    unsigned int status; 
    
    
    //* get Usart status register 
    status = USART_pt->US_CSR; 
   
   
    if (( status & AT91C_US_TIMEOUT) || ( status & AT91C_US_RXBUFF))      //wystapil timeout lub odebrano dane
      {

        AT91C_BASE_US1->US_CR = AT91C_US_STTTO;
        AT91C_BASE_US1->US_RCR = 0;

     
        unsigned int xxx= AT91F_US_ReceiveFrame (
	USART_pt,
	US1_Buffer,
	US1_BUFFER_Rx_SIZE,
	0,
	0 );
        
        WizFiResponseInterpreter(US1_Buffer,Buff_TX_1 ,AT91C_BASE_US1, &NET);

      }
   
}//_______________________ Koniec funkcji Usart0_handler _________________________
 

