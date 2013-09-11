//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                  Microcontroller Software Support                        *//                       
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : RF_Exe.c                                 *//
//*  Opis                         : Plik wykonawczy komunikacji radiowej     *//
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


#include "RF_Exe.h"
#include "Application_prog.h" 
#include "spi_main.h"  
#include "main_H.h"  

//tymczasowe
extern int licznik_cykli;
int poprawnych;
extern int odb[2];
extern char IndWys; 
extern volatile char SlaveMode;
extern long long int g_counter;
unsigned int        RF_Period;                                              //cykl transmisji

char IndWys=0; 
char IndWysNeg=1; 
int odb[2];
int licznik_cykli=0;
extern unsigned int CyklPerSekCount; 
extern unsigned long long g_NextTimeProgram; 
extern volatile char Busy_program;
extern volatile long long TimeInMasterMode;
extern _NeuronsListUnit NeuronsList[NumberNeurons];
extern AT91_SPI_Buff               SPI_Buff;
//tymczasowo koniec-----------
extern volatile char ReciveFrameToMe;
extern unsigned char ChangeStageFlag;
extern AT91PS_MEM pAT91C_IFLASH_MEM;

#ifdef _LIVE_RJESTR
extern unsigned short LIVE_TAB[LIVE_TAB_SIZE];
extern char LIVE_TAB_IDX;




#endif


 

//Zmienne globalne -------------------------------------------------------------
  
  _RF_TaskTab  RF_TaskTab;
  signed char           RF_CurrentTask=-1; 
  signed char           RF_SpecialTask=-1; 
  char g_RF_TimeOutCounter =0;  //bierz¹cy czas odliczania do timeout`u
  
 
  
  unsigned short       ReadNodeSoftVersion;
  _RF_NodeTest         RF_NodeTest; 
   // unsigned  short    TimeToSwithToMaster=MAX_TIME_FOR_SLAVE_ASK;
    char               BridgeMode=0;
//__________________________Koniec zmiennych globalnych________________________


  
  
//Zmienne globalne importowane--------------------------------------------------  
  extern _RF_Status   RF_Status;
  extern _MainStage  MainStage;
  extern _TransQuality TransQuality;
  //extern volatile char SwitchToMasterBlock;
//___________________________Koniec zmiennych globalnych importowanych_________

  
 
  


//#define IncCurentRfTask  RF_CurrentTask++;  if ((RF_TaskTab[RF_CurrentTask].Node==0) || (RF_CurrentTask>=RF_Tasks))  {RF_CurrentTask=0; CyklPerSekCount++;}
   


void SwitchToMaster(void)
{
    if ((SPI_Buff.WordToSendCount) /*|| (RF_Status.EnTx!=0) || (RF_Status.EnRx==0)*/) return;
   
   RF_Status.SlaveModeTimeout=MAX_TIME_FOR_SLAVE_ASK;
   RF_Status.TimeInMasterMode=0;
   RF_Status.TimeInSlaveMode=0;  
  
  
   // AT91C_BASE_PIOA->PIO_SODR =SpeakerPin;
   // ButtonBipCounter=g_counter+10000;

   SPI_SendWord(0x8219,Cs_RF); 
   SlaveMode=0;
   RF_Status.MasterMode=1;
   ReciveFrameToMe=0;
   SPI_SendWord(0xA000 | RF_SetFreqReg(RF_Status.Ch),Cs_RF);  
   RF_Status.EnableRX();

}

void SwitchToSlave(void)
{
   RF_Status.TimeInSlaveMode=0;
   RF_Status.TimeInMasterMode=0;
   RF_Status.SlaveModeTimeout=MAX_TIME_FOR_SLAVE_ASK;
  
   // AT91C_BASE_PIOA->PIO_CODR =SpeakerPin;
   
   SPI_SendWord(0x8219,Cs_RF); 
  // Delay(0x4FF);
   SPI_SendWord(0x80E7,Cs_RF); 
   SlaveMode=1;

   RF_Status.MasterMode=0;
   SPI_SendWord(0xA000 | RF_SetFreqReg(RF_Status.ChSlave),Cs_RF);  
   RF_Status.EnableRX();

}


//-----------------------------------------------------------------------------
//!  Nazwa funkcji :    RF_ExecuteOrder
//!  Funkcja interpretuje i realizuje otrzymany rozkaz
//-----------------------------------------------------------------------------
//  Res   - wskaŸnik na bufor danych
//  Size  - wielkoœæ bufora danych
//  Buff  - wskaŸnik na ostatni bajt danych 
//-----------------------------------------------------------------------------
  _RF_Command  *pRF_Command;
