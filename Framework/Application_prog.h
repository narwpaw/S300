//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                                 CEURON                                   *//                       
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : Application_prog.h                       *//
//*  Opis                         : Plik nag³ówkowy dla Application_prog.c   *//
//*  Autor                        : Pawe³ Narwojsz                           *//
//*  Data utrzorzenia             : 10.12.2009                               *//
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
#ifndef _Application_prog_
#define _Application_prog_



#include "RF.h"
#include "ScreenMENU2.h"





#define ModeS200 1
#define ModeS300 2



//Definicje --------------------------------------------------------------------
    #define NumberNeurons     35    //iloœæ neuronów
    #define NumberReg         11    //iloœæ rejestrów pojedyñczego wêz³a        //11 rejestr to info o wersji
    #define NumberNeuronsPort      16    //iloœæ wejœæ dwustanowych w urz¹dzeniu
    #define NumberNeuronsPort       16    //iloœæ wejœæ w urz¹dzeniu
    #define NumberSceneMax    10    //maksymalna iloœæ scen (scena 0=off)
    #define NodeLabelLethers         10   //d³ugoœæ opiski noda 
    #define NumberButton      ScreenNr*10    //iloœæ przycisków ekranowych

    #define MaxPrcInLine      10     //maksymalna iloœæ procedur w jednym rzêdzie
    #define MaxPrcLines       200    //maksymalna iloœæ linii
    #define MaxPort           NumberNeuronsPort     //maksymalna iloœæ portów w urz¹dzeniu
    #define TimeUnitQual      600   // [ms] Jednostka jakoœci. Okres pomiaru jakoœci po³¹czenia z wêz³ami. 100% jakoœæ wystêpuje kiedy sterownik nawi¹zujê przynajmniej jedno po³¹czenie w ka¿dej jednostce czasowej TimeUnitQual    
    #define ThermostatHysteresis  2 //histereza termostatu 0.3st.
    //#define ChangeStageFlag    10   //ile razy (maksymalnie) sterownik ma wys³aæ zmiane stanu wyjœcia
    #define _ProgFormatTyp  ModeS300    //format programu
    #define ProgramPeriod     350   //[ms]    
    #define AnalogVarAmount   20
    #define CHANGE_STAGE_TIME  2    //[s]
    //zmienne analogowe
    //* Value - 16 bit  - (-3276.8 % 3276.7) skalowane na zakres -3276.8 % 3276.7
    

   

    //definicje typów procedór
    typedef enum {
                  None=0,
                  ProcLogStart,       //--------------------
                    ProcConect,         //przeniesienie stanu na lewo
                    ProcDown,           //przeniesienie stanu w dó³
                    ProcDownRight,       //przeniesienie stanu w dó³ i prawo
                    ProcSumLogDown,      //suma prawo i góra, przeniesienie  stanu w dó³
                    ProcSumLogRight,     //suma prawo i góra, przeniesienie  stanu w prawo
                    ProcUpToRight,       //przeniesienie stanu z góry w prawo
                    ProcUpToRightDown,   //przeniesienie stanu z góry w prawo i dó³
                    ProcNOT,            //przeniesienie stanu z negacj¹
                    ProcToggleRS,       //Procedura przerzutnika RS
                    ProcToggleT,        //Procedura przerzutnika T
                    ProcTLI1,           //procedura impulsu jednocyklowego
                  ProcLogStop,        //--------------------
                  REZ0,REZ1,REZ2,REZ3,REZ4,         //rezerwa
                  ProcStStart,        //--------------------
                    ProcStI,            //procedura stan wejœcia (RIN)
                    ProcStNI,            //procedura stan wejœcia zanegowany (NRIN)
                    ProcStQ,            //procedura stan wyjœcia
                    ProcStB,            //procedura stan przycisku
                    ProcStS,            //procedura stan sceny
                  ProcStStop,        //--------------------
                  REZ10,REZ11,REZ12,REZ13,REZ14,      //rezerwa
                  ProcSetStart,      //--------------------
                    ProcWOUT,           //Ustawienie wyjœcia
                    ProcWOUTS,           //Ustawienie wyjœcia na 1
                    ProcWOUTR,          //Ustawienie wyjœcia na 0
                    ProcWOUTC,          //zmiana stanu wyjœcia na przeciwny
                    ProcSetScn,         //Ustawienie konkretnej sceny (nr sceny w polu Tag.NrScene)
                    ProcInc,         //Inkrementacja sceny Dzia³a na zbocze narastaj¹ce wejœcia.
                    ProcDec,         //dekrementacja sceny Dzia³a na zbocze narastaj¹ce wejœcia.
                    ProcSPK,         //procedura g³oœnika
                  ProcSetStop,       //--------------------
                  REZ20,REZ21,REZ22,         //rezerwa
                  ProcTimeStart,     //--------------------
                    ProcTimerSet,     //Uruchomienie odliczania timera. Po odliczeniu ustawia wyjœcie na 1
                    ProcClock,          //procedura budzika zegarowego
                    ProcDay,            //procedura dnia tygodnia
                  ProcTimeStop,      //--------------------
                  REZ30,REZ31,REZ32,REZ33,REZ34,         //rezerwa
                  ProcAnalogStart,    //--------------------
                    ProcConstAnal,          //Ustawiwna wartoœæ analogu
                    ProcGetReg,            //Procedura pobrania wartoœci analogowej   wejœcia
                    ProcSetReg,            //Procedura ustawienia wartoœci analogowej wyjœcia
                    ProcLim,                //Procedura porównania je¿eli 1>2 Q=1
                    ProcEven,               //Procedura równoœci je¿eli 1=2 Q=1
                    ProcPlus,
                    ProcMinus,
                    ProcMul,
                    ProcDiv,
                  ProcAnalogStop,     //--------------------

                  
    } _ProcType;
