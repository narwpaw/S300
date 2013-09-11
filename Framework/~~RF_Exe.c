//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                                 CEURON                                   *//                       
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : RF_Exe.c                                 *//
//*  Opis                         : Plik wykonawczy komunikacji radiowej     *//
//*  Autor                        : Pawe� Narwojsz                           *//
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

//


#include "RF_Exe.h"
#include "Application_prog.h" 


//tymczasowe
extern volatile char SlaveMode;
extern long long int g_counter;
unsigned int        RF_Period;                                              //cykl transmisji
short int SymulacjaRejWej=0xAB;
short int SymulacjaRejWyj;
char IndWys=0; 
char IndWysNeg=1; 
int odb[2];
int licznik_cykli=0;
extern unsigned int CyklPerSekCount; 
extern unsigned long long g_NextTimeProgram; 
extern volatile char Busy_program;
extern _NeuronsListUnit NeuronsList[NumberNeurons];
//tymczasowo koniec-----------




extern volatile char SlaveMode;
extern long long int g_counter;
extern unsigned int CyklPerSekCount; 
extern unsigned long long g_NextTimeProgram; 
extern volatile char Busy_program;
extern _NeuronsListUnit NeuronsList[NumberNeurons];
extern unsigned int CyklPerSek;


#ifdef _LIVE_RJESTR
extern unsigned short LIVE_TAB[LIVE_TAB_SIZE];
extern char LIVE_TAB_IDX;
#endif


//Zmienne globalne -------------------------------------------------------------
  
  _RF_TaskTab  RF_TaskTab;
  signed char           RF_CurrentTask=-1; 
  char g_RF_TimeOutCounter =0;  //bierz�cy czas odliczania do timeout`u
  
//___________________________Koniec zmiennych globalnych________________________


  
  
//Zmienne globalne importowane--------------------------------------------------  
  extern _RF_Status   RF_Status;
  extern _MainStage  MainStage;
  extern _TransQuality TransQuality;
//___________________________Koniec zmiennych globalnych importowanych_________





//-----------------------------------------------------------------------------
//!  Nazwa funkcji :    RF_ExecuteOrder
//!  Funkcja interpretuje i realizuje otrzymany rozkaz
//-----------------------------------------------------------------------------
//  Res   - wska�nik na bufor danych
//  Size  - wielko�� bufora danych
//-----------------------------------------------------------------------------
  _RF_Command  *pRF_Command;