char RF_ReciveFarm(char *Buff, char Size)
{
          char FrameOk=0;
          
          if ((Size==8) && (FrameOk==0))
          {
             pRF_Command=(_pRF_Command)(Buff-7); //rzutowanie na strukturê rozkazu
             if (pRF_Command->B8.CRC == crc16(pRF_Command->Streem, Size-2))  FrameOk=1;
          }
          else if (Size==6)
          {
             pRF_Command=(_pRF_Command)(Buff-5); //rzutowanie na strukturê rozkazu
             if (pRF_Command->B6.CRC == crc16(pRF_Command->Streem, Size-2))  FrameOk=1;
          }   
          else 
          { 
            if ((Size>8) && (FrameOk==0))
            {
              pRF_Command=(_pRF_Command)(Buff-7); //rzutowanie na strukturê rozkazu
              if (pRF_Command->B8.CRC == crc16(pRF_Command->Streem, 6))  FrameOk=1;
            }
            
            if ((Size>6) && (FrameOk==0))
            {
             pRF_Command=(_pRF_Command)(Buff-5); //rzutowanie na strukturê rozkazu
             if (pRF_Command->B6.CRC == crc16(pRF_Command->Streem, 4)) FrameOk=1;
            } 
          
          }
         
          

          
          //je¿eli ramka poprawna
          if (FrameOk==1)
          {  
              
                  char Odbierz=0;
                  //Sprawdzenie zgodnoœci ID 
                  if(((pRF_Command->ID==RF_TaskTab[RF_CurrentTask].Command.ID) && (!SlaveMode)) ||
                     ((pRF_Command->ID==RF_Status.RF_ID) && (SlaveMode))
                     ) 
                  {
                     Odbierz=1;
                  }
                  else if ((pRF_Command->ID==0)  && (SlaveMode)) //ramka rozg³oœna 
                  {
                    Odbierz=1;
                  }
                  else if ((AT91C_IFLASH_MEM->Config.SlaveMode) && (SlaveMode) && (BridgeMode)) 
                  {
                    for (char ND=0; ND<NumberNeurons-1; ND++)
                    {
                      if ((pRF_Command->ID==NeuronsList[ND].NeuronID) && (TransQuality.QuickMensur[ND]))
                      {
                        Odbierz=1;
                        break;
                      }
                    }
                  }else if (((RF_TaskTab[RF_CurrentTask].Command.Order==R_BC_Order) || (RF_TaskTab[RF_CurrentTask].Command.Order==W_BC_Order)) && (pRF_Command->BC_ID2==RF_TaskTab[RF_CurrentTask].Command.ID))
                  {
                    Odbierz=1;
                  }
                  
                  
                  
                  
                  
                  if (Odbierz)
                  {
                    if ((SlaveMode==0) && (RF_Status.ConfResive==1)) RF_Status.ConfResive=2;  //znacznik odebrania danych (dla kontroli kana³u)
                    if ((SlaveMode==1) && (RF_Status.ConfResiveSlave==1)) RF_Status.ConfResiveSlave=2;  //znacznik odebrania danych (dla kontroli kana³u)
                     
                    RF_ExecuteOrder(pRF_Command);   //realizacja rozkazu   
                    
                    
                    if (SlaveMode)
                    {
                      if  (RF_Status.TimeInSlaveMode>MAX_TIME_FOR_SLAVE_ASK)
                      {
                       RF_Status.SlaveModeTimeout=2;
                      }else if  (RF_Status.TimeInSlaveMode>100)
                      {
                       RF_Status.SlaveModeTimeout=SLAVE_MODE_TIMEOUT; 
                      }
                    }
                    
                    

                  }
                  
                  if ((SlaveMode) && (pRF_Command->ID!=RF_Status.RF_ID) && (pRF_Command->ID!=0) && (RF_Status.SlaveModeTimeout>0) && (RF_Status.TimeInSlaveMode>100))
                  {
                    RF_Status.SlaveModeTimeout=0;
                  }

                  poprawnych++;
                  odb[IndWys]++;
          }
          return FrameOk;

}





