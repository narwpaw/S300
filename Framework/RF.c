//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                  Microcontroller Software Support                        *//                       
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : RF.c                                     *//
//*  Opis                         : Plik obs³ugi sprzêtowej komunikacji radiowej *//
//*  Autor                        : Pawe³ Narwojsz                           *//
//*  Data utrzorzenia             : 31.06.2010                               *//
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



      #include "RF.h"   
      #include "RF_Exe.h"
      #include <intrinsics.h>
      #include "Framework.h"
      #include "core/device.h"   
      #include "board.h"   
      #include "spi_main.h"
      #include "core/trace.h"
      #include "InterruptPriority.h"





_RF_Rx RF_Rx;
_RF_Tx RF_Tx;

char SPI_RFM12_Read=0;

const char Preambu[]={0xAA, 0xAA, 0xAA};  //Preambu³a
const char SynKay[]= {0x2D, 0xD4};        //s³owo synchronizacyjne











//Zmienne globalne -------------------------------------------------------------
  _RF_Status RF_Status;
//___________________________Koniec zmiennych globalnych________________________


  extern unsigned long long  g_counter;
  extern char g_RF_TimeOutCounter;
  extern volatile char SlaveMode;
  
 char tmp_int=0;
  
//Tworzenie pse³do-klasy RF_Status --------------------------------------------
    
  //Uruchomienie trybu Nadajnika
  void _RF_EnableTX(void)
  {
     
    RF_Status.EnRx=0;
    RF_Status.EnTx=0;
    RF_Status.Transmit=0;
    RF_Tx.BytesToSend=0;
    RF_Status.EnRxWait=0;
    RF_Status.RxStarting=0;
    RF_Status.TxStarting=1;

    SPI_SendWord(0x8219,Cs_RF);                          //wy³¹cz odbiornik/nadajnik
    RF_Status.InitTx=1;

#ifdef __MaxRf
    AT91C_BASE_PIOA->PIO_SODR =TxPin; 
#endif    
  }


  
  
  
  
 //Uruchomienie trybu odbiornika
 void _RF_EnableRX(void)
  {
    //Je¿eli przy prubie prze³¹czenia wysy³ane s¹ dane to poczekaj na zakoñczenie wysy³ki
    if ((RF_Tx.BytesToSend>0)) 
    {
      RF_Status.EnRxWait=1; 
    }else{

      SPI_SendWord(0x8067,Cs_RF); 
      
      RF_Status.EnTx=0;
      RF_Status.EnRx=0;
      RF_Tx.BytesToSend=0;
      RF_Status.Transmit=0;
      RF_Status.TxStarting=0;
      RF_Status.RxStarting=1;
      
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






void InitRF_Interrupt(void)
{
    AT91F_PIO_CfgInput(IR_PIO_BUS, RfIntBUS);
    AT91F_PIO_InterruptEnable(PIO_RfIntBUS, RfIntBUS);
    AT91F_AIC_EnableIt(AT91C_BASE_AIC, ID_RfIntBUS);
}

  
  


void RF_Int(void)
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
 

}//_____________________________________________________________________________
//







void ConfigureRFBus(void)
{
#if !defined(IR_RF_DEF)
    AT91F_PMC_EnablePeriphClock(AT91C_BASE_PMC, 1 << ID_RfIntBUS);
    AT91F_PIO_CfgInput(IR_PIO_BUS, RfIntBUS);
    
#endif
}




char IntCA81Flag=0;

void RF_Init(void)
{ 
  #ifndef RF_Int_Init_DEF
  
  TRACE_INFO("RF_Init \n\r"); 
  
      //ustawienia pocz¹tkowe uk³adu
      RF_Status.RF_Status = _pRF_Status;
      RF_Status.RF_Status();  //konstruktor pse³do-klasy RF_Status
        
      
      
            RF_Status.EnTx=0;
      RF_Status.EnRx=0;
      RF_Tx.BytesToSend=0;
      RF_Status.Transmit=0;
      RF_Status.TxStarting=0;
      RF_Status.RxStarting=0;
          RF_Status.EnRx=0;
    RF_Status.EnTx=0;
    RF_Status.Transmit=0;
    RF_Tx.BytesToSend=0;
    RF_Status.EnRxWait=0;
    RF_Status.RxStarting=0;
    RF_Status.TxStarting=0;
    RF_Status.RfFuse=0;
      
        
    //Konfiguracja wstêpna
        #ifdef RF_Master 
              RF_Status.EnTx=1; 
              RF_Status.EnRx=0; 
        #else   
              RF_Status.EnRx=1;
              RF_Status.EnTx=0; 
        #endif
      
   
              
        
        IntCA81Flag=0;
        RF_Status.Init=1;
        SPI_SendWord(0x8067,Cs_RF); //EL,EF,868band,12.0pF   - pierwszy rozkaz konfiguracji

    
       // AT91C_BASE_PIOA->PIO_SODR =SpeakerPin;  
       //ButtonBipCounter=g_counter+1;
  
        
   #endif
}//_____________________________________________________________________________















char CountVal=16;
void SPI_RFM12_Execute(char *Buffer_Tx, char *Buffer_Rx)                                       
{
    int Order=  Buffer_Tx[0] | Buffer_Tx[1]<<8;

    
    //Je¿eli uk³ad RF12 w trakcie inicjalizacji pierwotnej---------------------
    


    
    
    if (RF_Status.Init)
    {
               
      if (Order==0x8067) 
      //{ if (RF_Status.EnRx)  RF_Status.EnableRX(); else RF_Status.EnableTX(); } //Rozkaz: 0x80E7 - EL,EF,868band,12.0pF 
      { SPI_SendWord(0x8219,Cs_RF); 
       if (RF_Status.EnRx)  SPI_SendWord(0x8299,Cs_RF); else SPI_SendWord(0x8239,Cs_RF); 
      } //Rozkaz: 0x80E7 - EL,EF,868band,12.0pF 
     

      if ((Order==0x8299) || (Order==0x8239) || (Order==0x8219))
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
        
          TRACE_INFO("Enable RX\n\r"); 
          
          InitRF_Interrupt();
          
        
        
        
      }
     
      
    }//Aktywny RX ---------------------------------------------------------------
    else if ((RF_Status.EnRx) && (RF_Status.Init==0))
    {
      /*
      if  (((Order==0x0000) && (Buffer_Rx[1]&0x80)) 
           ||((Order==0x0000) && ((Buffer_Rx[1]&0x02)==0)))  //zmiana przy v0.8d
      */
       if  ((Order==0x0000) && (Buffer_Rx[1]&0x81))                        //zmiana przy v0.8e by³o (Buffer_Rx[1]&0x81) 
           { 
             SPI_SendWord(0xB000,Cs_RF);
           }
      
      if  (Order==0xCA81)   { SPI_SendWord(0xCA83,Cs_RF); }
      if  (Order==0xCA83)   { SPI_SendWord(0x0000,Cs_RF); }
      if  (Order==0xB000)  { RF_ReciveByte(Buffer_Rx[0]); }
     
      

    
    
    
    }//Trzb oczekiwania na prze³¹czenie na TX ---------------------------------------------------------------
    else if ((RF_Status.InitTx) && (Order!=0x0000))
        {
                
          if (Order==0x8219)
          {
            SPI_SendWord(0x80E7,Cs_RF);
          }
          else if (Order==0x80E7)
          {                                     
            SPI_SendWord(0x8239,Cs_RF);
          }
          else if (Order==0x8239)
          {
            SPI_SendWord(0x0000,Cs_RF);
          }
        }  
      else if ((RF_Status.InitTx) && (Order==0x0000))
        {    

          if (Buffer_Rx[1]&0x20)
          {
            SPI_SendWord(0x0000,Cs_RF);
          }
          //w³¹czenie nadawania danych
          else
          {
            RF_Status.InitTx=0;
            RF_Status.EnTx=1;
            RF_Status.Transmit=1;
            RF_transmit();
          }
          
         

    }//Aktywny TX ---------------------------------------------------------------
    else if (RF_Status.EnTx)
    {


    
    }


    //W³¹czenie odbiornika
    else if ((RF_Status.EnRx==0) && (RF_Status.EnTx==0) && (Order==0x8299))
    {
       RF_Status.EnRx=1;
       SPI_SendWord(0x0000,Cs_RF);
       
       #ifdef __MaxRf
            AT91C_BASE_PIOA->PIO_CODR =TxPin; 
       #endif
    }

    
 
    else if ((RF_Status.InitTx==0) && (RF_Status.EnRx==0) && (RF_Status.EnTx==0) && (Order==0x8239))
    {
      RF_Status.InitTx=1;
     
      SPI_SendWord(0x0000,Cs_RF);    
    }
 
    
  
 
}  







