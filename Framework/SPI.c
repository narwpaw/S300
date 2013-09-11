//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                 IASE Microcontroller Software Support                    *//
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : SPI.c                                  *//
//*  Opis                         : G³ówny plik programu                     *//
//*  Autor                        : Pawe³ Narwojsz                           *//
//*  Data utrzorzenia             : 17.06.2008                               *//
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
      int  g_SPI_BAUDRATE             = 500000;                                   //prêdkoœæ transmisji US0
      int  SPI_Comm_TIMEOUT           = 500;                                      //(w ms) Czas oczekiwania na odpowiedŸ urz¹dzenia odpytywanego poprzez MODBUS

      
//___________________________Koniec zmiennych lokalnych________________________





//------------------------------------------------------------------------------
//      Includes
//------------------------------------------------------------------------------
#include "core/common.h"
#include "core/device.h"                                                        //aduje bibliotreki dla odpowiedniego procesora
#include "core/board.h"
//#include "core/trace.h"
#include "SPI_H.h"



//Zmienne lokalne -------------------------------------------------------------
    
//___________________________Koniec zmiennych lokalnych________________________


    
   // #define SPI_Read_BUFFER_SIZE             30                                 //bufor odbiorczy SPI   //14.03.2013 - zmniejszenie bufora do 30B
   // #define SPI_Write_BUFFER_SIZE            30                                 //bufor nadawczy SPI    //14.03.2013 - zmniejszenie bufora do 30B

//Zmienne globalne -------------------------------------------------------------
    //unsigned char  SPI_Buffer_Rx[SPI_Read_BUFFER_SIZE];                         //bufor odbiorczy SPI 0
    //unsigned char  SPI_Buffer_Tx[SPI_Write_BUFFER_SIZE];                        //bufor odbiorczy SPI 0
    
    

    
//___________________________Koniec zmiennych globalnych________________________
    
    

//Zmienne zewnêtrzne -----------------------------------------------------------    
extern void Delay (unsigned long a);
//___________________________Koniec zmiennych zewnêtrznych______________________    
 

    
    
    
    
//-----------------------------------------------------------------------------
//!  Nazwa funkcji :    US0_init
//!  Funkcja inicjalizacji USART 0
//-----------------------------------------------------------------------------
//! 
//-----------------------------------------------------------------------------
    
    
    
    
void EnableSpi() 
{ 
   AT91F_PMC_EnablePeriphClock(AT91C_BASE_PMC, // PIO controller base address 
                        ((unsigned int) 1 << AT91C_ID_SPI)); 
   AT91F_SPI_Enable (AT91C_BASE_SPI); 
} 

void DisableSpi() 
{ 
   AT91F_PMC_DisablePeriphClock(AT91C_BASE_PMC, // PIO controller base address 
                        ((unsigned int) 1 << AT91C_ID_SPI)); 
   
} 

#define MCK 48054841
#define spi AT91C_BASE_SPI
#define spi_pdc AT91C_BASE_PDC_SPI
#define SPIBAUD 1000000
#define CS_BAUD MCK / SPIBAUD
void SPI_init(void) 
{ 
    //AT91F_SPI_Disable (AT91C_BASE_SPI);   
    
    
    AT91F_SPI_CfgPIO();                 // Konfiguruj wejœcia  
    AT91F_SPI_CfgPMC();                 // Konfiguracja PMC na uaktywnienie zegara peryferjów SPI 
    AT91F_SPI_Reset (AT91C_BASE_SPI);   // Reset SPI

   
   //  spi->SPI_MR = ((32 << 24) | AT91C_SPI_MSTR | /* Master */
   //                    AT91C_SPI_PS_FIXED); /* Fixed periheral (== 0) */
   
    
   // AT91F_SPI_CfgMode (AT91C_BASE_SPI, AT91C_SPI_MSTR | AT91C_SPI_PS_FIXED | AT91C_SPI_PCSDEC); 
    
   // AT91F_SPI_CfgMode (AT91C_BASE_SPI, AT91C_SPI_MSTR | AT91C_SPI_PS_VARIABLE |AT91C_SPI_PCSDEC ); 
    
     

     
/*
    AT91F_SPI_CfgCs(spi, 0,
AT91C_SPI_CPOL | 
(AT91C_SPI_BITS & AT91C_SPI_BITS_8) |
(AT91C_SPI_SCBR & (CS_BAUD << 8)) |
(AT91C_SPI_DLYBS & (128 << 16)) |
(AT91C_SPI_DLYBCT & (0 << 24)) 
);
*/

 AT91F_SPI_CfgMode (AT91C_BASE_SPI, AT91C_SPI_MSTR | AT91C_SPI_PS_FIXED | AT91C_SPI_PCSDEC | 1<<16);    
    
 
 
 AT91F_PDC_Open(spi_pdc);

   
    
    
    

    
    EnableSpi(); 
} 

