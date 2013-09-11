//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                 IASE Microcontroller Software Support                    *//                       
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : RS_WIN.c                                 *//
//*  Opis                         : Plik komunikacji z programem RS-win      *//
//*  Autor                        : Pawe³ Narwojsz                           *//
//*  Data utrzorzenia             : 09.09.2011                               *//
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



#include <intrinsics.h>
#include "core/common.h"
#include "core/device.h"
#include "AT91SAM7S256.h"
#include "core/trace.h"
#include "flash.h"
#include "RS_WIN.h"
#include "Framework.h"

     


//Zmienne zewnêtrzne -----------------------------------------------------------
    extern char g_czas_petli;
    extern const char identyfikacja[];
    extern const char MODULE_NAME[];
    extern const char SOFT_VERSION[];
    //extern _PROGREG g_PROGREG;
    extern char g_Stop_Program;                                                     //1- zatrzymuje program
    extern char g_Restart_Program;                                                  //1- restartuje program
//    extern volatile char *gUserProgram;                                                      //WskaŸnik na bierz¹cy program u¿ytkownika we flashu. Po restarcie gUserProgram=gUserProgram
//    extern volatile char *gUserProgramNew;
//    char   *gUserProgramNext;                                                    //obszar awaryjny
    extern  char SYSTEM_NAME[];
    extern char ChangeSetup=0;                                                  //flaga zmiany ustawieñ
    RAM ram;
//_______________Koniec globalnych zmiennych zewnêtrznych ______________________


    char NewSystem=0;
    
//Zmienne zewnêtrzne -----------------------------------------------------------

//*_____________________________________________________________________________    

        

