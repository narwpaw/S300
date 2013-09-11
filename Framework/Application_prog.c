//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                                 CEURON                                   *//                       
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : Application_prog.c                       *//
//*  Opis                         : Plik obs³ugi programu u¿ytkowego         *//
//*  Autor                        : Pawe³ Narwojsz                           *//
//*  Data utrzorzenia             : 10.09.2007                               *//
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




#include "Application_prog.h" 

#include "Framework.h"
#include "core/device.h"                                                        //aduje bibliotreki dla odpowiedniego procesora
#include "modbus.h"
#include "usart.h"
#include "RF_Exe.h"
#include "ScreenMENU2.h"


//Zmienne globalne -------------------------------------------------------------
  char NumberScene=NumberSceneMax;  //iloœæ u¿ywanych scen. Wartoœæ pierwotnie NumberSceneMax nastêpnie wyliczana
  short ScreenUse;
  char SceneBefOff=0; //Je¿eli >0 to oznacza ¿e sceny zosta³y wy³¹czone przyskiem/zdarzeniem off.  SceneBefOff= nr sceny przed wy³¹czeniem
  _TransQuality TransQuality; //tablica informacyjna jakoœci po³¹czenia pola odpowiadaj¹ polom _ProgramTab.NeuronsIdTab

  //___________________________Koniec zmiennych globalnych________________________




#ifdef _LIVE_RJESTR   
  extern unsigned short LIVE_TAB[LIVE_TAB_SIZE];
  extern char LIVE_TAB_IDX;
#endif
  

extern RAM ram;
extern int g_US0_BAUDRATE;
//extern FlashPagesOffset;
Convers_DW_F ProgConvers_DW_F;

extern  _ScreenPaternDesign ScreenSetTab_0[];
extern  _ScreenPaternDesign ScreenSetTab_1[];
extern int int1;
extern int FlashProgramOfset;
extern int g_counter;
extern char AdressModbus;
extern char q_US0_Comm_Mode;
extern char US0_MasterMODBUS_ENABLE;
extern void Delay (unsigned long a);
extern char g_Stop_Program;                                                     //1- zatrzymuje program
extern char g_Restart_Program;                                                  //1- restartuje program
extern unsigned int CyklPerSek;
extern _DateTime DateTime;
extern _RF_TaskTab  RF_TaskTab;
_ButtonGraph  ButtonExe[ScreenNr][ScrenButtonNr];                                         //do tej zmiennej przepisywane sa stany 1 z Button


volatile _ProgramTab gProg;
char        *pProg=(char*)&gProg;
_NeuronsListUnit NeuronsList[NumberNeurons];




char Variables[13];    //zmienne
char VariablesImpulse[13];    //zmienne - flaga impulsu
//char VariablesNetTmp[13];    //zmienne tyczmasowe dla komunikacji WiFi stosowana jako separacja  Variables[] ¿eby nie nadpisywaæ zmiennych
short int VariablesAnal[AnalogVarAmount];    //zmienne

extern unsigned char ChangeStageFlag;


_MainStage  MainStage; 
_Trap      Trap;

extern _ZD_MODBUS ZD_MODBUS_0[ZD_MODBUS_Tab_Max];
void SetModbusTask(void)
{

      ptr_ZD_MODBUS ZD_MODBUS;


      ZD_MODBUS         =   ZD_MODBUS_0;
  

   
      
      char i=0; 
  
 
         
      ZD_MODBUS[i].Adress=1;
      ZD_MODBUS[i].Function=0xF;
      ZD_MODBUS[i].Target=0x0;
      ZD_MODBUS[i].StartAdr=0;
      ZD_MODBUS[i].Coil=8;
      ZD_MODBUS[i].Error=0x0;
      
      i=1; 
      ZD_MODBUS[i].Adress=2;
      ZD_MODBUS[i].Function=0xF;
      ZD_MODBUS[i].Target=0x0;
      ZD_MODBUS[i].StartAdr=1;
      ZD_MODBUS[i].Coil=8;
      ZD_MODBUS[i].Error=0x0;
 
      i=2; 
      ZD_MODBUS[i].Adress=3;
      ZD_MODBUS[i].Function=0xF;
      ZD_MODBUS[i].Target=0x0;
      ZD_MODBUS[i].StartAdr=2;
      ZD_MODBUS[i].Coil=8;
      ZD_MODBUS[i].Error=0x0;      

     
      
}


//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    PortStageRead
//* Funkcja wydobywa wartoœæ bitu o numerze PortNr ze zmiennej pod adresem *Stage 
//*-----------------------------------------------------------------------------
char PortStageRead(short int Stage, char PortNr)
{
    char odp=0;
    odp=(Stage>>PortNr)&0x1;
    return odp;
}

//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    PortStageSet
//* Funkcja ustawia stan portu (bit PortNr dla zmiennej pod adresem *Stage) na Value 
//*-----------------------------------------------------------------------------
void PortStageSet(short int *Stage, char PortNr, char Value)
{
    Value&=0x1;
    *Stage=(*Stage&(~(0x1<<PortNr))) | (Value<<PortNr);
}



//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcStI_Action
//* Funkcja procedury "Stan wejœcia"
//*-----------------------------------------------------------------------------
void ProcStI_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg) 
{  
  //je¿eli port w wêŸle == 1 i wiarygodny pomiar
  if (Proc->Nr>=0)
  {
    if ((MainStg->StageI[Proc->Nr].Port !=0x8AAA))
    {
       if (*PrvOut)
       {
          Proc->Out=PortStageRead(MainStg->StageI[Proc->Nr].Port, Proc->Tag.Port);  
       }else{
         Proc->Out=0;
       }
    }
    
  //je¿eli zmienna dwustanowa  
  }else{
      Proc->Out= (Variables[Proc->Tag.Port/8]>> (Proc->Tag.Port%8))&0x1;
  }

}//_______________________ Koniec funkcji  ____________________________



//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcStI_Action
//* Funkcja procedury "Stan wejœcia"
//*-----------------------------------------------------------------------------
void ProcStNI_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg) 
{  
         //je¿eli port w wêŸle == 1 i wiarygodny pomiar
  if ((MainStg->StageI[Proc->Nr].Port !=0x8AAA))
  {
  
   if (*PrvOut)
   {

         if (Proc->Nr>=0)
         {
           if (PortStageRead(MainStg->StageI[Proc->Nr].Port, Proc->Tag.Port)==1)
           {
            Proc->Out=0; 
           }else{
            Proc->Out=1;
           }
         }else{
           if (((Variables[Proc->Tag.Port/8]>> (Proc->Tag.Port%8))&0x1)==1)
           {
            Proc->Out=0; 
           }else{
            Proc->Out=1;
           }
         }
       }else{
         Proc->Out=0;
       }
   }

}//_______________________ Koniec funkcji  ____________________________




//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcGetAnal_Action
//* Funkcja procedury "Stan wejœcia analogowego"
//*-----------------------------------------------------------------------------
void ProcGetReg_Action(_Procedure *Proc, _MainStage *MainStg) 
{  
  if (Proc->Nr>=0)
  {
      if ((MainStg->StageI[Proc->Nr].Value[Proc->Tag.RegNr])!=(signed short int )0x8AAA)
      {
        Proc->Out=MainStg->StageI[Proc->Nr].Value[Proc->Tag.RegNr];  
      }
  }else{
      if ((MainStg->StageI[Proc->Nr].Value[Proc->Tag.RegNr])!=(signed short int )0x8AAA)
      {
        Proc->Out=VariablesAnal[Proc->Tag.RegNr];
      }
  }
     
}//_______________________ Koniec funkcji  ____________________________



//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcSetAnal_Action
//* Funkcja procedury "Ustawienie wyjœcia analogowego"
//*-----------------------------------------------------------------------------
void ProcSetAnal_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg) 
{  
  if (Proc->Nr>=0)
  {
      MainStg->StageQ[Proc->Nr].Value[Proc->Tag.RegNr]=*PrvOut;
  }else{
      VariablesAnal[Proc->Tag.RegNr]=*PrvOut;
  }  
  Proc->Out=*PrvOut;
 
}//_______________________ Koniec funkcji  ____________________________




//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcConstAnal_Action
//* Funkcja procedury "staa analogowa"
//*-----------------------------------------------------------------------------
void ProcConstAnal_Action(_Procedure *Proc) 
{  
  Proc->Out=Proc->Tag.ConstAnValue;  
}//_______________________ Koniec funkcji  ____________________________





//*-----------------------------------------------------------------------------





//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcStB_Action
//* Funkcja procedury "Akcja przycisków przycisków"
//*-----------------------------------------------------------------------------
void ProcStB_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg) 
{  
   if (*PrvOut)
   {
      Proc->Out=MainStg->StageButtonExe[Proc->Nr];  

   }else{
     Proc->Out=0;
   }

}//_______________________ Koniec funkcji  ____________________________



//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcStQ_Action
//* Funkcja procedury "Stan wyjœcia"
//*-----------------------------------------------------------------------------
void ProcStQ_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg) 
{  
   if (*PrvOut)
   {
      Proc->Out=PortStageRead(MainStg->StageQ[Proc->Nr].Port,Proc->Tag.Port);  
   }else{
      Proc->Out=0;
   }

}//_______________________ Koniec funkcji  ____________________________





