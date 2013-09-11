#include "RF.h" 
#include <intrinsics.h>
     #include "Framework.h"
     #include "core/device.h"   
     #include "board.h"   
#include "spi_main.h"
#include "core/trace.h"
#include "Application_prog.h" 
#include "InterruptPriority.h"

_RF_Rx RF_Rx;
_RF_Tx RF_Tx;

char SPI_RFM12_Read=0;

const char Preambu[]={0xAA, 0xAA, 0xAA};  //Preambu³a
const char SynKay[]= {0x2D, 0xD4};        //s³owo synchronizacyjne




extern _MainStage  MainStage;
extern _TransQuality TransQuality;




//Zmienne globalne -------------------------------------------------------------
  _RF_Status   RF_Status;
  _RF_TaskTab  RF_TaskTab;
  char         RF_CurrentTask=0; 
  char g_RF_TimeOutCounter =0;  //bierz¹cy czas odliczania do timeout`u
//___________________________Koniec zmiennych globalnych________________________


  
//tymczasowo
int wyslanych=0;
int poprawnych=0;
int wys[2];
int odb[2];
char IndWys=0; 
char IndWysNeg=1; 
int blad=0;
int licznik_cykli=0;
//tymczasowo koniec-----------



  
  
//Tworzenie pse³do-klasy RF_Status --------------------------------------------
    
  //Uruchomienie trybu Nadajnika
  void _RF_EnableTX(void)
  {
    RF_Status.EnRx=0;
    RF_Status.EnTx=0;
    RF_Status.Transmit=0;
    RF_Tx.BytesToSend=0;
    RF_Status.EnRxWait=0;

    SPI_SendWord(0x8239,Cs_RF);
  }

  //Uruchomienie trybu odbiornika
  void _RF_EnableRX(void)
  {

    //Je¿eli przy prubie prze³¹czenia wysy³ane s¹ dane to poczekaj na zakoñczenie wysy³ki
    if ((RF_Tx.BytesToSend>0)) 
    {
      RF_Status.EnRxWait=1; 
    }else{


      for (int aaa=0; aaa<0x5FF; aaa++) asm( "nop" );    //uœpienie ~200us - wymagany czas prze³¹czenia z nadajnika na odbiornik

      
      RF_Status.EnTx=0;
      RF_Status.EnRx=0;
      RF_Tx.BytesToSend=0;
      SPI_SendWord(0x8299,Cs_RF);
    }
  }

  //Konstruktor RF_Status
  void _pRF_Status(void)
  {
    RF_Status.EnableTX = _RF_EnableTX;
    RF_Status.EnableRX = _RF_EnableRX; 
  }

//________koniec tworzenia pse³do-klasy RF_Status _______________________________








  
  


void RF_Int(void)
{
  
      unsigned int status = AT91F_PIO_GetInterruptStatus(PIO_RfIntBUS);


if (status & RfIntBUS)
{
  
  if ((( AT91C_BASE_PIOA->PIO_PDSR) & RfIntBUS)==0) 
  {
    //je¿eli uka³d dzia³a jako odbiornik
    

    if (RF_Status.EnRx) 
    {
      if (RF_Status.Init==0) 
      {
        
        SPI_SendWord(0x0000,Cs_RF);
        
      }
    }
    //je¿eli uka³d dzia³a jako nadajnik
    else if ((RF_Status.EnTx) && (RF_Status.Transmit==1))  
    {
      if (RF_Tx.BytesToSend>0) 
      {
        RF_transmit();
      }
      else if (RF_Status.EnRxWait)
      {
        RF_Status.Transmit=0;
        RF_Status.EnRxWait=0;
        RF_Status.EnableRX();
      }
      else
      {
         RF_Status.Transmit=0; 
      }
    }
  }

}

}//_____________________________________________________________________________
//