//-----------------------------------------------------------------------------
//!  Nazwa funkcji :    RF_ExecuteOrder
//!  Funkcja interpretuje i realizuje otrzymany rozkaz
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void RF_ExecuteOrder(_RF_Command *Command)
{
 // TRACE_INFO("Id: %X,  Ord: %X,  Reg: %X,  Val: %X \n\r", Command->ID, Command->Order, Command->Auxi, Command->B8.Val); 
   licznik_cykli=0;  
   char Nd;
   char FoundNode, FoundTask;             
   
   _RF_Command CMD;
   for (char i=0; i<sizeof(_RF_Command); i++) {CMD.Streem[i]=Command->Streem[i];}
   int BC_ID2=(int)CMD.BC_ID2; 
   char BC_REG=(char)CMD.BC_REG;
   char* pNODE_ID=(char*)&NODE_ID;
   char Order=CMD.Order;
   _RF_Command  CommandToSend;
   
   //czêœæ wspólna wszystkich odpowiedzi
   CommandToSend.ID     = RF_Status.RF_ID;
   CommandToSend.Auxi   = Command->Auxi;
   
       
    char ByteToSend=0;          
    char NextModule=0;
    switch (Order)
    {  
     
       //Rozkazy dla Slave ------------------------------------------------------------
      
            case R_RegOrd :  //Odczyt pojedyñczego rejestru - rozkaz
                     //wykonanie ------
                     if (Command->ID==RF_Status.RF_ID)  //je¿eli odpytanie bezpoœrednie
                     {
                        CommandToSend.B8.Val  =  MainStage.StageQ[0].Value[CommandToSend.Auxi];                     // przepisz dane z rejestru nr: Command.Auxi  (tymczasowo  SymulacjaRej)
                     }else{ 
                        for (char ND=0; ND<NumberNeurons-1; ND++)
                        {
                          if ((NeuronsList[ND].Bridge) && (Command->ID==NeuronsList[ND].NeuronID))
                          {
                            CommandToSend.B8.Val  =  MainStage.StageI[ND+1].Value[CommandToSend.Auxi];                     // przepisz dane z rejestru nr: Command.Auxi  (tymczasowo  SymulacjaRej)
                          }
                        }

                        CommandToSend.ID=Command->ID;
                     }
                     //odpowiedŸ ------
                     CommandToSend.Order  = R_RegRes;
                     CommandToSend.B8.CRC = crc16(CommandToSend.Streem,6);    
                     ByteToSend =8;
                 break;
                 
            case W_RegOrd :  //Zapis pojedyñczego rejestru  -rozkaz
                    
                    //przyœpieszenie wykonania programu
                    if ((CommandToSend.Auxi==0) && (MainStage.StageQ[0].Value[CommandToSend.Auxi]!=Command->B8.Val) && (Busy_program==0))
                    {   
                      g_NextTimeProgram=g_counter+1;
                    }
              
                    
                    
                     //wykonanie ------
                     if (Command->ID==RF_Status.RF_ID)  //je¿eli wykonanie bezpoœrednie
                     {
                       MainStage.StageI[0].Value[CommandToSend.Auxi]=Command->B8.Val;                                    //Ustaw rejestr nr: Command.Auxi 
                     }else{
                       CommandToSend.ID=Command->ID; 
                       for (char ND=0; ND<NumberNeurons-1; ND++)
                        {
                          if ((NeuronsList[ND].Bridge) && (Command->ID==NeuronsList[ND].NeuronID))
                          {
                            MainStage.StageQ[ND+1].Value[CommandToSend.Auxi]=Command->B8.Val;                                    //Ustaw rejestr nr: Command.Auxi 
                          }
                        }
                     }
                    
                    
                     //odpowiedŸ ------
                     CommandToSend.Order  = W_RegRes;
                     CommandToSend.B6.CRC = crc16(CommandToSend.Streem,4);    
                     ByteToSend =6;
                 break;
                
                 
               
              case R_BC_Order :  //OdpowiedŸ na pytanie kto ma wêze³ (+ dodanie zadania odpytania)
                     //wykonanie ------
                     //CommandToSend.B8.Val  =  MainStage.StageQ[0].Value[CommandToSend.Auxi];                     // przepisz dane z rejestru nr: Command.Auxi  (tymczasowo  SymulacjaRej)
                    
                    // AT91C_BASE_PIOA->PIO_SODR =SpeakerPin;
                    // ButtonBipCounter=g_counter+10000;
                 
         
                     
                     Nd=0;
                     FoundNode=0xFF;
                     FoundTask=0;
                
              
                     if (BC_ID2==RF_Status.RF_ID) //jezeli zapytanie rozg³oœne o mnie 
                     {

                         CommandToSend.Order= RW_BC_Res;
                         CommandToSend.BC_ID1=RF_Status.RF_ID; 
                         CommandToSend.B8.CRC = crc16(CommandToSend.Streem,6);    
                         ByteToSend =8;
                      
                     }else{  //jezeli zapytanie rozg³oœne o trzeci modu³
                     
                             while ((FoundNode==0xFF) && (NeuronsList[Nd].NeuronID!=BC_ID2) && (Nd<(NumberNeurons-1))) Nd++; 
                               
                             if (NeuronsList[Nd].NeuronID==CMD.BC_ID2 ) FoundNode=Nd;     
                    
              
                             if (FoundNode==0xFF)//je¿eli nie znaleziono wêz³a to dodaj
                             {
                                 //znalezienie miejsca dla nowego wêz³a ---------------------------
                                 //char NewNode=NumberNeurons-1; 
                                 //while ((NewNode>0) && (NeuronsList[NewNode].NeuronID!=0)) {NewNode--;}
        
                                 FoundNode=0;
                                 while ((NeuronsList[FoundNode].NeuronID!=0) && (FoundNode<NumberNeurons-1)) {FoundNode++;}
                                 
                                 if ((FoundNode>=NumberNeurons-1) || (NeuronsList[FoundNode].NeuronID!=0))  //je¿eli jest miejsce na dodanie nowego wêz³a
                                 {
                                   FoundNode=0xFF;
                                 }else{
                                   NeuronsList[FoundNode].NeuronID=BC_ID2;
                                   NeuronsList[FoundNode].Bridge=1;
                                   NeuronsList[FoundNode].NeuronType=NodeRF_Period;
                                   BridgeMode=1; 
                                 }
                                 FoundTask=0;

                             }else{ //je¿eli wêze³ znaleziony
           
                                char i=0;
                                while ((i<RF_Tasks-1) && (FoundTask==0))
                                {
                                   if ((RF_TaskTab[i].Node==FoundNode+1) && 
                                       (RF_TaskTab[i].Command.ID==NeuronsList[FoundNode].NeuronID) &&
                                       (RF_TaskTab[i].Command.Order==R_RegOrd) &&
                                       (RF_TaskTab[i].Command.Auxi==BC_REG))
                                      {
                                        FoundTask=1;
                                      }
                                  
                                   i++;
                                }
                                
                                
                                
                                if ((FoundTask) && (TransQuality.Mensur[FoundNode]))
                                 {
                                  //odpowiedŸ ------
                                   CommandToSend.Order= RW_BC_Res;
                                   CommandToSend.BC_ID2=BC_ID2; 
                                   CommandToSend.B8.CRC = crc16(CommandToSend.Streem,6);    
                                   ByteToSend =8;
                                 } 
                               
                              
                             }  
                             
                             
                             if ((FoundTask==0) && (FoundNode!=0xFF))
                             if (AddRfTask(R_RegOrd, FoundNode+1, BC_REG)==0)  //je¿eli nie uda³o siê dodaæ zadania to usuñ wszystkie œlady po tym wêŸle
                                 {
                                   RemoveRfNodeAllTaska(FoundNode);
                                   NeuronsList[FoundNode].NeuronID=0;
                                   NeuronsList[FoundNode].Bridge=0;
                                   NeuronsList[FoundNode].NeuronType=0;
                                 }else{
                                   RF_Status.TimeToRemBridgeNoCom= MAX_TIME_TO_REM_BRIDGE_NO_CON;
                                 }
                            

                     
                    } //koniec zapytania rozg³oœnego o trzeci modu³
                  
               
                     //AT91C_BASE_PIOA->PIO_CODR =SpeakerPin;

                     
               break;   

                 
              case W_BC_Order :  //OdpowiedŸ na pytanie kto ma wêze³ (+ dodanie zadania odpytania)
                     //wykonanie ------
                     //CommandToSend.B8.Val  =  MainStage.StageQ[0].Value[CommandToSend.Auxi];                     // przepisz dane z rejestru nr: Command.Auxi  (tymczasowo  SymulacjaRej)
             
                     
                    Nd=0;
                     FoundNode=0xFF;
                     FoundTask=0;
                
              
                     if (BC_ID2==RF_Status.RF_ID) //jezeli zapytanie rozg³oœne o mnie 
                     {

                         CommandToSend.Order= RW_BC_Res;
                         CommandToSend.BC_ID1=RF_Status.RF_ID; 
                         CommandToSend.B8.CRC = crc16(CommandToSend.Streem,6);    
                         ByteToSend =8;
                      
                     }else{  //jezeli zapytanie rozg³oœne o trzeci modu³
                     
                             while ((FoundNode==0xFF) && (NeuronsList[Nd].NeuronID!=BC_ID2) && (Nd<(NumberNeurons-1))) Nd++; 
                               
                             if (NeuronsList[Nd].NeuronID==CMD.BC_ID2 ) FoundNode=Nd;     
                    
              
                             if (FoundNode==0xFF)//je¿eli nie znaleziono wêz³a to dodaj
                             {
                                 //znalezienie miejsca dla nowego wêz³a ---------------------------
                                 //char NewNode=NumberNeurons-1; 
                                 //while ((NewNode>0) && (NeuronsList[NewNode].NeuronID!=0)) {NewNode--;}
        
                                 FoundNode=0;
                                 while ((NeuronsList[FoundNode].NeuronID!=0) && (FoundNode<NumberNeurons-1)) {FoundNode++;}
                                 
                                 if ((FoundNode>=NumberNeurons-1) || (NeuronsList[FoundNode].NeuronID!=0))  //je¿eli jest miejsce na dodanie nowego wêz³a
                                 {
                                   FoundNode=0xFF;
                                 }else{
                                   NeuronsList[FoundNode].NeuronID=BC_ID2;
                                   NeuronsList[FoundNode].Bridge=1;
                                   NeuronsList[FoundNode].NeuronType=NodeRF_Period;
                                   BridgeMode=1; 
                                 }
                                 FoundTask=0;

                             }else{ //je¿eli wêze³ znaleziony
           
                                char i=0;
                                while ((i<RF_Tasks-1) && (FoundTask==0))
                                {
                                   if ((RF_TaskTab[i].Node==FoundNode+1) && 
                                       (RF_TaskTab[i].Command.ID==NeuronsList[FoundNode].NeuronID) &&
                                       (RF_TaskTab[i].Command.Order==W_RegOrd) &&
                                       (RF_TaskTab[i].Command.Auxi==BC_REG))
                                      {
                                        FoundTask=1;
                                      }
                                  
                                   i++;
                                }
                                
                                
                                
                                if ((FoundTask) && (TransQuality.Mensur[FoundNode]))
                                 {
                                  //odpowiedŸ ------
                                   CommandToSend.Order= RW_BC_Res;
                                   CommandToSend.BC_ID2=BC_ID2; 
                                   CommandToSend.B8.CRC = crc16(CommandToSend.Streem,6);    
                                   ByteToSend =8;
                                 } 
                               
                              
                             }  
                             
                             
                             if ((FoundTask==0) && (FoundNode!=0xFF))
                             if (AddRfTask(W_RegOrd, FoundNode+1, BC_REG)==0)  //je¿eli nie uda³o siê dodaæ zadania to usuñ wszystkie œlady po tym wêŸle
                                 {
                                   RemoveRfNodeAllTaska(FoundNode);
                                   NeuronsList[FoundNode].NeuronID=0;
                                   NeuronsList[FoundNode].Bridge=0;
                                   NeuronsList[FoundNode].NeuronType=0;
                                 }
                            

                     
                    } //koniec zapytania rozg³oœnego o trzeci modu³
                  
                 
               break;
               

                     
            case W_NewIdOrd :  //zapis noweg ID
                     //wykonanie ------
                    // NODE_ID=Command->CHID_NEW_ID; 
                       
                     //CommandToSend.CHID_CUR_ID=RF_Status.RF_ID;

                     RF_Status.RF_ID=Command->CHID_NEW_ID;//skrócone id
                     RF_Status.RF_ID_R=3000000+Command->CHID_NEW_ID;//pe³ne id

                     
                     unsigned int Save = __get_interrupt_state();            
                     __disable_interrupt();
                     FlashWrite((char*)(&(AT91C_IFLASH_MEM->NODE_ID)), (char*)&RF_Status.RF_ID_R,sizeof(RF_Status.RF_ID_R)); //(char *ptrMem, char *value, int length)
                     __set_interrupt_state(Save);
                     __enable_interrupt();  
                       
                     
                     
                     //odpowiedŸ ------
                     CommandToSend.Order  = W_NewIdRes;
                     CommandToSend.B8.CRC = crc16(CommandToSend.Streem,6);    
                     ByteToSend =8;
             break;
               
               

               
               
               
//***********************************************************************************************                 
 //Odpowiedzi rozkazów od slave do sterownika ------------------------------------------------------------   
//***********************************************************************************************
               
               
               
               
            case R_RegRes :  //Odczyt pojedyñczego rejestru - odp
                     //wykonanie ------
                     //CommandToSend.B8.Val  =  0xCC;                            // przepisz dane z rejestru nr: Command.Auxi  (tymczasowo  0xCC)

#ifdef _LIVE_RJESTR  
                   LIVE_TAB[LIVE_TAB_IDX]=0xB000 | (((RF_TaskTab[RF_CurrentTask].Node)&0xF)<<8) | ((Command->B8.Val)&0xFF);
                   if (LIVE_TAB_IDX<LIVE_TAB_SIZE) LIVE_TAB_IDX++; else LIVE_TAB_IDX=0;
#endif              
              
                    //przyœpieszenie wykonania programu
                    if ((RF_TaskTab[RF_CurrentTask].Command.Auxi==0) && (MainStage.StageI[RF_TaskTab[RF_CurrentTask].Node].Value[RF_TaskTab[RF_CurrentTask].Command.Auxi]!=Command->B8.Val) && (Busy_program==0))
                    {   
                      g_NextTimeProgram=g_counter+1;
                      
                 
                      //sygna³ dzwiêkowy je¿eli odczytano zmianê stanu na 1
                  /*    
                       if ((Command->B8.Val>MainStage.StageI[RF_TaskTab[RF_CurrentTask].Node].Value[RF_TaskTab[RF_CurrentTask].Command.Auxi]) && (NeuronsList[RF_TaskTab[RF_CurrentTask].Node-1].NeuronType==NodeRF_Remote) && (!(AT91C_IFLASH_MEM->Config.SoundOff)))
                       {
                          AT91C_BASE_PIOA->PIO_SODR =SpeakerPin;  
                          ButtonBipCounter=g_counter+1;
                       }
                   */
                      
                      
                        #ifdef _LIVE_RJESTR    
                            LIVE_TAB[LIVE_TAB_IDX]=0xFFFF;
                          if (LIVE_TAB_IDX<LIVE_TAB_SIZE) LIVE_TAB_IDX++; else LIVE_TAB_IDX=0;
                      #endif   
                    
                    }

                    MainStage.StageI[RF_TaskTab[RF_CurrentTask].Node].Value[RF_TaskTab[RF_CurrentTask].Command.Auxi]=Command->B8.Val; 
                    
                    
                    #ifdef  DIRECT_Q_MESURE
                     TransQuality.DirectkMensur[RF_TaskTab[RF_CurrentTask].Node-1]--; 
                     #endif
                    
         
                     TransQuality.Mensur[RF_TaskTab[RF_CurrentTask].Node-1] |= 0x1 << TransQuality.MensurIndex;   //wpis do tablicy jakoœci
                     TransQuality.QuickMensur[RF_TaskTab[RF_CurrentTask].Node-1] |= 0x1 << TransQuality.QuickMensurIndex;   //wpis do tablicy jakoœci 
                     TransQuality.SlowMensur[RF_TaskTab[RF_CurrentTask].Node-1] |= 0x1 << TransQuality.SlowMensurIndex;   //wpis do tablicy jakoœci d³ugotrwa³ej
                      
                     
                     if ((RF_NodeTest.NodeNr==RF_TaskTab[RF_CurrentTask].Node) && (RF_NodeTest.Sent>0))
                      {
                          RF_NodeTest.Recived++; 
                          RF_NodeTest.SentSyn=RF_NodeTest.Sent;
                      }
                     
                     NextModule=1;
                     
                     
                    // //Reakcja na rz¹danie linku
                    // NeuronsList[RF_TaskTab[RF_CurrentTask].Node].Link
                     
                     
                 break;
                 
            case W_RegRes :  //Zapis pojedyñczego rejestru  -odp
                     //wykonanie ------
              
                   
                       MainStage.SendStageChange[RF_TaskTab[RF_CurrentTask].Node]=0;
                       RF_SpecialTask=-1;
                   
                      if (RF_TaskTab[RF_CurrentTask].Node-1<=NumberNeurons)
                      {
                          TransQuality.Mensur[RF_TaskTab[RF_CurrentTask].Node-1] |= 0x1 << TransQuality.MensurIndex;   //wpis do tablicy jakoœci
                          TransQuality.QuickMensur[RF_TaskTab[RF_CurrentTask].Node-1] |= 0x1 << TransQuality.QuickMensurIndex;   //wpis do tablicy jakoœci 
                          TransQuality.SlowMensur[RF_TaskTab[RF_CurrentTask].Node-1] |= 0x1 << TransQuality.SlowMensurIndex;   //wpis do tablicy jakoœci d³ugotrwa³ej
                      }
                      
                      if ((RF_NodeTest.NodeNr==RF_TaskTab[RF_CurrentTask].Node) && (RF_NodeTest.Sent>0))
                      {
                          RF_NodeTest.Recived++;
                          RF_NodeTest.SentSyn=RF_NodeTest.Sent;
                      }
                      
                          
                  //   g_RF_TimeOutCounter=0;
                     
                     NextModule=1;
                  #ifdef _LIVE_RJESTR            
                     LIVE_TAB[LIVE_TAB_IDX]=0xD000 | (((RF_TaskTab[RF_CurrentTask].Node)&0xF)<<8) | 0xFF;
                   if (LIVE_TAB_IDX<LIVE_TAB_SIZE) LIVE_TAB_IDX++; else LIVE_TAB_IDX=0;
                  #endif   
                   
                   
                   #ifdef  DIRECT_Q_MESURE
                     TransQuality.DirectkMensur[RF_TaskTab[RF_CurrentTask].Node-1]--; 
                     #endif
                   /*
                   if (RF_TaskTab[RF_CurrentTask].Node==6)
                   {
                      AT91C_BASE_PIOA->PIO_CODR =SpeakerPin; 
                      Delay(0xFFF);
                      AT91C_BASE_PIOA->PIO_SODR =SpeakerPin;
                   }
                   */
               break;
               
               
               
               
               case R_Iam :  //odpowiedŸ z ramki rozg³oœnej 
     
                    //Uwaga!!! ramka nie jest obs³ugiwana
                    
                    #ifdef  DIRECT_Q_MESURE
                     TransQuality.DirectkMensur[RF_TaskTab[RF_CurrentTask].Node-1]--; 
                     #endif
         
                     TransQuality.Mensur[RF_TaskTab[RF_CurrentTask].Node-1] |= 0x1 << TransQuality.MensurIndex;   //wpis do tablicy jakoœci
                     TransQuality.QuickMensur[RF_TaskTab[RF_CurrentTask].Node-1] |= 0x1 << TransQuality.QuickMensurIndex;   //wpis do tablicy jakoœci 
                     TransQuality.SlowMensur[RF_TaskTab[RF_CurrentTask].Node-1] |= 0x1 << TransQuality.SlowMensurIndex;   //wpis do tablicy jakoœci d³ugotrwa³ej
                      
                     
                     if ((RF_NodeTest.NodeNr==RF_TaskTab[RF_CurrentTask].Node) && (RF_NodeTest.Sent>0))
                      {
                          RF_NodeTest.Recived++; 
                          RF_NodeTest.SentSyn=RF_NodeTest.Sent;
                      }
                     
                     NextModule=1;
               break;  
               
               
              case W_NewIdRes :  //odpowiedŸ/potwierdzenie nadania nowego ID
                     
                    TransQuality.Mensur[0]=0;
                    TransQuality.QuickMensur[0]=0;
                    TransQuality.SlowMensur[0]=0; 
                     //wyczyœæ zadania
                     for (char i=0; i<RF_Tasks-1; i++) 
                     {
                        if(RF_TaskTab[i].Command.Order==W_NewIdOrd)
                        {
                         RemoveRfTask(i);  
                        }
                     }
               
                     
                     NextModule=1;
               break;  

               
    
               case RW_BC_Res :  //OdpowiedŸ na ramkê rozg³oœn¹ "Kto ma wêze³"
              
                      NeuronsList[RF_TaskTab[RF_CurrentTask].Node-1].IndirectCon=1;
                      AddRfTask_RemoveSearchNodeTask(RF_TaskTab[RF_CurrentTask].Node);  
                     
                      
               break;
               
               
               
               
               
               
      
    }
    
             
            //Wyœlij odpowiedŸ
            if (ByteToSend)
            {
                RF_Status.EnableTX();
                 RF_SendData(CommandToSend.Streem, ByteToSend);
                RF_Status.EnableRX();
               
            }           
   
            //odpytaj kolejny modu³
            if (NextModule)
            {
               g_RF_TimeOutCounter=RF_TimeOut; 
               RF_MasterExecut();
              /*
              if (RF_Status.MasterMode)
                {
                  //if ((!SlaveMode) && (g_Stop_Program==0))
                  if (g_RF_TimeOutCounter>0)
                  {
                  }else{
                     CyklPerSekCount++;
                     g_RF_TimeOutCounter=RF_TimeOut; 
                     RF_MasterExecut();
              
                  }
                }
              */
            }
  
  
}
//_____________________________________________________________________________