//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcStS_Action
//* Funkcja procedury "Stan sceny"
//*-----------------------------------------------------------------------------
void ProcStS_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg) 
{  
  Proc->Out=0;
   if (*PrvOut)
   {
      if (Proc->Nr==MainStg->SceneNr) Proc->Out=1;
   }

}//_______________________ Koniec funkcji  ____________________________



/*

//-----------------------------------------------------------------------------
// Nazwa funkcji :    ProcStT_Action
// Funkcja procedury "Stan timera"
//-----------------------------------------------------------------------------
void ProcStT_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg) 
{  
   if (*PrvOut)
   {
      Proc->Out=PortStageRead(MainStg->StageTimer[Proc->Nr].Port,Proc->Tag.Port);  
   }else{
      Proc->Out=0;
   }

}//_______________________ Koniec funkcji  ____________________________




//-----------------------------------------------------------------------------
// Nazwa funkcji :    ProcStT_Action
// Funkcja procedury "Stan timera"
//-----------------------------------------------------------------------------
void ProcStNT_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg) 
{  
   if (*PrvOut) 
   {
      if (PortStageRead(MainStg->StageTimer[Proc->Nr].Port,Proc->Tag.Port))
      {
        Proc->Out=0;
      }else{
        Proc->Out=1;  
      }
   }else{
      Proc->Out=0;
   }

}//_______________________ Koniec funkcji  ____________________________
*/



 extern unsigned long long g_NextTimeProgram;


//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcWOUT_Action
//* Funkcja procedury "Ustaw stan wyjœcia"
//*-----------------------------------------------------------------------------

void ProcWOUT_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg) 
{  

     if ((*PrvOut==1) && (Proc->Out==0))
     {
      if ((Proc->Nr>=0) && (Proc->Nr!=0xFF))   //je¿eli wêze³
      {
         PortStageSet((&(MainStg->StageQ[Proc->Nr].Port)),Proc->Tag.Port,1); 
         if (TransQuality.Mensur[Proc->Nr-1]>0)  MainStg->SendStageChange[Proc->Nr]=ChangeStageFlag;
       
         #ifdef _LIVE_RJESTR 
             LIVE_TAB[LIVE_TAB_IDX]=0xF000 | ((Proc->Nr&0xF)<<8) | ((MainStg->StageQ[Proc->Nr].Port)&0xFF);
             if (LIVE_TAB_IDX<LIVE_TAB_SIZE) LIVE_TAB_IDX++; else LIVE_TAB_IDX=0;
         #endif

      }else{  //je¿eli zmienna dwustanowa  
          Variables[Proc->Tag.Port/8] |= 1<<(Proc->Tag.Port%8);
          
          #ifdef _LIVE_RJESTR   
              LIVE_TAB[LIVE_TAB_IDX]=0xF000 | ((Proc->Nr&0xF)<<8) | ((MainStg->StageQ[Proc->Nr].Port)&0xFF);
              if (LIVE_TAB_IDX<LIVE_TAB_SIZE) LIVE_TAB_IDX++; else LIVE_TAB_IDX=0;
          #endif
      }
     }

     if ((*PrvOut==0) && (Proc->Out==1))
     {
        if (Proc->Nr>=0)    //je¿eli wêze³
        {
           PortStageSet((&(MainStg->StageQ[Proc->Nr].Port)),Proc->Tag.Port,0); 
         
           if (TransQuality.Mensur[Proc->Nr-1]>0)   
           MainStg->SendStageChange[Proc->Nr]=ChangeStageFlag;
          
        }else //je¿eli zmienna dwustanowa
        { 
            Variables[Proc->Tag.Port/8]&= ~(1<<(Proc->Tag.Port%8));
            #ifdef _LIVE_RJESTR 
              LIVE_TAB[LIVE_TAB_IDX]=0xF000 | (Proc->Nr&0xF)<<1 | (MainStg->StageQ[Proc->Nr].Port)&0xFF;
              if (LIVE_TAB_IDX<LIVE_TAB_SIZE) LIVE_TAB_IDX++; else LIVE_TAB_IDX=0;
            #endif
        }  
     }
     Proc->Out=*PrvOut;
  
}//_______________________ Koniec funkcji  ____________________________


      

//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcWOUTS_Action
//* Funkcja procedury "Ustaw stan wyjœcia na 1"
//*-----------------------------------------------------------------------------

void ProcWOUTS_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg) 
{  

     if ((*PrvOut==1) && (Proc->Out==0))
     {
      if (Proc->Nr>=0)    //je¿eli wêze³
      {
       PortStageSet((&(MainStg->StageQ[Proc->Nr].Port)),Proc->Tag.Port,1); 
       
       if (TransQuality.Mensur[Proc->Nr-1]>0)   
         MainStg->SendStageChange[Proc->Nr]=ChangeStageFlag;
      
       #ifdef _LIVE_RJESTR 
       LIVE_TAB[LIVE_TAB_IDX]=0xF000 | (Proc->Nr&0xF)<<1 | (MainStg->StageQ[Proc->Nr].Port)&0xFF;
       if (LIVE_TAB_IDX<LIVE_TAB_SIZE) LIVE_TAB_IDX++; else LIVE_TAB_IDX=0;
       #endif
       
       //je¿eli zmienna dwustanowa  
      }else{
          Variables[Proc->Tag.Port/8]|= 1<<(Proc->Tag.Port%8);
      }
     }
     Proc->Out=*PrvOut;
  
}//_______________________ Koniec funkcji  ____________________________


//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcWOUTR_Action
//* Funkcja procedury "Ustaw stan wyjœcia  na 0"
//*-----------------------------------------------------------------------------

void ProcWOUTR_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg) 
{  

     if ((*PrvOut==1) && (Proc->Out==0))
     {
      if (Proc->Nr>=0)    //je¿eli wêze³
      { 
       PortStageSet((&(MainStg->StageQ[Proc->Nr].Port)),Proc->Tag.Port,0); 
       
       if (TransQuality.Mensur[Proc->Nr-1]>0)   
         MainStg->SendStageChange[Proc->Nr]=ChangeStageFlag;
       
       #ifdef _LIVE_RJESTR 
        LIVE_TAB[LIVE_TAB_IDX]=0xF000 | (Proc->Nr&0xF)<<1 | (MainStg->StageQ[Proc->Nr].Port)&0xFF;
        if (LIVE_TAB_IDX<LIVE_TAB_SIZE) LIVE_TAB_IDX++; else LIVE_TAB_IDX=0;
       #endif
        
        //je¿eli zmienna dwustanowa  
      }else{
          Variables[Proc->Tag.Port/8]&= ~(1<<(Proc->Tag.Port%8));
      }
     }
     Proc->Out=*PrvOut;
  
}//_______________________ Koniec funkcji  ____________________________




//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcWOUTC_Action
//* Funkcja procedury "zmaina stanu wyjœcia/zmiennej na przeciwny "
//*-----------------------------------------------------------------------------

void ProcWOUTC_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg) 
{  

     if ((*PrvOut==1) && (Proc->Out==0))
     {
      if (Proc->Nr>=0)    //je¿eli wêze³
      {
           if (PortStageRead(MainStg->StageQ[Proc->Nr].Port,Proc->Tag.Port)==1) //je¿eli wyjœcie w stanie wysokim to zeruj je¿eli nie to ustaw 1
           { 
              PortStageSet((&(MainStg->StageQ[Proc->Nr].Port)),Proc->Tag.Port,0); 
           }else{
              PortStageSet((&(MainStg->StageQ[Proc->Nr].Port)),Proc->Tag.Port,1);
           }
         
           if (TransQuality.Mensur[Proc->Nr-1]>0)   
           MainStg->SendStageChange[Proc->Nr]=ChangeStageFlag;
        
           #ifdef _LIVE_RJESTR 
            LIVE_TAB[LIVE_TAB_IDX]=0xF000 | (Proc->Nr&0xF)<<1 | (MainStg->StageQ[Proc->Nr].Port)&0xFF;
            if (LIVE_TAB_IDX<LIVE_TAB_SIZE) LIVE_TAB_IDX++; else LIVE_TAB_IDX=0;
           #endif
         
       
       //je¿eli zmienna dwustanowa  
      }else{
            if (Variables[Proc->Tag.Port/8] && (1<<(Proc->Tag.Port%8)))
            {
               Variables[Proc->Tag.Port/8] &= ~(1<<(Proc->Tag.Port%8));
            }else{
               Variables[Proc->Tag.Port/8] |= (1<<(Proc->Tag.Port%8));
            }
      }
     }
     Proc->Out=*PrvOut;
  
}//_______________________ Koniec funkcji  ____________________________





/*
//-----------------------------------------------------------------------------
// Nazwa funkcji :    ProcSetT_Action
// Funkcja procedury "Forsuj stan timera"
//-----------------------------------------------------------------------------

void ProcRstT_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg) 
{  
     if ((*PrvOut==1) && (Proc->Out==0))
     {
        MainStg->StageTimer[Proc->Nr].Curent=0;
        MainObiect.Timer[Proc->Nr]=0;
     }
     Proc->Out=*PrvOut;
  
}//_______________________ Koniec funkcji  ____________________________
*/



//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcNOT_Action
//* Funkcja procedury "Negacja
//*-----------------------------------------------------------------------------
void ProcNOT_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg) 
{  
   if (*PrvOut)
   {
      Proc->Out=0;  
   }else{
      Proc->Out=1;
   }
}//_______________________ Koniec funkcji__ ____________________________