void ConfigureRFBus(void)
{
#if !defined(IR_RF_DEF)
    AT91F_PMC_EnablePeriphClock(AT91C_BASE_PMC, 1 << ID_RfIntBUS);
    AT91F_PIO_CfgPullup(IR_PIO_BUS, ~RfIntBUS);
    AT91F_PIO_CfgInput(IR_PIO_BUS, RfIntBUS);
#endif
}




char IntCA81Flag=0;

void RF_Init(void)
{ 
  #ifndef RF_Int_Init_DEF
  
      //ustawienia pocz¹tkowe uk³adu
      RF_Status.RF_Status = _pRF_Status;
      RF_Status.RF_Status();  //konstruktor pse³do-klasy RF_Status
        
        
    //Konfiguracja wstêpna
        #ifdef RF_Master 
              RF_Status.EnTx=1; 
              RF_Status.EnRx=0; 
        #else   
              RF_Status.EnRx=1;
              RF_Status.EnTx=0; 
        #endif
      
   
              
        RF_Status.Init=1;
        IntCA81Flag=0;
        SPI_SendWord(0x80E7,Cs_RF); //EL,EF,868band,12.0pF   - pierwszy rozkaz konfiguracji

  
  
        
   #endif
}//_____________________________________________________________________________













char CountTmp=16;

short int cztmp[4];

