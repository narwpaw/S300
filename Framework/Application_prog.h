//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                                 CEURON                                   *//                       
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : Application_prog.h                       *//
//*  Opis                         : Plik nag��wkowy dla Application_prog.c   *//
//*  Autor                        : Pawe� Narwojsz                           *//
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
    #define NumberNeurons     35    //ilo�� neuron�w
    #define NumberReg         11    //ilo�� rejestr�w pojedy�czego w�z�a        //11 rejestr to info o wersji
    #define NumberNeuronsPort      16    //ilo�� wej�� dwustanowych w urz�dzeniu
    #define NumberNeuronsPort       16    //ilo�� wej�� w urz�dzeniu
    #define NumberSceneMax    10    //maksymalna ilo�� scen (scena 0=off)
    #define NodeLabelLethers         10   //d�ugo�� opiski noda 
    #define NumberButton      ScreenNr*10    //ilo�� przycisk�w ekranowych

    #define MaxPrcInLine      10     //maksymalna ilo�� procedur w jednym rz�dzie
    #define MaxPrcLines       200    //maksymalna ilo�� linii
    #define MaxPort           NumberNeuronsPort     //maksymalna ilo�� port�w w urz�dzeniu
    #define TimeUnitQual      600   // [ms] Jednostka jako�ci. Okres pomiaru jako�ci po��czenia z w�z�ami. 100% jako�� wyst�puje kiedy sterownik nawi�zuj� przynajmniej jedno po��czenie w ka�dej jednostce czasowej TimeUnitQual    
    #define ThermostatHysteresis  2 //histereza termostatu 0.3st.
    //#define ChangeStageFlag    10   //ile razy (maksymalnie) sterownik ma wys�a� zmiane stanu wyj�cia
    #define _ProgFormatTyp  ModeS300    //format programu
    #define ProgramPeriod     350   //[ms]    
    #define AnalogVarAmount   20
    #define CHANGE_STAGE_TIME  2    //[s]
    //zmienne analogowe
    //* Value - 16 bit  - (-3276.8 % 3276.7) skalowane na zakres -3276.8 % 3276.7
    

   

    //definicje typ�w proced�r
    typedef enum {
                  None=0,
                  ProcLogStart,       //--------------------
                    ProcConect,         //przeniesienie stanu na lewo
                    ProcDown,           //przeniesienie stanu w d�
                    ProcDownRight,       //przeniesienie stanu w d� i prawo
                    ProcSumLogDown,      //suma prawo i g�ra, przeniesienie  stanu w d�
                    ProcSumLogRight,     //suma prawo i g�ra, przeniesienie  stanu w prawo
                    ProcUpToRight,       //przeniesienie stanu z g�ry w prawo
                    ProcUpToRightDown,   //przeniesienie stanu z g�ry w prawo i d�
                    ProcNOT,            //przeniesienie stanu z negacj�
                    ProcToggleRS,       //Procedura przerzutnika RS
                    ProcToggleT,        //Procedura przerzutnika T
                    ProcTLI1,           //procedura impulsu jednocyklowego
                  ProcLogStop,        //--------------------
                  REZ0,REZ1,REZ2,REZ3,REZ4,         //rezerwa
                  ProcStStart,        //--------------------
                    ProcStI,            //procedura stan wej�cia (RIN)
                    ProcStNI,            //procedura stan wej�cia zanegowany (NRIN)
                    ProcStQ,            //procedura stan wyj�cia
                    ProcStB,            //procedura stan przycisku
                    ProcStS,            //procedura stan sceny
                  ProcStStop,        //--------------------
                  REZ10,REZ11,REZ12,REZ13,REZ14,      //rezerwa
                  ProcSetStart,      //--------------------
                    ProcWOUT,           //Ustawienie wyj�cia
                    ProcWOUTS,           //Ustawienie wyj�cia na 1
                    ProcWOUTR,          //Ustawienie wyj�cia na 0
                    ProcWOUTC,          //zmiana stanu wyj�cia na przeciwny
                    ProcSetScn,         //Ustawienie konkretnej sceny (nr sceny w polu Tag.NrScene)
                    ProcInc,         //Inkrementacja sceny Dzia�a na zbocze narastaj�ce wej�cia.
                    ProcDec,         //dekrementacja sceny Dzia�a na zbocze narastaj�ce wej�cia.
                    ProcSPK,         //procedura g�o�nika
                  ProcSetStop,       //--------------------
                  REZ20,REZ21,REZ22,         //rezerwa
                  ProcTimeStart,     //--------------------
                    ProcTimerSet,     //Uruchomienie odliczania timera. Po odliczeniu ustawia wyj�cie na 1
                    ProcClock,          //procedura budzika zegarowego
                    ProcDay,            //procedura dnia tygodnia
                  ProcTimeStop,      //--------------------
                  REZ30,REZ31,REZ32,REZ33,REZ34,         //rezerwa
                  ProcAnalogStart,    //--------------------
                    ProcConstAnal,          //Ustawiwna warto�� analogu
                    ProcGetReg,            //Procedura pobrania warto�ci analogowej   wej�cia
                    ProcSetReg,            //Procedura ustawienia warto�ci analogowej wyj�cia
                    ProcLim,                //Procedura por�wnania je�eli 1>2 Q=1
                    ProcEven,               //Procedura r�wno�ci je�eli 1=2 Q=1
                    ProcPlus,
                    ProcMinus,
                    ProcMul,
                    ProcDiv,
                  ProcAnalogStop,     //--------------------

                  
    } _ProcType;