//___________________________Koniec definicji___________________________________


    //definicje typów procedór czasowych
    typedef enum {
      PrcMounth=0,
      PrcDay,
      PrcHour,
      PrcMinute,
      PrcSecond,
    }_ClockProcType;


    //definicje typów ni tygodnia
    typedef enum {
      Monday=1,
      Tuesday,
      Wednesday,
      Thursday,
      Friday,
      Saturday,
      Sunday
    }_DayOfWeekType;
    



 //zmienna dla Zegara+
typedef struct{  
       char  Year;
       char  Mounth;
       char  Day;
       char  Hour;
       char  Minute;
       char  Second;
       char  DayOfWeek;
      }_DateTime;





typedef enum {IncScene,
              DecScene
              }_IncDecScene;


//typ uniwersalnej zmiennej procedury


 typedef union {
                unsigned long long     Tag;            //zmienna dodatkowa uniwersalna 4B
                unsigned char          TagB[8];         //4 zmienne dodatkowe uniwersale 1B
                signed short int           ConstAnValue;    //sta³a analogowa 
                char                   SpeakerType;     //typ dŸwiêku
                                            //zmienna dla wejœæ/wyjœæ
                char Port;                  //Numer portu
                char RegNr;                 //Numer rejestru      
                
                struct{
                  unsigned int SetCounter;               //zmienna dla Timetra
                  unsigned int CounterStartTime;                //sekunda startowa timera wed³ug 
                };
                              //zmienna dla Zegara+
                struct{
                _ClockProcType  ClkPrcType;
                char            ClkPrcVal;  
                };
                char DayOfWeek;                       //dzieñ tygodnia
                char NrScene;
              
                struct{                             //zmienne dla procedury IncDecScene
                      _IncDecScene IncDecScene;         //ustawienie inkrementacji lub dekrementacji
                      
                };
                }_Tag;






//Typ danych opisuj¹cy stan logiczny wejœæia/wyjœcia/sceny itp
 typedef union
{
    signed short int  Value[NumberReg];
    unsigned short int  Port;
    short int  Curent : 1,
               Last   : 1;
}_StateId;


