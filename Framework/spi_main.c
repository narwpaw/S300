//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                 IASE Microcontroller Software Support                    *//                       
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : spi_main.c                               *//
//*  Opis                         : obs³oga transmisji SPI                   *//
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



//Konfiguracja -----------------------------------------------------------------
  
//___________________________Koniec konfiguracji________________________________



// Pliki do³¹czane -------------------------------------------------------------
    #include "core/common.h"
    #include "core/device.h" 
    #include "spi_main.h"
    #include <intrinsics.h>
    //#include  "Clock.h"
#include "core/trace.h"
   // #include  "RF.h"
#include "core/board.h"
#include "InterruptPriority.h"
//-----------------------Koniec plików do³¹czanych -----------------------------



//Definicje --------------------------------------------------------------------
    AT91PS_SPI     pSPI = AT91C_BASE_SPI;
//___________________________Koniec definicji___________________________________





//Sta³e ------------------------------------------------------------------------

//____________________________Koniec sta³ych ___________________________________





//Zmienne globalne -------------------------------------------------------------

//___________________________Koniec zmiennych globalnych________________________





//Zmienne lokalne -------------------------------------------------------------
    unsigned char volatile      spi_Rdrf_Flag     = 0;
    unsigned char volatile	spi_Tdre_Flag     = 0;
    unsigned char volatile	spi_Modf_Flag     = 0;
    unsigned char volatile	spi_Ovred_Flag    = 0;
    unsigned char volatile	spi_Nssr_Flag     = 0;
    unsigned long volatile	spi_Txempty_Flag  = 0;
    unsigned char volatile	spi_Spiens_Flag   = 0;
    unsigned long volatile 	spi_Endrx_Flag    = 0;
    unsigned long volatile 	spi_Endtx_Flag    = 0;
    unsigned long volatile      spi_Rxbuff_Flag   = 0;
    unsigned long volatile      spi_Txbuff_Flag   = 0;
    unsigned long volatile	spi_Status        = 0;
    AT91_SPI_Buff               SPI_Buff;
    AT91_SPI_Buff               *pSPI_Buff        = &SPI_Buff;
//___________________________Koniec zmiennych lplkalnych________________________



//Zmienne zewnêtrzne -------------------------------------------------------------
    extern    char Clock_Set_Command[6];

//___________________________Koniec zmiennych zewnêtrznych________________________

  