//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcToggleT_Action
//* Funkcja procedury "Przerzutnik typu T"
//*-----------------------------------------------------------------------------
void ProcToggleRS_Action(short int *PrvOut, short int *UpOut, _Procedure *Proc) 
{  
   if ((*PrvOut))
   {
     Proc->Out=1;
   }
   else if (*UpOut)
   {
     Proc->Out=0;
   }
   

     
}//_______________________ Koniec funkcji__ ____________________________




//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcToggleT_Action
//* Funkcja procedury "Przerzutnik typu T"
//*-----------------------------------------------------------------------------
void ProcToggleT_Action(short int *PrvOut, _Procedure *Proc) 
{  
   if ((*PrvOut) && (Proc->LastIn==0) && (Proc->Out==0))
   {
     Proc->Out=1;
   }
   else if ((*PrvOut) && (Proc->LastIn==0) && (Proc->Out==1))
   {
     Proc->Out=0;
   }
   
   Proc->LastIn=*PrvOut;
       


}//_______________________ Koniec funkcji__ ____________________________


//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcTLI1_Action
//* Funkcja procedury "Procedura impulsu jednocyklowego
//*-----------------------------------------------------------------------------
void ProcTLI1_Action(short int *PrvOut, _Procedure *Proc) 
{  
   if ((*PrvOut) && (Proc->LastIn==0))
   {
     Proc->Out=1;
   }
   else
   {
     Proc->Out=0;
   }
   
   Proc->LastIn=*PrvOut;
       


}//_______________________ Koniec funkcji__ ____________________________




//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcLim_Action
//* Funkcja procedury "Porównanie wartoœci. Je¿eli 1>2 q=1"
//*-----------------------------------------------------------------------------
void ProcLim_Action(short int *PrvOut, short int *UpOut, _Procedure *Proc) 
{  
   if ((*PrvOut)>(*UpOut))
   {
     Proc->Out=1;
   }
   else
   {
     Proc->Out=0;
   }
     
}//_______________________ Koniec funkcji__ ____________________________




//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcEven_Action
//* Funkcja procedury "Porównanie wartoœci. Je¿eli 1=2 q=1"
//*-----------------------------------------------------------------------------
void ProcEven_Action(short int *PrvOut, short int *UpOut, _Procedure *Proc) 
{  
   if ((*PrvOut)==(*UpOut))
   {
     Proc->Out=1;
   }
   else
   {
     Proc->Out=0;
   }
   

     
}//_______________________ Koniec funkcji__ ____________________________




//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcPlus_Action
//* Funkcja procedury "Plus"
//*-----------------------------------------------------------------------------
void ProcPlus_Action(short int *PrvOut, short int *UpOut, _Procedure *Proc) 
{  
     Proc->Out=(*PrvOut)+(*UpOut);  
}//_______________________ Koniec funkcji__ ____________________________



//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcPlus_Action
//* Funkcja procedury "Plus"
//*-----------------------------------------------------------------------------
void ProcMinus_Action(short int *PrvOut, short int *UpOut, _Procedure *Proc) 
{  
     Proc->Out=(*PrvOut)-(*UpOut);  
}//_______________________ Koniec funkcji__ ____________________________


//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcPlus_Action
//* Funkcja procedury "Plus"
//*-----------------------------------------------------------------------------
void ProcMul_Action(short int *PrvOut, short int *UpOut, _Procedure *Proc) 
{    
     int odp;
     odp=(*PrvOut)*(*UpOut)/10; 
     if (odp>30000)
     {
        if (odp>0) Proc->Out=30000; else Proc->Out=-30000;
     }else{
        Proc->Out=odp; 
     }
}//_______________________ Koniec funkcji__ ____________________________


//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcPlus_Action
//* Funkcja procedury "Plus"
//*-----------------------------------------------------------------------------
void ProcDiv_Action(short int *PrvOut, short int *UpOut, _Procedure *Proc) 
{  
     if ((*UpOut)==0)
     {
       if ((*PrvOut)>0) Proc->Out=30000; else Proc->Out=-30000;
     }else{
      
      Proc->Out=((*PrvOut)*10)/(*UpOut); 
      if ( (((*PrvOut)*100)/(*UpOut))>((((*PrvOut)*10)/(*UpOut))*10)+5) Proc->Out++;
     }
}//_______________________ Koniec funkcji__ ____________________________





//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcConect_Action
//* Funkcja procedury "Przeniesienie stanu w prawo"
//*-----------------------------------------------------------------------------
void ProcConect_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg) 
{  
      Proc->Out=*PrvOut;  
  
}//_______________________ Koniec funkcji__ ____________________________



//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcDown_Action
//* Funkcja procedury "Przeniesienie stanu w dó³"
//*-----------------------------------------------------------------------------
void ProcDown_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg) 
{  
      Proc->Out=*PrvOut;  
}//_______________________ Koniec funkcji__ ____________________________



//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcDownRight_Action
//* Funkcja procedury "Przeniesienie stanu w dó³ i prawo"
//*-----------------------------------------------------------------------------
void ProcDownRight_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg) 
{  
      Proc->Out=*PrvOut;
}//_______________________ Koniec funkcji__ ____________________________





//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcUpToRight_Action
//* Funkcja procedury "Przeniesienie stanu z góry w prawo"
//*-----------------------------------------------------------------------------
void ProcUpToRight_Action(short int *PrvUpOut, _Procedure *Proc) 
{  
      Proc->Out=*PrvUpOut;
}//_______________________ Koniec funkcji__ ____________________________



//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcUpToRightDown_Action
//* Funkcja procedury "Przeniesienie stanu z góry w prawo i dó³"
//*-----------------------------------------------------------------------------
void ProcUpToRightDown_Action(short int *PrvUpOut, _Procedure *Proc) 
{  
      Proc->Out=*PrvUpOut;
}//_______________________ Koniec funkcji__ ____________________________


//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcSetScn
//* Funkcja procedury "Ustaw scenê"
//*-----------------------------------------------------------------------------
char LastScene=0;
void ProcSetScn_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg) 
{  
     
  
     if ((*PrvOut==1) && (Proc->Out==0))
     {  
        //if (Proc->Tag.NrScene>NumberScene) Proc->Tag.NrScene=NumberScene;
       if (Proc->Nr==0)  //je¿eli on/off scen
       {
         if (LastScene)
         {
            MainStg->SceneNr=LastScene;
            LastScene=0;
         }else{ 
            LastScene=MainStg->SceneNr;
            MainStg->SceneNr=0;
         }
         
       }else{
        MainStg->SceneNr=Proc->Nr;
       }
     }

     
     Proc->Out=*PrvOut;

}//_______________________ Koniec funkcji__ ____________________________




//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcInc_Action
//* Funkcja procedury "Inkrementuj scenê"
//*-----------------------------------------------------------------------------
void ProcInc_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg) 
{  
     if ((*PrvOut) && (Proc->LastIn==0))
     {  
        
          if (MainStg->SceneNr<NumberScene) MainStg->SceneNr++;
          LastScene=0;
     }
     

     Proc->LastIn=*PrvOut;
     Proc->Out=*PrvOut;
 
}//_______________________ Koniec funkcji__ ____________________________


//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcIncDec_Action
//* Funkcja procedury "Dekrementuj scenê"
//*-----------------------------------------------------------------------------
void ProcDec_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg) 
{  
     if ((*PrvOut) && (Proc->LastIn==0))
     {  
        
        if (MainStg->SceneNr>0) MainStg->SceneNr--;
        LastScene=0;
     }
     

     Proc->LastIn=*PrvOut;
     Proc->Out=*PrvOut;

}//_______________________ Koniec funkcji__ ____________________________



//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcSPK_Action
//* Funkcja procedury "Dekrementuj scenê"
//*-----------------------------------------------------------------------------
extern     char Speaker[SPK_Amount];                                                   //sygna³ dŸwiêkowy
void ProcSPK_Action(short int *PrvOut, _Procedure *Proc) 
{  
     if ((*PrvOut) && (Proc->Out==0))
     {  
        for (char i=0; i<SPK_Amount; i++) Speaker[i]=0;
        Speaker[Proc->Tag.SpeakerType]=1;
        if (Proc->Tag.SpeakerType==0) 
        {
               ButtonBipCounter=g_counter+300000;
               AT91C_BASE_PIOA->PIO_SODR =SpeakerPin;  
        }
     }else if (*PrvOut==0) {
       for (char i=0; i<SPK_Amount; i++) Speaker[i]=0;
     }
     if ((*PrvOut==0) && (Proc->Out==1))
     {  
        AT91C_BASE_PIOA->PIO_CODR =SpeakerPin;  
     }
     Proc->Out=*PrvOut;
}//_______________________ Koniec funkcji__ ____________________________





//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcSumL_Action
//* Funkcja procedury "Sumy logicznej"
//*-----------------------------------------------------------------------------
void ProcSumL_Action(short int *PrvOut, short int *UpOut, _Procedure *Proc) 
{  
  Proc->Out=*PrvOut | *UpOut;   
}//_______________________ Koniec funkcji__ ____________________________