int TMPx=10000;

void ReadWriteSpi (unsigned char* pReadBuffer, int nReadLen, unsigned char* pWriteBuffer, int nWriteLen, bool bHoldCS) 
{ 
   // Configure SPI 

 //   AT91C_BASE_PIOA->PIO_ODR = AT91C_PA12_MISO; //Configure in Input
 // AT91C_BASE_PIOA->PIO_PPUER = AT91C_PA12_MISO; //Enable PA15
  
   //AT91F_SPI_CfgCs (AT91C_BASE_SPI, 0, AT91C_SPI_CPOL | AT91C_SPI_BITS_8 |  (AT91C_SPI_SCBR & (CS_BAUD << 8)) | (bHoldCS ? AT91C_SPI_CSAAT : 0) |0x01010000 );  
   AT91F_SPI_CfgCs (AT91C_BASE_SPI, 1, AT91C_SPI_CPOL |  AT91C_SPI_BITS_8 |  (AT91C_SPI_SCBR & (CS_BAUD << 8)) | (bHoldCS ? AT91C_SPI_CSAAT : 0) | 0x01010000); 
  
   //AT91F_SPI_CfgCs (AT91C_BASE_SPI, 0, AT91C_SPI_CPOL | AT91C_SPI_BITS_8 |  (AT91C_SPI_SCBR & (CS_BAUD << 8)) | (bHoldCS ? AT91C_SPI_CSAAT : 0));  
   //AT91F_SPI_CfgCs (AT91C_BASE_SPI, 1, AT91C_SPI_CPOL |  AT91C_SPI_BITS_8 |  (AT91C_SPI_SCBR & (CS_BAUD << 8)) | (bHoldCS ? AT91C_SPI_CSAAT : 0)); 
  
   // AT91F_SPI_CfgCs (AT91C_BASE_SPI, 0, AT91C_SPI_NCPHA | AT91C_SPI_CPOL | AT91C_SPI_BITS_8 | (AT91C_SPI_SCBR & (CS_BAUD << 8)) | (bHoldCS ? AT91C_SPI_CSAAT : 0));  
   // AT91F_SPI_CfgCs (AT91C_BASE_SPI, 1, AT91C_SPI_NCPHA | AT91C_SPI_CPOL |  AT91C_SPI_BITS_8 | (AT91C_SPI_SCBR & (CS_BAUD << 8)) | (bHoldCS ? AT91C_SPI_CSAAT : 0)); 
   
   // AT91F_SPI_CfgCs (AT91C_BASE_SPI, 0,  AT91C_SPI_NCPHA |  AT91C_SPI_BITS_8 |  (AT91C_SPI_SCBR & (CS_BAUD << 8)) | (bHoldCS ? AT91C_SPI_CSAAT : 0));  
   // AT91F_SPI_CfgCs (AT91C_BASE_SPI, 1,  AT91C_SPI_NCPHA |  AT91C_SPI_BITS_8 |  (AT91C_SPI_SCBR & (CS_BAUD << 8)) | (bHoldCS ? AT91C_SPI_CSAAT : 0)); 
      
      
   //AT91C_BASE_SPI->SPI_TDR |= 1<<16;

  
   // Open the SPI - PDC (Reset) 
   AT91F_PDC_Open (AT91C_BASE_PDC_SPI); 
    
   // Disable Spi activity during initialization 
   AT91F_PDC_DisableRx (AT91C_BASE_PDC_SPI); 
   AT91F_PDC_DisableTx (AT91C_BASE_PDC_SPI); 

   // Initialize buffers 
   const char* pWBuffer; 
   if (nWriteLen == 0)     // Read-only 
   { 
      nWriteLen = nReadLen; 
      pWBuffer = (char*) pReadBuffer; 
   } 
    else 
      pWBuffer = (const char*) pWriteBuffer;

   if (nWriteLen < nReadLen)
   {
      nWriteLen = nReadLen;
   }
   

   AT91F_SPI_SendFrame (AT91C_BASE_SPI, (char*) pWBuffer, nWriteLen, 0, 0); 
   
   // Start transfers 
   if (nReadLen != 0)      // Read-only 
   { 

      AT91F_SPI_ReceiveFrame (AT91C_BASE_SPI, (char*) pReadBuffer, nReadLen, 0, 0); 
      AT91F_PDC_EnableRx (AT91C_BASE_PDC_SPI); 
   } 
   
    AT91F_PDC_EnableTx (AT91C_BASE_PDC_SPI); 
      

    //AT91C_BASE_SPI->SPI_IER = AT91C_SPI_ENDTX|AT91C_SPI_ENDRX; 
    // Wait Until Last RX 
//    while (! (AT91C_BASE_SPI->SPI_SR & AT91C_SPI_ENDRX)); 

   
   while ((AT91C_BASE_SPI->SPI_SR & AT91C_SPI_TXBUFE) == 0 && (AT91C_BASE_SPI->SPI_SR & AT91C_SPI_RXBUFF) == 0); 


   
   AT91F_PDC_Close (AT91C_BASE_PDC_SPI); 

// Flush SPI RDR buffer (in case there is pending data) 
   volatile int nRDR = AT91F_SPI_GetChar (AT91C_BASE_SPI); 
} 




















  
  
  /*
  
  
      // Configure USART
      AT91F_US0_CfgPMC();
      AT91F_PDC_DisableRx(AT91C_BASE_PDC_SPI);
      AT91F_PDC_DisableTx(AT91C_BASE_PDC_SPI);
      
      AT91F_SPI_Configure(AT91C_BASE_US0, AT91C_MASTER_CLOCK, USART485_MODE_8_PA, g_US0_BAUDRATE, 0);

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
	US_BUFFER_SIZE,
	0,
	0 );

      AT91C_BASE_US0->US_RTOR = TIME_OUT_US;                                    //ustawienie czasu time-out (w bitach)
      AT91C_BASE_US0->US_CR = AT91C_US_STTTO;                                   //restart time-out
      
      AT91F_AIC_EnableIt (AT91C_BASE_AIC, AT91C_ID_US0);                        //uruchomienie przerwania od USARTA
      
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
    
    
    // get Usart status register 
    status = USART_pt->US_CSR; 
   
     
    if ( status & AT91C_US_TIMEOUT)       //wystapil timeout lub odebrano dane
      {

        AT91C_BASE_US0->US_CR = AT91C_US_STTTO;
        AT91C_BASE_US0->US_RCR = 0;

        unsigned int xxx= AT91F_US_ReceiveFrame (
	USART_pt,
	US0_Buffer,
	US_BUFFER_SIZE,
	0,
	0 );
        
        SendModbusEnable=1;
        

        if (US0_MasterMODBUS_ENABLE)                                            
        {
          if (ModBus_MASTER_Return(US0_Buffer,0))                             //je¿eli Modbus US0 w trybie master
          {
            g_US0_Connect_OK=2;        
            SendModbusEnable=1;
          }
          
        }
        else
        { 
          if (ModBus_CommS(US0_Buffer,0)) g_US0_Connect_OK=2;                 //je¿eli Modbus US0 w trybie slave
        }
          
  
  
        status = USART_pt->US_CSR; 


      }
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
                       USART485_MODE, g_US1_BAUDRATE, 0);
    
      AT91C_BASE_US1->US_CR = AT91C_US_STTTO;

      AT91F_US_EnableIt(AT91C_BASE_US1,AT91C_US_RXBUFF | AT91C_US_TIMEOUT );     //uruchomienie/ustawienie przerwania od przepe³nienia bufora oraz od time-out

      // konfiguracja przerqwania USART0 
      AT91F_AIC_ConfigureIt ( AT91C_BASE_AIC, AT91C_ID_US1, USART0_INTERRUPT_LEVEL, AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL, Usart1_handler); 
        
      AT91F_US_EnableRx(AT91C_BASE_US1);
      AT91F_US_EnableTx(AT91C_BASE_US1);

      
      AT91F_US1_CfgPIO();                                                       //konfiguracja pinów
      
      unsigned int xxx= AT91F_US_ReceiveFrame (                                 //Ustawienie bufora odbiorczego
	AT91C_BASE_US1,
	US1_Buffer,
	US_BUFFER_SIZE,
	0,
	0 );

      AT91C_BASE_US1->US_RTOR = TIME_OUT_US;                                    //ustawienie czasu time-out (w bitach)
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
    
    
    // get Usart status register 
    status = USART_pt->US_CSR; 
   
   
    if ( status & AT91C_US_TIMEOUT)       //wystapil timeout lub odebrano dane
      {

        AT91C_BASE_US1->US_CR = AT91C_US_STTTO;
        AT91C_BASE_US1->US_RCR = 0;

        unsigned int xxx= AT91F_US_ReceiveFrame (
	USART_pt,
	US1_Buffer,
	US_BUFFER_SIZE,
	0,
	0 );
        
        SendModbusEnable=1;

       
        if (US1_MasterMODBUS_ENABLE)                                            
        {
          //if //(ModBus_MASTER_Return(US1_Buffer,1))                             //je¿eli Modbus US1 w trybie master
          {
            g_US1_Connect_OK=2;        
            SendModbusEnable=1;
          }        
        }
        else
        { 
          //if (ModBus_CommS(US1_Buffer,1)) g_US1_Connect_OK=2;                 //je¿eli Modbus US1 w trybie slave
        }
   

        status = USART_pt->US_CSR; 

      }
   
}//_______________________ Koniec funkcji Usart0_handler _________________________
 

*/