//-----------------------------------------------------------------------------
//! Nazwa funkcji :    AT91F_SPI_Interrupt_handler
//! Funkcja realizacji przerwania SPI
//-----------------------------------------------------------------------------
void AT91F_SPI_Interrupt_handler (void)
{

  	spi_Status = pSPI -> SPI_SR;


	// The Receive Counter Register has reached 0 since the last write in SPI_RCR or SPI_RNCR.
	if (spi_Status & AT91C_SPI_ENDRX)	
	{
                 spi_Endrx_Flag  = 1;
		pSPI -> SPI_IDR = AT91C_SPI_ENDRX; // Interrupt is disabled to avoid inopportune interrupt 
          
                  unsigned int Save = __get_interrupt_state();   
                  __disable_interrupt(); 
                    SPI_Response_Interpretation((pSPI_Buff->Buffer_TxCount));
                  __set_interrupt_state(Save);
                  __enable_interrupt();   
                 
                  if (SPI_Buff.WordToSendCount>0) 
                  {
                    SPI_Send_Executive();
                  }
                  else
                  {
                    pSPI_Buff->Buffer_TxCount=0;
                  }
                   
                 
                
        }
        
          
        // Both SPI_RCR and SPI_RNCR have a value of 0.
	if (spi_Status & AT91C_SPI_RXBUFF)	
	{
		spi_Rxbuff_Flag  = 1;
		//pSPI -> SPI_IDR = AT91C_SPI_RXBUFF; // Interrupt is disabled to avoid inopportune interrupt  
                
        }	        
               
        
        /*

        // The Transmit Counter Register has reached 0 since the last write in SPI_TCR or SPI_TNCR.
	if (spi_Status & AT91C_SPI_ENDTX)	
	{
		spi_Endtx_Flag  = 1;
		//pSPI -> SPI_IDR = AT91C_SPI_ENDTX; // Interrupt is disabled to avoid inopportune interrupt 
        }	
        
        
 
        
   
	// A data has been recieved and transmitted to the RBR.
	if (spi_Status & AT91C_SPI_RDRF)	
	{ 
		spi_Rdrf_Flag = 1;
		pSPI -> SPI_IDR = AT91C_SPI_RDRF; // Interrupt is disabled to avoid inopportune interrupt 
                

	}

	// The last data written in the TDR has been transmitted to the serializer
	if (spi_Status & AT91C_SPI_TDRE)	
	{
		spi_Tdre_Flag  = 1;
		pSPI -> SPI_IDR = AT91C_SPI_TDRE; // Interrupt is disabled to avoid inopportune interrupt  
	}

	// A mode fault has occured (NCS0 was tied low).
	if (spi_Status & AT91C_SPI_MODF)	
	{
		spi_Modf_Flag  = 1;
		pSPI -> SPI_IDR = AT91C_SPI_MODF; // Interrupt is disabled to avoid inopportune interrupt      
	}

	// An overrun error has occured.
	if (spi_Status & AT91C_SPI_OVRES)	
	{
		spi_Ovred_Flag  = 1;
		pSPI -> SPI_IDR = AT91C_SPI_OVRES; // Interrupt is disabled to avoid inopportune interrupt 
	}
	


	// Both SPI_TCR and SPI_TNCR have a value of 0.
	if (spi_Status & AT91C_SPI_TXBUFE)	
	{
		spi_Txbuff_Flag  = 1;
		pSPI -> SPI_IDR = AT91C_SPI_TXBUFE; // Interrupt is disabled to avoid inopportune interrupt         
	}

	// NSS has risen
	if (spi_Status & AT91C_SPI_NSSR)	
	{
		spi_Nssr_Flag  = 1;
		pSPI -> SPI_IDR = AT91C_SPI_NSSR; // Interrupt is disabled to avoid inopportune interrupt
        }

            
	// TDR and shift registers are empty
	if (spi_Status & AT91C_SPI_TXEMPTY)	
	{
		spi_Txempty_Flag  = 1;
		pSPI -> SPI_IDR = AT91C_SPI_TXEMPTY; // Interrupt is disabled to avoid inopportune interrupt   
	}
        
        */

}//_____________________________________________________________________________