//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcIncDec_Action
//* Funkcja procedury "Inkrementuj scenê"
//*-----------------------------------------------------------------------------
void ProcTimerSet_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg) 
{  
   if ((*PrvOut)==0)
   {
     Proc->Out=0;
   }else{
     if ((*PrvOut) && (Proc->LastIn==0))
     {  
       Proc->Tag.CounterStartTime=g_seconds_counter; 
       Proc->Out=0;
     }
     
     if ((Proc->Tag.CounterStartTime+Proc->Tag.SetCounter)<g_seconds_counter)
     {
       Proc->Out=1;
     }else{
       Proc->Out=0;
     }
   }   
   
   Proc->LastIn=*PrvOut;
}//_______________________ Koniec funkcji__ ____________________________






//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcClock_Action
//* Funkcja procedury "Zegar"
//*-----------------------------------------------------------------------------

void ProcClock_Action(short int *PrvOut, _Procedure *Proc) 
{  
   Proc->Out=0;
   if (*PrvOut)
   {
      if ((Proc->Tag.ClkPrcType==PrcMounth) && (Proc->Tag.ClkPrcVal==DateTime.Mounth))  Proc->Out=1; 
      if ((Proc->Tag.ClkPrcType==PrcDay)    && (Proc->Tag.ClkPrcVal==DateTime.Day))     Proc->Out=1; 
      if ((Proc->Tag.ClkPrcType==PrcHour)   && (Proc->Tag.ClkPrcVal==DateTime.Hour))    Proc->Out=1; 
      if ((Proc->Tag.ClkPrcType==PrcMinute) && (Proc->Tag.ClkPrcVal==DateTime.Minute))  Proc->Out=1;  
      if ((Proc->Tag.ClkPrcType==PrcSecond) && (Proc->Tag.ClkPrcVal==DateTime.Second))  Proc->Out=1; 
              

   }

}//_______________________ Koniec funkcji  ____________________________



//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ProcDay_Action
//* Funkcja procedury "Dnia tygodnia"
//*-----------------------------------------------------------------------------

void ProcDay_Action(short int *PrvOut, _Procedure *Proc) 
{  
   Proc->Out=0;
   if (*PrvOut)
   {
      if (Proc->Tag.DayOfWeek==DateTime.DayOfWeek)  Proc->Out=1; 

   }
}//_______________________ Koniec funkcji  ____________________________







//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    Program
//* Funkcja opóŸnienia
//*-----------------------------------------------------------------------------
volatile char Busy_program=0;

char SceneNrLast=0;
char RealPrcLines=MaxPrcLines;

int PtogTime=0;
unsigned long last_time;

void Program (_ProgramTab *ProgTab) 
{ 
    static int MemCol=0;
    static int MemRow=0;
    int StartLineNr=0;
    int StartPrcInLine=0;
    
    if (Busy_program==0)
    {
      Busy_program=1;
    

   last_time=g_counter;
  

  
  short int StPrvPrc; //stan poprzedniej procedury
  short int StUpPrc; //stan procedury z linii wy¿ej, z tej samej kolumny dla sumy logicznej
  
  if ((Trap.Enable) && (Trap.Change))
        {
          StartPrcInLine=MemCol;
          StartLineNr=MemRow;
          Trap.Change=0;
        }
  
   
      
                
       
         
  
       for (char NB=0; NB<NumberButton; NB++)
       {
            MainStage.StageButtonExe[NB]=MainStage.StageButton[NB];  
       }
  
  
  
      for (int LineNr=StartLineNr; LineNr<RealPrcLines; LineNr++)
      {
        for (int PrcInLine=StartPrcInLine; PrcInLine<MaxPrcInLine; PrcInLine++)
        {
         
          

          
          
          
         if ((Trap.Enable) &&  (Trap.Row==LineNr) && (Trap.Col==PrcInLine)) 
         {
           Trap.Activ=1; 
           MemCol=PrcInLine;
           MemRow=LineNr;
            
         }
         if ((Trap.Enable==0) || (Trap.Activ==0) )
         {

           
         if (Trap.Enable==0)
         {
          MemCol=PrcInLine;
          MemRow=LineNr;
         }  
           
           
           if (ProgTab->Line[LineNr].Proc[PrcInLine].Type!=None)
           {
              if (PrcInLine==0)
              {
                StPrvPrc= 1;
              }else{
                StPrvPrc= (ProgTab->Line[LineNr].Proc[PrcInLine-1].Out);
              }
              
              

              switch (ProgTab->Line[LineNr].Proc[PrcInLine].Type)
              {    
                  case ProcConect     : ProcConect_Action(&StPrvPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine]), &MainStage); break;
                  case ProcDown       : ProcDown_Action(&StPrvPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine]), &MainStage); break;
                  case ProcDownRight  : ProcDownRight_Action(&StPrvPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine]), &MainStage); break;
                  case ProcSumLogRight     : 
                                      StUpPrc= 0;
                                      if (LineNr!=0) StUpPrc=(ProgTab->Line[LineNr-1].Proc[PrcInLine].Out);
                                      ProcSumL_Action(&StPrvPrc, &StUpPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine])); 
                                      break;
                  case ProcSumLogDown     : 
                                      StUpPrc= 0;
                                      if (LineNr!=0) StUpPrc=(ProgTab->Line[LineNr-1].Proc[PrcInLine].Out);
                                      ProcSumL_Action(&StPrvPrc, &StUpPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine])); 
                                      break;
                  case ProcUpToRight     : 
                                      StUpPrc= 0;
                                      if (LineNr!=0) StUpPrc=(ProgTab->Line[LineNr-1].Proc[PrcInLine].Out);
                                      ProcUpToRight_Action(&StUpPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine])); 
                                      break;
                                      
                  case ProcUpToRightDown    : 
                                      StUpPrc= 0;
                                      if (LineNr!=0) StUpPrc=(ProgTab->Line[LineNr-1].Proc[PrcInLine].Out);
                                      ProcUpToRightDown_Action(&StUpPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine])); 
                                      break;                    
                                   
                 
                  
                  case ProcNOT        : ProcNOT_Action(&StPrvPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine]), &MainStage); break;         
                  case ProcToggleRS   : if (LineNr!=0) StUpPrc=(ProgTab->Line[LineNr-1].Proc[PrcInLine].Out);
                                        ProcToggleRS_Action(&StPrvPrc, &StUpPrc,  &(ProgTab->Line[LineNr].Proc[PrcInLine])); break;    
                  case ProcToggleT    : ProcToggleT_Action(&StPrvPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine])); break;         
                  case ProcTLI1       : ProcTLI1_Action(&StPrvPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine])); break;         
                  case ProcStI        : ProcStI_Action(&StPrvPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine]), &MainStage); break;
                  case ProcStNI       : ProcStNI_Action(&StPrvPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine]), &MainStage); break;
                  case ProcStQ        : ProcStQ_Action(&StPrvPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine]), &MainStage); break;
                  case ProcStB        : ProcStB_Action(&StPrvPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine]), &MainStage); break;
                  case ProcStS        : ProcStS_Action(&StPrvPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine]), &MainStage); break;
                  case ProcWOUT       : ProcWOUT_Action(&StPrvPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine]), &MainStage); break;
                  case ProcWOUTS      : ProcWOUTS_Action(&StPrvPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine]),&MainStage); break;
                  case ProcWOUTR      : ProcWOUTR_Action(&StPrvPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine]),&MainStage); break;
                  case ProcWOUTC      : ProcWOUTC_Action(&StPrvPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine]),&MainStage); break;
                  case ProcSetScn     : ProcSetScn_Action(&StPrvPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine]), &MainStage); break;
                  case ProcInc        : ProcInc_Action(&StPrvPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine]), &MainStage); break;             
                  case ProcDec        : ProcDec_Action(&StPrvPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine]), &MainStage); break;             
                  case ProcSPK        : ProcSPK_Action(&StPrvPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine])); break; 
                  case ProcTimerSet   : ProcTimerSet_Action(&StPrvPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine]), &MainStage); break;
                  case ProcClock      : ProcClock_Action(&StPrvPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine])); break;
                  case ProcDay        : ProcDay_Action(&StPrvPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine])); break;
                  case ProcGetReg     : ProcGetReg_Action(&(ProgTab->Line[LineNr].Proc[PrcInLine]), &MainStage); break;
                  case ProcSetReg     : ProcSetAnal_Action(&StPrvPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine]), &MainStage); break;
                  case ProcConstAnal  : ProcConstAnal_Action(&(ProgTab->Line[LineNr].Proc[PrcInLine])); break;
                  case ProcLim        : if (LineNr!=0) StUpPrc=(ProgTab->Line[LineNr-1].Proc[PrcInLine].Out);
                                        ProcLim_Action(&StPrvPrc, &StUpPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine])); break;
                  case ProcEven       : if (LineNr!=0) StUpPrc=(ProgTab->Line[LineNr-1].Proc[PrcInLine].Out);
                                        ProcEven_Action(&StPrvPrc, &StUpPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine])); break;
                  case ProcPlus       : if (LineNr!=0) StUpPrc=(ProgTab->Line[LineNr-1].Proc[PrcInLine].Out);
                                        ProcPlus_Action(&StPrvPrc, &StUpPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine])); break;
                  case ProcMinus      : if (LineNr!=0) StUpPrc=(ProgTab->Line[LineNr-1].Proc[PrcInLine].Out);
                                        ProcMinus_Action(&StPrvPrc, &StUpPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine])); break;
                  case ProcMul        : if (LineNr!=0) StUpPrc=(ProgTab->Line[LineNr-1].Proc[PrcInLine].Out);
                                        ProcMul_Action(&StPrvPrc, &StUpPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine])); break;
                  case ProcDiv        : if (LineNr!=0) StUpPrc=(ProgTab->Line[LineNr-1].Proc[PrcInLine].Out);
                                        ProcDiv_Action(&StPrvPrc, &StUpPrc, &(ProgTab->Line[LineNr].Proc[PrcInLine])); break;

                  
              }
              
           }  
          
         
         
         
         
         
         if ((LineNr==RealPrcLines-1) && (PrcInLine==MaxPrcInLine-1))
          { 
          
              //ThermostatsExecute();

                /* 
                for (char iNrQ=0; iNrQ<NumberNeurons; iNrQ++)
                {  
                  for (char iNrReg=0; iNrReg<NumberReg; iNrReg++)
                {
                  MainStage.StageQ[iNrQ].Value[iNrReg]=MainStage.StageQ_TMP[iNrQ].Value[iNrReg]; 
                }
                
                }
                */

            /*
            for (char iNrReg=0; iNrReg<NumberReg; iNrReg++)
            {
              MainStage.StageI[0].Value[iNrReg]=MainStage.StageQ[0].Value[iNrReg]; 
            }
           */
 
  
            if (MainStage.SceneNr!=SceneNrLast)
            {
              SceneNrLast=MainStage.SceneNr;
              MenuPaint();
            }
 
         

         }
         
         
        }
        
        
      }
    }
  
  
  
  //zerowanie wyjœæ  z flagami impulsu      
       for (char iNrQ=0; iNrQ<NumberNeurons; iNrQ++)
       {
          if ((MainStage.StageQImpulse[iNrQ]) && (MainStage.SendStageChange[iNrQ]==0))
          {
            MainStage.StageQ[iNrQ].Port &=~(MainStage.StageQImpulse[iNrQ]); 
            MainStage.StageQImpulse[iNrQ]=0;
            MainStage.SendStageChange[iNrQ]=ChangeStageFlag;
          } 
       }
  
  //zerowanie zmiennych  z flagami impulsu  
  for (char i=0; i<sizeof(VariablesImpulse); i++)
  {
    if (VariablesImpulse[i]>0)
    {
      Variables[i]&=~(VariablesImpulse[i]);
      VariablesImpulse[i]=0;
    }
  }
  
  
  
  Busy_program=0;
  
    }
   
    
    PtogTime=g_counter-last_time;

}//_______________________ Koniec funkcji program__ ____________________________