__ramfunc void NewSystemSave(void)    
{
  /*
    CleanFlash((char*)0x100000, UseProgramSize);
    FlashWrite((char*)0x100000,(char*)gUserProgramNew,UseProgramSize);   
    CleanFlash((char*)(0x100000+UseProgramSize),UseProgramSize);
    FlashWrite((char*)(0x100000+UseProgramSize),(char*)gUserProgramNext,UseProgramSize); 
    
   */ 
}


        
//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    RsWin
//* Interpretacja i wykonanie rozkazów wysy³anych z programu RS-win
//* Rx-dane odebrane
//* Rx-dane do wys³ania
//* Funkcja zwraca iloœæ bajtów do wys³ania
//*-----------------------------------------------------------------------------
char *RxTmp;
short int komunikcja_RSwin(_pRsWinResive Rx, char *Tx)
{
  RxTmp=(char*)Rx;
  
  //if (SuspendRsWinKomunication>g_counter) return;  //chwilowe wstrzymanie transmisji jako znacznik restartu dla RsWin
  
  
  char ByteToSend=0;
  static unsigned int LoadingNewPgog=0;
  static unsigned int Suspend=0;
  union{
       unsigned int u;
       char         b[4];
     }war;
  int Vart;
  
  
  //odczyt/zapis ram oraz rozkazy specjalne
  if (Rx->Section==RsWin_RamSection)
  {
    switch (Rx->Order)
    {
        case RsWin_OrderRead:
             ByteToSend=RsWin_ReadRam(Rx->RamReadWrite.Adress, Rx->RamReadWrite.Length, Tx);  
        break;
        
        case RsWin_OrderWrite:
             ByteToSend=RsWin_WriteRam(Rx->RamReadWrite.Adress, Rx->RamReadWrite.Length, &(Rx->RamReadWrite.Data), Tx);
        break;
        
        case RsWin_OrderConfigBinGroup:
             ByteToSend=RsWin_ConfigBinGroup(Rx->ConfigGroup.Kode, Rx->ConfigGroup.Length, &(Rx->ConfigGroup.VarSymbol), Tx);
        break;
        case RsWin_OrderConfigAnaGroup:
             ByteToSend=RsWin_ConfigAnaGroup(Rx->ConfigGroup.Kode, Rx->ConfigGroup.Length, &(Rx->ConfigGroup.VarSymbol), Tx);
        break;
        
        case RsWin_OrderRestart:
/*                
                if (LoadingNewPgog)
                {
                     
                     g_PROGREG.Stop_Program=1;
                     g_PROGREG.NewProgramStart=1;
                     g_PROGREG.Start_cykl=1;
                }else{
                     g_PROGREG.InitUserPrg=1; 
                }
*/                
                Tx[0]=0xAC;
                Tx[1]=0x02; 
                Tx[2]=RsWin_SK(Tx, 2);    
                ByteToSend=3;
                Suspend=2;
                LoadingNewPgog=0;
        break;
        
        case RsWin_OrderRemember:
 /*         
                 Vart=(int)(gUserProgram);
                  unsigned int Save = __get_interrupt_state();            
                __disable_interrupt();
                    //  FlashWrite((char*)&FLASH->pt_UserProgram,(char*)(&Vart),4); 
                __set_interrupt_state(Save);
                __enable_interrupt();  
                Tx[0]=0xAC;
                Tx[1]=0x02; 
                Tx[2]=RsWin_SK(Tx, 2);   
                LoadingNewPgog=0;
                ByteToSend=3;
                Suspend=2;
                SaveLog(LogSaveNewProgram);//wpis do rejestru "LogWriteNewProgram" 
*/ 
        break;
        case RsWin_NewSystem:
          {
                    unsigned int Save = __get_interrupt_state();            
                    __disable_interrupt();

                    NewSystemSave();
                    NewSystem=0;
                    
                    __set_interrupt_state(Save);
                    __enable_interrupt();  
                   
                     //restart
                    AT91C_BASE_RSTC->RSTC_RCR = AT91C_RSTC_PROCRST | AT91C_RSTC_PERRST | AT91C_RSTC_KEY;
                    
            }
        break;
    };
    
  }
  //odczyt zapis ram/flash
  else if (Rx->Section==RsWin_RamOrFlash)
  {  
    switch (Rx->Order)
    {
        case RsWin_OrderRead:
             if (Rx->ReadWrite.Section==0x0000)  {ByteToSend=RsWin_ReadRam(Rx->ReadWrite.Adress, Rx->ReadWrite.Length, Tx);  }
 //            if ((Rx->ReadWrite.Section==0x1000) &&(LoadingNewPgog==0))  ByteToSend=RsWin_ReadUserProgram(Rx->ReadWrite.Adress, Rx->ReadWrite.Length, Tx,(char*)gUserProgram);
 //            if ((Rx->ReadWrite.Section==0x1000) &&(LoadingNewPgog==1))  ByteToSend=RsWin_ReadUserProgram(Rx->ReadWrite.Adress, Rx->ReadWrite.Length, Tx,(char*)gUserProgramNew);
             if ((Rx->ReadWrite.Section==0x6000) && (Suspend==0))  {ByteToSend=RsWin_CurentProgramPlace(Rx->ReadWrite.Adress,Tx);   }  else {if(Suspend>0) Suspend--;}
             if (Rx->ReadWrite.Section==0x5000)  {ByteToSend=0;}
 //            if (Rx->ReadWrite.Section==0x9000)  {ByteToSend=RsWin_ReadUserProgram(Rx->ReadWrite.Adress, Rx->ReadWrite.Length, Tx,(char*)gUserProgram);  }
             if (Rx->ReadWrite.Section==0xD000)  {ByteToSend=RsWin_ReadSystem(Rx->ReadWriteSys.Adress, Rx->ReadWriteSys.Length, Tx,(char*)0x100000);  }        //odczyt systemu
        break;
        
        case RsWin_OrderWrite:
             if (Rx->ReadWrite.Section==0x0000)  {ByteToSend=RsWin_WriteRam(Rx->ReadWrite.Adress, Rx->ReadWrite.Length, &(Rx->ReadWrite.Data), Tx); }
             if (Rx->ReadWrite.Section==0x1000)  {ByteToSend=RsWin_WriteUserProgram(Rx->ReadWrite.Adress, Rx->ReadWrite.Length, &(Rx->ReadWrite.Data), Tx); LoadingNewPgog=1;}
             if (Rx->ReadWrite.Section==0xD000)  {ByteToSend=RsWin_WriteSystem(Rx->ReadWriteSys.Adress, Rx->ReadWriteSys.Length, &(Rx->ReadWriteSys.Data), Tx); LoadingNewPgog=1;}  //Zapis systemu
        break;
    }    
    
    
  }
  
  return  ByteToSend;
}
//*____________________________________________________________________________         
              
        
        
        
        