//___________________________Koniec definicji___________________________________


    //definicje typ�w proced�r czasowych
    typedef enum {
      PrcMounth=0,
      PrcDay,
      PrcHour,
      PrcMinute,
      PrcSecond,
    }_ClockProcType;


    //definicje typ�w ni tygodnia
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
                signed short int           ConstAnValue;    //sta�a analogowa 
                char                   SpeakerType;     //typ d�wi�ku
                                            //zmienna dla wej��/wyj��
                char Port;                  //Numer portu
                char RegNr;                 //Numer rejestru      
                
                struct{
                  unsigned int SetCounter;               //zmienna dla Timetra
                  unsigned int CounterStartTime;                //sekunda startowa timera wed�ug 
                };
                              //zmienna dla Zegara+
                struct{
                _ClockProcType  ClkPrcType;
                char            ClkPrcVal;  
                };
                char DayOfWeek;                       //dzie� tygodnia
                char NrScene;
              
                struct{                             //zmienne dla procedury IncDecScene
                      _IncDecScene IncDecScene;         //ustawienie inkrementacji lub dekrementacji
                      
                };
                }_Tag;






//Typ danych opisuj�cy stan logiczny wej��ia/wyj�cia/sceny itp
 typedef union
{
    signed short int  Value[NumberReg];
    unsigned short int  Port;
    short int  Curent : 1,
               Last   : 1;
}_StateId;


//strukt�ra opisuj�ca procedur�
 typedef struct __Procedure
{
   _ProcType Type; //typ procedury
   signed char  Nr;   //nr. w�z�a, nr. timera itp, nr sceny
   _Tag      Tag;
   char      LastIn;                     //poprzedni stan wej�cia (w niekt�rych procedurach zabezpieczenie np przed wielokrotn� incrementacj�/dec.) 
   signed short int Out;
}_Procedure;


//linia w tablic procedur
typedef struct __Line{
                 _Procedure  Proc[MaxPrcInLine];   //procedury               
}_Line;


//typy neuron�w
typedef enum {
                  NoneNodeType=0,
                  BeginRS=1,
                    NodeRS=1,             //Node uniwersalny, stale w��czony, z komunikacj� RS485
                  BeginRF=2,  
                    NodeRSRF=2,           //Node uniwersalny, stale w��czony, z komunikacj� RS485 oraz komunikacja radiowa
                  EndRS=2,
                    NodeRF=3,             //Node uniwersalny, stale w��czony, komunikacja radiowa
                    NodeRF_Remote=4,      //Pilot, komunikacja radiowa (urz�dzenie baterjne)
                    NodeRF_Period=5,      //Czujnik za��czany okresowo, komunikacja radiowa (urz�dzenie bateryjne)
                  EndRF=5,
                  NodeTEST=9,
}_NeuronType;
            