//-----------------------------------------------------------------------------
//! Nazwa funkcji :    SpiMaster_Init
//! Funkcja inicjalizacji komunikacji SPI w trybie master
//-----------------------------------------------------------------------------
void SpiMaster_Init (void)
{
	unsigned char error;
	AT91PS_SPI     pSPI = AT91C_BASE_SPI;
	
	error = 0;

	AT91F_SPI_Open ();	

        pSPI -> SPI_IER =  	(AT91C_SPI_RDRF + AT91C_SPI_TDRE + AT91C_SPI_MODF + AT91C_SPI_OVRES+ 
					 AT91C_SPI_NSSR + AT91C_SPI_TXEMPTY + AT91C_SPI_SPIENS +AT91C_SPI_TXBUFE);

         __enable_interrupt();
         
         
         
         
                  //SPI CONTROL REGISTER
    //=========================================================================================
    //| -------- | -------- | -------- | -------- | -------- | -------- | -------- | LASTXFER |
    //=========================================================================================
    
    //=========================================================================================
    //| -------- | -------- | -------- | -------- | -------- | -------- | -------- | -------- |
    //========================================================================================= 
    
    //=========================================================================================
    //| -------- | -------- | -------- | -------- | -------- | -------- | -------- | -------- |
    //=========================================================================================
    
    //=========================================================================================
    //|  SWRST   | -------- | -------- | -------- | -------- | -------- |  SPIDIS  |   SPEN   |
    //=========================================================================================
    //pSPI->SPI_CR = (AT91C_SPI_SPIEN | AT91C_SPI_SWRST);	
    pSPI->SPI_CR = AT91C_SPI_SPIEN;
    


    
        //SPI MODE REGISTER
    //=========================================================================================
    //|                                         DLYBCS                                        |
    //=========================================================================================
    
    //=========================================================================================
    //| -------- | -------- | -------- | -------- |                    PCS                    |
    //========================================================================================= 
    
    //=========================================================================================
    //| -------- | -------- | -------- | -------- | -------- | -------- | -------- | -------- |
    //=========================================================================================
    
    //=========================================================================================
    //|   LLB    | -------- | -------- |  MODFDIS | -------- |  PCSDEC  |    PS    |   MSTR   |
    //=========================================================================================
    //pSPI->SPI_MR = (AT91C_SPI_MSTR | AT91C_SPI_MODFDIS | AT91C_SPI_PCSDEC |AT91C_SPI_PS_VARIABLE | ((0x0)<<16));
pSPI->SPI_MR = (AT91C_SPI_MSTR | AT91C_SPI_MODFDIS  |AT91C_SPI_PS_VARIABLE | ((0x0)<<16));




    
    //SPI CHIP SELECT REGISTER 
    //=========================================================================================
    //|                                         DLYBCT                                        |
    //=========================================================================================
    
    //=========================================================================================
    //|                                          DLYBS                                        |
    //========================================================================================= 
    
    //=========================================================================================
    //|                                          SCBR                                         |
    //=========================================================================================
    
    //=========================================================================================
    //|                 BITS                      |   CSAAT  | -------- |   NCPHA  |   CPOL   |
    //=========================================================================================
   
    
    char CS_BAUD = MCK / SPIBAUD_CSR0; 
    AT91F_SPI_CfgCs (AT91C_BASE_SPI, 0, AT91C_SPI_CPOL |  AT91C_SPI_BITS_16 | (AT91C_SPI_SCBR & (CS_BAUD << 8)) ); /*ustawienia dla RTC  :*/
    CS_BAUD = MCK / SPIBAUD_CSR1; 
    AT91F_SPI_CfgCs (AT91C_BASE_SPI, 1, AT91C_SPI_NCPHA |  AT91C_SPI_BITS_16 | AT91C_SPI_DLYBCT |(AT91C_SPI_SCBR & (CS_BAUD << 8)) ); /*ustawienia dla RF  :*/
    CS_BAUD = MCK / SPIBAUD_CSR2; 
    AT91F_SPI_CfgCs (AT91C_BASE_SPI, 2, AT91C_SPI_NCPHA |  AT91C_SPI_BITS_16 | AT91C_SPI_DLYBCT |(AT91C_SPI_SCBR & (CS_BAUD << 8)) ); 
    CS_BAUD = MCK / SPIBAUD_CSR3; 
      AT91F_SPI_CfgCs (AT91C_BASE_SPI, 3, AT91C_SPI_CPOL |  AT91C_SPI_BITS_16 | (AT91C_SPI_SCBR & (CS_BAUD << 8)) );
                

    


          // konfiguracja PDC
        AT91F_PDC_Open(AT91C_BASE_PDC_SPI);  //wyczyszczenie PDC

        AT91F_PDC_DisableTx (AT91C_BASE_PDC_SPI); 
        AT91F_PDC_DisableRx (AT91C_BASE_PDC_SPI); 
     

        unsigned long xxR= AT91F_SPI_ReceiveFrame (AT91C_BASE_SPI, pSPI_Buff->Buffer_Rx1, 0,pSPI_Buff->Buffer_Rx2,0); //Ustawia bufor odbiorczy bufor dodatkowy
      
 
        AT91F_PDC_EnableTx (AT91C_BASE_PDC_SPI); 
        AT91F_PDC_EnableRx (AT91C_BASE_PDC_SPI); 
    
        
        pSPI -> SPI_IDR = 0xFFFFFFFF;  
        
        
        pSPI_Buff->BuffToSend = pSPI_Buff->Buffer_Tx1;   

 
        AT91C_BASE_SPI -> SPI_IER = AT91C_SPI_ENDRX | AT91C_SPI_RXBUFF;
        
}//_____________________________________________________________________________





//-----------------------------------------------------------------------------
//!  Nazwa funkcji :    SPI_Send_Buff
//!  Funkcja dodaje zadanie wys³ania poprzez SPI bufora danych do kolejki
//-----------------------------------------------------------------------------
//! 
//-----------------------------------------------------------------------------
void SPI_SendB_Buff (char *Data_To_Send, unsigned int QuantityByte, char CS)
{
    
   // if ((QuantityByte==0) || (!CS)) return;
    if ((QuantityByte==0)) return;
    
    short int QuantityWord   = QuantityByte/2;
    short int RegToSendCount = SPI_Buff.WordToSendCount;
    //short int QuantityREG    = QuantityWord/2;
      
    if (QuantityWord>SPI_BUFFER_REG_SIZE) return;
    

    
    while(QuantityWord+ (SPI_Buff.WordToSendCount)>SPI_BUFFER_REG_SIZE);
    
 
    for (short int i=0; i<QuantityWord; i++)
    {

      pSPI_Buff->BuffToSend[((pSPI_Buff->WordToSendCount+i)*4)+1]=Data_To_Send[i*2];
      pSPI_Buff->BuffToSend[((pSPI_Buff->WordToSendCount+i)*4)]=Data_To_Send[(i*2)+1];
      
      pSPI_Buff->BuffToSend[((pSPI_Buff->WordToSendCount+i)*4)+2]=(~CS);
      //pSPI_Buff->WordToSendCount=pSPI_Buff->WordToSendCount+i+1;  
    }
    pSPI_Buff->WordToSendCount=pSPI_Buff->WordToSendCount+QuantityWord;
    

    
    if (pSPI_Buff->Buffer_TxCount==0)
    {
    SPI_Send_Executive();
    }
}//_____________________________________________________________________________