char CountVal=16;
char SPI_RFM12_Execute(char *Buffer_Tx, char *Buffer_Rx)                                       
{
    int Order= Buffer_Tx[0] | Buffer_Tx[1]<<8;
    
    //Je¿eli uk³ad RF12 w trakcie inicjalizacji pierwotnej---------------------
    
    cztmp[0]=RF_SetFreqReg(0);

    
    
    if (RF_Status.Init)
    {
      RF_Status.Ch=0;
  
      if (Order==0x80E7) 
                       { if (RF_Status.EnRx)  RF_Status.EnableRX(); else RF_Status.EnableTX(); } //Rozkaz: 0x80E7 - EL,EF,868band,12.0pF 
      if ((Order==0x8299) || (Order==0x8239) )
                       { SPI_SendWord(0xA000 | RF_SetFreqReg(RF_Status.Ch),Cs_RF); }       //Rozkaz: 0xA6xx Czêstotliwoœæ bazowa (Kana³ 0)  - oryginalnie by³o 0xA640 - 868,0Mhz
      if (Order==(0xA000 | RF_SetFreqReg(RF_Status.Ch)))  
                       { SPI_SendWord(0xC607,Cs_RF); }                          //Rozkaz: 0xC607 Szybkoœæ transmisji 43kb/s         - oryginalnie by³o 0xC647 - 4,8kb/s
      if (Order==0xC607)  
                       { SPI_SendWord(0x94A0,Cs_RF); }                          //Rozkaz: 0x94A0 VDI,FAST,134kHz,0dBm,-103dBm 
      if (Order==0x94A0)  
                       { SPI_SendWord(0xC2AC,Cs_RF); }                          //Rozkaz: 0xC2AC AL,!ml,DIG,DQD4 
      if (Order==0xC2AC)  
                       { SPI_SendWord(0xCA81,Cs_RF); }                          //Rozkaz: 0xCA81 FIFO8,SYNC,!ff,DR 
      if ((Order==0xCA81) && (IntCA81Flag==0))  
                       { SPI_SendWord(0xCED4,Cs_RF); IntCA81Flag=1; Order=0xFFFF;}   //Rozkaz: 0xCED4 ustawienie s³owa synchronizacyjnego na 0x2DD4 
      if (Order==0xCED4)  
                       { SPI_SendWord(0xC493,Cs_RF); }                          //Rozkaz: 0xC493 @PWR, AFC:-80+75Khz ,!st,!fi,OE,EN       - oryginalnie 0xC483 @PWR,AFC - NO RSTRIC,!st,!fi,OE,EN 
      if (Order==0xC493)  
                       { SPI_SendWord(0x9850,Cs_RF); }                          //Rozkaz: 0x9850 !mp,dewiacja +-90kHz ,MAX OUT   
      if (Order==0x9850)  
                       { SPI_SendWord(0xCC17,Cs_RF); }                          //Rozkaz: 0xCC17 OB1COB0, LPX,IddyCDDITC
      if (Order==0xCC17)  
                       { SPI_SendWord(0xE000,Cs_RF); }                          //Rozkaz: 0xE000
      if (Order==0xE000)  
                       { SPI_SendWord(0xC800,Cs_RF); }                          //Rozkaz: 0xC800
      if (Order==0xC800)  
                       { SPI_SendWord(0xC040,Cs_RF); }                          //Rozkaz: 0xC040 1.66MHz,2.2V 
      if (Order==0xC040)  
                       { SPI_SendWord(0xCA81,Cs_RF); }                          //Rozkaz: 0xCA81 Start fifo RX
      if ((Order==0xCA81) && (IntCA81Flag==1))  
                       { SPI_SendWord(0xCA83,Cs_RF); }                          //Rozkaz: 0xCA83 Restart fifo RX
      if (Order==0xCA83)  
                       { 
      


        
             
      
        RF_Status.EnRx=1;
        RF_Status.Init=0; 

   //ustawienie przerwania 
        ConfigureRFBus();

       // Configure and enable the Vbus detection interrupt
        AT91F_AIC_ConfigureIt(AT91C_BASE_AIC,
                          ID_RfIntBUS,
                          RF_INT_PRIOR, //AT91C_AIC_PRIOR_LOWEST, AT91C_AIC_PRIOR_HIGHEST
                          3,//AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL,
                          RF_Int);

        AT91F_PIO_InterruptEnable(PIO_RfIntBUS, RfIntBUS);
        AT91F_AIC_EnableIt(AT91C_BASE_AIC, ID_RfIntBUS);

     
        
        
      }
     
      
    }//Aktywny RX ---------------------------------------------------------------
    else if ((RF_Status.EnRx) && (RF_Status.Init==0))
    {
    
      if  ((Order==0x0000) && (Buffer_Rx[1]&0x80))  { SPI_SendWord(0xB000,Cs_RF); }
      if  (Order==0xCA81)   { SPI_SendWord(0xCA83,Cs_RF); }
      if  (Order==0xCA83)   { SPI_SendWord(0x0000,Cs_RF); }
      if  (Order==0xB000)  { RF_ReciveByte(Buffer_Rx[0]); }
     
      

    
    
    
    }//Aktywny TX ---------------------------------------------------------------
    else if (RF_Status.EnTx)
    {
      
      /*
         if ((RF_Status.Transmit==0) && (Order==0x0000))
        {
          RF_Status.Transmit=1;

            RF_transmit();
         
          
        }
      */
      
      if ((RF_Status.Transmit==0) && (Order==0x0000))
        {
          //TRACE_INFO("Tx %X \n\r ", Buffer_Rx[1]); 

          if (Buffer_Rx[1]&0x20)
          {

            SPI_SendWord(0x0000,Cs_RF);
          }
          else
          {
          RF_Status.Transmit=1;

            RF_transmit();
          }
          
        }
      
    }
    //W³¹czenie Odbiornika
    else if ((RF_Status.EnRx==0) && (RF_Status.EnTx==0) && (Order==0x8299))
    {
       RF_Status.EnRx=1;
       SPI_SendWord(0x0000,Cs_RF);
    }
    //W³¹czenie Nadajnika
    else if ((RF_Status.EnRx==0) && (RF_Status.EnTx==0) && (Order==0x8239))
    {
      for (int aaa=0; aaa<0x5FF; aaa++) asm( "nop" );    //uœpienie ~200us - wymagany czas prze³¹czenia z odbiornika na nadajnik
      RF_Status.EnTx=1;
      
      if (RF_Tx.BytesToSend>0) 
      {
          RF_transmit();
      }
    }
    
  
  
}  _RF_Command  *pRF_Command;