//struktóra opisuj¹ca procedurê
 typedef struct __Procedure
{
   _ProcType Type; //typ procedury
   signed char  Nr;   //nr. wêz³a, nr. timera itp, nr sceny
   _Tag      Tag;
   char      LastIn;                     //poprzedni stan wejœcia (w niektórych procedurach zabezpieczenie np przed wielokrotn¹ incrementacj¹/dec.) 
   signed short int Out;
}_Procedure;


//linia w tablic procedur
typedef struct __Line{
                 _Procedure  Proc[MaxPrcInLine];   //procedury               
}_Line;


//typy neuronów
typedef enum {
                  NoneNodeType=0,
                  BeginRS=1,
                    NodeRS=1,             //Node uniwersalny, stale w³¹czony, z komunikacj¹ RS485
                  BeginRF=2,  
                    NodeRSRF=2,           //Node uniwersalny, stale w³¹czony, z komunikacj¹ RS485 oraz komunikacja radiowa
                  EndRS=2,
                    NodeRF=3,             //Node uniwersalny, stale w³¹czony, komunikacja radiowa
                    NodeRF_Remote=4,      //Pilot, komunikacja radiowa (urz¹dzenie baterjne)
                    NodeRF_Period=5,      //Czujnik za³¹czany okresowo, komunikacja radiowa (urz¹dzenie bateryjne)
                  EndRF=5,
                  NodeTEST=9,
}_NeuronType;
            




//typy rozkazów 
typedef enum {
                  NonOrd=0,         //Bez rozkazu
                  SI=1,             //Odczyt wejœæ/rejestrów wyjœciowych
                  SQ=2,             //Zapis rejestów/wyjœæ
                  SQI=3,            //Odczyt+Zapis rejestów/wyjœæ
                  SNewId=4,         //Ustaw nowy numer ID
                  SBC=5,            //Ramka rozg³oœna  
}_OrderType;


typedef struct{
                  char  Enable;
                  char  Row;
                  char  Col;
                  char  Activ;
                  char  Change;
               }_Trap;



//lista neuronów
typedef struct __NeuronsListUnit{
                _NeuronType   NeuronType;   //typ neuronu
                int           NeuronID;     //numer Id neuronu bez pierwszej cyfry oznaczaj¹cej typ
                char  ReadOften    : 1, 
                      ReadRarely   : 1,
                      ReadSuspend  : 1, //wstrzymanie nadawania do tego wêz³a
                      IndirectCon  : 1, //po³¹czenie poœrednie - wykonywane za pomoc¹ innego wêz³a 
                      Bridge       : 1, //po³¹czenie mostowe - oznacza ¿e ten wêze³ jest lub prubuje byæ poœrednikiem pomiêdzy dwoma wêz³em nadrzêdnym a trzecim wêz³em
                      rez1         : 1,
                      rez2         : 1,
                      rez3         : 1;
}_NeuronsListUnit;


#define ProgPreambu 0xAA

//tablica procedur
typedef  struct  __ProgramTab{
                 char       Preambu;                         //preambua programu
                 char       ProgFormatTyp;                   //format programu
                 unsigned short int Size;                        //wielkoœæ programu
                 char       ModyfiData[19];                  //data ostatniej zmiany programu       
                 _Line      Line[MaxPrcLines];               //linie procedur   
                 _Screen    Screen[ScreenNr];                //ekrany konfigurowalne  (160B*ScreenNr)
                 int        NeuronsIdTab[NumberNeurons];     //typ neuronu  + neuron ID
                 char       NodeLabel[NumberNeurons][NodeLabelLethers];
                 char       Rez[1000];                       //rezerwa dla PC (edytora komputerowego) 
}_ProgramTab, _pProgramTab;


#define ProgramTabSize sizeof(_ProgramTab)