//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ReadUserProgram
//* Funkcja odczytu programu urzytkowego
//* Tx-dane do wys³ania
//* Funkcja zwraca iloœæ bajtów do wys³ania
//*-----------------------------------------------------------------------------
short int RsWin_ReadUserProgram(unsigned short Adress, unsigned char Length, char *Tx, char* Memory)
{       
        char *pTx=Tx;
        char ByteToSend=0;

        #ifdef MSU 
          
          char *pMem=Memory+Adress;
          *pTx=0xAC; pTx++;
          *pTx=0x04; pTx++;
             
          for (char k=0; k<Length; k++) {*pTx=*pMem; pMem++; pTx++;}
          ByteToSend=pTx-Tx;
          *pTx=RsWin_SK(Tx, ByteToSend++);
          
          
          
          
        #else
        if (Adress==0x0040) 
        {
          char *pMem;
          *pTx=0xAC; pTx++;
          *pTx=0x04; pTx++;
          *pTx=0xEA; pTx++;
          pMem=(char*)MODULE_NAME;
          while ((*pMem!=' ') && (*pMem!=0)) {*pTx=*pMem; pMem++; pTx++;}
          *pTx=' '; pTx++; *pTx='v'; pTx++; 
          pMem=(char*)SOFT_VERSION;
          while ((*pMem!=' ') && (*pMem!=0)) {*pTx=*pMem; pMem++; pTx++;}
          pTx++; *pTx=0;  *pTx=0xEA; pTx++; 
          ByteToSend=pTx-Tx;
          *pTx=RsWin_SK(Tx, ByteToSend++);
        }
        #endif

        
        return ByteToSend;
}//*____________________________________________________________________________         




