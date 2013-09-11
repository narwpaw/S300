//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                 IASE Microcontroller Software Support                    *//                       
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : spi_main.h                               *//
//*  Opis                         : Plik nag��wkowy spi_main.c               *//
//*  Autor                        : Pawe� Narwojsz                           *//
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


#ifndef spi_main_H
#define spi_main_H  


//Konfiguracja -----------------------------------------------------------------
        #define MCK                        48054841                             //Cz�stotliwo�� zegara
        #define SPIBAUD_CSR0               600000                              //Cz�stotliwo�� taktowania SPI dla CS1  do CS3  dla PCSDEC=1
        #define SPIBAUD_CSR1               2000000                              //Cz�stotliwo�� taktowania SPI dla CS4  do CS7  dla PCSDEC=1
        #define SPIBAUD_CSR2               3000000                               //Cz�stotliwo�� taktowania SPI dla CS8  do CS11 dla PCSDEC=1
        #define SPIBAUD_CSR3               4000000                               //Cz�stotliwo�� taktowania SPI dla CS12 do CS15 dla PCSDEC=1
        #define SPI_BUFFER_SIZE            50                                   //Rozmiar bufor�w odbiorczych i nadawczych SPI (ilo�� danych w buforach: SPI_BUFFER_SIZE/2 s��w)
//___________________________Koniec konfiguracji________________________________




//Definicje --------------------------------------------------------------------
        #define Cs_Clock                      0x1
        #define Cs_RF                         0x2
        //#define Cs_Clock                      0x4
        #define Cs_Eth1                       0x4
        #define Cs_Eth2                       0x5

	#define ENABLE_SPI   		      0
	#define DISABLE_SPI  		      1
	#define ENABLE_DISABLE_SPI   	      2
	#define CSR0   			      0
	#define CSR1   			      1
	#define CSR2   			      2
	#define CSR3                          3
        #define SPI_BUFFER_REG_SIZE           SPI_BUFFER_SIZE/4                 //ilo�� s��w w buforach    
//___________________________Koniec definicji___________________________________



// Pliki do��czane -------------------------------------------------------------

//-----------------------Koniec plik�w do��czanych -----------------------------








    //Strukt�ra bufr�w komunikacji SPI
    typedef struct _AT91_SPI_Buff 
        {
          union{
                char           Buffer_Rx1[SPI_BUFFER_SIZE];                 //pierwszy bufor odbiorczy SPI
                unsigned long  Buffer_Rx1Reg[SPI_BUFFER_REG_SIZE];           //pierwszy bufor odbiorczy SPI
                };
          union{
                char           Buffer_Tx1[SPI_BUFFER_SIZE];                 //pierwszy bufor nadawczy SPI
                unsigned long  Buffer_Tx1Reg[SPI_BUFFER_REG_SIZE];           //pierwszy bufor nadawczy SPI
                };
          union{
                char           Buffer_Rx2[SPI_BUFFER_SIZE];             //drugi bufor odbiorczy SPI
                unsigned long  Buffer_Rx2Reg[SPI_BUFFER_REG_SIZE];       //drugi bufor odbiorczy SPI
                };
          union{
                char           Buffer_Tx2[SPI_BUFFER_SIZE];             //drugi bufor nadawczy SPI
                unsigned long  Buffer_Tx2Reg[SPI_BUFFER_REG_SIZE];       //drugi bufor nadawczy SPI
                };   
          char *pBuffer_Rx;                                                     //wska�nik do bufora nadawczego (pierwszego lub drugiego) obecnie realizowanego lub zrealizowanego                                                                                                            
          char *pBuffer_Tx;                                                     //wska�nik do bufora odbiorczego (pierwszego lub drugiego) obecnie realizowanego lub zrealizowanego
          char *BuffToSend;                                                     //wska�nik do bufora nadawczego przeznaczonego do realizacji
          char WordToSendCount;                                                 //licznik bajt�w do wys�ania dla pBuffer_RxNext. Je�eli n>0 dane zostan� wys�ane kiedy tylko b�dzie to mo�liwe
          char Buffer_TxCount;                                                  //licznik bajt�w wysy�anych lub w�a�nie wys�anych. Je�eli n>0 - wysy��nie danych nie zako�czy�o si� lub nie zako�czy�a si� interpretacja odpowiedzi 
        }AT91_SPI_Buff;




// Definicje nag��wkowe funkcji-------------------------------------------------
 

        // Definicje nag��wkowe funkcji o zasiegu lokalnym ---------------------
        void            AT91F_SPI_SWReset (const AT91PS_SPI pSpi);
	void            AT91F_Set_SPI_CSR (const AT91PS_SPI pSpi, unsigned long Csr_Value, unsigned long Csr_Number);
        void            AT91F_SPI_Send_Data (const AT91PS_SPI pSpi, unsigned long Data_To_Send);
	unsigned long   AT91F_SPI_Read_Data (const AT91PS_SPI pSpi);
	void            AT91F_SPI_En_Dis (const AT91PS_SPI pSpi, unsigned char Enable);
	void            AT91F_SPI_Open (void);	
        void            SpiMaster_Init (void);
        void            AT91F_SPI_Interrupt_handler (void); 
        void            SPI_Send_Executive (void);
        void            SPI_Response_Interpretation(char TxCount);

       
        // Definicje nag��wkowe funkcji o zasi�gu globalnym---------------------
        extern void     SPI_SendB_Buff (char *Data_To_Send, unsigned int QuantityByte, char CS);
        
        


void SPI_RFM12_Execute(char *Buffer_Tx, char *Buffer_Rx) ;
unsigned short int SoftSPI_SendWord(short int Val);
void SoftSPI_Init(void);

#endif