//-----------------------------------------------------------------------------
//!  Nazwa funkcji :    SPI_Send_Executive pSPI_Buff->Buffer_Rx
//!  Funkcja wysy³a dane z bufora oczekuj¹cego a nastepnie zamienia bufory oczekuj¹cy z realizowanym
//-----------------------------------------------------------------------------
//! 
//-----------------------------------------------------------------------------
void SPI_Send_Executive ()
{
      char QuantityWord;
  
      AT91C_BASE_SPI -> SPI_IER = AT91C_SPI_ENDRX;

        spi_Endrx_Flag=0;
        spi_Endtx_Flag=0;  
        spi_Rxbuff_Flag=0;  
      
                   
        
      //ustawienie buforów nadawczych i odbiorczych naprzemiennie Buffer_Rx1->Buffer_Rx2, Buffer_Rx2->Buffer_Rx1, Buffer_Tx1->Buffer_Tx2
      if (pSPI_Buff->pBuffer_Tx == pSPI_Buff->Buffer_Tx1)
      {
         pSPI_Buff->pBuffer_Tx = pSPI_Buff->Buffer_Tx2;
         pSPI_Buff->pBuffer_Rx = pSPI_Buff->Buffer_Rx2;
         pSPI_Buff->BuffToSend = pSPI_Buff->Buffer_Tx1; 
      }
      else 
      {
         pSPI_Buff->pBuffer_Tx  = pSPI_Buff->Buffer_Tx1; 
         pSPI_Buff->pBuffer_Rx  = pSPI_Buff->Buffer_Rx1;
         pSPI_Buff->BuffToSend = pSPI_Buff->Buffer_Tx2; 
      }
     
      QuantityWord=pSPI_Buff->WordToSendCount;
      pSPI_Buff->WordToSendCount=0;
      pSPI_Buff->Buffer_TxCount=QuantityWord;
      unsigned long xxR= AT91F_SPI_ReceiveFrame (AT91C_BASE_SPI,pSPI_Buff->pBuffer_Rx, QuantityWord,0,0); 
      unsigned long xxT = AT91F_SPI_SendFrame (AT91C_BASE_SPI,pSPI_Buff->pBuffer_Tx,QuantityWord,0,0 );
      
}//_____________________________________________________________________________





void SPI_SendWord(short int Val, char CS)
{
  char tabSPI[3];
  tabSPI[0]=(char)((Val>>8) & 0xFF);
  tabSPI[1]=(char)(Val & 0xFF);
  tabSPI[2]=0;
  
  SPI_SendB_Buff(tabSPI,2,CS);
}



    

//-----------------------------------------------------------------------------
//! Nazwa funkcji :    SPI_Response_Interpretation
//! Funkcja interpretacji danych otrzymanych z lini SPI 
//! paramentr funkcji to iloœæ odebranych danych
//-----------------------------------------------------------------------------


char *zm_tmp1;
char *zm_tmp2;
char zm_tmp3;


char Real_Cs_RF=0xFF-Cs_RF;

