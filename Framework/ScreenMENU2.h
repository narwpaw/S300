//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                                 CEURON                                   *//                      
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : ScreenMenu.h                             *//
//*  Opis                         : Biblioteka obslugi grafiki               *//
//*  Autor                        : Pawe³ Narwojsz                           *//
//*  Data utrzorzenia             : 19.09.2007                               *//
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


#ifndef _ScreenMenu_
#define _ScreenMenu_ 



//#include "Application_prog.h"
#include "Framework.h "
#include "core/device.h"                                                        //³aduje bibliotreki dla odpowiedniego procesora
#include "common.h"



       


#define ScrenButtonNr  9  //maksymalna iloœæ przcisków ekranowych 
#define ScrenGraphNr   4  //maksymalna iloœæ grafik ekranowych 
#define ScrenAnalogNr  4  //maksymalna iloœæ analogów ekranowych 
#define ScrenLineNr    2  //maksymalna iloœæ analogów ekranowych 

#define ButonMaxText   7 //maksymalna iloœæ liter w przycisku


#define MainScreenTermo       ScreenSetTab_0_Nr
#define SubScreenTermo        ScreenSetTab_1_Nr
#define MainScreenTimetable   ScreenSetTab_0_Nr+1
#define SubScreenTimetable    ScreenSetTab_1_Nr+1
#define MainScreenAlarm       ScreenSetTab_0_Nr+2
#define SubScreenAlarm        ScreenSetTab_1_Nr+2



typedef enum {
      DispWaitNoActive=0,
      DispWaitOn,
      DispWaitOn2,
      DispWaitActive,
      DispWaitOff,
    }_DispWait;


typedef enum {
              button5=0,
              button7,
              button9,
             }_ScreenDesign;
 
 
typedef enum {
              GraphNone=0, //pocz¹tek grafik
              GraphZarowka,  
              GraphRolety, 
              GraphWetylator,
              GraphBrama, 
              GraphDrzwi, 
              GraphKran, 
              GraphTermometr,
              GraphText,
              GraphScena,
              GraphWartosc,
              GraphOnOff,
              GraphTextNeg,
             }_TypeGraph;
 

typedef struct{
                _TypeGraph Type;
                char Text[3];
                char Nr;
                union{
                  char Reg;
                  char Port;
                };
                union{
                  char PlusOnOff: 1;
                  char Flip;
                };
              }_Graph;      


typedef struct{
                _TypeGraph Type;
                  char Text[3];
                  char Nr;
                  char Reg;
                  char Flip;
              }_Analog;   


typedef struct
            {   
              char Text[ButonMaxText];
              char OpenPage : 4;//je¿eli ró¿ne od 0 to nr w³¹czanej strony -1
              char Hide     : 1; //1- ukryj przycisk
                   
            }_ButtonGraph;



typedef struct
            {
               signed char  ActorNode;              //numer wêz³a wykonawczego
               unsigned char  ActorPort;              //numer portu wêz³a wykonawczego
               unsigned char  AutoOn;                 //1-Termostat w trybie auto                  
               signed char  SensorNode;             //numer wêz³a czujnika temperatury
               unsigned char  SensorReg;              //numer rejestru w wêz³a czujnika temperatury
               unsigned char  Week;                   //przebieg tygodniowy temperatury dla wykresu A
               signed   short Temperature[2];         //temperatura T1 i T2  
               signed   short SetTemperature;         //temperatura ustawiana rêcznie lub bierz¹co ustawiona temperatura w trybie auto
               unsigned long  Chart[2];               //przebiegi godzinowe temperatur (wykres A i B)
            }_Thermo;

typedef struct
            {
               signed char  ActorNode;              //numer wêz³a wykonawczego
               unsigned char  ActorPort;              //numer portu wêz³a wykonawczego 
               unsigned char  ActionType;             //typ reakcji 0-tak jak ProcWOUT, 1-ProcWOUTS, 2-ProcWOUTR,   
               unsigned char  Min1h;                  //flaga falstartu 1
            }_TimetableSet;  


typedef struct
            {
               signed char  ActorNode;              //numer wêz³a wykonawczego
               unsigned char  ActorPort;              //numer portu wêz³a wykonawczego /syreny 
               signed   char  SensorNode;             //numer wêz³a czujnika ruchu
               unsigned   char  SensorPort;              //numer rejestru w wêz³a czujnika temperatury
            }_AlarmSet;  



#define PaternDesignThermoMS ScreenSetTab_0_Nr+2   
#define PaternDesignThermoSS ScreenSetTab_1_Nr+2  
#define PaternDesignTimetableMS ScreenSetTab_0_Nr+3   
#define PaternDesignTimetableSS ScreenSetTab_1_Nr+3  
#define PaternDesignAlarmMS ScreenSetTab_0_Nr+4   
#define PaternDesignAlarmSS ScreenSetTab_1_Nr+4 