//-----------------------------------------------------------------------------
// Nazwa funkcji :    SceneNrCount
// Funkcja zlicza u¿ywane w programie u¿ytkowym sceny. Odpowiedzi¹ jest iloœæ scen
//-----------------------------------------------------------------------------
char SceneNrCount(_ProgramTab *ProgTab)
{
      char MaxScnNr=0;
      for (int LineNr=0; LineNr<MaxPrcLines; LineNr++)
      {
        for (int PrcInLine=0; PrcInLine<MaxPrcInLine; PrcInLine++)
        {
           if ((ProgTab->Line[LineNr].Proc[PrcInLine].Type==ProcStS) && (ProgTab->Line[LineNr].Proc[PrcInLine].Nr>MaxScnNr))
           {
             MaxScnNr=ProgTab->Line[LineNr].Proc[PrcInLine].Nr;
           }
        }
        
      }
  
      return MaxScnNr;
}//_______________________ Koniec funkcji_______________________________________




   
//-----------------------------------------------------------------------------
// Nazwa funkcji :    ScreenNrCount
// Funkcja zlicza u¿ywane ekrany. Odpowiedzi¹ jest suma logiczna u¿ywanych ekranów
//-----------------------------------------------------------------------------
short CheckScreenUse(_ProgramTab *ProgTab)
{
  
      short ScreenUse=1; 
     
      char f=1;  
  
      char sc=0;
      _ScreenPaternDesign *ScreenPaternDesign; 
      while ((f)&&(sc<ScreenNr))
      {
        f=0;
        
             if (sc==0) 
            {
              ScreenPaternDesign=ScreenSetTab_0;
            }else{
              ScreenPaternDesign=ScreenSetTab_1; 
            }  

        for (int bt=0; bt<ScrenButtonNr; bt++)
        {
          if ((ScreenPaternDesign[ProgTab->Screen[sc].PaternDesign].Button[bt].Width>0) && (ProgTab->Screen[sc].Button[bt].OpenPage>0) )
          {
            ScreenUse |=1<<(ProgTab->Screen[sc].Button[bt].OpenPage-1);
            f=1;
          }
        }
        sc++;
        
      }
      
 
  
      return ScreenUse;
  
}//_______________________ Koniec funkcji_______________________________________


     
     
     
void CountLines(_ProgramTab *ProgTab)
{
     char UseLine=0;
        for (char j=0; j<MaxPrcLines; j++)
          for (char i=0; i<MaxPrcInLine; i++) 
            if (ProgTab->Line[j].Proc[i].Type>0) UseLine=j;
    RealPrcLines=UseLine+1; 
}



void RefreshNeuronsList(_ProgramTab *ProgTab)
{
  for (char i=0; i<NumberNeurons; i++)
  {
   if (ProgTab->NeuronsIdTab[i]>0)
   {
       NeuronsList[i].NeuronType=(_NeuronType)((ProgTab->NeuronsIdTab[i])/1000000);
       NeuronsList[i].NeuronID=(ProgTab->NeuronsIdTab[i])-(NeuronsList[i].NeuronType*1000000);
   }else{
       NeuronsList[i].NeuronType=(_NeuronType)0;
       NeuronsList[i].NeuronID=0;
   }
  }
}



//-----------------------------------------------------------------------------
// Nazwa funkcji :    AddRsTask
// Funkcja ustawia dodanie zadania dla transmisji Rs
// Ord - typ rozkazu : odczyt wejœæ, zapis
// Node - nr wêz³a/urz¹dzenia
// Reg  - nr rejestru w wêŸle 
//-----------------------------------------------------------------------------
void AddRsTask(_OrderType Ord, char Node, char Reg)
{ 
     ptr_ZD_MODBUS ZD_MODBUS;
     ZD_MODBUS         =   ZD_MODBUS_0;
     
    char OrdExist=0;
    char RfOrd=0;
    char FreePlace=0;
    
    
    //przepisanie rozkazu na zrozumia³y dla tablicy zadañ Rs
    if (Ord==SI) RfOrd=0x4; 
    if (Ord==SQ) RfOrd=0xF; 

      

          
    //Sprawdzenie czy taki rozkaz nie istnieje w tablicy
    char i=0;
    while ((i<ZD_MODBUS_Tab_Max) && (OrdExist==0))
    {
       if ((ZD_MODBUS[i].Adress==Node) && 
           (ZD_MODBUS[i].Function==RfOrd) &&
           (ZD_MODBUS[i].Target==Reg) &&
           (ZD_MODBUS[i].StartAdr==Node-0))
          {
            OrdExist=1;
          }
       i++;
    }
    
    //je¿eli rozkaz nie istnieje to dodaj
    if (OrdExist==0)
    {
      //znajdŸ wolne miejsce w tablicy zadañ 
      FreePlace=0;
      while ((FreePlace<ZD_MODBUS_Tab_Max) && (ZD_MODBUS[FreePlace].Adress!=0)) FreePlace++;
      
      
      if (FreePlace<ZD_MODBUS_Tab_Max)     //jezeli jest miejsce w tablicy to dodaj
      { 
        ZD_MODBUS[FreePlace].Adress=Node;
        ZD_MODBUS[FreePlace].Function=RfOrd;
        ZD_MODBUS[FreePlace].Target=Reg;
        ZD_MODBUS[FreePlace].StartAdr=Node-0;   //miejsce w pamiêci do pobrania danych (odpowiada nodowi)
        ZD_MODBUS[FreePlace].Coil=16;
        ZD_MODBUS[FreePlace].Error=0x0; 
      }
      
      
    }
    
      

      
                
}//_______________________ Koniec funkcji  AddRsTask _________________





//-----------------------------------------------------------------------------
// Nazwa funkcji :    AddRfTask_SearchNode
// Funkcja ustawia dodanie zadania dla transmisji Rs "Ramka rozg³oœna z wyszukaniem wêz³a"
// Funkcja zastêpuje wszystkie zadanie z rozkazem R_RegOrd (odczytu statusu) rz¹danego wêz³a oraz blokuje odpytywanie wêz³a
// Node - nr wêz³a/urz¹dzeia
//-----------------------------------------------------------------------------
void AddRfTask_SearchNodeTask(char Node)
{ 
    if (NeuronsList[Node-1].ReadSuspend) return;

    for (char i=0; i<RF_Tasks-1; i++)  
    {
       //znalezienie rozkazu odpytania statusu
       if ((RF_TaskTab[i].Node==Node) && 
           (RF_TaskTab[i].Command.ID==NeuronsList[Node-1].NeuronID) &&
           ((RF_TaskTab[i].Command.Order==R_RegOrd) || (RF_TaskTab[i].Command.Order==W_RegOrd)) )
       {
           if (RF_TaskTab[i].Command.Order==R_RegOrd)
           {
              RF_TaskTab[i].Command.Order=R_BC_Order;
           }else{
              RF_TaskTab[i].Command.Order=W_BC_Order;
           }
           NeuronsList[Node-1].ReadSuspend=1;
       }
    } 
}