void SPI_Response_Interpretation(char TxCount)
{ 
   char *TMP_pBuffer_Tx = pSPI_Buff->pBuffer_Tx;
   char *TMP_pBuffer_Rx = pSPI_Buff->pBuffer_Rx;
   char move;  
   

   
   if (TMP_pBuffer_Tx[2]==Real_Cs_RF)
   {
     SPI_RFM12_Execute(TMP_pBuffer_Tx, TMP_pBuffer_Rx);  
     TMP_pBuffer_Tx+=4;
     TMP_pBuffer_Rx+=4;
   }

   if (TMP_pBuffer_Tx[2]==0xFF-Cs_Clock)
   {
     move=SPI_Clock_Execute(TMP_pBuffer_Tx+1, TMP_pBuffer_Rx);  
     TMP_pBuffer_Tx+=move;
     TMP_pBuffer_Rx+=move;
   }
  
   /*
     char wsk=0;

    zm_tmp1=&TxCount;
    zm_tmp2=&wsk;
    
    TxCount *=4;

    wsk=0;
   
    while (TxCount>wsk)  //dopuki wszystkie odebrane dane nie zosta³y zinterpretowane
    {
      
      switch (0xF-((pSPI_Buff->pBuffer_Tx[wsk+2])&0xF))
      {
        
/*
        case Cs_Clock:  
           wsk += SPI_Clock_Execute(pSPI_Buff->pBuffer_Tx+wsk+1, pSPI_Buff->pBuffer_Rx+wsk); 
           //wsk *=2; 
            wsk=TxCount;
        break;

        case Cs_RF:  
          wsk=0;
           SPI_RFM12_Execute(pSPI_Buff->pBuffer_Tx+wsk, pSPI_Buff->pBuffer_Rx+wsk);       
           wsk +=4;
           if (0xF-((pSPI_Buff->pBuffer_Tx[wsk+2])&0xF)==Cs_RF) wsk +=4;
           if (0xF-((pSPI_Buff->pBuffer_Tx[wsk+2])&0xF)==Cs_RF) wsk +=4;
           if (0xF-((pSPI_Buff->pBuffer_Tx[wsk+2])&0xF)==Cs_RF) wsk +=4;

        break;
        
        default:
          wsk=TxCount;
          wsk=0;
           SPI_RFM12_Execute(pSPI_Buff->pBuffer_Tx+wsk, pSPI_Buff->pBuffer_Rx+wsk); 
           
        break;  
   
      }
 
      //zm_tmp3=(0xF-((pSPI_Buff->pBuffer_Tx[wsk+2])&0xF));
      wsk=TxCount;
    }
  
wsk =0;
     SPI_RFM12_Execute(pSPI_Buff->pBuffer_Tx+wsk, pSPI_Buff->pBuffer_Rx+wsk);  

*/
  
  
  
}//_____________________________________________________________________________














//FUNKCJE BIBLIOTECZNE *********************************************************


//*--------------------------------------------------------------------------------------
//* Function Name       	: AT91F_SPI_SWReset
//* Object              	: SPI Software reset and check reg after reset
//* Input Parameters    : none.
//* Output Parameters  : none.
//*--------------------------------------------------------------------------------------
void AT91F_SPI_SWReset (const AT91PS_SPI pSpi)
{
	pSpi -> SPI_CR  = AT91C_SPI_SWRST; // SW reset
}



//*--------------------------------------------------------------------------------------
//* Function Name       	: AT91F_SPI_En_Dis
//* Object              	: SPI Enable  Disable
//* Input Parameters    : Enable.
//* Output Parameters  : error.
//*--------------------------------------------------------------------------------------
void AT91F_SPI_En_Dis (const AT91PS_SPI pSpi, unsigned char Enable)
{
	volatile unsigned int error;

	error = 0;

	switch (Enable)
	{ 
		case ENABLE_SPI:
		pSpi -> SPI_CR  = AT91C_SPI_SPIEN; // SPI is enable
		Delay (0xFF);
		break;

		case DISABLE_SPI:
		pSpi -> SPI_CR  = AT91C_SPI_SPIDIS; // SPI is Disable
		Delay (0xFF);
		break;

		case ENABLE_DISABLE_SPI:
		pSpi -> SPI_CR  = (AT91C_SPI_SPIDIS | AT91C_SPI_SPIEN); // Enable and disable are set @ the same time => SPI shoulb be disable
		Delay (0xFF);
		break;
	}
}


//*--------------------------------------------------------------------------------------
//* Function Name       : AT91F_Set_SPI_CSR
//* Object              : Allows programming SPI_CSR registers
//* Input Parameters    : Csr_Value.
//* Input Parameters    : Csr_Number.
//* Output Parameters   : none.
//*--------------------------------------------------------------------------------------
void AT91F_Set_SPI_CSR (const AT91PS_SPI pSpi, unsigned long Csr_Value, unsigned long Csr_Number)
{
	switch (Csr_Number)
	{
		case CSR0:
		pSpi -> SPI_CSR[CSR0]  = Csr_Value; // CSR0 <- Csr_Value
		break;

		case CSR1:
		pSpi -> SPI_CSR[CSR1]  = Csr_Value; // CSR1 <- Csr_Value
		break;
		
		case CSR2:
		pSpi -> SPI_CSR[CSR2]  = Csr_Value; // CSR2 <- Csr_Value
		break;

		case CSR3:
		pSpi -> SPI_CSR[CSR3]  = Csr_Value; // CSR3 <- Csr_Value
		break;

		default:
		break; 
	}
}