//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ReadUserProgram
//* Funkcja odczytu programu urzytkowego
//* Tx-dane do wys³ania
//* Funkcja zwraca iloœæ bajtów do wys³ania
//*-----------------------------------------------------------------------------
short int RsWin_ReadSystem(unsigned long Adress, unsigned char Length, char *Tx, char* Memory)
{       
        char *pTx=Tx;
        char ByteToSend=0;

          
          char *pMem=Memory+Adress;
          *pTx=0xAC; pTx++;
          *pTx=0x04; pTx++;
             
          for (char k=0; k<Length; k++) {*pTx=*pMem; pMem++; pTx++;}
          ByteToSend=pTx-Tx;
          *pTx=RsWin_SK(Tx, ByteToSend);
          ByteToSend=ByteToSend+1;
          
        
        return ByteToSend;
}//*____________________________________________________________________________         




        
//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ReadUserProgram
//* Funkcja zapisu programu urzytkowego
//* Tx-dane do wys³ania
//* Funkcja zwraca iloœæ bajtów do wys³ania
//*-----------------------------------------------------------------------------
short int RsWin_WriteUserProgram(unsigned short Adress, unsigned char Length, char *Data, char* Tx)
{    
  /*
        unsigned short Adress1=0; 
        unsigned char Length1=0;
        unsigned short Adress2=0; 
        unsigned char Length2=0;
  
        if (Adress+Length<UseProgramSize*2) 
        {
          if (Adress+Length<UseProgramSize) 
          {
             Adress1=Adress;
             Length1=Length;
          }
          else if (Adress>=UseProgramSize) 
          {
             Adress2=Adress-UseProgramSize;
             Length2=Length;
          }else{
             Adress1=Adress;
             Length1=UseProgramSize-Adress;
             Adress2=0;
             Length2=Length-Length1;
          }
          
          char UP=0;
          char UPN=0;
          //wyznaczenie kolejnego obszaru do zapisu je¿eli wgrywany jest system
          if ((Length2>0) && (Adress2==0))
          {
            
            while ((UP<2) && ((int)FLASH->pt_UserProgram!=(int)(&FLASH->UserProgram[UP]))) {UP++;}
            while ((UPN<2) && (gUserProgramNew!=FLASH->UserProgram[UPN])) {UPN++;}
            
            for (char i=0; i<3; i++)
            {
              if ((i!=UP) && (i!=UPN))  gUserProgramNext=FLASH->UserProgram[i];
            }
            
            NewSystem=1;
          }
          
            
          unsigned int Save = __get_interrupt_state();            
          __disable_interrupt();
              if ((Adress1==0) && (Length1>0))  CleanFlash((char*)gUserProgramNew,(int)UseProgramSize);
              FlashWrite((char*)gUserProgramNew+Adress1,Data,Length1);  
              
              if ((Adress2==0) && (Length2>0))  CleanFlash((char*)gUserProgramNext,(int)UseProgramSize);
              FlashWrite((char*)gUserProgramNext+Adress2,Data+Length1,Length2);  
              
          __set_interrupt_state(Save);
          __enable_interrupt();  
  
          Tx[0]=0xAC;
          Tx[1]=0x02; 
          Tx[2]=RsWin_SK(Tx, 2);    
  
          if (Adress==0) SaveLog(LogWriteNewProgram);//wpis do rejestru "LogWriteNewProgram" 
          
  
          return 3;
        }
  */
}//*____________________________________________________________________________         




//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ReadUserProgram
//* Funkcja zapisu programu urzytkowego
//* Tx-dane do wys³ania
//* Funkcja zwraca iloœæ bajtów do wys³ania
//*-----------------------------------------------------------------------------
short int RsWin_WriteSystem(unsigned long Adress, unsigned char Length, char *Data, char* Tx)
{       
     /*
       unsigned long Adress1=0; 
        unsigned char Length1=0;
        unsigned long Adress2=0; 
        unsigned char Length2=0;
  
        if (Adress+Length<UseProgramSize*2) 
        {
          if (Adress+Length<UseProgramSize) 
          {
             Adress1=Adress;
             Length1=Length;
          }else if (Adress>=UseProgramSize) 
          {
             Adress2=Adress-UseProgramSize;
             Length2=Length;
          }else{
             Adress1=Adress;
             Length1=UseProgramSize-Adress;
             Adress2=0;
             Length2=Length-Length1;
          }
          
          char UP=0;
          char UPN=0;
          //wyznaczenie kolejnego obszaru do zapisu je¿eli wgrywany jest system
          if ((Length2>0) && (Adress2==0))
          {
            
            while ((UP<2) && ((int)FLASH->pt_UserProgram!=(int)(&FLASH->UserProgram[UP]))) {UP++;}
            while ((UPN<2) && (gUserProgramNew!=FLASH->UserProgram[UPN])) {UPN++;}
            
            for (char i=0; i<3; i++)
            {
              if ((i!=UP) && (i!=UPN))  gUserProgramNext=FLASH->UserProgram[i];
            }
            
            NewSystem=1;
          }
          
            
          unsigned int Save = __get_interrupt_state();            
          __disable_interrupt();
              if ((Adress1==0) && (Length1>0))  CleanFlash((char*)gUserProgramNew,(int)UseProgramSize);
              FlashWrite((char*)gUserProgramNew+Adress1,Data,Length1);  
              
              if ((Adress2==0) && (Length2>0))  CleanFlash((char*)gUserProgramNext,(int)UseProgramSize);
              FlashWrite((char*)gUserProgramNext+Adress2,Data+Length1,Length2);  
              
          __set_interrupt_state(Save);
          __enable_interrupt();  
  
          Tx[0]=0xAC;
          Tx[1]=0x02; 
          Tx[2]=RsWin_SK(Tx, 2);    
  
          if (Adress==0) SaveLog(LogWriteNewProgram);//wpis do rejestru "LogWriteNewProgram" 
          
          return 3;
        }
  */
}//*____________________________________________________________________________         