void RF_ReciveFarm(char *Buff, char Size)
{
        

          pRF_Command=(_pRF_Command)(Buff); //rzutowanie na struktur� rozkazu
        
          if (
              ((Size==6) && (pRF_Command->B6.CRC == crc16(pRF_Command->Streem, Size-2))) || 
              ((Size==8) && (pRF_Command->B8.CRC == crc16(pRF_Command->Streem, Size-2)))
             )
          {  
            
          //Sprawdzenie zgodno�ci ID 
            
          if(((pRF_Command->ID==RF_TaskTab[RF_CurrentTask].Command.ID) && (!SlaveMode)) ||
             ((pRF_Command->ID==RF_ID) && (SlaveMode))
             ) 
          {
            if(RF_Status.ConfResive==1) RF_Status.ConfResive=2;  //znacznik odebrania danych (dla kontroli kana�u)
            RF_ExecuteOrder(pRF_Command);   //realizacja rozkazu         
          }  



          }
  
  

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
               
              
   _RF_Command  CommandToSend;
   
   //cz�� wsp�lna wszystkich odpowiedzi
   CommandToSend.ID     = RF_ID;
   CommandToSend.Auxi   = Command->Auxi;
   
       
    char ByteToSend=0;          
    char NextModule=0;
    switch (Command->Order)
    {  
     
       //Rozkazy dla Slave ------------------------------------------------------------
      
            case R_RegOrd :  //Odczyt pojedy�czego rejestru - rozkaz
                     //wykonanie ------
                     CommandToSend.B8.Val  =  SymulacjaRejWej;                     // przepisz dane z rejestru nr: Command.Auxi  (tymczasowo  SymulacjaRej)
     
                     //odpowied� ------
                     CommandToSend.Order  = R_RegRes;
                     CommandToSend.B8.CRC = crc16(CommandToSend.Streem,6);    
                     ByteToSend =8;
                 break;
                 
            case W_RegOrd :  //Zapis pojedy�czego rejestru  -rozkaz
                     //wykonanie ------
                     SymulacjaRejWyj=Command->B8.Val;                                    //Ustaw rejestr nr: Command.Auxi 
                     //odpowied� ------
                     CommandToSend.Order  = W_RegRes;
                     CommandToSend.B6.CRC = crc16(CommandToSend.Streem,4);    
                     ByteToSend =6;
                 break;

                 
         //Odpowiedzi rozkaz�w od slave do sterownika ------------------------------------------------------------   
            
            case R_RegRes :  //Odczyt pojedy�czego rejestru - odp
                     //wykonanie ------
                     //CommandToSend.B8.Val  =  0xCC;                            // przepisz dane z rejestru nr: Command.Auxi  (tymczasowo  0xCC)

#ifdef _LIVE_RJESTR  
                   LIVE_TAB[LIVE_TAB_IDX]=0xB000 | (((RF_TaskTab[RF_CurrentTask].Node)&0xF)<<8) | ((Command->B8.Val)&0xFF);
                   if (LIVE_TAB_IDX<LIVE_TAB_SIZE) LIVE_TAB_IDX++; else LIVE_TAB_IDX=0;
#endif              
              
                    //przy�pieszenie wykonania programu
                    if ((RF_TaskTab[RF_CurrentTask].Command.Auxi==0) && (MainStage.StageI[RF_TaskTab[RF_CurrentTask].Node].Value[RF_TaskTab[RF_CurrentTask].Command.Auxi]!=Command->B8.Val) && (Busy_program==0))
                    {   
                      if (g_NextTimeProgram>3) g_NextTimeProgram=g_counter+3;
                      
                 
                       //sygna� dzwi�kowy je�eli odczytano zmian� stanu na 1
                       if ((Command->B8.Val>MainStage.StageI[RF_TaskTab[RF_CurrentTask].Node].Value[RF_TaskTab[RF_CurrentTask].Command.Auxi]) && (NeuronsList[RF_TaskTab[RF_CurrentTask].Node-1].NeuronType==NodeRF_Remote) && (!(AT91C_IFLASH_MEM->Config.SoundOff)))
                       {
                          AT91C_BASE_PIOA->PIO_SODR =SpeakerPin;  
                          ButtonBipCounter=g_counter+1;
                       }
   
                      
                      
                        #ifdef _LIVE_RJESTR    
                            LIVE_TAB[LIVE_TAB_IDX]=0xFFFF;
                          if (LIVE_TAB_IDX<LIVE_TAB_SIZE) LIVE_TAB_IDX++; else LIVE_TAB_IDX=0;
                      #endif   
                    
                    }
                    
                    
                      MainStage.StageI[RF_TaskTab[RF_CurrentTask].Node].Value[RF_TaskTab[RF_CurrentTask].Command.Auxi]=Command->B8.Val; 
                    
                    
                    
                    
                    
         
         
         
                     TransQuality.Mensur[RF_TaskTab[RF_CurrentTask].Node-1] |= 0x1 << TransQuality.MensurIndex;   //wpis do tablicy jako�ci
                     TransQuality.QuickMensur[RF_TaskTab[RF_CurrentTask].Node-1] |= 0x1 << TransQuality.QuickMensurIndex;   //wpis do tablicy jako�ci 
                     TransQuality.SlowMensur[RF_TaskTab[RF_CurrentTask].Node-1] |= 0x1 << TransQuality.SlowMensurIndex;   //wpis do tablicy jako�ci d�ugotrwa�ej
                     #ifdef  DIRECT_Q_MESURE
                     TransQuality.DirectkMensur[RF_TaskTab[RF_CurrentTask].Node-1]--; 
                     #endif
                     NextModule=1;
                 break;
                 
            case W_RegRes :  //Zapis pojedy�czego rejestru  -odp
                     //wykonanie ------
              
                   
                       MainStage.SendStageChange[RF_TaskTab[RF_CurrentTask].Node]=0;
                    
                   
                      if (RF_TaskTab[RF_CurrentTask].Node-1<=NumberNeurons)
                      {
                          TransQuality.Mensur[RF_TaskTab[RF_CurrentTask].Node-1] |= 0x1 << TransQuality.MensurIndex;   //wpis do tablicy jako�ci
                          TransQuality.QuickMensur[RF_TaskTab[RF_CurrentTask].Node-1] |= 0x1 << TransQuality.QuickMensurIndex;   //wpis do tablicy jako�ci 
                          TransQuality.SlowMensur[RF_TaskTab[RF_CurrentTask].Node-1] |= 0x1 << TransQuality.SlowMensurIndex;   //wpis do tablicy jako�ci d�ugotrwa�ej
                      }
                      
                      
                     #ifdef  DIRECT_Q_MESURE
                     TransQuality.DirectkMensur[RF_TaskTab[RF_CurrentTask].Node-1]--; 
                     #endif
                          
                     //g_RF_TimeOutCounter=0;
                     
                     NextModule=1;
                  #ifdef _LIVE_RJESTR            
                     LIVE_TAB[LIVE_TAB_IDX]=0xD000 | (((RF_TaskTab[RF_CurrentTask].Node)&0xF)<<8) | 0xFF;
                   if (LIVE_TAB_IDX<LIVE_TAB_SIZE) LIVE_TAB_IDX++; else LIVE_TAB_IDX=0;
                  #endif   
                   

               break;
      
    }
    
             
            //Wy�lij odpowied�
            if (ByteToSend)
            {
                RF_Status.EnableTX();
                 RF_SendData(CommandToSend.Streem, ByteToSend);
                RF_Status.EnableRX();
               
            }           
   
            //odpytaj kolejny modu�
            if (NextModule)
            {
               g_RF_TimeOutCounter=RF_TimeOut; 
               RF_MasterExecut();
               //g_RF_TimeOutCounter=0; 
            }else{
              
             g_RF_TimeOutCounter=0; 
            }  
  
  
}
//_____________________________________________________________________________