typedef struct{
                    char          PaternDesign;
                    char          Label[10];
                    _ButtonGraph  Button[ScrenButtonNr];
                    _Graph        Graph[ScrenGraphNr];
                    _Analog       Analog[ScrenAnalogNr]; 
                    union{
                    _Thermo       Thermo;
                    _TimetableSet Timetable;
                    _AlarmSet     Alarm;
                          };
              }_Screen;






typedef struct{
                char x;
                char y;
              }_Poz;

typedef struct{
                _Poz Poz;
                char Width;
                char Height;
              }_ScreenObject;




typedef struct{
                _ScreenObject Button[ScrenButtonNr];
                _ScreenObject Graph[ScrenGraphNr];
                _ScreenObject Value[ScrenAnalogNr];
                _ScreenObject Line[ScrenLineNr];
                _ScreenObject Temperature;
                _ScreenObject Time;
                _ScreenObject Date;
              }_ScreenPaternDesign;





typedef struct
                {
                  signed char x;
                  signed char y;
                }_Point;
  


                  





void MenuPaint(void);
void ScreenEvent(void);
char FloToStr2 (float Value, char Vstring[8],char ill);
void AddToMonitoring(char Type, int NrSygnal);
void CleanReg_Event(void);
void Forse_Event(char Type, int NrSygnal);
void Forse_Paint(char Type);
void Info_Event(void);
void Info_Paint(void);
void Info_Refresh(void);
void IO_Side_Event(void);
void IO_Side_Paint(void);
void MainSide_Event(_Screen* ScreenLokal);
void MainSide_Paint(_Screen* ScreenLokal);
void MainSide_Refresh(_Screen* ScreenLokal);
void MenuAccess_Paint(void);
void MenuAccess_Event(void);
void MenuAccessPass_Paint(char level);
void MenuAccessPass_Event(char level);
void MenuSide_Event(void);
void MenuSide_Paint(void);
void MonitoringSide_Event(void);
void Options_Paint(void);
void Options_Event(void);
void Lader_Event(void);
void Lader_Paint(void);
void Restart_Event(void);
char Scroll_Event(int Size_scroll, int *_Suw_poz, char max);
void Scroll_list_selsct_paint(char Label_move, int max);
void ScrollMenu_Paint(int *_Suw_poz, char mol, char Type);
void ScrollMenu_Event(char Type);
void Setup_Event(void);
void Setup_Paint(void);
void SetupTime_Paint(void);
void SetupTime_Event(void);
void SetupLang_Paint(void);
void SetupLang_Event(void);
void SetupPass_Paint(void);
void SetupPass_Event(void);
void SetupOldPass_Paint();
void SetupOldPass_Event(char level);
void SetupNewPass_Paint();
void SetupNewPass_Event(char level);
void Sygnal_Paint(void);
void Sygnal_Event(void);
void Sygnal_ScrollList_Paint(int Size_scroll, int *_Suw_poz );
void ZM_AnalogSide_Paint(void);
void ZM_AnalogSide_Event(void);
void ZM_DwustanSide_Paint(void);
void ZM_DwustanSide_Event(void);
void SetupTime_Refresh(void);
void LaderChange_Paint(void);
void LaderChange_Event(void);
void Procedure_Paint(void);
void Procedure_Event(void);
void AddRow(void);
void RemRow_Paint(void);
void RemRow_Event(void);
void SaveChanges_Evant(void);
void ScreenSet_Paint(_Screen* ScreenLokal);
void ScreenSet_Event(_Screen* ScreenLokal);
void ScreenDesign1_Paint(_Screen *ScreenLocal);
void ScreenDesign1_Event(_Screen *ScreenLocal);
void ZoneButon(char _Label[15], char poz_X , char poz_Y ,char width, char height, char *Vstring);
void ZoneGraph_Paint(_Screen* ScreenLokal);
void ZoneGraph_Event(_Screen* ScreenLokal);
void ScreenGraph(char poz_X , char poz_Y ,char width, char height,_Graph *Graph);
void TextChange_Paint(char* text, char* MaxChar);
void TextChange_Evant(char* text, char* MaxChar, char* MaxPix);
void TextChange_Refresh(char* text, char* MaxChar);
void AnalogGraph_Paint(_Analog *Anal);
void AnalogGraph_Event(_Analog *Anal);
void SetTouchScreen_Paint(void);
void SetTouchScreen_Event(void);
void StartAnimation_Paint(void);
void StartAnimation_Event(void);
void ValueChange_Evant( short int *ValToChange);
void ValueChange_Paint( short int *ValToChange);
void ButtonOptions_Paint(volatile _ButtonGraph *ButtonGraph);
void ButtonOptions_Event(volatile _ButtonGraph *ButtonGraph);
void ButtonAction_Paint(volatile _ButtonGraph *ButtonGraph);
void ButtonAction_Event(volatile _ButtonGraph *ButtonGraph);
void NeuronsList_Paint(void);
void NeuronsList_Event(void);
void NeuronNrChange_Paint( int *ValToChange);
void NeuronNrChange_Evant( int *ValToChange);
void NeuronsList_Refresh(void);
void CheckChannel(void);
void RF_Channel_Paint(void);
void RF_Channel_Event(void);
void RF_Channel_Refresh(void);
void ScreenSever_Paint(void);
void ScreenSever_Event(void);
void Animation_Event(void);
void MasterSlave_Paint(void);
void MasterSlave_Event(void);
void Info_Paint(void);
void Event_Paint(void);
void Wait_Paint(void);
void Termostat_Paint(_Screen* ScreenLokal);
void Termostat_Event(_Screen* ScreenLokal);
void Termostat_Refresh(_Screen* ScreenLokal);
void ThermostatAutoSet_Paint(_Screen* ScreenLokal);
void ThermostatAutoSet_Event(_Screen* ScreenLokal);
void ThermostatChartSet_Paint(_Screen* Screen, char Chart);
void ThermostatChartSet_Event(_Screen* Screen, char Chart);
void ThermostatChartSet_Refresh(_Screen* Screen, char Chart);
void ThermostatSet_Paint(_Screen* Screen);
void ThermostatSet_Event(_Screen* Screen);
void BigTemperLabel_Paint(void);
void BigTemperLabel_Event(void);
void Timetable_Paint(char Screen);
void Timetable_Event(char Screen);
void TimetableSet_Paint(void);
void TimetableSet_Event(void);
void ScreenTimetableSet_Paint(_Screen* Screen);
void ScreenTimetableSet_Event(_Screen* Screen);
void MESSAGE_Refresh(void);
void MESSAGE(char *String);
void CleanAll_Event(void);
void AdvanceSet_Paint(void);
void AdvanceSet_Event(void);
void Info_Refresh(void);
void ScreenAlarmSet_Paint(_Screen* Screen);
void ScreenAlarmSet_Event(_Screen* Screen);
void Alarm_Paint(char CurrentSheet);
void Alarm_Event(char CurrentSheet);
void Alarm_Refresh(char CurrentSheet);
void AlarmPass_Paint(char CurrentSheet);
void AlarmPass_Event(char CurrentSheet);
void AlarmPass_Refresh(void);
void AlarmPassChange_Paint(char CurrentSheet);
void AlarmPassChange_Event(char CurrentSheet);
void Node_Paint(char CurrentNode);
void Node_Event(char CurrentNode);
void NodeTestTrans_Paint(char CurrentNode);
void NodeTestTrans_Event(char CurrentNode);
void NodeTestTrans_Refresh(char CurrentNode);
void NodeStanIO_Paint(char CurrentNode);
void NodeStanIO_Event(char CurrentNode);
void NodeStanIO_Refresh(char CurrentNode);
void Net_Paint(void);
void Net_Event(void);
void UstawSiec_Paint(void);
void UstawSiec_Event(void);
void Punkt_dostepuWiFi_Paint(void);
void Punkt_dostepuWiFi_Event(void);
void WiFiConecting_Paint(void);
void WiFiConecting_Refresh(void);
void Host_WiFi_Paint(void);
void Host_WiFi_Event(void);
void NetPeriodicity_Paint(void);
void NetPeriodicity_Event(void);
void IpTCP_Paint(void);
void IpTCP_Event(void);
void  IpConfig_Paint(void);
void  IpConfig_Event(void);
void  NetInfo_Paint(void);
void  NetInfo_Event(void);
void  NetInfo_Refresh(void);  
void ScreenSetup_Paint(void);
void ScreenSetup_Event(void);
void ScreenBrightness_Paint(void);
void ScreenEvent_Paint(void);
void ScreenBrightness_Event(void);
void NodeInfo_Paint(char CurrentNode);
void NodeInfo_Event(char CurrentNode);
void NodeInfo_Refresh(char CurrentNode);
void ChangeID_Paint(int *NeuronIdToChange, int *NeuronIdAfterChange);
void ChangeID_Event(int *NeuronIdToChange, int *NeuronIdAfterChange);
void ChangeID_Refresh(int *NeuronIdToChange, int *NeuronIdAfterChange);
void CheckingModule_Paint(int *NeuronId);
void CheckingModule_Event(int *NeuronId);
void CheckingModule_Refresh(int *NeuronId);
void NodeWeAnalog_Paint(char CurrentNode);
void NodeWeAnalog_Event(char CurrentNode);
void NodeWeAnalog_Refresh(char CurrentNode);
void Tester_MG1_Paint(char CurrentNode);
void Tester_MG1_Event(char CurrentNode);
void Tester_MG1_Refresh(char CurrentNode);
void HTTP_podglad_MENU_Paint(void);
void HTTP_podglad_MENU_Event(void);
void Zmienne_Paint(void);
void Zmienne_Event(void);
void ZmienneDwustanowe_Paint(void);
void ZmienneDwustanowe_Event(void);
void ZmienneDwustanowe_Repaint(void);
void ZmienneAnalogowe_Paint(void);
void ZmienneAnalogowe_Event(void);
void ZmienneAnalogowe_Repaint(void);
extern _DispWait DispWait;
#endif