//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    CurentProgramPlace
//* Funkcja odczytu informacji o Ÿródle programu //0=brak, 1-program zapisany, 2-program nowo wgrany
//* Tx-dane do wys³ania
//* Funkcja zwraca iloœæ bajtów do wys³ania
//*-----------------------------------------------------------------------------
short int RsWin_CurentProgramPlace(unsigned short Adress, char *Tx)
{           
        char *pTx=Tx;   
        char ByteToSend;
        *pTx=0xAC; pTx++;
        *pTx=0x04; pTx++;
 /*       
        if ((int)FLASH->pt_UserProgram==(int)gUserProgram) {*pTx=1;
        } else {*pTx=2;} 
         pTx++; 
*/        
        ByteToSend=pTx-Tx;
        *pTx=RsWin_SK(Tx, ByteToSend++);
        return ByteToSend;
}//*____________________________________________________________________________         






//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ReadRam
//* Funkcja odczytu ramu
//* Tx-dane do wys³ania
//* Funkcja zwraca iloœæ bajtów do wys³ania
//*-----------------------------------------------------------------------------
short int RsWin_ReadRam(unsigned short  Adress,unsigned char Length, char *Tx)
{       
  /*
        char *pTx=Tx;
        char Fok=1; //flaga poprawnoœci
        char *pMem;
        unsigned int *pMemDW;
        char ByteToSend=0;
        short CzasPetliPU;  //Szybkoœæ pêtli programu u¿ytkowego /10

        if ((Adress>=0xB000) && (Adress<0xB0FF))
        {//odczyt zmiennych analogowych z pamiêci zbioru analogów
          short Adr=Adress -0xB000;
          
              *pTx=0xAC; pTx++;
              *pTx=0x04; pTx++;
          
              for (char k=Adr; k<Length+Adr;)  
              {
                if (k<2)
                {
                    *pTx=ram.PZA[k];     pTx++;
                    k++;
                }else{  
                    pMem=(char*)(&(ram.AnaVarRAM[(ram.PZA[(k/4)+2]&0xFFF)]));
                    for (char j=0; j<4; j++)
                    {
                      *pTx=*pMem;
                      pTx++;
                      pMem++;
                      
                    }
                    k+=4;
                }
              
              }
        
             
              ByteToSend=pTx-Tx;
              *pTx=RsWin_SK(Tx, ByteToSend++);
          Fok=0; 
        }         
        
        else if ((Adress>=0xB400) && (Adress<0xB4FF))
        {//odczyt zmiennych dwustanowych z pamiêci zbioru dwustanów
          short Adr=Adress -0xB400;
              *pTx=0xAC; pTx++;
              *pTx=0x04; pTx++;

              for (char k=Adr; k<Length+Adr; k++)  
              {
                if (k<2)
                {
                    *pTx=ram.PZB[k];     pTx++;
                }else{  
                    *pTx=BinToMaster(&ram.BinVarRAM[(ram.PZB[k]&0xFFF)]);
                    pTx++;
                }
              }
              
              ByteToSend=pTx-Tx;
              *pTx=RsWin_SK(Tx, ByteToSend++);
          Fok=0; 
        }        
        else if ((Adress>=0xC000) && (Adress<=0xC000+BinVarRAM_Size) && (Adress<0xC800))
        {//odczyt zmiennych dwustanowych
          pMem=ram.BinVarRAM+(Adress -0xC000); 
          
              *pTx=0xAC; pTx++;
              *pTx=0x04; pTx++;
           
              for (char k=0; k<Length; k++)  {*pTx=BinToMaster(pMem); pMem++; pTx++;}
              ByteToSend=pTx-Tx;
              *pTx=RsWin_SK(Tx, ByteToSend++);
          Fok=0; 
        }
        else if ((Adress>=0xC800) && (Adress<=0xC800+(AnaVarRAM_Size*4)+sizeof(_Setup)) &&  (Adress<=0xE800))
        {//odczyt zmiennych analogowych
          pMem=((char*)ram.AnaVarRAMu)+(Adress -0xC800);
  
        }
        else if ((Adress>=0xF700) && (Adress<=0xF700+CommVarRAM_Size) &&  (Adress<=0xFEFF))
        {//odczyt buforów komunikacyjnych
          pMem=ram.VarCom_Byte+Adress-0xF700;        
        }
        else if (Adress==RsWin_DeviceName)
        {//Nazwa urzadzenia
          pMem=SYSTEM_NAME;
        }
        else if (Adress==RsWin_DeviceNameOld)
        {//Nazwa urzadzenia
          pMem=(char*)identyfikacja;
        }
        else if (Adress==RsWin_ProgramSpeed)
        {//Szybkoœæ pêtli programu u¿ytkowego 
          CzasPetliPU=g_czas_petli / 10;
          pMem=(char*)&CzasPetliPU;
        }        
        else if (Adress==RsWin_ProgramSpeed)
        {//Szybkoœæ pêtli programu u¿ytkowego 
          CzasPetliPU=g_czas_petli / 10;
          pMem=(char*)&CzasPetliPU;
        }
        else{
          //nieznany rozkaz/obszar
          Fok=0; 
        }
        

        if(Fok)
        {
              *pTx=0xAC; pTx++;
              *pTx=0x04; pTx++;
           
              for (char k=0; k<Length; k++)  {*pTx=*pMem; pMem++; pTx++;}
              ByteToSend=pTx-Tx;
              *pTx=RsWin_SK(Tx, ByteToSend++);
              
        }     
        return ByteToSend;
        
        
    */
}
//*____________________________________________________________________________         