typedef struct
{ 
  char          MensurIndex;            //aktualny indeks dla Mensur inkrementowany po ka¿dym okresie TimeUnitQual   
  char          SlowMensurIndex;              //aktualny indeks dla Mensur inkrementowany po ka¿dym okresie SlowTimeUnitQual  
  char          QuickMensurIndex;              //aktualny indeks dla Mensur inkrementowany po ka¿dym okresie SlowTimeUnitQual  
  char          SlowMensur[NumberNeurons];    //[ocena w okresie (4 minuty)] pomiar jakoœci po³¹czenia na jednostkê czasu 1minuta. Ka¿dy bit odpowiada nawi¹zaniu przynajmniej jednego po³¹czenia w okresie 1min
  char          QuickMensur[NumberNeurons];  //pomiar jakoœci po³¹czenia -krótki
#ifdef  DIRECT_Q_MESURE 
  unsigned short DirectkMensur[NumberNeurons];  //bezpoœredni pomiar jakoœci po³aczenia (iloœc b³ednych odpytañ od czsu uruchomienia sterownika)
#endif  
  unsigned int  Mensur[NumberNeurons];        //pomiar jakoœci po³¹czenia na jednostkê czasu TimeUnitQual. Czas pomiaru = 32*TimeUnitQual [ms]. Ka¿dy bit odpowiada nawi¹zaniu przynajmniej jednego po³¹czenia w okresie TimeUnitQual
}_TransQuality;





//struktóra wykonawcza dla urz¹dzeñ wejœciowych/wyjœciowych/scen
 typedef struct {
                char              StageButton[NumberButton];       //stan przycisków   
                char              StageButtonExe[NumberButton];       //stan przycisków opóŸnione dla programu
                _StateId          StageI[NumberNeurons];                //stan wejœæ  
                _StateId          StageQ[NumberNeurons];                 //stan wyjœæ
                unsigned short    StageQImpulse[NumberNeurons];          //flaga impulsu wyjœæ
                char              SceneNr;                         //nr aktualnej sceny   
                char              SendStageChange[NumberNeurons];  //wyœlij zmianê stanu (1-w jednym cyklu 2-w dwuch cyklach itd)
}_MainStage;









void ProcStI_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg);
void ProcStNI_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg);
void ProcGetAnal_Action(_Procedure *Proc, _MainStage *MainStg);
void ProcSetAnal_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg);
void ProcStT_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg);
void ProcStA_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg);
void ProcStQ_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg);
void ProcSPK_Action(short int *PrvOut, _Procedure *Proc);
void ProcWOUT_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg);
void ProcWOUTS_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg);
void ProcWOUTR_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg);
void ProcSetT_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg);
void ProcSetA_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg);
void ProcNOT_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg);
void ProcToggleRS_Action(short int *PrvOut, short int *UpOut, _Procedure *Proc);
void ProcToggleT_Action(short int *PrvOut, _Procedure *Proc);
void ProcTLI1_Action(short int *PrvOut, _Procedure *Proc);
void ProcSetScn_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg);
void ProcIncDec_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg);
void ProcSumL_Action(short int *PrvOut, short int *UpOut, _Procedure *Proc);
void ProcStartTimer_Action(short int *PrvOut, _Procedure *Proc, _MainStage *MainStg);
void ProcClock_Action(short int *PrvOut, _Procedure *Proc);
void ProcDay_Action(short int *PrvOut, _Procedure *Proc);
char SceneNrCount(_ProgramTab *ProgTab);
void ProgramChangeExecute(_ProgramTab *ProgTab);
void RamToFlash(void);
char PortStageRead(short int Stage, char PortNr);
void PortStageSet(short int *Stage, char PortNr, char Value);
void Program (_ProgramTab *ProgTab); 
void ProcConstAnal_Action(_Procedure *Proc);
void ProcLim_Action(short int *PrvOut, short int *UpOut, _Procedure *Proc);
void ProcEven_Action(short int *PrvOut, short int *UpOut, _Procedure *Proc);
void GetRegPackComm(short int Adr, char Reg, char LengthBit, unsigned char* Tab );
void CountLines(_ProgramTab *ProgTab);
void RefreshNeuronsList(_ProgramTab *ProgTab);
void SetTask(_ProgramTab *ProgTab);
char AddRfTask(_OrderType Ord, char Node, char Reg);
char RemoveRfTask(char TSK);
void AddRSTask(_OrderType Ord, char Node, char Reg);
void ThermostatsExecute(void);
void AddRfTask_SearchNodeTask(char Node);
void AddRfTask_RemoveSearchNodeTask(char Node);
#endif