//-----------------------------------------------------------------------------
//!  Nazwa funkcji :    RF_SendData
//!  Funkcja wysy³a dane przez RF12
//-----------------------------------------------------------------------------
//  *data - wska¿nik na pierwszy bajt
//  *size - iloœæ bajtów do wys³ania
//-----------------------------------------------------------------------------
void RF_SendData(unsigned char *data, char size)
{
    RF_SendFarm(data, size);
}
//------------------------------------------------------------------------------

extern  AT91_SPI_Buff               SPI_Buff;



char IncCurentRfTask(void)
{
    static char Switch=0;
    static char WaitToSwithToSlave=0;

    char RF_CurrentTaskTMP=RF_CurrentTask;
  
    char Fuss=RF_Tasks+10; 
    do{ 
            if (Fuss==0) break;
            RF_CurrentTaskTMP++;  
            if ((RF_TaskTab[RF_CurrentTaskTMP].Node==0) || (RF_CurrentTaskTMP>=RF_Tasks))  
            {
              RF_CurrentTaskTMP=0; 
              
              if ((AT91C_IFLASH_MEM->Config.SlaveMode) && (Switch) && (RF_Status.TimeInMasterMode>200))  //zagwarantowane minimum 110ms w trybie master
              {
                if (WaitToSwithToSlave<2)
                {
                  WaitToSwithToSlave++; 
                }
                else
                {
                  WaitToSwithToSlave=0;
                  Switch=0;
                  SwitchToSlave();
                }
                return 0;
              }else{
                Switch=1;
                
              }
              
            }
            Fuss--;
       }while ((NeuronsList[RF_TaskTab[RF_CurrentTaskTMP].Node-1].ReadSuspend) && ((RF_TaskTab[RF_CurrentTaskTMP].Command.Order!=R_BC_Order) && (RF_TaskTab[RF_CurrentTaskTMP].Command.Order!=W_BC_Order)));
        
    if (RF_CurrentTaskTMP==0) CyklPerSekCount++;
    RF_CurrentTask=RF_CurrentTaskTMP;
    return 1;
      

}