//-----------------------------------------------------------------------------
//!  Nazwa funkcji :    RF_SendData
//!  Funkcja wysy�a dane przez RF12
//-----------------------------------------------------------------------------
//  *data - wska�nik na pierwszy bajt
//  *size - ilo�� bajt�w do wys�ania
//-----------------------------------------------------------------------------
void RF_SendData(unsigned char *data, char size)
{
    RF_SendFarm(data, size);
}
//------------------------------------------------------------------------------



signed char RF_RememberTask=-1;


//-----------------------------------------------------------------------------
//!  Nazwa funkcji :    RF_MasterExecut
//!  Funkcja wysy�a dane wed�ug tablicy rozkaz�w RF_TaskTab
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void RF_MasterExecut(void)
{
  char ByteToSend=0;
  unsigned int SendOrder=0;    //flaga wys�ania
 

  
//wyszukanie rozkaz�w priorytetowych--------------------------------------------
//------------------------------------------------------------------------------


  
  //szukanie naj�wierzszego rozkazu �adania wys�ania stanu ---------------------
  char Nd=1;
  char SpecNd=0;                                                                //Nod z naj�wie�szym rozkazem wys�ania zmiany stanu. Je�eli 0 tz. brak �adania w �adnym z w�z��w
  char NCont=0;
  while (Nd<NumberNeurons)          
  {
      if ((MainStage.SendStageChange[Nd]>NCont))      //je�eli jest �adanie wys�ania stanu 
      {
          SpecNd=Nd;
          NCont=MainStage.SendStageChange[Nd];
      }
      Nd++;  
  }  
  //----------------------------------------------------------------------------
  
  
  
  
  
  //Znalezienie odpowiedniego zadania w tablicy RF_TaskTab je�eli istnieje rozkaz priorytetowy (nowy)-----------
  signed char RF_SpecialTask=-1;
  unsigned char Tn=0;
  if ((SpecNd>0) && (NCont==ChangeStageFlag))
  {
        Tn=0;
        while (((RF_TaskTab[Tn].Node!=0) && (Tn<RF_Tasks) && (RF_SpecialTask==-1)))
        {
           if ((RF_TaskTab[Tn].Command.Order==W_RegOrd) &&  (RF_TaskTab[Tn].Command.Auxi==0) && (RF_TaskTab[Tn].Node==SpecNd)) 
           {
               RF_SpecialTask=Tn;
               SendOrder=1;
           }
           Tn++;
        }
  }  
  //----------------------------------------------------------------------------      

  
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

  
  //je�eli istnieje rozkaz priorytetowy
  if (RF_SpecialTask>=0)
  {
    RF_RememberTask=RF_CurrentTask;                                             //zapami�taj bierz�ce zadanie
    RF_CurrentTask=RF_SpecialTask;                                              //jako bierzace zadanie wyznacz zadanie rozkazu priorytetowego
    MainStage.SendStageChange[SpecNd]--;                                        //zmniejszenie priorytetu wys�ania
    SendOrder=1; 
    
      AT91C_BASE_PIOA->PIO_SODR =SpeakerPin;  
                          ButtonBipCounter=g_counter+3;
    
  }
  else //normalny tryb odpytywania
  {
    if (RF_RememberTask>=0)                                                     //je�eli istnieje zapamietane zadanie to ustaw jako bierzace
    {
     RF_CurrentTask=RF_RememberTask;
     RF_RememberTask=-1;  
    }
    
    
    RF_CurrentTask++;                                                           //inkrementuj bierzce zadanie
    if ((RF_TaskTab[RF_CurrentTask].Node==0) || (RF_CurrentTask>=RF_Tasks))     //jezeli brak zadania lub przekroczono liczb� zada� to ustaw zadanie nr 0
    {
      RF_CurrentTask=0;
      CyklPerSekCount++;
    }    
    
    
    //wykluczenie rozkaz�w ustawiania stan�w oraz odczytu rejestr�w je�eli brak transmisji
    while (((RF_TaskTab[RF_CurrentTask].Node!=0) && (RF_CurrentTask<RF_Tasks)) && (SendOrder==0))
    {
       if (
           ( //
            (RF_TaskTab[RF_CurrentTask].Command.Order==W_RegOrd) &&             //je�eli rozkaz wys�ania stanu
            ((RF_TaskTab[RF_CurrentTask].Command.Auxi==0) &&                    //i rejestr nr 0
             (MainStage.SendStageChange[RF_TaskTab[RF_CurrentTask].Node]==0) && //i istnieje �adanie wys�ania stanu
             (NeuronsList[RF_TaskTab[RF_CurrentTask].Node-1].NeuronType!=NodeRF_Period) )//i nie jest to pilot (bo pilot musi mie� ca�y czas wysy�any stan kt�ry jest wy�wietlany na diodach)
            ) /* || ( //brak transmisji
             (RF_TaskTab[RF_CurrentTask].Command.Order==R_RegOrd) &&  
             (RF_TaskTab[RF_CurrentTask].Command.Auxi>0) && 
             (TransQuality.QuickMensur[RF_TaskTab[RF_CurrentTask].Node-1]==0)
            ) */
         ) 
       {//znajd� nowe zadanie je�eli obecne zosta�o wykluczone z powy�szych powod�w
          RF_CurrentTask++; 
          if ((RF_TaskTab[RF_CurrentTask].Node==0) || (RF_CurrentTask>=RF_Tasks))     //jezeli brak zadania lub przekroczono liczb� zada� to ustaw zadanie nr 0
          {
            RF_CurrentTask=0;
            CyklPerSekCount++;
          }
       }else{ 
          if ((RF_TaskTab[RF_CurrentTask].Command.Order==W_RegOrd) &&  (RF_TaskTab[RF_CurrentTask].Command.Auxi==0) && (MainStage.SendStageChange[RF_TaskTab[RF_CurrentTask].Node]>0))  MainStage.SendStageChange[RF_TaskTab[RF_CurrentTask].Node]--;    //zmniejszenie priorytetu wys�ania
          SendOrder=1;   
       }
    }
  }//koniec ustawiania normalnego trybu odpytywania 
 


  //wysy�anie zadania ----------------------------------------------------------
  if (SendOrder)
  {
      _RF_Command  RF_Command;
      RF_Command.ID     = RF_TaskTab[RF_CurrentTask].Command.ID;
      RF_Command.Order  = RF_TaskTab[RF_CurrentTask].Command.Order;
      RF_Command.Auxi   = RF_TaskTab[RF_CurrentTask].Command.Auxi;
      
      if (RF_TaskTab[RF_CurrentTask].Command.Order==R_RegOrd) 
      {
          ByteToSend=6;
          RF_Command.B6.CRC = crc16(RF_Command.Streem,4);
      }else if (RF_TaskTab[RF_CurrentTask].Command.Order==W_RegOrd)  
      {  
          ByteToSend=8;
          RF_Command.B8.Val = MainStage.StageQ[RF_TaskTab[RF_CurrentTask].Node].Value[RF_TaskTab[RF_CurrentTask].Command.Auxi];
          RF_Command.B8.CRC = crc16(RF_Command.Streem,6);
      }

      //RF_Status.EnableTX();
      RF_SendData(RF_Command.Streem, ByteToSend);
      //RF_Status.EnableRX();
      
      #ifdef  DIRECT_Q_MESURE 
            TransQuality.DirectkMensur[RF_TaskTab[RF_CurrentTask].Node-1]++;
      #endif
      }
  //----------------------------------------------------------------------------

  


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