//*--------------------------------------------------------------------------------------
//* Function Name       	: AT91F_SPI_Read_Data
//* Object              	: Read SPI_RDR
//* Input Parameters    : none.
//* Output Parameters  : Data_Recieved.
//*--------------------------------------------------------------------------------------
unsigned long AT91F_SPI_Read_Data (const AT91PS_SPI pSpi)
{
	unsigned long Data_Recieved;

        AT91C_BASE_PIOA->PIO_SODR = AT91C_PIO_PA3 ;AT91C_BASE_PIOA->PIO_CODR = AT91C_PIO_PA3; 
	/*spi_Rdrf_Flag = 0;
	pSpi -> SPI_IER = AT91C_SPI_RDRF; // Disabled in the interrupt handler

	while (spi_Rdrf_Flag == 0);
        */
	Data_Recieved = (pSpi -> SPI_RDR & 0x000FFFFF); // Read RDR and mask non used bits

	pSpi -> SPI_IER = AT91C_SPI_RDRF; // RDF re enabled (see handler)

        AT91C_BASE_PIOA->PIO_SODR = AT91C_PIO_PA3 ;AT91C_BASE_PIOA->PIO_CODR = AT91C_PIO_PA3; 
	return Data_Recieved; // Return RDR (Read Data + PCS)
}


//*--------------------------------------------------------------------------------------
//* Function Name       : AT91F_SPI_Send_Data
//* Object              : Send a data
//* Input Parameters    : Data_To_Send.
//* Output Parameters   : none.
//*--------------------------------------------------------------------------------------
void AT91F_SPI_Send_Data (const AT91PS_SPI pSpi, unsigned long Data_To_Send)
{
	// IMPORTANT: Data_To_Send = TD (in fixed mode)
	// IMPORTANT: Data_To_Send = TD + PCS + LASTXFER (in variable mode)
	spi_Tdre_Flag = 0;

	Data_To_Send &= 0x010FFFFF; // mask non used bits
	pSpi -> SPI_TDR = Data_To_Send;
}


//*--------------------------------------------------------------------------------------
//* Function Name       	: AT91F_SPI_Open
//* Object              	: Open the SPI0
//* Input Parameters    : none.
//* Output Parameters  : error.
//*--------------------------------------------------------------------------------------
void  AT91F_SPI_Open (void)
{
// MISO, MOSI, CLK, NPCS[3:0] are configured
// SPI (IP) clock is activated
// Interrupt Handler is configured
// SPI is resetted

	AT91PS_SPI     pSPI = AT91C_BASE_SPI;

	//  Cfp SPI PIO, PMC, ...
		// Configure SPI PIOs
  		AT91F_SPI_CfgPIO ();

	  	// Configure PMC by enabling SPI clock
  		AT91F_SPI_CfgPMC ();

		// Configure SPI interrupt
		AT91F_AIC_ConfigureIt ( AT91C_BASE_AIC, AT91C_ID_SPI, SPI_PRIOR, AT91C_AIC_SRCTYPE_INT_POSITIVE_EDGE, AT91F_SPI_Interrupt_handler);
	  	AT91F_AIC_EnableIt (AT91C_BASE_AIC, AT91C_ID_SPI);	

		// Reset the SPI (note it is possible to reset the SPI even if SPI is disabled)
		AT91F_SPI_SWReset (AT91C_BASE_SPI);

		spi_Txempty_Flag = 0;

		pSPI -> SPI_IER =  	(AT91C_SPI_RDRF + AT91C_SPI_TDRE + AT91C_SPI_MODF + AT91C_SPI_OVRES+ 
							 AT91C_SPI_NSSR + AT91C_SPI_TXEMPTY + AT91C_SPI_SPIENS + AT91C_SPI_TXBUFE);

		 AT91F_SPI_En_Dis (AT91C_BASE_SPI, ENABLE_SPI);

		spi_Txempty_Flag = 0;
}