//-----------------------------------------------------------------------------
//!  Nazwa funkcji :    RF_ReciveByte
//!  Funkcja interpretacji otrzymanego bajtu
//-----------------------------------------------------------------------------
void RF_ReciveByte(char Res)
{
     
  
          for (char i=0; i<RF_BuffSizeRX-1; i++) {RF_Rx.Buff[i]=RF_Rx.Buff[i+1];}
          RF_Rx.Buff[RF_BuffSizeRX-1]=Res;

          if (RF_Rx.ReciveBytes<=RF_BuffSizeRX) RF_Rx.ReciveBytes++;

          //sprawdzenie koñca
          if (
          (RF_Rx.Buff[RF_BuffSizeRX-3]==Preambu[0]) &&
            (RF_Rx.Buff[RF_BuffSizeRX-2]==Preambu[1]) &&
          (RF_Rx.Buff[RF_BuffSizeRX-1]==Preambu[2]) 
          /*  && 
          ((RF_Rx.ReciveBytes%2)!=0)
            */)  
          {           
            if ((SlaveMode==0) && (RF_Status.ConfResive==0)) RF_Status.ConfResive=1;  //znacznik odebrania danych (dla kontroli kana³u)
            if ((SlaveMode==1) && (RF_Status.ConfResiveSlave==0)) RF_Status.ConfResiveSlave=1;  //znacznik odebrania danych (dla kontroli kana³u)
            
            RF_Status.RfFuse=1;
            RF_ReciveFarm(&RF_Rx.Buff[RF_BuffSizeRX-4], RF_Rx.ReciveBytes-3);  //funkcja interpretacji otrzymanej ramki
          
             RF_Rx.ReciveBytes=0; 
           
            
            //RF_ReciveFarm(&RF_Rx.Buff[RF_BuffSizeRX-RF_Rx.ReciveBytes], RF_Rx.ReciveBytes-3);  //funkcja interpretacji otrzymanej ramki

            //RF_Rx.ReciveBytes=0;

            if (!RF_Tx.BytesToSend) SPI_SendWord(0xCA81,Cs_RF); 
          }
}