//-----------------------------------------------------------------------------
// Nazwa funkcji :    AddRfTask_SearchNode
// Funkcja ustawia dodanie zadania dla transmisji Rs "Wy³¹czenie ramki rozg³oœnej z wyszukaniem wêz³a"
// Funkcja w³¹cza ponowne odpytanie wêz³a zablokowanego funkcj¹ AddRfTask_SearchNode
// Node - nr wêz³a/urz¹dzeia
//-----------------------------------------------------------------------------
void AddRfTask_RemoveSearchNodeTask(char Node)
{ 
    for (char i=0; i<RF_Tasks-1; i++)  
    {
       //znalezienie rozkazu odpytania statusu
       if ((RF_TaskTab[i].Node==Node) && 
           (RF_TaskTab[i].Command.ID==NeuronsList[Node-1].NeuronID) &&
           ((RF_TaskTab[i].Command.Order==R_BC_Order) || (RF_TaskTab[i].Command.Order==W_BC_Order)))
       {
           if (RF_TaskTab[i].Command.Order==R_BC_Order)
           {
              RF_TaskTab[i].Command.Order=R_RegOrd;
           }else{
              RF_TaskTab[i].Command.Order=W_RegOrd;
           }
           NeuronsList[Node-1].ReadSuspend=0;
       }
    } 
}


//usuñ wszystkie zadania dla tego wêz³a
char RemoveRfNodeAllTaska( char Node)
{
  for (char TSK=0; TSK<RF_Tasks-1; TSK++)
  {
     if (RF_TaskTab[TSK].Node==Node) //wyczyœæ wêze³
     {
        RF_TaskTab[TSK].Node=0;
        RF_TaskTab[TSK].Command.ID=0;
        RF_TaskTab[TSK].Command.Order=0;
        RF_TaskTab[TSK].Command.Auxi=0;
     }
     
     //przesuñ pozosta³e zadania do góry
     char Tp=TSK;
     while ((Tp+1<RF_Tasks-1) && (RF_TaskTab[Tp+1].Node))
     {
       RF_TaskTab[Tp].Node=RF_TaskTab[Tp+1].Node;
       RF_TaskTab[Tp].Command.ID=RF_TaskTab[Tp+1].Command.ID;
       RF_TaskTab[Tp].Command.Order=RF_TaskTab[Tp+1].Command.Order;
       RF_TaskTab[Tp].Command.Auxi=RF_TaskTab[Tp+1].Command.Order;
       Tp++;
     }
     
     //usuñ pozosta³oœæ po czyszczeniu
     if ((Tp<RF_Tasks-1) && (RF_TaskTab[Tp].Node))
     {
        RF_TaskTab[Tp].Node=0;
        RF_TaskTab[Tp].Command.ID=0;
        RF_TaskTab[Tp].Command.Order=0;
        RF_TaskTab[Tp].Command.Auxi=0; 
     } 
  }  
}




//usuñ wszystkie zadania dla tego wêz³a
char RemoveRfTask(char TSK)
{

      RF_TaskTab[TSK].Node=0;
      RF_TaskTab[TSK].Command.ID=0;
      RF_TaskTab[TSK].Command.Order=0;
      RF_TaskTab[TSK].Command.Auxi=0;

     //przesuñ pozosta³e zadania do góry
     char Tp=TSK;
     while ((Tp+1<RF_Tasks-1) && (RF_TaskTab[Tp+1].Node))
     {
       RF_TaskTab[Tp].Node=RF_TaskTab[Tp+1].Node;
       RF_TaskTab[Tp].Command.ID=RF_TaskTab[Tp+1].Command.ID;
       RF_TaskTab[Tp].Command.Order=RF_TaskTab[Tp+1].Command.Order;
       RF_TaskTab[Tp].Command.Auxi=RF_TaskTab[Tp+1].Command.Auxi;
       Tp++;
     }
     
     //usuñ pozosta³oœæ po czyszczeniu
     if ((Tp<RF_Tasks-1) && (RF_TaskTab[Tp].Node))
     {
        RF_TaskTab[Tp].Node=0;
        RF_TaskTab[Tp].Command.ID=0;
        RF_TaskTab[Tp].Command.Order=0;
        RF_TaskTab[Tp].Command.Auxi=0; 
     } 
 
}



//-----------------------------------------------------------------------------
// Nazwa funkcji :    AddRfTask
// Funkcja ustawia dodanie zadania dla transmisji Rs
// Ord - typ rozkazu : odczyt wejœæ, zapis
// Node - nr wêz³a/urz¹dzeia
// Reg  - nr rejestru w wêŸle 
//-----------------------------------------------------------------------------
// Funkcja zwrca 1- je¿eli powodzenie dodawania zadania
//-----------------------------------------------------------------------------
char AddRfTask(_OrderType Ord, char Node, char Reg)
{ 

    if (NeuronsList[Node-1].NeuronID==0) return 0;
   
    //1-dodano zadanie, 0-niedodano
    char OrdExist=0;
    char RfOrd;
    char FreePlace=0;
    char FirstOccureNode=0xFF;        //ostatnie wyst¹pienie zadania tego wêz³a. 0xFF oznacza ¿e nie znaleziono ¿adnego inneo zadania z tym wêz³em
    char LastOccureNode=0xFF;        //ostatnie wyst¹pienie zadania tego wêz³a. 0xFF oznacza ¿e nie znaleziono ¿adnego inneo zadania z tym wêz³em
   
    //zabezpieczenie przed przekroczeniami
    if (Node>NumberNeurons) return 0;
    if (Reg>99) return 0;
    
    
    //przepisanie rozkazu na zrozumia³y dla tablicy zadañ RF
    switch (Ord)
    {
        case SI:      RfOrd=R_RegOrd; break; 
        case SQ:      RfOrd=W_RegOrd; break; 
        case SNewId:  RfOrd=W_NewIdOrd; break; 
        case SBC:     RfOrd=R_BC_Order; break; 
    }  
 

      
    //Sprawdzenie czy taki rozkaz nie istnieje w tablicy
    char i=0;
    while ((i<RF_Tasks-1) && (OrdExist==0))
    {
       if ((RF_TaskTab[i].Node==Node) && 
           (RF_TaskTab[i].Command.ID==NeuronsList[Node-1].NeuronID) &&
           (RF_TaskTab[i].Command.Order==RfOrd) &&
           (RF_TaskTab[i].Command.Auxi==Reg))
          {
            OrdExist=1;
          }
      
       i++;
    }
    
    i=0;
    char Rf_Zad=0; //iloœæ zadañ.  
    //Odszukanie ostatniego i ewentualnie pierwszego zadania z tym wêz³em - ma to na cenu u³o¿enie zadañ wêz³ów po kolei co jest konieczne do prawid³owego odpytywania sterowników w trybie slave. Sterowniki takie czekaj¹ na wszystkie swoje odpytania a je¿eli wyst¹pi odpytanie innego wêz³a lub ponowne odpytanie rejestru 0 odpytywanego sterownika to prze³¹czaj¹ sie na tryb master, wykonuj¹ w³asne odpytania a nastêpnie prze³¹czaj¹ siê ponownie w trym slave. Pierwszym zadaniem w kolejce zawsze musi byæ odpytanie rejestru 0
    //Obliczenie iloœci zadañ
    while (i<RF_Tasks-1)
    {
         if (RF_TaskTab[i].Node==Node)
         {
           if ((RF_TaskTab[i].Command.Order==R_RegOrd) && (Reg==0) &&  (FirstOccureNode==0xFF))  FirstOccureNode=Rf_Zad; //pierwsze wyst¹pienie zadania tego wêz³a. 0xFF oznacza ¿e nie znaleziono ¿adnego inneo zadania z tym wêz³em
           //if (FirstOccureNode==0xFF)  FirstOccureNode=Rf_Zad; //pierwsze wyst¹pienie zadania tego wêz³a. 0xFF oznacza ¿e nie znaleziono ¿adnego inneo zadania z tym wêz³em
           LastOccureNode=Rf_Zad;                                               //ostatnie wyst¹pienie zadania tego wêz³a. 0xFF oznacza ¿e nie znaleziono ¿adnego inneo zadania z tym wêz³em
         }
         
         if (RF_TaskTab[i].Node!=0) Rf_Zad++;
         i++;    
    }
    if(Rf_Zad>=RF_Tasks-1) return 0;    //je¿eli nie ma ju¿ miejsca na nowe zadania to wyjdŸ z funkcji
    
    
    
    //je¿eli rozkaz nie istnieje to dodaj. Je¿eli na liœcie s¹ ju¿ 
    if (OrdExist==0)
    {
      if (FirstOccureNode!=0xFF)                                                //znajdŸ miejsce w tablicy zadañ dla nowego zadania
      {
         FreePlace=FirstOccureNode;                                              //miejsce w tablicy na nowe zadanie przed dotychczasowymi zadaniami
      }
      else if (LastOccureNode!=0xFF)  
      {
         FreePlace=LastOccureNode+1;                                             //miejsce w tablicy na nowe zadanie za dotychczasowymi zadaniami
      }else{
         FreePlace=Rf_Zad;                                                          //miejsce w tablicy na nowe zadanie
      }
      
      //je¿eli zadanie ma byæ wprowadzone pomiêdzy inne zadania
      if (((FirstOccureNode!=0xFF) && (FirstOccureNode+1>Rf_Zad)) || 
          ((LastOccureNode!=0xFF)  && (LastOccureNode<Rf_Zad)))
      {
        //rozsuniêcie zadañ
        signed char  Pz=Rf_Zad-1;
        while (Pz>=FreePlace)
        {
           RF_TaskTab[Pz+1].Node=RF_TaskTab[Pz].Node;
           RF_TaskTab[Pz+1].Command.ID=RF_TaskTab[Pz].Command.ID;
           RF_TaskTab[Pz+1].Command.Order=RF_TaskTab[Pz].Command.Order;
           RF_TaskTab[Pz+1].Command.Auxi=RF_TaskTab[Pz].Command.Auxi;
           Pz--;
        }
      }
      
       //while ((FreePlace<RF_Tasks) && (RF_TaskTab[FreePlace].Node!=0)) FreePlace++;   //tak by³o do wersji 1.2 w³acznie
      if (FreePlace<RF_Tasks-1)     //jezeli jest miejsce w tablicy to dodaj
      {
        RF_TaskTab[FreePlace].Node=Node;
        RF_TaskTab[FreePlace].Command.ID=NeuronsList[Node-1].NeuronID;
        RF_TaskTab[FreePlace].Command.Order=RfOrd;
        RF_TaskTab[FreePlace].Command.Auxi=Reg;
        
      }
      
      
    }

    return 1; 


}//_______________________ Koniec funkcji  AddRfTask _________________