//typy rozkaz�w 
typedef enum {
                  NonOrd=0,         //Bez rozkazu
                  SI=1,             //Odczyt wej��/rejestr�w wyj�ciowych
                  SQ=2,             //Zapis rejest�w/wyj��
                  SQI=3,            //Odczyt+Zapis rejest�w/wyj��
                  SNewId=4,         //Ustaw nowy numer ID
                  SBC=5,            //Ramka rozg�o�na  
}_OrderType;


typedef struct{
                  char  Enable;
                  char  Row;
                  char  Col;
                  char  Activ;
                  char  Change;
               }_Trap;



//lista neuron�w
typedef struct __NeuronsListUnit{
                _NeuronType   NeuronType;   //typ neuronu
                int           NeuronID;     //numer Id neuronu bez pierwszej cyfry oznaczaj�cej typ
                char  ReadOften    : 1, 
                      ReadRarely   : 1,
                      ReadSuspend  : 1, //wstrzymanie nadawania do tego w�z�a
                      IndirectCon  : 1, //po��czenie po�rednie - wykonywane za pomoc� innego w�z�a 
                      Bridge       : 1, //po��czenie mostowe - oznacza �e ten w�ze� jest lub prubuje by� po�rednikiem pomi�dzy dwoma w�z�em nadrz�dnym a trzecim w�z�em
                      rez1         : 1,
                      rez2         : 1,
                      rez3         : 1;
}_NeuronsListUnit;


#define ProgPreambu 0xAA

//tablica procedur
typedef  struct  __ProgramTab{
                 char       Preambu;                         //preambua programu
                 char       ProgFormatTyp;                   //format programu
                 unsigned short int Size;                        //wielko�� programu
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
  char          MensurIndex;            //aktualny indeks dla Mensur inkrementowany po ka�dym okresie TimeUnitQual   
  char          SlowMensurIndex;              //aktualny indeks dla Mensur inkrementowany po ka�dym okresie SlowTimeUnitQual  
  char          QuickMensurIndex;              //aktualny indeks dla Mensur inkrementowany po ka�dym okresie SlowTimeUnitQual  
  char          SlowMensur[NumberNeurons];    //[ocena w okresie (4 minuty)] pomiar jako�ci po��czenia na jednostk� czasu 1minuta. Ka�dy bit odpowiada nawi�zaniu przynajmniej jednego po��czenia w okresie 1min
  char          QuickMensur[NumberNeurons];  //pomiar jako�ci po��czenia -kr�tki
#ifdef  DIRECT_Q_MESURE 
  unsigned short DirectkMensur[NumberNeurons];  //bezpo�redni pomiar jako�ci po�aczenia (ilo�c b�ednych odpyta� od czsu uruchomienia sterownika)
#endif  
  unsigned int  Mensur[NumberNeurons];        //pomiar jako�ci po��czenia na jednostk� czasu TimeUnitQual. Czas pomiaru = 32*TimeUnitQual [ms]. Ka�dy bit odpowiada nawi�zaniu przynajmniej jednego po��czenia w okresie TimeUnitQual
}_TransQuality;





//strukt�ra wykonawcza dla urz�dze� wej�ciowych/wyj�ciowych/scen
 typedef struct {
                char              StageButton[NumberButton];       //stan przycisk�w   
                char              StageButtonExe[NumberButton];       //stan przycisk�w op�nione dla programu
                _StateId          StageI[NumberNeurons];                //stan wej��  
                _StateId          StageQ[NumberNeurons];                 //stan wyj��
                unsigned short    StageQImpulse[NumberNeurons];          //flaga impulsu wyj��
                char              SceneNr;                         //nr aktualnej sceny   
                char              SendStageChange[NumberNeurons];  //wy�lij zmian� stanu (1-w jednym cyklu 2-w dwuch cyklach itd)
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