//-----------------------------------------------------------------------------
//!  Nazwa funkcji :    RF_SendFarm
//!  Funkcja przygotowuje dane do wys³ania przez RF12. Opakowuje dane w ramkê
//-----------------------------------------------------------------------------
//  *data - wska¿nik na pierwszy bajt
//  *size - iloœæ bajtów do wys³ania
//-----------------------------------------------------------------------------
void RF_SendFarm(unsigned char *data, char size)
{ 
    RF_Status.EnableTX();
    char BTS;
    //char DTS[20];
    BTS=RF_Tx.BytesToSend;
    
     //Wpisanie preambu³y otwieraj¹cej
     RF_Tx.Buff[BTS++]=Preambu[0];
     RF_Tx.Buff[BTS++]=Preambu[1];
     RF_Tx.Buff[BTS++]=Preambu[2];
     
     //Wpisanie s³owa synchronizuj¹cego
     RF_Tx.Buff[BTS++]=SynKay[0];
     RF_Tx.Buff[BTS++]=SynKay[1];
     
     //Wpisanie danych do kolejki
     for (char i=0; i<size; i++)
     {
        if (BTS<=RF_BuffSizeTX-3)
        {
          RF_Tx.Buff[BTS]=data[i]; 
          BTS++;
        }
     }
     
     //Wpisanie preambu³y zamykaj¹cej
     RF_Tx.Buff[BTS++]=Preambu[0];
     RF_Tx.Buff[BTS++]=Preambu[1];
     RF_Tx.Buff[BTS++]=Preambu[2];
     RF_Tx.Buff[BTS++]=0x00;
     RF_Tx.Buff[BTS]=0x00;
     
     
     //for (char i=0; i<BTS; i++) RF_Tx.Buff[i]=DTS[i];
     RF_Tx.BytesToSend=BTS;
     if ((RF_Status.Transmit==0) && (RF_Status.EnTx) && (RF_Status.InitTx==0))  RF_transmit();

     if ((SlaveMode) && (RF_Status.TimeInSlaveMode>MAX_TIME_FOR_SLAVE_ASK))
                      {
                       RF_Status.SlaveModeTimeout=4;
                      }
     
     RF_Status.EnableRX();
} 
//-----------------------------------------------------------------------------






//-----------------------------------------------------------------------------
//!  Nazwa funkcji :    RF_transmit
//!  Funkcja wysy³a dane przez RF12
//-----------------------------------------------------------------------------
void RF_transmit(void)
{
   if ((RF_Tx.BytesToSend==0) || (AT91C_BASE_PIOA->PIO_PDSR & RfIntBUS)) return;
   
   if (RF_Status.Transmit) 
   {
      char BTS=RF_Tx.Buff[0];
      RF_Tx.BytesToSend--;
      for (char i=0; i<RF_Tx.BytesToSend; i++)  RF_Tx.Buff[i]=RF_Tx.Buff[i+1]; //przesuñ kolejke
      SPI_SendWord((0xB800 | BTS),Cs_RF); //wyœlij bajt
   }
}



//-----------------------------------------------------------------------------
//!  Nazwa funkcji :    RF_CheckChannel
//!  Funkcja sprawdza czy kana³ jest wolny
//-----------------------------------------------------------------------------
extern int MenuLevel;

char RF_CheckChannel(void)
{
     int MenuLevel_Member=MenuLevel;
     MenuLevel=3;
     MenuPaint();
     
     RF_Status.ConfResive=0; 
     

     
     Delay(0x100000);

     
     
     MenuLevel=MenuLevel_Member;  
     MenuPaint();
     return RF_Status.ConfResive;

}