char AddRfTaskTMP(_OrderType Ord, char Node, char Reg)
{ 

    if (NeuronsList[Node-1].NeuronID==0) return 0;
   
    //1-dodano zadanie, 0-niedodano
    char OrdExist=0;
    char RfOrd;
    char FreePlace=0;
    char FirstOccureNode=0xFF;        //ostatnie wyst¹pienie zadania tego wêz³a. 0xFF oznacza ¿e nie znaleziono ¿adnego inneo zadania z tym wêz³em
    char LastOccureNode=0xFF;        //ostatnie wyst¹pienie zadania tego wêz³a. 0xFF oznacza ¿e nie znaleziono ¿adnego inneo zadania z tym wêz³em
   
    //zabezpieczenie przed przekroczeniami
    if (Node>NumberNeurons) return 0;
    if (Reg>99) return 0;
    
    
    //przepisanie rozkazu na zrozumia³y dla tablicy zadañ RF
    switch (Ord)
    {
        case SI:      RfOrd=R_RegOrd; break; 
        case SQ:      RfOrd=W_RegOrd; break; 
        case SNewId:  RfOrd=W_NewIdOrd; break; 
        case SBC:     RfOrd=R_BC_Order; break; 
    }  
 

     
   /*
    //Sprawdzenie czy taki rozkaz nie istnieje w tablicy
    char i=0;
    while ((i<35) && (OrdExist==0))
    {
       if ((RF_TaskTab[i].Node==Node) && 
           (RF_TaskTab[i].Command.ID==NeuronsList[Node-1].NeuronID) &&
           (RF_TaskTab[i].Command.Order==R_RegOrd) &&
           (RF_TaskTab[i].Command.Auxi==Reg))
          {
            OrdExist=1;
          }
      
       i++;
    }
    */
    Delay(0xFF);
  
 /*
    while ((i<35) && (OrdExist==0))
    {
       if ((RF_TaskTab[i].Node==Node) && 
           (RF_TaskTab[i].Command.ID==NeuronsList[Node-1].NeuronID) &&
           (RF_TaskTab[i].Command.Order==R_RegOrd) &&
           (RF_TaskTab[i].Command.Auxi==Reg))
          {
            OrdExist=1;
          }
      
       i++;
    }
 */ 

    
/*
    i=0;
    char Rf_Zad=0; //iloœæ zadañ.  
    //Odszukanie ostatniego i ewentualnie pierwszego zadania z tym wêz³em - ma to na cenu u³o¿enie zadañ wêz³ów po kolei co jest konieczne do prawid³owego odpytywania sterowników w trybie slave. Sterowniki takie czekaj¹ na wszystkie swoje odpytania a je¿eli wyst¹pi odpytanie innego wêz³a lub ponowne odpytanie rejestru 0 odpytywanego sterownika to prze³¹czaj¹ sie na tryb master, wykonuj¹ w³asne odpytania a nastêpnie prze³¹czaj¹ siê ponownie w trym slave. Pierwszym zadaniem w kolejce zawsze musi byæ odpytanie rejestru 0
    //Obliczenie iloœci zadañ
    while (i<RF_Tasks-1)
    {
         if (RF_TaskTab[i].Node==Node)
         {
           if ((RfOrd==R_RegOrd) && (Reg==0) &&  (FirstOccureNode==0xFF))  FirstOccureNode=Rf_Zad; //pierwsze wyst¹pienie zadania tego wêz³a. 0xFF oznacza ¿e nie znaleziono ¿adnego inneo zadania z tym wêz³em
           LastOccureNode=Rf_Zad;                                               //ostatnie wyst¹pienie zadania tego wêz³a. 0xFF oznacza ¿e nie znaleziono ¿adnego inneo zadania z tym wêz³em
         }
         
         if (RF_TaskTab[i].Node!=0) Rf_Zad++;
         i++;    
    }
    if(Rf_Zad>=RF_Tasks-1) return 0;    //je¿eli nie ma ju¿ miejsca na nowe zadania to wyjdŸ z funkcji
  */  
   /* 
   
    //je¿eli rozkaz nie istnieje to dodaj. Je¿eli na liœcie s¹ ju¿ 
    if (OrdExist==0)
    {
      if (FirstOccureNode!=0xFF)                                                //znajdŸ miejsce w tablicy zadañ dla nowego zadania
      {
         FreePlace=FirstOccureNode;                                              //miejsce w tablicy na nowe zadanie przed dotychczasowymi zadaniami
      }
      else if (LastOccureNode!=0xFF)  
      {
         FreePlace=LastOccureNode+1;                                             //miejsce w tablicy na nowe zadanie za dotychczasowymi zadaniami
      }else{
         FreePlace=Rf_Zad;                                                          //miejsce w tablicy na nowe zadanie
      }
      
      //je¿eli zadanie ma byæ wprowadzone pomiêdzy inne zadania
      if (((FirstOccureNode!=0xFF) && (FirstOccureNode+1>Rf_Zad)) || 
          ((LastOccureNode!=0xFF)  && (LastOccureNode<Rf_Zad)))
      {
        //rozsuniêcie zadañ
        unsigned char  Pz=Rf_Zad-1;
        while (Pz>=FreePlace)
        {
           RF_TaskTab[Pz+1].Node=RF_TaskTab[Pz].Node;
           RF_TaskTab[Pz+1].Command.ID=RF_TaskTab[Pz].Command.ID;
           RF_TaskTab[Pz+1].Command.Order=RF_TaskTab[Pz].Command.Order;
           RF_TaskTab[Pz+1].Command.Auxi=RF_TaskTab[Pz].Command.Auxi;
           Pz--;
        }
      }
      
       //while ((FreePlace<RF_Tasks) && (RF_TaskTab[FreePlace].Node!=0)) FreePlace++;   //tak by³o do wersji 1.2 w³acznie
      if (FreePlace<RF_Tasks-1)     //jezeli jest miejsce w tablicy to dodaj
      {
        RF_TaskTab[FreePlace].Node=Node;
        RF_TaskTab[FreePlace].Command.ID=NeuronsList[Node-1].NeuronID;
        RF_TaskTab[FreePlace].Command.Order=RfOrd;
        RF_TaskTab[FreePlace].Command.Auxi=Reg;
        
      }
      
      
    }
    */
   
  

  /*
  RF_TaskTab[5].Node=Node;
  RF_TaskTab[5].Command.ID=100008;
  RF_TaskTab[5].Command.Order=R_RegOrd;
  RF_TaskTab[5].Command.Auxi=Reg;
  */
    return 1; 


}//_______________________ Koniec funkcji  AddRfTask _________________