#define SpeedExePeriods 0


//-----------------------------------------------------------------------------
//!  Nazwa funkcji :    RF_MasterExecut
//!  Funkcja wysy³a dane wed³ug tablicy rozkazów RF_TaskTab
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void RF_MasterExecut(void)
{

  
  char ByteToSend=0;
  //char TaskNr;
  
  unsigned int SendOrder=0;    //flaga wys³ania
 
  if (SlaveMode) return;


  
  //wyszukanie rozkazów priorytetowych
  char Nd=0;
  unsigned char Tn=0;
  static signed char  RF_RememberTask=-1;
  
  
  signed char TMP_RF_SpecialTask=-1;
  
  while ((Nd<NumberNeurons) && (TMP_RF_SpecialTask==-1))
  {
      if ((MainStage.SendStageChange[Nd]>=ChangeStageFlag-SpeedExePeriods))    
      {
        
        Tn=0;
        while (((RF_TaskTab[Tn].Node!=0) && (Tn<RF_Tasks-1) && (TMP_RF_SpecialTask==-1)))
        {
           if ((RF_TaskTab[Tn].Command.Order==W_RegOrd) &&  (RF_TaskTab[Tn].Command.Auxi==0) && (RF_TaskTab[Tn].Node==Nd)) 
           {
               TMP_RF_SpecialTask=Tn;
               SendOrder=1;
               MainStage.SendStageChange[Nd]--;
           }
           Tn++;
        }
        
      }
      
      
      Nd++;  
  }  
  
  RF_SpecialTask=TMP_RF_SpecialTask;
  
  if (RF_SpecialTask>=0)
  {
    RF_RememberTask=RF_CurrentTask;
    RF_CurrentTask=RF_SpecialTask;


                          
              
  }else{
    if (RF_RememberTask>=0) 
    {
     RF_CurrentTask=RF_RememberTask;
     RF_RememberTask=-1;  
    }
    
    if (IncCurentRfTask()==0) return;

   

  
    
    //wykluczenie rozkazów ustawiania stanów oraz odczytu rejestrów je¿eli brak transmisji
    while (((RF_TaskTab[RF_CurrentTask].Node!=0) && (RF_CurrentTask<RF_Tasks-1)) && (SendOrder==0))
    {
       if (((RF_TaskTab[RF_CurrentTask].Command.Order==W_RegOrd) &&  ((RF_TaskTab[RF_CurrentTask].Command.Auxi==0) && (MainStage.SendStageChange[RF_TaskTab[RF_CurrentTask].Node]==0) && (NeuronsList[RF_TaskTab[RF_CurrentTask].Node-1].NeuronType!=NodeRF_Period) && (NeuronsList[RF_TaskTab[RF_CurrentTask].Node-1].NeuronType!=NodeRF_Remote) ))  ||
          ((RF_TaskTab[RF_CurrentTask].Command.Order==R_RegOrd) &&  (RF_TaskTab[RF_CurrentTask].Command.Auxi>0) && (TransQuality.QuickMensur[RF_TaskTab[RF_CurrentTask].Node-1]==0)))
       {
             if (IncCurentRfTask()==0) return;   
         
       }else{ 
          if ((RF_TaskTab[RF_CurrentTask].Command.Order==W_RegOrd) &&  (RF_TaskTab[RF_CurrentTask].Command.Auxi==0) && ((MainStage.SendStageChange[RF_TaskTab[RF_CurrentTask].Node]>0)|| (NeuronsList[RF_TaskTab[RF_CurrentTask].Node-1].NeuronType==NodeRF_Remote)))  MainStage.SendStageChange[RF_TaskTab[RF_CurrentTask].Node]--;
          SendOrder=1;   
       }
    }
  }
  
  
  if (RF_Status.MasterMode==0) return;

     
  
  if (SendOrder)
  {
    
    
      _RF_Command  RF_Command;
      RF_Command.ID     = RF_TaskTab[RF_CurrentTask].Command.ID;
      RF_Command.Order  = RF_TaskTab[RF_CurrentTask].Command.Order;
      RF_Command.Auxi   = RF_TaskTab[RF_CurrentTask].Command.Auxi;
      

      if (RF_TaskTab[RF_CurrentTask].Command.Order==W_NewIdOrd)                 //rozkaz specjalny zmiany ID
      {
         ByteToSend=8;
         RF_Command.CHID_CUR_ID=NeuronsList[0].NeuronID;
         RF_Command.CHID_KODE=0xC;
         RF_Command.CHID_NEW_ID=NeuronsList[1].NeuronID;
         RF_Command.CHID_CRC=crc16(RF_Command.Streem,6);
      }
      else if ((RF_TaskTab[RF_CurrentTask].Command.Order==R_BC_Order)   || (RF_TaskTab[RF_CurrentTask].Command.Order==W_BC_Order))           //rozkaz specjalny ramka rozg³oœna
      {
         ByteToSend=8;
         RF_Command.BC_ID1=0;
         RF_Command.BC_REG=RF_TaskTab[RF_CurrentTask].Command.Auxi;
         RF_Command.BC_ID2=NeuronsList[RF_TaskTab[RF_CurrentTask].Node-1].NeuronID;
         RF_Command.BC_CRC=crc16(RF_Command.Streem,6);
      
      }
      else if (RF_TaskTab[RF_CurrentTask].Command.Order==R_RegOrd) 
      {
          ByteToSend=6;
          RF_Command.B6.CRC = crc16(RF_Command.Streem,4);

          
      }else if (RF_TaskTab[RF_CurrentTask].Command.Order==W_RegOrd)
      {  
          ByteToSend=8;
          RF_Command.B8.Val = MainStage.StageQ[RF_TaskTab[RF_CurrentTask].Node].Value[RF_TaskTab[RF_CurrentTask].Command.Auxi];
          RF_Command.B8.CRC = crc16(RF_Command.Streem,6);
          
          #ifdef _DEBUG_
             AT91C_BASE_PIOA->PIO_SODR =SpeakerPin;  
             ButtonBipCounter=g_counter+1;       
          #endif
    
      }
    
       #ifdef  DIRECT_Q_MESURE
                     TransQuality.DirectkMensur[RF_TaskTab[RF_CurrentTask].Node-1]++; 
       #endif
      
      
      if (RF_NodeTest.NodeNr==RF_TaskTab[RF_CurrentTask].Node)
      {
        RF_NodeTest.Sent++; 
      }
      
      
      
      //RF_Status.EnableTX();
      RF_SendData(RF_Command.Streem, ByteToSend);
      //RF_Status.EnableRX();
      

     
      

      
      
    
  }
  
  static int RF_LastPerTime;
    if (RF_CurrentTask==0){
      RF_Period=g_counter-RF_LastPerTime;
      RF_LastPerTime=g_counter;
    }
  


         #ifdef _LIVE_RJESTR  
     if (RF_TaskTab[RF_CurrentTask].Command.Order==R_RegOrd) 
      {
         LIVE_TAB[LIVE_TAB_IDX]=0xA000 | (((RF_TaskTab[RF_CurrentTask].Node)&0xF)<<8) | ((MainStage.StageQ[RF_TaskTab[RF_CurrentTask].Node].Port)&0xFF);
         if (LIVE_TAB_IDX<LIVE_TAB_SIZE) LIVE_TAB_IDX++; else LIVE_TAB_IDX=0;

      }else if (RF_TaskTab[RF_CurrentTask].Command.Order==W_RegOrd)  
      {  
         LIVE_TAB[LIVE_TAB_IDX]=0xC000 | (((RF_TaskTab[RF_CurrentTask].Node)&0xF)<<8) | ((MainStage.StageQ[RF_TaskTab[RF_CurrentTask].Node].Port)&0xFF);
         if (LIVE_TAB_IDX<LIVE_TAB_SIZE) LIVE_TAB_IDX++; else LIVE_TAB_IDX=0;
      }
     #endif

  
}