//zminne tymczasowe
short int SymulacjaRejWej=0xAB;
short int SymulacjaRejWyj;
extern int licznik_cykli;
extern int poprawnych;
extern int odb[2];
extern char IndWys; 
short int abcd=0; 

//-----------------------

//-----------------------------------------------------------------------------
//!  Nazwa funkcji :    RF_ReciveByte
//!  Funkcja interpretacji otrzymanego bajtu
//-----------------------------------------------------------------------------
void RF_ReciveByte(char Res)
{

          for (char i=0; i<RF_BuffSizeRX-1; i++) {RF_Rx.Buff[i]=RF_Rx.Buff[i+1];}
          RF_Rx.Buff[RF_BuffSizeRX-1]=Res;
          
      
          RF_Rx.ReciveBytes++;
          

          
          
          //sprawdzenie koñca
          if (

          (RF_Rx.Buff[RF_BuffSizeRX-3]==Preambu[0]) &&
            (RF_Rx.Buff[RF_BuffSizeRX-2]==Preambu[1]) &&
          (RF_Rx.Buff[RF_BuffSizeRX-1]==Preambu[2]))
          {
            
          RF_Rx.ReciveBytes -=3; //zmiejszenie iloœci odebranych danych o preambu³e    
          
          
          
          //je¿eli danych wiêcej ni¿ max iloœæ bajtów w rozkazie to próba dopasowania  
          if (RF_Rx.ReciveBytes>8)
          {
             pRF_Command=(_pRF_Command)&(RF_Rx.Buff[6+3]); //rzutowanie na strukturê rozkazu
             if (pRF_Command->B6.CRC == crc16(pRF_Command->Streem, 4)) RF_Rx.ReciveBytes=6;
             pRF_Command=(_pRF_Command)&(RF_Rx.Buff[8+3]); //rzutowanie na strukturê rozkazu
             if (pRF_Command->B6.CRC == crc16(pRF_Command->Streem, 6)) RF_Rx.ReciveBytes=8;
          }
            
          
 
          pRF_Command=(_pRF_Command)&(RF_Rx.Buff[RF_BuffSizeRX-(RF_Rx.ReciveBytes+3)]); //rzutowanie na strukturê rozkazu
          
         abcd=crc16(pRF_Command->Streem, RF_Rx.ReciveBytes-2); 
          
          
          
          if (
              ((RF_Rx.ReciveBytes==6) && (pRF_Command->B6.CRC == crc16(pRF_Command->Streem, RF_Rx.ReciveBytes-2))) || 
              ((RF_Rx.ReciveBytes==8) && (pRF_Command->B8.CRC == crc16(pRF_Command->Streem, RF_Rx.ReciveBytes-2)))
             )
          {  
            
          
          g_RF_TimeOutCounter=0;  //Restart timeoutu  
            
          ///Sprawdzenie zgodnoœci ID  
          //if (pRF_Command->ID == RF_ID)
          //{
            RF_ExecuteOrder(pRF_Command);   //realizacja rozkazu
          //}  

          
          
          //Testowe dla mastera
         // if (pRF_Command->ID == RF_ID_Master)
         // {

                   poprawnych++;
                   odb[IndWys]++;
         // }            
          
            
          RF_Rx.ReciveBytes=0;

          
          
           
           
         
           // TRACE_INFO("\n\r"); 
           // for (char i=0; i<20; i++) TRACE_INFO("%X ", RF_Rx.Buff[i]); 
            
            /*
            if ((RF_Rx.Buff[RF_BuffSizeRX-9]==0x45) )
             {  
             }        
            else if ((RF_Rx.Buff[RF_BuffSizeRX-9]==0x35) )
             {  
             } 
            */
            
            
            
           
            
          }else
          {
            RF_Rx.ReciveBytes=0;    
          }
          
          if (!RF_Tx.BytesToSend) SPI_SendWord(0xCA81,Cs_RF); 

          }
          else{
             if ((( AT91C_BASE_PIOA->PIO_PDSR) & RfIntBUS)==0) SPI_SendWord(0x0000,Cs_RF); 
            }
  

}