//-----------------------------------------------------------------------------
// Nazwa funkcji :    SetTask
// Funkcja ustawia zadania transmisji RS oraz RF
//-----------------------------------------------------------------------------
void SetTask(_ProgramTab *ProgTab)
{ 
      _Procedure Proc; 
      _OrderType SetOrd=NonOrd;
      char       RegOrd=0;
  
      
      //wyczyszczenie tablicy zadañ
      for (char i=0; i<RF_Tasks; i++) 
      {
        RF_TaskTab[i].Node=0;
        RF_TaskTab[i].Command.ID=0;
        RF_TaskTab[i].Command.Order=0;
        RF_TaskTab[i].Command.Auxi=0;
      }
      
      for (char node=0; node<NumberNeurons; node++)
      {
         if((NeuronsList[node].NeuronType!=NoneNodeType) && (NeuronsList[node].NeuronType>=BeginRF) && (NeuronsList[node].NeuronType<=EndRF))  AddRfTask(SI, node+1, 0);
      }
      
      
      for (int LineNr=0; LineNr<RealPrcLines; LineNr++)
      {
        for (int PrcInLine=0; PrcInLine<MaxPrcInLine; PrcInLine++)
        {

              Proc=ProgTab->Line[LineNr].Proc[PrcInLine];
              
              RegOrd=Proc.Tag.RegNr;
              SetOrd=NonOrd;
              //wybór typu rozkazu
              switch (Proc.Type)
              {
                 case ProcWOUT:       SetOrd=SQ;  RegOrd=0; break;
                 case ProcWOUTS:      SetOrd=SQ;  RegOrd=0; break;
                 case ProcWOUTR:      SetOrd=SQ;  RegOrd=0; break;
                 case ProcWOUTC:      SetOrd=SQ;  RegOrd=0; break;
                 case ProcStI:        SetOrd=SI;  RegOrd=0; break;
                 case ProcStNI:       SetOrd=SI;  RegOrd=0; break;
                 case ProcSetReg:     SetOrd=SQ;  RegOrd=Proc.Tag.RegNr; break;
                 case ProcGetReg:     SetOrd=SI;  RegOrd=Proc.Tag.RegNr; break;
              }     
               
              
              //je¿eli rozkaz
              if (SetOrd!=NonOrd)
              {
                    
                      //Je¿eli nody z transmisj¹ RS
                      if (((NeuronsList[Proc.Nr-1].NeuronType>=BeginRS) && (NeuronsList[Proc.Nr-1].NeuronType<=EndRS)) || (NeuronsList[Proc.Nr-1].NeuronType==0))
                      {
                        AddRsTask(SetOrd, Proc.Nr, RegOrd);                //Dodaj rozkaz do tablicy zadañ Modbus
                      }
                      //Je¿eli nody z transmisj¹ RF
                      if ((NeuronsList[Proc.Nr-1].NeuronType>=BeginRF) && (NeuronsList[Proc.Nr-1].NeuronType<=EndRF))
                      {
                         AddRfTask(SetOrd, Proc.Nr, RegOrd);                //Dodaj rozkaz do tablicy zadañ RF
                      }
              }  
              
        }
        
      }
      
      
      
      
      
      //Bezpoœrednie zmienne ekranowe
      volatile _Screen* ScreenLokal;
       _ScreenPaternDesign *ScreenPaternDesign;

      
       for (char Sheet=0; Sheet<ScreenNr; Sheet++)
       {
           if (ScreenUse&1<<Sheet)
           {
              if (Sheet==0) 
              {
                ScreenPaternDesign=ScreenSetTab_0;
              }else{
                ScreenPaternDesign=ScreenSetTab_1; 
              } 
             
             ScreenLokal=&gProg.Screen[Sheet];
             for (char i=0; i<ScrenAnalogNr; i++)
                {

                      if ( (ScreenPaternDesign[ScreenLokal->PaternDesign].Value[i].Width>0) && ((ScreenLokal->Analog[i].Nr>0) || ((ScreenLokal->Analog[i].Nr==0)&&(ScreenLokal->Analog[i].Reg>0))))
                       {
                          AddRfTask(SI, ScreenLokal->Analog[i].Nr, ScreenLokal->Analog[i].Reg);
                       }
                }
             for (char i=0; i<ScrenGraphNr; i++)
                {
                      if ((ScreenPaternDesign[ScreenLokal->PaternDesign].Graph[i].Width>0) && ((ScreenLokal->Graph[i].Type==GraphOnOff) || (ScreenLokal->Graph[i].PlusOnOff==1) || (ScreenLokal->Graph[i].Type==GraphWartosc)))
                       {
                          AddRfTask(SI, ScreenLokal->Graph[i].Nr, ScreenLokal->Graph[i].Reg);
                       }
                }
             
             //je¿eli na stronie jest termostat
             if (((Sheet==0) && (gProg.Screen[Sheet].PaternDesign==MainScreenTermo)) || 
                    ((Sheet) && (gProg.Screen[Sheet].PaternDesign==SubScreenTermo)))
              {
              
             if (ScreenLokal->Thermo.ActorNode>0)
                       {
                          AddRfTask(SQ, ScreenLokal->Thermo.ActorNode, ScreenLokal->Thermo.ActorPort);
                       }   
             if (ScreenLokal->Thermo.SensorNode>0)
                       {
                          AddRfTask(SI, ScreenLokal->Thermo.SensorNode, ScreenLokal->Thermo.SensorReg);
                       }  
             
              }
           
           }

       }
      //-------------------------------
      
      
      //Dodanie standardowego odpytywania wez³ów nie u¿ywanych w programie
       /*
       char ExstF=0;
      
      for (char i=0; i<NumberNeurons; i++)
      {
        ExstF=0;
        if (NeuronsList[i].NeuronID!=0)
        {
           for (char j=0; j<RF_Tasks; j++)
           {
             if (RF_TaskTab[j].Command.ID==0)  //je¿eli brak zadañ to wyjdŸ
             { 
               j=RF_Tasks;
             }else if (RF_TaskTab[j].Command.ID==NeuronsList[i].NeuronID)    //je¿eli zadanie dla tego noda istnieje
             {
               ExstF=1;   //ustaw flagê
             }
           }
           
           //je¿eli nie znaleziono zadania dla wez³a przypisz pytanie standardowe
           if (ExstF==0)
           {
               AddRfTask(SI, i+1, 0);                //Dodaj pytanie standardowe do tablicy zadañ RF
           }
        }
        
      }
       */
     
      
      
                
}//_______________________ Koniec funkcji  SetTask_________________








//-----------------------------------------------------------------------------
// Nazwa funkcji :    ProgramChangeSave
// Funkcja zapisu zmian w programie u¿ytkowym
//-----------------------------------------------------------------------------
void ProgramChangeExecute(_ProgramTab *ProgTab)
{
   //obliczenie iloœci u¿ywanych scen
   NumberScene=SceneNrCount(ProgTab);
   
   //obliczenie iloœci u¿ywanych linii
   CountLines(ProgTab);
   
   //obliczenie iloœci u¿ywanych stron ekranowych
   ScreenUse=CheckScreenUse(ProgTab);
   

   
   //odœwierzenie/przepisanie listy neuronów
   RefreshNeuronsList(ProgTab);
   
   
   //ustawienie zadañ Rs i RF
   SetTask(ProgTab);
  
}//_______________________ Koniec funkcji zapisu zmian programu_________________






//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    GetBinVarPack
//* Funkcja zwraca paczkê w tablicy wartoœci zmienncyh dwustanowych z zakresu B000 - B700
//*-----------------------------------------------------------------------------
void GetBinVarPack(WORD adr, WORD Length,unsigned char *Tab )
{
  /*
        WORD  RamByte=(adr/8);
        WORD  RamBit= (adr) % 8;
        char  CurentByte;
        char  CurentBit;  
  
    
      for (WORD i=0; i<Length; i++)
            {
               Tab[CurentByte] &= ~(1<<CurentBit);
               Tab[CurentByte] |=  (((ram.BinVarRAM[RamByte]>>RamBit) & 0x1)<< CurentBit);
                         
               if (CurentBit<7) {CurentBit++;} else {CurentBit=0; CurentByte++;}
               if (RamBit<7) {RamBit++;} else {RamBit=0; RamByte++;}
            }  
  
 */
}//_______________________ Koniec funkcji GetBinVar ____________________________









//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    GetBinVar
//* Funkcja przepisuje paczkê wartoœci zmiennych dwustanowej komunikacyjnej DA00 - DDFF do Tab
//*-----------------------------------------------------------------------------
void GetBinVarPackComm(WORD adr, WORD Length, unsigned char* Tab )
{
        WORD  RamByte=adr;
        WORD  RamBit= 0;
        char  CurentByte = 0;
        char  CurentBit = 0;  

        for (char i=0; i<16; i++)  {Tab[i]=0;}
     
    
      for (WORD i=0; i<Length; i++)
            {
               if ((ram.VarCom_Word[RamByte]>>RamBit) & 0x1) { Tab[CurentByte] |= 0x1<<CurentBit;} else {Tab[CurentByte] &= ~(0x1<<CurentBit);}
              
               
               if (CurentBit<7) {CurentBit++;} else {CurentBit=0; CurentByte++; Tab[CurentByte]=0;}
               if (RamBit<7) {RamBit++;} else {RamBit=0; RamByte++;}
            }  
}//_______________________ Koniec funkcji GetBinVarComm ________________________


//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    GetRegPackComm
//* Funkcja przepisuje StageQ
//*   Adr - nr StageQ
//*   Reg - nr rejestru
//*   LengthBit - ile bitów do kopiowania je¿eli wiêcej ni¿ 16 to kopiuje z nastêpnego rejestru
//*-----------------------------------------------------------------------------
void GetRegPackComm(short int  Adr, char Reg, char LengthBit, unsigned char* Tab )
{
        WORD  RamWord=Reg;
        WORD  RamBitInWord= 0;
        char  CurentByte = 0;
        char  CurentBit = 0;  
 
        Adr+=1;
        
        for (char i=0; i<16; i++)  {Tab[i]=0;}
     
    
      for (WORD i=0; i<LengthBit; i++)
            {
               if ((MainStage.StageQ[Adr].Value[RamWord]>>RamBitInWord) & 0x1) { Tab[CurentByte] |= 0x1<<CurentBit;} else {Tab[CurentByte] &= ~(0x1<<CurentBit);}
              
               
               if (CurentBit<7) {CurentBit++;} else {CurentBit=0; CurentByte++; Tab[CurentByte]=0;}
               if (RamBitInWord<15) {RamBitInWord++;} else {RamBitInWord=0; RamWord++;}
            }  
}//_______________________ Koniec funkcji GetRegPackComm ________________________






//****************************************************************************//
//****************************************************************************//
//*************************** P R O C E D U R Y ******************************//
//****************************************************************************//
//****************************************************************************//