//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ReadRam
//* Funkcja zapis ramu
//* Rx-dane otrzymane
//* Tx-dane do wys³ania
//* Adress
//* Length- iloœc danych do zapisania
//* Funkcja zwraca iloœæ bajtów do wys³ania
//*-----------------------------------------------------------------------------
short int RsWin_WriteRam(unsigned short  Adress,unsigned char Length,char *Data, char *Tx)
{       
  /*
        char *pTx=Tx;
        char Fok=1; //flaga poprawnoœci
        char *pMem;
        char ByteToSend=0;
        short CzasPetliPU;  //Szybkoœæ pêtli programu u¿ytkowego /10

        if ((Adress>=0xC000) && (Adress<=0xC000+BinVarRAM_Size) && (Adress<0xC800))
        {//odczyt zmiennych dwustanowych
               pMem=ram.BinVarRAM+(Adress -0xC000); 
               
               for (char k=0; k<Length; k++)  {MasterToBin(pMem++, Data++); }
               
              *pTx=0xAC; pTx++;
              *pTx=0x02; pTx++;
              *pTx=RsWin_SK(Tx, 2);
               Fok=0; 
        }
        else if ((Adress>=0xC800) && (Adress<=0xC800+AnaVarRAM_Size*4+sizeof(_Setup)) &&  (Adress<=0xE800))
        {//zapis zmiennych analogowych
          pMem=((char*)ram.AnaVarRAMu)+(Adress -0xC800);
          if (Adress>=AnaVarRAM_Size) ChangeSetup=1; 
        }
        else if ((Adress>=0xF700) && (Adress<=0xF700+CommVarRAM_Size) &&  (Adress<=0xFEFF))
        {//odczyt buforów komunikacyjnych
          pMem=ram.VarCom_Byte+Adress-0xF700;        
        }
        else if (Adress==RsWin_DeviceName)
        {//Nazwa urzadzenia
          pMem=(char*)identyfikacja;
        }
        else if (Adress==RsWin_ProgramSpeed)
        {//Szybkoœæ pêtli programu u¿ytkowego 
          CzasPetliPU=g_czas_petli / 10;
          pMem=(char*)&CzasPetliPU;
        }        
        else{
          //nieznany rozkaz/obszar
          Fok=0; 
        }
        

        if(Fok)
        {
              for (char k=0; k<Length; k++)  {*pMem=*Data; pMem++; Data++;}
              
              *pTx=0xAC; pTx++;
              *pTx=0x02; pTx++;
              *pTx=RsWin_SK(Tx, 2);
              
        }     
        return 3;
        
  */      

}
//*____________________________________________________________________________         