//-----------------------------------------------------------------------------
//!  Nazwa funkcji :    RF_ExecuteOrder
//!  Funkcja interpretuje i realizuje otrzymany rozkaz
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void RF_ExecuteOrder(_RF_Command *Command)
{
  TRACE_INFO("Id: %X,  Ord: %X,  Reg: %X,  Val: %X \n\r", Command->ID, Command->Order, Command->Auxi, Command->B8.Val); 
              licznik_cykli=0;  
               
              
   _RF_Command  CommandToSend;
   
   //czêœæ wsplna wszystkich odpowiedzi
   CommandToSend.ID     = RF_ID_Master;
   CommandToSend.Auxi   = Command->Auxi;
   
       
    char ByteToSend=0;          
    
    switch (Command->Order)
    {  
       //Rozkazy dla Slave ------------------------------------------------------------
      
            case R_RegOrd :  //Odczyt pojedyñczego rejestru - rozkaz
                     //wykonanie ------
                     CommandToSend.B8.Val  =  SymulacjaRejWej;                     // przepisz dane z rejestru nr: Command.Auxi  (tymczasowo  SymulacjaRej)
     
                     //odpowiedŸ ------
                     CommandToSend.Order  = R_RegRes;
                     CommandToSend.B8.CRC = crc16(CommandToSend.Streem,6);    
                     ByteToSend =8;
                 break;
                 
            case W_RegOrd :  //Zapis pojedyñczego rejestru  -rozkaz
                     //wykonanie ------
                     SymulacjaRejWyj==Command->B8.Val;                                    //Ustaw rejestr nr: Command.Auxi 
                     //odpowiedŸ ------
                     CommandToSend.Order  = W_RegRes;
                     CommandToSend.B6.CRC = crc16(CommandToSend.Streem,4);    
                     ByteToSend =6;
                 break;
            /*     
            case RW_RegOrd :  //Zapis + odczyt pojedyñczego rejestru - rozkaz
                     //wykonanie ------
                     CommandToSend.B8.Val  =  SymulacjaRejWej;                            // przepisz dane z rejestru nr: Command.Auxi  (tymczasowo  SymulacjaRej)
                     SymulacjaRejWyj==Command->B8.Val;                                    //Ustaw rejestr nr: Command.Auxi 
                     //odpowiedŸ ------
                     CommandToSend.Order  = RW_RegRes;
                     CommandToSend.B8.CRC = crc16(CommandToSend.Streem,6);    
                     ByteToSend =8;
                 break;
              */   
                 
                 
         //Odpowiedzi rozkazów od slave do sterownika ------------------------------------------------------------   
            
            case R_RegRes :  //Odczyt pojedyñczego rejestru - odp
                     //wykonanie ------
                     //CommandToSend.B8.Val  =  0xCC;                            // przepisz dane z rejestru nr: Command.Auxi  (tymczasowo  0xCC)
                     MainStage.StageI[RF_TaskTab[RF_CurrentTask].Node].Value[RF_TaskTab[RF_CurrentTask].Command.Auxi]=Command->B8.Val; 
                     
                     TransQuality.Mensur[RF_TaskTab[RF_CurrentTask].Node-1] |= 0x1 << TransQuality.MensurIndex;   //wpis do tablicy jakoœci
                     
                     
                 break;
                 
            case W_RegRes :  //Zapis pojedyñczego rejestru  -odp
                     //wykonanie ------
              
                      if ((TransQuality.MensurIndex>32) || (TransQuality.MensurIndex<0))
                      {
                        TransQuality.MensurIndex=0;
                      }
              
              
                      if (RF_TaskTab[RF_CurrentTask].Node-1<=NumberNeurons)
                      {
                          TransQuality.Mensur[RF_TaskTab[RF_CurrentTask].Node-1] |= 0x1 << TransQuality.MensurIndex;   //wpis do tablicy jakoœci
                      }
              
                 
      
    }
    
             
            //Wyœlij odpowiedŸ
            if (ByteToSend)
            {
                RF_Status.EnableTX();
                RF_SendData(CommandToSend.Streem, ByteToSend);
                RF_Status.EnableRX();
            }           
   
    
    

  
  
  
}
//_____________________________________________________________________________






//-----------------------------------------------------------------------------
//!  Nazwa funkcji :    RF_SendData
//!  Funkcja przygotowuje dane do wys³ania przez RF12
//-----------------------------------------------------------------------------
//  *data - wska¿nik na pierwszy bajt
//  *size - iloœæ bajtów do wys³ania
//-----------------------------------------------------------------------------
void RF_SendData(unsigned char *data, char size)
{ 
     //Wpisanie preambu³y otwieraj¹cej
     RF_Tx.Buff[RF_Tx.BytesToSend++]=Preambu[0];
     RF_Tx.Buff[RF_Tx.BytesToSend++]=Preambu[1];
     RF_Tx.Buff[RF_Tx.BytesToSend++]=Preambu[2];
     
     //Wpisanie s³owa synchronizuj¹cego
     RF_Tx.Buff[RF_Tx.BytesToSend++]=SynKay[0];
     RF_Tx.Buff[RF_Tx.BytesToSend++]=SynKay[1];
     
     //Wpisanie danych do kolejki
     for (char i=0; i<size; i++)
     {
        if (RF_Tx.BytesToSend<=RF_BuffSizeTX-3)
        {
          RF_Tx.Buff[RF_Tx.BytesToSend]=data[i]; 
          RF_Tx.BytesToSend++;
        }
        
     }
     
     //Wpisanie preambu³y zamykaj¹cej
     RF_Tx.Buff[RF_Tx.BytesToSend++]=Preambu[0];
     RF_Tx.Buff[RF_Tx.BytesToSend++]=Preambu[0];
     RF_Tx.Buff[RF_Tx.BytesToSend++]=Preambu[0];
     
     if ((RF_Status.Transmit==0) && (RF_Status.EnTx))  RF_transmit();

     
} 





//-----------------------------------------------------------------------------
//!  Nazwa funkcji :    RF_transmit
//!  Funkcja wysy³a dane przez RF12
//-----------------------------------------------------------------------------
void RF_transmit(void)
{
   if ((RF_Tx.BytesToSend==0) || (AT91C_BASE_PIOA->PIO_PDSR & RfIntBUS)) return;
   
   if (RF_Status.Transmit==0) 
   {
    SPI_SendWord(0x0000,Cs_RF); 
    //Delay(0xFFFF);
    //RF_Status.Transmit=1;
   }else{
  
   char BTS=RF_Tx.Buff[0];
   RF_Tx.BytesToSend--;
   for (char i=0; i<RF_Tx.BytesToSend; i++)  RF_Tx.Buff[i]=RF_Tx.Buff[i+1]; //przesuñ kolejke
   SPI_SendWord((0xB800 | BTS),Cs_RF); //wyœlij bajt
   }
}









void RFM12B_SendData(unsigned char data)
{ 
  short int SendWord=0;
 //while(WAIT_NIRQ_LOW());  //??? 
  //WAIT_NIRQ_LOW(); 
  Delay(0x2FF); 
  while(AT91C_BASE_PIOA->PIO_PDSR & AT91C_PIO_PA3);  //???   
  SoftSPI_SendWord((0xB800 | data)); //1.66MHz,2.2V 
} 