//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    RsWin_ConfigBinGroup
//* Funkcja konfiguracji zbioru zmiennych dwustanowych
//* Tx-dane do wys³ania
//* Kode
//* Length- iloœc danych do zapisania
//* Funkcja zwraca iloœæ bajtów do wys³ania
//*-----------------------------------------------------------------------------
short int RsWin_ConfigBinGroup(char  Kode,unsigned char Length,short *Symbol, char *Tx)
{    
  /*
        char *pTx=Tx;
        char idx1=0;
        ram.PZB[idx1++]=Kode;
        ram.PZB[idx1++]=Length;
        for (char idx2=0; idx2<Length; idx2++)
        {
           //ram.PZB[idx1++]=ram.BinVarRAM[(*Symbol)&0xFFF]; 
           ram.PZB[idx1++]=*Symbol; 
           Symbol++;
        }

        *pTx=0xAC; pTx++;
        *pTx=0x02; pTx++;
        *pTx=RsWin_SK(Tx, 2);
              
        return 3;
  */
}
//*____________________________________________________________________________        





//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    RsWin_ConfigAnaGroup
//* Funkcja konfiguracji zbioru zmiennych analogowych
//* Tx-dane do wys³ania
//* Kode
//* Length- iloœc danych do zapisania
//* Funkcja zwraca iloœæ bajtów do wys³ania
//*-----------------------------------------------------------------------------
short int RsWin_ConfigAnaGroup(char  Kode,unsigned char Length,short *Symbol, char *Tx)
{       
  /*
        char *pTx=Tx;
        char idx1=0;
        
        ram.PZA[idx1++]=Kode;
        ram.PZA[idx1++]=Length;
        for (char idx2=0; idx2<Length; idx2++)
        {
           ram.PZA[idx1++]=*Symbol; 
           Symbol++;
        }

        *pTx=0xAC; pTx++;
        *pTx=0x02; pTx++;
        *pTx=RsWin_SK(Tx, 2);
      */        
        return 3;
}
//*____________________________________________________________________________        



//SUMA KONTEROLNA---------------------------------------------------------------
char RsWin_SK(char *TabLiczb, unsigned char IlLiczb)
{
        byte SK=0;
        for (int k = 0; k<IlLiczb; k++) 
        {
          SK += TabLiczb[k]; 
        }
        SK = 0 - SK;
        TabLiczb[IlLiczb]=SK;
        return  SK;
}
//*____________________________________________________________________________         






//funkcja konwersji zmiennej dwustanowej z formatu Float na format MASTER ------
char BinToMaster(char *adr)                    
{
  /*
    char odp =0;
    odp = ((*adr & 0x1)<<7);         
    if (*adr & BFF) {odp |= 0x15;}
    return odp;
  */
}
//*____________________________________________________________________________         




//Funkcja konwersji zmiennej dwustanowej z formatu MASTER na format MSU------- 
void MasterToBin(char *adr, char *vart)           //(nr. zmiennej, wartoœæ do konwersji)
{
        *adr=*vart>>7; 
        
        //Ustawienie forsowanie
        if ((*vart & 0x1F)==0x15) 
        { 
          *adr |=0x80;
        }
         else
        {  
          *adr &=0x7F;
        }
  
}
//*____________________________________________________________________________         



