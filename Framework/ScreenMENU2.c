//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                                 CEURON                                   *//                       
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : Prog.Screen.c                                 *//
//*  Opis                         : Obsluga grafiki i ekranu dotykowego      *//
//*  Autor                        : Pawe³ Narwojsz                           */F/
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



#include "ScreenMENU2.h"
#include "Framework.h "
#include "Language.h"
#include "core/device.h"                                                        //³aduje bibliotreki dla odpowiedniego procesora
#include "LCD.h"
#include "Application_prog.h"
#include <intrinsics.h>
#include "Animation.c"
#include "ScreenSorce.h"
#include "RF_Exe.h"
#include "spi_main.h"
 #include "WizFi.h"                            



extern _Correct korekta_Y;
extern _Correct korekta_X;
extern char Arial_7[100][10];

extern char LCD_Canvas[8][128];
extern unsigned int CyklPerSek;
extern char g_level;
extern char g_Password_1[8];
extern char g_Password_2[8];
extern int g_Message;
extern char g_US0_Connect_OK;
extern char g_US1_Connect_OK;
extern char g_Stop_Program;
extern RAM ram;
extern _RF_Status RF_Status;
extern _DateTime DateTime;
extern volatile char SlaveMode;
extern char g_SlaveConnect;
extern unsigned long long  g_counter;
extern volatile char Busy_program;
extern char g_NeedPass;
extern _TransQuality TransQuality; //tablica informacyjna jakoœci po³¹czenia pola odpowiadaj¹ polom _ProgramTab.NeuronsIdTab
extern const char identyfikacja;
extern const char system;
extern char AlarmOn;
extern int g_info;
char MainSide_puls=0;
extern _RF_TaskTab  RF_TaskTab;
extern signed char           RF_CurrentTask;
int volatile MenuLevel=0;      //poziom menu
extern unsigned long long g_NextTimeProgram;
extern char ekran_komunikacja[8][128];
extern signed short X_down;
extern signed short Y_down;
extern signed short X_up;
extern signed short Y_up;
extern int  g_leng;
extern LangTab Speech[2];
extern char g_PC_Connected;
extern _ProgramTab gProg;
extern char NumberScene;
extern _NeuronsListUnit NeuronsList[NumberNeurons];
extern char Label (char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw);
extern char FloToStr (float Value, char Vstring[8],char ill) ;
extern _MainStage  MainStage; 
extern _TransQuality TransQuality;
extern _RF_NodeTest  RF_NodeTest; 
extern char Variables[];    //zmienne
 extern _Net NET; 
extern char WypMaxJasnosci;                                                        //jasnoœc ekranu przy aktywnym ekranie
extern char WypMinJasnosci;                                                        //jasnoœc ekranu przy nieaktywnym ekranie
extern char gWypMinJasnosciTest;
extern short int VariablesAnal[]; 
float wartosc_tmp;
extern unsigned char ChangeStageFlag; 
char text_label_tmp[10];

extern char SceneBefOff;
extern char ChangeingIdMode;

char Label_move = 0;
int Suw_poz_IO=0;
int Suw_poz_analog=0;
int Suw_poz_dwustan=0;
int Suw_poz_monit=0;
int ScrollMenu_poz=-1;
int ScrollMenu_Paint_exist=-1;
int Rej_poz=0;
char CyklMonit=0;
char *TextToChange=0;
char TextToChangeLength=0;
char TextToChangeMaxPixSize=0;
volatile _ButtonGraph *ButtonToChange=0;
char CurentChartModyfi;
int NeuronIdToChange;
int NeuronIdAfterChange;
char NeuronNrChange=0;



signed short int *ValueChange=0;
int *NeuronIdChange=0;



_Analog *AnalogToChange;

char CurrentSheet=0;
char CurModifSheet=0;
char NeuronsListLine=0;

char SheetHistoryIndex=0;
char SheetHistory[20];


extern _ProgramTab gProg;
extern int PtogTime;





int Monitoring_array[30][2]; //tablica monitoringu 1 kol - adres, 2 kol- typ (0 analog, 1 dwustan, 2 IO)
int Monit_Size_scroll=0;
char SetupTimeCarPoz=1; 
extern char ProgramChange=0;      //Je¿eli 1 - zmiana programu przez u¿ytkownika 
//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ScreenEvent
//* Funkcja wrysowania menu ekranowego
//*-----------------------------------------------------------------------------
//* Efekt widoczny po odœwierzeniu LCD (RepaintLCD)
//*-----------------------------------------------------------------------------





char TmpFlg=0;
void MenuPaint(void)
{
  
  
    if (TmpFlg==0)
  {
    TmpFlg=1;

  }

  if (g_Message==0) 
  switch (MenuLevel)
  {
    case 0        : StartAnimation_Paint();         break;
    case 1        : MainSide_Paint(&gProg.Screen[CurrentSheet]);         break;
    case 3        : CheckChannel();         break;
    case 4        : SetTouchScreen_Paint();  break;
    case 5        : AdvanceSet_Paint();  break;
      case 51     : Ask_Paint(Speech[g_leng].Czysc_pamiec,Speech[g_leng].Potwierdz,"");  break;
      
      case 10     : MenuAccessPass_Paint(1);  break; 
      case 11     : MenuSide_Paint();         break; 
            case 111   : Lader_Paint();        break;
                case 1110 : Ask_Paint("Info ",Speech[g_leng].Zapisz,Speech[g_leng].Zmiany);  break;
                case 1111: Procedure_Paint();  break;
                  case 11111: ValueChange_Paint(ValueChange);  break;
                  case 111111 : Message("Max=", "+/- 3000 ",11111);  break;
                case 1112: LaderChange_Paint();  break;
                  case 11122: RemRow_Paint();  break;         
            case 112  : ScreenSet_Paint(&gProg.Screen[CurModifSheet]);        break;   
                case 1120 : Ask_Paint("Info ",Speech[g_leng].Zapisz,Speech[g_leng].Zmiany);  break;
                case 1121  : ScreenDesign1_Paint(&gProg.Screen[CurModifSheet]); break;
                    case 11211  : ZoneGraph_Paint(&gProg.Screen[CurModifSheet]); break;
                        case 112111  :  TextChange_Paint(TextToChange, &TextToChangeLength); break;
                        case 112112  :  AnalogGraph_Paint(AnalogToChange);break;
                          case 1121121  :  TextChange_Paint(TextToChange, &TextToChangeLength); break;
                    case 11212  :  TextChange_Paint(TextToChange, &TextToChangeLength); break;
                    case 11213  :  AnalogGraph_Paint(AnalogToChange);break;
                      case 112131  :  TextChange_Paint(TextToChange, &TextToChangeLength); break;
                    case 11214  :  ButtonOptions_Paint(ButtonToChange); break;
                      case 112141  :  TextChange_Paint(TextToChange, &TextToChangeLength); break;
                      case 112142  :  ButtonAction_Paint(ButtonToChange); break;
                    case 11215  :  BigTemperLabel_Paint(); break;
                case 1122  :  ThermostatSet_Paint(&gProg.Screen[CurModifSheet]); break;
                      case 11224   :  ButtonOptions_Paint(ButtonToChange); break;
                      case 112241  :  TextChange_Paint(TextToChange, &TextToChangeLength); break;
                      case 112242  :  ButtonAction_Paint(ButtonToChange); break;
                case 1123  :  ScreenTimetableSet_Paint(&gProg.Screen[CurModifSheet]); break;
                case 1124  :  ScreenAlarmSet_Paint(&gProg.Screen[CurModifSheet]); break;
            case 113   : Options_Paint();          break;  
                case 1131 : Ask_Paint("Restart",Speech[g_leng].Potwierdz,"");  break;
                case 1133 :  Zmienne_Paint();  break;
                        case 11331 :  ZmienneDwustanowe_Paint();  break;
                        case 11332 :  ZmienneAnalogowe_Paint();  break;
                case 1134 : CheckingModule_Paint(&NeuronIdToChange);  break;    //uwaga numer MenuLevel u¿ywany w Main jako startowy dla TESTERa
                      case 11341 : NeuronNrChange_Paint(NeuronIdChange);  break;  
                      case 113421 : NodeStanIO_Paint(NeuronNrChange);  break;
                      case 113431 : NodeWeAnalog_Paint(NeuronNrChange);  break;
                      case 11344 : Tester_MG1_Paint(NeuronNrChange);  break;    
                case 1135 : ChangeID_Paint(&NeuronIdToChange, &NeuronIdAfterChange);  break;
                      case 11351 : NeuronNrChange_Paint(NeuronIdChange);  break;      
            case 114   : Setup_Paint();            break;
                case 1141 : SetupLang_Paint();         break;
                case 1142 : SetupTime_Paint();        break;  
                case 1143 : SetupPass_Paint();         break;
                    case 11431 : SetupOldPass_Paint();         break;
                        case 114311 : SetupNewPass_Paint();         break;
                    case 11432 : SetupOldPass_Paint();         break;
                        case 114321 : SetupNewPass_Paint();         break; 
                case 1144 : ScreenSetup_Paint();  break;    
                    case 11441 : ScreenBrightness_Paint();  break;
                    case 11442 : ScreenSever_Paint();  break;
                    case 11443 : SetTouchScreen_Paint();  break;
                case 1146 : MasterSlave_Paint();  break;
                case 1147 : RF_Channel_Paint();  break;
            case 115  : NeuronsList_Paint();    break;
               case 1150 : Ask_Paint("Info ",Speech[g_leng].Zapisz,Speech[g_leng].Zmiany);  break;
               case 1151 : NeuronNrChange_Paint(NeuronIdChange);  break;
               case 1152 :  TextChange_Paint(TextToChange, &TextToChangeLength); break;
               case 1153 :  Node_Paint(NeuronNrChange); break;
                  case 11531 : NeuronNrChange_Paint(NeuronIdChange);  break;
                  case 11534 : NodeTestTrans_Paint(NeuronNrChange);  break;     
                  case 11535 : NodeStanIO_Paint(NeuronNrChange);  break;
                  case 11536 : NodeWeAnalog_Paint(NeuronNrChange);  break;
                  case 11537 : NodeInfo_Paint(NeuronNrChange);  break;
            case 116  : Info_Paint();    break;
            case 117  : Net_Paint();     break;
               case 1171  : UstawSiec_Paint();     break;
                  case 11711  : Punkt_dostepuWiFi_Paint();     break;
                    case 117111  :  TextChange_Paint(TextToChange, &TextToChangeLength); break;
                    case 117112 :  WiFiConecting_Paint(); break;
                  case 11712  : IpTCP_Paint(); break;
                    case 117122 : IpConfig_Paint(); break;
                      case 1171221  :  TextChange_Paint(TextToChange, &TextToChangeLength); break;
               case 1172 :  Host_WiFi_Paint(); break;
                  case 11721  :  TextChange_Paint(TextToChange, &TextToChangeLength); break;
               case 1173 : NetPeriodicity_Paint(); break;  
               case 1174 : NetInfo_Paint(); break; 
      case 12     : ThermostatAutoSet_Paint(&gProg.Screen[CurrentSheet]);         break;  
            case 120 :   Ask_Paint("Info ",Speech[g_leng].Zapisz,Speech[g_leng].Zmiany);  break;
            case 121     : ThermostatChartSet_Paint(&gProg.Screen[CurrentSheet], CurentChartModyfi);         break;  
      case 13     : TimetableSet_Paint();         break;   
      case 14     : AlarmPass_Paint(CurrentSheet);         break;   
      case 15     : AlarmPassChange_Paint(CurrentSheet);         break;  

                   
  }
   
}//_______________________ Koniec funkcji ScreenEvent __________________________





//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ScreenEvent
//* Funkcja obs³ugi zdarzenia menu ekranowego
//*-----------------------------------------------------------------------------
//* Efekt widoczny po odœwierzeniu LCD (RepaintLCD)
//*-----------------------------------------------------------------------------


void ScreenEvent(void)
{
  

  
  if (g_Message==0)
  {
  switch (MenuLevel)
  {
    case 0     : StartAnimation_Event();  break;
    case 1     : MainSide_Event(&gProg.Screen[CurrentSheet]);              break;
    case 4     : SetTouchScreen_Event();  break; 
    case 5     : AdvanceSet_Event();  break;
    case 51    : CleanAll_Event();          break; 
    case 10: MenuAccessPass_Event(1);  break;
      case 11: MenuSide_Event(); break;
      
            case 111  : Lader_Event();        break;
                  case 1110 : SaveChanges_Evant(); break;
                  case 1111: Procedure_Event(); break;
                    case 11111: ValueChange_Evant(ValueChange);  break;
                      case 111111 : Message("Max=", "+/- 3000 ",11111);  break;
                  case 1112: LaderChange_Event();  break;
                    case 11122: RemRow_Event();  break;
                  
            case 112  : ScreenSet_Event(&gProg.Screen[CurModifSheet]);        break;
                  case 1120 : SaveChanges_Evant(); break;
                  case 1121  : ScreenDesign1_Event(&gProg.Screen[CurModifSheet]); break;
                      case 11211  : ZoneGraph_Event(&gProg.Screen[CurModifSheet]); break;
                          case 112111  :  TextChange_Evant(TextToChange, &TextToChangeLength, &TextToChangeMaxPixSize); break;
                          case 112112  :  AnalogGraph_Event(AnalogToChange);break;
                            case 1121121  :  TextChange_Evant(TextToChange, &TextToChangeLength, &TextToChangeMaxPixSize); break;
                      case 11212  :  TextChange_Evant(TextToChange, &TextToChangeLength, &TextToChangeMaxPixSize); break;
                      case 11213  :  AnalogGraph_Event(AnalogToChange);break;
                          case 112131  :  TextChange_Evant(TextToChange, &TextToChangeLength, &TextToChangeMaxPixSize); break;
                      case 11214   :  ButtonOptions_Event(ButtonToChange); break;
                      case 112141  :  TextChange_Evant(TextToChange, &TextToChangeLength, &TextToChangeMaxPixSize); break;
                      case 112142  :  ButtonAction_Event(ButtonToChange); break;
                      case 11215   :  BigTemperLabel_Event(); break;
                   case 1122  :  ThermostatSet_Event(&gProg.Screen[CurModifSheet]); break;
                      case 11224   :  ButtonOptions_Event(ButtonToChange); break;
                      case 112241  :  TextChange_Evant(TextToChange, &TextToChangeLength, &TextToChangeMaxPixSize); break;
                      case 112242  :  ButtonAction_Event(ButtonToChange); break;
                   case 1123  :  ScreenTimetableSet_Event(&gProg.Screen[CurModifSheet]); break;
                   case 1124  :  ScreenAlarmSet_Event(&gProg.Screen[CurModifSheet]); break;
            case 113  : Options_Event();          break; 
                  case 1131 : Restart_Event();          break; 
                  case 1133 :  Zmienne_Event();  break;
                        case 11331 :  ZmienneDwustanowe_Event();  break;
                        case 11332 :  ZmienneAnalogowe_Event();  break;
                  case 1134 : CheckingModule_Event(&NeuronIdToChange);  break;
                        case 11341: NeuronNrChange_Evant(NeuronIdChange);  break;
                        case 113421 : NodeStanIO_Event(NeuronNrChange);  break;
                        case 113431 : NodeWeAnalog_Event(NeuronNrChange);  break;
                        case 11344 : Tester_MG1_Event(NeuronNrChange);  break;
                  case 1135 : ChangeID_Event(&NeuronIdToChange, &NeuronIdAfterChange);  break;
                        case 11351: NeuronNrChange_Evant(NeuronIdChange);  break;      
            case 114  : Setup_Event();            break; 
                  case 1141 : SetupLang_Event();         break;
                  case 1142 : SetupTime_Event();        break;
                  case 1143 : SetupPass_Event();         break;   
                        case 11431 : SetupOldPass_Event(1);         break;
                             case 114311 : SetupNewPass_Event(1);         break;
                        case 11432 : SetupOldPass_Event(2);         break;
                             case 114321 : SetupNewPass_Event(2);         break;
                  case 1144 : ScreenSetup_Event();  break;           
                    case 11441 : ScreenBrightness_Event();  break;
                    case 11442 : ScreenSever_Event();  break;
                    case 11443 : SetTouchScreen_Event();  break;
                  case 1146 : MasterSlave_Event();  break;
                  case 1147 : RF_Channel_Event();  break;
            case 115  : NeuronsList_Event();  break;
                  case 1150 : SaveChanges_Evant(); break;
                  case 1151: NeuronNrChange_Evant(NeuronIdChange);  break;
                  case 1152  :  TextChange_Evant(TextToChange, &TextToChangeLength, &TextToChangeMaxPixSize); break;
                  case 1153 :  Node_Event(NeuronNrChange); break;
                      case 11531 : NeuronNrChange_Evant(NeuronIdChange);  break;
                      case 11534 : NodeTestTrans_Event(NeuronNrChange);  break;
                      case 11535 : NodeStanIO_Event(NeuronNrChange);  break;
                      case 11536 : NodeWeAnalog_Event(NeuronNrChange);  break;
                      case 11537 : NodeInfo_Event(NeuronNrChange);  break;
            case 116  : Info_Event();    break;
            case 117  : Net_Event();     break;
                  case 1171  : UstawSiec_Event();     break;
                      case 11711  : Punkt_dostepuWiFi_Event();     break;
                        case 117111  :  TextChange_Evant(TextToChange, &TextToChangeLength, &TextToChangeMaxPixSize); break;
                      case 11712  : IpTCP_Event(); break;
                        case 117122 : IpConfig_Event(); break;
                          case 1171221  :  TextChange_Evant(TextToChange, &TextToChangeLength, &TextToChangeMaxPixSize); break;
                  case 1172 : Host_WiFi_Event(); break;  
                      case 11721  :  TextChange_Evant(TextToChange, &TextToChangeLength, &TextToChangeMaxPixSize); break;
                  case 1173 : NetPeriodicity_Event(); break; 
                  case 1174 : NetInfo_Event(); break; 
          case 12     : ThermostatAutoSet_Event(&gProg.Screen[CurrentSheet]);         break;  
            case 120      : SaveChanges_Evant(); break;
            case 121      : ThermostatChartSet_Event(&gProg.Screen[CurrentSheet], CurentChartModyfi);         break;  
          case 13     : TimetableSet_Event();         break;   
          case 14     : AlarmPass_Event(CurrentSheet);         break; 
          case 15     : AlarmPassChange_Event(CurrentSheet);         break;  
  } 
  }
  else
  {
    if ((X_up>0) && (Y_up>0)  )
    {
        g_Message=0;
        g_info=0;
        MenuPaint();
    }
  }
  
   X_up=-1;
 Y_up=-1;



}//_______________________ Koniec funkcji ScreenEvent __________________________





//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    ScreenEvent
//* Funkcja wrysowania menu ekranowego
//*-----------------------------------------------------------------------------
//* Efekt widoczny po odœwierzeniu LCD (RepaintLCD)
//*-----------------------------------------------------------------------------
_DispWait DispWait=0; 
void MenuRefresh(Void)
{
  
 if (g_info==0)
  {
  static char Message;
  
  if ((g_counter>g_Message) && (!Message)) 
  {    
    if (DispWait==DispWaitNoActive)      //tryb wyswietlania menu
    {
      switch (MenuLevel)
      {
        case 0        : StartAnimation_Paint();         break;
        case 1        : MainSide_Refresh(&gProg.Screen[CurrentSheet]);         break;
        case 3        : CheckChannel();         break;
        case 14       : AlarmPass_Refresh();break;
            case 112111  :  TextChange_Refresh(TextToChange, &TextToChangeLength); break;
            case 1121121  : TextChange_Refresh(TextToChange, &TextToChangeLength); break;
            case 11212  :   TextChange_Refresh(TextToChange, &TextToChangeLength); break;
            case 112131  :  TextChange_Refresh(TextToChange, &TextToChangeLength); break;
            case 112141  :  TextChange_Refresh(TextToChange, &TextToChangeLength); break;
            case 1142    : SetupTime_Refresh();        break;
            case 1148    : RF_Channel_Refresh();  break;
            
            case 11331 :  ZmienneDwustanowe_Repaint();  break;
            case 11332 :  ZmienneAnalogowe_Repaint();  break;
            case 1134 : CheckingModule_Refresh(&NeuronIdToChange);  break;
              case 113421 : NodeStanIO_Refresh(NeuronNrChange);  break;
              case 113431 : NodeWeAnalog_Refresh(NeuronNrChange);  break;
              case 11344 : Tester_MG1_Refresh(NeuronNrChange);  break;
            case 1135 : ChangeID_Refresh(&NeuronIdToChange, &NeuronIdAfterChange);  break;
              
              
         case 115  : NeuronsList_Refresh();  break;
              case 1152    : TextChange_Refresh(TextToChange, &TextToChangeLength); break;
              case 11534   : NodeTestTrans_Refresh(NeuronNrChange);  break;
              case 11535   : NodeStanIO_Refresh(NeuronNrChange);  break;
              case 11536   : NodeWeAnalog_Refresh(NeuronNrChange);  break;
              case 11537   : NodeInfo_Refresh(NeuronNrChange);  break;
        
            
        case 116  : Info_Refresh();    break;
             case 1174 : NetInfo_Refresh(); break; 
                
             
             case 117111  :  TextChange_Refresh(TextToChange, &TextToChangeLength); break;
             case 117112 :  WiFiConecting_Refresh(); break;
             case 11721  :  TextChange_Refresh(TextToChange, &TextToChangeLength); break;
             case 1171221  :  TextChange_Refresh(TextToChange, &TextToChangeLength); break;
             
             
        
        case 121     : ThermostatChartSet_Refresh(&gProg.Screen[CurrentSheet], CurentChartModyfi);  break;   
        
      }
    }
    else if(DispWait==DispWaitOn)
    {
      Wait_Paint();
    }
    else if(DispWait==DispWaitOn2)
    {
      Wait_Paint();
      DispWait=DispWaitActive;
    }
    else if(DispWait==DispWaitOff)
    {
      DispWait=DispWaitNoActive;
      MenuPaint();
    }
    

    
    
  }
  
  
    if ((g_counter>g_Message) && (Message)){Message=0; g_Message=0; MenuPaint();}
  if ((g_counter<g_Message) && (!Message)){Message=1;}
  if ((g_counter<g_Message) && (MenuLevel<2)) { MESSAGE_Refresh();}
  
  } 
}//_______________________ Koniec funkcji ScreenEvent __________________________




extern char Animation_tab[11][8][15];

char InitPointer=0;
char CCKier=0;

//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    MainSide_Paint
//* Funkcja strony g³ównej menu
//*-----------------------------------------------------------------------------
//* Efekt widoczny po odœwierzeniu LCD (RepaintLCD)
//*-----------------------------------------------------------------------------
char AnimationNr=0;
void StartAnimation_Paint(void)
{
  CleanCanvas();
  
  CopyToCanvasPoint((char*)Logo_Ceuron, 0, 70, 64,16);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     


  
  Label((char*)&identyfikacja,2,0,52,15,63,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
  Label("Firmware: ",0,5,5,15,63,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
  Label((char*)&system,0,48,5,4,14,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
 
  //wskaŸnik postêpu
  H_line(InitPointer+5,48,24,1); // (char X, int Y, char lenght, char typ)
  H_line(InitPointer+4,49,26,1); // (char X, int Y, char lenght, char typ)
  H_line(InitPointer+5,50,24,1); // (char X, int Y, char lenght, char typ)
  if ((InitPointer<30) && (CCKier==0)) {InitPointer+=2;}else{CCKier=1;}
  if ((InitPointer>0) && (CCKier==1)) {InitPointer-=2;}else{CCKier=0;}
  

  
  
  #ifdef _ADMINISTRATOR_ 
    Label("Administrator",2,0,115,15,63,0,128,1);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
  #endif   
  
  #ifdef _TESTER_ 
    Label("TESTER",2,0,115,15,63,0,128,1);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
  #endif    
    
   
    
    
   #ifdef _TESTER_ 
            NeuronIdToChange=9999999;
            NeuronNrChange=1;
  #endif   
  
  if ((AnimationNr==14) || (SlaveMode))
  {
      korekta_X.A=AT91C_IFLASH_MEM->Config.Cor_X.A;
      korekta_X.B=AT91C_IFLASH_MEM->Config.Cor_X.B;
      korekta_Y.A=AT91C_IFLASH_MEM->Config.Cor_Y.A;
      korekta_Y.B=AT91C_IFLASH_MEM->Config.Cor_Y.B;
      
      //je¿eli brak korekt to ustaw korekty
      if ((korekta_X.A==0)||(korekta_Y.A==0))
      {
        MenuLevel=4;
        
 
    
      }else{
       // MenuLevel=1;
      }
      AnimationNr++;
      MenuPaint();
  }else{
     AnimationNr++; 
  }
    


}


//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    StartAnimation_Event
//* Funkcja strony g³ównej menu
//*-----------------------------------------------------------------------------
//* Efekt widoczny po odœwierzeniu LCD (RepaintLCD)
//*-----------------------------------------------------------------------------
char StartAdvance=0;
void StartAnimation_Event(void)
{

    if ((X_up>0) && (Y_up>0))
    {
      MenuLevel=4;
      StartAdvance=1;
      MenuPaint();
    }
    
    
     
}//_______________________ Koniec funkcji StartAnimation_Event __________________________





//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    MainSide_Paint
//* Funkcja strony g³ównej menu
//*-----------------------------------------------------------------------------
//* Efekt widoczny po odœwierzeniu LCD (RepaintLCD)
//*-----------------------------------------------------------------------------

void CheckChannel(void)
{

  CleanCanvas();

     Label("Checking Wi-Fi",0,1,60,14,6,0,120,0);

      H_line(InitPointer+5,48,24,1); // (char X, int Y, char lenght, char typ)
      H_line(InitPointer+4,49,26,1); // (char X, int Y, char lenght, char typ)
      H_line(InitPointer+5,50,24,1); // (char X, int Y, char lenght, char typ)
      if ((InitPointer<30) && (CCKier==0)) {InitPointer+=2;}else{CCKier=1;}
      if ((InitPointer>0) && (CCKier==1)) {InitPointer-=2;}else{CCKier=0;}
      
}





char ekran_komunikacja[8][128] = {
                  {0,0,0,0,0,0,0,0,0,0,28,34,2,2,2,34,28,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,248,8,232,232,232,104,104,104,104,104,104,104,104,104,104,104,104,232,232,232,8,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,224,248,24,6,6,6,3,3,3,3,6,6,12,60,240,192,0,0,0,0,0},
                  {0,0,0,0,0,0,0,0,0,0,30,33,33,33,33,33,30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,0,255,255,255,192,192,192,192,192,192,192,192,192,192,192,192,255,255,255,0,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,192,224,48,24,0,0,0,255,255,0,0,24,24,96,224,128,0,0,0,0,0},
                  {0,0,0,0,0,0,0,0,0,0,145,153,153,149,147,147,145,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,2,2,10,18,34,66,66,130,130,2,2,2,2,2,2,2,2,2,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,195,225,112,48,48,48,48,48,48,48,48,48,48,48,51,3,0,0,0,0,0},
                  {0,0,0,0,0,0,0,0,0,0,200,76,76,202,73,73,200,0,0,0,4,4,4,60,68,68,60,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,0,0,0,1,1,1,130,194,164,24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,14,28,24,24,24,24,24,24,24,24,24,24,24,152,128,0,0,0,0,0},
                  {0,0,0,0,0,0,0,0,0,0,199,32,32,39,32,32,199,0,0,0,28,34,2,2,2,34,28,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,1,2,84,168,80,33,206,14,206,67,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,96,48,48,24,24,12,12,28,28,96,96,96,96,48,63,15,0,0,0,0,0},
                  {0,0,0,0,0,0,0,0,0,0,65,66,64,64,64,66,241,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,62,124,85,170,85,0,255,126,255,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,248,220,142,6,6,6,6,6,6,6,6,6,6,140,252,112,0,0,0,0,0},
                  {0,0,0,0,0,0,0,0,0,0,248,8,8,248,8,8,249,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,63,127,255,192,128,81,162,85,0,255,248,255,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,48,49,51,51,51,51,51,51,51,51,51,51,51,49,97,192,0,0,0,0,0},
                  {0,0,0,0,0,0,0,0,0,0,28,36,68,68,68,36,28,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,7,14,29,24,31,1,31,16,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,16,31,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,56,108,198,198,198,198,198,198,198,198,198,198,198,198,195,1,0,0,0,0,0},
                  };




//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    MainSide_Paint
//* Funkcja strony g³ównej menu
//*-----------------------------------------------------------------------------
//* Efekt widoczny po odœwierzeniu LCD (RepaintLCD)
//*-----------------------------------------------------------------------------
void MainSide_Paint(_Screen* ScreenLokal)
{
  //Komunikacja z komputerem
  if (g_PC_Connected==1)
  {
    CopyToCanvas(ekran_komunikacja);
  }
  else
  {
  
        char PaternDesign=ScreenLokal->PaternDesign;
        
        CleanCanvas();
         char txt[10];
       
    
      //CleanCanvas(); 
      
      
      SheetHistory[0]=0;
      
      if ((SheetHistory[SheetHistoryIndex]!=CurrentSheet) && (CurrentSheet<sizeof(SheetHistory)-1))
      {
          SheetHistory[++SheetHistoryIndex]=CurrentSheet;
      }
        
      
      if      ((CurrentSheet==0)  && (PaternDesign==MainScreenTermo)) {Termostat_Paint(ScreenLokal);}
      else if ((CurrentSheet>0)   && (PaternDesign==SubScreenTermo)) {Termostat_Paint(ScreenLokal);}
      else if ((CurrentSheet==0)  && (PaternDesign==MainScreenTimetable)) {Timetable_Paint(CurrentSheet);}
      else if ((CurrentSheet>0)   && (PaternDesign==SubScreenTimetable)) {Timetable_Paint(CurrentSheet);}  
      #ifdef __ALARM      
            else if ((CurrentSheet==0)  && (PaternDesign==MainScreenAlarm)) {Alarm_Paint(CurrentSheet);}
            else if ((CurrentSheet>0)   && (PaternDesign==SubScreenAlarm)) {Alarm_Paint(CurrentSheet);} 
      #endif
      else  
      { 
            
            const _ScreenPaternDesign *ScreenPaternDesign;
            if (CurrentSheet==0) 
            {
              ScreenPaternDesign=ScreenSetTab_0;
            }else{
              ScreenPaternDesign=ScreenSetTab_1; 
              List_Paint(ScreenLokal->Label,1,108,0,0,3," ",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      
          
            }  
            
            
            
            //grafika  i text
            for (char i=0; i<ScrenGraphNr; i++)
            {
              if (ScreenPaternDesign[PaternDesign].Graph[i].Width>0)
              {
                      ScreenGraph(//(char poz_X , char poz_Y ,char width, char height,_Graph *Graph)
                      ScreenPaternDesign[PaternDesign].Graph[i].Poz.x,
                      ScreenPaternDesign[PaternDesign].Graph[i].Poz.y,
                      ScreenPaternDesign[PaternDesign].Graph[i].Width,
                      ScreenPaternDesign[PaternDesign].Graph[i].Height,
                      &(ScreenLokal->Graph[i]));  //graf2   //*  Label- etykieta,  poz_X - pozycja lewy róg, poz_Y - pozycja dolny róg, height, with
              
              }
             }
            
            

          

            
            
              //Analogi
            char IlLiter=0;
            signed short Value;
            for (char i=0; i<ScrenAnalogNr; i++)
            {
              if (ScreenPaternDesign[PaternDesign].Value[i].Width>0)
              {         
                     if ((ScreenLokal->Analog[i].Nr>0) || ((ScreenLokal->Analog[i].Nr==0)&&(ScreenLokal->Analog[i].Reg>0)))
                     {
                     IlLiter=0;
                     Value=MainStage.StageI[ScreenLokal->Analog[i].Nr].Value[ScreenLokal->Analog[i].Reg];

                     if (Value&0x8000) //je¿eli sygna³ niewiarygodny
                     {
                       txt[0]='-'; txt[1]='?';txt[2]='?';txt[3]='?'; txt[4]='-'; IlLiter+=5;
                     }else{ //je¿eli sygna³ wiarygodny
                         if (ScreenLokal->Analog[i].Flip)
                         {
                             IlLiter=sIntToStr(Value, &(txt[0])); //(char Value, char *Vstring, char ill);
                             for (char k=0; k<3; k++) if (ScreenLokal->Analog[i].Text[k]>0) {txt[IlLiter++]=ScreenLokal->Analog[i].Text[k];}  
                         }else{
                             for (char k=0; k<3; k++) if (ScreenLokal->Analog[i].Text[k]>0) {IlLiter++; txt[k]=ScreenLokal->Analog[i].Text[k];}  
                             IlLiter+=sIntToStr(Value, &(txt[IlLiter])); //(char Value, char *Vstring, char ill);
                         }
                     }  
                         Label(txt,0,
                               ScreenPaternDesign[PaternDesign].Value[i].Poz.x, 
                               ScreenPaternDesign[PaternDesign].Value[i].Poz.y,IlLiter,
                               ScreenPaternDesign[PaternDesign].Value[i].Width,
                               0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
                     
                     }
             }
            }
          
             //Linie
             for (char i=0; i<ScrenLineNr; i++)
             {
              if (ScreenPaternDesign[PaternDesign].Line[i].Width>0)
              {
                  if (ScreenPaternDesign[PaternDesign].Line[i].Width>ScreenPaternDesign[PaternDesign].Line[i].Height)
                  {
                      H_line(ScreenPaternDesign[PaternDesign].Line[i].Poz.x,ScreenPaternDesign[PaternDesign].Line[i].Poz.y,ScreenPaternDesign[PaternDesign].Line[i].Width,1); // (char X, int Y, char lenght, char typ)
                  }else{
                      V_line(ScreenPaternDesign[PaternDesign].Line[i].Poz.x,ScreenPaternDesign[PaternDesign].Line[i].Poz.y,ScreenPaternDesign[PaternDesign].Line[i].Height,1); // (char X, int Y, char lenght, char typ)
              
                  }
              }
             }
            
                        
               //przyciski
            for (char i=0; i<ScrenButtonNr; i++)
            {
              if ((ScreenPaternDesign[PaternDesign].Button[i].Width>0) && (ScreenLokal->Button[i].Hide==0))
              {
                      Button(ScreenLokal->Button[i].Text,
                      ScreenPaternDesign[PaternDesign].Button[i].Poz.x,
                      ScreenPaternDesign[PaternDesign].Button[i].Poz.y,
                      ScreenPaternDesign[PaternDesign].Button[i].Width,
                      ScreenPaternDesign[PaternDesign].Button[i].Height); //*  Label- etykieta,  poz_X - pozycja lewy róg, poz_Y - pozycja dolny róg, height, with
             
              }
             }
            
            
            //Du¿a temperatura

      
        
      }
  }
}//_______________________ Koniec funkcji MainSide_Paint __________________________




//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    Message
//* Funkcja wyœwietlenia wiadomoœci   
//*-----------------------------------------------------------------------------
//*  String   - wskaŸnik na tekst
//*  StringLength - d³ugoœæ tekstu
//*-----------------------------------------------------------------------------
#define MAX_LITER_W_LINI   14
void MESSAGE(char *String)
{
  
     char StrTmp[8][15];
     char StringLength=0;
     
     while ((String[StringLength]>0) && (StringLength<250)) StringLength++;                             //okreœlenie d³ugoœci stringu         
     if (StringLength>250) StringLength=250;
    char znak=0;
    char ZWL=0; //znak w linii
    char SPC=0; //spacja
    char Wiersz=0;
    while (znak<StringLength)
    {
       ZWL=0;
       SPC=znak;
       while ((ZWL<MAX_LITER_W_LINI+1) && (znak+ZWL<=StringLength))
       {
           if (String[znak+ZWL]=='\n')     //je¿eli nastêpna linia
           {
             SPC=znak+ZWL;
             break; 
           }else if((String[znak+ZWL]==0) || (String[znak+ZWL]==' ') || ((String[znak+ZWL]=='.') && (String[znak+ZWL+1]!=' ')))     //je¿eli spacja lub kropka bez spacji na koñcu linii
           {  
              SPC=znak+ZWL;
           }  
           ZWL++;
       }
       if(znak+ZWL>=StringLength) SPC=StringLength;
       
       ZWL=0;
       char ZWL=0;
       while ((znak<SPC) && (ZWL<MAX_LITER_W_LINI))  
       {
         StrTmp[Wiersz][ZWL++]=String[znak++];
       }
       StrTmp[Wiersz][ZWL]=0;
       if (String[znak]==' ') znak++;
       if (String[znak]=='\n') znak++;
       if (String[znak]==' ') znak++;
       
       
       
       if (znak<StringLength)Wiersz++;
    }
     
     

    CleanCanvas();
    
    char BazeY=56+10*(Wiersz/2);
   
     Label(" INFO ",2,2,BazeY+17,15,59,0,127,1);      //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
    
     
     
    for (char i=0; i<=Wiersz; i++)
    {
        Label(StrTmp[i],0,2,BazeY,15,56,0,127,0);      //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
        BazeY=BazeY-10;
    }
   
    
    
    char PozD=BazeY+3; 
    char PozG=BazeY+10*(Wiersz+1)+15; 
    char Wys=PozG-PozD;
    V_line(0,PozD-2,Wys+14,1); //(char X, int Y, char lenght, char typ{0-bia³a 1=ci¹gla, 2=kropkowana, 3=kreskowana}) 
    V_line(63,PozD-2,Wys+14,1); //(char X, int Y, char lenght, char typ{0-bia³a 1=ci¹gla, 2=kropkowana, 3=kreskowana}) 
    H_line(0,PozG+12,74,1); //(char X, int Y, char lenght, char typ{0-bia³a 1=ci¹gla, 2=kropkowana, 3=kreskowana}) 
    H_line(2,PozG,60,1); //(char X, int Y, char lenght, char typ{0-bia³a 1=ci¹gla, 2=kropkowana, 3=kreskowana}) 
    H_line(0,PozD-2,64,1); //(char X, int Y, char lenght, char typ{0-bia³a 1=ci¹gla, 2=kropkowana, 3=kreskowana}) 

    
    g_Message=g_counter+(5+(Wiersz/2))*700;         //czas wyœwietlania wiadomoœci (jednostka- ms)
}










//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    MainSide_Refresh
//* Funkcja strony g³ównej menu
//*-----------------------------------------------------------------------------
//* Efekt widoczny po odœwierzeniu LCD (RepaintLCD)
//*-----------------------------------------------------------------------------
void MESSAGE_Refresh(void)
{

   
   if ((MainSide_puls==0) || (MainSide_puls>1))
    { 
      char ekran1[2][13] = {
                 {0,0,224,24,232,244,244,244,232,24,224,0,0},
                  {0,0,0,3,2,5,5,5,2,3,0,0,0}
                  };
      
      for (char i=0; i<=12; i++)
      {
         
          LCD_Canvas[0][i]=ekran1[0][i];
          LCD_Canvas[1][i]=ekran1[1][i];
      }
      
    }
    if ((MainSide_puls==1))
    {
      
      
      char ekran2[2][13] = {
                  {240,28,230,250,251,253,253,253,251,250,230,28,240},
                  {1,7,12,11,27,23,23,23,27,11,12,7,1},
                  };
      
      for (char i=0; i<=12; i++)
      {
         
          LCD_Canvas[0][i]=ekran2[0][i];
          LCD_Canvas[1][i]=ekran2[1][i];
      }         
    }
     
    if (MainSide_puls>3) {MainSide_puls=0;} else {MainSide_puls++;} 
  
}//_______________________ Koniec funkcji MainSide_Refresh __________________________










//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    MainSide_Paint
//* Funkcja strony g³ównej menu
//*-----------------------------------------------------------------------------
//* Efekt widoczny po odœwierzeniu LCD (RepaintLCD)
//*-----------------------------------------------------------------------------
void MainSide_Event(_Screen* ScreenLokal)
{
  signed char BODP=-1;
  
  if (!g_PC_Connected)
  {
  char ChangeSheet=0;
  char MenuPaintExecute=0;
  static char Xdown=0;
  
  char PaternDesign=(ScreenLokal->PaternDesign);
  
  const _ScreenPaternDesign *ScreenPaternDesign;
  if (CurrentSheet==0) 
  {
    ScreenPaternDesign=ScreenSetTab_0;
  }else{
    ScreenPaternDesign=ScreenSetTab_1; 
    if (List_Event(X_up, Y_up, X_down, Y_down, 0 , 15, 0,0)==-1)
    {
         if (SheetHistoryIndex>0)
          {
             CurrentSheet=SheetHistory[--SheetHistoryIndex];
          } 
         ChangeSheet=1;
         MenuPaintExecute=1;
    }
  }  
  
  
  
  if (ChangeSheet==0)
  {
  
  
    //akcja przycisku menu
    if ((X_up>1) && (X_up<15) && (Y_up>115) && (Y_up<128))
    {
      #ifdef  NOPASS
        MenuLevel=11;
      #else
        if ((g_level==0) && (g_NeedPass==1))
        {
          MenuLevel=10;
        }else{
          MenuLevel=11;
        }
      #endif
      
      MenuPaintExecute=1;
        
      if ((!(AT91C_IFLASH_MEM->Config.SoundOff))) {
        ButtonBipCounter=g_counter+8;
          AT91C_BASE_PIOA->PIO_SODR =SpeakerPin;
        } 
      Xdown=0;
    }
    else  if ((X_down>1) && (X_down<15) && (Y_down>115) && (Y_down<128))
    {
       if ((!(AT91C_IFLASH_MEM->Config.SoundOff)) && (Xdown==0)) {
        ButtonBipCounter=g_counter+8;
          AT91C_BASE_PIOA->PIO_SODR =SpeakerPin;
        } 
       Xdown=1;
    }
    else
    {

      Xdown=0;
      
    //STREFY przycisków
      if      ((CurrentSheet==0)  && (PaternDesign==MainScreenTermo)) {Termostat_Event(ScreenLokal);}
      else if ((CurrentSheet>0)   && (PaternDesign==SubScreenTermo)) {Termostat_Event(ScreenLokal);}
      else if ((CurrentSheet==0)  && (PaternDesign==MainScreenTimetable)) {Timetable_Event(CurrentSheet);}
      else if ((CurrentSheet>0)   && (PaternDesign==SubScreenTimetable)) {Timetable_Event(CurrentSheet);}    
      else if ((CurrentSheet==0)  && (PaternDesign==MainScreenAlarm)) {Alarm_Event(CurrentSheet);}
      else if ((CurrentSheet>0)   && (PaternDesign==SubScreenAlarm)) {Alarm_Event(CurrentSheet);}   
      else {  
      
      
          for (char i=0; i<ScrenButtonNr; i++)
          {
            
            if ((ScreenPaternDesign[PaternDesign].Button[i].Width>0) && (ScreenLokal->Button[i].Hide==0))
            {
            
                  BODP=Button(ScreenLokal->Button[i].Text,  //*  (char _Label[15], char poz_X , char poz_Y ,char width, char height, char LineType) 
                  ScreenPaternDesign[PaternDesign].Button[i].Poz.x,
                  ScreenPaternDesign[PaternDesign].Button[i].Poz.y,
                  ScreenPaternDesign[PaternDesign].Button[i].Width,
                  ScreenPaternDesign[PaternDesign].Button[i].Height
                  );
               
                MenuPaintExecute=1;  
               if (BODP==1)
                 {   
                  //je¿eli ustawiona akcja otwarcia strony 
                  if (ScreenLokal->Button[i].OpenPage) 
                   {
                     CurrentSheet=ScreenLokal->Button[i].OpenPage-1;
                     
                   }
                 }
                if (BODP==2)
                 {
                   if (MainStage.StageButton[i+1+(10*CurrentSheet)]==0) 
                   {
                     MainStage.StageButton[i+1+(10*CurrentSheet)]=1;
                     g_NextTimeProgram=g_counter+ProgramPeriod;
                     Program(&gProg);
                   }
                  
                   
                 }else{
                   MainStage.StageButton[i+1+(10*CurrentSheet)]=0;
                   
                 }
            }
          }
      }
    }

  
  }
  
  if (MenuPaintExecute) MenuPaint();
    
  }
}//_______________________ Koniec funkcji MainSide_Event __________________________





//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    MainSide_Refresh
//* Funkcja strony g³ównej menu
//*-----------------------------------------------------------------------------
//* Efekt widoczny po odœwierzeniu LCD (RepaintLCD)
//*-----------------------------------------------------------------------------
void MainSide_Refresh(_Screen* ScreenLokal)
{
  if (!g_PC_Connected)
  {
  
  char PaternDesign=(ScreenLokal->PaternDesign);
  
  
  const _ScreenPaternDesign *ScreenPaternDesign;
  if (CurrentSheet==0) 
  {
    ScreenPaternDesign=ScreenSetTab_0;
  }else{
    ScreenPaternDesign=ScreenSetTab_1; 
  }  
  

         
      
  if      ((CurrentSheet==0)  && (PaternDesign==MainScreenTermo)) {Termostat_Refresh(ScreenLokal);}
  else if ((CurrentSheet>0)   && (PaternDesign==SubScreenTermo)) {Termostat_Refresh(ScreenLokal);}
      else if ((CurrentSheet==0)  && (PaternDesign==MainScreenTimetable)) {  }
      else if ((CurrentSheet>0)   && (PaternDesign==SubScreenTimetable)) {  }    
      #ifdef __ALARM
        else if ((CurrentSheet==0)  && (PaternDesign==MainScreenAlarm)) {Alarm_Refresh(CurrentSheet);}
        else if ((CurrentSheet>0)   && (PaternDesign==SubScreenAlarm)) {Alarm_Refresh(CurrentSheet);} 
      #endif
      else   
      {  
  
             char val;
           
          //on/off
            for (char i=0; i<ScrenGraphNr; i++)
            {
                if ((ScreenLokal->Graph[i].Type==GraphOnOff)&& (ScreenLokal->Graph[i].PlusOnOff==0) && (ScreenPaternDesign[PaternDesign].Graph[i].Width>0))
                {
                  //je¿eli wejœcie
                  if ((ScreenLokal->Graph[i].Nr>=0) && (ScreenLokal->Graph[i].Nr!=0xFF))
                  {
                     val=(PortStageRead(MainStage.StageI[ScreenLokal->Graph[i].Nr].Port, ScreenLokal->Graph[i].Port));
                  }//je¿eli zmienna
                  else
                  {
                     val=(Variables[ScreenLokal->Graph[i].Port/8]>>(ScreenLokal->Graph[i].Port%8)) & 0x1;
                  }
                  
                  
                  if (val)
                   {
                      Label(Speech[g_leng].Wl,2,ScreenPaternDesign[PaternDesign].Graph[i].Poz.x,ScreenPaternDesign[PaternDesign].Graph[i].Poz.y+(ScreenPaternDesign[PaternDesign].Graph[i].Height/2)-3,3,ScreenPaternDesign[PaternDesign].Graph[i].Width,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
                   }else{
                      Label(Speech[g_leng].Wyl,2,ScreenPaternDesign[PaternDesign].Graph[i].Poz.x,ScreenPaternDesign[PaternDesign].Graph[i].Poz.y+(ScreenPaternDesign[PaternDesign].Graph[i].Height/2)-3,3,ScreenPaternDesign[PaternDesign].Graph[i].Width,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
                   }
                }else if ((((ScreenLokal->Graph[i].Type==GraphText)  && (ScreenLokal->Graph[i].PlusOnOff==1)) || (ScreenLokal->Graph[i].Type==GraphOnOff)) && (ScreenPaternDesign[PaternDesign].Graph[i].Width>0))
                {
                   //je¿eli wejœcie
                  if ((ScreenLokal->Graph[i].Nr>=0) && (ScreenLokal->Graph[i].Nr!=0xFF))
                  {
                     val=(PortStageRead(MainStage.StageI[ScreenLokal->Graph[i].Nr].Port, ScreenLokal->Graph[i].Port));
                  }//je¿eli zmienna
                  else
                  {
                     val=(Variables[ScreenLokal->Graph[i].Port/8]>>(ScreenLokal->Graph[i].Port%8)) & 0x1;
                  }
                  
                  
                   if (val)
                   {
                      Label(Speech[g_leng].Wl,2,ScreenPaternDesign[PaternDesign].Graph[i].Poz.x,ScreenPaternDesign[PaternDesign].Graph[i].Poz.y+(ScreenPaternDesign[PaternDesign].Graph[i].Height/2)-7,3,ScreenPaternDesign[PaternDesign].Graph[i].Width,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
                   }else{
                      Label(Speech[g_leng].Wyl,2,ScreenPaternDesign[PaternDesign].Graph[i].Poz.x,ScreenPaternDesign[PaternDesign].Graph[i].Poz.y+(ScreenPaternDesign[PaternDesign].Graph[i].Height/2)-7,3,ScreenPaternDesign[PaternDesign].Graph[i].Width,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
                   }
                }
             }
          
            
 
                  
        //analogi
          char IlLiter=0;
          char txt[10]={0,0,0,0,0,0,0,0,0,0};
          signed short Value;         
          for (char i=0; i<ScrenAnalogNr; i++)
          {
            if ((ScreenPaternDesign[PaternDesign].Value[i].Width>0)  & ((ScreenLokal->Analog[i].Nr>0) || ((ScreenLokal->Analog[i].Nr==0)&&(ScreenLokal->Analog[i].Reg>0))))
                     {
                     IlLiter=0;
                     Value=MainStage.StageI[ScreenLokal->Analog[i].Nr].Value[ScreenLokal->Analog[i].Reg];

                     if (Value==(signed short)0x8AAA)  //je¿eli sygna³ niewiarygodny
                     {
                       txt[0]='-'; txt[1]='?';txt[2]='?';txt[3]='?'; txt[4]='-'; IlLiter+=5;
                     }else{ //je¿eli sygna³ wiarygodny
                         if (ScreenLokal->Analog[i].Flip)
                         {
                             IlLiter=sIntToStr(Value, &(txt[0])); //(char Value, char *Vstring, char ill);
                             for (char k=0; k<3; k++) if (ScreenLokal->Analog[i].Text[k]>0) {txt[IlLiter++]=ScreenLokal->Analog[i].Text[k];}  
                         }else{
                             for (char k=0; k<3; k++) if (ScreenLokal->Analog[i].Text[k]>0) {IlLiter++; txt[k]=ScreenLokal->Analog[i].Text[k];}  
                             IlLiter+=sIntToStr(Value, &(txt[IlLiter])); //(char Value, char *Vstring, char ill);
                         }
                     }   
                         Label(txt,0,
                               ScreenPaternDesign[PaternDesign].Value[i].Poz.x, 
                               ScreenPaternDesign[PaternDesign].Value[i].Poz.y,IlLiter,
                               ScreenPaternDesign[PaternDesign].Value[i].Width,
                               0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
                         
                     }
          }
          
           
          
          //grafika
            for (char i=0; i<ScrenGraphNr; i++)
            {
              if (ScreenLokal->Graph[i].Type==GraphWartosc)
              if (ScreenPaternDesign[PaternDesign].Graph[i].Width>0)
              {
                      ScreenGraph(//(char poz_X , char poz_Y ,char width, char height,_Graph *Graph)
                      ScreenPaternDesign[PaternDesign].Graph[i].Poz.x,
                      ScreenPaternDesign[PaternDesign].Graph[i].Poz.y,
                      ScreenPaternDesign[PaternDesign].Graph[i].Width,
                      ScreenPaternDesign[PaternDesign].Graph[i].Height,
                      &(ScreenLokal->Graph[i]));  //graf2   //*  Label- etykieta,  poz_X - pozycja lewy róg, poz_Y - pozycja dolny róg, height, with
              
              }
             }
         
          
          
        
            //Du¿a temperatura
            if ((ScreenPaternDesign[PaternDesign].Temperature.Width>0))
              {
                  char dl=0;
                  signed short Value=MainStage.StageI[ScreenLokal->Analog[0].Nr].Value[ScreenLokal->Analog[0].Reg];
                  
                  //je¿eli sygna³ nie wiarygodny
                  if  ((Value)==(signed short)0x8AAA)          
                  { 
                    txt[0]='-';txt[1]=' ';txt[2]='?'; txt[3]=' ';txt[4]='-'; txt[5]=0;
                    dl=5;                    
                  }//je¿eli s³aba bateria
                  else if  ((Value)&(signed short)0x8000)          
                  { 
                    txt[0]='B';txt[1]='A';txt[2]='T'; txt[3]='E';txt[4]='R'; txt[5]='Y'; txt[5]=0;
                    dl=6;    
                  }else{
                    
                    dl=sIntToStr(Value,txt);
                    txt[dl++]=127;
                  }
                  Label20(txt,2,0,ScreenPaternDesign[PaternDesign].Temperature.Poz.y,dl,60,0,105,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 

              }
              
           char time_char[9];
            //Czas
            if ((ScreenPaternDesign[PaternDesign].Time.Width>0))
              {                  
                  
                  TimeToStr(DateTime.Hour,DateTime.Minute,DateTime.Second,time_char);
                  time_char[8]=0;
                  Label(time_char,0,
                        ScreenPaternDesign[PaternDesign].Time.Poz.x,
                        ScreenPaternDesign[PaternDesign].Time.Poz.y,8,43,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
              }
          
            //Date
            if ((ScreenPaternDesign[PaternDesign].Date.Width>0))
              {                  
                  DateToStr(DateTime.Day,DateTime.Mounth,DateTime.Year,time_char);//(char dzin, char mies, char rok, char Vstring[8])
                  time_char[8]=0;
                  Label(time_char,0,
                        ScreenPaternDesign[PaternDesign].Date.Poz.x,
                        ScreenPaternDesign[PaternDesign].Date.Poz.y,
                        8,33,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
              }          
    
      } 
            //wskaŸnik ¿ycia
          if (CurrentSheet==0) 
          {
          
            if ((MainSide_puls==0) || (MainSide_puls>3))
            { 
              
              char ekran1[2][13] = {
                         {0,0,224,24,232,244,244,244,232,24,224,0,0},
                          {0,0,0,3,2,5,5,5,2,3,0,0,0}
                          };
        
              char ekranSlave[2][13] = {
                          {0,0,224,248,152,124,180,220,56,248,224,0,0},
                          {0,0,0,3,3,7,5,7,3,3,0,0,0},
                          };
              
              if ((SlaveMode) )//je¿eli tryb slave
              {
                    for (char i=0; i<=12; i++)
                    {
                 
                    LCD_Canvas[0][i+115]=ekranSlave[0][i];
                    LCD_Canvas[1][i+115]=ekranSlave[1][i];
                    }
              }else{
                
                  for (char i=0; i<=12; i++)
                  {
                 
                    LCD_Canvas[0][i+115]=ekran1[0][i];
                    LCD_Canvas[1][i+115]=ekran1[1][i];
                  }
                }
             //if ((!g_SlaveConnect) && (SlaveMode)) H_line(2,121,7,0);
            }
            if ((MainSide_puls==1) )
            {
              
              if ((g_SlaveConnect) || (!SlaveMode))
              {
                char ekran2[2][13] = {
                          {240,28,230,250,251,253,253,253,251,250,230,28,240},
                          {1,7,12,11,27,23,23,23,27,11,12,7,1},
                          };
              
                for (char i=0; i<=12; i++)
                {
                 
                  LCD_Canvas[0][i+115]=ekran2[0][i];
                  LCD_Canvas[1][i+115]=ekran2[1][i];
                }
              }
        
            }
             
        
          if (g_Stop_Program==0)
          {  
           if (MainSide_puls>7) {MainSide_puls=0;} else {MainSide_puls++;}    
          }else{
            MainSide_puls=7; 
          }
           
          }
          
  
  
  /*
         char txt[5];
          sprintf(txt, "%d", RF_Status.Ch);
          Label(txt,0,3,70,8,33,1,127,0);
          sprintf(txt, "%d", RF_Status.ChSlave);
          Label(txt,0,3,60,8,33,1,127,0);
    */  
  
    if (g_PC_Connected) MenuPaint();
  }  
}//_______________________ Koniec funkcji MainSide_Refresh __________________________








//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    Termostat_Paint
//* Funkcja strony g³ównej menu
//*-----------------------------------------------------------------------------
//* Efekt widoczny po odœwierzeniu LCD (RepaintLCD)
//*-----------------------------------------------------------------------------
void Termostat_Paint(_Screen* ScreenLokal)
{
  if (CurrentSheet!=0)  List_Paint(Speech[g_leng].Termo_,1,108,0,0,3," ",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      
          
  char txt[7];

  H_line(0,70,65,1);
  char GraphTermometr[3][18] = 
                    { 
                     {128,192,192,128,128,224,128,224,128,224,128,128,128,128,128,128,128,0},
                     {3,7,7,3,3,3,3,3,2,2,2,14,2,14,2,14,2,1},
                     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                  };
  
  CopyToCanvasPoint(GraphTermometr[0], 1, 33, 18,18);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);           

  
  if (ScreenLokal->Thermo.AutoOn==1)
  {
            
      Label("Auto",0,17,52,6,20,0,105,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
      
      char dl=sIntToStr((ScreenLokal->Thermo.SetTemperature),txt); 
      txt[dl++]=127;
      Label(txt,0,17,43,dl,30,0,105,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
      char txt[2]; txt[1]=0; 
      
      
      char MaxTmp;
      if (ScreenLokal->Thermo.Temperature[0]>ScreenLokal->Thermo.Temperature[1])
      {
        MaxTmp=ScreenLokal->Thermo.Temperature[0]; 
      }else{
        MaxTmp=ScreenLokal->Thermo.Temperature[1];
      }
      
      if (ScreenLokal->Thermo.SetTemperature==MaxTmp)
      {
       txt[0]=31; 
      }else{
       txt[0]=30;       
      }
      
      Button(txt,45,44,15,17);
      Button(Speech[g_leng].Ustaw,17,26,43,13); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
      Button("Auto off",4,3,56,13); //(etykieta ,char poz_X , char poz_Y ,char width, char height)

}else{

      char dl=sIntToStr((ScreenLokal->Thermo.SetTemperature),txt); 
      txt[dl++]=' ';
      txt[dl++]=127;
      Label(txt,2,20,39,dl,40,0,105,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
      txt[1]=0; txt[0]=30;
      Button(txt,19,48,41,18);
      txt[0]=31;
      Button(txt,19,21,41,18);
      Button("Auto on",4,3,56,13); //(etykieta ,char poz_X , char poz_Y ,char width, char height) 
  }
  
  if ((ScreenLokal->Button[0].Hide==0) && (CurrentSheet==0))
  {
    Button(ScreenLokal->Button[0].Text,  //*  (char _Label[15], char poz_X , char poz_Y ,char width, char height, char LineType) 
                  25,103,39,22 // X,Y,Width,Height
                  );
  }
}//_______________________ Koniec funkcji Termostat_Paint __________________________
  


//sprawdzenie czy zmienne specjalne termostatu s¹ odczytywane przez serwer. Je¿eli tak to przyœpieszenie wys³ania danych;
void ExecuteSpecialThermVar(void)
{
   for (char IDX=0; IDX<NET.HTTP.VarSetCount; IDX++)
     {  
       if ((NET.HTTP.NetNodeSet[IDX].NetDataType=='c') || (NET.HTTP.NetNodeSet[IDX].NetDataType=='d'))
       {
         NET.Status.NextHttpOrder=HTTP_POST;
         break;
       } 
     }
}


//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    Termostat_Event
//* Funkcja strony g³ównej menu
//*-----------------------------------------------------------------------------
//*-----------------------------------------------------------------------------


void Termostat_Event(_Screen* ScreenLokal)
{
  char Repaint=0;
  char WriteTmp;
  char txt[5];
  char txt2[5];

 if ((CurrentSheet>0) && (List_Event(X_up, Y_up, X_down, Y_down, 0 , 15, 0,0)==-1))
    {
       if (SheetHistoryIndex>0)
          {
             CurrentSheet=SheetHistory[--SheetHistoryIndex];
          } 
         Repaint=1; 
    }else{
  
            //przycisk opcjonalny
                if ((ScreenLokal->Button[0].Hide==0) && (CurrentSheet==0))
                {
                   if (Button(ScreenLokal->Button[0].Text,  //*  (char _Label[15], char poz_X , char poz_Y ,char width, char height, char LineType) 
                            25,103,39,22 // X,Y,Width,Height
                            )==1)
                       {
                         MainStage.StageButton[1+(10*CurrentSheet)]=1;
                         
                         //je¿eli ustawiona akcja otwarcia strony
                         if (ScreenLokal->Button[0].OpenPage) 
                         {
                           CurrentSheet=ScreenLokal->Button[0].OpenPage-1;
                           Repaint=1;
                         }
                         
                       }
                }
      
                
                
      
            _ProgramTab *pProgramTab=(_ProgramTab*)((AT91C_IFLASH_MEM->FlashProgram));
            
            
            //je¿eli AutoOn
            if (ScreenLokal->Thermo.AutoOn==1)
            {
                txt[1]=0; txt[0]=31;
            
                if (Button("Auto off",4,3,56,13)==1)  //(etykieta ,char poz_X , char poz_Y ,char width, char height)
                    {
                    Repaint=1; 
                                
                      //zapis do flash
                        WriteTmp=0;
                        ScreenLokal->Thermo.AutoOn=WriteTmp;
                        unsigned int Save = __get_interrupt_state();  
                        __disable_interrupt();
                        FlashWrite((char*)&(pProgramTab->Screen[CurrentSheet].Thermo.AutoOn),&WriteTmp,1);
                        __set_interrupt_state(Save);
                        __enable_interrupt(); 
                        
                        //koniec zapisu do flash
                        
                        gProg.Screen[CurrentSheet].Thermo.SetTemperature=pProgramTab->Screen[CurrentSheet].Thermo.SetTemperature;
                   }
                
                if (Button(Speech[g_leng].Ustaw,17,26,43,13)==1) //(etykieta ,char poz_X , char poz_Y ,char width, char height)
                {
                  MenuLevel=12;
                  Repaint=1; 
                }
                
                
                //prze³¹czenie temperatur w trybie auto
                signed   short H_Tmp;
                if (ScreenLokal->Thermo.Temperature[0]>ScreenLokal->Thermo.Temperature[1])
                {
                  H_Tmp=ScreenLokal->Thermo.Temperature[0]; 
                }else{
                  H_Tmp=ScreenLokal->Thermo.Temperature[1];
                }
                
                
                if (ScreenLokal->Thermo.SetTemperature==H_Tmp)
                {
                 txt[0]=31; 
                }else{
                 txt[0]=30;
                }
                txt[1]=0;
                
                
                if (Button(txt,45,44,15,17)==1)
                  {
                      if (ScreenLokal->Thermo.SetTemperature==ScreenLokal->Thermo.Temperature[0])
                      {      
                          ScreenLokal->Thermo.SetTemperature=ScreenLokal->Thermo.Temperature[1];
                          Repaint=1;
                        
                      }else{
                          ScreenLokal->Thermo.SetTemperature=ScreenLokal->Thermo.Temperature[0];
                          Repaint=1;
                      }
                      
                  }
                  
                
            }
            else
            {
         
                if (Button("Auto on",4,3,56,13)==1) //(etykieta ,char poz_X , char poz_Y ,char width, char height) 
                  {
                    Repaint=1; 
                                
                      //zapis do flash
                        WriteTmp=1;
                        ScreenLokal->Thermo.AutoOn=WriteTmp;
                        unsigned int Save = __get_interrupt_state();  
                        __disable_interrupt();
                        FlashWrite((char*)&(pProgramTab->Screen[CurrentSheet].Thermo.AutoOn),&WriteTmp,1);
                        __set_interrupt_state(Save);
                        __enable_interrupt(); 
                        //koniec zapisu do flash
                        
                        if (((gProg.Screen[CurrentSheet].Thermo.Chart[DateTime.DayOfWeek]>>DateTime.Hour) & 0x1)==1)
                        {
                          gProg.Screen[CurrentSheet].Thermo.SetTemperature=gProg.Screen[CurrentSheet].Thermo.Temperature[0]; 
                        }else{
                          gProg.Screen[CurrentSheet].Thermo.SetTemperature=gProg.Screen[CurrentSheet].Thermo.Temperature[1]; 
                        }

                   }
                        
            
                txt[1]=0; txt[0]=30;
                if ((Button(txt,19,48,41,18)==2)  && ((ScreenLokal->Thermo.SetTemperature)<500)) {((ScreenLokal->Thermo.SetTemperature))++;  Repaint=1; }  //w górê
            
                txt2[0]=31; txt2[1]=0;
                if ((Button(txt2,19,21,41,18)==2) && ((ScreenLokal->Thermo.SetTemperature)>0)) {((ScreenLokal->Thermo.SetTemperature))--; Repaint=1; }   //w dó³
            
                 //zapis zmiany temperatury zadanej w trybie manualnym
                 if ((Button(txt,19,48,41,18)==1) || (Button(txt2,19,21,41,18)==1))
                 {
                      //zapis do flash
                        unsigned int Save = __get_interrupt_state();  
                        __disable_interrupt();
                          FlashWrite((char*)&(pProgramTab->Screen[CurrentSheet].Thermo.SetTemperature),&ScreenLokal->Thermo.SetTemperature,1);
                        __set_interrupt_state(Save);
                        __enable_interrupt(); 
                        //koniec zapisu do flash
                 }

            }
            
            

            
    }
 
 

 
 if (Repaint==1) 
 {
   ThermostatsTimeChartExecute(); 
   ThermostatsExecute();
   MenuPaint();
   ExecuteSpecialThermVar();
 }
}//_______________________ Koniec funkcji Termostat_Event __________________________
  




//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    Termostat_Refresh
//* Funkcja strony g³ównej menu
//*-----------------------------------------------------------------------------
//*-----------------------------------------------------------------------------
void Termostat_Refresh(_Screen* ScreenLokal)
{

  char dl;
  char txt[6];
  signed short Value=MainStage.StageI[ScreenLokal->Thermo.SensorNode].Value[ScreenLokal->Thermo.SensorReg];
  
   if (ScreenLokal->Thermo.AutoOn==1)
  {
           
      char dl=sIntToStr((ScreenLokal->Thermo.SetTemperature),txt); 
      txt[dl++]=127;
      Label(txt,0,17,43,dl,27,0,105,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
      
  }
  
  
   if  ((Value)!=(signed short)0x8AAA)          
  { 
    
    dl=sIntToStr((MainStage.StageI[ScreenLokal->Thermo.SensorNode].Value[ScreenLokal->Thermo.SensorReg]),txt);
    txt[dl++]=127;
  }else{
    txt[0]='-';txt[1]=' ';txt[2]='?'; txt[3]=' ';txt[4]='-'; txt[5]=0;
    dl=5;
  }

  char Ypoz=81;
  if ((ScreenLokal->Button[0].Hide==0) || (CurrentSheet>0))
  {
    Ypoz=73;
  }
  
  
  static DzielBat;
  
  
  if (DzielBat<20) {DzielBat++;} else {DzielBat=0;} 
  
  if((MainStage.StageI[ScreenLokal->Thermo.SensorNode].Port & 0x8000) && (DzielBat>12) && ((Value)!=(signed short)0x8AAA))
  {
    Label20(0,2,1,Ypoz,0,60,0,105,0);
    CopyToCanvasPoint((char*)BateriaWyladowanaDuza, 24, Ypoz+10, 19,8);
  }  
  else
  {  
    Label20(txt,2,1,Ypoz,dl,60,0,105,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
  }
  
  
  
  V_line(9,36,12,0);
  
  static int TermAmionation;
  
  TermAmionation++;
  if (TermAmionation>12) TermAmionation=1;
  if ((ScreenLokal->Thermo.ActorNode<0xFF) && (ScreenLokal->Thermo.ActorNode>=0))   //je¿eli wêze³
  {
    if ((PortStageRead(MainStage.StageQ[ScreenLokal->Thermo.ActorNode].Port , ScreenLokal->Thermo.ActorPort))==0) TermAmionation=4;
  }else{  //Je¿eli zmienna
    if(((Variables[ScreenLokal->Thermo.ActorPort/8]>> (ScreenLokal->Thermo.ActorPort%8))&0x1)==0) TermAmionation=4;
  }
  
  V_line(9,36,TermAmionation,1);

  static short int LastTemp;
  
  if (LastTemp!=ScreenLokal->Thermo.SetTemperature)
  {  

        signed   short H_Tmp;
      if (ScreenLokal->Thermo.Temperature[0]>ScreenLokal->Thermo.Temperature[1])
      {
        H_Tmp=ScreenLokal->Thermo.Temperature[0]; 
      }else{
        H_Tmp=ScreenLokal->Thermo.Temperature[1];
      }
      
      if (ScreenLokal->Thermo.AutoOn==1)
      {
        if (ScreenLokal->Thermo.SetTemperature==H_Tmp)
        {
         txt[0]=31; 
        }else{
         txt[0]=30;       
        }
        txt[1]=0; 
        
        Button(txt,45,44,15,17);
      }
      LastTemp=ScreenLokal->Thermo.SetTemperature;
  }

  
}//_______________________ Koniec funkcji Termostat_Refresh __________________________
  






void Timetable_Paint(char Screen)
{         
  
     if(Screen) List_Paint(Speech[g_leng].Terminarz,0,1,0,0,3," ",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      


    char L_TaskAmount;
  
    L_TaskAmount=AT91C_IFLASH_MEM->Config.Timetable[Screen].TaskAmount;
    if (L_TaskAmount>4) L_TaskAmount=4;
    char *ptTaskAmount=&(AT91C_IFLASH_MEM->Config.Timetable[Screen].TaskAmount);
    if (L_TaskAmount==0)
    {
      L_TaskAmount=1;
      
              //zapis do flash
          unsigned int Save = __get_interrupt_state();  
          __disable_interrupt();
          //at91flashWrite((int*)(&(AT91C_IFLASH_MEM->Config.Timetable[Screen].TaskAmount)),0, &L_TaskAmount,1);
          FlashWrite(ptTaskAmount, &L_TaskAmount, 1);
          __set_interrupt_state(Save);
          __enable_interrupt();
          //koniec zapisu do flash
  
    }
   
      
      
      char Ychart=108;
      char CharOnOff[]={130,0};
      char TimeString[6];
      TimeString[5]=0;
      for (char TN=0; TN<L_TaskAmount; TN++)
      {
          
          H_line(1,Ychart,62,1);  
          H_line(1,Ychart-1,62,1);  
          H_line(1,Ychart-27,62,1);   
          V_line(0,Ychart-26,26,1); 
          V_line(63,Ychart-26,26,1); 
          
          
          Button(CharOnOff,2,Ychart-15,13,13);
          if ((TN>0) && (TN==L_TaskAmount-1)) Button("-",53,Ychart-13,11,14);
          
          char ekran_tab[10] =  {0,4,4,10,11,16,16,32,64,0};
          char LabelNeg=0;
          if (AT91C_IFLASH_MEM->Config.Timetable[Screen].Task[TN].Enable) 
          {
           CopyToCanvasPoint((char *)ekran_tab,44,Ychart-13,8,10);   
           LabelNeg=1;
          }
          
          CharToStr2(AT91C_IFLASH_MEM->Config.Timetable[Screen].Task[TN].Hour, TimeString); //(char Value, char *Vstring, char ill);
          TimeString[2]=':';
          CharToStr2(AT91C_IFLASH_MEM->Config.Timetable[Screen].Task[TN].Minute, &(TimeString[3])); //(char Value, char *Vstring, char ill);
          Label(TimeString,2,17,Ychart-13,5,25,0,128,LabelNeg);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)   
       
                
          for (char i=0; i<7; i++)
          {
            Label(&(Speech[g_leng].PWSCPSN[i]),2,4+i*8,Ychart-25,1,6,0,128,((AT91C_IFLASH_MEM->Config.Timetable[Screen].Task[TN].Week>>i) &0x1));//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)   
          }
          

          
          Ychart=Ychart-27;
          
          if ((TN==L_TaskAmount-1)&& (TN<3)) Button("+",53,Ychart-13,11,14);
 
      }
      
    

      

}//________________________________________________________________________



__Task *TaskToChange;
char TimetableSet_Hour;
char TimetableSet_Minute;
char TimetableSet_Week;
void Timetable_Event(char Screen)
{      
   char Repaint=0;
   char CharToSave=0;
   char TaskAmount=AT91C_IFLASH_MEM->Config.Timetable[Screen].TaskAmount;

   if (TaskAmount>4) TaskAmount=4;
   
   if ((ScreenNr>0) && (List_Event(X_up, Y_up, X_down, Y_down, 0 , 15, 0,0)==-1))
    {
       if (SheetHistoryIndex>0)
          {
             CurrentSheet=SheetHistory[--SheetHistoryIndex];
          } 
         Repaint=1; 
    }else{
            
      char Ychart=108;
     
      if ((X_down>1) && (X_down<63) && (Y_down>108)&& (Y_down<128)) Repaint=1;
      
     char PlusMinButton=0;  
      
      char CharOnOff[]={130,0};
      char ButtonRes;
      for (char TN=0; TN<TaskAmount; TN++)
      {
        if ((TN==TaskAmount-1)&& (TN<3)) 
        {
            ButtonRes=Button("+",53,Ychart-13-27,11,14);
            if (ButtonRes==1)
            {
              TaskAmount++;
              //zapis do flash
              unsigned int Save = __get_interrupt_state();  
              __disable_interrupt();
              FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.Timetable[Screen].TaskAmount)), (char*) (&(TaskAmount)), 1);
              __set_interrupt_state(Save);
              __enable_interrupt();
              //koniec zapisu do flash
              Repaint=1; 
              PlusMinButton=1;
            }
            if (ButtonRes==2)
            {
              PlusMinButton=1;
            }
        }
        
        if ((PlusMinButton==0) && (TN>0) && (TN==TaskAmount-1)) 
        {
          ButtonRes=Button("-",53,Ychart-13,11,14);
          if (ButtonRes==1)
          {
              TaskAmount--;
              //zapis do flash
              unsigned int Save = __get_interrupt_state();  
              __disable_interrupt();
              FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.Timetable[Screen].TaskAmount)), (char*) (&(TaskAmount)), 1);
              __set_interrupt_state(Save);
              __enable_interrupt();
              //koniec zapisu do flash
              Repaint=1; 
              PlusMinButton=1;
          }
          if (ButtonRes==2)
          {
              PlusMinButton=1;
          }
           
        }
        
        if (AT91C_IFLASH_MEM->Config.Timetable[Screen].Task[TN].Enable) 
        {
            if (Button(CharOnOff,2,Ychart-15,13,13)==1)
            {
               CharToSave=0;
              //zapis do flash
                unsigned int Save = __get_interrupt_state();  
                __disable_interrupt();
                FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.Timetable[Screen].Task[TN].Enable)), (char*) (&(CharToSave)), 1);
                __set_interrupt_state(Save);
                __enable_interrupt();
                //koniec zapisu do flash
                Repaint=1; 
            } 
         }else{
              if(Button(CharOnOff,2,Ychart-15,13,13)==1)
              {
                 CharToSave=1;
                //zapis do flash
                  unsigned int Save = __get_interrupt_state();  
                  __disable_interrupt();
                  FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.Timetable[Screen].Task[TN].Enable)), (char*) (&(CharToSave)), 1);
                  __set_interrupt_state(Save);
                  __enable_interrupt();
                  //koniec zapisu do flash
                  Repaint=1; 
              }
            }
            
            //wybór strefy 
            if ((Button(CharOnOff,2,Ychart-15,13,13)==0) && (PlusMinButton==0) && (X_down>1) && (X_down<63) && (Y_down>Ychart-27)&& (Y_down<Ychart))
            {
              MenuPaint();
              H_line(1,Ychart-2,62,1);  
              H_line(1,Ychart-26,62,1);   
              V_line(1,Ychart-26,24,1); 
              V_line(62,Ychart-26,26,1); 
            }
        
            //edycja strefy 
            if ((Button(CharOnOff,2,Ychart-15,13,13)==0) && (PlusMinButton==0) && (X_up>1) && (X_up<63) && (Y_up>Ychart-27)&& (Y_up<Ychart))
            {
                TaskToChange=&(AT91C_IFLASH_MEM->Config.Timetable[Screen].Task[TN]);
                TimetableSet_Hour=AT91C_IFLASH_MEM->Config.Timetable[Screen].Task[TN].Hour;
                TimetableSet_Minute=AT91C_IFLASH_MEM->Config.Timetable[Screen].Task[TN].Minute;
                TimetableSet_Week=AT91C_IFLASH_MEM->Config.Timetable[Screen].Task[TN].Week;
                MenuLevel=13;
                Repaint=1; 
            }
        
            
        
          Ychart=Ychart-27;

        }//koniec for (TN)
      
      
  
    }
   if (Repaint) MenuPaint();
}






void TimetableSet_Paint(void)
{          
       List_Paint(Speech[g_leng].Terminarz,1,108,0,0,3," ",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      
      
      char day=0;
      char hlight=0;

      for (char row=0; row<3; row++)
      for (char col=0; col<3; col++)
      { 
        if (++day<8)
        {
            H_line(4+col*18,105-row*14,19,1);
            H_line(4+col*18,105-(row+1)*14,19,1);
            V_line(4+col*18,105-(row+1)*14,14,1);
            V_line(4+(col+1)*18,105-(row+1)*14,14,1);
            
            hlight=(((TimetableSet_Week>>(day-1))) &0x1);
            Label((char*)DeyOfWeekStr[g_leng][day],2,6+col*18 ,105-(row+1)*14+3,2,14,0,128,hlight);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)   

        }
      }
      
       Label(Speech[g_leng].Godzina,0,5,51,12,40,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
              
      char LabelButt[2];
        
      LabelButt[1]=0;
      LabelButt[0]=30; //góra - godzina
      Button(LabelButt,5,35,26,15);
      LabelButt[0]=30; //góra- minuta
      Button(LabelButt,34,35,26,15);
      
      char time_char[2];
      
       CharToStr2(TimetableSet_Hour, time_char); //(char Value, char *Vstring, char ill);
       Label(time_char,1,29,25,2,15,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
       
       Label(":",0,32,25,2,15,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
       
       CharToStr2(TimetableSet_Minute, time_char); //(char Value, char *Vstring, char ill);
       Label(time_char,0,36,25,2,15,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
              
      

      LabelButt[0]=31; //dó³ godzina
      Button(LabelButt,5, 9,26,15);
      LabelButt[0]=31; //dó³ minuta
      Button(LabelButt,34, 9,26,15);
       

       
}//________________________________________________________________________

    
//*-----------------------------------------------------------------------------
void TimetableSet_Event(void)
      {

         char Repaint=0;
        
  
        if (List_Event(X_up, Y_up, X_down, Y_down, 0 , 15, 0,0)<0)
        {
           //zapis do flash
                  unsigned int Save = __get_interrupt_state();  
                  __disable_interrupt();
                  FlashWrite((char*)(&(TaskToChange->Hour)), (char*) (&(TimetableSet_Hour)), 1);
                  FlashWrite((char*)(&(TaskToChange->Minute)), (char*) (&(TimetableSet_Minute)), 1);
                  FlashWrite((char*)(&(TaskToChange->Week)), (char*) (&(TimetableSet_Week)), 1);
                  __set_interrupt_state(Save);
                  __enable_interrupt();
                  //koniec zapisu do flash
          
          
          MenuLevel=(MenuLevel/10);
          MenuPaint();
        } else if ((Y_down<112)  || (Y_up<112))
        {  
            char day;
            day=0;
            for (char row=0; row<3; row++)
            for (char col=0; col<3; col++)
            { 
              if (++day<8)
              {
                  if ((X_up>4+col*18) && (X_up<4+(col+1)*18) && (Y_up<105-row*14) && (Y_up>105-(row+1)*14))
                  {
                    if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}
                    if ((TimetableSet_Week>>(day-1)) & 0x1)  //jezeli dzieñ aktywny to wy³acz je¿eli nie to w³¹cz
                    {
                       TimetableSet_Week &= ~(1<<(day-1));
                    }else{
                       TimetableSet_Week |= (1<<(day-1));
                    }

                    Repaint=1;
        
                  }

              }
            }
            
            
                  char LabelButt[2];
        
            
            LabelButt[1]=0;
            LabelButt[0]=30; //góra - godzina
            if (Button(LabelButt,5,35,26,15)==2){Repaint=1; if (TimetableSet_Hour<23) TimetableSet_Hour++; else TimetableSet_Hour=0; }
            LabelButt[0]=30; //góra- minuta
            if (Button(LabelButt,34,35,26,15)==2){Repaint=1; if (TimetableSet_Minute<59) TimetableSet_Minute++; else TimetableSet_Minute=0;}
            
      
            LabelButt[0]=31; //dó³ godzina
            if (Button(LabelButt,5, 9,26,15)==2){Repaint=1; if (TimetableSet_Hour>0) TimetableSet_Hour--; else TimetableSet_Hour=23;}
            LabelButt[0]=31; //dó³ minuta
            if (Button(LabelButt,34, 9,26,15)==2){Repaint=1; if (TimetableSet_Minute>0) TimetableSet_Minute--; else TimetableSet_Minute=59;}
                
           
           
           if (Repaint==1) MenuPaint();      
          
        }
}//________________________________________________________________________
    














//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    Wait_Paint
//* Funkcja strony g³ównej menu
//*-----------------------------------------------------------------------------
//* Efekt widoczny po odœwierzeniu LCD (Wait_Paint)
//*-----------------------------------------------------------------------------
void Wait_Paint(void)
{
    CleanCanvas(); 
    
    char ekran_tab[4][46] = {
                  {0,240,252,127,143,243,251,251,251,227,131,3,7,15,14,30,60,120,240,224,192,128,0,0,0,128,192,224,240,112,56,156,206,239,119,3,3,3,227,255,255,31,7,31,254,252},
                  {252,255,255,0,255,255,255,255,255,255,255,254,248,48,48,0,0,0,0,1,3,39,183,182,183,183,123,253,254,254,255,255,255,1,0,0,0,0,255,255,0,0,0,0,255,255},
                  {1,63,255,248,199,63,127,127,127,31,7,1,128,128,192,224,240,120,60,30,15,7,3,3,3,7,15,30,61,123,247,239,223,158,176,0,0,0,31,255,252,192,128,224,255,255},
                  {0,0,1,3,3,7,7,7,7,7,7,3,3,3,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,3,3,3,7,7,7,7,7,7,3,1,0},
                  };
    

    CopyToCanvasPoint((char*)ekran_tab, 20, 40, 32,46);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     

 
    RepaintLCD();
    RepaintLCD();
}//_______________________ Koniec funkcji Wait_Paint __________________________









//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************





//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    MainSide_Paint
//* Funkcja strony g³ównej menu
//*-----------------------------------------------------------------------------
//* Efekt widoczny po odœwierzeniu LCD (RepaintLCD)
//*-----------------------------------------------------------------------------
void Animation_Event(void)
{
  
    //akcja przycisku menu
    if ((X_up>1) && (X_up<60) && (Y_up>1) && (Y_up<128))
    {
      MenuLevel=4;
      MenuPaint();
    }
}//_______________________ Koniec funkcji MainSide_Event __________________________








      //*-----------------------------------------------------------------------
      void AdvanceSet_Paint(void)
      {
        
        List_Paint(Speech[g_leng].Serwis,3,15,15,0,3, "Restart", Speech[g_leng].Czysc_pamiec, Speech[g_leng].Wyjdz , 0,0,0,0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])

        
      }//_______________________ Koniec funkcji MainSide_Paint __________________________


      //*-----------------------------------------------------------------------
      void AdvanceSet_Event(void)
      {
        //char podswietl=0;
        int ans;
        
        ans=List_Event(X_up, Y_up, X_down, Y_down, 3 , 15, 0,15); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        if (ans)
          {
            if (ans==1) 
            {
              //restart
              
              AT91C_BASE_RSTC->RSTC_RCR = AT91C_RSTC_PROCRST | AT91C_RSTC_PERRST | AT91C_RSTC_KEY;
            }
            if (ans==2) 
            {
              MenuLevel=(MenuLevel*10+1);
              //czyœæ wczystko
              
            }
            
          if ((ans<0) || (ans==3)) MenuLevel=1;
          MenuPaint();
          } 
      }//_______________________________________________________________________


   
    //Wyczyœæ wsyzstko
      //*-----------------------------------------------------------------------
      void CleanAll_Event(void)
      {
        AT91S_RSTC  g_AT91S_RSTC;
        int ans;
 
        ans=Ask_Event(X_up, Y_up, X_down, Y_down); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        if (ans)
          {
          if (ans==2) 
          {
                  korekta_X.A=AT91C_IFLASH_MEM->Config.Cor_X.A;
                  korekta_X.B=AT91C_IFLASH_MEM->Config.Cor_X.B;
                  korekta_Y.A=AT91C_IFLASH_MEM->Config.Cor_Y.A;
                  korekta_Y.B=AT91C_IFLASH_MEM->Config.Cor_Y.B;
            
            //czyœæ wszystko 
                  unsigned int Save = __get_interrupt_state();  
                  __disable_interrupt();
                   CleanFlash((char*)(AT91C_IFLASH_MEM),64+sizeof(AT91S_MEM)+sizeof(_Config));
                                     
                    FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.Cor_X)), (char*) (&(korekta_X.A)), sizeof(korekta_X));
                    FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.Cor_Y)), (char*) (&(korekta_Y.A)), sizeof(korekta_Y));
                  __set_interrupt_state(Save);
                  __enable_interrupt();
                  
                      //restar
                  AT91C_BASE_RSTC->RSTC_RCR = AT91C_RSTC_PROCRST | AT91C_RSTC_PERRST | AT91C_RSTC_KEY;


                __set_interrupt_state(Save);
                //koniec zapisu do flash
                
          }
          if ((ans<0) || (ans==1))  MenuLevel=(MenuLevel/10);
          MenuPaint();
          } 
      }//_______________________________________________________________________
    
    


//Lista MENU ******************************************************************************************************************


      //*-----------------------------------------------------------------------
      void MenuSide_Paint(void)
      {
        
        #ifdef __WIFI_MODULE   
          List_Paint("MENU",7,15,0,0,3, "Program ", Speech[g_leng].Ekran, Speech[g_leng].Opcje , Speech[g_leng].Ustawienia,Speech[g_leng].Wezly,"Info ",Speech[g_leng].Siec,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])
        #else
          List_Paint("MENU",6,15,0,0,3, "Program ", Speech[g_leng].Ekran, Speech[g_leng].Opcje , Speech[g_leng].Ustawienia,Speech[g_leng].Wezly,"Info ",0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])
        #endif
                                                                                                                                                    
      }//_______________________ Koniec funkcji MainSide_Paint __________________________



      //*-----------------------------------------------------------------------
      void MenuSide_Event(void)
      {
        //char podswietl=0;
        int ans;
        
        #ifdef __WIFI_MODULE  
          char il=7;
        #else
          char il=6;
        #endif

 
        ans=List_Event(X_up, Y_up, X_down, Y_down, il , 15, 0,0); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        if (ans)
          {
          if ((ans>0) && (ans<=il)) MenuLevel=(MenuLevel*10+ans);
          if (ans<0) {
            MenuLevel=(MenuLevel/10);
           CurModifSheet=0; 
          }
          MenuPaint();
          } 
      }//_______________________________________________________________________





//Wybór poziomu dostêpu *************************************************************************************************************
      //*-----------------------------------------------------------------------
      void MenuAccess_Paint(void)
      {
          #define  Y   4   //przesuniêcie w dó³
          List_Paint("X",3,15,Y,5,0,Speech[g_leng].Poziom1, Speech[g_leng].Poziom2,  Speech[g_leng].Poziom3,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      
          char ekran_tab[10] =  {0,4,4,10,11,16,16,32,64,0};
          CopyToCanvasPoint((char *)ekran_tab,49,97-Y-(15*(g_level)),8,10); 
      }//_______________________ Koniec funkcji MainSide_Paint __________________________


        char pass_tmp[9]={0,0,0,0,0,0,0,0,0};
      //*-----------------------------------------------------------------------
      void MenuAccess_Event(void)
      {
        #define  Y   4   //przesuniêcie w dó³
        int ans;
 
        ans=List_Event(X_up, Y_up, X_down, Y_down, 3 , 15, 5,Y); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        if (ans)
          {
            if ((ans>0) && (ans<4))
            {
              if (ans>1)
              {
                for (char i=0; i<8; i++) pass_tmp[i]=0;
                MenuLevel=(MenuLevel*10+ans);
                MenuPaint();
               
              }
              else
              {
                 MenuLevel=(MenuLevel/10);
                 g_level=ans-1;
                 Message_Paint(2,"","",Speech[g_leng].Poziom1,"","");
              }
            }
            
          }
        else
          {
            if ((X_up>0) &&(Y_up>0)) 
            {
              MenuLevel=(MenuLevel/10);
              MenuPaint();
            }
          }
      }//_______________________________________________________________________





//has³o poziomu 1/2*************************************************************************************************************
char InsertPass1[9]= {0,0,0,0,0,0,0,0,0};

      //*-----------------------------------------------------------------------

      void MenuAccessPass_Paint(char level)
      {
         Number_Paint(Speech[g_leng].Wpisz,"C","E");
          
         if (level==1) Label(Speech[g_leng].Haslo,0,3,102,15,58,0,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
         if (level==2) Label(Speech[g_leng].Haslo,0,3,102,15,58,0,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
         
         
      }//_______________________ Koniec funkcji MainSide_Paint __________________________




      //*-----------------------------------------------------------------------
      void MenuAccessPass_Event(char level)
      {
        int ans;
        char roz=0;
 
        
        ans=Number_Event(X_up, Y_up, X_down, Y_down); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        if (ans>0)          {
            
            if (InsertPass1[8]<=7) 
            { 
                if ((ans<10) || (ans==11)) 
                {
                  if (ans<10) InsertPass1[InsertPass1[8]]=ans+48;
                  if (ans==11) InsertPass1[InsertPass1[8]]=ans+37;
                  InsertPass1[8]++;
                }
                
            }
            if ((ans==10) && (InsertPass1[8]>0) ) {InsertPass1[8]--; InsertPass1[InsertPass1[8]]=0;}
            
            
           char InsertPassVar=0;
           for (char i=0; i<=8; i++)  pass_tmp[i]=0;
           while ((InsertPass1[InsertPassVar]) &&  (InsertPassVar<8))
           {
             pass_tmp[InsertPassVar]=42; //42="*"
             InsertPassVar++;
           }

            
            //Sprawdzenie has³a
            if (ans==12)
            {
              roz=0;
              for (int i=0; i<=7; i++)
              {
                  if ((g_Password_1[i]!=InsertPass1[i]) && (level==1)) roz=1;
                  if ((g_Password_2[i]!=InsertPass1[i]) && (level==2)) roz=1;
              }
              //has³o programisty
              if (roz)
              {
                char MHS[]="060480";
                  for (int i=0; i<6; i++)
                  {
                      if ((MHS[i]!=InsertPass1[i]) && (level==1)) roz=1;
                      if ((MHS[i]!=InsertPass1[i]) && (level==2)) roz=1;
                  }              
              }
              
              
              if (roz==0)                                 //je¿eli poprawne haslo
              {
                 MenuLevel=11;
                 g_level=level;         //poziom dostêpu 1
                 for (int i=0; i<=7; i++) InsertPass1[i]=0;
                 InsertPass1[8]=0;
                 if (level==1)  Message_Paint(2,"","",Speech[g_leng].Poziom2,"","");
                 if (level==2)  Message_Paint(2,"","",Speech[g_leng].Poziom3,"","");
              } 
              else
              {
                 //informacja o b³ednym haœle
                for (int i=0; i<=7; i++) InsertPass1[i]=0;
                 InsertPass1[8]=0;
              
                 Message_Paint(2,"",Speech[g_leng].Bledne,Speech[g_leng].haslo,"","");
              }
              //MenuPaint();
            }
           
           
            
            //MenuPaint();
            //Label(pass_tmp,2,4,90,InsertPass1[8],55,0,127,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
  
            if ((ans>=1) && (ans<=11)) 
            {
              MenuPaint();
              Label(pass_tmp,2,4,90,InsertPass1[8],55,0,127,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
            }
            
          }  
          else if (ans==0) 
            {
              MenuPaint();
              Label(pass_tmp,2,4,90,InsertPass1[8],55,0,127,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
            }
        
         else if (ans<0)
          {
              MenuLevel=(MenuLevel/100);
              MenuPaint();
              for (int i=0; i<=7; i++) InsertPass1[i]=0;
              InsertPass1[8]=0;
          }
       
      }//_______________________________________________________________________


      
      
//Odrysuj procedurê **************************************************************************************************************
//*  x,y          - wspó³¿êdna lewego dolnego rogu boksu procedury

      void PaintProc(char x, char y, _Procedure *Proc)
      {   
          char PixPlab;
          char txt[7]={0,0,0,0,0,0,0};
          char ind;
          switch (Proc->Type)
          {
            
          case ProcNOT:
              CopyToCanvasPoint((char*)ProcNot_graf, x+1, y+3, 19,8);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
          break;  
          
          case ProcConect:
              H_line(x,y+6,21,1); 
              H_line(x,y+7,21,1); 
          break;  
            
          case ProcDown:
              H_line(x,y+6,11,1); 
              H_line(x,y+7,11,1);
              V_line(x+9,y,6,1);
              V_line(x+10,y,6,1); 
          break;  
          
          case ProcDownRight:
              H_line(x,y+6,21,1); 
              H_line(x,y+7,21,1);
              V_line(x+9,y,6,1);
              V_line(x+10,y,6,1); 
          break;  

          case ProcSumLogDown:
              H_line(x,y+6,9,1); 
              H_line(x,y+7,9,1);
              V_line(x+9,y,21,1);
              V_line(x+10,y,21,1); 
          break;  
          
          case ProcSumLogRight:
              H_line(x,y+6,21,1); 
              H_line(x,y+7,21,1);
              V_line(x+9,y+7,14,1);
              V_line(x+10,y+7,14,1); 
          break;  
          
          case ProcUpToRight:
              H_line(x+9 ,y+6,12,1); 
              H_line(x+9,y+7,12,1);
              V_line(x+9,y+7,15,1);
              V_line(x+10,y+7,15,1); 
          break;  

          
          case ProcUpToRightDown:
              H_line(x+9 ,y+6,12,1); 
              H_line(x+9,y+7,12,1);
              V_line(x+9,y,21,1);
              V_line(x+10,y,21,1); 
          break;  
          
          
          case ProcToggleRS:
              CopyToCanvasPoint((char*)ProcToggleRS_graf, x+1, y+2, 19,19);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
          break;   

          case ProcToggleT:
              CopyToCanvasPoint((char*)ProcToggleT_graf, x+1, y+2, 19,19);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
          break;             
          
          case ProcTLI1:
              CopyToCanvasPoint((char*)ProcTLI1_graf, x+1, y+2, 19,19);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
          break;      

          case ProcLim:
              CopyToCanvasPoint((char*)ProcLim_graf, x+1, y+3, 19,18);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
          break;  
          
          
          case ProcEven:
              CopyToCanvasPoint((char*)ProcEven_graf, x+1, y+3, 19,18);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
          break;  
          

          case ProcPlus:
              CopyToCanvasPoint((char*)ProcPlus_graf, x+1, y+3, 19,18);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
          break;  
          
          case ProcMinus:
              CopyToCanvasPoint((char*)ProcMinus_graf, x+1, y+3, 19,18);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
          break;  

          case ProcMul:
              CopyToCanvasPoint((char*)ProcMul_graf, x+1, y+3, 19,18);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
          break;  

          case ProcDiv:
              CopyToCanvasPoint((char*)ProcDiv_graf, x+1, y+3, 19,18);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
          break;  

          
          case ProcStI:
              CopyToCanvasPoint((char*)ProcSt_graf, x+1, y+3, 19,8);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
              
              if (Proc->Nr>=0)
              {
                ind=CharToStr(Proc->Nr, txt, 2); //(char Value, char *Vstring, char ill);
                txt[ind]='.';
                CharToStr(Proc->Tag.Port, &txt[ind+1], 2); //(char Value, char *Vstring, char ill);
              }else{
                txt[0]='b';
                CharToStr(Proc->Tag.Port, &txt[1], 2); //(char Value, char *Vstring, char ill);
              }     
              
              Label(txt,2,x+4,y+11,5,12,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
              txt[0]='I'; txt[1]=0;
              Label(txt,2,x+6,y+2,1,6,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          break;

          case ProcStNI:
              CopyToCanvasPoint((char*)ProcSt_graf, x+1, y+3, 19,8);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
              H_line(x+7,y+11,6,1);
              
              if (Proc->Nr>=0)
              {
                ind=CharToStr(Proc->Nr, txt, 2); //(char Value, char *Vstring, char ill);
                txt[ind]='.';
                CharToStr(Proc->Tag.Port, &txt[ind+1], 2); //(char Value, char *Vstring, char ill);
              }else{
                txt[0]='b';
                CharToStr(Proc->Tag.Port, &txt[1], 2); //(char Value, char *Vstring, char ill);
              }   
              Label(txt,2,x+4,y+12,5,12,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
              txt[0]='I'; txt[1]=0;
              Label(txt,2,x+6,y+2,1,6,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          break;
          
          
          case ProcStQ:
              CopyToCanvasPoint((char*)ProcSt_graf, x+1, y+3, 19,8);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
              if (Proc->Nr>=0)
              {
                ind=CharToStr(Proc->Nr, txt, 2); //(char Value, char *Vstring, char ill);
                txt[ind]='.';
                CharToStr(Proc->Tag.Port, &txt[ind+1], 2); //(char Value, char *Vstring, char ill);
              }else{
                txt[0]='b';
                CharToStr(Proc->Tag.Port, &txt[1], 2); //(char Value, char *Vstring, char ill);
              } 
              Label(txt,2,x+4,y+11,5,12,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
              txt[0]='Q'; txt[1]=0;
              Label(txt,2,x+7,y+2,1,6,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          break;

          case ProcStS:
              CopyToCanvasPoint((char*)ProcSt_graf, x+1, y+3, 19,8);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
              ind=CharToStr(Proc->Nr, txt, 2); //(char Value, char *Vstring, char ill);

              Label(txt,2,x+4,y+11,2,12,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
              txt[0]='S'; txt[1]=0;
              Label(txt,2,x+7,y+2,1,6,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          break;      
          
          
          case ProcStB:
              CopyToCanvasPoint((char*)ProcSt_Button, x+1, y+6, 19,6);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
              ind=CharToStr(Proc->Nr, txt, 2); //(char Value, char *Vstring, char ill);

              Label(txt,2,x+4,y+11,2,12,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          break;   
          
          

          
          case ProcWOUT:
              CopyToCanvasPoint((char*)ProcSet_graf, x+1, y+3, 19,9);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
              if (Proc->Nr>=0)
              {
                ind=CharToStr(Proc->Nr, txt, 2); //(char Value, char *Vstring, char ill);
                txt[ind]='.';
                CharToStr(Proc->Tag.Port, &txt[ind+1], 2); //(char Value, char *Vstring, char ill);
              }else{
                txt[0]='b';
                CharToStr(Proc->Tag.Port, &txt[1], 2); //(char Value, char *Vstring, char ill);
              } 
              Label(txt,2,x+4,y+11,5,12,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
              txt[0]='Q'; txt[1]=0;
              Label(txt,2,x+8,y+2,1,6,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          break;
          
          
          
          case ProcWOUTS:
              CopyToCanvasPoint((char*)ProcWOUTS_graf, x+1, y+3, 19,9);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
              if (Proc->Nr>=0)
              {
                ind=CharToStr(Proc->Nr, txt, 2); //(char Value, char *Vstring, char ill);
                txt[ind]='.';
                CharToStr(Proc->Tag.Port, &txt[ind+1], 2); //(char Value, char *Vstring, char ill);
              }else{
                txt[0]='b';
                CharToStr(Proc->Tag.Port, &txt[1], 2); //(char Value, char *Vstring, char ill);
              } 
              Label(txt,2,x+4,y+11,5,12,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
              //txt[0]='Q'; txt[1]=0;
              //Label(txt,2,x+8,y+2,1,6,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          break;
          
          case ProcWOUTR:
              CopyToCanvasPoint((char*)ProcWOUTR_graf, x+1, y+3, 19,9);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
              if (Proc->Nr>=0)
              {
                ind=CharToStr(Proc->Nr, txt, 2); //(char Value, char *Vstring, char ill);
                txt[ind]='.';
                CharToStr(Proc->Tag.Port, &txt[ind+1], 2); //(char Value, char *Vstring, char ill);
              }else{
                txt[0]='b';
                CharToStr(Proc->Tag.Port, &txt[1], 2); //(char Value, char *Vstring, char ill);
              } 
              Label(txt,2,x+4,y+11,5,12,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          break;  
          
          
          case ProcWOUTC:
              CopyToCanvasPoint((char*)ProcWOUTC_graf, x+1, y+3, 19,9);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
              if (Proc->Nr>=0)
              {
                ind=CharToStr(Proc->Nr, txt, 2); //(char Value, char *Vstring, char ill);
                txt[ind]='.';
                CharToStr(Proc->Tag.Port, &txt[ind+1], 2); //(char Value, char *Vstring, char ill);
              }else{
                txt[0]='b';
                CharToStr(Proc->Tag.Port, &txt[1], 2); //(char Value, char *Vstring, char ill);
              } 
              Label(txt,2,x+4,y+11,5,12,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          break; 
          
          case ProcSPK:
              CopyToCanvasPoint((char*)ProcSPK_graf, x+1, y+3, 19,9);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
              CopyToCanvasPoint((char*)SPK_1[Proc->Tag.SpeakerType], x+3,y+13, 16,5); 

          break;  


          
          
          case ProcSetScn:
              CopyToCanvasPoint((char*)ProcSet_graf, x+1, y+3, 19,9);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
             if (Proc->Nr==0)
              {
                txt[0]='0'; txt[1]='/'; txt[2]='1'; txt[3]=0; //(char Value, char *Vstring, char ill);
                Label(txt,2,x+3,y+11,3,12,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
              }else{
                ind=CharToStr(Proc->Nr, txt, 3); //(char Value, char *Vstring, char ill);
                Label(txt,2,x+4,y+11,2,12,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
              }
              
              txt[0]='S'; txt[1]=0;
              Label(txt,2,x+8,y+2,1,6,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          break;             
  
          case ProcInc:
              CopyToCanvasPoint((char*)ProcSet_graf, x+1, y+3, 19,9);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
              txt[0]='+'; txt[1]=0; txt[2]=0;
              Label(txt,2,x+7,y+4,1,9,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
              txt[0]='S';txt[1]='c';txt[2]='n';
              Label(txt,2,x+4,y+11,3,11,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
               
         break;               
  
          case ProcDec:
              CopyToCanvasPoint((char*)ProcSet_graf, x+1, y+3, 19,9);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
            
              txt[0]='-'; txt[1]='-'; txt[2]=0;
              Label(txt,2,x+7,y+4,2,6,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
              txt[0]='S';txt[1]='c';txt[2]='n';
              Label(txt,2,x+4,y+11,3,11,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
             
          break;    
          
          case ProcTimerSet:
              
              CopyToCanvasPoint((char*)ProcTime_graf, x+1, y+3, 19,8);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
            
              ind=FloToStr(Proc->Tag.SetCounter, txt, 5); //(char Value, char *Vstring, char ill);
              PixPlab=Label(txt,0,x+2,y+11,4,17,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
              if(PixPlab<14) Label("s",0,x+3+PixPlab,y+11,1,4,0,120,0);
          break;   
          

            
          case ProcClock:
            
              CopyToCanvasPoint((char*)ProcClock_graf, x+1, y+3, 19,8);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     

              Label((char*)ShortDate[Proc->Tag.ClkPrcType],0,x+2,y+11,1,17,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
              CharToStr(Proc->Tag.ClkPrcVal, txt, 2); //(char Value, char *Vstring, char ill);
              Label(txt,0,x+10,y+11,5,9,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
                      
            break;         
          

          case ProcDay:
            
              CopyToCanvasPoint((char*)ProcDay_graf, x+1, y+3, 19,8);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
              Label((char*)DeyOfWeekStr[g_leng][Proc->Tag.DayOfWeek],0,x+4,y+11,2,15,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
           
            break;         
          
          
          case ProcConstAnal:
            
              //CopyToCanvasPoint((char*)ProcConstAnal_graf, x+1, y+3, 19,18);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
              

              H_line(x+2 ,y+2,19,1);
              V_line(x+20 ,y+3,6,1);
              
              ind=sIntToStr(Proc->Tag.ConstAnValue, txt);
              
              if (ind>4)
              { 
                Label(txt,1,x+18,y+11,ind-4 ,4,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
                Label(&(txt[ind-4]),1,x+18,y+3,4 ,16,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
              }else{
                Label(txt,1,x+19,y+3,ind,16,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
               
              }
              //Label((char*)DeyOfWeekStr[g_leng][Proc->Tag.DayOfWeek],0,x+4,y+11,2,15,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
               
             
              
              
            break;   
            
           case ProcGetReg:
              CopyToCanvasPoint((char*)ProcGetAnal_graf, x+1, y+3, 19,17);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
              if (Proc->Nr>=0)
              {
                ind=CharToStr(Proc->Nr, txt, 2); //(char Value, char *Vstring, char ill);
                txt[ind]='.';
                CharToStr(Proc->Tag.RegNr, &txt[ind+1], 2); //(char Value, char *Vstring, char ill);
              }else{
                txt[0]='a';
                CharToStr(Proc->Tag.RegNr, &txt[1], 2); //(char Value, char *Vstring, char ill);
              }
              
              Label(txt,2,x+2, y+4,5,15,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 

            break;   
            
            case ProcSetReg:
              CopyToCanvasPoint((char*)ProcSetAnal_graf, x+1, y+3, 19,17);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
              if (Proc->Nr>=0)
              {
                ind=CharToStr(Proc->Nr, txt, 2); //(char Value, char *Vstring, char ill);
                txt[ind]='.';
                CharToStr(Proc->Tag.RegNr, &txt[ind+1], 2); //(char Value, char *Vstring, char ill);
              }else{
                txt[0]='a';
                CharToStr(Proc->Tag.RegNr, &txt[1], 2); //(char Value, char *Vstring, char ill);
              }
              Label(txt,2,x+3, y+4,5,15,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 

            break;   
            
            
          }
          
          
          
      }//_______________________________________________________________________
      
      
      

//lader **************************************************************************************************************
      char LaderLine=0;
      char LaderCol=0;
      //*----------------------------------------------------------------------------
      #define MaxRow 3
      #define MaxCol 3
      
      void Lader_Paint(void)
      {  
        
      char txt[5];
       
      
       List_Paint("Program ",1,108,0,0,3," ",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      

      //siatka
      
      for (char i=0; i<4; i++)
      {
          H_line(3,44+i*21,58,2);   
          if ((LaderCol==0) && (i<3))
          { 
             CharToStr(LaderLine+3-i, txt, 3); //(char Value, char *Vstring, char ill);
             Label(txt,0,3,51+i*21,4,23,16,105,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 

             H_line(17,50+i*21,5,1); 
             H_line(17,51+i*21,5,1); 
          }
      }
      for (char i=0; i<4; i++)
      {
          V_line(1+i*20,44,64,2);         
      }
      //------koniec siatki
      
      
      if (LaderCol==0)
      {
         V_line(16,44,64,1);
         V_line(17,44,64,1);  
         V_line(18,44,64,1); 
      }
      
      CharToStr(LaderCol, txt, 2); //(char Value, char *Vstring, char ill);
      Label(txt,0,8,35,8,23,16,105,0);
        
      
        //Label(Speech[g_leng].UruchomienieSter,0,2,j+(Label_move*2)-8,15,8,16,96,0); break;
         
            char LabelButt[2];
            LabelButt[1]=0;
            LabelButt[0]=30;
            Button(LabelButt,22,23,20,18); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
            LabelButt[0]=31;
            Button(LabelButt,22,3,20,18); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
            LabelButt[0]='>';
            Button(LabelButt,42,13,20,18); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
            LabelButt[0]='<';
            Button(LabelButt,2,13,20,18); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
           
        char row, col;


    
        row=LaderLine; 
        for (int j=(MaxRow-1); j>=0; j--)
        {
          
          col=LaderCol;
          for (int i=0; i<MaxCol; i++)
          {
            
            //if (LaderCol) PaintProc(21+j*20,44+i*21,&gProg.Line[row].Proc[col-1]);
            if ((LaderCol) || ((LaderCol==0)&&(i!=0)))  PaintProc(1+i*20,44+j*21,&gProg.Line[row].Proc[col-1]);
            col++;
          }
          row++;
        }
        
      }//________________________________________________________________________

    
      //*-----------------------------------------------------------------------------
      long long int Touch_EventTime;
      char ChoseRow, ChoseCol;
      void Lader_Event(void)
      {
       char LabelButt[2];
            LabelButt[1]=0;
            LabelButt[0]='>';
            if ((Button(LabelButt,42,13,20,18)) && ((g_counter>Touch_EventTime+200) || (Touch_EventTime>g_counter)) && (LaderCol<=MaxPrcInLine-MaxCol))
            {
              Touch_EventTime=g_counter;
              LaderCol++;    //(etykieta ,char poz_X , char poz_Y ,char width, char height)
              MenuPaint();
            }
            LabelButt[0]='<';
            if ((Button(LabelButt,2,13,20,18))  && ((g_counter>Touch_EventTime+200) || (Touch_EventTime>g_counter)) && (LaderCol>0))            
            {
              Touch_EventTime=g_counter;
              LaderCol--; //(etykieta ,char poz_X , char poz_Y ,char width, char height)
              MenuPaint();
            } 
            
            LabelButt[0]=30;
            if ((Button(LabelButt,22,23,20,18))  && ((g_counter>Touch_EventTime+50) || (Touch_EventTime>g_counter)) && (LaderLine>0))            
            {
              Touch_EventTime=g_counter;
              LaderLine--; //(etykieta ,char poz_X , char poz_Y ,char width, char height)
              MenuPaint();
            } 
            
            LabelButt[0]=31;
            if ((Button(LabelButt,22,3,20,18)) && ((g_counter>Touch_EventTime+50) || (Touch_EventTime>g_counter)) && (LaderLine<MaxPrcLines-MaxRow))
            {
              Touch_EventTime=g_counter;
              LaderLine++;    
              MenuPaint();
            }
            

        //zdarzenie typu down "X"
        if ((X_down>46) && (X_down<63) && (Y_down>112) && (Y_down<127))    //"X"
        {
          //MenuPaint();
          H_line(49,114,10,0);
          H_line(49,123,10,0); 
          V_line(49,114,10,0); 
          V_line(58,114,10,0); 
          if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}
          
        }
  
        

        
        //edycja procedury
        char row, col;
        
        row=LaderLine; 
        for (int j=(MaxRow-1); j>=0; j--)
        {          
          col=LaderCol;
          for (int i=0; i<MaxCol; i++)
          {
              if ((X_up>1+i*20) && (X_up<1+(i+1)*20) && (Y_up>44+j*21) && (Y_up<44+(j+1)*21))                    //"X"
              {
                if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}
                
                ChoseRow=row;
                ChoseCol=col-1;
                if (ChoseCol!=0xff)  //je¿eli nie zerowa kolumna
                {
                MenuLevel=MenuLevel*10+1;
                }else{
                MenuLevel=MenuLevel*10+2;  
                }
                
                MenuPaint();
               
              }  
            col++;
          }
          row++;
        }
        
         


    
        if ((X_up>46) && (X_up<63) && (Y_up>112) && (Y_up<127))                    //"X"
        {
          Rej_poz=0;
          if (ProgramChange) 
          {
           
            MenuLevel=(MenuLevel*10+0);
             
            
          }else{
            MenuLevel=(MenuLevel/10);
          }
          if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}
          MenuPaint();
        }  
  
     

      }//________________________________________________________________________
    
void WybProc(char col, char row)
{
   row=2-row;
   H_line(4+col*19,50+row*19,17,1);  
   H_line(3+col*19,50+17+row*19,17,1); 
   V_line(3+col*19,50+row*19,18,1); 
   V_line(3+17+col*19,50+row*19,18,1); 
}

char SetTimeCell=1;
void Procedure_Paint(void)
      { 
        
        
        List_Paint(Speech[g_leng].Procedura,1,108,0,0,3," ",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      
      
         char txt[5];
         //CharToStr(LaderLine+3-i, txt, 2); //(char Value, char *Vstring, char ill);

         
         for (char i=1; i<4; i++)
         {
          H_line(3,49+i*19,18,3);  
          H_line(22,49+i*19,18,3);  
          H_line(41,49+i*19,18,3);  
          }
          for (char i=0; i<4; i++)
          {
          V_line(2+i*19,69,18,3);  
          V_line(2+i*19,88,18,3);  
          }
         
         CopyToCanvasPoint((char*)ProcSt_iko, 24, 93,16,8);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
         CopyToCanvasPoint((char*)ProcSet_iko, 43, 93,16,8);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
         CopyToCanvasPoint((char*)ProcLog_iko, 5, 73,15,8);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
         CopyToCanvasPoint((char*)ProcTime_iko, 25, 72,13,12);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
         CopyToCanvasPoint((char*)ProcAnl_iko, 43, 73,15,8);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);     
         

                 
         
         //jaka procedura
         
         _Procedure Prc=(gProg.Line[ChoseRow].Proc[ChoseCol]);
         
         
            if  (Prc.Type==None)
            {
              WybProc(0,0);
            }

            
            if  ((Prc.Type>ProcLogStart) && (Prc.Type<ProcLogStop))
            { 
              WybProc(0,1);
              txt[1]=0;
              txt[0]='>';
              Button(txt,43,39,18,18);
              txt[0]='<';
              Button(txt,2,39,18,18);
              PaintProc(21, 40, &Prc);
            }
            
            
            
            if  (((Prc.Type>ProcStStart) && (Prc.Type<ProcStStop)) || 
                 ((Prc.Type>ProcSetStart) && (Prc.Type<ProcSetStop))||
                 ((Prc.Type>ProcTimeStart) && (Prc.Type<ProcTimeStop))||
                 ((Prc.Type>ProcAnalogStart) && (Prc.Type<ProcAnalogStop))
                 ) 
                
            { 
              txt[1]=0;
              txt[0]='>';
              if (Prc.Type>ProcStStart) Button(txt,43,46,18,18);
              
              
              if (
                 (Prc.Type==ProcConstAnal) ||
                 (Prc.Type==ProcLim) ||
                 (Prc.Type==ProcEven) || 
                 (Prc.Type==ProcPlus) || 
                 (Prc.Type==ProcMinus) || 
                 (Prc.Type==ProcMul) ||
                 (Prc.Type==ProcDiv)
                 )
              {
                PaintProc(20, 38, &Prc);
              }else if 
                (
                 (Prc.Type==ProcGetReg) || 
                 (Prc.Type==ProcSetReg)  
                )
                
              {  
                PaintProc(20, 41, &Prc);
                Label(" ",0,3,55,15,39,16,105,0);
              }else{
                PaintProc(20, 43, &Prc);
                Label(" ",0,3,55,15,39,16,105,0);
              }
              char Htmp=58;  //po³o¿enie napisu
              
              switch (Prc.Type)
              {
                case ProcStI: Label(Speech[g_leng].StanWejscia,0,3,Htmp,15,39,16,105,0); break;
                case ProcStNI: Label(Speech[g_leng].NegWejscia,0,3,Htmp,15,39,16,105,0); break;
                case ProcStQ: Label(Speech[g_leng].StanWyjscia,0,3,Htmp,15,39,16,105,0); break;
                case ProcStS: Label(Speech[g_leng].StSceny,0,3,Htmp,15,39,16,105,0); break;
               // case ProcStT: Label(Speech[g_leng].StanTimera,0,3,Htmp,15,39,16,105,0); break;
               // case ProcStNT: Label(Speech[g_leng].StanTimeraNeg,0,3,Htmp,15,39,16,105,0); break;
                case ProcWOUT: Label(Speech[g_leng].UstawWyjscie,0,2,Htmp,15,39,16,105,0); break;
                case ProcWOUTS: Label(Speech[g_leng].WlaczWyjscie,0,2,Htmp,15,39,16,105,0); break;
                case ProcWOUTR: Label(Speech[g_leng].ZerujWyjscie,0,2,Htmp,15,39,16,105,0); break;
                case ProcWOUTC: Label(Speech[g_leng].ZerujWyjscie,0,2,Htmp,15,39,16,105,0); break;
                case ProcSetScn: Label(Speech[g_leng].W³Scene,0,2,Htmp,15,39,16,105,0); break;
                case ProcInc: Label(Speech[g_leng].IncScene,0,2,Htmp,15,39,16,105,0); break;
                case ProcDec: Label(Speech[g_leng].DecScene,0,2,Htmp,15,39,16,105,0); break;
                case ProcSPK: Label(Speech[g_leng].Glosnik,0,2,Htmp,15,34,16,105,0); break;
                case ProcTimerSet: Label(Speech[g_leng].Timer,0,2,Htmp,15,39,16,105,0); break;
                case ProcClock: Label(Speech[g_leng].Zegar,0,2,Htmp,15,39,16,105,0); break;
                case ProcDay: Label(Speech[g_leng].DzienTygodnia,0,2,Htmp,15,39,16,105,0); break;
                case ProcConstAnal: Label("Const. ",0,2,Htmp,15,39,16,105,0); break;
                case ProcGetReg: Label("Get reg.",0,2,Htmp,15,39,16,105,0); break;
                case ProcSetReg: Label("Set reg.",0,2,Htmp,15,39,16,105,0); break;
                case ProcLim: Label("1>2 Q=1",0,2,Htmp,15,39,16,105,0); break;
                case ProcEven: Label("1=2 Q=1",0,2,Htmp,15,39,16,105,0); break;
                case ProcPlus: Label("Q=1+2",0,2,Htmp,15,39,16,105,0); break;
                case ProcMinus: Label("Q=1-2",0,2,Htmp,15,39,16,105,0); break;
                case ProcMul: Label("Q=1x2",0,2,Htmp,15,39,16,105,0); break;
                case ProcDiv: Label("Q=1/2",0,2,Htmp,15,39,16,105,0); break;

                             
              }
              
              
              
              if (
                 (Prc.Type!=ProcInc) && 
                 (Prc.Type!=ProcDec) &&
                 ((Prc.Type<ProcTimeStart)
                  || (Prc.Type==ProcGetReg)
                  || (Prc.Type==ProcSetReg)
                 )
                )
              {
                txt[0]='>';
                Button(txt,43,25,18,18);
                txt[0]='<';
                Button(txt,2,25,18,18);
               
                if (Prc.Type!=ProcSPK)
                {
                  if (Prc.Nr>=0)
                  {
                    txt[0]='N';txt[1]='r';txt[2]=':';
                    CharToStr(Prc.Nr, &txt[3], 2); //(char Value, char *Vstring, char ill);
                    
                  }else{
                    txt[0]='V';txt[1]='a';txt[2]='r';
                  }
                  Label(txt,2,21,29,5,20,0,105,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)        
                }else{
                  if(Prc.Tag.SpeakerType>SPK_Amount-1) Prc.Tag.SpeakerType=0;
                  CopyToCanvasPoint((char*)SPK_1[Prc.Tag.SpeakerType], 23,31, 16,5); 
                }
              }
              
              if ((Prc.Type==ProcStI) || (Prc.Type==ProcStNI) || (Prc.Type==ProcStQ) || (Prc.Type==ProcWOUT) || (Prc.Type==ProcWOUTS) || (Prc.Type==ProcWOUTR)|| (Prc.Type==ProcWOUTC))
              {
                txt[1]=0;
                txt[0]='>';
                Button(txt,43,4,18,18);
                txt[0]='<';
                Button(txt,2,4,18,18);
                if (Prc.Nr>=0)
                  {
                    txt[0]='P';txt[1]='o';txt[2]='r';txt[3]='t';txt[4]=':';
                    Label(txt,2,21,12,5,20,0,105,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
                    txt[1]=0;txt[2]=0;
                    CharToStr(Prc.Tag.Port, txt, 2); //(char Value, char *Vstring, char ill);
                    Label(txt,0,26,4,2,10,0,105,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)
                  }else{
                    txt[0]='b';txt[1]=0;txt[2]=0;
                    CharToStr(Prc.Tag.Port, txt+1, 2); //(char Value, char *Vstring, char ill);
                    Label(txt,2,21,12,3,20,0,105,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
                
                   /* txt[1]=0;txt[2]=0;
                CharToStr(Prc.Tag.Port, txt, 2); //(char Value, char *Vstring, char ill);
                Label(txt,0,26,4,2,10,0,105,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)
                    */
                  }
                 
              }
            }

          
            if ((Prc.Type>ProcStStart) && (Prc.Type<ProcStStop)) WybProc(1,0);
              
            
            
            if  ((Prc.Type>ProcSetStart) && (Prc.Type<ProcSetStop))
            { 
              WybProc(2,0);
            }

 

            if  ((Prc.Type>ProcTimeStart) && (Prc.Type<ProcTimeStop))
            { 
              WybProc(1,1);
            } 

            if  ((Prc.Type>ProcAnalogStart) && (Prc.Type<ProcAnalogStop))
            { 
              WybProc(2,1);
            } 
            
        if ((Prc.Type==ProcTimerSet))
        {
          Label(Speech[g_leng].IleSek,0,3,37,13,33,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
                    
          FloToStr(Prc.Tag.SetCounter , txt, 9); //(char Value, char *Vstring, char ill);
          Label(txt,0,12,26,9,44,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 

          H_line(10,36,46,1); // (char X, int Y, char lenght, char typ) 
          H_line(10,24,46,1); // (char X, int Y, char lenght, char typ) 
          V_line(10,25,11,1); // (char X, int Y, char lenght, char typ) 
          V_line(55,25,11,1); // (char X, int Y, char lenght, char typ) 
          
          
          
          txt[1]=0;
          txt[0]=30;
          Button(txt,33,3,19,19); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
          txt[0]=31;
          Button(txt,12,3,19,19); //(etykieta char poz_X , char poz_Y ,char width, char height) 
       
        }
        
        
        
        if (Prc.Type==ProcClock)
        {

       
       Label(   Speech[g_leng].Typ_,0,4,33,10,33,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
             
       Label(   Speech[g_leng].Miesiac+(Prc.Tag.ClkPrcType  *15),0,4,24,10,33,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
                    

       
       CharToStr(Prc.Tag.ClkPrcVal, txt, 2); //(char Value, char *Vstring, char ill);
       Label(txt,2,24,7,2,14,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
        
       
       //przycisk zmiany typu procedury zegara (godzina, minuta,...)
       Button("> ",43,24,18,19); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
       
       H_line(23,17,17,1); // (char X, int Y, char lenght, char typ) 
       H_line(23,6,17,1); // (char X, int Y, char lenght, char typ) 
       V_line(23,6,11,1); // (char X, int Y, char lenght, char typ) 
       V_line(39,6,11,1); // (char X, int Y, char lenght, char typ) 

       
      
       

       txt[1]=0;
       txt[0]=30;
       Button(txt,43,3,18,19); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
       txt[0]=31;
       Button(txt,2,3,18,19); //(etykieta char poz_X , char poz_Y ,char width, char height) 
       
              
            }   
      /*   
       if (Prc.Type==ProcStartTimer) 
       {
         Button("> ",2,46,18,11); //(etykieta char poz_X , char poz_Y ,char width, char height) 
       }
      */   
       
       if (Prc.Type==ProcDay) 
       {
                Button("> ",43,25,18,18);
                Button("< ",2,25,18,18);
                
                if(Prc.Tag.DayOfWeek==0) Prc.Tag.DayOfWeek=1;
                Label((char*)DeyOfWeekStr[g_leng][Prc.Tag.DayOfWeek],2,20,29,2,20,0,105,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)        
              
       }
       
       
       if (Prc.Type==ProcConstAnal) 
       {
                Button(Speech[g_leng].Zmiana,10,16,40,18);
       
       }
       
       if ((Prc.Type==ProcGetReg) || (Prc.Type==ProcSetReg) )
       {
       
           Button("> ",43,5,18,18);
           Button("< ",2, 5,18,18);
           Label("Reg:",0,22,15,4,14,1,127,0);
           txt[1]=0;
           char il=CharToStr(Prc.Tag.RegNr, txt, 2); //(char Value, char *Vstring, char ill);
           Label(txt,2,22,5,4,18,1,127,0);
       }
       
      }
      
char ProcGlobType_down;


void Procedure_Event(void)
      {
       char MenuPaintExecut=0;
       char ProcGlobType_up;
       char ind;
       char txt[2];
       char exit;
       
       
              exit=0;
       //zdarzenie typu down "X"
        if ((X_down>46) && (X_down<63) && (Y_down>112) && (Y_down<127))    //"X"
        {
          //MenuPaint();
          H_line(49,114,10,0);
          H_line(49,123,10,0); 
          V_line(49,114,10,0); 
          V_line(58,114,10,0); 
          exit=1;
          if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}
        }
  
    
        if ((X_up>46) && (X_up<63) && (Y_up>112) && (Y_up<127))                    //"X"
        {
          if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}
          exit=1;
          Rej_poz=0;

          CountLines(&gProg);
  
          
            MenuLevel=(MenuLevel/10);
            MenuPaintExecut=1;

        }  
       
if (exit==0)       
{
        
        ProgramChange=1;  
        ProcGlobType_up=0;
        ind=0;
        for (char j=2; j>=1; j--)
         for(char i=0; i<3; i++)
         {
            ind++;
            if ((X_up>2+i*19) && (X_up<2+17+i*19) && (Y_up>51+j*19) && (Y_up<50+17+j*19))    //"X"
            {
              ProcGlobType_up=ind;
              gProg.Line[ChoseRow].Proc[ChoseCol].Tag.Tag=0;
              gProg.Line[ChoseRow].Proc[ChoseCol].LastIn=0;
              if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}
              
            }
            
         }

    ProcGlobType_down=ProcGlobType_up;
      
    _Procedure *TypePrc=&(gProg.Line[ChoseRow].Proc[ChoseCol]);
         
        

            
           
       if ((ProcGlobType_up>0) && (ProcGlobType_down==ProcGlobType_up) && (exit==0))
       {
       ProcGlobType_down=0;
       gProg.Line[ChoseRow].Proc[ChoseCol].Out=0;
       gProg.Line[ChoseRow].Proc[ChoseCol].LastIn=0;
       gProg.Line[ChoseRow].Proc[ChoseCol].Tag.Tag=0;
       
       switch (ProcGlobType_up)
       {
         case 1:  //Pusty
           gProg.Line[ChoseRow].Proc[ChoseCol].Type=None;
         break;
         
         
         case 2: //Stan
           gProg.Line[ChoseRow].Proc[ChoseCol].Type=(_ProcType)(ProcStStart+1);
         break;
         
         
         case 3: //ustaw
           gProg.Line[ChoseRow].Proc[ChoseCol].Type=(_ProcType)(ProcSetStart+1);
         break;
         
         
         case 4: //Logika
           gProg.Line[ChoseRow].Proc[ChoseCol].Type=(_ProcType)(ProcLogStart+1);
         break;
         
         
         case 5: //timery, zegary
           gProg.Line[ChoseRow].Proc[ChoseCol].Type=(_ProcType)(ProcTimeStart+1);
         break;
         
         
         case 6: //timery, zegary
           gProg.Line[ChoseRow].Proc[ChoseCol].Type=(_ProcType)(ProcAnalogStart+1);
         break;
       }
       
      
         MenuPaintExecut=1; 
         
       }
         

       
       //Je¿eli logika
        if  ((TypePrc->Type>ProcLogStart) && (TypePrc->Type<ProcLogStop))
            {     
              txt[0]='>';
              txt[1]=0;
              if (Button(txt,43,39,18,18)==1) 
                if (TypePrc->Type+1<ProcLogStop) {TypePrc->Type++;MenuPaintExecut=1;  } else {TypePrc->Type=(_ProcType)(ProcLogStart+1);MenuPaintExecut=1;  }
              txt[0]='<';
              if (Button(txt,2,39,18,18)==1) 
                if (TypePrc->Type-1>ProcLogStart) {TypePrc->Type--;MenuPaintExecut=1;  } else {TypePrc->Type=(_ProcType)(ProcLogStop-1);MenuPaintExecut=1;  }
         
            }
 
       
       //zmiana numeru procedur
       if ((((TypePrc->Type>ProcStStart) && (TypePrc->Type<ProcTimeStart))
           || (TypePrc->Type==ProcGetReg) || (TypePrc->Type==ProcSetReg)   
           ) && (TypePrc->Type!=ProcInc) && (TypePrc->Type!=ProcInc))
       {  
          txt[1]=0;
          txt[0]='>';
          if (Button(txt,43,25,18,18)==1) 
          {
            switch (TypePrc->Type)
            {
              case ProcStI: if (TypePrc->Nr+1<NumberNeurons) {TypePrc->Nr++;} else {TypePrc->Nr=-1;} break;
              case ProcStNI: if (TypePrc->Nr+1<NumberNeurons) {TypePrc->Nr++;} else {TypePrc->Nr=-1;} break;
              case ProcStQ: if (TypePrc->Nr+1<NumberNeurons) {TypePrc->Nr++;} else {TypePrc->Nr=-1;} break;
              case ProcStB: if (TypePrc->Nr+1<NumberButton) {TypePrc->Nr++;} else {TypePrc->Nr=0;} break;
              case ProcStS: if (TypePrc->Nr+1<NumberSceneMax) {TypePrc->Nr++;} else {TypePrc->Nr=0;} break;
              case ProcWOUT: if (TypePrc->Nr+1<NumberNeurons) {TypePrc->Nr++;} else {TypePrc->Nr=-1;} break;
              case ProcWOUTS: if (TypePrc->Nr+1<NumberNeurons) {TypePrc->Nr++;} else {TypePrc->Nr=-1;} break;
              case ProcWOUTR: if (TypePrc->Nr+1<NumberNeurons) {TypePrc->Nr++;} else {TypePrc->Nr=-1;} break;
              case ProcWOUTC: if (TypePrc->Nr+1<NumberNeurons) {TypePrc->Nr++;} else {TypePrc->Nr=-1;} break;
              case ProcSetScn: if (TypePrc->Nr+1<NumberSceneMax) {TypePrc->Nr++;} else {TypePrc->Nr=0;} break;
              case ProcGetReg: if (TypePrc->Nr+1<NumberNeurons) {TypePrc->Nr++;} else {TypePrc->Nr=-1;} break;
              case ProcSetReg: if (TypePrc->Nr+1<NumberNeurons) {TypePrc->Nr++;} else {TypePrc->Nr=-1;} break;
              case ProcSPK: if (TypePrc->Tag.SpeakerType+1<SPK_Amount) {TypePrc->Tag.SpeakerType++;} else {TypePrc->Tag.SpeakerType=0;} break;
              

            }
            
            
            
            MenuPaintExecut=1; 
          }
          txt[0]='<';
          if (Button(txt,2,25,18,18)==1) 
          {
            
            switch (TypePrc->Type)
            {
              case ProcStI: if (TypePrc->Nr>=0) {TypePrc->Nr--;} else {TypePrc->Nr=NumberNeurons-1;} break;
              case ProcStNI: if (TypePrc->Nr>=0) {TypePrc->Nr--;} else {TypePrc->Nr=NumberNeurons-1;} break;
              case ProcStQ: if (TypePrc->Nr>=0) {TypePrc->Nr--;} else {TypePrc->Nr=NumberNeurons-1;} break;
              case ProcStS: if (TypePrc->Nr>0) {TypePrc->Nr--;} else {TypePrc->Nr=NumberSceneMax-1;} break;
              case ProcStB: if (TypePrc->Nr>0) {TypePrc->Nr--;} else {TypePrc->Nr=NumberNeurons-1;} break;
              case ProcWOUT: if (TypePrc->Nr>=0) {TypePrc->Nr--;} else {TypePrc->Nr=NumberNeurons-1;} break;
              case ProcWOUTS: if (TypePrc->Nr>=0) {TypePrc->Nr--;} else {TypePrc->Nr=NumberNeurons-1;} break;
              case ProcWOUTR: if (TypePrc->Nr>=0) {TypePrc->Nr--;} else {TypePrc->Nr=NumberNeurons-1;} break;
              case ProcWOUTC: if (TypePrc->Nr>=0) {TypePrc->Nr--;} else {TypePrc->Nr=NumberNeurons-1;} break;
              case ProcSetScn: if (TypePrc->Nr>0) {TypePrc->Nr--;} else {TypePrc->Nr=NumberSceneMax-1;} break;
              case ProcGetReg: if (TypePrc->Nr>=0) {TypePrc->Nr--;} else {TypePrc->Nr=NumberNeurons-1;} break;
              case ProcSetReg: if (TypePrc->Nr>=0) {TypePrc->Nr--;} else {TypePrc->Nr=NumberNeurons-1;} break;
              case ProcSPK: if (TypePrc->Tag.SpeakerType>0) {TypePrc->Tag.SpeakerType--;} else {TypePrc->Tag.SpeakerType=SPK_Amount-1;} break;
            }
            

            MenuPaintExecut=1; 
          }
       }
       

       //zmiana numeru portu
       if(((TypePrc->Type==ProcStI) ||
          (TypePrc->Type==ProcStNI) ||
          (TypePrc->Type==ProcStQ) ||
          (TypePrc->Type==ProcWOUT) ||
          (TypePrc->Type==ProcWOUTS) ||
          (TypePrc->Type==ProcWOUTR) ||
          (TypePrc->Type==ProcWOUTC)  
            )
         && (TypePrc->Type<ProcTimeStart))
       {  
          txt[1]=0;
          txt[0]='>';
          if (Button(txt,43,4,18,18)==1) 
          {
            switch (TypePrc->Type)
            {
              case ProcStI: if ((TypePrc->Tag.Port+1<NumberNeuronsPort)   || ((TypePrc->Nr<0) &&(TypePrc->Tag.Port+1<99))) {TypePrc->Tag.Port++;} else {TypePrc->Tag.Port=0;} break;
              case ProcStNI: if ((TypePrc->Tag.Port+1<NumberNeuronsPort)  || ((TypePrc->Nr<0) &&(TypePrc->Tag.Port+1<99))) {TypePrc->Tag.Port++;} else {TypePrc->Tag.Port=0;} break;
              case ProcStQ: if ((TypePrc->Tag.Port+1<NumberNeuronsPort)   || ((TypePrc->Nr<0) &&(TypePrc->Tag.Port+1<99))) {TypePrc->Tag.Port++;} else {TypePrc->Tag.Port=0;} break;
              case ProcWOUT: if ((TypePrc->Tag.Port+1<NumberNeuronsPort)  || ((TypePrc->Nr<0) &&(TypePrc->Tag.Port+1<99))) {TypePrc->Tag.Port++;} else {TypePrc->Tag.Port=0;} break;
              case ProcWOUTS: if ((TypePrc->Tag.Port+1<NumberNeuronsPort) || ((TypePrc->Nr<0) &&(TypePrc->Tag.Port+1<99))) {TypePrc->Tag.Port++;} else {TypePrc->Tag.Port=0;} break;
              case ProcWOUTR: if ((TypePrc->Tag.Port+1<NumberNeuronsPort) || ((TypePrc->Nr<0) &&(TypePrc->Tag.Port+1<99))) {TypePrc->Tag.Port++;} else {TypePrc->Tag.Port=0;} break;
              case ProcWOUTC: if ((TypePrc->Tag.Port+1<NumberNeuronsPort) || ((TypePrc->Nr<0) &&(TypePrc->Tag.Port+1<99))) {TypePrc->Tag.Port++;} else {TypePrc->Tag.Port=0;} break;
            }
            MenuPaintExecut=1; 
          }
          txt[0]='<';
          if (Button(txt,2,4,18,18)==1) 
          {
            switch (TypePrc->Type)
            {
              case ProcStI: if (TypePrc->Tag.Port>0)  {TypePrc->Tag.Port--;} else {if(TypePrc->Nr>0) TypePrc->Tag.Port=NumberNeuronsPort-1; else TypePrc->Tag.Port=99; } break;
              case ProcStNI: if (TypePrc->Tag.Port>0) {TypePrc->Tag.Port--;} else {if(TypePrc->Nr>0)  TypePrc->Tag.Port=NumberNeuronsPort-1; else TypePrc->Tag.Port=99; } break;
              case ProcStQ: if (TypePrc->Tag.Port>0) {TypePrc->Tag.Port--;} else {if(TypePrc->Nr>0) TypePrc->Tag.Port=NumberNeuronsPort-1; else TypePrc->Tag.Port=99; } break;
              case ProcWOUT: if (TypePrc->Tag.Port>0) {TypePrc->Tag.Port--;} else {if(TypePrc->Nr>0) TypePrc->Tag.Port=NumberNeuronsPort-1; else TypePrc->Tag.Port=99; } break;
              case ProcWOUTS: if (TypePrc->Tag.Port>0) {TypePrc->Tag.Port--;} else {if(TypePrc->Nr>0) TypePrc->Tag.Port=NumberNeuronsPort-1; else TypePrc->Tag.Port=99; } break;
              case ProcWOUTR: if (TypePrc->Tag.Port>0) {TypePrc->Tag.Port--;} else {if(TypePrc->Nr>0) TypePrc->Tag.Port=NumberNeuronsPort-1; else TypePrc->Tag.Port=99; } break;
              case ProcWOUTC: if (TypePrc->Tag.Port>0) {TypePrc->Tag.Port--;} else {if(TypePrc->Nr>0) TypePrc->Tag.Port=NumberNeuronsPort-1; else TypePrc->Tag.Port=99; } break;
            }
            MenuPaintExecut=1; 
          }
       }
       
       //Zmiana typu procedury w ramach grupy
       txt[1]=0;
       txt[0]='>';
       if ((TypePrc->Type>ProcStStart) )
       if (Button(txt,43,46,18,18)==1)
       {
         //je¿eli stany
         if  ((TypePrc->Type>ProcStStart) && (TypePrc->Type<ProcStStop))
            { 
              if (TypePrc->Type+1<ProcStStop) 
              {
                TypePrc->Type++;MenuPaintExecut=1;  
                if (TypePrc->Type>ProcStQ) {TypePrc->Nr=0; TypePrc->Tag.Port=0;}
              } else {TypePrc->Type=(_ProcType)(ProcStStart+1);MenuPaintExecut=1;  }
            }
         if  ((TypePrc->Type>ProcSetStart) && (TypePrc->Type<ProcSetStop))
            { 
              if (TypePrc->Type+1<ProcSetStop) 
              {
                TypePrc->Type++;MenuPaintExecut=1;  
                if (TypePrc->Type>ProcWOUTC) {TypePrc->Nr=0; TypePrc->Tag.Port=0;}
              } else {TypePrc->Type=(_ProcType)(ProcSetStart+1);MenuPaintExecut=1;  }
            }
         
         
         if  ((TypePrc->Type>ProcTimeStart) && (TypePrc->Type<ProcTimeStop))
         {  
            if (TypePrc->Type+1<ProcTimeStop) 
            {
              TypePrc->Type++;MenuPaintExecut=1;  
            } else {
              TypePrc->Type=(_ProcType)(ProcTimeStart+1);MenuPaintExecut=1;  
            }
            TypePrc->Tag.CounterStartTime=0;

         }
         
         if  ((TypePrc->Type>ProcAnalogStart) && (TypePrc->Type<ProcAnalogStop))
            { 
              if (TypePrc->Type+1<ProcAnalogStop) 
              {TypePrc->Type++;MenuPaintExecut=1;  } else {TypePrc->Type=ProcAnalogStart+1;MenuPaintExecut=1;  }
            }
         
         
         
         if ((TypePrc->Type==ProcGetReg) || (TypePrc->Type==ProcSetReg) )
              {
                TypePrc->Tag.RegNr=1;
              }
         
         
       }
       
       
       /*
       if (TypePrc->Type==ProcStartTimer)
       if (Button("> ",2,46,18,11)==1)
       {
         if (TypePrc->Nr+1<NumberTimer) {TypePrc->Nr++;} else {TypePrc->Nr=0;}   
         
         MenuPaintExecut=1; 
       }
       */
       
       if (TypePrc->Type==ProcClock)
       {

          txt[1]=0;
          txt[0]=30;
          if (Button(txt,43,3,18,19)==1) //(etykieta char poz_X , char poz_Y ,char width, char height) 
          {
          
            if ((TypePrc->Tag.ClkPrcType==PrcMounth) && (TypePrc->Tag.ClkPrcVal<12)) 
                {TypePrc->Tag.ClkPrcVal++;} 
            if ((TypePrc->Tag.ClkPrcType==PrcDay) &&(TypePrc->Tag.ClkPrcVal<31)) 
                {TypePrc->Tag.ClkPrcVal++;}
            if ((TypePrc->Tag.ClkPrcType==PrcHour) &&(TypePrc->Tag.ClkPrcVal<23)) 
               {TypePrc->Tag.ClkPrcVal++;} 
            if ((TypePrc->Tag.ClkPrcType==PrcMinute) &&(TypePrc->Tag.ClkPrcVal<59)) 
                {TypePrc->Tag.ClkPrcVal++;} 
            if ((TypePrc->Tag.ClkPrcType==PrcSecond) &&(TypePrc->Tag.ClkPrcVal<59)) 
                {TypePrc->Tag.ClkPrcVal++;}
          
             
             
             
             MenuPaintExecut=1;  
          }
    
          
          txt[0]=31;
          if  (Button(txt,2,3,18,19)==1)//(etykieta ,char poz_X , char poz_Y ,char width, char height)
          { 
            if (TypePrc->Tag.ClkPrcType==PrcMounth) if (TypePrc->Tag.ClkPrcVal>1)
                {TypePrc->Tag.ClkPrcVal--; } else {TypePrc->Tag.ClkPrcVal=12;}
            if (TypePrc->Tag.ClkPrcType==PrcDay) if(TypePrc->Tag.ClkPrcVal>1) 
                {TypePrc->Tag.ClkPrcVal--;} else {TypePrc->Tag.ClkPrcVal=31;}
            if (TypePrc->Tag.ClkPrcType==PrcHour) if(TypePrc->Tag.ClkPrcVal>0)
                {TypePrc->Tag.ClkPrcVal--;} else {TypePrc->Tag.ClkPrcVal=23;}
            if (TypePrc->Tag.ClkPrcType==PrcMinute) if(TypePrc->Tag.ClkPrcVal>0) 
                {TypePrc->Tag.ClkPrcVal--;} else {TypePrc->Tag.ClkPrcVal=59;}
            if (TypePrc->Tag.ClkPrcType==PrcSecond) if(TypePrc->Tag.ClkPrcVal>0) 
                {TypePrc->Tag.ClkPrcVal--;} else {TypePrc->Tag.ClkPrcVal=59;}
              
             MenuPaintExecut=1;  
            
          }
     
          
         //przycisk zmiany typu procedury zegara (godzina, minuta,...)
         if (Button("> ",43,24,18,19)==1) //(etykieta ,char poz_X , char poz_Y ,char width, char height)
         {
           if (TypePrc->Tag.ClkPrcType<PrcSecond) 
           {
             TypePrc->Tag.ClkPrcType++;
           }else{
             TypePrc->Tag.ClkPrcType=PrcMounth;
           }

             TypePrc->Tag.ClkPrcVal=0; 
             if ((TypePrc->Tag.ClkPrcType==PrcMounth) || (TypePrc->Tag.ClkPrcType==PrcDay)) TypePrc->Tag.ClkPrcVal=1; 


           
           MenuPaintExecut=1; 
           
         }
          
       }
     
       if (TypePrc->Type==ProcTimerSet)
       {  
         
          txt[1]=0;
          txt[0]=30;
          if ((Button(txt,33,3,19,19)) &&  ((g_counter>Touch_EventTime+200) || (Touch_EventTime>g_counter)) )//(etykieta ,char poz_X , char poz_Y ,char width, char height)
          {
            Touch_EventTime=g_counter;
            if (TypePrc->Tag.SetCounter<10000)  
            { 
             TypePrc->Tag.SetCounter++;
             
            }else {
             TypePrc->Tag.SetCounter=0; 
            }
            MenuPaintExecut=1; 
          }
          
          txt[0]=31;
          if ((Button(txt,12,3,19,19)) && ((g_counter>Touch_EventTime+200) || (Touch_EventTime>g_counter))) //(etykieta char poz_X , char poz_Y ,char width, char height) 
          {
            Touch_EventTime=g_counter;
            if (TypePrc->Tag.SetCounter>0) 
            {
             TypePrc->Tag.SetCounter--;
             
            }else{
              TypePrc->Tag.SetCounter=100000;
            }
            MenuPaintExecut=1; 
          }
       }
       
              //zmiana numeru procedur Dnia
       if (TypePrc->Type==ProcDay)
       {  

          
          if (Button("> ",43,25,18,18)==1) 
          {
  
            if (TypePrc->Tag.DayOfWeek<Sunday) {TypePrc->Tag.DayOfWeek++;} else {TypePrc->Tag.DayOfWeek=Monday;}
            MenuPaintExecut=1; 
          }
    
          if (Button("< ",2,25,18,18)==1) 
          {
            
              if (TypePrc->Tag.DayOfWeek>Monday) {TypePrc->Tag.DayOfWeek--;} else {TypePrc->Tag.DayOfWeek=Sunday;}

            MenuPaintExecut=1; 
          }
       }
       
       
        if (TypePrc->Type==ProcConstAnal) 
       {
               if  (Button(Speech[g_leng].Zmiana,10,16,40,18)==1)
               {
                 ValueChange= &(TypePrc->Tag.ConstAnValue);
                 MenuLevel=(MenuLevel*10+1);
                 MenuPaintExecut=1;
               }
       }
       
       
        if ((TypePrc->Type==ProcGetReg) || (TypePrc->Type==ProcSetReg)    )
       {
       
            if (Button("> ",43,5,18,18)==1) 
            {
              if (TypePrc->Nr<0)
              {
                if (TypePrc->Tag.RegNr<AnalogVarAmount-1) TypePrc->Tag.RegNr++;  
              }else{
                if (TypePrc->Tag.RegNr<NumberReg-1) TypePrc->Tag.RegNr++;
              }
              MenuPaintExecut=1;
            }
            
            if (Button("< ",2,5,18,18)==1)
            {
              if (TypePrc->Nr<0)
              {
                if (TypePrc->Tag.RegNr>0) TypePrc->Tag.RegNr--;
              }else{
                if (TypePrc->Tag.RegNr>1) TypePrc->Tag.RegNr--;
              }
              MenuPaintExecut=1;
            }
       }
       
}
       
  
       if ((X_up>0) && (Y_up>0))   exit=0;                 
        
       
       if (MenuPaintExecut) MenuPaint();

      }//________________________________________________________________________
         


  
char ValueChangeTxtTmp[8]; 
char VCTT_ill=0;   //iloœæ znaków napisu ValueChangeTxtTmp
char VCTT_point_flag=0;  //pozycja kropki

void ValueChange_Paint( short int *ValToChange)
      { 
        List_Paint(Speech[g_leng].Wartosc,1,108,0,0,3," ",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      
        
   
        if (VCTT_ill==0) for (char i=0; i<7; i++) ValueChangeTxtTmp[i]=0;
        
        H_line(11,106,40,1);
        H_line(11,96,40,1);
        V_line(10,97,9,1); 
        V_line(51,97,9,1); 

        
        //char txt[7];
        
       
        //tmpVal=*ValueChange;
        //char dl=sIntToStr(tmpVal, txt);
        
        Label(ValueChangeTxtTmp,1,50,97,7,38,0,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)        
                      

        
        char txt[2];
        txt[1]=0;
        char ind=0;
        for (char j=0; j<3; j++)
          for (char i=0; i<3; i++)
          {
            ind++;
            CharToStr(ind, txt, 1); //(char Value, char *Vstring, char ill);
            Button(txt,2+20*i,76-(19*j),18,17);
          }
        Button(".",2+20*0,76-(19*3),18,17);
        Button("0",2+20*1,76-(19*3),18,17);
        Button("-",2+20*2,76-(19*3),18,17);
        
        Button("Ok ",22,4,38,13);
        Button("C ",3,4,17,13);
      }



void ValueChange_Evant( short int *ValToChange)
      { 
       int ValueTmp=0; 
       char ind=0;
       char txt[2];
       char Refresh=0;
       char MaxChar;
       

       txt[1]=0;
        if ((X_up>46) && (X_up<63) && (Y_up>112) && (Y_up<127))                    //"X"
        {
             for (char i=0; i<7; i++) ValueChangeTxtTmp[i]=0;
                ValueTmp=0;
                VCTT_ill=0;
                VCTT_point_flag=0;
          
            MenuLevel=(MenuLevel/10);
            Refresh=1;
        } else {
          
                 
          if (ValueChangeTxtTmp[0]=='-')
          {
            if  (VCTT_point_flag) {MaxChar=7;} else {MaxChar=5;}
          }else{
            if  (VCTT_point_flag) {MaxChar=6;} else {MaxChar=4;}
          }
         
             
          for (char j=0; j<3; j++)
          {
          for (char i=0; i<3; i++)
          {
            ind++;
            CharToStr(ind, txt, 1); //(char Value, char *Vstring, char ill);
            if ((Button(txt,2+20*i,76-(19*j),18,17)==1) && (VCTT_ill<MaxChar) && ((VCTT_point_flag==0) || (VCTT_ill<=VCTT_point_flag+1) ) ){ValueChangeTxtTmp[VCTT_ill++]=48+ind;   Refresh=1;}
            
           }
          }
            
            
            if ((Button(".",2+20*0,76-(19*3),18,17)==1) && (VCTT_ill<MaxChar+2) && (VCTT_point_flag==0)) 
            {ValueChangeTxtTmp[VCTT_ill++]='.';   Refresh=1; VCTT_point_flag=VCTT_ill-1;
              if (ValueChangeTxtTmp[0]=='-')
               {
                if  (VCTT_point_flag) {MaxChar=7;} else {MaxChar=5;}
                }else{
                if  (VCTT_point_flag) {MaxChar=6;} else {MaxChar=4;}
                }
            }
            
         
            if ((Button("0",2+20*1,76-(19*3),18,17)==1) && (VCTT_ill<MaxChar) && ((VCTT_point_flag==0) || (VCTT_ill<=VCTT_point_flag+1))) {ValueChangeTxtTmp[VCTT_ill++]=48;    Refresh=1;}
            if ((Button("-",2+20*2,76-(19*3),18,17)==1) && (VCTT_ill==0))
            {
              ValueChangeTxtTmp[VCTT_ill++]='-';    
              if  (VCTT_point_flag) {MaxChar=7;} else {MaxChar=5;}
              Refresh=1;
            }
            
            
        
            
            if (Button("C ",3,4,17,13)==1)
            {
              if (VCTT_ill) VCTT_ill--;
              if (ValueChangeTxtTmp[VCTT_ill]=='.') VCTT_point_flag=0;
              ValueChangeTxtTmp[VCTT_ill]=0;
              
              if (ValueChangeTxtTmp[0]=='-')
              {
                if  (VCTT_point_flag) {MaxChar=7;} else {MaxChar=5;}
              }else{
                if  (VCTT_point_flag) {MaxChar=6;} else {MaxChar=4;}
              }
              Refresh=1;
            }
          
        
            int mnz=10;     
            char prs=0;
            if (ValueChangeTxtTmp[0]=='-')  prs=1; 
            //zapamiêtanie wartoœci
            if (Button("Ok ",22,4,38,13)==1)
            {
              //je¿eli nie ma kropki 
              if (VCTT_point_flag==0)
              {
                for (char i=0; i<VCTT_ill-prs; i++)
                {
                   ValueTmp+=(ValueChangeTxtTmp[VCTT_ill-i-1]-48)*mnz;
                   mnz*=10;
                }
                
                
              }else{
                ValueTmp+=ValueChangeTxtTmp[VCTT_ill-1]-48;
                
                for (char i=1; i<=VCTT_point_flag-prs; i++)
                {
                   ValueTmp+=(ValueChangeTxtTmp[VCTT_point_flag-i]-48)*mnz;
                   mnz*=10;
                }
                
              }
              if (ValueTmp>30000) 
              {
                //ValueTmp=30000;
                MenuLevel=MenuLevel*10+1;
              }else{
                MenuLevel/=10; 
                if (ValueChangeTxtTmp[0]=='-')  ValueTmp*=-1; 
                *ValToChange=ValueTmp;
                for (char i=0; i<7; i++) ValueChangeTxtTmp[i]=0;
                ValueTmp=0;
                VCTT_ill=0;
                VCTT_point_flag=0;
                
              }
              
              
              Refresh=1;
              
            }
          
          
        }
        
        
        if (Refresh==1) MenuPaint();
        
}
      

//*-----------------------------------------------------------------------
char *ptxt;
void LaderChange_Paint(void)
      { 
       char y=20;   //przesuniêcie w dó³

       
        List_Paint(Speech[g_leng].Opcje,2,15,y,0,3,Speech[g_leng].DodajWiersz, Speech[g_leng].UsunWiersz, 0,0,0,0,0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])


      }



void LaderChange_Event(void)
      { 
        char y=20;   //przesuniêcie w dó³
        int ans;
        char Repaint=0;

        
        ans=List_Event(X_up, Y_up, X_down, Y_down, 2 , 15, 0,y); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        if (ans>0)
          {
            if ((ans>0) && (ans<3))
            {
              if (ans==1) 
              {
                MenuLevel=(MenuLevel/10);
                AddRow();
                
              }else{
                MenuLevel=(MenuLevel*10+ans);
              }
              Repaint=1;
            }
            
          }
        else if (ans<0)
          {
            
            if ((X_up>0) &&(Y_up>0)) 
            {
              MenuLevel=(MenuLevel/10);
              Repaint=1;
              
            }
            
          }
        if (Repaint) MenuPaint();
}



int row_tmp;
void AddRow(void)
      { 
    
        while (Busy_program) {}
 
      Busy_program=1;
          //for ( row_tmp=5; row_tmp>ChoseRow; row_tmp--)
          for ( row_tmp=MaxPrcLines-1; row_tmp>ChoseRow; row_tmp--)
          {

            for (char col_tmp=0; col_tmp<MaxPrcInLine; col_tmp++)
            {
           
              gProg.Line[row_tmp].Proc[col_tmp].Type=gProg.Line[row_tmp-1].Proc[col_tmp].Type;
              gProg.Line[row_tmp].Proc[col_tmp].Nr=gProg.Line[row_tmp-1].Proc[col_tmp].Nr;
              gProg.Line[row_tmp].Proc[col_tmp].Tag=gProg.Line[row_tmp-1].Proc[col_tmp].Tag;
              gProg.Line[row_tmp].Proc[col_tmp].LastIn=gProg.Line[row_tmp-1].Proc[col_tmp].LastIn;
              gProg.Line[row_tmp].Proc[col_tmp].Out=gProg.Line[row_tmp-1].Proc[col_tmp].Out;

            }
          }
        
           for (int col_tmp=0; col_tmp<MaxPrcInLine; col_tmp++)
            {
              gProg.Line[ChoseRow].Proc[col_tmp].Type=None;
              gProg.Line[ChoseRow].Proc[col_tmp].Nr=0;
              gProg.Line[ChoseRow].Proc[col_tmp].Tag.Tag=0;
              gProg.Line[ChoseRow].Proc[col_tmp].LastIn=0;
              gProg.Line[ChoseRow].Proc[col_tmp].Out=0;

            }
        
          ProgramChange=1;

        Busy_program=0;
  
}
    


//procedura usuniêcia lini
void RemRow(void)
{
          for (int row_tmp=ChoseRow; row_tmp<MaxPrcLines-1; row_tmp++)
          {
            for (char col_tmp=0; col_tmp<MaxPrcInLine; col_tmp++)
            {
           
              gProg.Line[row_tmp].Proc[col_tmp].Type=gProg.Line[row_tmp+1].Proc[col_tmp].Type;
              gProg.Line[row_tmp].Proc[col_tmp].Nr=gProg.Line[row_tmp+1].Proc[col_tmp].Nr;
              gProg.Line[row_tmp].Proc[col_tmp].Tag=gProg.Line[row_tmp+1].Proc[col_tmp].Tag;
              gProg.Line[row_tmp].Proc[col_tmp].LastIn=gProg.Line[row_tmp+1].Proc[col_tmp].LastIn;
              gProg.Line[row_tmp].Proc[col_tmp].Out=gProg.Line[row_tmp+1].Proc[col_tmp].Out;
            }
          }
          
           for (int col_tmp=0; col_tmp<MaxPrcInLine; col_tmp++)
            {
              gProg.Line[MaxPrcLines-1].Proc[col_tmp].Type=None;
              gProg.Line[MaxPrcLines-1].Proc[col_tmp].Nr=0;
              gProg.Line[MaxPrcLines-1].Proc[col_tmp].Tag.Tag=0;
              gProg.Line[MaxPrcLines-1].Proc[col_tmp].LastIn=0;
              gProg.Line[MaxPrcLines-1].Proc[col_tmp].Out=0;

            }
  
}



void RemRow_Paint(void)
      { 

        //czy s¹ procedury na lini do usuniêia
        char ProcExst=0;
        for (int col_tmp=0; col_tmp<MaxPrcInLine; col_tmp++)
            {
              if (gProg.Line[ChoseRow].Proc[col_tmp].Type!=None) ProcExst=1;
              
            }
        
        //je¿eli s¹ procedury w lini
        if (ProcExst)
        {
          Ask_Paint("?",Speech[g_leng].UsunWiersz,Speech[g_leng].Zprocedurami);  

        }else{  //je¿eli linia pusta
          RemRow();
          MenuLevel=(MenuLevel/100);
          MenuPaint();
        }
        
      }



//restart
      //*-----------------------------------------------------------------------
      void RemRow_Event(void)
      {
        int ans;
        ans=Ask_Event(X_up, Y_up, X_down, Y_down); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        if (ans)
          {
          if (ans==2) 
          {
            RemRow();
            MenuLevel=(MenuLevel/100);
            ProgramChange=1;
          }
          if ((ans<0) || (ans==1))  MenuLevel=(MenuLevel/100);
          MenuPaint();
          } 
      }//_______________________________________________________________________
    
  


//SaveChanges_Evant****************************************************************
void SaveChanges_Evant(void)
{
          int ans;
        ans=Ask_Event(X_up, Y_up, X_down, Y_down); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        if (ans)
          {
            if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}
          if (ans==2) 
          {
             ProgramChange=0;
              
            ProgramChangeExecute(&gProg); 
            RamToFlash();
            
            MenuLevel=(MenuLevel/100);
          }

            ProgramChangeExecute(&gProg);
         
          if ((ans<0) || (ans==1))  MenuLevel=(MenuLevel/100);
          MenuPaint();
          } 
  
}


         

          

char OpenScreenSet=0;

//Ustawienie grafiki ekranu******************************************************************************************************************
void ScreenSet_Paint(_Screen* ScreenLokal)
{
  char PaternDesignNr;
  
  if (CurModifSheet==0) 
  {
    PaternDesignNr=ScreenSetTab_0_Nr;
  }else{
    PaternDesignNr=ScreenSetTab_1_Nr; 
  }      
  
  if (OpenScreenSet==0)
  {
    OpenScreenSet=1; 
    if (ScreenLokal->PaternDesign>4)    if (CurModifSheet==0) RowsPoz=3; else RowsPoz=2;
  }
  
  

#ifdef __ALARM 
  if (CurModifSheet==0)
  {
    List_Paint(Speech[g_leng].Ekran,PaternDesignNr+4,13,0,0,0,"", Speech[g_leng].Styl1, Speech[g_leng].Styl2, Speech[g_leng].Styl3,Speech[g_leng].Styl4,Speech[g_leng].Styl5,Speech[g_leng].Styl6,Speech[g_leng].Styl7, Speech[g_leng].Styl8,Speech[g_leng].Termostat,Speech[g_leng].Terminarz, "Alarm"); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])
  }else{
    List_Paint(Speech[g_leng].Ekran,PaternDesignNr+4,13,0,0,0,"", Speech[g_leng].Styl1, Speech[g_leng].Styl2, Speech[g_leng].Styl3,Speech[g_leng].Styl4,Speech[g_leng].Styl5,Speech[g_leng].Termostat,Speech[g_leng].Terminarz, "Alarm"); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])
  }
#else  
  if (CurModifSheet==0)
  {
    List_Paint(Speech[g_leng].Ekran,PaternDesignNr+3,13,0,0,0,"", Speech[g_leng].Styl1, Speech[g_leng].Styl2, Speech[g_leng].Styl3,Speech[g_leng].Styl4,Speech[g_leng].Styl5,Speech[g_leng].Styl6,Speech[g_leng].Styl7, Speech[g_leng].Styl8,Speech[g_leng].Termostat,Speech[g_leng].Terminarz); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])
  }else{
    List_Paint(Speech[g_leng].Ekran,PaternDesignNr+3,13,0,0,0,"", Speech[g_leng].Styl1, Speech[g_leng].Styl2, Speech[g_leng].Styl3,Speech[g_leng].Styl4,Speech[g_leng].Styl5,Speech[g_leng].Termostat,Speech[g_leng].Terminarz); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])
  }
#endif
  
   char ekran_tab[10] =  {0,4,4,10,11,16,16,32,64,0};
   char MarkPoz= ScreenLokal->PaternDesign+1-RowsPoz; 
   if (CurModifSheet==0)
     {
      if ((MarkPoz!=0) && (MarkPoz<=6))   CopyToCanvasPoint((char *)ekran_tab,52,98-(13*( MarkPoz)),8,10);  
     }else{
      if ((MarkPoz!=0) && (MarkPoz<=6))   CopyToCanvasPoint((char *)ekran_tab,52,98-(13*( MarkPoz)),8,10); 
     }

  
  CleanCanvasArea(1,97,60,11,0);//(char X, char Y, char X_Lenght, char Y_Lenght, char Colour)
    
  Button("< ",1,96,15,13);
  Button("> ",47,96,15,13);
  
  char txt[5]={'N', 'r', '.',':', 0,};
  
  CharToStr(CurModifSheet, &txt[4], 1); //(char Value, char *Vstring, char ill);
  Label(txt,0,22,98,5,20,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 

  H_line(0,95,61,1); 

}

    char MenuPaintExecute=0;

void ScreenSet_Event(_Screen* ScreenLokal)
{
    char PaternDesignNr;

    MenuPaintExecute=0;
  if (CurModifSheet==0) 
  {
    PaternDesignNr=ScreenSetTab_0_Nr;
  }else{
    PaternDesignNr=ScreenSetTab_1_Nr; 
  }      

        int ans;
#ifdef __ALARM
        ans= List_Event(X_up, Y_up, X_down, Y_down, PaternDesignNr+4 , 13, 0,0);//(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
#else
        ans= List_Event(X_up, Y_up, X_down, Y_down, PaternDesignNr+3 , 13, 0,0);//(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
#endif        
        if ((ans>0) && (RowsPoz+1>=ans)) ans=0;
            
              if (ans>1) 
              { 
                OpenScreenSet=0;
                ScreenLokal->PaternDesign=ans-2;
                if (((CurModifSheet==0) && (ans==PaternDesignThermoMS) ) || ((CurModifSheet>0) && (ans==PaternDesignThermoSS)))
                {
                  MenuLevel=(MenuLevel*10+2);         //uruchomienie ustawiania termostatu
                }
                else if (((CurModifSheet==0) && (ans==PaternDesignTimetableMS) ) || ((CurModifSheet>0) && (ans==PaternDesignTimetableSS)))
                {
                  MenuLevel=(MenuLevel*10+3);         //ustawianie ekranów
            
                #ifdef __ALARM
                }
                else if (((CurModifSheet==0) && (ans==PaternDesignAlarmMS) ) || ((CurModifSheet>0) && (ans==PaternDesignAlarmSS)))
                {
                MenuLevel=(MenuLevel*10+4);         //ustawianie ekranów
                #endif
                }else{
                  MenuLevel=(MenuLevel*10+1);         //ustawianie ekranów
                }
                MenuPaintExecute=1;
                ProgramChange=1;
              }else if (ans==1)
              {

              }  
              
                              
                if ((Button("< ",1,96,15,13)==1) && (CurModifSheet>0)) {MenuPaintExecute=1; CurModifSheet--; RowsPoz=0; OpenScreenSet=0;}
                if ((Button("> ",47,96,15,13)==1)&& (CurModifSheet<ScreenNr-1)) {
                  OpenScreenSet=0;
                  RowsPoz=0;
                  MenuPaintExecute=1; 
                  CurModifSheet++; 
                   }
                
              
              else if (ans<0)
              {
                   if (ProgramChange) 
                    {
                        MenuLevel=(MenuLevel*10+0);
                    }else{
                        MenuLevel=(MenuLevel/10);
                    }
                      MenuPaintExecute=1;
                
              }else{

              }
              
              
              
              if (MenuPaintExecute) MenuPaint(); 
           
   
}




void ThermostatSet_Paint(_Screen* Screen)
{          
       List_Paint("Therm set ",0,108,0,0,3," ",0 , 0, 0, 0, 0, 0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])                                                                                                                                     

       char txt[8];
       
       
          Label(Speech[g_leng].Czujnik,2,5, 100 ,10,52,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)      
          char ind;
          if ((Screen->Thermo.SensorNode>=0) && (Screen->Thermo.SensorNode!=0xFF))
          {
            txt[0]='I'; txt[1]='d'; txt[2]=':'; txt[3]=' '; txt[4]=0;
            ind=CharToStr(Screen->Thermo.SensorNode, &(txt[3]), 2); //(char Value, char *Vstring, char ill);
            txt[ind+3]=0;
            Label(txt,2,20, 88 ,ind+3,22,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          }else{
            Label("Var",2,20, 88 ,3,22,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          }
                  
          txt[0]='<';txt[1]=0;
          Button(txt,3,85,15,15); 
          txt[0]='>';
          Button(txt,45,85,15,15); 
          
          if ((Screen->Thermo.SensorNode>=0) && (Screen->Thermo.SensorNode!=0xFF))
          {
            txt[0]='R'; txt[1]='e'; txt[2]='g'; txt[3]=':'; txt[4]=' ';
          
            ind=CharToStr(Screen->Thermo.SensorReg, &(txt[4]), 2); //(char Value, char *Vstring, char ill);
             Label(txt,2,20, 72 ,ind+4,22,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 

            txt[ind+4]=0;
          }else{
            txt[0]='a'; 
            ind=CharToStr(Screen->Thermo.SensorReg, &(txt[1]), 2); //(char Value, char *Vstring, char ill);
             Label(txt,2,20, 72 ,ind+1,22,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          }
         
          txt[0]='<';txt[1]=0;
          Button(txt,3,69,15,15); 
          txt[0]='>';
          Button(txt,45,69,15,15); 
  
          
          
      
          Label(Speech[g_leng].Wykonawca,2,5, 58 ,10,52,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)      
          if (((Screen->Thermo.ActorNode)>=0) && (Screen->Thermo.ActorNode!=0xFF))
          {
            txt[0]='I'; txt[1]='d'; txt[2]=':'; txt[3]=' '; txt[4]=0;
            ind=CharToStr(Screen->Thermo.ActorNode, &(txt[3]), 2); //(char Value, char *Vstring, char ill);
            txt[ind+3]=0;
            Label(txt,2,20, 45 ,ind+3,22,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          }else{
            Label("Var",2,20, 45, 3,22,1,127,0);
          }
          txt[0]='<';txt[1]=0;
          Button(txt,3,42,15,15); 
          txt[0]='>';
          Button(txt,45,42,15,15); 
          
           if (((Screen->Thermo.ActorNode)>=0) && (Screen->Thermo.ActorNode!=0xFF))
          {
            txt[0]='P'; txt[1]=':'; txt[2]=' '; 
            ind=CharToStr(Screen->Thermo.ActorPort, &(txt[2]), 2); //(char Value, char *Vstring, char ill);
            txt[ind+2]=0;
          }else{
            txt[0]='b'; txt[1]=' '; txt[2]=' '; 
            ind=CharToStr(Screen->Thermo.ActorPort, &(txt[1]), 2); //(char Value, char *Vstring, char ill);
            txt[ind+1]=0;
          }
          txt[ind+2]=0;
          Label(txt,2,20, 29 ,ind+2,22,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
        
          txt[0]='<';txt[1]=0;
          Button(txt,3,26,15,15); 
          txt[0]='>';
          Button(txt,45,26,15,15); 

          if (CurModifSheet==0) ZoneButon(Screen->Button[0].Text,4,3,56,22,(char*)ButtonName[10*CurModifSheet]); //*  Label- etykieta,  poz_X - pozycja lewy róg, poz_Y - pozycja dolny róg, height, with
   
       
}

void ThermostatSet_Event(_Screen* Screen)
{
  char Repaint=0;
          if (List_Event(X_up, Y_up, X_down, Y_down, 0 , 15, 0,0)<0)
        {
          MenuLevel=(MenuLevel/10);
          Repaint=1;
        } else if ((Y_down<112) || (Y_up<112))
        { 
          char MaxSensorReg=0;
          if ((Screen->Thermo.SensorNode>=0) && (Screen->Thermo.SensorNode!=0xFF))
          {
            MaxSensorReg=NumberReg;
          }else{
            MaxSensorReg=20; 
          }
          
          
          char MaxActorPort=0;
          if (((Screen->Thermo.ActorNode)>=0) && (Screen->Thermo.ActorNode!=0xFF))
          {
            MaxActorPort=NumberNeuronsPort;
          }else{
            MaxActorPort=100; 
          }
          
          
          char txt[5];         
          txt[0]='<';txt[1]=0;
          if (Button(txt,3,85,15,15)==1) { if(Screen->Thermo.SensorNode>=0) Screen->Thermo.SensorNode--; else Screen->Thermo.SensorNode=NumberNeurons-1; Repaint=1;}  
          txt[0]='>';
          if (Button(txt,45,85,15,15)==1) { if(Screen->Thermo.SensorNode+1<NumberNeurons) Screen->Thermo.SensorNode++; else Screen->Thermo.SensorNode=-1; Repaint=1;} 
         
          txt[0]='<';txt[1]=0;
          if (Button(txt,3,69,15,15)==1) { if(Screen->Thermo.SensorReg>1) Screen->Thermo.SensorReg--; else Screen->Thermo.SensorReg=MaxSensorReg-1; Repaint=1;}  
          txt[0]='>';
          if (Button(txt,45,69,15,15)==1) { if(Screen->Thermo.SensorReg+1<MaxSensorReg) Screen->Thermo.SensorReg++; else Screen->Thermo.SensorReg=1; Repaint=1;} 
         
          txt[0]='<';txt[1]=0;
          if (Button(txt,3,42,15,15)==1) { if(Screen->Thermo.ActorNode>=0) Screen->Thermo.ActorNode--; else Screen->Thermo.ActorNode=NumberNeurons-1; Repaint=1;}  
          txt[0]='>';
          if (Button(txt,45,42,15,15)==1) { if(Screen->Thermo.ActorNode+1<NumberNeurons) Screen->Thermo.ActorNode++; else Screen->Thermo.ActorNode=-1; Repaint=1;} 
         
          txt[0]='<';txt[1]=0;
          if (Button(txt,3,26,15,15)==1) { if(Screen->Thermo.ActorPort-1>0) Screen->Thermo.ActorPort--; else Screen->Thermo.ActorPort=MaxActorPort-1; Repaint=1;}  
          txt[0]='>';
          if (Button(txt,45,26,15,15)==1) { if(Screen->Thermo.ActorPort+1<MaxActorPort) Screen->Thermo.ActorPort++; else Screen->Thermo.ActorPort=0; Repaint=1;} 
              
          
          if (CurModifSheet==0) 
          if (
                  Zone(0,  //*  (char _Label[15], char poz_X , char poz_Y ,char width, char height, char LineType) 
                  4,3,56,22,
                  2)==1
                 )
                 {
                    ButtonToChange=&(Screen->Button[0]);
                    MenuLevel=(MenuLevel*10+4); Repaint=1;
                 }
          
          
          
        }
        if (Repaint) MenuPaint();
}







void ScreenTimetableSet_Paint(_Screen* Screen)
{          
       //List_Paint(Speech[g_leng].Opcje,2,15,y,0,3,Speech[g_leng].DodajWiersz, Speech[g_leng].UsunWiersz, 0,0,0,0,0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])
       List_Paint("Actor",0,108,0,0,3,0,0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      
       
       char txt[8];
       


          char ind=0;
          
          Label(Speech[g_leng].Wykonawca,2,5, 88 ,10,52,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)      
         
          if (Screen->Timetable.ActorNode>=0)
          {
            txt[0]='I'; txt[1]='d'; txt[2]=':'; txt[3]=' ';
            ind=CharToStr(Screen->Timetable.ActorNode, &(txt[3]), 2); //(char Value, char *Vstring, char ill);
            Label(txt,2,20, 75 ,ind+3,22,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          }else{
            Label("Var",2,20, 75 ,ind+3,22,1,127,0);  
          }
          txt[0]='<';txt[1]=0;
          Button(txt,3,72,15,15); 
          txt[0]='>';
          Button(txt,45,72,15,15); 
          
          if (Screen->Timetable.ActorNode>=0)
          {
            txt[0]='P'; txt[1]=':'; txt[2]=' '; 
             ind=CharToStr(Screen->Timetable.ActorPort, &(txt[2]), 2); //(char Value, char *Vstring, char ill);
             ind+=2;
          }else{
            txt[0]='N'; txt[1]='r'; txt[2]=':'; txt[3]=' '; 
             ind=CharToStr(Screen->Timetable.ActorPort, &(txt[3]), 2); //(char Value, char *Vstring, char ill);
             ind+=3;
          }
          
         
         
          Label(txt,2,20, 55 ,ind,22,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
      
          
          
          txt[0]='<';txt[1]=0;
          Button(txt,3,52,15,15); 
          txt[0]='>';
          Button(txt,45,52,15,15); 

         
          txt[0]='<';txt[1]=0;
          Button(txt,3,24,15,15); 
          txt[0]='>';
          Button(txt,45,24,15,15); 
       
          switch (Screen->Timetable.ActionType)
          {
            case 0: CopyToCanvasPoint((char*)ProcSet_graf, 21, 27,24,9); 
                      txt[0]='Q'; txt[1]=0;
                      Label(txt,2,28,27,1,6,0,120,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 

            
                      break; //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght); 
            case 1: CopyToCanvasPoint((char*)ProcWOUTS_graf, 21, 27,24,9); break;
            case 2: CopyToCanvasPoint((char*)ProcWOUTR_graf, 21, 27,24,9); break;
          }
          

            
          
          Label( "-1h",1,27,6,5,33,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          H_line(30,4,12,1); 
          H_line(30,16,12,1); 
          V_line(30,4,12,1);
          V_line(42,4,13,1); 
          

          if (Screen->Timetable.Min1h)
          {
             char ekran_tab[10] =  {0,4,4,10,11,16,16,32,64,0};
            CopyToCanvasPoint((char *)ekran_tab,33,5,8,10);  //(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght
          }

}

void ScreenTimetableSet_Event(_Screen* Screen)
{
  char Repaint=0;
          if (List_Event(X_up, Y_up, X_down, Y_down, 0 , 15, 0,0)<0)
        {
          MenuLevel=(MenuLevel/10);
          Repaint=1;
        } else if ((Y_down<112) || (Y_up<112))
        { 
          char txt[3];
          txt[0]='<';txt[1]=0;
          if (Button(txt,3,72,15,15)==1) { if(Screen->Timetable.ActorNode>-1) Screen->Timetable.ActorNode--; else Screen->Timetable.ActorNode=NumberNeurons-1; Repaint=1;}  
          txt[0]='>';
          if (Button(txt,45,72,15,15)==1) { if(Screen->Timetable.ActorNode+1<NumberNeurons) Screen->Timetable.ActorNode++; else Screen->Timetable.ActorNode=-1; Repaint=1;} 
         
          
          if (Screen->Timetable.ActorNode>=0)
          {
            txt[0]='<';txt[1]=0;
            if (Button(txt,3,52,15,15)==1) { if(Screen->Timetable.ActorPort>0) Screen->Timetable.ActorPort--; else Screen->Timetable.ActorPort=NumberNeuronsPort-1; Repaint=1;}  
            txt[0]='>';
            if (Button(txt,45,52,15,15)==1) { if(Screen->Timetable.ActorPort+1<NumberNeuronsPort) Screen->Timetable.ActorPort++; else Screen->Timetable.ActorPort=0; Repaint=1;} 
          }else{
            //je¿eli znienna 
            txt[0]='<';txt[1]=0;
            if (Button(txt,3,52,15,15)==1) { if(Screen->Timetable.ActorPort>0) Screen->Timetable.ActorPort--; else Screen->Timetable.ActorPort=NumberNeuronsPort-1; Repaint=1;}  
            txt[0]='>';
            if (Button(txt,45,52,15,15)==1) { if(Screen->Timetable.ActorPort+1<NumberNeuronsPort) Screen->Timetable.ActorPort++; else Screen->Timetable.ActorPort=99; Repaint=1;} 
          }
            
          
          txt[0]='<';txt[1]=0;
          if (Button(txt,3,24,15,15)==1) {if (Screen->Timetable.ActionType>0) Screen->Timetable.ActionType--; else Screen->Timetable.ActionType=2; Repaint=1;} 
          txt[0]='>';
          if (Button(txt,45,24,15,15)==1) {if (Screen->Timetable.ActionType<2) Screen->Timetable.ActionType++; else Screen->Timetable.ActionType=0; Repaint=1;}  
          
          //jezeli przyciœniêto znacznik -1h
          if ((X_up>=30) && (X_up<=42) && (Y_up>=4) && (Y_up<=16)) {if (Screen->Timetable.Min1h) Screen->Timetable.Min1h=0; else Screen->Timetable.Min1h=1;  Repaint=1;}
        }  
        if (Repaint) MenuPaint();
        

}







void ScreenAlarmSet_Paint(_Screen* Screen)
{          
       List_Paint("Alarm",1,108,0,0,3,0,0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      
       
       char txt[8];

       
       
          char ind=0; 
          Label(Speech[g_leng].Czujnik,2,5, 100 ,10,52,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)         
          if (((Screen->Alarm.SensorNode)>=0) && (Screen->Alarm.SensorNode!=0xFF))
          {
            txt[0]='I'; txt[1]='d'; txt[2]=':'; txt[3]=' '; txt[4]=0;
            ind=CharToStr(Screen->Alarm.SensorNode, &(txt[3]), 2); //(char Value, char *Vstring, char ill);
            txt[ind+3]=0;
            Label(txt,2,20, 88 ,ind+3,22,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          }else{
            Label("Var",2,20, 88, 3,22,1,127,0);
          }
          
          
          txt[0]='<';txt[1]=0;
          Button(txt,3,85,15,15); 
          txt[0]='>';
          Button(txt,45,85,15,15); 
          
           if (((Screen->Alarm.SensorNode)>=0) && (Screen->Alarm.SensorNode!=0xFF))
          {
            txt[0]='P'; txt[1]=':'; txt[2]=' '; 
            ind=CharToStr(Screen->Alarm.SensorPort, &(txt[2]), 2); //(char Value, char *Vstring, char ill);
            txt[ind+2]=0;
          }else{
            txt[0]='b'; txt[1]=' '; txt[2]=' '; 
            ind=CharToStr(Screen->Alarm.SensorPort, &(txt[1]), 2); //(char Value, char *Vstring, char ill);
            txt[ind+1]=0;
          }
          txt[ind+2]=0;
          Label(txt,2,20, 72 ,ind+2,22,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
        
          txt[0]='<';txt[1]=0;
          Button(txt,3,69,15,15); 
          txt[0]='>';
          Button(txt,45,69,15,15); 



       
       
          Label(Speech[g_leng].Syrena,2,5, 58 ,10,52,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)      
          if (((Screen->Alarm.ActorNode)>=0) && (Screen->Alarm.ActorNode!=0xFF))
          {
            txt[0]='I'; txt[1]='d'; txt[2]=':'; txt[3]=' '; txt[4]=0;
            ind=CharToStr(Screen->Alarm.ActorNode, &(txt[3]), 2); //(char Value, char *Vstring, char ill);
            txt[ind+3]=0;
            Label(txt,2,20, 45 ,ind+3,22,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          }else{
            Label("Var",2,20, 45, 3,22,1,127,0);
          }
          txt[0]='<';txt[1]=0;
          Button(txt,3,42,15,15); 
          txt[0]='>';
          Button(txt,45,42,15,15); 
     
           if (((Screen->Alarm.ActorNode)>=0) && (Screen->Alarm.ActorNode!=0xFF))
          {
            txt[0]='P'; txt[1]=':'; txt[2]=' '; 
            ind=CharToStr(Screen->Alarm.ActorPort, &(txt[2]), 2); //(char Value, char *Vstring, char ill);
            txt[ind+2]=0;
          }else{
            txt[0]='b'; txt[1]=' '; txt[2]=' '; 
            ind=CharToStr(Screen->Alarm.ActorPort, &(txt[1]), 2); //(char Value, char *Vstring, char ill);
            txt[ind+1]=0;
          }
          txt[ind+2]=0;
          Label(txt,2,20, 29 ,ind+2,22,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
   
          txt[0]='<';txt[1]=0;
          Button(txt,3,26,15,15); 
          txt[0]='>';
          Button(txt,45,26,15,15); 
       
}

void ScreenAlarmSet_Event(_Screen* Screen)
{
   char Repaint=0;
          if (List_Event(X_up, Y_up, X_down, Y_down, 0 , 15, 0,0)<0)
        {
          MenuLevel=(MenuLevel/10);
          Repaint=1;
        } else if ((Y_down<112) || (Y_up<112))
        { 
          
          char MaxSensorPort=0;
          if (((Screen->Alarm.SensorNode)>=0) && (Screen->Alarm.SensorNode!=0xFF))
          {
            MaxSensorPort=NumberNeuronsPort;
          }else{
            MaxSensorPort=100; 
          }
          
          char MaxActorPort=0;
          if (((Screen->Alarm.ActorNode)>=0) && (Screen->Alarm.ActorNode!=0xFF))
          {
            MaxActorPort=NumberNeuronsPort;
          }else{
            MaxActorPort=100; 
          }
          
          
          char txt[5];         
          txt[0]='<';txt[1]=0;
          if (Button(txt,3,85,15,15)==1) { if(Screen->Alarm.SensorNode>=0) Screen->Alarm.SensorNode--; else Screen->Alarm.SensorNode=NumberNeurons-1; Repaint=1;}  
          txt[0]='>';
          if (Button(txt,45,85,15,15)==1) { if(Screen->Alarm.SensorNode+1<NumberNeurons) Screen->Alarm.SensorNode++; else Screen->Alarm.SensorNode=-1; Repaint=1;} 
         
          txt[0]='<';txt[1]=0;
          if (Button(txt,3,69,15,15)==1) { if(Screen->Alarm.SensorPort>1) Screen->Alarm.SensorPort--; else Screen->Alarm.SensorPort=MaxSensorPort-1; Repaint=1;}  
          txt[0]='>';
          if (Button(txt,45,69,15,15)==1) { if(Screen->Alarm.SensorPort+1<MaxSensorPort) Screen->Alarm.SensorPort++; else Screen->Alarm.SensorPort=0; Repaint=1;} 
         
          txt[0]='<';txt[1]=0;
          if (Button(txt,3,42,15,15)==1) { if(Screen->Alarm.ActorNode>=0) Screen->Alarm.ActorNode--; else Screen->Alarm.ActorNode=NumberNeurons-1; Repaint=1;}  
          txt[0]='>';
          if (Button(txt,45,42,15,15)==1) { if(Screen->Alarm.ActorNode+1<NumberNeurons) Screen->Alarm.ActorNode++; else Screen->Alarm.ActorNode=-1; Repaint=1;} 
         
          txt[0]='<';txt[1]=0;
          if (Button(txt,3,26,15,15)==1) { if(Screen->Alarm.ActorPort>1) Screen->Alarm.ActorPort--; else Screen->Alarm.ActorPort=MaxActorPort-1; Repaint=1;}  
          txt[0]='>';
          if (Button(txt,45,26,15,15)==1) { if(Screen->Alarm.ActorPort+1<MaxActorPort) Screen->Thermo.ActorPort++; else Screen->Alarm.ActorPort=0; Repaint=1;} 
              
          
          
          /*txt[0]='<';txt[1]=0;
          if (Button(txt,3,85,15,15)==1) { if(Screen->Alarm.SensorNode>0) Screen->Alarm.SensorNode--; else Screen->Alarm.SensorNode=NumberNeurons-1; Repaint=1;}  
          txt[0]='>';
          if (Button(txt,45,85,15,15)==1) { if(Screen->Alarm.SensorNode+1<NumberNeurons) Screen->Alarm.SensorNode++; else Screen->Alarm.SensorNode=0; Repaint=1;} 
         
          txt[0]='<';txt[1]=0;
          if (Button(txt,3,69,15,15)==1) { if(Screen->Alarm.SensorPort>0) Screen->Alarm.SensorPort--; else Screen->Alarm.SensorPort=NumberReg-1; Repaint=1;}  
          txt[0]='>';
          if (Button(txt,45,69,15,15)==1) { if(Screen->Alarm.SensorPort+1<NumberReg) Screen->Alarm.SensorPort++; else Screen->Alarm.SensorPort=0; Repaint=1;} 
         
          txt[0]='<';txt[1]=0;
          if (Button(txt,3,42,15,15)==1) { if(Screen->Alarm.ActorNode-1>0) Screen->Alarm.ActorNode--; else Screen->Alarm.ActorNode=NumberNeurons-1; Repaint=1;}  
          txt[0]='>';
          if (Button(txt,45,42,15,15)==1) { if(Screen->Alarm.ActorNode+1<NumberNeurons) Screen->Alarm.ActorNode++; else Screen->Alarm.ActorNode=0; Repaint=1;} 
         
          txt[0]='<';txt[1]=0;
          if (Button(txt,3,26,15,15)==1) { if(Screen->Alarm.ActorPort>0) Screen->Alarm.ActorPort--; else Screen->Alarm.ActorPort=NumberNeuronsPort-1; Repaint=1;}  
          txt[0]='>';
          if (Button(txt,45,26,15,15)==1) { if(Screen->Alarm.ActorPort+1<NumberNeuronsPort) Screen->Alarm.ActorPort++; else Screen->Alarm.ActorPort=0; Repaint=1;} 
          */
        }
        if (Repaint) MenuPaint();
}






void ZoneButon(char _Label[15], char poz_X , char poz_Y ,char width, char height, char *Vstring)
{
 if (height>12)
 {
  Zone(0,poz_X,poz_Y,width,height,2); //*  Label- etykieta,  poz_X - pozycja lewy róg, poz_Y - pozycja dolny róg, height, with
  Label(Vstring,0,poz_X+2,poz_Y+height-10,3,10,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
  Zone(_Label,poz_X+2,poz_Y+2,width-4,height-12,1); //*  Label- etykieta,  poz_X - pozycja lewy róg, poz_Y - pozycja dolny róg, height, with 
 }else{
  Zone(0,poz_X,poz_Y,width,height,2); //*  Label- etykieta,  poz_X - pozycja lewy róg, poz_Y - pozycja dolny róg, height, with, LineType- typ lini 0-bia³a 1=ci¹gla, 2=kropkowana, 3=kreskowana
  
  Label(Vstring,0,poz_X+2,poz_Y+1,3,10,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
  Zone(_Label,poz_X+17,poz_Y+2,width-19,height-4,1); //*  Label- etykieta,  poz_X - pozycja lewy róg, poz_Y - pozycja dolny róg, height, with 
   
 }
}

void ZoneGraph(char _Label[15], char poz_X , char poz_Y ,char width, char height, char *Vstring, _Graph *Graph)
{
   if ((Graph->Type>GraphNone) && (Graph->Type<=GraphTermometr))
   {
     CopyToCanvasPoint((char*) GraphTab[Graph->Type], poz_X, poz_Y+3, 18,18);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);           
   
   }   

   if ((Graph->Type==GraphOnOff) && (Graph->PlusOnOff==0))
   {
     Label(Speech[g_leng].Wl,2,poz_X,poz_Y+(height/2)+2,3,width,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
     Label(Speech[g_leng].Wyl,2,poz_X,poz_Y+(height/2)-6,3,width,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 

   }   
   
   if (Graph->Type==GraphScena)
   {
     Label("Scn ",2,poz_X,poz_Y+(height/2)-3,3,width,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
     H_line(poz_X+3,poz_Y+(height/2)-5,width-6,1); 
   }
   
   if ((Graph->Type==GraphText) && (Graph->PlusOnOff==0))
   {
     Label(Graph->Text,2,poz_X,poz_Y+(height/2)-3,3,width,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
   }   
   
   if ((Graph->PlusOnOff) && (((Graph->Type==GraphOnOff))||(Graph->Type==GraphText)))
   {
      Label(Graph->Text,2,poz_X,poz_Y+(height/2)+1,3,width,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
      Label(Speech[g_leng].Wl,2,poz_X,poz_Y+(height/2)-7,3,width,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
   }
   
   
   if (Graph->Type==GraphWartosc)
    {
      char txt[5];
      txt[0]='A';txt[1]='n';txt[2]='a'; txt[3]=0; 
      Label(txt,2,poz_X-1,poz_Y+(height/2)-4,3,width,1,127,0); 
    }
   
  Zone(0,poz_X,poz_Y,width,height,2); //*  Label- etykieta,  poz_X - pozycja lewy róg, poz_Y - pozycja dolny róg, height, with
  if (Vstring!=0) Label(Vstring,0,poz_X+2,poz_Y+height-10,11,10,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
 
}

void ScreenGraph(char poz_X , char poz_Y ,char width, char height, _Graph *Graph)
{
   char Val;
  
   if ((Graph->Type>GraphNone) && (Graph->Type<=GraphTermometr))
   {
     CopyToCanvasPoint((char*) GraphTab[Graph->Type], poz_X, poz_Y+3, 18,18);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);           
   
   }   

   if ((Graph->Type==GraphOnOff) && (Graph->PlusOnOff==0))
   {
       if ((Graph->Nr>=0) && (Graph->Nr!=0xFF))    //je¿eli wêze³
       {
          Val=PortStageRead(MainStage.StageI[Graph->Nr].Port, Graph->Port);
       }else{
          Val=(Variables[Graph->Port/8]>>(Graph->Port%8)) & 0x1;
       }
       
       if (Val)
       {
          Label(Speech[g_leng].Wl,2,poz_X,poz_Y+(height/2)-3,3,width,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
       }else{
          Label(Speech[g_leng].Wyl,2,poz_X,poz_Y+(height/2)-3,3,width,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
       }
   }
   
   if (Graph->Type==GraphScena)
   {
      char LabelButt[2] = {0,0};
      
      CharToStr(MainStage.SceneNr,LabelButt,1);
      Label(LabelButt,2,poz_X,poz_Y+(height/2)-3,3,width,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
   }
   
   if ((Graph->Type==GraphText) && (Graph->PlusOnOff==0))
   {
     Label(Graph->Text,2,poz_X,poz_Y+(width/2)-3,3,width,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
   }   
   
      if ((Graph->PlusOnOff) && (((Graph->Type==GraphOnOff))||(Graph->Type==GraphText)))
   {
      Label(Graph->Text,2,poz_X,poz_Y+(height/2)+1,3,width,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
      
      
      if ((Graph->Nr>=0) && (Graph->Nr!=0xFF))    //je¿eli wêze³
       {
          Val=PortStageRead(MainStage.StageI[Graph->Nr].Port, Graph->Port);
       }else{
          Val=(Variables[Graph->Port/8]>>(Graph->Port%8)) & 0x1;
       }
       
      
      if (Val)
       {
          Label(Speech[g_leng].Wl,2,poz_X,poz_Y+(height/2)-8,3,width,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
       }else{
          Label(Speech[g_leng].Wyl,2,poz_X,poz_Y+(height/2)-8,3,width,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
       }
   }
   
   
                 //Analogi
     char IlLiter=0;
    signed short Value;
    char txt[5];
   if (Graph->Type==GraphWartosc)
   {
    
                     if ((Graph->Nr>0) || ((Graph->Nr==0)&&(Graph->Reg>0)))
                     {
                     IlLiter=0;
                     Value=MainStage.StageI[Graph->Nr].Value[Graph->Reg];

                     if (Value==(signed short)0x8AAA)  //je¿eli sygna³ niewiarygodny
                     {
                       txt[0]='?';txt[1]='?';txt[2]='?'; txt[3]=0; IlLiter+=3;
                       Label(txt,2,poz_X,poz_Y+(height/2)-8,3,width,1,127,0);
                     }else{ //je¿eli sygna³ wiarygodny
                      
                       if (Graph->Flip)
                         {
                             IlLiter=sIntToStr(Value, &(txt[0])); //(char Value, char *Vstring, char ill);
                             Label(txt,2,poz_X,poz_Y+(height/2)+0,IlLiter,width,1,127,0);
                             Label(Graph->Text,2,poz_X,poz_Y+(height/2)-9,3,width,1,127,0);
                             
                         }else{
                             IlLiter=sIntToStr(Value, &(txt[0])); //(char Value, char *Vstring, char ill);
                             Label(txt,2,poz_X,poz_Y+(height/2)-9,IlLiter,width,1,127,0);
                             Label(Graph->Text,2,poz_X,poz_Y+(height/2)+0,3,width,1,127,0);
                         }

                     }  
       
                     
                                              
                     }
   }
            
   
   
}



void ScreenDesign1_Paint(_Screen *ScreenLocal)
{

  const _ScreenPaternDesign *ScreenPaternDesign;
  CleanCanvas();
  if (CurModifSheet==0) 
  {
    ScreenPaternDesign=ScreenSetTab_0;
  }else{
    ScreenPaternDesign=ScreenSetTab_1; 
    
    //Strefa etykiety podstrony
     ZoneGraph(0, 15,114,38,12,
            ScreenLocal->Label,0);  //graf2   //*  Label- etykieta,  poz_X - pozycja lewy róg, poz_Y - pozycja dolny róg, height, with
    
    
  }  

 
   char PaternDesign=ScreenLocal->PaternDesign;

   
   
   
   char LabelButt[2] = {129,0};
   Button(LabelButt,0,113,13,13);

   //przyciski
  for (char i=0; i<ScrenButtonNr; i++)
  {
    if (ScreenPaternDesign[PaternDesign].Button[i].Width>0)
    {
            ZoneButon(ScreenLocal->Button[i].Text,
            ScreenPaternDesign[PaternDesign].Button[i].Poz.x,
            ScreenPaternDesign[PaternDesign].Button[i].Poz.y,
            ScreenPaternDesign[PaternDesign].Button[i].Width,
            ScreenPaternDesign[PaternDesign].Button[i].Height,(char*)ButtonName[i+10*CurModifSheet]); //*  Label- etykieta,  poz_X - pozycja lewy róg, poz_Y - pozycja dolny róg, height, with
    }
   }


  //grafika
  for (char i=0; i<ScrenGraphNr; i++)
  {

    if (ScreenPaternDesign[PaternDesign].Graph[i].Width>0)
    {
            ZoneGraph(0,
            ScreenPaternDesign[PaternDesign].Graph[i].Poz.x,
            ScreenPaternDesign[PaternDesign].Graph[i].Poz.y,
            ScreenPaternDesign[PaternDesign].Graph[i].Width,
            ScreenPaternDesign[PaternDesign].Graph[i].Height,
            0,&(ScreenLocal->Graph[i]));  //graf2   //*  Label- etykieta,  poz_X - pozycja lewy róg, poz_Y - pozycja dolny róg, height, with
    
    }
   }

   //analogi
   for (char i=0; i<ScrenAnalogNr; i++)
   {
       if (ScreenPaternDesign[PaternDesign].Value[i].Width>0)
    {
            ZoneGraph(0,
            ScreenPaternDesign[PaternDesign].Value[i].Poz.x,
            ScreenPaternDesign[PaternDesign].Value[i].Poz.y,
            ScreenPaternDesign[PaternDesign].Value[i].Width,
            ScreenPaternDesign[PaternDesign].Value[i].Height,
            "Analo. ",0);  //graf2   //*  Label- etykieta,  poz_X - pozycja lewy róg, poz_Y - pozycja dolny róg, height, with
    
    }
   }
  
   //Linie
   for (char i=0; i<ScrenLineNr; i++)
   {
    if (ScreenPaternDesign[PaternDesign].Line[i].Width>0)
    {
        if (ScreenPaternDesign[PaternDesign].Line[i].Width>ScreenPaternDesign[PaternDesign].Line[i].Height)
        {
            H_line(ScreenPaternDesign[PaternDesign].Line[i].Poz.x,ScreenPaternDesign[PaternDesign].Line[i].Poz.y,ScreenPaternDesign[PaternDesign].Line[i].Width,1); // (char X, int Y, char lenght, char typ)
        }else{
            V_line(ScreenPaternDesign[PaternDesign].Line[i].Poz.x,ScreenPaternDesign[PaternDesign].Line[i].Poz.y,ScreenPaternDesign[PaternDesign].Line[i].Height,1); // (char X, int Y, char lenght, char typ)
    
        }
    }
   }

   //Zegar
   for (char i=0; i<ScrenLineNr; i++)
   {
    if (ScreenPaternDesign[PaternDesign].Time.Width>0)
    {
            ZoneGraph(0,
            ScreenPaternDesign[PaternDesign].Time.Poz.x,
            ScreenPaternDesign[PaternDesign].Time.Poz.y,
            ScreenPaternDesign[PaternDesign].Time.Width,
            ScreenPaternDesign[PaternDesign].Time.Height,
            Speech[g_leng].Zegar,0);  //graf2   //*  Label- etykieta,  poz_X - pozycja lewy róg, poz_Y - pozycja dolny róg, height, with
    }
   }  
  
   //Du¿a temperatura
   for (char i=0; i<ScrenLineNr; i++)
   {
    if (ScreenPaternDesign[PaternDesign].Temperature.Width>0)
    {
            ZoneGraph(0,
            ScreenPaternDesign[PaternDesign].Temperature.Poz.x,
            ScreenPaternDesign[PaternDesign].Temperature.Poz.y,
            ScreenPaternDesign[PaternDesign].Temperature.Width,
            ScreenPaternDesign[PaternDesign].Temperature.Height,
            Speech[g_leng].Temperatura,0);  //graf2   //*  Label- etykieta,  poz_X - pozycja lewy róg, poz_Y - pozycja dolny róg, height, with
            

            
            
    }
   }   
  
  
}


char ZoneButtonClick=0;
char ZoneGrapfNrClick=0;
char ZoneValueNrClick=0;
char ZoneLabelClick=0;
int TxtSpaceAvable=0;

void ScreenDesign1_Event(_Screen* ScreenLokal)
{
  char PaternDesign=ScreenLokal->PaternDesign;
  
  const _ScreenPaternDesign *ScreenPaternDesign;
  if (CurModifSheet==0) 
  {
    ScreenPaternDesign=ScreenSetTab_0;
  }else{
    ScreenPaternDesign=ScreenSetTab_1; 
  }  
  
  
         char LabelButt[2] = {129,0};
         if (Button(LabelButt,0,113,13,13)==1)
         {
              if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}
              MenuLevel=(MenuLevel/10);
              MenuPaint();
          } 
         else
         {   
           
           
          
         ZoneButtonClick=0;
         ZoneGrapfNrClick=0;
       
         
    
         //STREFY przycisków
          for (char i=0; i<ScrenButtonNr; i++)
          {
            if (ScreenPaternDesign[PaternDesign].Button[i].Width>0)
            {
              if (
                  Zone(0,  //*  (char _Label[15], char poz_ , char poz_Y ,char width, char height, char LineType) 
                  ScreenPaternDesign[PaternDesign].Button[i].Poz.x,
                  ScreenPaternDesign[PaternDesign].Button[i].Poz.y,
                  ScreenPaternDesign[PaternDesign].Button[i].Width,
                  ScreenPaternDesign[PaternDesign].Button[i].Height,
                  2)==1
                 )
                 {
                   if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}
                   ZoneButtonClick=i+1;
                   TxtSpaceAvable=ScreenPaternDesign[PaternDesign].Button[i].Width-6;
                 }
            }
          }
          

           
           
           //Strefy grafiki
           ZoneGrapfNrClick=0xff;
          for (char i=0; i<ScrenGraphNr; i++)
          {
            if (ScreenPaternDesign[PaternDesign].Graph[i].Width>0)
            {
              if (
                  Zone(0,  //*  (char _Label[15], char poz_X , char poz_Y ,char width, char height, char LineType) 
                  ScreenPaternDesign[PaternDesign].Graph[i].Poz.x,
                  ScreenPaternDesign[PaternDesign].Graph[i].Poz.y,
                  ScreenPaternDesign[PaternDesign].Graph[i].Width,
                  ScreenPaternDesign[PaternDesign].Graph[i].Height,
                  2)==1
                 )
                 {
                   if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}
                   TxtSpaceAvable=ScreenPaternDesign[PaternDesign].Graph[i].Width-6;
                   ZoneGrapfNrClick=i;
                 }
            }
          }
          

          //Strefy analogów
          ZoneValueNrClick=0xff;
          for (char i=0; i<ScrenAnalogNr; i++)
          {
            if (ScreenPaternDesign[PaternDesign].Value[i].Width>0)
            {
              if (
                  Zone(0,  //*  (char _Label[15], char poz_X , char poz_Y ,char width, char height, char LineType) 
                  ScreenPaternDesign[PaternDesign].Value[i].Poz.x,
                  ScreenPaternDesign[PaternDesign].Value[i].Poz.y,
                  ScreenPaternDesign[PaternDesign].Value[i].Width,
                  ScreenPaternDesign[PaternDesign].Value[i].Height,
                  2)==1
                 )
                 {
                   if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}
                   ZoneValueNrClick=i;
                 }
            }
          }
          
          
          
          
             //Du¿a temperatura
           char  ZoneTemperature=0;

            if (ScreenPaternDesign[PaternDesign].Temperature.Width>0)
            {   
                  if(
                      Zone(0,
                      ScreenPaternDesign[PaternDesign].Temperature.Poz.x,
                      ScreenPaternDesign[PaternDesign].Temperature.Poz.y,
                      ScreenPaternDesign[PaternDesign].Temperature.Width,
                      ScreenPaternDesign[PaternDesign].Temperature.Height,
                      2)==1  
                    )
                   {
                     if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}
                     ZoneTemperature=1;
                   }
           }
          

          //Strefa etykiety podstrony
           ZoneLabelClick=0;

           if (CurModifSheet>0) 
          {
              if (
                  Zone(0, 15,114,38,12,
                  2)==1
                 )
                 {
                   ZoneLabelClick=1;
                 }          
          } 
          
              
          
         
          

         
           if (ZoneButtonClick>0)
           {
             //TextToChange=ScreenLokal->Button[ZoneButtonClick-1].Text;
             ButtonToChange=&(ScreenLokal->Button[ZoneButtonClick-1]);
             MenuLevel=(MenuLevel*10+4); MenuPaint();
           }
          
          if (ZoneValueNrClick<0xFF)
           {
             AnalogToChange=&(ScreenLokal->Analog[ZoneValueNrClick]);
             MenuLevel=(MenuLevel*10+3); MenuPaint();
           }
          
         
           if (ZoneLabelClick>0)
           {
             TextToChange=ScreenLokal->Label;
             MenuLevel=(MenuLevel*10+2); MenuPaint();
           }
           
           if (ZoneTemperature>0)
           {
             MenuLevel=(MenuLevel*10+5); MenuPaint();
           }          
   
          
           if (ZoneGrapfNrClick!=0xFF){MenuLevel=(MenuLevel*10+1); MenuPaint();}
 
        
            
           
         
  
         }
         
         
}




void ButtonOptions_Paint(volatile _ButtonGraph *ButtonGraph)
{
 RowsPoz=0;

 List_Paint(
              Speech[g_leng].Przycisk, //(char name[7]
              3,        //char Rows,
              15,       //char RowWith,
              0,        //char Y,
              0,        //char FormStyle,
              3,        //char algin,
              Speech[g_leng].Text, Speech[g_leng].Akcja, Speech[g_leng].Ukryj," ",0,0,0,0); //char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])
    
           //znacznik ukrycia przycisku
         if (ButtonGraph->Hide)
              {
                    
                char ekran_tab[10] =  {0,4,4,10,11,16,16,32,64,0};
                CopyToCanvasPoint((char *)ekran_tab,50,97-(15*(2)),8,10); 
              }    
}


void ButtonOptions_Event(volatile _ButtonGraph *ButtonGraph)
{
  int odp=List_Event(X_up, Y_up, X_down, Y_down, 3 , 15, 0,0);
  if (odp<0) 
        {
          MenuLevel=(MenuLevel/10);
          MenuPaint();
        }else if (odp>0) 
        {
         
          if  (odp<3)
          {
            if (odp==1) {
              TextToChange=(char*)ButtonGraph->Text; 
              TextToChangeLength=ButonMaxText;
              TextToChangeMaxPixSize=TxtSpaceAvable;
            }
            MenuLevel=(MenuLevel*10)+odp; 
          }else{
            if (ButtonGraph->Hide) ButtonGraph->Hide=0; else ButtonGraph->Hide=1; 
          }
         
          MenuPaint();
        }
          
          
          
        
}


void ButtonAction_Paint(volatile _ButtonGraph *ButtonGraph)
{

  List_Paint(Speech[g_leng].Akcja,1,98,0,0,0,"",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      
     
  
  Button("< ",3,71,16,23);
  Button("> ",44,71,16,23);
  
  
  Label(Speech[g_leng].OtworzStrone,0,3,96,15,58,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
  
  char txt[5]={'N','r', ':',0,0};
  if (ButtonGraph->OpenPage==0)
  {
      Label(Speech[g_leng].Wyl,0,23,75,5,20,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 

  }else{
      CharToStr(ButtonGraph->OpenPage-1, &txt[3], 1); //(char Value, char *Vstring, char ill);
      Label(txt,0,22,76,5,20,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 

      Button(Speech[g_leng].Konfiguracja,3,54,57,14);
  }
  
}


void ButtonAction_Event(volatile _ButtonGraph *ButtonGraph)
{
  char MenuPaintExecute=0;
  int odp=List_Event(X_up, Y_up, X_down, Y_down, 0 , 15, 0,0);
  if (odp<0) 
        {
          MenuLevel=(MenuLevel/10);
          MenuPaintExecute=1;
        }else 
        {
          if ((Button("< ",3,71,16,23)==1) && (ButtonGraph->OpenPage>0))  
          { 
            if ((ButtonGraph->OpenPage-1)!=CurModifSheet+1)
            {
              ButtonGraph->OpenPage--;  MenuPaintExecute=1;
            }else if ((ButtonGraph->OpenPage-1)>0)
            {
              ButtonGraph->OpenPage -=2;  MenuPaintExecute=1;
            }
          } 
          if ((Button("> ",44,71,16,23)==1) && (ButtonGraph->OpenPage<ScreenNr)) 
          {
             if ((ButtonGraph->OpenPage+1)!=CurModifSheet+1)
            {
              ButtonGraph->OpenPage++;  MenuPaintExecute=1;
            }else if (ButtonGraph->OpenPage+1<ScreenNr)
            {
              ButtonGraph->OpenPage +=2;  MenuPaintExecute=1;
            }
          }
          if (Button(Speech[g_leng].Konfiguracja,3,54,57,14)==1)
          {
            CurModifSheet=ButtonGraph->OpenPage-1;
            MenuLevel=112;
            MenuPaintExecute=1;
          }
         
          

          
        }
          
          
        if (MenuPaintExecute)  MenuPaint();
      
}






void BigTemperLabel_Paint(void)
      {  
        char txt[5];
        List_Paint("Temeper.",1,108,0,0,3," ",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      

          Label("Node ID:",2,20, 92 ,8,22,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          char ind=CharToStr(gProg.Screen[0].Analog[0].Nr, &(txt[0]), 2); //(char Value, char *Vstring, char ill);
          Label(txt,2,20, 77 ,ind+3,22,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
        
          txt[0]='<';txt[1]=0;
          Button(txt,3,70,15,20); 
          txt[0]='>';
          Button(txt,45,70,15,20); 
          
          Label("Reg:",2,20, 50 ,4,22,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          
          ind=CharToStr(gProg.Screen[0].Analog[0].Reg, txt, 2); //(char Value, char *Vstring, char ill);
         
          Label(txt,2,20, 40 ,ind+2,22,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
        
          txt[0]='<';txt[1]=0;
          Button(txt,3,40,15,20); 
          txt[0]='>';
          Button(txt,45,40,15,20);   
           
        
      }

void BigTemperLabel_Event(void)
      {  
       char Repaint=0; 
       char txt[8];
       _Analog *Anal=&(gProg.Screen[0].Analog[0]);       
       
       
        //zdarzenie typu down "X"
        if ((X_down>46) && (X_down<63) && (Y_down>112) && (Y_down<127))    //"X"
        {
          //MenuPaint();
          H_line(49,114,10,0);
          H_line(49,123,10,0); 
          V_line(49,114,10,0); 
          V_line(58,114,10,0); 
          
        }
  
           if ((X_up>46) && (X_up<63) && (Y_up>112) && (Y_up<127))                    //"X"
        {
          MenuLevel=(MenuLevel/10);
          MenuPaint();
        }  
        else
        {
            txt[0]='<';txt[1]=0;
            if (Button(txt,3,70,15,20)==2){if (Anal->Nr>0) {Anal->Nr--;} else {Anal->Nr=NumberNeurons;} Repaint=1;}
            txt[0]='>';
            if (Button(txt,45,70,15,20)==2){if (Anal->Nr+1<NumberNeurons) {Anal->Nr++;} else {Anal->Nr=0;}  Repaint=1;}
          
            txt[0]='<';txt[1]=0;
            if (Button(txt,3,40,15,20)==2){if (Anal->Reg>1) {Anal->Reg--;} else {Anal->Reg=NumberReg;} Repaint=1;} 
            txt[0]='>';
            if (Button(txt,45,40,15,20)==2){if (Anal->Reg+1<NumberReg) {Anal->Reg++;} else {Anal->Reg=1;}  Repaint=1;}
      }
      
      if (Repaint) MenuPaint();

}



      #define MaxRowGr 4
      #define MaxColGr 3
      #define Y_offset 27

void ZoneGraph_Paint(_Screen* ScreenLokal)
    {

        
       List_Paint(Speech[g_leng].Grafika,1,108,0,0,3," ",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      
       char txt[8];
       
       
       
       
      //siatka 
       
      for (char i=0; i<=MaxRowGr; i++)
      {
          H_line(3,Y_offset+i*20,58,2);   
      }
      for (char i=0; i<=MaxColGr; i++)
      {
          V_line(1+i*20,Y_offset,80,2);         
      }
      //------koniec siatki

      //zaznaczenie wybranego
      char RowChos = MaxRowGr-ScreenLokal->Graph[ZoneGrapfNrClick].Type/3-1;
      char ColChos = ScreenLokal->Graph[ZoneGrapfNrClick].Type-((ScreenLokal->Graph[ZoneGrapfNrClick].Type/3)*3);
      H_line(1+ColChos*20  ,Y_offset+RowChos*20,20,1);  
      H_line(1+ColChos*20    ,Y_offset+(RowChos+1)*20,20,1); 
      V_line(1+ColChos*20    ,Y_offset+RowChos*20,20,1);  
      V_line(1+(ColChos+1)*20    ,Y_offset+RowChos*20,20,1);  
      //---------------------
      
      
      
       if (((ScreenLokal->Graph[ZoneGrapfNrClick].Type==GraphText) || (ScreenLokal->Graph[ZoneGrapfNrClick].Type==GraphOnOff)) && (ScreenLokal->Graph[ZoneGrapfNrClick].PlusOnOff))
        {
         Label("+",2,5+2*20, Y_offset+17 ,1,12,1,127,0);

        }
        
      
        char idx=0;

        for (int j=(MaxRowGr-1); j>=0; j--)
        {
          for (int i=0; i<MaxColGr; i++)
          {
            if (idx<=MaxGraphIdx) CopyToCanvasPoint((char*)&GraphTab[idx], 2+i*20, Y_offset+1+j*20, 18,18);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);           
            if (idx==MaxGraphIdx+1) {
              Label("Text",0,2 +2*20, Y_offset+6+j*20 ,4,10,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
            }
            idx++;
          }
        }
        
        
        
        
        Label("Scn ",0,4 +0*20, Y_offset+7 ,3,10,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
        Label("Val ",0,5 +1*20, Y_offset+7 ,3,15,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
        //Label("Text",0,2 +2*20, Y_offset+7 ,4,10,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
        Label(Speech[g_leng].Wl,0,6 +2*20,  Y_offset+9,3,10,1,127,0);
        Label(Speech[g_leng].Wyl,0,6 +2*20, Y_offset+1,3,10,1,127,0);
        
        //je¿eli On / Off
        if (ScreenLokal->Graph[ZoneGrapfNrClick].Type==GraphOnOff) 
        {
          
          if ((ScreenLokal->Graph[ZoneGrapfNrClick].Nr>=0) && ( (ScreenLokal->Graph[ZoneGrapfNrClick].Nr!=0xFF)))
          {
            txt[0]='I'; txt[1]='d'; txt[2]=':'; txt[3]=' ';
            char ind=CharToStr(ScreenLokal->Graph[ZoneGrapfNrClick].Nr, &(txt[3]), 2); //(char Value, char *Vstring, char ill);
            Label(txt,2,20, 16 ,ind+3,22,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          
             txt[0]='P'; txt[1]=':'; txt[2]=0; 
            ind=CharToStr(ScreenLokal->Graph[ZoneGrapfNrClick].Port, &(txt[2]), 2); //(char Value, char *Vstring, char ill);
            Label(txt,2,20, 3 ,ind+2,22,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          }else{
            txt[0]='V'; txt[1]='a'; txt[2]='r'; txt[3]=':'; txt[4]=' ';
            Label(txt,2,20, 16 ,4,22,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          
             txt[0]='b'; txt[1]=' '; 
            char ind=CharToStr(ScreenLokal->Graph[ZoneGrapfNrClick].Port, &(txt[1]), 2); //(char Value, char *Vstring, char ill);
            Label(txt,2,20, 3 ,ind+1,22,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          
          }
          
          
          
          txt[0]='<';txt[1]=0;
          Button(txt,3,15,15,11); 
          txt[0]='>';
          Button(txt,45,15,15,11); 
          
         
          txt[0]='<';txt[1]=0;
          Button(txt,3,3,15,11); 
          txt[0]='>';
          Button(txt,45,3,15,11); 
          
          
        }        
        
        //je¿eli tekst
        if (ScreenLokal->Graph[ZoneGrapfNrClick].Type==GraphText) 
        {

          if (ScreenLokal->Graph[ZoneGrapfNrClick].Text[0]!=0)
          {
            Label(ScreenLokal->Graph[ZoneGrapfNrClick].Text,0,31, 17 ,6,10,1,127,0); 
          }else{
            Label("...... ",0,31, 17 ,6,10,1,127,0); 
          }
          
          Button("Text",2,15,25,11); 
          
          Label("+On/Off",0,3, 4 ,7,10,1,127,0); 
          
          if (ScreenLokal->Graph[ZoneGrapfNrClick].PlusOnOff) 
          {
            txt[0]='O'; txt[1]='f';txt[2]='f'; txt[3]=0;
          }else{
            txt[0]='O'; txt[1]='n'; txt[2]=0;
          }
          
          Button(txt,40,3,21,11); 
          
          
        }
        

             
             
        //je¿eli wartoœæ
        if (ScreenLokal->Graph[ZoneGrapfNrClick].Type==GraphWartosc) 
        {
          for (char i=0; i<3; i++) 
          {
           if (ScreenLokal->Graph[ZoneGrapfNrClick].Text[i]>=30) 
           {Label(&(ScreenLokal->Graph[ZoneGrapfNrClick].Text[i]),2,30-((3-i)*9),17,1,7,1,127,1); 
           }else{
            Label(" ",2,30-((3-i)*9),17,1,8,1,127,1);  
           }
          }
          
          txt[0]='N'; txt[1]=':'; txt[2]=0;
          Label(txt,0,33, 17 ,2,10,1,127,0); 
          txt[1]=0;txt[2]=0;
          char ind=CharToStr(ScreenLokal->Graph[ZoneGrapfNrClick].Nr, txt, 2); //(char Value, char *Vstring, char ill);
          txt[ind++]='.';
          ind+=CharToStr(ScreenLokal->Graph[ZoneGrapfNrClick].Reg, &txt[ind], 2); //(char Value, char *Vstring, char ill);
          Label(txt,0,42, 17 ,ind,10,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
        
          Button(Speech[g_leng].Zmiana,8,3,44,13); 
   
        }
        
        if (ScreenLokal->Graph[ZoneGrapfNrClick].Type==GraphScena) 
        {
          Label(Speech[g_leng].NumerSceny,0,2, 12 ,15,10,1,127,0); 
        }
        
        
      }//________________________________________________________________________

    


 void ZoneGraph_Event(_Screen* ScreenLokal)
      {
           char ButonClickNr=0;
        //zdarzenie typu down "X"
        if ((X_down>46) && (X_down<63) && (Y_down>112) && (Y_down<127))    //"X"
        {
          //MenuPaint();
          H_line(49,114,10,0);
          H_line(49,123,10,0); 
          V_line(49,114,10,0); 
          V_line(58,114,10,0); 
          if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}
          
        }
  
           if ((X_up>46) && (X_up<63) && (Y_up>112) && (Y_up<127))                    //"X"
        {
            if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}
            MenuLevel=(MenuLevel/10);
            MenuPaint();
        }  
        else
        {

        
        //wybór grafiki
        char idx=0;
        char click=0;

        for (int j=(MaxRowGr-1); j>=0; j--)
        {
          for (int i=0; i<MaxColGr; i++)
          {
              
              if ((X_up>1+i*20) && (X_up<1+(i+1)*20) && (Y_up>Y_offset+j*20) && (Y_up<Y_offset+1+(j+1)*20))                    //"X"
              {

                if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}

                ScreenLokal->Graph[ZoneGrapfNrClick].Type=idx;
                click=1;
                ProgramChange=1;
                MenuPaint();
               
              }
              idx++;
            
          }
          
        }
        

  
     
        if (click)
        {

        
        if ((ScreenLokal->Graph[ZoneGrapfNrClick].Type==GraphText) )
        {
          ProgramChange=1;

          MenuPaint(); 
        }
        
        }
        else
        {
        
       
 
        //je¿eli On / Off
        if (ScreenLokal->Graph[ZoneGrapfNrClick].Type==GraphOnOff) 
        {
          char txt[2];
          char Repaint=0;
          txt[0]='<';txt[1]=0;
          if (Button(txt,3,15,15,11)==1) { if((ScreenLokal->Graph[ZoneGrapfNrClick].Nr>=0) && (ScreenLokal->Graph[ZoneGrapfNrClick].Nr!=0xFF)) ScreenLokal->Graph[ZoneGrapfNrClick].Nr--; else ScreenLokal->Graph[ZoneGrapfNrClick].Nr=NumberNeurons-1; Repaint=1;}  
          txt[0]='>';
          if (Button(txt,45,15,15,11)==1) { if(ScreenLokal->Graph[ZoneGrapfNrClick].Nr+1<NumberNeurons) ScreenLokal->Graph[ZoneGrapfNrClick].Nr++; else ScreenLokal->Graph[ZoneGrapfNrClick].Nr=0; Repaint=1;} 
         
          
          if ((ScreenLokal->Graph[ZoneGrapfNrClick].Nr>=0) && (ScreenLokal->Graph[ZoneGrapfNrClick].Nr!=0xFF)) //je¿eli wejœcie
          {
            if (ScreenLokal->Graph[ZoneGrapfNrClick].Port>(NumberNeuronsPort-1)) ScreenLokal->Graph[ZoneGrapfNrClick].Port=NumberNeuronsPort-1;
            
            txt[0]='<';txt[1]=0;
            if (Button(txt,3,3,15,11)==1) { if(ScreenLokal->Graph[ZoneGrapfNrClick].Port>0) ScreenLokal->Graph[ZoneGrapfNrClick].Port--; else ScreenLokal->Graph[ZoneGrapfNrClick].Port=NumberNeuronsPort-1; Repaint=1;}  
            txt[0]='>';
            if (Button(txt,45,3,15,11)==1) { if(ScreenLokal->Graph[ZoneGrapfNrClick].Port+1<NumberNeuronsPort) ScreenLokal->Graph[ZoneGrapfNrClick].Port++; else ScreenLokal->Graph[ZoneGrapfNrClick].Port=0; Repaint=1;} 
          }else{   //je¿eli zmienna
            txt[0]='<';txt[1]=0;
            if (Button(txt,3,3,15,11)==1) { if(ScreenLokal->Graph[ZoneGrapfNrClick].Port>0) ScreenLokal->Graph[ZoneGrapfNrClick].Port--; else ScreenLokal->Graph[ZoneGrapfNrClick].Port=99; Repaint=1;}  
            txt[0]='>';
            if (Button(txt,45,3,15,11)==1) { if(ScreenLokal->Graph[ZoneGrapfNrClick].Port+1<99) ScreenLokal->Graph[ZoneGrapfNrClick].Port++; else ScreenLokal->Graph[ZoneGrapfNrClick].Port=0; Repaint=1;} 
          
          }
          
          if (Repaint) MenuPaint(); 
        } 
           
           
          if (ScreenLokal->Graph[ZoneGrapfNrClick].Type==GraphWartosc) 
          {
             if (Button(Speech[g_leng].Zmiana,8,3,44,13)==1) ButonClickNr=1;
          }
          
          if ((ScreenLokal->Graph[ZoneGrapfNrClick].Type==GraphText))
          {
            if (Button("Text",2,15,25,11)==1) ButonClickNr=2;
            if (ScreenLokal->Graph[ZoneGrapfNrClick].PlusOnOff) 
            {
              if (Button(Speech[g_leng].Wyl,40,3,21,11)==1) {ScreenLokal->Graph[ZoneGrapfNrClick].PlusOnOff=0;
              ButonClickNr=3;}
            }else{
              if (Button(Speech[g_leng].Wl,40,3,21,11)==1) {ScreenLokal->Graph[ZoneGrapfNrClick].PlusOnOff=1;
              ButonClickNr=3;}
            }
          
             
          }    
          
           if (ButonClickNr==1) 
              {
                ProgramChange=1;
                AnalogToChange=(_Analog*)&ScreenLokal->Graph[ZoneGrapfNrClick];
                MenuLevel=(MenuLevel*10+2); 
                MenuPaint(); 
          
              }
        
            if (ButonClickNr==2) 
            {
              ProgramChange=1;
            TextToChangeLength=3;//TextToChangeMaxPixSize
            TextToChange=ScreenLokal->Graph[ZoneGrapfNrClick].Text;
            MenuLevel=(MenuLevel*10+1); 
            MenuPaint(); 
            }
            if (ButonClickNr==3) 
            {
              MenuPaint(); 
            }
           
        
        
        }
        
        }
      }//________________________________________________________________________
    



 
 
#define MaxLetherOnScr    24 
 
 
char TextChangeCharNr=0;
char TextChangePoz=0;

const char QWERTY[3][MaxLetherOnScr]={
              {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','W','Y','Z'}, 
              {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','w','y','z'}, 
              {' ','0','1','2','3','4','5','6','7','8','9','+','-','_','<','>','.','x','/',127,30,31,35,36},
              };

//* text- wskaŸnik do stringa
//* MaxChar - maksymalna iloœæ znaków tekstu. Maksymalnie 6. Je¿eli MaxChar=0 to domyœlnie MaxChar=6;
char AlfaType=0;

void TextChange_Paint(char* text, char* MaxChar)
      {  
        List_Paint("Text ",1,108,0,0,3," ",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      
        V_line(62,98,15,0);
        
        char idx=0;
        char y=86;

        char PrzesZnak=0;
        if (TextChangePoz>12) PrzesZnak=TextChangePoz-12;
        
        H_line(1,109,62,0);
        
        char DlTxt=0; //d³ugoœæ tekstu
        while ((DlTxt<*MaxChar) && (text[DlTxt]>0)) DlTxt++;
        Label(&text[PrzesZnak],0,2,100,DlTxt-PrzesZnak,11,1,127,0); 
        
         for (char j=0; j<6; j++) 
         {
            for (char i=0; i<4; i++) 
            {
              if (QWERTY[AlfaType][idx]>0)
              {
                Rectangle(2+i*15,y,14,13);
                Label((char*)&QWERTY[AlfaType][idx],2,3+i*15,y+2,1,11,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
                idx++;
              }
            }
            y-=14;
         }
        

        Label("C",2,3,4,1,11,1,127,1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
        Label("<",2,3+1*15,4,1,11,1,127,1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
        Label(">",2,3+2*15,4,1,11,1,127,1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
        Label("...",2,3+3*15,4,3,11,1,127,1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 

        
        
        V_line(63,98,15,1);
        
 }
        
        
void TextChange_Refresh(char* text, char* MaxChar)
{
  char DlTxt; //d³ugoœæ tekstu
  char Pix;  //piksel
  char offset=30;
  //while ((DlTxt<MaxChar) && (text[DlTxt]>0)) DlTxt++;
  char PrzesZnak=0;
        if (TextChangePoz>12) PrzesZnak=TextChangePoz-12;
  
  static char count=0;
  static char mig=0;
  if (count<8) 
  {
    count++;
  }else{
      count=0;
      if (mig) mig=0; else mig=1;  
      Pix=1;
      for (char Znak=0; Znak<TextChangePoz-PrzesZnak; Znak++)
      {
         Pix += Arial_7[text[Znak+PrzesZnak]-offset][0];
         Pix+=1; //przerwa pomiêdzy znakami
      }
      
      if (mig) 
          {
            V_line(Pix,100,9,1);
          }else{ 
            V_line(Pix,100,9,0);
          }
      
      
  }
  
 
}
 

long long int LastText_EventTime=0;
void TextChange_Evant(char* text, char* MaxChar, char* MaxPix)
      {  
        char y=86;
        char Repaint=0;
        //zdarzenie typu down "X"
        if ((X_down>46) && (X_down<63) && (Y_down>112) && (Y_down<127))    //"X"
        {
          //MenuPaint();
          H_line(49,114,10,0);
          H_line(49,123,10,0); 
          V_line(49,114,10,0); 
          V_line(58,114,10,0); 
          if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}
          
        }
  
           if ((X_up>46) && (X_up<63) && (Y_up>112) && (Y_up<127))                    //"X"
        {
          if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}

          char LastChar=0;
          for (char i=0; i<*MaxChar; i++) if ((text[i]>=30) &&  (text[i]<=129) && (text[i]!=32)) LastChar=i;
          for (char i=LastChar+1; i<*MaxChar; i++) text[i]=0;

          TextChangeCharNr=0;
          TextChangePoz=0;
          TextToChangeMaxPixSize=0;
          MaxChar=0;
          Rej_poz=0;
          
          MenuLevel=(MenuLevel/10);
          MenuPaint();
        }  
        else
        {
          Label("<",2,3+1*15,4,1,11,1,127,1);
          
          if ((X_up>3) && (X_up<62) && (Y_up>4) &&  (Y_up<108)) Repaint=1;
          
          //przyciski funkcyjne
          char FunkButt=0;
          for (char i=0; i<4; i++)
          {
            if ((X_down>3+i*15) && (X_down<3+i*15+15) && (Y_down>4) &&  (Y_down<4+9)) 
            {
              MenuPaint();
              Rectangle(2+i*15,3,14,11);
              
            }
            
            if ((X_up>3+i*15) && (X_up<3+i*15+15) && (Y_up>4) &&  (Y_up<4+9))
            {
              FunkButt=i+1;
              if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}
            }
          }
          
          //obliczenie d³ugoœci tekstu
          char TextLength=0;
          while ((TextLength<*MaxChar) && (text[TextLength]!=0)) TextLength++;

          
           //przycisk zmiany alfabetu
          if ((FunkButt==1) && (TextChangePoz>0)) 
          {
            for (char i=TextChangePoz; i<TextLength; i++)  text[i-1]=text[i];
            TextChangePoz--;
            TextLength--;
            text[TextLength]=0;  
            Repaint=1;
          }
          if (FunkButt==2) {if (TextChangePoz>0) {TextChangePoz--;} Repaint=1;}
          if (FunkButt==3) {if (TextChangePoz<TextLength) {TextChangePoz++;} Repaint=1;}
          if (FunkButt==4) {if (AlfaType<2) AlfaType++; else AlfaType=0; Repaint=1;}
          

         char idx=0;
          for (char j=0; j<6; j++) 
         {
            for (char i=0; i<4; i++) 
            {
              if (QWERTY[AlfaType][idx]>0)
              {
                if ((X_down>3+i*15) && (X_down<3+i*15+15) && (Y_down>y) &&  (Y_down<y+13))
                {
                  MenuPaint();
                  Label((char*)&QWERTY[AlfaType][idx],2,3+i*15,y+2,1,11,1,127,1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
                }
                if ((X_up>3+i*15) && (X_up<3+i*15+15) && (Y_up>y) &&  (Y_up<y+13)) 
                {
                  if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}
                  if (TextChangePoz<*MaxChar)
                  {
                     if ((*MaxPix==0) || ((LabelLength(text,*MaxChar)+LabelLength((char*)&(QWERTY[AlfaType][idx]),1))<*MaxPix ))
                     {
                      text[TextChangePoz]=QWERTY[AlfaType][idx];
                      TextChangePoz++;
                      if ((TextChangePoz==1) && (text[TextChangePoz]==0) && (AlfaType==0)) AlfaType=1;
                     }else{
                      if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+200;} 
                     }
                  }else{
                   if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+200;} 
                  }
                   
                  char AT=1;
                  //for (char AT=0; AT<3; AT++)
                  //{
                      for (char i=0; i<MaxLetherOnScr; i++)
                      {
                        if (text[TextChangePoz]==QWERTY[AT][i]) AlfaType=AT; 
                      }
                  //}
                  
                  Repaint=1;
                }
                idx++;
              }
            }
            y-=14;
         }
          
          
          
         
          

          
        }
        
        
        if (Repaint) MenuPaint();
        
      }






void AnalogGraph_Paint(_Analog *Anal)
      {  
        
        List_Paint("Analog ",1,108,0,0,3," ",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      
        char txt[8];
        
        Button("<>",24,96,15,11);
        
        char Tp=0;
        char Tb=4;
        char Tk=0;
        char Ap=32;
        char Ab=32;
        char Ak=32;
        
        if (Anal->Flip)
        {
           Tp=32;
           Tb=33;
           Tk=29;
           Ap=0;
           Ab=1;
           Ak=6;
        }
        
        V_line(31,10,85,1); 
        
        Label("Text "  ,2,Tp+2 ,86,4,27,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
        Label("Analog",2,Ap+1,86,7,27,1,127,0); 
        
        char LastChar=0;
        char SumOfPixel=1;
        for (char i=0; i<3; i++) if ((Anal->Text[i]>=30) && (Anal->Text[i]!=' ')) 
        {
          LastChar=i+1;
          SumOfPixel+= Arial_7[Anal->Text[i]-30][0]; 
        }
        
        
        H_line(Tk+3,70,27,2); 
        H_line(Tk+3,80,27,2); 
        for (char i=0; i<=3; i++) V_line((Tk+30)-9*i,70,10,2);  
        for (char i=0; i<LastChar; i++) Label(&Anal->Text[i],2,(Tk+30)-((LastChar-i)*9),71,1,8,1,127,1); 

        H_line(Ak,70,4*6,2); 
        H_line(Ak,80,4*6,2); 
        for (char i=0; i<=6; i++) V_line(Ak+4*i,70,11,2); 
        
        
        char LastPixel=29-SumOfPixel;
        char MaxCharTmp=LastPixel/4;
        if (MaxCharTmp>6) MaxCharTmp=6;
        for (char i=0; i<MaxCharTmp; i++)
        {
          V_line(Ak+4*i+1,71,9,1); 
          V_line(Ak+4*i+2,71,9,1); 
          V_line(Ak+4*i+3,71,9,1); 
        }
        
         Button("Text",Tb,50,26,15);
        
        
        txt[0]='I'; txt[1]='d'; txt[2]=':'; txt[3]=0;
          Label(txt,0,Ab+4, 53 ,3,10,1,127,0); 
          txt[1]=0;txt[2]=0;
          char ind=CharToStr(Anal->Nr, txt, 2); //(char Value, char *Vstring, char ill);
          Label(txt,0,Ab+19, 53 ,ind,8,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
                      
          txt[0]='<';txt[1]=0;
          Button(txt,Ab+1,35,13,15); 
          txt[0]='>';
          Button(txt,Ab+16,35,13,15); 
          
          
          txt[0]='R'; txt[1]='e'; txt[2]='g'; txt[3]=':'; txt[4]=0;
          Label(txt,0,Ab+1, 23 ,4,10,1,127,0); 
          txt[1]=0;txt[2]=0;
          
          ind=CharToStr(Anal->Reg, txt, 2); //(char Value, char *Vstring, char ill);
          Label(txt,0,Ab+21, 23 ,ind,8,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
        
          txt[0]='<';txt[1]=0;
          Button(txt,Ab+1,5,13,15); 
          txt[0]='>';
          Button(txt,Ab+16,5,13,15); 
        
        
      }

void AnalogGraph_Event(_Analog *Anal)
      {  
        
       //List_Paint(Speech[g_leng].ZmAnalog,1,108,0,0,3," ",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      
       char txt[8];
       

        char Tb=4;
        char Ab=32;
        
        if (Anal->Flip)
        {
           Tb=33;
           Ab=1;
        }
       
       
               //zdarzenie typu down "X"
        if ((X_down>46) && (X_down<63) && (Y_down>112) && (Y_down<127))    //"X"
        {
          //MenuPaint();
          H_line(49,114,10,0);
          H_line(49,123,10,0); 
          V_line(49,114,10,0); 
          V_line(58,114,10,0); 
          if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}
          
        }
  
           if ((X_up>46) && (X_up<63) && (Y_up>112) && (Y_up<127))                    //"X"
        {
          if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}
          MenuLevel=(MenuLevel/10);
          MenuPaint();
        }  
        else
        {
          
          char ButtonDownNr=0;
          
          
          if (Button("Text",Tb,50,26,15)==1) ButtonDownNr=1;
          txt[0]='>'; txt[1]=0; 
          if (Button(txt,Ab+16,35,13,15)==1) ButtonDownNr=2;
          txt[0]='<';txt[1]=0;
          if (Button(txt,Ab+1,35,13,15)==1) ButtonDownNr=3;
          txt[0]='>'; txt[1]=0; 
          if (Button(txt,Ab+16,5,13,15)==1) ButtonDownNr=4;
          txt[0]='<';txt[1]=0;
          if (Button(txt,Ab+1,5,13,15)==1) ButtonDownNr=5;
          if (Button ("<>",24,96,15,11)==1) ButtonDownNr=6;
          
          if (ButtonDownNr==1)
          {

            ProgramChange=1;
            TextToChangeLength=3;
            TextToChange=Anal->Text;
            MenuLevel=(MenuLevel*10+1); 
            MenuPaint(); 
          }
          
          
          if (ButtonDownNr==2) 
          {
            
              ProgramChange=1;
              if (Anal->Nr+1<NumberNeurons) {Anal->Nr++;} else {Anal->Nr=0;} 
              MenuPaint(); 
          }
           
          
          
          if (ButtonDownNr==3) 
          {
             ProgramChange=1;
             if (Anal->Nr>0) {Anal->Nr--;} else {Anal->Nr=NumberNeurons;}
            MenuPaint(); 
          }
          
          
          if (ButtonDownNr==4) 
          {
            
              ProgramChange=1;
              if (Anal->Reg+1<NumberReg) {Anal->Reg++;} else {Anal->Reg=1;} 
              MenuPaint(); 
          }
           
          
          
          if (ButtonDownNr==5) 
          {
             ProgramChange=1;
             if (Anal->Reg>1) {Anal->Reg--;} else {Anal->Reg=NumberReg;}
            MenuPaint(); 
          }
          
          if (ButtonDownNr==6) 
          {
             if (Anal->Flip) Anal->Flip=0; else Anal->Flip=1;
             ProgramChange=1;

            MenuPaint(); 
          }
          
        }
       
       
      }



      
      
//opcje  **************************************************************************************************************
   const char ProgStopStartLabel[2][15]=
   {
      "Program stop",
      "Program start ",
   };


      //*----------------------------------------------------------------------------
      void Options_Paint(void)
      {  
        

        #ifdef _ADMINISTRATOR_
             List_Paint(Speech[g_leng].Opcje,5,15,0,0,3,Speech[g_leng].RestartSter, ProgStopStartLabel[g_Stop_Program],Speech[g_leng].Zmienne ,"Tester", "Zmiana ID",0,0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])
        #else
          #ifdef _TESTER_
                List_Paint(Speech[g_leng].Opcje,4,15,0,0,3,Speech[g_leng].RestartSter, ProgStopStartLabel[g_Stop_Program], Speech[g_leng].Zmienne, "Tester",0,0,0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])
          #else
                List_Paint(Speech[g_leng].Opcje,3,15,0,0,3,Speech[g_leng].RestartSter, ProgStopStartLabel[g_Stop_Program],Speech[g_leng].Zmienne,0,0,0,0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])
          #endif
        #endif

        
        
        
    
      }//________________________________________________________________________

    

      //*-----------------------------------------------------------------------------
      void Options_Event(void)
      {
        int ans;
        #ifdef _ADMINISTRATOR_
          ans= List_Event(X_up, Y_up, X_down, Y_down, 5 , 15, 0,0);//(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
        #else
          #ifdef _TESTER_
            ans= List_Event(X_up, Y_up, X_down, Y_down, 4 , 15, 0,0);//(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
          #else  
            ans= List_Event(X_up, Y_up, X_down, Y_down, 3 , 15, 0,0);//(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
          #endif
        #endif

        if (ans)
          {
            
              if (ans==1) MenuLevel=(MenuLevel*10+ans);
              if (ans==2) 
                  {if (g_Stop_Program==0) 
                    {g_Stop_Program=1; MenuPaint();} else {g_Stop_Program=0; MenuPaint();}
                  }
              #if (_ADMINISTRATOR_) || (_TESTER_)
              if (ans==3)
                  {
                    MenuLevel=(MenuLevel*10+ans);
                  }
              if (ans==4)
                  {
                     //NeuronIdToChange   =3100008;
                     NeuronIdToChange   =9999999;
                     ChangeingIdMode=1;
                      NeuronNrChange=1;
                      AddRfTask(SI, 1, 0);                //Dodaj rozkaz do tablicy zadañ RF
                     MenuLevel=(MenuLevel*10+ans);
                  }
              #endif
              #ifdef _ADMINISTRATOR_
              if (ans==5)
                  {
                    NeuronNrChange=1;
                     NeuronIdToChange   =9999999;
                     NeuronIdAfterChange=3100000;
                     MenuLevel=(MenuLevel*10+ans);
                  }
               #endif
              if (ans<0) MenuLevel=(MenuLevel/10);
              MenuPaint();
          } 
          

        
      }//________________________________________________________________________
    
            






      //*-----------------------------------------------------------------------
      void ChangeID_Paint(int *NeuronIdToChange, int *NeuronIdAfterChange)
      {  
            char Txt[9];

            List_Paint("New ID",1,108,0,0,3," ",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      

          
           //wyczyœæ listê wêz³ów
           for (char i=0; i<NumberNeurons; i++) 
           {
             NeuronsList[i].NeuronType=NoneNodeType;
             NeuronsList[i].NeuronID=0;
             NeuronsList[i].ReadOften=0;
             NeuronsList[i].ReadRarely=0;
           }
           
           //wyczyœæ zadania
           for (char i=0; i<RF_Tasks-1; i++) 
           {
            RF_TaskTab[i].Node=0;
            RF_TaskTab[i].Command.ID=0;
            RF_TaskTab[i].Command.Order=0;
            RF_TaskTab[i].Command.Auxi=0;
           }
                      
            
           
            
           //dodanie numeru ID do zmiany
           NeuronsList[0].NeuronType=(_NeuronType)(*NeuronIdToChange/1000000);
           NeuronsList[0].NeuronID=(*NeuronIdToChange)-(NeuronsList[0].NeuronType*1000000);
           NeuronsList[0].ReadOften=1;
           
           //dodanie nowego numeru
           NeuronsList[1].NeuronType=(_NeuronType)(*NeuronIdAfterChange/1000000);
           NeuronsList[1].NeuronID=(*NeuronIdAfterChange)-(NeuronsList[1].NeuronType*1000000);
           NeuronsList[1].ReadOften=1;
           
            
            
            
            Label("ID do zmiany",0,1,97,15,62,1,127,1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
            sprintf(&Txt, "%d",  *NeuronIdToChange, 'B');
            Label(Txt,0,10,86,7,40,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw)           

            Button("...",45,81,16,15);
            
            Label("ID po zmianie",0,1,67,15,62,1,127,1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
            sprintf(&Txt, "%d",  *NeuronIdAfterChange, 'B');
            Label(Txt,0,10,56,7,40,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw)           
  
            Button("...",45,51,16,15); 
            
            AddRfTask(SI, 1, 0);                //Dodaj rozkaz do tablicy zadañ RF
            AddRfTask(SI, 2, 0);                //Dodaj rozkaz do tablicy zadañ RF
            
            TransQuality.Mensur[0]=0;
            TransQuality.Mensur[1]=0;
            
            
            
            
            
      }//_______________________________________________________________________ 




      //*------------------------------------------- ----------------------------
      void ChangeID_Event(int *NeuronIdToChange, int*NeuronIdAfterChange)
      {
        int ans;

       
        ans=List_Event(X_up, Y_up, X_down, Y_down, 0 , 15, 0,0); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        char Repaint=0;

        
        
        if (ans)
          {
          if (ans<0) {
            
            
            SetTask(&gProg);   //usuniêcie nie potrzebnie dodanych zadañ
            RefreshNeuronsList(&gProg); 
            MenuLevel=(MenuLevel/10);
            ChangeingIdMode=0;
             Repaint=1;

          }
          
          }else{
          
            //zmien -id pierwotne
            if (Button("...",45,81,16,15)==1)
            {
              NeuronIdChange=NeuronIdToChange;
              MenuLevel=(MenuLevel*10+1);
              Repaint=1;
            }
          
            //zmien na id 
            if (Button("...",45,51,16,15)==1)
            {
              NeuronIdChange=NeuronIdAfterChange;
              MenuLevel=(MenuLevel*10+1);
              Repaint=1;
            }
          
            //przycisk konwertuj
            if ((TransQuality.QuickMensur[0]!=0) && (TransQuality.QuickMensur[1]==0))
            {
              if (Button("Konwertuj",3,10,58,20)==1)
              {
                TransQuality.QuickMensur[1]=0;
                TransQuality.SlowMensur[1]=0; 
                
                AddRfTask(SNewId, 1, 0);                //Dodaj rozkaz do tablicy zadañ RF
                TransQuality.Mensur[0]=0;
                TransQuality.Mensur[1]=0;
              }
            }
            
            
           
          
            
          } 
        
          if (Repaint) MenuPaint();
        
        
      }//_______________________________________________________________________



      

       void ChangeID_Refresh(int *NeuronIdToChange, int*NeuronIdAfterChange)
       {
           static char Dziel5=0;
           if (Dziel5<5)
           {
              Dziel5++;
              return;
           }
           Dziel5=0;
           
        char ERRx1=0;  //iloœæ b³êdów pojedyñczych
        char ERRx2=0;  //iloœæ b³êdów podwójnych (dwukrotny brak komunikacji w czasie TimeUnitQual)
        char ERRx3=0;  //iloœæ b³êdów podtrujnych i wiêkszych (przynajmniej trzykrotny brak komunikacji w czasie TimeUnitQual)
        char CurErr=0;
           
           
            //wyœwietl jakoœæ transmisji starego ID
            char Ypoz=87;  
                       
                      CleanCanvasArea(2,Ypoz-1,8,9,0);
                      for (char b=0; b<32; b++)
                      {
                        if ((TransQuality.Mensur[0]) & (0x1<<b) )  //jezeli poprawna transmisja w tej jednostce czasu
                        {  
                          CurErr=0;
                        }else{
                          CurErr++;
                          if (CurErr==1) {ERRx1++;}
                          if (CurErr==2) {ERRx2++; ERRx1--; }
                          if (CurErr==3) {ERRx3++; ERRx2--; CurErr=0;}
        
                        }
                        
                      }
           
                      //wskaŸnik jakoœci              
                      if (TransQuality.Mensur[0]) 
                      {
                        H_line(5,Ypoz,1,1); // (char X, int Y, char lenght, char typ) 
                        H_line(4,Ypoz+2,3,1); // (char X, int Y, char lenght, char typ)    
                      }
                      if ((ERRx1<=10)  && (ERRx2<=6)  && (ERRx3<=1)) H_line(3,Ypoz+4,5,1); // (char X, int Y, char lenght, char typ)  
                      if ((ERRx1<=4)   && (ERRx2<=1)  && (ERRx3==0)) H_line(2,Ypoz+6,7,1); // (char X, int Y, char lenght, char typ)
           
            
                   
                      if (TransQuality.Mensur[0])  Button("Konwertuj",3,10,58,20);    
                      
                    
                 char NewN=1;
                 if (NeuronsList[0].NeuronID==NeuronsList[1].NeuronID) NewN=0;
                 
                      
              //wyœwietl jakoœæ transmisji nowego ID
                      Ypoz=57;
                      ERRx1=0; 
                      ERRx2=0;  
                      ERRx3=0;  
                      CurErr=0;
                      CleanCanvasArea(2,Ypoz-1,8,9,0);
                      for (char b=0; b<32; b++)
                      {
                        if ((TransQuality.Mensur[NewN]) & (0x1<<b) )  //jezeli poprawna transmisja w tej jednostce czasu
                        {  
                          CurErr=0;
                        }else{
                          CurErr++;
                          if (CurErr==1) {ERRx1++;}
                          if (CurErr==2) {ERRx2++; ERRx1--; }
                          if (CurErr==3) {ERRx3++; ERRx2--; CurErr=0;}
        
                        }
                        
                      }
           
                      //wskaŸnik jakoœci              
                      if (TransQuality.Mensur[NewN]) 
                      {  
                        H_line(5,Ypoz,1,1); // (char X, int Y, char lenght, char typ) 
                        H_line(4,Ypoz+2,3,1); // (char X, int Y, char lenght, char typ)  
                      }
                      if ((ERRx1<=10)  && (ERRx2<=6)  && (ERRx3<=1)) H_line(3,Ypoz+4,5,1); // (char X, int Y, char lenght, char typ)  
                      if ((ERRx1<=4)   && (ERRx2<=1)  && (ERRx3==0)) H_line(2,Ypoz+6,7,1); // (char X, int Y, char lenght, char typ)    
                      
                
                      
              if ((TransQuality.QuickMensur[0]!=0) && (TransQuality.QuickMensur[1]==0))
              {
               Button("Konwertuj",3,10,58,20);
              } 
              else if ((TransQuality.QuickMensur[0]!=0) && (TransQuality.QuickMensur[1]!=0))
              {
                 Label("Konflikt ID",2,3,19,15,58,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
                 Label("po zmianie",2,3,10,15,58,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
              }
              else{
                CleanCanvasArea(3,10,58,20,0); 
                
                if (TransQuality.QuickMensur[1]!=0)
                {
                  Label("Konwersja",2,3,19,15,58,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
                  Label("dokonana",2,3,10,15,58,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
                }else{
                  Label("Nawiazywanie",2,3,19,15,58,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
                  Label("polaczenia ",2,3,10,15,58,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
                }
                
              }        
                      
       }
      
      
      
      
      
      
      
      
      
      


//Testowanie modu³ów
      //*-----------------------------------------------------------------------
      void CheckingModule_Paint(int *NeuronId)
      {  
            char Txt[9];

            List_Paint("TESTER",1,108,0,0,3," ",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      


            
          
            RF_NodeTest.NodeNr=1;
            RF_NodeTest.Sent=0;
            RF_NodeTest.SentSyn=0;   
            RF_NodeTest.Recived=0;
            
           //wyczyœæ listê wêz³ów
           for (char i=0; i<NumberNeurons; i++) 
           {
             NeuronsList[i].NeuronType=NoneNodeType;
             NeuronsList[i].NeuronID=0;
             NeuronsList[i].ReadOften=0;
             NeuronsList[i].ReadRarely=0;
           }
           
           //wyczyœæ zadania
           for (char i=0; i<RF_Tasks-1; i++) 
           {
            RF_TaskTab[i].Node=0;
            RF_TaskTab[i].Command.ID=0;
            RF_TaskTab[i].Command.Order=0;
            RF_TaskTab[i].Command.Auxi=0;
           }
                      
            
            
           //dodanie numeru ID do testowania
           NeuronsList[0].NeuronType=(_NeuronType)(*NeuronId/1000000);
           NeuronsList[0].NeuronID=(*NeuronId)-(NeuronsList[0].NeuronType*1000000);
           NeuronsList[0].ReadOften=1;
           
           //dodanie modu³ów o innych ID dla testu odpornoœci na inne ID
           NeuronsList[1].NeuronType=(_NeuronType)3;
           NeuronsList[1].NeuronID=999998;
           NeuronsList[1].ReadOften=1;
           NeuronsList[2].NeuronType=(_NeuronType)3;
           NeuronsList[2].NeuronID=999997;
           NeuronsList[2].ReadOften=1;
           NeuronsList[3].NeuronType=(_NeuronType)3;
           NeuronsList[3].NeuronID=999996;
           NeuronsList[3].ReadOften=1;

           AddRfTask(SI, 2, 0);                //Dodaj rozkaz do tablicy zadañ RF   
           AddRfTask(SI, 3, 0);                //Dodaj rozkaz do tablicy zadañ RF   
            
           
           AddRfTask(SI, 4, 0);                //Dodaj rozkaz do tablicy zadañ RF 
            
            
            
            Label("ID modulu",0,1,97,15,62,1,127,1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
            sprintf(&Txt, "%d",  *NeuronId, 'B');
            Label(Txt,0,10,86,7,40,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw)           

            Button("...",45,84,16,12);
            

            
            AddRfTask(SI, 1, 0);                //Dodaj rozkaz do tablicy zadañ RF          
            AddRfTask(SI, 2, 0);                //Dodaj rozkaz do tablicy zadañ RF       
            AddRfTask(SI, 3, 0);                //Dodaj rozkaz do tablicy zadañ RF       
            AddRfTask(SI, 4, 0);                //Dodaj rozkaz do tablicy zadañ RF       
            AddRfTask(SI, 5, 0);                //Dodaj rozkaz do tablicy zadañ RF       
            
            
            
            TransQuality.Mensur[0]=0;



            
            
            NeuronNrChange=1;
            
            
      }//_______________________________________________________________________ 




      //*------------------------------------------- ----------------------------
      void CheckingModule_Event(int *NeuronId)
      {
        int ans;

       
        ans=List_Event(X_up, Y_up, X_down, Y_down, 0 , 15, 0,0); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        char Repaint=0;

        
        
        if (ans)
          {
          if (ans<0) {
            
            
            SetTask(&gProg);   //usuniêcie nie potrzebnie dodanych zadañ
            RefreshNeuronsList(&gProg); 
            MenuLevel=(MenuLevel/100);

             Repaint=1;

          }
          
          }else{
          
            //zmien -id pierwotne
            if (Button("...",45,84,16,12)==1)
            {
              NeuronIdChange=NeuronId;
              MenuLevel=(MenuLevel*10+1);
              Repaint=1;
            }
          
            if  (Button("We/Wy",3,51,58,15)==1)
            {
              NeuronIdChange=NeuronId;
              MenuLevel=(MenuLevel*100+21);
              Repaint=1;
            }
          
            if  (Button("We analog.",3,35,58,15)==1)
            {
              NeuronIdChange=NeuronId;
              MenuLevel=(MenuLevel*100+31);
              Repaint=1;
            }
            
            if  (Button("Test MG1",3,19,58,15)==1)
            {
              NeuronIdChange=NeuronId;
              MenuLevel=(MenuLevel*10+4);
              Repaint=1;
            }
            
          } 
        
          if (Repaint) MenuPaint();
        
        
      }//_______________________________________________________________________

      
      
      

       void CheckingModule_Refresh(int *NeuronId)
       {
           static char Dziel5=0;
           if (Dziel5<5)
           {
              Dziel5++;
              return;
           }
           Dziel5=0;
           char txt[15];
           
        char ERRx1=0;  //iloœæ b³êdów pojedyñczych
        char ERRx2=0;  //iloœæ b³êdów podwójnych (dwukrotny brak komunikacji w czasie TimeUnitQual)
        char ERRx3=0;  //iloœæ b³êdów podtrujnych i wiêkszych (przynajmniej trzykrotny brak komunikacji w czasie TimeUnitQual)
        char CurErr=0;
           
           
            //wyœwietl jakoœæ transmisji starego ID
            char Ypoz=87;  
                       
                      CleanCanvasArea(2,Ypoz-1,8,9,0);
                      for (char b=0; b<32; b++)
                      {
                        if ((TransQuality.Mensur[0]) & (0x1<<b) )  //jezeli poprawna transmisja w tej jednostce czasu
                        {  
                          CurErr=0;
                        }else{
                          CurErr++;
                          if (CurErr==1) {ERRx1++;}
                          if (CurErr==2) {ERRx2++; ERRx1--; }
                          if (CurErr==3) {ERRx3++; ERRx2--; CurErr=0;}
        
                        }
                        
                      }
           
                      //wskaŸnik jakoœci              
                      if (TransQuality.Mensur[0]) H_line(5,Ypoz,1,1); // (char X, int Y, char lenght, char typ) 
                      if ((ERRx1<=12)  && (ERRx2<=10)  && (ERRx3<=5)) H_line(4,Ypoz+2,3,1); // (char X, int Y, char lenght, char typ)    
                      if ((ERRx1<=10)  && (ERRx2<=6)  && (ERRx3<=1)) H_line(3,Ypoz+4,5,1); // (char X, int Y, char lenght, char typ)  
                      if ((ERRx1<=4)   && (ERRx2<=1)  && (ERRx3==0)) H_line(2,Ypoz+6,7,1); // (char X, int Y, char lenght, char typ)
           
            
                      
                      
                      
               if (TransQuality.Mensur[0])
              { 
                  unsigned short Sent=RF_NodeTest.SentSyn; 
                  unsigned short Recived=RF_NodeTest.Recived; 
               
                 
                  sprintf(txt, "%d", Recived);
                  Label("Ode:",0,3,66,10,30,1,127,0);  
                  Label(txt,0,27,66,10,34,1,127,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
                               
                  sprintf(txt, "%d", Sent);
                  Label("Wys:",0,3,75,10,30,1,127,0); 
                  Label(txt,0,27,75,10,34,1,127,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
                  
                  Button("We/Wy",3,51,58,15);
                  Button("We analog.",3,35,58,15);
                  Button("Test MG1",3,19,58,15);
                  Button("Test MR1.",3,3,58,15);
                  
              }else{           
                 
                RF_NodeTest.Sent=0;
                RF_NodeTest.SentSyn=0;   
                RF_NodeTest.Recived=0;
              }

                      
       }
      
      
      
 
      
      
      
      
            
      
      
      
      

//Ustawienia ******************************************************************************************************************
char RF_Channel_nr_tmp;


      //*----------------------------------------------------------------------------
      void Setup_Paint(void)
      {  
         char *MS="Slave";
         /*
         if (AT91C_IFLASH_MEM->Config.SlaveMode)
         {
           MS="Master";    
         }else{
           MS="Slave";
         } 
         */
        
         if (!AT91C_IFLASH_MEM->Config.SlaveMode) 
          {
            List_Paint(Speech[g_leng].Ustawienia,7,13,0,0,0,Speech[g_leng].Jezyk,Speech[g_leng].DataCzas,Speech[g_leng].Hasla,Speech[g_leng].Ekran,Speech[g_leng].Dzwiek, MS, Speech[g_leng].RfChannel,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char back, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])
          }else{
            List_Paint(Speech[g_leng].Ustawienia,6,13,0,0,0,Speech[g_leng].Jezyk,Speech[g_leng].DataCzas,Speech[g_leng].Hasla,Speech[g_leng].Ekran, Speech[g_leng].Dzwiek,MS,0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char back, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])
          }
          RF_Channel_nr_tmp=RF_Status.Ch;
         
         //znacznik dzwieku
         if (!(AT91C_IFLASH_MEM->Config.SoundOff))
              {
                    
                char ekran_tab[10] =  {0,4,4,10,11,16,16,32,64,0};
                CopyToCanvasPoint((char *)ekran_tab,45,99-(13*(4)),8,10); 
              }
                  //znacznik dzwieku
         if (AT91C_IFLASH_MEM->Config.SlaveMode)
              {
                    
                char ekran_tab[10] =  {0,4,4,10,11,16,16,32,64,0};
                CopyToCanvasPoint((char *)ekran_tab,45,99-(13*(5)),8,10); 
              }
         
      }//________________________________________________________________________



      //*-----------------------------------------------------------------------
      void Setup_Event(void)
      {
        int ans;
        char repaint=0;
        if (!AT91C_IFLASH_MEM->Config.SlaveMode) 
          {
            ans=List_Event(X_up, Y_up, X_down, Y_down, 7 , 13, 0,0); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
          }else{
            ans=List_Event(X_up, Y_up, X_down, Y_down, 6 , 13, 0,0); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
          }
        if (ans>0)
          {

                if (ans==5)
                {
                  char SoundOff;
                  if (AT91C_IFLASH_MEM->Config.SoundOff)
                    {
                      SoundOff=0;
                    }else{
                      SoundOff=1; 
                    }
                  
                  //zapis do flash
                    unsigned int Save = __get_interrupt_state();  
                    __disable_interrupt();
                    FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.SoundOff)), (char*) (&(SoundOff)), 1);
                    __set_interrupt_state(Save);
                    __enable_interrupt();
                    //koniec zapisu do flash
                   
                } else if (ans==6)
                {
                    char SlaveModeTmp;
                    if (AT91C_IFLASH_MEM->Config.SlaveMode)
                    {
                      SlaveModeTmp=0;
                    }else{
                      SlaveModeTmp=1;
                    }
                    SlaveMode=SlaveModeTmp;
                    
                    //zapis do flash
                    unsigned int Save = __get_interrupt_state();  
                    __disable_interrupt();
                    //at91flashWrite((int*)(&(AT91C_IFLASH_MEM->Config.SlaveMode)),0, &SlaveMode,1);
                    FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.SlaveMode)), (char*) (&(SlaveModeTmp)), 1);
                    __set_interrupt_state(Save);
                    __enable_interrupt(); 
                    //koniec zapisu do flash
                    
                    if (!AT91C_IFLASH_MEM->Config.SlaveMode) //je¿eli master 
                    {
                      RF_Status.MasterMode=1;
                      _RF_EnableRX(); 
                     for (char i=0; i<NumberNeurons; i++) TransQuality.Mensur[i]=0; 
                    }else{
                      RF_Status.MasterMode=0; 
                    }
                       AT91PS_MEM pAT91C_IFLASH_MEM=AT91C_IFLASH_MEM;
                      RF_Status.Ch=pAT91C_IFLASH_MEM->Config.RF_channel;
                      SPI_SendWord(0xA000 | RF_SetFreqReg(RF_Status.Ch),Cs_RF); 
                      while (RF_CheckChannel())
                      {
                        if (RF_Status.Ch<RF_ChannelQuanti-1) {RF_Status.Ch++;} else {RF_Status.Ch=1;}
                        SPI_SendWord(0xA000 | RF_SetFreqReg(RF_Status.Ch),Cs_RF); 
                      } 
                     
                   repaint=1;
                }else{
                    repaint=1;
                    MenuLevel=(MenuLevel*10+ans);
                  
                }
          
          }else if (ans<0){
            MenuLevel=(MenuLevel/10);
            repaint=1;
          } 
        
         if (repaint) MenuPaint();
      }//_______________________________________________________________________



//Ustawienia jêzyka******************************************************************************************************************

      //*----------------------------------------------------------------------------
      void SetupLang_Paint(void)
      {  
          //char Y=3;
          int Y_tmp = 3;   //przesuniêcie w dó³

          List_Paint("Jezyk",2,15,Y_tmp,7,0,Speech[g_leng].Polski, Speech[g_leng].Angielski,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])
      
          
          char ekran_tab[10] =  {0,4,4,10,11,16,16,32,64,0};
       
        
          CopyToCanvasPoint((char *)ekran_tab,45,97-Y_tmp-(15*(g_leng)),8,10); 
          
      }//________________________________________________________________________



      //*-----------------------------------------------------------------------
      void SetupLang_Event(void)
      {
        int ans;
 
        ans=List_Event(X_up, Y_up, X_down, Y_down, 2 , 15, 7,3); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        if (ans)
          {
            if ((ans>0) && (ans<3))
            {
              g_leng=ans-1;
              MenuLevel=(MenuLevel/10);
              MenuPaint();
            }
          } 
        else
          {
            if ((X_up>0) &&(Y_up>0)) 
            {
              MenuLevel=(MenuLevel/10);
              MenuPaint();
            }
          }
      }//_______________________________________________________________________







//Ustawienia hasla wybór poziomu******************************************************************************************************************

      //*----------------------------------------------------------------------------
      void SetupPass_Paint(void)
      {  

  
           
          if (g_NeedPass==1)
          {
            List_Paint(Speech[g_leng].Hasla     ,2,15,10,0,0,Speech[g_leng].HasloWl, Speech[g_leng].ZmHasla,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])
            char ekran_tab[10] =  {0,4,4,10,11,16,16,32,64,0};  
            CopyToCanvasPoint((char *)ekran_tab,50,97-(10),8,10); 
          }else
          {
            List_Paint(Speech[g_leng].Hasla     ,1,15,10,0,0,Speech[g_leng].HasloWl, 0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12]) 
          }
        
      }//________________________________________________________________________



      //*-----------------------------------------------------------------------
      void SetupPass_Event(void)
      {
        int ans;

        if (g_NeedPass==1)
          {  
            ans=List_Event(X_up, Y_up, X_down, Y_down, 2 , 15, 0,10); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
          }else{
            ans=List_Event(X_up, Y_up, X_down, Y_down, 1 , 15, 0,10); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
          }
        
        if (ans)          {
            if (ans==1)
            
              
            if (g_NeedPass) 
            {
              char NeedPassFlag=0xEC;
              g_NeedPass=0; 
              //zapis do flash
              unsigned int Save = __get_interrupt_state();  
              __disable_interrupt();
              FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.NeedPass1)), (char*) (&(NeedPassFlag)), 1);
              __set_interrupt_state(Save);
              __enable_interrupt(); 
              //koniec zapisu do flash
            }else
            {
              g_NeedPass=1;
              char NeedPassFlag=0xEB;
              //zapis do flash
              unsigned int Save = __get_interrupt_state();  
              __disable_interrupt();
              FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.NeedPass1)), (char*) (&(NeedPassFlag)), 1);
              __set_interrupt_state(Save);
              __enable_interrupt(); 
              //koniec zapisu do flash
            }
             
          
            if (ans==2)
            {
               MenuLevel=(MenuLevel*10+1);
            }
            
            
            MenuPaint();
          }
          if (ans<0)
          {
            if ((X_up>0) &&(Y_up>0)) 
            {
              MenuLevel=(MenuLevel/10);
              MenuPaint();
            }
          }
      }//_______________________________________________________________________


      




//Wpisanie obecnego has³a  poziom 1/2**************************************************************************************************************
      char InsertOldPass1[9]={0,0,0,0,0,0,0,0,0};
    //*----------------------------------------------------------------------------
    void SetupOldPass_Paint()
    {  
          Number_Paint(Speech[g_leng].Wpisz,"C","E");
          
          Label(Speech[g_leng].ObecneHaslo,0,3,102,15,58,0,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
    
          if (InsertOldPass1[8])  Label(InsertOldPass1,2,4,90,8,55,0,127,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 

          
    }//________________________________________________________________________


    
    //*-----------------------------------------------------------------------------
    void SetupOldPass_Event(char level)
    {
        int ans;
        char roz=0;

        ans=Number_Event(X_up, Y_up, X_down, Y_down); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        if (ans>0)
          {
            
            //MenuPaint();
            if (InsertOldPass1[8]<=7) 
            { 
                if ((ans<10) || (ans==11)) 
                {
                  if (ans<10) InsertOldPass1[InsertOldPass1[8]]=ans+48;
                  if (ans==11) InsertOldPass1[InsertOldPass1[8]]=ans+37;
                  InsertOldPass1[8]++;
                  
                }
                
            }
            
            if ((ans==10) && (InsertOldPass1[8]>0) ) 
            {InsertOldPass1[8]--; 
            InsertOldPass1[InsertOldPass1[8]]=0;
            }
            
            if (ans==12)
            {
              roz=0;
              for (int i=0; i<=7; i++)
              {
                  if ((g_Password_1[i]!=InsertOldPass1[i]) && (level==1)) roz=1;
                  if ((g_Password_2[i]!=InsertOldPass1[i]) && (level==2)) roz=1;
              }
              
              //has³o programisty
              if (roz)
              {
                roz=0;
                char MHS[]="060480";
                  for (int i=0; i<6; i++)
                  {
                      if ((MHS[i]!=InsertPass1[i]) && (level==1)) roz=1;
                      if ((MHS[i]!=InsertPass1[i]) && (level==2)) roz=1;
                  }              
              }
              
              if (roz==0)
              {
                 MenuLevel=(MenuLevel*10+1);
                 for (int i=0; i<=7; i++) InsertOldPass1[i]=0;
                 InsertOldPass1[8]=0;
                 MenuPaint();
              } 
              else
              {
                 //informacja o b³ednym haœle
                Message_Paint(2,"",Speech[g_leng].Bledne,Speech[g_leng].haslo,"","");
              }
     
            }
            
            if ((ans>=1) && (ans<=11)) 
            {
              MenuPaint();
              Label(InsertOldPass1,2,4,90,InsertOldPass1[8],55,0,127,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
            }
            
            
          }
        if (ans<0)
          {
              MenuLevel=(MenuLevel/100);
              MenuPaint();
              for (int i=0; i<=7; i++) InsertOldPass1[i]=0;
              InsertOldPass1[8]=0;
          }
     
    }//________________________________________________________________________

    


//Wpisanie nowego has³a  poziom 1**************************************************************************************************************
    char InsertNewPass1[9]={0,0,0,0,0,0,0,0,0};
    //*----------------------------------------------------------------------------
    void SetupNewPass_Paint(void)
    {  
          Number_Paint(Speech[g_leng].Wpisz,"C","E");
          
          Label(Speech[g_leng].NoweHaslo,0,3,102,15,58,0,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
    
          if (InsertNewPass1[8])  Label(InsertNewPass1,2,4,90,8,55,0,127,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 

          
    }//________________________________________________________________________



    //*-----------------------------------------------------------------------------
    void SetupNewPass_Event(char level)
    {
        int ans;
        ans=Number_Event(X_up, Y_up, X_down, Y_down); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        if (ans>0)
          {
            //MenuPaint();
            if (InsertNewPass1[8]<=7) 
            { 
                if ((ans<10) || (ans==11)) 
                {
                  if (ans<10) InsertNewPass1[InsertNewPass1[8]]=ans+48;
                  if (ans==11) InsertNewPass1[InsertNewPass1[8]]=ans+37;
                  InsertNewPass1[8]++;
                }
                
            }
            if ((ans==10) && (InsertNewPass1[8]>0) ) {InsertNewPass1[8]--; InsertNewPass1[InsertOldPass1[8]]=0;}
            //Label(InsertNewPass1,2,4,90,InsertNewPass1[8],55,0,127,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
            
            
            
            
            
            if ((ans==12) && (InsertNewPass1[8]>0))
            {
              
              for (int i=0; i<=7; i++)
              {
                  if (level==1) g_Password_1[i]=InsertNewPass1[i];
                  if (level==2) g_Password_2[i]=InsertNewPass1[i];
                  InsertNewPass1[i]=0;
              }
              InsertNewPass1[8]=0;

              //zapis has³a do flash
              unsigned int Save = __get_interrupt_state();  
              __disable_interrupt();
              //at91flashWrite((int*)(&(AT91C_IFLASH_MEM->Config.Pass1[0])),0,g_Password_1,9);
              FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.Pass1[0])), (char*) (&(g_Password_1)), 1);
              __set_interrupt_state(Save);
              __enable_interrupt(); 
              //koniec zapisu do flash
              

              
                 //informacja o zmianie hasla
               MenuLevel=(MenuLevel/1000);
                if (level==1) Message_Paint(2,Speech[g_leng].Nowe,Speech[g_leng].haslo,"------------",g_Password_1,"");
                if (level==2) Message_Paint(2,Speech[g_leng].Nowe,Speech[g_leng].haslo,"------------",g_Password_2,"");
            
              
              //MenuPaint();
            } 
            
            
            if ((ans>=1) && (ans<=11)) 
            {
              MenuPaint();
              Label(InsertNewPass1,2,4,90,InsertNewPass1[8],55,0,127,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
            }
            
            
           }
          
        if (ans<0)
          {
              MenuLevel=(MenuLevel/100);
              MenuPaint();
              for (int i=0; i<=7; i++) InsertNewPass1[i]=0;
              InsertNewPass1[8]=0;
          }
      
    }//________________________________________________________________________


    
    
    

    
    
    
    

//Ustawienia ******************************************************************************************************************


    


    
//*----------------------------------------------------------------------------
void ScreenSetup_Paint(void)
{  
    List_Paint(Speech[g_leng].Ekran,3,13,0,0,0,Speech[g_leng].Jasnosc,Speech[g_leng].ScreenSever,Speech[g_leng].Dotyk,0,0,0,0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char back, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])
}//________________________________________________________________________

    
    
//*-----------------------------------------------------------------------
void ScreenSetup_Event(void)
      {
        int ans;
        ans=List_Event(X_up, Y_up, X_down, Y_down, 3 , 13, 0,0); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)

        if (ans>0)
          {
            MenuLevel=(MenuLevel*10+ans);
            MenuPaint();
          
          }else if (ans<0){
            MenuLevel=(MenuLevel/10);
            MenuPaint();
          } 
        
      }//_______________________________________________________________________



    
    
//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    SetTouchScreen_Paint
//* Funkcja sustawienia ekranu dotykowego
//*-----------------------------------------------------------------------------
//*-----------------------------------------------------------------------------
char SetTouchStep=0;


void SetTouchScreen_Paint(void)
{

if (SetTouchStep==0)
{
         korekta_X.A=1;
        korekta_X.B=0;
        korekta_Y.A=1;
        korekta_Y.B=0; 
}
  
if (SetTouchStep==MaxTouchTest)
{
      H_line(0,15,127,1);
      H_line(0,45,127,1);
      H_line(0,75,127,1);
      H_line(0,110,127,1);
      V_line(15,0,127,1);
      V_line(32,0,127,1);
      V_line(50,0,127,1);
}  
else
{
    CleanCanvas();
  
    Label(Speech[g_leng].Dotknij,    2,1,70,15,62,1,128,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
    Label(Speech[g_leng].precyzyjnie,2,1,60,15,62,1,128,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
    Label(Speech[g_leng].wskaznik,   2,1,50,15,62,1,128,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 

     CanvasPixel(TouchTestSor[SetTouchStep].x,TouchTestSor[SetTouchStep].y,1);   //(char X, char Y, char Colour)
     H_line(TouchTestSor[SetTouchStep].x+2,TouchTestSor[SetTouchStep].y,5,1); // (char X, int Y, char lenght, char typ)
     H_line(TouchTestSor[SetTouchStep].x-6,TouchTestSor[SetTouchStep].y,5,1); // (char X, int Y, char lenght, char typ) 
     V_line(TouchTestSor[SetTouchStep].x,TouchTestSor[SetTouchStep].y+2,5,1); // (char X, int Y, char lenght, char typ) 
     V_line(TouchTestSor[SetTouchStep].x,TouchTestSor[SetTouchStep].y-6,5,1); // (char X, int Y, char lenght, char typ) 
}
}


extern _Correct korekta_Y;
extern _Correct korekta_X;
signed short Diff_X[MaxTouchTest];
signed short Diff_Y[MaxTouchTest];
float Xo1, Xo2, Yo1, Yo2;
void SetTouchScreen_Event(void)
{


if (SetTouchStep==MaxTouchTest)
{
   if ((X_down>0) && (Y_down>0))
   {
      CanvasPixel(X_down,Y_down,1);
     
   }
}else{
  
  if ((X_up>0) && (Y_up>0)&& ((g_counter>Touch_EventTime+400) || (Touch_EventTime>g_counter)) )
  {
      Touch_EventTime=g_counter;
      Diff_X[SetTouchStep]=X_up;
      Diff_Y[SetTouchStep]=Y_up;
      
      if (SetTouchStep<MaxTouchTest-1)
      {
        SetTouchStep++;
        MenuPaint();
      }else{
        
        
        Xo1=(Diff_X[0]+Diff_X[2])/2;
        Xo2=(Diff_X[1]+Diff_X[3])/2;
        Yo1=(Diff_Y[0]+Diff_Y[1])/2;
        Yo2=(Diff_Y[2]+Diff_Y[3])/2;
        
        float Xw1=TouchTestSor[0].x;
        float Xw2=TouchTestSor[1].x;
        float Yw1=TouchTestSor[0].y;
        float Yw2=TouchTestSor[3].y;
        
        
        korekta_X.A=(Xw2-Xw1)/(Xo2-Xo1);
        korekta_X.B=Xw1-korekta_X.A*Xo1;
        korekta_Y.A=(Yw2-Yw1)/(Yo2-Yo1);
        korekta_Y.B=Yw1-korekta_Y.A*Yo1;
        
             
        //zapis korekt do flash
        unsigned int Save = __get_interrupt_state();  
        __disable_interrupt();
            FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.Cor_X)), (char*) (&(korekta_X.A)), sizeof(korekta_X));
            FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.Cor_Y)), (char*) (&(korekta_Y.A)), sizeof(korekta_Y));
        __set_interrupt_state(Save);
        __enable_interrupt(); 
        //koniec zapisu do flash
      
        //je¿eli wejœcie w opcje zaawansowane
        if (StartAdvance)
        {
          StartAdvance=0;
          MenuLevel=5;
        }else{
          MenuLevel=1;
        }
        //SetTouchStep=MaxTouchTest;
        //CleanCanvas();
        SetTouchStep=0;
        MenuPaint();
      }
  }
}

}
    
    
    
    

//restart
      //*-----------------------------------------------------------------------
      void Restart_Event(void)
      {
        AT91S_RSTC  g_AT91S_RSTC;
        int ans;
 
        ans=Ask_Event(X_up, Y_up, X_down, Y_down); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        if (ans)
          {
          if (ans==2) 
          {

            //restart
            AT91C_BASE_RSTC->RSTC_RCR = AT91C_RSTC_PROCRST | AT91C_RSTC_PERRST | AT91C_RSTC_KEY;
          }
          if ((ans<0) || (ans==1))  MenuLevel=(MenuLevel/10);
          MenuPaint();
          } 
      }//_______________________________________________________________________
    
    

    


    
//czyœæ rejestr
      //*-----------------------------------------------------------------------
    
      void CleanReg_Event(void)
      {
        int ans;
 
        ans=Ask_Event(X_up, Y_up, X_down, Y_down); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        if (ans)
          {
          if (ans==2) 
          {
            //czyszczenie rejestru
            CleanReg();
            MenuLevel=(MenuLevel/10);
            MenuPaint();
          }
          if ((ans<0) || (ans==1))  MenuLevel=(MenuLevel/10);
          MenuPaint();
          } 
      }//_______________________________________________________________________
         
    
    
//Ustawienia czasu  **************************************************************************************************************
    //*----------------------------------------------------------------------------
    void SetupTime_Paint(void)
    {  
       List_Paint(Speech[g_leng].DataCzas,1,108,0,0,1," ", 0 ,0,0,0,0,0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])
       

       
       Label(Speech[g_leng].CzasGMS,0,3,99,10,33,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          
          
       char time_char[3];
        
       CharToStr(DateTime.Hour, time_char, 2); //(char Value, char *Vstring, char ill);
       Label(time_char,2,12,87,2,11,1,127,((SetupTimeCarPoz)&0x1) );  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
       CharToStr(DateTime.Minute, time_char, 2); //(char Value, char *Vstring, char ill);
       Label(time_char,2,27,87,2,11,1,127,(SetupTimeCarPoz>>1)&0x1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
       CharToStr(DateTime.Second, time_char, 2); //(char Value, char *Vstring, char ill);
       Label(time_char,2,42,87,2,11,1,127,(SetupTimeCarPoz>>2)&0x1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
        
       
       Label(Speech[g_leng].DataDMR,0,3,74,10,33,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
      
       CharToStrData(DateTime.Day, time_char); //(char Value, char *Vstring, char ill);
       Label(time_char,2,12,62,2,10,1,127,(SetupTimeCarPoz>>3)&0x1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
       CharToStrData(DateTime.Mounth, time_char); //(char Value, char *Vstring, char ill);
       Label(time_char,2,27,62,2,10,1,127,(SetupTimeCarPoz>>4)&0x1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
       CharToStrData(DateTime.Year, time_char); //(char Value, char *Vstring, char ill);
       Label(time_char,2,42,62,2,10,1,127,(SetupTimeCarPoz>>5)&0x1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
        
    
       
       H_line(10,97,46,1); // (char X, int Y, char lenght, char typ) 
       H_line(10,85,46,1); // (char X, int Y, char lenght, char typ) 
       V_line(10,86,11,1); // (char X, int Y, char lenght, char typ) 
       Label(":",0,25,88,1,1,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
       Label(":",0,40,88,1,1,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
         
       //V_line(25,86,11,1); // (char X, int Y, char lenght, char typ) 
       //V_line(40,86,11,1); // (char X, int Y, char lenght, char typ) 
       V_line(55,86,11,1); // (char X, int Y, char lenght, char typ) 
       
       H_line(10,72,46,1); // (char X, int Y, char lenght, char typ) 
       H_line(10,60,46,1); // (char X, int Y, char lenght, char typ) 
       V_line(10,61,11,1); // (char X, int Y, char lenght, char typ) 
       H_line(23,66,4,1); // (char X, int Y, char lenght, char typ) 
       H_line(38,66,4,1); // (char X, int Y, char lenght, char typ) 
       //V_line(25,61,11,1); // (char X, int Y, char lenght, char typ) 
       //V_line(40,61,11,1); // (char X, int Y, char lenght, char typ) 
       V_line(55,61,11,1); // (char X, int Y, char lenght, char typ) 
       
      
      // Button("> ",42,5,18,18);
      // Button("< ",3,5,18,18);

       Label((char*)DeyOfWeekLongStr[g_leng][DateTime.DayOfWeek],2,2,9,12,58,0,105,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)        
              
       
       
       char LabelButt[2];
       LabelButt[1]=0;
       LabelButt[0]=30;
       Button(LabelButt,33,30,19,25); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
       LabelButt[0]=31;
       Button(LabelButt,12,30,19,25); //(etykieta char poz_X , char poz_Y ,char width, char height) 
       
        //H_line(4,42,30,1); // (char X, int Y, char lenght, char typ) 
    }//________________________________________________________________________

    void SetupTime_Refresh(void)
    {  

       char time_char[3];
        
       CharToStr(DateTime.Hour, time_char, 2); //(char Value, char *Vstring, char ill);
       Label(time_char,2,12,87,2,11,1,127,((SetupTimeCarPoz)&0x1) );  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
       CharToStr2(DateTime.Minute, time_char, 2); //(char Value, char *Vstring, char ill);
       Label(time_char,2,27,87,2,11,1,127,(SetupTimeCarPoz>>1)&0x1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
       CharToStr2(DateTime.Second, time_char, 2); //(char Value, char *Vstring, char ill);
       Label(time_char,2,42,87,2,11,1,127,(SetupTimeCarPoz>>2)&0x1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
        

       CharToStrData(DateTime.Day, time_char); //(char Value, char *Vstring, char ill);
       Label(time_char,2,12,62,2,10,1,127,(SetupTimeCarPoz>>3)&0x1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
       CharToStrData(DateTime.Mounth, time_char); //(char Value, char *Vstring, char ill);
       Label(time_char,2,27,62,2,10,1,127,(SetupTimeCarPoz>>4)&0x1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
       CharToStrData(DateTime.Year, time_char); //(char Value, char *Vstring, char ill);
       Label(time_char,2,42,62,2,10,1,127,(SetupTimeCarPoz>>5)&0x1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
        
       
       Label((char*)DeyOfWeekLongStr[g_leng][DateTime.DayOfWeek],2,2,9,12,58,0,105,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)        
              
    }//________________________________________________________________________


int przestepny(int rok) {
	return ((rok % 4 == 0  &&  rok % 100 != 0) || rok % 400 == 0);
}


int dzienTygodnia(int dzien, int miesiac, int rok) 
{
	int dr, yy, c, g, wynik;
	int liczbaDni[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
	dr = dzien + liczbaDni[miesiac-1];
	if ((miesiac > 2) && (przestepny(rok) == 1))
		dr++;
	yy = (rok - 1) % 100;
	c = (rok - 1) - yy;
	g = yy + (yy / 4);
	wynik = (((((c / 100) % 4) * 5) + g) % 7);
	wynik += dr - 1;
	wynik %= 7;
	return wynik+1;
}



    //*-----------------------------------------------------------------------------
    char SleepModTime=0;
    void SetupTime_Event(void)
    {
        int ans=List_Event(X_up, Y_up, X_down, Y_down, 1 , 0, 0,0);  //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
        
        char  SetupPozTmp=0;
        if ((X_up>10) && (X_up<56) &&  (Y_up>85) && (Y_up<97))  
        {
           SetupPozTmp = (X_up-10)/15;
           SetupTimeCarPoz=0;
           SetupTimeCarPoz=1<<SetupPozTmp;
           if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}
        }
        if ((X_up>10) && (X_up<56) &&  (Y_up>60) && (Y_up<72))  
        {
           SetupPozTmp = (X_up-10)/15;
           SetupTimeCarPoz=0;
           SetupTimeCarPoz=1<<SetupPozTmp+3;
           if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}
        }
        
       if (ans<0)  
       {
          SetRTC_DateTime();  //Ustawinie: sekundy, minuty,  godziny, dzieñ tyg, dnia, miesi¹ca, roku
          MenuLevel=(MenuLevel/10);
          MenuPaint();
       }
       else
       {
        char odp=0;
        int set_zm=0;
        char LabelButt[1]={30};
        odp = (Button(LabelButt,33,30,19,25));
        if ((odp==2) && (SleepModTime==0)) set_zm=1;
        if (odp==2) { SleepModTime++; if (SleepModTime>10) SleepModTime=0;};            //(char poz_X , char poz_Y ,char width, char height)
        
        
        if (odp==1) {SleepModTime=0;}
        
        LabelButt[0]=31;
        odp=0;
        odp = (Button(LabelButt,12,30,19,25));
        if ((odp==2)&& (SleepModTime==0))  set_zm=-1;
        if (odp==2) { SleepModTime++; if (SleepModTime>10) SleepModTime=0;};            //(char poz_X , char poz_Y ,char width, char height)
        
        if (odp==1) {SleepModTime=0;} 
        
        
        signed int dzien_tmp= DateTime.Day;
        signed int mies_tmp=DateTime.Mounth;
        signed int rok_tmp=DateTime.Year;
        signed int godz_tmp=DateTime.Hour;
        signed int mi_tmp=DateTime.Minute;
        signed int sek_tmp=DateTime.Second;
        
        
        switch (SetupTimeCarPoz)
        {
          case 1: godz_tmp+=set_zm;  break;
          case 2: mi_tmp+=set_zm;     break;
          case 4: sek_tmp+=set_zm;   break;
          case 8: dzien_tmp+=set_zm;  /*DateTime.DayOfWeek+=set_zm;*/  break;
          case 16: mies_tmp+=set_zm;   break;
          case 32: rok_tmp+=set_zm;   break; 
        }
        
        if (sek_tmp>59) {sek_tmp=0;} 
        if (mi_tmp>59)  {mi_tmp=0;}
        if (godz_tmp>23) godz_tmp=0;  
        
        if (sek_tmp<0) {sek_tmp=59;}
        if (mi_tmp<0)  {mi_tmp=59;}
        if (godz_tmp<0) godz_tmp=23;
       
        char mies_tmp2 = mies_tmp;
        switch (mies_tmp2)
        {
          case 1  : if (dzien_tmp>31) {dzien_tmp=1;} break;
          case 2  : if ((rok_tmp%4==0) && (rok_tmp%100 != 0) || (rok_tmp%400 == 0))  //je¿eli rok przestêpny
                      {  
                        if (dzien_tmp>29) {dzien_tmp=1; } 
                        }else {if (dzien_tmp>28) {dzien_tmp=1; } 
                        }     
                    break;
          case 3  : if (dzien_tmp>31) {dzien_tmp=1; } break;
          case 4  : if (dzien_tmp>30) {dzien_tmp=1; } break;
          case 5  : if (dzien_tmp>31) {dzien_tmp=1; } break;
          case 6  : if (dzien_tmp>30) {dzien_tmp=1; } break;
          case 7  : if (dzien_tmp>31) {dzien_tmp=1; } break;
          case 8  : if (dzien_tmp>31) {dzien_tmp=1; } break;
          case 9  : if (dzien_tmp>30) {dzien_tmp=1; } break;
          case 10 : if (dzien_tmp>31) {dzien_tmp=1; } break;
          case 11 : if (dzien_tmp>30) {dzien_tmp=1; } break;
          case 12 : if (dzien_tmp>31) {dzien_tmp=1; } break;
        }  
        
        if (mies_tmp>12)  {mies_tmp=1; }
        if (rok_tmp>99) rok_tmp=0;  
       
        mies_tmp2 = mies_tmp;
        if (dzien_tmp<=0)
        switch (mies_tmp2)
        {
          case 1  : dzien_tmp=31; break;
          case 2  : dzien_tmp=31; break;
          case 3  : if ((rok_tmp%4==0) && (rok_tmp%100 != 0) || (rok_tmp%400 == 0))  //je¿eli rok przestêpny
                   { dzien_tmp=29;} else { dzien_tmp=28; }  break;
          case 4  : dzien_tmp=31;  break;
          case 5  : dzien_tmp=30; break;
          case 6  : dzien_tmp=31; break;
          case 7  : dzien_tmp=30; break;
          case 8  : dzien_tmp=31; break;
          case 9  : dzien_tmp=31; break;
          case 10 : dzien_tmp=30; break;
          case 11 : dzien_tmp=31; break;
          case 12 : dzien_tmp=30; break;
        } 
         
        if (mies_tmp<0)  {mies_tmp=12;}
        if (rok_tmp<0) rok_tmp=0;


        if (dzien_tmp>31) dzien_tmp=31;
        
        DateTime.Day= dzien_tmp;
        DateTime.Mounth=mies_tmp;
        DateTime.Year=rok_tmp;
        DateTime.Hour=godz_tmp;
        DateTime.Minute=mi_tmp;
        DateTime.Second=sek_tmp;
        

       
       
       DateTime.DayOfWeek=dzienTygodnia(DateTime.Day,DateTime.Mounth,DateTime.Year ); //(int dzien, int miesiac, int rok) 

       }
    }//________________________________________________________________________


    
    
//ustawienie podœwietlenia
#define TAPozY 80
#define TNPozY 40
//*---------------------------------------------------------------------------- 
void ScreenBrightness_Paint(void)
    {  
       List_Paint(Speech[g_leng].Jasnosc,1,108,0,0,1," ", 0 ,0,0,0,0,0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])
          
       
      //tryb aktywny wyœwietlacza --------------------------------------------------
       Label(Speech[g_leng].Tryb_aktywny,0,1,TAPozY+5,15,33,1,127,0);    //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 

       H_line(4,TAPozY+1,54,1); // (char X, int Y, char lenght, char typ) 
       H_line(4,TAPozY-1,54,1); // (char X, int Y, char lenght, char typ) 
       CanvasPixel(3, TAPozY, 1);
       CanvasPixel(58, TAPozY, 1);
       
       char WypMaxJasnosciTMP=((WypMaxJasnosci-3)*2);
       if (WypMaxJasnosciTMP>100) WypMaxJasnosciTMP=100;
       char poz1=  (48 * WypMaxJasnosciTMP/100)+7;
       H_line(poz1-1,TAPozY+3,3,1); // (char X, int Y, char lenght, char typ)
       H_line(poz1-2,TAPozY+2,5,1); // (char X, int Y, char lenght, char typ)
       H_line(poz1-2,TAPozY,5,1); // (char X, int Y, char lenght, char typ)
       H_line(poz1-2,TAPozY-2,5,1); // (char X, int Y, char lenght, char typ)
       H_line(poz1-1,TAPozY-3,3,1); // (char X, int Y, char lenght, char typ)
       
       if (poz1<10)
       {
         poz1=10; 
       }else if (poz1>55)
       {
         poz1=55; 
       }
       
       char TXT[5];
       char IL=CharToStr(WypMaxJasnosciTMP, TXT, 3); //(char Value, char *Vstring, char ill)
       TXT[IL++]='%';
       //Label(TXT,2,poz1,TAPozY-12,IL,33,1,127,0);                               //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
       Label(TXT,0,26,TAPozY-13,IL,20,1,127,0);                               //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 

       
       
      //tryb nieaktywny wyœwietlacza -------------------------------------------------- 
       Label(Speech[g_leng].Tryb_nieaktywny,0,1,TNPozY+5,15,33,1,127,0);       //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 

       H_line(4,TNPozY+1,54,1);                                                 // (char X, int Y, char lenght, char typ) 
       H_line(4,TNPozY-1,54,1);                                                 // (char X, int Y, char lenght, char typ) 
       CanvasPixel(3, TNPozY, 1);
       CanvasPixel(58, TNPozY, 1);
       
       char WypMinJasnosciTMP=((WypMinJasnosci-3)*2);
       if (WypMinJasnosciTMP>100) WypMinJasnosciTMP=100;
       poz1=  (48 * WypMinJasnosciTMP/100)+7;
       H_line(poz1-1,TNPozY+3,3,1); // (char X, int Y, char lenght, char typ)
       H_line(poz1-2,TNPozY+2,5,1); // (char X, int Y, char lenght, char typ)
       H_line(poz1-2,TNPozY,5,1); // (char X, int Y, char lenght, char typ)
       H_line(poz1-2,TNPozY-2,5,1); // (char X, int Y, char lenght, char typ)
       H_line(poz1-1,TNPozY-3,3,1); // (char X, int Y, char lenght, char typ)
       
       if (poz1<10)
       {
         poz1=10; 
       }else if (poz1>55)
       {
         poz1=55; 
       }
       
       IL=CharToStr(WypMinJasnosciTMP, TXT, 3); //(char Value, char *Vstring, char ill)
       TXT[IL++]='%';
       //Label(TXT,2,poz1,TNPozY-12,IL,33,1,127,0);                                     //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
       Label(TXT,0,26,TNPozY-13,IL,20,1,127,0);                                     //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
 
}//________________________________________________________________________
    
    
    
    
    
  
//*-----------------------------------------------------------------------------
void ScreenBrightness_Event(void)
    {
      char RepaintTMP=0;
      int ans=List_Event(X_up, Y_up, X_down, Y_down, 1 , 0, 0,0); 
      if (ans<0)  
       {
          MenuLevel=(MenuLevel/10);
          gWypMinJasnosciTest=0;
          AT91C_BASE_PWMC->PWMC_DIS = 0x1;
          Set_LCD_Light_PWM_Fill(WypMaxJasnosci); 
          AT91C_BASE_PWMC->PWMC_ENA = 0x1;
          RepaintTMP=1;
       }else{
           char Xtym=0;
           if ((X_down>1) && (X_down<63) && (Y_down>TAPozY-10) && (Y_down<TAPozY+5))                    //ustawienie wype³nienia podœwietlenia ekranu aktywnego
           {
                Xtym=X_down;
                if (Xtym<7) Xtym=7;
                if (Xtym>55) Xtym=55;
                Xtym -=7;
                Xtym=(Xtym *100)/48;
                if (Xtym<10) Xtym=10;                                           //ograniczeni minimalnej jasnoœci wyœwietlacza w trybie aktywnym 
                
                char WypMaxJasnosciTMP=Xtym/2 +3;
                if (WypMaxJasnosciTMP>50) WypMaxJasnosciTMP=100;
                WypMaxJasnosci=WypMaxJasnosciTMP;
                if (WypMaxJasnosci<WypMinJasnosci) WypMinJasnosci=WypMaxJasnosci;
                RepaintTMP=1;
                
                AT91C_BASE_PWMC->PWMC_DIS = 0x1;
                Set_LCD_Light_PWM_Fill(WypMaxJasnosci); 
                AT91C_BASE_PWMC->PWMC_ENA = 0x1;
           }
           if ((X_down>1) && (X_down<63) && (Y_down>TNPozY-10) && (Y_down<TNPozY+5))                    //ustawienie wype³nienia podœwietlenia ekranu nieaktywnego
           {
                Xtym=X_down;
                if (Xtym<7) Xtym=7;
                if (Xtym>55) Xtym=55;
                Xtym -=7;
                char WypMinJasnosciTMP=(((Xtym *100)/48))/2+3;
                if (WypMinJasnosciTMP>50) WypMinJasnosciTMP=100;
                WypMinJasnosci=WypMinJasnosciTMP;
                if (WypMaxJasnosci<WypMinJasnosci) WypMaxJasnosci=WypMinJasnosci;
                RepaintTMP=1;
                gWypMinJasnosciTest=1;
                
                AT91C_BASE_PWMC->PWMC_DIS = 0x1;
                Set_LCD_Light_PWM_Fill(WypMinJasnosci); 
                AT91C_BASE_PWMC->PWMC_ENA = 0x1;
           }
           
           if ((X_up>0) && (X_up<63) && (((Y_up>TAPozY-10) && (Y_up<TAPozY+5)) ||  ((Y_up>TNPozY-10) && (Y_up<TNPozY+5))))                    //zapamiêtanie wype³nienia podœwietlenia ekranu 
           {
               
              //zapis do flash
              unsigned int Save = __get_interrupt_state();  
              __disable_interrupt();
              FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.ScreenBrightActive)), (char*) (&(WypMaxJasnosci)), 1);
              FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.ScreenBrightInactive)), (char*) (&(WypMinJasnosci)), 1);
              __set_interrupt_state(Save);
              __enable_interrupt(); 
              //koniec zapisu do flash
            
              
              AT91C_BASE_PWMC->PWMC_DIS = 0x1;
                Set_LCD_Light_PWM_Fill(WypMaxJasnosci); 
                AT91C_BASE_PWMC->PWMC_ENA = 0x1;
              gWypMinJasnosciTest=0;
             
           }
           
         
       }
       
      
       if (RepaintTMP) MenuPaint();  
    }//________________________________________________________________________
    


    
    
    
//Ustawienia wygaszacza  **************************************************************************************************************
extern char g_ScreenSever_On;
extern char g_ScreenSever_Time_On;
extern char g_ScreenSever_Time_Off;
//*---------------------------------------------------------------------------- 
    void ScreenSever_Paint(void)
    {  
      
       List_Paint(Speech[g_leng].ScreenSever,1,108,0,0,1," ", 0 ,0,0,0,0,0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])
       
       g_Stop_Program=1;
       
       //Speech[g_leng].RfChannel
       Label("On/off",0,6,90,10,33,1,127,0);
       if (g_ScreenSever_On==0)
       {
          Zone(" ",40,86,15,16,1);//(char _Label[15], char poz_X , char poz_Y ,char width, char height, char LineType) 
       }else{
          Zone("X",40,86,15,16,1);//(char _Label[15], char poz_X , char poz_Y ,char width, char height, char LineType) 
      
       char tmp_char[3];
       Label(Speech[g_leng].TimeOn,0,5,73,15,33,1,127,0);
       CharToStr(g_ScreenSever_Time_On, tmp_char, 2); //(char Value, char *Vstring, char ill);
       Label(tmp_char,1,29,54,2,10,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
       Label(":00",0,30,54,3,10,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
                
       
       char LabelButt[2];
       LabelButt[1]=0;
       LabelButt[0]=30;
       Button(LabelButt,44,47,17,25); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
       LabelButt[0]=31;
       Button(LabelButt,2,47,17,25); //(etykieta char poz_X , char poz_Y ,char width, char height) 
       
       Label(Speech[g_leng].TimeOff,0,5,34,15,33,1,127,0);
       CharToStr(g_ScreenSever_Time_Off, tmp_char, 2); //(char Value, char *Vstring, char ill);
       Label(tmp_char,1,29,16,2,18,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
       Label(":00",0,30,16,3,18,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
                           
     
       LabelButt[1]=0;
       LabelButt[0]=30;
       Button(LabelButt,44,8,17,25); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
       LabelButt[0]=31;
       Button(LabelButt,2,8,17,25); //(etykieta char poz_X , char poz_Y ,char width, char height) 
       }

        //H_line(4,42,30,1); // (char X, int Y, char lenght, char typ) 
    }//________________________________________________________________________


    
//*-----------------------------------------------------------------------------
    void ScreenSever_Event(void)
    {
      char RepaintTMP=0;
      int ans=List_Event(X_up, Y_up, X_down, Y_down, 1 , 0, 0,0); 
      if (ans<0)  
       {
          //zapis do flash
              unsigned int Save = __get_interrupt_state();  
              __disable_interrupt();
              //at91flashWrite((int*)(&(AT91C_IFLASH_MEM->Config.ScreenSever_On)),0, &g_ScreenSever_On,1);
              //at91flashWrite((int*)(&(AT91C_IFLASH_MEM->Config.ScreenSever_Time_On)),0, &g_ScreenSever_Time_On,1);
              //at91flashWrite((int*)(&(AT91C_IFLASH_MEM->Config.ScreenSever_Time_Off)),0, &g_ScreenSever_Time_Off,1);
              FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.ScreenSever_On)), (char*) (&(g_ScreenSever_On)), 1);
              FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.ScreenSever_Time_On)), (char*) (&(g_ScreenSever_Time_On)), 1);
              FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.ScreenSever_Time_Off)), (char*) (&(g_ScreenSever_Time_On)), 1);
              __set_interrupt_state(Save);
              __enable_interrupt(); 
              //koniec zapisu do flash
         
          g_Stop_Program=0;
          MenuLevel=(MenuLevel/10);
          RepaintTMP=1;
       }else{
      
         if (g_ScreenSever_On==1)
         {
          if (Zone("X",40,86,15,16,1)==1)  //(char _Label[15], char poz_X , char poz_Y ,char width, char height, char LineType) 
          {
            g_ScreenSever_On=0;
            RepaintTMP=1;
          }
          
          char LabelButt[2];
            LabelButt[1]=0;
            LabelButt[0]=30;
            if (Button(LabelButt,44,47,17,25)==1) //(etykieta ,char poz_X , char poz_Y ,char width, char height)
            {
              if (g_ScreenSever_Time_On<23) 
              {
                g_ScreenSever_Time_On++;
              }
              RepaintTMP=1;
            }
            LabelButt[0]=31;
            if (Button(LabelButt,2,47,17,25)==1) //(etykieta char poz_X , char poz_Y ,char width, char height) 
            {
               if (g_ScreenSever_Time_On>0) g_ScreenSever_Time_On--;
              RepaintTMP=1;
            }
            LabelButt[0]=30;
            if (Button(LabelButt,44,8,17,25)==1) //(etykieta ,char poz_X , char poz_Y ,char width, char height)
            {
              if (g_ScreenSever_Time_Off<23) g_ScreenSever_Time_Off++;
              RepaintTMP=1;
            }
            LabelButt[0]=31;
            if (Button(LabelButt,2,8,17,25)==1) //(etykieta char poz_X , char poz_Y ,char width, char height) 
            {
               if (g_ScreenSever_Time_Off>0) 
               {
                 g_ScreenSever_Time_Off--;
               }
              RepaintTMP=1;
            }
         }else{
           if (Zone(" ",40,86,15,16,1)==1)  //(char _Label[15], char poz_X , char poz_Y ,char width, char height, char LineType) 
          {
            g_ScreenSever_On=1;
            g_ScreenSever_Time_On=0;
            g_ScreenSever_Time_Off=0;
            RepaintTMP=1;
          }
         }
         
         
       }
       
     
      
       if (RepaintTMP) MenuPaint();  
       
     
    }//________________________________________________________________________
    
    
    
//Ustawienia jêzyka******************************************************************************************************************

      //*----------------------------------------------------------------------------
      void MasterSlave_Paint(void)
      {  
          //char Y=3;
          int Y_tmp = 66;   //przesuniêcie w dó³

          List_Paint("Mode",2,15,Y_tmp,7,0,"Master ", "Slave ",0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])
      
          
          char ekran_tab[10] =  {0,4,4,10,11,16,16,32,64,0};
       
        
          CopyToCanvasPoint((char *)ekran_tab,45,97-Y_tmp-(15*(SlaveMode)),8,10); 
          
      }//________________________________________________________________________



      //*-----------------------------------------------------------------------
      void MasterSlave_Event(void)
      {
        int ans;
 
        ans=List_Event(X_up, Y_up, X_down, Y_down, 2 , 15, 7,66); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        if (ans)
          {
            if ((ans>0) && (ans<3))
            {
              SlaveMode=ans-1;
              
              //zapis do flash
              unsigned int Save = __get_interrupt_state();  
              __disable_interrupt();
              //at91flashWrite((int*)(&(AT91C_IFLASH_MEM->Config.SlaveMode)),0, &SlaveMode,1);
              FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.SlaveMode)), (char*) (&(SlaveMode)), 1);
              __set_interrupt_state(Save);
              __enable_interrupt(); 
              //koniec zapisu do flash
              
              if (!AT91C_IFLASH_MEM->Config.SlaveMode) //je¿eli master 
              {
                RF_Status.MasterMode=1;
                _RF_EnableRX(); 
               for (char i=0; i<NumberNeurons; i++) TransQuality.Mensur[i]=0; 
              }else{
                RF_Status.MasterMode=0; 
              }
                 AT91PS_MEM pAT91C_IFLASH_MEM=AT91C_IFLASH_MEM;
                RF_Status.Ch=pAT91C_IFLASH_MEM->Config.RF_channel;
                SPI_SendWord(0xA000 | RF_SetFreqReg(RF_Status.Ch),Cs_RF); 
                while (RF_CheckChannel())
                {
                  if (RF_Status.Ch<RF_ChannelQuanti-1) {RF_Status.Ch++;} else {RF_Status.Ch=1;}
                  SPI_SendWord(0xA000 | RF_SetFreqReg(RF_Status.Ch),Cs_RF); 
                } 
              
              MenuLevel=(MenuLevel/10);
              MenuPaint();
            }
          } 
        else
          {
            if ((X_up>0) &&(Y_up>0)) 
            {
              MenuLevel=(MenuLevel/10);
              MenuPaint();
            }
          }
      }//_______________________________________________________________________

    
    
    
 
//Ustawienia kana³u transmisji WiFi  **************************************************************************************************************

    //*----------------------------------------------------------------------------
    char RF_ChOkBlock=0;
    void RF_Channel_Paint(void)
    {  
       List_Paint(Speech[g_leng].RfChannel,1,108,0,0,1," ", 0 ,0,0,0,0,0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])
      if (!AT91C_IFLASH_MEM->Config.SlaveMode)  //je¿eli tryb master
      {
       _RF_EnableRX();
       g_Stop_Program=1;
       
       //Speech[g_leng].RfChannel
       Label(Speech[g_leng].RfChannel,0,3,90,10,33,1,127,0);
  
       char tmp_char[3];
       CharToStr(RF_Channel_nr_tmp, tmp_char, 2); //(char Value, char *Vstring, char ill);
       
       Label(tmp_char,1,30,77,2,33,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          
       
       char LabelButt[2];
       LabelButt[1]=0;
       LabelButt[0]=30;
       Button(LabelButt,33,48,19,25); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
       LabelButt[0]=31;
       Button(LabelButt,12,48,19,25); //(etykieta char poz_X , char poz_Y ,char width, char height) 
       
       if ((RF_ChOkBlock) && (RF_Status.Ch!=RF_Channel_nr_tmp))
       {
         Label(Speech[g_leng].KanalZajety,0,6,30,14,33,1,127,0);
       }else{
         Button("OK",12,12,40,25); //(etykieta char poz_X , char poz_Y ,char width, char height) 
       }
      }else{ //je¿eli tryb slave
         Label(Speech[g_leng].Niedostepne,2,2,70,14,59,1,127,0);
         Label(Speech[g_leng].W_Trybie_Slave,2,2,60,14,56,1,127,0);
      }
        //H_line(4,42,30,1); // (char X, int Y, char lenght, char typ) 
    }//________________________________________________________________________


    void RF_Channel_Refresh(void)
    {  
      if (AT91C_IFLASH_MEM->Config.SlaveMode)  //je¿eli tryb master
      {
      
       List_Paint(Speech[g_leng].RfChannel,1,108,0,0,1," ", 0 ,0,0,0,0,0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])
       
       g_Stop_Program=1;
       
       //Speech[g_leng].RfChannel
       Label(Speech[g_leng].RfChannel,0,3,90,10,33,1,127,0);
  
       char tmp_char[3];
       CharToStr(RF_Channel_nr_tmp, tmp_char, 2); //(char Value, char *Vstring, char ill);
       
       Label(tmp_char,1,30,77,2,33,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          
       
       char LabelButt[2];
       LabelButt[1]=0;
       LabelButt[0]=30;
       Button(LabelButt,33,48,19,25); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
       LabelButt[0]=31;
       Button(LabelButt,12,48,19,25); //(etykieta char poz_X , char poz_Y ,char width, char height) 
       
       if ((RF_Status.ConfResive) && (RF_Status.Ch!=RF_Channel_nr_tmp))
       {
         Label(Speech[g_leng].KanalZajety,0,6,30,14,33,1,127,0);
       }else{
         Button("OK",12,12,40,25); //(etykieta char poz_X , char poz_Y ,char width, char height) 
       }

      } 
    }//________________________________________________________________________
    
    
    
    //*-----------------------------------------------------------------------------

    void RF_Channel_Event(void)
    {
      int ans=List_Event(X_up, Y_up, X_down, Y_down, 1 , 0, 0,0); 
      if (ans<0)  
       {
          g_Stop_Program=0;
          MenuLevel=(MenuLevel/10);
          MenuPaint();
       }else{
      

       char LabelButt[2];
       LabelButt[1]=0;
       LabelButt[0]=30;

       if (Button(LabelButt,33,48,19,25)==1) 
       {
          if (RF_Channel_nr_tmp<RF_ChannelQuanti-1) {RF_Channel_nr_tmp++;} else {RF_Channel_nr_tmp=1;}
          SPI_SendWord(0xA000 | RF_SetFreqReg(RF_Channel_nr_tmp),Cs_RF);

          RF_ChOkBlock=RF_CheckChannel();
          MenuPaint(); 
       }
       LabelButt[0]=31;

       if (Button(LabelButt,12,48,19,25)==1)
       {
          if (RF_Channel_nr_tmp>1) {RF_Channel_nr_tmp--;} else {RF_Channel_nr_tmp=RF_ChannelQuanti-1;}

          SPI_SendWord(0xA000 | RF_SetFreqReg(RF_Channel_nr_tmp),Cs_RF);
          
 
        


       

          RF_ChOkBlock=RF_CheckChannel();
          MenuPaint(); 
       }       
       
       
       if (RF_ChOkBlock)
       {
         Label(Speech[g_leng].KanalZajety,0,6,30,14,33,1,127,0);
       }else{
        if (Button("OK",12,12,40,25)==1)
        {
         g_Stop_Program=0;
         RF_Status.Ch=RF_Channel_nr_tmp;
         

              //zapis do flash
              unsigned int Save = __get_interrupt_state();  
              __disable_interrupt();
              //at91flashWrite((int*)(&(AT91C_IFLASH_MEM->Config.RF_channel)),0, &RF_Channel_nr_tmp,1);
              FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.RF_channel)), (char*) (&(RF_Channel_nr_tmp)), 1);
              __set_interrupt_state(Save);
              __enable_interrupt(); 
              //koniec zapisu do flash
         
         
         MenuLevel=(MenuLevel/10);
         MenuPaint();
         
        }
       }
       
       
      
       
       
       }
    }//________________________________________________________________________
    
    
    
    
    
    
    
    
    
    
    

//Wybór poziomu dostêpu *************************************************************************************************************
      //*-----------------------------------------------------------------------
      void Sygnal_Paint(void)
      {
          #define _Y   10   //przesuniêcie w dó³
          List_Paint(Speech[g_leng].Sygnaly,4,15,_Y,0,3,Speech[g_leng].Obserwowane,Speech[g_leng].WeWy, Speech[g_leng].ZmAnalog, Speech[g_leng].ZmDwust,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      
      }//_______________________ Koniec funkcji MainSide_Paint __________________________    
    
    
          //*-----------------------------------------------------------------------
      void Sygnal_Event(void)
      {
        int Y_tmp = 10;   //przesuniêcie w dó³
        int ans;

        ans=List_Event(X_up, Y_up, X_down, Y_down, 4 , 15,0,Y_tmp); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        if (ans)
          {
            if ((ans>0) && (ans<5))
            {
                MenuLevel=(MenuLevel*10+ans);
                MenuPaint();
            }
            if (ans<0)
            {
              MenuLevel=(MenuLevel/10);
              MenuPaint();
            }
          }

      }//_______________________________________________________________________




    
    
  long long int WaitOnCheckNode=0;     
  char OpisWezl=0;    
  void NeuronsList_Paint(void)
  {  
       if (WaitOnCheckNode==0) WaitOnCheckNode=g_counter+1000+TimeUnitQual*32; 

      char txt[8];
       
      
       List_Paint(Speech[g_leng].Wezly,1,108,0,0,3," ",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      
     
      
    //  if (!SlaveMode)//je¿eli tryb Master
    //  {

       char LabelButt[2];
       LabelButt[1]=0;
       LabelButt[0]=30;
       Button(LabelButt,24,3,19,19); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
       LabelButt[0]=31;
       Button(LabelButt,3,3,19,19); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
       
       if (OpisWezl) LabelButt[0]=36; else LabelButt[0]=35;
       Button(LabelButt,46,3,14,19); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
        
       H_line(1,25+6*12,62,3); 
       for (char i=0; i<6; i++) 
       {H_line(1,25+i*12,62,3); 
           CharToStr(NeuronsListLine+6-i, txt, 2); //(char Value, char *Vstring, char ill);
           Label(txt,0,2,27+i*12,2,23,16,105,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
           
           if (gProg.NeuronsIdTab[(5-i)+NeuronsListLine]>0)
           {
               if (OpisWezl)
               {//opiski
                  Label(gProg.NodeLabel[(5-i)+NeuronsListLine],0,14,27+i*12,NodeLabelLethers,23,16,105,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 

               }else{
                 #ifdef  DIRECT_Q_MESURE 
                    sprintf(txt, "%d",  TransQuality.DirectkMensur[RF_TaskTab[RF_CurrentTask].Node-1]);
                    Label(txt,0,22,27+i*12,8,23,16,105,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
                 #else
                  //numery nodów 
                  sprintf(txt, "%d", gProg.NeuronsIdTab[(5-i)+NeuronsListLine]);
                  Label(txt,0,22,27+i*12,8,23,16,105,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
                #endif
               }
           }
  
     
       }
       if(OpisWezl)
             {
                 V_line(12,25,73,1);
           
             }else{
                 V_line(20,25,73,1); 
                 V_line(58,25,73,1); 
             }
           
           
       Label("Node ",0,2,99,4,23,16,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
       if(OpisWezl==0) Label("Id ",0,37,99,2,23,16,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 

      /*     
      }else{ //je¿eli tryb slave
         Label(Speech[g_leng].Niedostepne,2,2,70,14,59,1,127,0);
         Label(Speech[g_leng].W_Trybie_Slave,2,2,60,14,55,1,127,0);
      } 
       */
       
      }//________________________________________________________________________
int wpistmp=0;


      //*-----------------------------------------------------------------------------
      void NeuronsList_Event(void)
      {

            

        //zdarzenie typu down "X"
        if ((X_down>46) && (X_down<63) && (Y_down>112) && (Y_down<127))    //"X"
        {
          
          
          //MenuPaint();
          H_line(49,114,10,0);
          H_line(49,123,10,0); 
          V_line(49,114,10,0); 
          V_line(58,114,10,0); 
          if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}
          
        }
  
        



    
        if ((X_up>46) && (X_up<63) && (Y_up>112) && (Y_up<127))                    //"X"
        {
          OpisWezl=0;
          Rej_poz=0;
          if (ProgramChange) 
          {
            if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}
            MenuLevel=(MenuLevel*10+0);
             
            
          }else{
            if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}
            MenuLevel=(MenuLevel/10);
          }
          
          MenuPaint();
          WaitOnCheckNode=0;
        }
        else{
  
                 
   //   if (!SlaveMode)//je¿eli tryb Master
  //    {
            char LabelButt[2]; LabelButt[1]=0;
            LabelButt[0]=30;
            if ((Button(LabelButt,24,3,19,19))  && ((g_counter>Touch_EventTime+100) || (Touch_EventTime>g_counter)) && (NeuronsListLine>0))            
            {
              Touch_EventTime=g_counter;
              NeuronsListLine--; //(etykieta ,char poz_X , char poz_Y ,char width, char height)
              MenuPaint();
            } 
            
            LabelButt[0]=31;
            if ((Button(LabelButt,3,3,19,19)) && ((g_counter>Touch_EventTime+100) || (Touch_EventTime>g_counter)) && (NeuronsListLine<NumberNeurons-6))
            {
              Touch_EventTime=g_counter;
              NeuronsListLine++;    
              MenuPaint();
            }
            
            
            if (OpisWezl) LabelButt[0]=36; else LabelButt[0]=35;
            if (Button(LabelButt,46,3,14,19)==1){if (OpisWezl) OpisWezl=0; else OpisWezl=1;  MenuPaint();}
            
            
            for (char i=0; i<6; i++) 
            {
             if ((Y_down>25+i*12) && (Y_down<25+((i+1)*12)))                
              {
                 MenuPaint();
                 H_line(1,26+i*12,62,1); 
                 H_line(1,24+(i+1)*12,62,1); 
              }
              if ((Y_up>25+i*12) && (Y_up<25+((i+1)*12)))                
              { 
                if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}
                if (OpisWezl==0)
                {
                    NeuronIdChange=&(gProg.NeuronsIdTab[(5-i)+NeuronsListLine]);
                    NeuronNrChange=(5-i)+NeuronsListLine;
                    //je¿eli jest ju¿ nadany ID
                    if (gProg.NeuronsIdTab[(5-i)+NeuronsListLine]>0)
                    {
                      NeuronNrChange=(6-i)+NeuronsListLine;
                      TextToChange=gProg.NodeLabel[(5-i)+NeuronsListLine]; 
                      TextToChangeLength=NodeLabelLethers;
                      
                      MenuLevel=(MenuLevel*10+3);
                    }else{
                      MenuLevel=(MenuLevel*10+1);
                    }
                }else{
                    if (gProg.NodeLabel[(5-i)+NeuronsListLine][0]!=0)
                    {
                      NeuronNrChange=(6-i)+NeuronsListLine;
                      TextToChange=gProg.NodeLabel[(5-i)+NeuronsListLine]; 
                      TextToChangeLength=NodeLabelLethers;
                      MenuLevel=(MenuLevel*10+3);
                    }else{  
                      TextToChange=gProg.NodeLabel[(5-i)+NeuronsListLine]; 
                      TextToChangeLength=NodeLabelLethers;
                      MenuLevel=(MenuLevel*10+2); 
                      ProgramChange=1;
                    }
                    
                }
                MenuPaint();
              }
             
            }
     // }
        }

      }//________________________________________________________________________
    


    void NeuronsList_Refresh(void)
    {  
      
      //for (char i=0; i<NumberNeurons; i++)  MainStage.SendStageChange[i]=20;
      
      
      
      static char Dzielnik20;
      
      //char CheckTrans=0;
      //if (WaitOnCheckNode>g_counter) CheckTrans=1;
      
      
      if (Dzielnik20<20) Dzielnik20++; else Dzielnik20=0;
      
      if (/*(!SlaveMode) &&*/ (OpisWezl==0))//je¿eli tryb Master
      {
        
        char ERRx1=0;  //iloœæ b³êdów pojedyñczych
        char ERRx2=0;  //iloœæ b³êdów podwójnych (dwukrotny brak komunikacji w czasie TimeUnitQual)
        char ERRx3=0;  //iloœæ b³êdów podtrujnych i wiêkszych (przynajmniej trzykrotny brak komunikacji w czasie TimeUnitQual)
        char CurErr=0;
        char LIC=0;
        unsigned int Qality=0;
        char QIdx=TransQuality.MensurIndex;
        char Ypoz;
          for (char i=0; i<6; i++) 
          {
            ERRx1=0; ERRx2=0; ERRx3=0; CurErr=0;
            Ypoz=28+i*12;
           
            
            char Pen=1;
            if ((NeuronsList[(5-i)+NeuronsListLine].Bridge) && (TransQuality.SlowMensur[(5-i)+NeuronsListLine]))
            {
              CleanCanvasArea(12,Ypoz-1,7,9,1);
              //numer wêz³a dla wêz³ów mostowych
                      char txt[9];
                      sprintf(txt, "x%d", NeuronsList[(5-i)+NeuronsListLine].NeuronID);
                      Label(txt,0,22,27+i*12,8,23,16,105,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
                 
              Pen=0;
            }else{
              CleanCanvasArea(12,Ypoz-1,8,9,0);
            }
            
            //ocena jakoœci
            if (TransQuality.Mensur[(5-i)+NeuronsListLine])  //jezeli wykryta transmisja
            {
              if (NeuronsList[(5-i)+NeuronsListLine].NeuronType==NodeRF_Remote)
              {      
                 
                  if ((Dzielnik20<10) && (MainStage.StageI[(6-i)+NeuronsListLine].Port& 0x8000) && ((MainStage.StageI[(6-i)+NeuronsListLine].Port)!=0x8AAA))
                  {
                      CopyToCanvasPoint((char*)BateriaWyladowana, 12, Ypoz-1, 6,9);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);           
                  }
                else
                  {
                
                
                      char MI=TransQuality.MensurIndex;
                      char PT=0;
                      for (char b=0; b<5; b++)
                      {
                        if ((TransQuality.Mensur[(5-i)+NeuronsListLine]) & (0x1 << MI)) PT++;
                        if (MI>0) MI--; else MI=31;   
                      }
                  
                        H_line(15,Ypoz,1,1); // (char X, int Y, char lenght, char typ) 
                        if (PT>1)H_line(14,Ypoz+2,3,1); // (char X, int Y, char lenght, char typ)    
                        if (PT>2)H_line(13,Ypoz+4,5,1); // (char X, int Y, char lenght, char typ)  
                        if (PT>3)H_line(12,Ypoz+6,7,1); // (char X, int Y, char lenght, char typ)  
                 }
 
             
              }
              else if (NeuronsList[(5-i)+NeuronsListLine].NeuronType==NodeRF_Period)
              {
              }
              else{
                    
                      /*
                      for (char b=0; b<32; b++)
                            {
                              if ((TransQuality.Mensur[(5-i)+NeuronsListLine]) & (0x1<<b) )  //jezeli poprawna transmisja w tej jednostce czasu
                              {  
                                CurErr=0;
                              }else{
                                CurErr++;
                                if (CurErr==1) {ERRx1++;}
                                if (CurErr==2) {ERRx2++; ERRx1--; }
                                if (CurErr==3) {ERRx3++; ERRx2--; CurErr=0;}
              
                              }
                              
                            }
           
                      //wskaŸnik jakoœci              
                      H_line(15,Ypoz,1,1); // (char X, int Y, char lenght, char typ) 
                      if ((ERRx1<=12)  && (ERRx2<=10)  && (ERRx3<=5)) H_line(14,Ypoz+2,3,1); // (char X, int Y, char lenght, char typ)    
                      if ((ERRx1<=10)  && (ERRx2<=6)  && (ERRx3<=1)) H_line(13,Ypoz+4,5,1); // (char X, int Y, char lenght, char typ)  
                      if ((ERRx1<=4)   && (ERRx2<=1)  && (ERRx3==0)) H_line(12,Ypoz+6,7,1); // (char X, int Y, char lenght, char typ)  
                      */
                
                      Qality=0;
                      QIdx=TransQuality.MensurIndex;
                      
                      for (char k=1; k<31; k++)
                      {
                        if (QIdx>0) {QIdx--;} else {QIdx=31;}
                        Qality+=(((TransQuality.Mensur[(5-i)+NeuronsListLine]>>(QIdx))&0x1)*(31-k));
                      }
                      
                      //wskaŸnik jakoœci  
                      if (Qality>300) H_line(12,Ypoz+6,7,Pen);                  //najwy¿sza jakoœæ
                      if (Qality>150) H_line(13,Ypoz+4,5,Pen);  //œrednia jakoœæ
                      if (Qality>50) H_line(14,Ypoz+2,3,Pen);   //niska jakoœæ  
                      H_line(15,Ypoz,1,Pen);                    //jest/by³o po³¹czenie w badanym okresie
                      
                      
                      if (NeuronsList[(5-i)+NeuronsListLine].IndirectCon) V_line(18,Ypoz,2,Pen);
                      
                      
              }       
                           
            }
            
            if (NeuronsList[(5-i)+NeuronsListLine].NeuronType==NodeRF_Period)
              {
    
                if ((Dzielnik20<10) && (MainStage.StageI[(6-i)+NeuronsListLine].Port& 0x8000) && (((unsigned short)(MainStage.StageI[(6-i)+NeuronsListLine].Port))!=0x8AAA))
                {
                    CopyToCanvasPoint((char*)BateriaWyladowana, 12, Ypoz-1, 6,9);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);           
                }
              else
                {
                    char SlowMensurTMP=0;
                    
                    
                    //przepisanie kolejno bit po bicie TransQuality.SlowMensur. Po przepisaniu 0bit -okreœla bierz¹c¹ minutê, 1Bit -poprzedni¹ minutê, 2Bit jeszcze wczeœniejsz¹ itd.
                    char idx=TransQuality.SlowMensurIndex;
                    char Stan=0;
                    for (char bit=0; bit<7; bit++) 
                    {
                      Stan=(TransQuality.SlowMensur[(5-i)+NeuronsListLine]>>idx)&0x1;
                      SlowMensurTMP |= Stan<<bit; 
                      
                      if (idx>0) {idx--;} else {idx=7;}
                    }
                    
                    if (((SlowMensurTMP&0x1) &&  (SlowMensurTMP&0x2)) || ((SlowMensurTMP&0x2) &&  (SlowMensurTMP&0x4)))     H_line(12,Ypoz+6,7,Pen); //najwy¿sza jakoœæ  - je¿eli w dwuch z ostatnich trzech minut by³o po³¹czenie
                    if (SlowMensurTMP&0xF)                                                                                  H_line(13,Ypoz+4,5,Pen); //œrednia jakoœæ - je¿eli w jednym z ci¹gu ostatnich czterech minut by³o po³¹czenie
                    if (SlowMensurTMP&0x3F)                                                                                 H_line(14,Ypoz+2,3,Pen); //niska jakoœæ- je¿eli w jednym z ci¹gu ostatnich szeœciu minut by³o po³¹czenie  
                    if (SlowMensurTMP) H_line(15,Ypoz,1,Pen);                                                                                        //najni¿sza jakoœæ- je¿eli w jednym z ci¹gu ostatnich oœmiu minut by³o po³¹czenie  
                      
                    
                    if ((SlowMensurTMP) && (NeuronsList[(5-i)+NeuronsListLine].IndirectCon)) V_line(18,Ypoz,2,Pen);
                }  
                   
                   /*
                    LIC=0;
                    for (char k=0; k<5; k++)
                    {
                      if ((TransQuality.SlowMensur[(5-i)+NeuronsListLine]>>k)&0x1) LIC++;
                    }
                    if (LIC>0) 
                    {
                      H_line(15,Ypoz,1,1); // (char X, int Y, char lenght, char typ) 
                      H_line(14,Ypoz+2,3,1); // (char X, int Y, char lenght, char typ)                      
                    }
                    if (LIC>1)
                    {
                      H_line(13,Ypoz+4,5,1); // (char X, int Y, char lenght, char typ)  
                      H_line(12,Ypoz+6,7,1); // (char X, int Y, char lenght, char typ) 
                    }
                  }
                  */
                  
                  
              
              }
            
              
              #ifdef  DIRECT_Q_MESURE 
              static char Div=0;
            
              if (Div<10)
              {
               Div++; 
              }else{
                Div=0;
              
                    char txt[6];
                    sprintf(txt, "%d",  TransQuality.DirectkMensur[(5-i)+NeuronsListLine]);
                    Label(txt,0,22,27+i*12,8,23,16,105,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
              }  
               #endif
            
              
          }
      }
      
      
    
   }//________________________________________________________________________



    
      
      
void NeuronNrChange_Paint( int *ValToChange)
      { 
        List_Paint(" ID ",1,108,0,0,3," ",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      
        
   
        if (VCTT_ill==0) for (char i=0; i<8; i++) ValueChangeTxtTmp[i]=0;
        
        H_line(12,106,39,1);
        H_line(12,96,39,1);
        V_line(11,97,9,1); 
        V_line(50,97,9,1); 


        
        Label(ValueChangeTxtTmp,1,48,97,8,34,0,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)        
                      

        
        char txt[2];
        txt[1]=0;
        char ind=0;
        for (char j=0; j<3; j++)
          for (char i=0; i<3; i++)
          {
            ind++;
            CharToStr(ind, txt, 1); //(char Value, char *Vstring, char ill);
            Button(txt,2+20*i,76-(19*j),18,17);
          }
        //Button(" ",2+20*0,76-(19*3),18,17);
        Button("0",2+20*1,76-(19*3),18,17);
        //Button("-",2+20*2,76-(19*3),18,17);
        
        
        Button("C ",3,4,17,13);
        if ((VCTT_ill==0) || (VCTT_ill>6)) Button("Ok ",22,4,38,13);
      }



void NeuronNrChange_Evant(int *ValToChange)
      { 
       int ValueTmp=0; 
       char ind=0;
       char txt[2];
       char Refresh=0;
 
       
       
       

       txt[1]=0;
        if ((X_up>46) && (X_up<63) && (Y_up>112) && (Y_up<127))                    //"X"
        {
          
             for (char i=0; i<8; i++) ValueChangeTxtTmp[i]=0;
                ValueTmp=0;
                VCTT_ill=0;
                VCTT_point_flag=0;
           
                if (!(AT91C_IFLASH_MEM->Config.SoundOff)) {AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; ButtonBipCounter=g_counter+8;}
                
                

            MenuLevel=(MenuLevel/10);
            Refresh=1;
        } else {
          
                 

         
             
          for (char j=0; j<3; j++)
          {
          for (char i=0; i<3; i++)
          {
            ind++;
            CharToStr(ind, txt, 1); //(char Value, char *Vstring, char ill);
            if ((Button(txt,2+20*i,76-(19*j),18,17)==1) && (VCTT_ill<7)  ){ValueChangeTxtTmp[VCTT_ill++]=48+ind;   Refresh=1;}
            
           }
          }

           if ((Button("0",2+20*1,76-(19*3),18,17)==1) && (VCTT_ill<7) ) {ValueChangeTxtTmp[VCTT_ill++]=48;    Refresh=1;}

            
            
        
            
            if (Button("C ",3,4,17,13)==1)
            {
              if (VCTT_ill) VCTT_ill--;
              ValueChangeTxtTmp[VCTT_ill]=0;

              Refresh=1;
            }
          
        
            int mnz=1;     
            char prs=0;

            //zapamiêtanie wartoœci
            if ((VCTT_ill==0) || (VCTT_ill>6))
            if (Button("Ok ",22,4,38,13)==1)
            {
              

                for (char i=0; i<VCTT_ill-prs; i++)
                {
                   ValueTmp+=(ValueChangeTxtTmp[VCTT_ill-i-1]-48)*mnz;
                   mnz*=10;
                }
                
                TransQuality.Mensur[NeuronNrChange-1]=0;
                TransQuality.SlowMensur[NeuronNrChange-1]=0;
                TransQuality.QuickMensur[NeuronNrChange-1]=0;
                
                ProgramChange=1;
                MenuLevel/=10; 
                 
                *ValToChange=ValueTmp;
                
                for (char i=0; i<8; i++) ValueChangeTxtTmp[i]=0;
                ValueTmp=0;
                VCTT_ill=0;
                VCTT_point_flag=0;
                
              
              
              
              Refresh=1;
              RefreshNeuronsList(&gProg);
              SetTask(&gProg);
              
            }
       
          
        }
        
        
        if (Refresh==1) MenuPaint();
        
}





void Info_Paint(void)
{          
       char txt[8];
       char labsize;
       List_Paint("Info ",1,108,0,0,3," ",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      

       Label("Model: ",0,3,100,15,58,0,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)        
       Label((char*)&identyfikacja,0,10,89,15,40,0,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)        
       H_line(1,85,62,2);      
       Label("Node ID: ",0,3,75,15,58,0,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)        
       sprintf(txt, "%d",RF_Status.RF_ID_R);
       Label(txt,0,10,64,15,40,0,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)        
       H_line(1,60,62,2);    
       Label("Sys. version: ",0,3,50,15,58,0,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)        
       Label((char*)&system,0,10,39,15,40,0,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)        
       H_line(1,35,62,2);  
       Label("Trans. period: ",0,3,25,15,58,0,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)        
       //labsize=sIntToStr(RF_Period*10,txt);
       labsize=sIntToStr((10000/(CyklPerSek)),txt);
       txt[labsize++]='m'; txt[labsize++]='s';
       Label(txt,0,10,14,labsize,40,0,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)        
       H_line(1,10,62,2);  
       
  
       
}//________________________________________________________________________

    


void Info_Refresh(void)
{          
  static int IRdiv; 
  
  if (IRdiv<20) {IRdiv++;} else {
       IRdiv=0;
       char txt[8];
       char labsize;

      labsize=sIntToStr((10000/(CyklPerSek)),txt); 
       
     
      
       txt[labsize++]='m'; txt[labsize++]='s';
       Label(txt,0,10,14,labsize,40,0,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)        
  }
}//________________________________________________________________________



//*-----------------------------------------------------------------------------
void Info_Event(void)
      {
        if ((X_up>46) && (X_up<63) && (Y_up>112) && (Y_up<127))                    //"X"
        {
          MenuLevel=(MenuLevel/10);
          MenuPaint();
        }  

}//________________________________________________________________________
    





void ThermostatAutoSet_Paint(_Screen* Screen)
{          
       List_Paint("Thermo",1,108,0,0,3," ",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      

       //Label("Model: ",0,3,100,15,58,0,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)        
      char txt[5]; txt[1]=0; 
      txt[0]=31;//dó³ 
      Button(txt,2,89,18,18);
      txt[1]=0; txt[0]=30;//góra 
      Button(txt,43,89,18,18);
      Label("T1",2,20,98,2,21,0,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)        
      
      sIntToStr(Screen->Thermo.Temperature[0],txt); 
      Label(txt,2,20,89,4,21,0,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)        
      

      
      
      txt[1]=0; txt[0]=31;//dó³
      Button(txt,2,69,18,18);
      txt[1]=0; txt[0]=30;//góra 
      Button(txt,43,69,18,18);
      Label("T2",2,20,78,2,21,0,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)   
      sIntToStr(Screen->Thermo.Temperature[1],txt); 
      Label(txt,2,20,69,4,21,0,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)   
       
      
      
      
      char Ychart=54;
      
      H_line(1,Ychart+12,62,1);  
      H_line(1,Ychart+11,62,1);  
   
      

      
      for (char i=0; i<7; i++)
      {
        Label(&(Speech[g_leng].PWSCPSN[i]),2,4+i*8,Ychart+2,1,6,0,128,((Screen->Thermo.Week>>i) &0x1));//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)   
      }
      
      
      for (char i=0; i<24; i++)
      {
         V_line(8+(i*2),Ychart-7,4+((Screen->Thermo.Chart[0]>>i) & 0x1 )*2,1);  
      }
      

      
      Label("0",0,4+1*2,Ychart-17,1,5,0,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)   
      V_line(6+1*2,Ychart-9,1,1); 
      Label("6",0,6+6*2,Ychart-17,1,5,0,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)   
      V_line(6+6*2,Ychart-9,1,1); 
      Label("12",0,4+12*2,Ychart-17,2,5,0,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)   
      V_line(6+12*2,Ychart-9,1,1); 
      Label("18",0,4+18*2,Ychart-17,2,5,0,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)   
      V_line(6+18*2,Ychart-9,1,1); 
      Label("23",0,2+24*2,Ychart-17,2,5,0,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)   
      V_line(6+24*2,Ychart-9,1,1);             
      
      
      Ychart=21;
      H_line(1,Ychart+12,62,1); 
       H_line(1,Ychart+11,62,1);  
      
      for (char i=0; i<7; i++)
      {
        Label(&(Speech[g_leng].PWSCPSN[i]),2,4+i*8,Ychart+2,1,6,0,128,((~(Screen->Thermo.Week>>i)) &0x1));//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)   
      } 
       
      for (char i=0; i<24; i++)
      {
         V_line(8+(i*2),Ychart-7,4+((Screen->Thermo.Chart[1]>>i) & 0x1 )*2,1);  
      }
      
      Label("0",0,4+1*2,Ychart-17,1,5,0,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)   
      V_line(6+1*2,Ychart-9,1,1); 
      Label("6",0,6+6*2,Ychart-17,1,5,0,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)   
      V_line(6+6*2,Ychart-9,1,1); 
      Label("12",0,4+12*2,Ychart-17,2,5,0,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)   
      V_line(6+12*2,Ychart-9,1,1); 
      Label("18",0,4+18*2,Ychart-17,2,5,0,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)   
      V_line(6+18*2,Ychart-9,1,1); 
      Label("23",0,2+24*2,Ychart-17,2,5,0,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)   
      V_line(6+24*2,Ychart-9,1,1);             
      
}//________________________________________________________________________

    
//*-----------------------------------------------------------------------------
void ThermostatAutoSet_Event(_Screen* Screen)
      {
   
        if (List_Event(X_up, Y_up, X_down, Y_down, 0 , 15, 0,0)<0)
        {
           if (ProgramChange) 
          {
              
              if (((gProg.Screen[CurrentSheet].Thermo.Chart[DateTime.DayOfWeek]>>DateTime.Hour) & 0x1)==1)
              {
                gProg.Screen[CurrentSheet].Thermo.SetTemperature=gProg.Screen[CurrentSheet].Thermo.Temperature[0]; 
              }else{
                gProg.Screen[CurrentSheet].Thermo.SetTemperature=gProg.Screen[CurrentSheet].Thermo.Temperature[1]; 
              }
              
              ThermostatsTimeChartExecute(); 
              ThermostatsExecute();
              
              MenuLevel=(MenuLevel*10+0); 
          }else{
            MenuLevel=(MenuLevel/10);
          }
          MenuPaint();
        } else if ((Y_down<112) || (Y_up<112))
        {  
        
               char Refresh=0; 
              
              char txt[5]; txt[1]=0; 
              txt[0]=31;//dó³ 
              if ((Button(txt,2,89,18,18)==2) && (Screen->Thermo.Temperature[0]>0)) {Screen->Thermo.Temperature[0]--; Refresh=1; ProgramChange=1;}
              txt[1]=0; txt[0]=30;//góra 
              if ((Button(txt,43,89,18,18)==2) && (Screen->Thermo.Temperature[0]<500)) {Screen->Thermo.Temperature[0]++; Refresh=1; ProgramChange=1;}
              
              txt[1]=0; txt[0]=31;//dó³
              if ((Button(txt,2,69,18,18)==2) && (Screen->Thermo.Temperature[1]>0)) {Screen->Thermo.Temperature[1]--;Refresh=1; ProgramChange=1;}
              txt[1]=0; txt[0]=30;//góra 
              if ((Button(txt,43,69,18,18)==2) && (Screen->Thermo.Temperature[1]<5000)) {Screen->Thermo.Temperature[1]++;Refresh=1; ProgramChange=1;}

              //je¿eli klikniêto obszar Chart 0
              if ((Y_up>34) && (Y_up<62) && (X_up>0))  {CurentChartModyfi=0; Refresh=1; MenuLevel=(MenuLevel*10+1);}
              if ((Y_up>0) && (Y_up<30) && (X_up>0))  {CurentChartModyfi=1; Refresh=1; MenuLevel=(MenuLevel*10+1);}
               
              if ((X_down>0) && (Y_down>0)) Refresh=1;
              if (Refresh==1) MenuPaint();  
              if ((Y_down>34) && (Y_down<62) && (X_down>0))  {
                  V_line(1,32,33,1);
                  V_line(61,32,33,1);
                  H_line(1,64,62,1);
                  H_line(1,34,62,1);
               }
              if ((Y_down>0) && (Y_down<30) && (X_down>0))  {
                  V_line(1,3,30,1);
                  V_line(61,3,30,1);
                  H_line(1,31,62,1);
                  H_line(1,2,62,1);
               }
              
        }
}//________________________________________________________________________
    





char ThermHourModyfi;
void ThermostatChartSet_Paint(_Screen* Screen, char Chart)
{          
       List_Paint("Thermo",1,108,0,0,3," ",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      


      
      char day=0;
      char hlight=0;

      for (char row=0; row<3; row++)
      for (char col=0; col<3; col++)
      { 
        if (++day<8)
        {
            H_line(4+col*18,105-row*14,19,1);
            H_line(4+col*18,105-(row+1)*14,19,1);
            V_line(4+col*18,105-(row+1)*14,14,1);
            V_line(4+(col+1)*18,105-(row+1)*14,14,1);
            
            if (Chart==1) hlight=(~(Screen->Thermo.Week>>(day-1)) &0x1); else hlight=(((Screen->Thermo.Week>>(day-1))) &0x1);
            Label((char*)DeyOfWeekStr[g_leng][day],2,6+col*18 ,105-(row+1)*14+3,2,14,0,128,hlight);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)   

        }
      }
      
     
       
       
      
      char Ychart=44;
      
      H_line(1,Ychart+14,62,1);  
      H_line(1,Ychart+13,62,1);  
  
      
      for (char i=0; i<24; i++)
      {
         V_line(8+(i*2),Ychart,5+((Screen->Thermo.Chart[Chart]>>i) & 0x1 )*3,1);  
      }
      
      Button("<",4,Ychart-32,17,18); 
      char txt[2]; txt[1]=0; 
      if ((Screen->Thermo.Chart[Chart]>>ThermHourModyfi) & 0x1) txt[0]=31; else txt[0]=30; 
      Button(txt,23,Ychart-32,17,18); 
      Button(">",42,Ychart-32,17,18); 
      
      
      
}//________________________________________________________________________

    
//*-----------------------------------------------------------------------------
void ThermostatChartSet_Event(_Screen* Screen, char Chart)
      {
        char Repaint=0;
        
        if (List_Event(X_up, Y_up, X_down, Y_down, 0 , 15, 0,0)<0)
        {
          MenuLevel=(MenuLevel/10);
          MenuPaint();
        } else if ((Y_down<112)  || (Y_up<112))
        {  
           char Ychart=44;
          
           if (Button("<",4,Ychart-32,17,18)==1) if (ThermHourModyfi>0) {ThermHourModyfi--; Repaint=1; ProgramChange=1;}
           char txt[2]; txt[1]=0; 
            if ((Screen->Thermo.Chart[Chart]>>ThermHourModyfi) & 0x1) txt[0]=31; else txt[0]=30; 
           if (Button(txt,23,Ychart-32,17,18)==1) 
           {
              if ((Screen->Thermo.Chart[Chart]>>ThermHourModyfi) & 0x1)
              {
                Screen->Thermo.Chart[Chart] &= ~(1<<ThermHourModyfi);
                Repaint=1;
              }else{
                Screen->Thermo.Chart[Chart] |=  (1<<ThermHourModyfi);
                Repaint=1;
              }
              ProgramChange=1;
           }
           if (Button(">",42,Ychart-32,17,18)==1) {if (ThermHourModyfi<23) ThermHourModyfi++; Repaint=1; ProgramChange=1;}
      
      
            char day;
            day=0;
            for (char row=0; row<3; row++)
            for (char col=0; col<3; col++)
            { 
              if (++day<8)
              {
                  if ((X_up>4+col*18) && (X_up<4+(col+1)*18) && (Y_up<105-row*14) && (Y_up>105-(row+1)*14))
                  {
                    if ((Screen->Thermo.Week>>(day-1)) & 0x1)  //jezeli dzieñ aktywny to wy³acz je¿eli nie to w³¹cz
                    {
                       Screen->Thermo.Week &= ~(1<<(day-1));
                    }else{
                      Screen->Thermo.Week |= (1<<(day-1));
                    }
                    Repaint=1;
                    ProgramChange=1;
                  }

              }
            }
           
           
           if (Repaint==1) MenuPaint();      
          
        }
}//________________________________________________________________________
    
      
      
void ThermostatChartSet_Refresh(_Screen* Screen, char Chart)
{          
   static char TCSR_Count;
   char kolor=0;
   char Ychart=44;
   if (TCSR_Count<3) TCSR_Count++; else TCSR_Count=0;
   if (TCSR_Count) kolor=1;
   V_line(8+(ThermHourModyfi*2),Ychart,5+((Screen->Thermo.Chart[Chart]>>ThermHourModyfi) & 0x1 )*3,kolor);  

   char txt[3]; txt[1]=0; txt[2]=0;
   CharToStr(ThermHourModyfi,txt,2);  //(char Value, char *Vstring, char ill) 
   Label(txt,2,ThermHourModyfi*2+2,Ychart-10,2,12,0,128,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)   

      
       
      
}//________________________________________________________________________








//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    Alarm_Paint
//* Funkcja strony g³ównej menu
//*-----------------------------------------------------------------------------
//* Efekt widoczny po odœwierzeniu LCD (RepaintLCD)
//*-----------------------------------------------------------------------------
void Alarm_Paint(char CurrentSheet)
{
  
  


    

      if ((AlarmScreenAction[CurrentSheet].Active==0) && (AlarmScreenAction[CurrentSheet].SecToActive==0))
      {
        if (CurrentSheet!=0)  List_Paint(Speech[g_leng].Termo_,1,108,0,0,3," ",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      
        Button(Speech[g_leng].WlOchrone,2,40,59,50);
        Button(Speech[g_leng].ZmienHaslo,2,4,59,13);
      }
  
      if ((AlarmScreenAction[CurrentSheet].Active==1) && (AlarmScreenAction[CurrentSheet].SecToActive==0))
      {
         
      }
}//_______________________ Koniec funkcji Alarm_Paint __________________________
  



//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    Alarm_Event
//* Funkcja strony g³ównej menu
//*-----------------------------------------------------------------------------
//*-----------------------------------------------------------------------------


void Alarm_Event(char CurrentSheet)
{
  char Repaint=0;


  
  
 if ((CurrentSheet>0) && (List_Event(X_up, Y_up, X_down, Y_down, 0 , 15, 0,0)==-1))
    {
       if (SheetHistoryIndex>0)
          {
             CurrentSheet=SheetHistory[--SheetHistoryIndex];
          } 
         Repaint=1; 
    }else{

      if ((AlarmScreenAction[CurrentSheet].Active==0) && (AlarmScreenAction[CurrentSheet].SecToActive==0))
      {
        if (Button(Speech[g_leng].WlOchrone,2,40,59,50)==1) {
         AlarmScreenAction[CurrentSheet].SecToActive=AlarmTimeToActive;
         AlarmScreenAction[CurrentSheet].Active=0;
         Repaint=1;
        } 
         if (Button(Speech[g_leng].ZmienHaslo,2,4,59,13)==1)
         {
            MenuLevel=15;
            Repaint=1;
         }
        
      }
      
            
    }
 
 
 
  if (Repaint==1) MenuPaint();
}//_______________________ Koniec funkcji Alarm_Event __________________________
  





//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    Alarm_Refresh
//* Funkcja strony g³ównej menu
//*-----------------------------------------------------------------------------
//*-----------------------------------------------------------------------------
void Alarm_Refresh(char CurrentSheet)
{

  char dl;
  char txt[5];
  static int LastCount;


  if ((AlarmScreenAction[CurrentSheet].Active==0) && (AlarmScreenAction[CurrentSheet].SecToActive>0) && (g_counter>LastCount+1000))
      {
        CleanCanvas(); 
        AlarmScreenAction[CurrentSheet].SecToActive--;
        Label(Speech[g_leng].AktywacjaZa,2,1,80,15,59,0,105,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw)   

        
        LastCount=g_counter;
        dl=CharToStr(AlarmScreenAction[CurrentSheet].SecToActive, txt,2);
        Label20(txt,2,0,45,dl,64,0,105,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 

        if (AlarmScreenAction[CurrentSheet].SecToActive==0)
        {
          MenuLevel=14;
          AlarmScreenAction[CurrentSheet].Active=1;
          MenuPaint(); 
        }
      }

}//_______________________ Koniec funkcji Alarm_Refresh __________________________
  







//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    Alarm_Paint
//* Funkcja strony g³ównej menu
//*-----------------------------------------------------------------------------
//* Efekt widoczny po odœwierzeniu LCD (RepaintLCD)
//*-----------------------------------------------------------------------------
void AlarmPass_Paint(char CurrentSheet)
{

          Number_Paint(Speech[g_leng].Wpisz,"C","E");
          Label(Speech[g_leng].Haslo,0,3,102,15,58,0,127,0);
          CleanCanvasArea(0,113,64,15,0);//(char X, char Y, char X_Lenght, char Y_Lenght, char Colour)
      
}//_______________________ Koniec funkcji AlarmPass_Paint __________________________
  



//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    AlarmPass_Event
//*-----------------------------------------------------------------------------
//*-----------------------------------------------------------------------------
void AlarmPass_Event(char CurrentSheet)
{
  char Repaint=0;


   int ans;
        char roz=0;
          
        ans=Number_Event(X_up, Y_up, X_down, Y_down); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        if (ans>0)          {
            
            if (InsertPass1[4]<=3) 
            { 
                if ((ans<10) || (ans==11)) 
                {
                  if (ans<10) InsertPass1[InsertPass1[4]]=ans+48;
                  if (ans==11) InsertPass1[InsertPass1[4]]=ans+37;
                  InsertPass1[4]++;
                }
                
            }
            if ((ans==10) && (InsertPass1[4]>0) ) {InsertPass1[4]--; InsertPass1[InsertPass1[4]]=0;}
            
            
           char InsertPassVar=0;
           for (char i=0; i<=3; i++)  pass_tmp[i]=0;
           while ((InsertPass1[InsertPassVar]) &&  (InsertPassVar<4))
           {
             pass_tmp[InsertPassVar]=42; //42="*"
             InsertPassVar++;
           }

            
            //Sprawdzenie has³a
            if (ans==12)
            {
              roz=0;
              for (int i=0; i<=3; i++)
              {
                  if ((InsertPass1[i]!=AT91C_IFLASH_MEM->Config.AlarmPass[CurrentSheet][i])) roz=1;
                  if  ((AT91C_IFLASH_MEM->Config.AlarmPass[CurrentSheet][0]==0) && 
                      (AT91C_IFLASH_MEM->Config.AlarmPass[CurrentSheet][1]==0) &&
                      (AT91C_IFLASH_MEM->Config.AlarmPass[CurrentSheet][2]==0) &&
                      (AT91C_IFLASH_MEM->Config.AlarmPass[CurrentSheet][3]==0) &&
                      (InsertPass1[0]=='0') && 
                        (InsertPass1[1]=='0') && 
                          (InsertPass1[2]=='0') && 
                            (InsertPass1[3]=='0'))
                      {
                        roz=0;
                      }
              }
              
              if (roz==0)                                 //je¿eli poprawne haslo
              {
                 for (int i=0; i<=3; i++) InsertPass1[i]=0;
                 InsertPass1[4]=0;
                 MESSAGE(Speech[g_leng].OchronaWyl); 
                 AlarmScreenAction[CurrentSheet].Active=0;
                 AlarmOn=0; 
                 MenuLevel=1;
              } 
              else
              {
                 //informacja o b³ednym haœle
                for (int i=0; i<=7; i++) InsertPass1[i]=0;
                 InsertPass1[4]=0;
                 MESSAGE(Speech[g_leng].BledneHaslo); 
              }

            }
           
            if ((ans>=1) && (ans<=11)) 
            {
              MenuPaint();
              Label(pass_tmp,2,4,90,InsertPass1[4],55,0,127,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
            }
            
          }  
          else if (ans==0) 
            {
              MenuPaint();
              Label(pass_tmp,2,4,90,InsertPass1[4],55,0,127,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
            } 
   
 
  if (Repaint==1) MenuPaint();
}//_______________________ Koniec funkcji AlarmPass_Event __________________________
  






void AlarmPass_Refresh(void)
{
 
         //wskaŸnik ¿ycia
          if (CurrentSheet==0) 
          {
          
            if ((MainSide_puls==0) || (MainSide_puls>3))
            { 
              
              char ekran1[2][13] = {
                         {0,0,224,24,232,244,244,244,232,24,224,0,0},
                          {0,0,0,3,2,5,5,5,2,3,0,0,0}
                          };
        
              char ekranSlave[2][13] = {
                          {0,0,224,248,152,124,180,220,56,248,224,0,0},
                          {0,0,0,3,3,7,5,7,3,3,0,0,0},
                          };
              
              if ((SlaveMode) )//je¿eli tryb slave
              {
                    for (char i=0; i<=12; i++)
                    {
                 
                    LCD_Canvas[0][i+115]=ekranSlave[0][i];
                    LCD_Canvas[1][i+115]=ekranSlave[1][i];
                    }
              }else{
                
                  for (char i=0; i<=12; i++)
                  {
                 
                    LCD_Canvas[0][i+115]=ekran1[0][i];
                    LCD_Canvas[1][i+115]=ekran1[1][i];
                  }
                }
             //if ((!g_SlaveConnect) && (SlaveMode)) H_line(2,121,7,0);
            }
            if ((MainSide_puls==1) )
            {
              
              if ((g_SlaveConnect) || (!SlaveMode))
              {
                char ekran2[2][13] = {
                          {240,28,230,250,251,253,253,253,251,250,230,28,240},
                          {1,7,12,11,27,23,23,23,27,11,12,7,1},
                          };
              
                for (char i=0; i<=12; i++)
                {
                 
                  LCD_Canvas[0][i+115]=ekran2[0][i];
                  LCD_Canvas[1][i+115]=ekran2[1][i];
                }
              }
        
            }
             
        
          if (g_Stop_Program==0)
          {  
           if (MainSide_puls>7) {MainSide_puls=0;} else {MainSide_puls++;}    
          }else{
            MainSide_puls=7; 
          }
           
          }

}//_______________________ Koniec funkcji AlarmPass_Refresh __________________________




extern char APC;
//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    Alarm_Paint
//* Funkcja strony g³ównej menu
//*-----------------------------------------------------------------------------
//* Efekt widoczny po odœwierzeniu LCD (RepaintLCD)
//*-----------------------------------------------------------------------------

void AlarmPassChange_Paint(char CurrentSheet)
{

          Number_Paint(Speech[g_leng].Wpisz,"C","E");
          if (APC==0)
          {
            Label(Speech[g_leng].ObecneHaslo,0,3,102,15,58,0,127,0);
          }else{
            Label(Speech[g_leng].NoweHaslo,0,3,102,15,58,0,127,0);
          }
          CleanCanvasArea(0,113,64,15,0);//(char X, char Y, char X_Lenght, char Y_Lenght, char Colour)
      
}//_______________________ Koniec funkcji AlarmPass_Paint __________________________
  



//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    AlarmPass_Event
//*-----------------------------------------------------------------------------
//*-----------------------------------------------------------------------------
void AlarmPassChange_Event(char CurrentSheet)
{
  short int Repaint=0;



  short int ans;
        short int roz=0;
          
        ans=Number_Event(X_up, Y_up, X_down, Y_down); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        if (ans>0)          {
            
            if (InsertPass1[4]<=3) 
            { 
                if ((ans<10) || (ans==11)) 
                {
                  if (ans<10) InsertPass1[InsertPass1[4]]=ans+48;
                  if (ans==11) InsertPass1[InsertPass1[4]]=ans+37;
                  InsertPass1[4]++;
                }
                
            }
            if ((ans==10) && (InsertPass1[4]>0) ) {InsertPass1[4]--; InsertPass1[InsertPass1[4]]=0;}
            
            
           short int InsertPassVar=0;
           for (char i=0; i<=3; i++)  pass_tmp[i]=0;
           while ((InsertPass1[InsertPassVar]) &&  (InsertPassVar<4))
           {
             pass_tmp[InsertPassVar]=42; //42="*"
             InsertPassVar++;
           }

          
            //Sprawdzenie has³a
            if (ans==12)
            {
              
              roz=0;
              for (int i=0; i<=3; i++)
              {
                if (APC==0)
                {
                  if ((InsertPass1[i]!=AT91C_IFLASH_MEM->Config.AlarmPass[CurrentSheet][i])) roz=1;
                  if  ((AT91C_IFLASH_MEM->Config.AlarmPass[CurrentSheet][0]==0) && 
                      (AT91C_IFLASH_MEM->Config.AlarmPass[CurrentSheet][1]==0) &&
                      (AT91C_IFLASH_MEM->Config.AlarmPass[CurrentSheet][2]==0) &&
                      (AT91C_IFLASH_MEM->Config.AlarmPass[CurrentSheet][3]==0) &&
                      (InsertPass1[0]=='0') && 
                        (InsertPass1[1]=='0') && 
                          (InsertPass1[2]=='0') && 
                            (InsertPass1[3]=='0'))
                      {
                        roz=0;
                      }
                 
                  
                }
              }
              
              
              if (roz==0)                                 //je¿eli poprawne haslo
              {
                 
                 

                 if (APC==1)
                 {
                    APC=0;
                    MenuLevel=1;
                    
                    unsigned int Save = __get_interrupt_state();  
                    __disable_interrupt();
                            FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.AlarmPass[CurrentSheet])), InsertPass1, 4);
                    __set_interrupt_state(Save);
                    __enable_interrupt(); 
                    
                 }else{
                  APC=1; 
                 }
                 for (int i=0; i<=3; i++) InsertPass1[i]=0;
                 InsertPass1[4]=0;
                 Repaint=1;
              } 
              else
              {
                 //informacja o b³ednym haœle
                for (int i=0; i<=7; i++) InsertPass1[i]=0;
                 InsertPass1[4]=0;
                 MESSAGE(Speech[g_leng].BledneHaslo); 
              }
              

            }
           
            if ((ans>=1) && (ans<=11)) 
            {
              MenuPaint();
              Label(pass_tmp,2,4,90,InsertPass1[4],55,0,127,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
            }
            
          }  
          else if (ans==0) 
            {
              MenuPaint();
              Label(pass_tmp,2,4,90,InsertPass1[4],55,0,127,0);//(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
           } 
 

 


 
  if (Repaint==1) MenuPaint();
}//_______________________ Koniec funkcji AlarmPass_Event __________________________
  




      //*-----------------------------------------------------------------------
      void Node_Paint(char CurrentNode)
      {
        char txt[8];
        txt[0]='N'; txt[1]='o';txt[2]='d';txt[3]='e';txt[4]=' ';
        char ind=CharToStr(CurrentNode, txt+5, 2);
        
        if ((TransQuality.Mensur[CurrentNode-1]>0) || (TransQuality.SlowMensur[CurrentNode-1]>0) || (TransQuality.QuickMensur[CurrentNode-1]>0))
        {
            List_Paint(txt,7,15,0,0,3, Speech[g_leng].ZmID, Speech[g_leng].UsunID, Speech[g_leng].ZmOpis, Speech[g_leng].TestTrans, Speech[g_leng].StanWeWy, Speech[g_leng].Analogi  ,"Info",0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])
        }else{
            List_Paint(txt,4,15,0,0,3, Speech[g_leng].ZmID, Speech[g_leng].UsunID, Speech[g_leng].ZmOpis, Speech[g_leng].TestTrans, 0 ,0,0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])

        }   
        
      }//_______________________ Koniec funkcji MainSide_Paint __________________________



      //*-----------------------------------------------------------------------
      void Node_Event(char CurrentNode)
      {
        int ans;
       
        if ((TransQuality.Mensur[CurrentNode-1]>0) || (TransQuality.SlowMensur[CurrentNode-1]>0) || (TransQuality.QuickMensur[CurrentNode-1]>0))
        {
          ans=List_Event(X_up, Y_up, X_down, Y_down, 7 , 15, 0,0); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
        }else{
          ans=List_Event(X_up, Y_up, X_down, Y_down, 4 , 15, 0,0); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
        }
        if (ans)
          {
          if ((ans>0) && (ans<=7)) 
          {
            RF_NodeTest.Sent=0;
            RF_NodeTest.SentSyn=0;   
            RF_NodeTest.Recived=0;
            if (ans==2) //je¿eli usun ID
            {
              gProg.NeuronsIdTab[CurrentNode-1]=0;
              NeuronsList[CurrentNode-1].NeuronID=0;
              NeuronsList[CurrentNode-1].NeuronType=NoneNodeType;
              ProgramChange=1;
              RefreshNeuronsList(&gProg);
              SetTask(&gProg);
              TransQuality.SlowMensur[CurrentNode-1]=0;
              TransQuality.QuickMensur[CurrentNode-1]=0;
              TransQuality.Mensur[CurrentNode-1]=0;
              MenuLevel=(MenuLevel/10);
            }
            else if (ans==3) //je¿eli zmien opis
            {
              MenuLevel=(MenuLevel/10)*10+2;
            }
            else{
              MenuLevel=(MenuLevel*10+ans);
            }
          }
           if (ans<0) {
            MenuLevel=(MenuLevel/10);
            NeuronNrChange=0;
            NeuronIdChange=0;
          }
          MenuPaint();
          } 
      }//_______________________________________________________________________

       



      //*-----------------------------------------------------------------------
      void NodeTestTrans_Paint(char CurrentNode)
      {  
           List_Paint(Speech[g_leng].TestTrans,1,108,0,0,3," ",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      
           RF_NodeTest.NodeNr=CurrentNode;
           
            char txt[10];
           
          //H_line(0,110,63,1); // (char X, int Y, char lenght, char typ)
           Label(Speech[g_leng].Dane_wyslane,0,2,100,15,8,1,127,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
           H_line(0,80,63,1); // (char X, int Y, char lenght, char typ)
           Label(Speech[g_leng].Dane_odebrane,0,2,70,15,8,1,127,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
           H_line(0,50,63,1); // (char X, int Y, char lenght, char typ)
           Label(Speech[g_leng].Dane_utracone,0,2,40,15,8,1,127,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 

           short Sent=RF_NodeTest.SentSyn; 
           short Recived=RF_NodeTest.Recived; 
           
           sprintf(txt, "%d", Sent);
           Label(txt,1,55,90,10,40,1,127,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
                  
           sprintf(txt, "%d", Recived);
           Label(txt,1,55,60,10,40,1,127,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
                  
           sprintf(txt, "%d", Sent-Recived);
           Label(txt,1,55,30,10,40,1,127,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          
           
      }//_______________________________________________________________________ 

                        

      //*-----------------------------------------------------------------------
      short ValNTT=0;
      void NodeTestTrans_Event(char CurrentNode)
      {
        int ans;
       
        ans=List_Event(X_up, Y_up, X_down, Y_down, 0 , 15, 0,0); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        if (ans)
          {
          if (ans<0) {
            ValNTT=0;
            MenuLevel=(MenuLevel/100);
           NeuronNrChange=0;
           NeuronIdChange=0;
           RF_NodeTest.NodeNr=0;
           RF_NodeTest.Sent=0;
           RF_NodeTest.SentSyn=0;   
           RF_NodeTest.Recived=0;
          }
          MenuPaint();                ;
          } 
      }//_______________________________________________________________________





      //*------------------------------------------------³-----------------------
      void NodeTestTrans_Refresh(char CurrentNode)
      {  
           char txt[10];
           char il;
           static char Dziel8;
           if (Dziel8<8) {Dziel8++;}else
           {
              Dziel8=0;
              short Sent=RF_NodeTest.SentSyn; 
              short Recived=RF_NodeTest.Recived; 
           
              sprintf(txt, "%d", Sent);
              Label(txt,1,55,90,10,40,1,127,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
                  
              sprintf(txt, "%d", Recived);
              Label(txt,1,55,60,10,40,1,127,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
                  
              sprintf(txt, "%d", Sent-Recived);
              Label(txt,1,55,30,10,40,1,127,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
          
           }
      }//_______________________________________________________________________ 



      char FirstNodeStanIO_Paint=0;
      //*-----------------------------------------------------------------------
      void NodeStanIO_Paint(char CurrentNode)
      {  
            char txt[8];
        txt[0]='N'; txt[1]='o';txt[2]='d';txt[3]='e';txt[4]=' ';
        char ind=CharToStr(CurrentNode, txt+5, 2);
        
           List_Paint(txt,1,108,0,0,3," ",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      
 /*
           CleanCanvasArea(14,6,2,95,1);
           CleanCanvasArea(14+32,6,2,95,1);

           Label("DI",2,2,99,2,25,1,127,1);
           Label("DO",2,34,99,2,25,1,127,1); 
   */        
           NodeStanIO_Refresh(CurrentNode);
           
           FirstNodeStanIO_Paint=1;
           AddRfTask(SQ, CurrentNode, 0);                //Dodaj rozkaz do tablicy zadañ RF
      }//_______________________________________________________________________ 




      //*-----------------------------------------------------------------------
      void NodeStanIO_Event(char CurrentNode)
      {
        int ans;
        char NP;
       
        ans=List_Event(X_up, Y_up, X_down, Y_down, 0 , 15, 0,0); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        
        
        NP=0;
        for (char j=0; j<2; j++) 
          for (char i=0; i<8; i++)
            {  
              
              if ((X_up>34+15*j) && (X_up<34+11+15*j) && (Y_up>87-i*12) && (Y_up<87+11-i*12)) 
              {
                if  ((MainStage.StageQ[CurrentNode].Port>>NP)&0x1)
                {
                   MainStage.StageQ[CurrentNode].Port &= ~(1<<NP);
                }else{
                   MainStage.StageQ[CurrentNode].Port |= (1<<NP);
                }
                MainStage.SendStageChange[CurrentNode]=ChangeStageFlag;
              }
             NP++;
            }
        
        
        
        if (ans)
          {
          if (ans<0) {
            MenuLevel=(MenuLevel/100);
           NeuronNrChange=0;
           NeuronIdChange=0;
           FirstNodeStanIO_Paint=0;
           SetTask(&gProg);   //usuniêcie nie potrzebnie dodanych zadañ
          }
          MenuPaint();
          } 
        
        
        
        
      }//_______________________________________________________________________



      
      
      


      //*-----------------------------------------------------------------------
      void NodeStanIO_Refresh(char CurrentNode)
      {  
           char Txt[6];
        
           static char Dziel5=5;
           if (MainStage.StageI[CurrentNode].Port !=(unsigned short int )0x8AAA) Dziel5=6;
           if (Dziel5<5) {Dziel5++;}else
           {
             Dziel5=0;
              if (MainStage.StageI[CurrentNode].Port !=(unsigned short int )0x8AAA) //je¿eli dane wiarygodne
               {
             
                           
                           CleanCanvasArea(1,3,61,95,0);
                           CleanCanvasArea(14,6,2,95,1);
                           CleanCanvasArea(14+32,6,2,95,1);
                
                           Label("DI",2,2,99,2,25,1,127,1);
                           Label("DO",2,34,99,2,25,1,127,1); 

                            char NP=0;
                            for (char j=0; j<2; j++) 
                            for (char i=0; i<8; i++)
                            {
                              if  ((MainStage.StageI[CurrentNode].Port>>NP)&0x1)
                              {
                                CopyToCanvasPoint((char*)&Frame[0][0],2+15*j,87-i*12,11,11);  //(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght)
                                CharToStr (NP, Txt, 2);
                                Label(Txt,2,3+15*j,88-i*12,2,8,1,127,1);  
                              } else{
                                CopyToCanvasPoint((char*)&Frame[0][0],2+15*j,87-i*12,11,11); 
                                CharToStr (NP, Txt, 2);
                                Label(Txt,2,3+15*j,88-i*12,2,8,1,127,0);  
                              }
                              NP++;
                            }
                            
                            NP=0;
                            for (char j=0; j<2; j++) 
                            for (char i=0; i<8; i++)
                            {  
                              if  ((MainStage.StageQ[CurrentNode].Port>>NP)&0x1)
                              {
                                CopyToCanvasPoint((char*)&Frame[0][0],34+15*j,87-i*12,11,11);  //(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght)
                                CharToStr (NP, Txt, 2);
                                Label(Txt,2,34+1+15*j,88-i*12,2,8,1,127,1);  
                              } else{
                                CopyToCanvasPoint((char*)&Frame[0][0],34+15*j,87-i*12,11,11); 
                                CharToStr (NP, Txt, 2);
                                Label(Txt,2,34+1+15*j,88-i*12,2,8,1,127,0);  
                              }
                             NP++;
                            }
                  
                  
                  }//je¿eli dane niewiarygodne - poinformuj o oczekiwaniu
               else
               {
                    CleanCanvasArea(1,3,61,95,0);

                    
                    Label(Speech[g_leng].Czekaj,2,3,80,10,57,1,127,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
                    
                    H_line(1,50,61,1); // (char X, int Y, char lenght, char typ)
                    Label("Reason:",0,3,40,10,30,1,127,0);
                    Label("Unreliable",2,3,27,10,57,1,127,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
                    Label("data.",2,3,18,10,57,1,127,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
               }
     
           }
      }//_______________________________________________________________________ 
                                                                   


 
 



      //*-----------------------------------------------------------------------
      void NodeWeAnalog_Paint(char CurrentNode)
      {  
            char txt[8];
        txt[0]='N'; txt[1]='o';txt[2]='d';txt[3]='e';txt[4]=' ';
        char ind=CharToStr(CurrentNode, txt+5, 2);
        
           List_Paint(txt,1,107,0,0,3," ",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      

           
           AddRfTask(SI, 1, 1);                //Dodaj rozkaz do tablicy zadañ RF   
           AddRfTask(SI, 1, 2);                //Dodaj rozkaz do tablicy zadañ RF     
           AddRfTask(SI, 1, 3);                //Dodaj rozkaz do tablicy zadañ RF     
           AddRfTask(SI, 1, 4);                //Dodaj rozkaz do tablicy zadañ RF     
           AddRfTask(SI, 1, 5);                //Dodaj rozkaz do tablicy zadañ RF     
           AddRfTask(SI, 1, 6);                //Dodaj rozkaz do tablicy zadañ RF     
           AddRfTask(SI, 1, 7);                //Dodaj rozkaz do tablicy zadañ RF     
           AddRfTask(SI, 1, 8);                //Dodaj rozkaz do tablicy zadañ RF   
           AddRfTask(SI, 1, 9);                //Dodaj rozkaz do tablicy zadañ RF     
           
           Label(Speech[g_leng].Analogi,0,4,100,14,40,1,127,0); 
           
           
           V_line(1,2,125,1); // (char X, int Y, char lenght, char typ)
      }//_______________________________________________________________________ 




      //*-----------------------------------------------------------------------
      void NodeWeAnalog_Event(char CurrentNode)
      {
        int ans;
        char NP;
       
        ans=List_Event(X_up, Y_up, X_down, Y_down, 0 , 15, 0,0); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        
        if (ans)
          {
          if (ans<0) {
            MenuLevel=(MenuLevel/100);
           NeuronNrChange=0;
           NeuronIdChange=0;
           FirstNodeStanIO_Paint=0;
           SetTask(&gProg);   //usuniêcie nie potrzebnie dodanych zadañ
          }
          MenuPaint();
          } 
        
        
        
        
      }//_______________________________________________________________________



      
      
      


      //*-----------------------------------------------------------------------
      void NodeWeAnalog_Refresh(char CurrentNode)
      {  
           char Txt[6];
        
           static char Dziel5;
           Dziel5=6;
           if (Dziel5<5) {Dziel5++;}else
           {
             Dziel5=0;

             char PozY=101;
             for (char i=1; i<10; i++)
             {
               CharToStr(i, Txt, 2);
               Label(Txt,0,2,PozY-(11*i)+1,1,10,1,127,1);  

               if (MainStage.StageI[CurrentNode].Value[i] !=(signed short int )0x8AAA) //je¿eli dane wiarygodne
               {
                 char IlLiter=sIntToStr( MainStage.StageI[CurrentNode].Value[i], &(Txt[0])); //(char Value, char *Vstring, char ill);
                 Label(Txt,0,15,PozY-(11*i)+1,IlLiter,40,1,127,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
               }//je¿eli dane niewiarygodne - poinformuj o oczekiwaniu
               else
               {
                 Label(Speech[g_leng].Czekaj,0,15,PozY-(11*i)+1,10,40,1,127,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
               }
               H_line(0,PozY-(11*i),63,1); // (char X, int Y, char lenght, char typ)
             }
   

           }
      }//_______________________________________________________________________ 
                                                                   

     

      
#define MaxWaitForResp  500    //[ms] czas oczekiwania na odpowiedz w teœcie
#define FirstTimeDelay  5000   //czas startowy
#define ShortTest  100   //iloœæ odpytañ w teœcie szybkim
#define LongTest  2000   //iloœæ odpytañ w teœcie d³ugim      
      
unsigned short MaxTime;
unsigned long LastSendTime;
unsigned long TestStartTime;
unsigned short TestStep;
//unsigned char TestMark; //0-testowanie, 1-test przeszed³, 2-test oblany
      //*-----------------------------------------------------------------------
      void Tester_MG1_Paint(char CurrentNode)
      {  
            char txt[8];
        txt[0]='N'; txt[1]='o';txt[2]='d';txt[3]='e';txt[4]=' ';
        char ind=CharToStr(CurrentNode, txt+5, 2);
        
           List_Paint(txt,1,107,0,0,3," ",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      


                                  //wyczyœæ zadania
           for (char i=0; i<RF_Tasks-1; i++) 
           {
            RF_TaskTab[i].Node=0;
            RF_TaskTab[i].Command.ID=0;
            RF_TaskTab[i].Command.Order=0;
            RF_TaskTab[i].Command.Auxi=0;
           }
           
           AddRfTask(SI, 1, 0);                //Dodaj rozkaz do tablicy zadañ RF  
           AddRfTask(SQ, 1, 0);                //Dodaj rozkaz do tablicy zadañ RF 
           PortStageSet((&(MainStage.StageI[CurrentNode].Port)),0,0);
           PortStageSet((&(MainStage.StageI[CurrentNode].Port)),1,0);
           MainStage.SendStageChange[1]=ChangeStageFlag;
           Label("MG1 TEST",0,4,100,14,40,1,127,0); 
           MaxTime=0;
           TestStartTime=g_counter;
           TestStep=0;
      }//_______________________________________________________________________ 



  unsigned short  TestAmount=ShortTest;
      //*-----------------------------------------------------------------------
      void Tester_MG1_Event(char CurrentNode)
      {
        int ans;
        char NP;
       
        ans=List_Event(X_up, Y_up, X_down, Y_down, 0 , 15, 0,0); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        char repaint;
        if (ans)
          {
          if (ans<0) {
           MenuLevel=(MenuLevel/10);
           TestStep=0;
           SetTask(&gProg);   //usuniêcie nie potrzebnie dodanych zadañ
           repaint=1;
          }
          }
          
            if (Button("Nowy test",3,18,58,13)==1)
            {
              TestAmount=ShortTest;
               CleanCanvasArea(2,1,60,61,0); 
              TestStep=0;
            }
            if (Button("Dlugi test",3,4,58,13)==1)
            {
              TestAmount=LongTest;
               CleanCanvasArea(2,1,60,61,0); 
              TestStep=0;      
            }
          

        
        if (repaint) MenuPaint();
        
      }//_______________________________________________________________________



      
      
      


      //*-----------------------------------------------------------------------


      void Tester_MG1_Refresh(char CurrentNode)
      {  
          if ((TestStartTime+1000)>g_counter) return;
        char txt[5];
              if (TestStep<TestAmount) 
              switch (TestStep%4)
              {
                case 0: 
                    if ((PortStageRead(MainStage.StageI[CurrentNode].Port, 0)==0) && ((PortStageRead(MainStage.StageI[CurrentNode].Port, 1)==0)))
                    {
                       if ((TestStartTime+10000)<g_counter)
                       {
                         if ((g_counter-LastSendTime)>MaxTime)  MaxTime=g_counter-LastSendTime;
                       }
                       
                       PortStageSet((&(MainStage.StageQ[CurrentNode].Port)),0,1);
                       PortStageSet((&(MainStage.StageQ[CurrentNode].Port)),1,0);
                       MainStage.SendStageChange[1]=ChangeStageFlag;
                       LastSendTime=g_counter;
                       TestStep++;
                    } else if ((TestStartTime+(TestStep*MaxWaitForResp+FirstTimeDelay))<g_counter)
                    {
                       TestStep=0xFFFF; 
                    }
                break;
                case 1: 
                    if ((PortStageRead(MainStage.StageI[CurrentNode].Port, 0)==1) && ((PortStageRead(MainStage.StageI[CurrentNode].Port, 1)==0)))
                    {
                      if ((TestStartTime+10000)<g_counter)
                       {
                         if ((g_counter-LastSendTime)>MaxTime)  MaxTime=g_counter-LastSendTime;
                       }
                       PortStageSet((&(MainStage.StageQ[CurrentNode].Port)),0,0);
                       PortStageSet((&(MainStage.StageQ[CurrentNode].Port)),1,1);
                       MainStage.SendStageChange[1]=ChangeStageFlag;
                       LastSendTime=g_counter;
                       TestStep++;
                    } else if ((TestStartTime+(TestStep*MaxWaitForResp+FirstTimeDelay))<g_counter)
                    {
                       TestStep=0xFFFF; 
                    }
                break;
                case 2: 
                    if ((PortStageRead(MainStage.StageI[CurrentNode].Port, 0)==0) && ((PortStageRead(MainStage.StageI[CurrentNode].Port, 1)==1)))
                    {
                      if ((TestStartTime+10000)<g_counter)
                       {
                         if ((g_counter-LastSendTime)>MaxTime)  MaxTime=g_counter-LastSendTime;
                       }
                       PortStageSet((&(MainStage.StageQ[CurrentNode].Port)),0,1);
                       PortStageSet((&(MainStage.StageQ[CurrentNode].Port)),1,1);
                       MainStage.SendStageChange[1]=ChangeStageFlag;
                       LastSendTime=g_counter;
                       TestStep++;
                    } else if ((TestStartTime+(TestStep*MaxWaitForResp+FirstTimeDelay))<g_counter)
                    {
                       TestStep=0xFFFF; 
                    }
                break; 
                case 3: 
                    if ((PortStageRead(MainStage.StageI[CurrentNode].Port, 0)==1) && ((PortStageRead(MainStage.StageI[CurrentNode].Port, 1)==1)))
                    {
                      if ((TestStartTime+10000)<g_counter)
                       {
                         if ((g_counter-LastSendTime)>MaxTime)  MaxTime=g_counter-LastSendTime;
                       }
                       PortStageSet((&(MainStage.StageQ[CurrentNode].Port)),0,0);
                       PortStageSet((&(MainStage.StageQ[CurrentNode].Port)),1,0);
                       MainStage.SendStageChange[1]=ChangeStageFlag;
                       LastSendTime=g_counter;
                       TestStep++;
                    } else if ((TestStartTime+(TestStep*MaxWaitForResp+FirstTimeDelay))<g_counter)
                    {
                       TestStep=0xFFFF; 
                    }
                break;           
                          
              }     
              
              if ((TestStep!=0xFFFF) && (TestAmount>=TestStep))   
                {
                  char LIL=sprintf(txt, "%d",  (TestAmount-TestStep));
                  Label(txt,2,4,92,LIL,57,1,127,0); 
                }

           
             
              
              
            //*******************************************
                 char PozY=80;
                 char PozX=3;
                 H_line(PozX,PozY,26,1); 
                 H_line(PozX,PozY+10,26,1);
                 V_line(PozX,PozY,10,1);
                 V_line(PozX+26,PozY,10,1); 
                 Label("Q1",0,PozX+1,PozY+1,2,15,1,127,1); 
                 
                 if ((MainStage.StageQ[CurrentNode].Port>>0)&0x1)
                 {
                   CleanCanvasArea(PozX+18,PozY+2,7,7,1); 
                 }else{
                   CleanCanvasArea(PozX+18,PozY+2,7,7,0);
                 }
                 
                 
                 PozY=80;
                 PozX=33;
                 H_line(PozX,PozY,26,1); 
                 H_line(PozX,PozY+10,26,1);
                 V_line(PozX,PozY,10,1);
                 V_line(PozX+26,PozY,10,1); 
                 Label("In1",0,PozX+1,PozY+1,3,15,1,127,1); 
                 
                 if ((MainStage.StageI[CurrentNode].Port>>0)&0x1)
                 {
                   CleanCanvasArea(PozX+18,PozY+2,7,7,1); 
                 }else{
                   CleanCanvasArea(PozX+18,PozY+2,7,7,0);
                 }
       
                 PozY=66;
                 PozX=3;
                 H_line(PozX,PozY,26,1); 
                 H_line(PozX,PozY+10,26,1);
                 V_line(PozX,PozY,10,1);
                 V_line(PozX+26,PozY,10,1); 
                 Label("Q2",0,PozX+1,PozY+1,3,15,1,127,1); 
                 
                 if ((MainStage.StageQ[CurrentNode].Port>>1)&0x1)
                 {
                   CleanCanvasArea(PozX+18,PozY+2,7,7,1); 
                 }else{
                   CleanCanvasArea(PozX+18,PozY+2,7,7,0);
                 }
                 
                 PozY=66;
                 PozX=33;
                 H_line(PozX,PozY,26,1); 
                 H_line(PozX,PozY+10,26,1);
                 V_line(PozX,PozY,10,1);
                 V_line(PozX+26,PozY,10,1); 
                 Label("In2",0,PozX+1,PozY+1,3,15,1,127,1); 
                 
                 if ((MainStage.StageI[CurrentNode].Port>>1)&0x1)
                 {
                   CleanCanvasArea(PozX+18,PozY+2,7,7,1); 
                 }else{
                   CleanCanvasArea(PozX+18,PozY+2,7,7,0);
                 }

          
           
                 
                 
                                           
                if (TestStep==0xFFFF)   //je¿eli test niezdany
                {
                  Label("Blad testu",2,2,31,10,5,1,127,0); 
                  
                }else if ((TestStep==TestAmount)|| (TestStep==TestAmount+1)) {
                  TestStep++;
                  Label("Test OK",2,2,52,10,60,1,127,1); 
                  Label("Czas t:",0,2,42,10,57,1,127,0); 
                  char Li=sprintf(txt, "%d",  ((g_counter-TestStartTime)/1000), "%d s");
                  txt[Li++]='s';
                  Label(txt,0,32,42,Li,25,1,127,0); 
                  
                  Label("Czas p:",0,2,33,10,57,1,127,0); 
                  Li=sprintf(txt, "%d",  MaxTime, "%d s");
                  txt[Li++]='m';
                  txt[Li++]='s';
                  Label(txt,0,32,33,Li,25,1,127,0); 
                  
                  
                  Button("Nowy test",3,18,58,13);
                  Button("Dlugi test",3,4,58,13); 
                    
                }
       

           
      }//_______________________________________________________________________ 
                                                                         


      
      
      
      //*-----------------------------------------------------------------------
      void NodeInfo_Paint(char CurrentNode)
      {  
            char txt[8];
        txt[0]='N'; txt[1]='o';txt[2]='d';txt[3]='e';txt[4]=' ';
        char ind=CharToStr(CurrentNode, txt+5, 2);
        
           List_Paint(txt,1,108,0,0,3," ",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      

           RefreshNeuronsList(&gProg); 

           AddRfTask(SI, CurrentNode, 0xA);                //Dodaj rozkaz do tablicy zadañ RF odczytu informacji o module
      }//_______________________________________________________________________ 



      //*-----------------------------------------------------------------------
      void NodeInfo_Event(char CurrentNode)
      {
        int ans;
        char NP;
       
        ans=List_Event(X_up, Y_up, X_down, Y_down, 0 , 15, 0,0); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        
       
        
        if (ans)
          {
          if (ans<0) {
            MenuLevel=(MenuLevel/100);
           //NeuronNrChange=0;
           //NeuronIdChange=0;
           //FirstNodeStanIO_Paint=0;
           SetTask(&gProg);   //usuniêcie nie potrzebnie dodanych zadañ
          }
          MenuPaint();
          } 
        
        
        
        
      }//_______________________________________________________________________





      //*-----------------------------------------------------------------------
      void NodeInfo_Refresh(char CurrentNode)
      {  
           char Txt[6];
        
           static char Dziel10=0;
           
           if (Dziel10<10) {Dziel10++;}else
           {
             Dziel10=0;
             
            _NodeInfo  NodeInfo;
            NodeInfo.NodeInfoWord=MainStage.StageI[CurrentNode].Value[0x0A];
            

            Label("Name:",0,1,97,15,62,1,127,1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
            Label((char*)NodeModelName[NodeInfo.Model],0,10,86,5,40,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw)           


            sprintf(Txt, "%d.%d",  NodeInfo.SoftVersion, NodeInfo.SoftSubVersion);
            Label("Soft version:",0,1,75,15,62,1,127,1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
            Label(Txt,0,10,64,15,40,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw)    
               
               
           }
      }//_______________________________________________________________________ 
                                                                   
     
      
      
      
      
      
      
      //*-----------------------------------------------------------------------
      void Net_Paint(void)
      {
         if (NET.WiFi.On)
         {
            List_Paint(Speech[g_leng].Siec,6,15,0,0,3, Speech[g_leng].UstawWiFi,"Host" , Speech[g_leng].Okresowosc, Speech[g_leng].Status, "Server reset" ,Speech[g_leng].Wyl_WiFi, 0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])                                                                                                                                     
         }else{
            List_Paint(Speech[g_leng].Siec,1,60,0,0,3," ",0 , 0, 0, 0, 0, 0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])                                                                                                                                     
            Button(Speech[g_leng].Wl_WiFi,7,75,50,13); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
         }
      }//_______________________ Koniec funkcji MainSide_Paint __________________________


        
       
       
      //*-----------------------------------------------------------------------
      void Net_Event(void)
      {
        if (NET.WiFi.On)
         {
              int ans=List_Event(X_up, Y_up, X_down, Y_down, 6 , 15, 0,0); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
        
              if (ans)
                {
                if ((ans>0) && (ans<=4)) MenuLevel=(MenuLevel*10+ans);
                if (ans==5)   //reset - pobranie konfiguracji z hosta
                {
                  NET.Status.NextHttpOrder=HTTP_GET_Config;
                  NET.HTTP.LastForseOrderNr=0;
                }
                if (ans==6)   //wy³¹cz WiFi
                {
                    NET.WiFi.On=0; 
                   //zapis do flash
                    unsigned int Save = __get_interrupt_state();  
                    __disable_interrupt();
                    
                      //at91flashWrite((unsigned int)(&(AT91C_IFLASH_MEM->Config.WiFi.On)), 0,  (char*) (&(NET.WiFi.On)), 1);
                      FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.WiFi.On)), (char*) (&(NET.WiFi.On)), 1);
                    __set_interrupt_state(Save);
                    __enable_interrupt();
                    //koniec zapisu do flash
                  
                }
                if (ans<0) {
                  MenuLevel=(MenuLevel/10);
                  CurModifSheet=0; 
                }
                MenuPaint();
                } 
         }else{
             int ans=List_Event(X_up, Y_up, X_down, Y_down, 0 , 15, 0,0);
              if (ans<0) {
                    MenuLevel=(MenuLevel/10);
                    CurModifSheet=0; 
                  }
             
           
             if (Button(Speech[g_leng].Wl_WiFi,7,75,50,13)==1)
             {
                   NET.Status.FreezTime=0;
                   NET.Status.HostLink=DISCONNECTED;
                   NET.Status.AccesPointAssociate=DISCONNECTED;
                   NET.Status.Timeout=0;
                   NET.Status.HttpNextQueryTime=0;
                   NET.Status.FreezTime=0;
               
                   NET.WiFi.On=1; 
                   //zapis do flash
                    unsigned int Save = __get_interrupt_state();  
                    __disable_interrupt();
                      FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.WiFi.On)), (char*) (&(NET.WiFi.On)), 1);
                    __set_interrupt_state(Save);
                    __enable_interrupt();
                    //koniec zapisu do flash
                    
                    
               MenuPaint();
             }
          
         }
           
      }//_______________________________________________________________________





      //*------------------------      -----------------------------------------------
      void UstawSiec_Paint(void)
      {
          List_Paint(Speech[g_leng].Siec,2,15,0,0,3,Speech[g_leng].Punkt_dostepu,"TCP/IP" , 0, 0, 0, 0, 0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])                                                                                                                                     
      }//_______________________ Koniec funkcji MainSide_Paint __________________________



      //*-----------------------------------------------------------------------
      void UstawSiec_Event(void)
      {
        int ans=List_Event(X_up, Y_up, X_down, Y_down, 2 , 15, 0,0); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        if (ans)
          {
          if ((ans>0) && (ans<=2)) MenuLevel=(MenuLevel*10+ans);
          if (ans<0) {
            MenuLevel=(MenuLevel/10);
           CurModifSheet=0; 
          }
          MenuPaint();
          } 
      }//_______________________________________________________________________

                           
//#include "WizFi.h"                            
// extern _Net NET;                         

      //*------------------------      -----------------------------------------------
       
      void Punkt_dostepuWiFi_Paint(void)
      {
    
          List_Paint(Speech[g_leng].Punkt_dostepu,1,108,0,0,3," ",0 , 0, 0, 0, 0, 0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])                                                                                                                                     

          Label(Speech[g_leng].Nazwa,0,1,97,15,62,1,127,1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
          Label(NET.WiFi.SSID,0,2,86,15,59,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw)           
          Button(Speech[g_leng].Zmien,15,68,34,13); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
          
          Label(Speech[g_leng].Klucz_seciowy,0,1,55,15,62,1,127,1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
          Label(NET.WiFi.Key,0,2,44,15,59,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
          Button(Speech[g_leng].Zmien,15,25,34,13); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
          
          H_line(0,20,63,1); // (char X, int Y, char lenght, char typ)
          Button(Speech[g_leng].Polacz,4,3,56,16); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
      
      }//_______________________ Koniec funkcji MainSide_Paint __________________________
      



      //*-----------------------------------------------------------------------
      void Punkt_dostepuWiFi_Event(void)
      {
      
         if (List_Event(X_up, Y_up, X_down, Y_down, 0 , 15, 0,0)<0)
        {
          MenuLevel=(MenuLevel/10);
          
        } 
         else if ((Y_down<112) || (Y_up<112))
        {
            
            if (Button(Speech[g_leng].Zmien,15,68,34,13)==1)
            {
              TextToChange=NET.WiFi.SSID;
              TextToChangeLength=15;
              TextToChangeMaxPixSize=62;
              MenuLevel=(MenuLevel*10+1);
            }else if (Button(Speech[g_leng].Zmien,15,25,34,13)==1)
            {
              TextToChange=NET.WiFi.Key;
              TextToChangeLength=15;
              TextToChangeMaxPixSize=62;
              MenuLevel=(MenuLevel*10+1);
            }else if (Button(Speech[g_leng].Polacz,4,3,56,16)==1)
            {
               NET.Status.AccesPointAssociateTest=0;
               MenuLevel=(MenuLevel*10+2);
            }
            
            
            
          
        }
         MenuPaint();
          
       
      }//_______________________________________________________________________


      
 
       
      void WiFiConecting_Paint(void)
      {
        
          //SetWiFiSuspend();
          List_Paint(Speech[g_leng].Punkt_dostepu,1,108,0,0,3," ",0 , 0, 0, 0, 0, 0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])                                                                                                                                     
          Wait_Paint();
        
      }//_______________________ Koniec funkcji MainSide_Paint __________________________

      
      typedef enum {
      WizFi_AT_NC=0,
      WizFi_AT_CONFIG,            //konfiguracja
      WizFi_AT_CONFIG_ERROR,      //b³ad konfiguracji
      WizFi_AT_CONNECTING,           //³¹czenie z hotspotem
      WizFi_AT_CONNECT_ERROR,     //b³¹d po³¹czenia z hotspotem
      WizFi_AT_CONNECTED,         //po³¹czono z hotspotem
    }_WiFiStat;
      
      void WiFiConecting_Refresh(void)
      {
 
           static char licz;
           static char end=0;
           char cyfra=licz%10;
           
           if (licz==0)
           {
                  //zapis do flash
                  unsigned int Save = __get_interrupt_state();  
                  __disable_interrupt();
                    FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.WiFi.SSID)), (char*) (&(NET.WiFi.SSID)), sizeof(NET.WiFi.SSID));
                    FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.WiFi.Key)), (char*) (&(NET.WiFi.Key)), sizeof(NET.WiFi.Key));
                  __set_interrupt_state(Save);
                  __enable_interrupt();
                  //koniec zapisu do flash
             
             
             NET.Status.FreezTime=g_counter+FREEZ_TIME;
             NET.Status.HostLink=DISCONNECTED;
             NET.Status.AccesPointAssociate=DISCONNECTED;
             NET.Status.Timeout=0;
             NET.Status.HttpNextQueryTime=0;
             NET.Status.FreezTime=0;
           }
           else if (end==0)
           {  
               if ((NET.Status.AccesPointAssociate==DISCONNECTED) && (NET.Status.AccesPointAssociateTest!=0xFF))
               {
                 Label("Not connected",2,2,15,15,59,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw)           
                 NET.Status.AccesPointAssociateTest=0;
               }  
               else if ((NET.Status.AccesPointAssociate==CONNECTING) && (NET.Status.AccesPointAssociateTest!=0xFF))
               {
                 if (cyfra==0) Label("Connecting",0,4,15,15,59,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw)           
                 if (cyfra==3) Label("Connecting .",0,4,15,15,59,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw)           
                 if (cyfra==5) Label("Connecting ..",0,4,15,15,59,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw)           
                 if (cyfra==7) Label("Connecting ...",0,4,15,15,59,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw)           
               }else if ((NET.Status.AccesPointAssociate==CONNECTED) || (NET.Status.AccesPointAssociateTest==0xFF))
               {
                 end=1;
                 licz=180;
                 Label("Connected",2,2,15,15,59,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw)            
               
                 

               
               }
             
           }
           
           if (licz==179) 
           {
             end=1;
             licz=180;
             Label("Not connected",2,2,15,15,59,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw)           
           }
           
           
           if (licz<210)
           {
              licz++; 
              if (NET.Status.AccesPointAssociateTest<220) NET.Status.AccesPointAssociateTest++;
           }else{
              licz=0;
              end=0;
              MenuLevel=(MenuLevel/10);
              licz=0;
              MenuPaint();
           }
           
           
          
      
      }

    
      
      
      char WiFiReConnect=0;
      
      void  Host_WiFi_Paint(void)
      {
          
          List_Paint("Host",1,108,0,0,3," ",0 , 0, 0, 0, 0, 0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])                                                                                                                                     

          char pY=72;    //pozycja dolna
          Label(Speech[g_leng].Domena,0,1,pY+25,15,62,1,127,1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
          Label(".................................",0,2,pY+14,25,59,1,127,0);
          Label(NET.WiFi.Domena,0,2,pY+16,15,59,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw)           
          Button(Speech[g_leng].Zmien,15,pY+1,34,13); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
          
          pY=37;    
          Label(Speech[g_leng].Subdomena,0,1,pY+25,15,62,1,127,1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
          Label("..................................",0,2,pY+14,25,59,1,127,0);
          Label(NET.WiFi.Subdomena,0,2,pY+16,15,59,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw)           
          Button(Speech[g_leng].Zmien,15,pY+1,34,13); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
          
          pY=2;    
          Label(Speech[g_leng].Scezka,0,1,pY+25,15,62,1,127,1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
          Label("..................................",0,2,pY+14,25,59,1,127,0);
          Label(NET.WiFi.Path,0,2,pY+16,15,59,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw)           
          Button(Speech[g_leng].Zmien,15,pY+1,34,13); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
          
          Button("?",36,112,11,13);
          
          
          if (WiFiReConnect)
          {
            NET.Status.FreezTime=g_counter+FREEZ_TIME;
             NET.Status.HostLink=DISCONNECTED;
             NET.Status.AccesPointAssociate=DISCONNECTED;
             for (char i=0; i<15; i++)NET.Status.DomenaIP[i]=0;
             NET.Status.Timeout=0;
             NET.Status.HttpNextQueryTime=0;
             NET.Status.FreezTime=0;    
            
            WiFiReConnect=0;
          }
          
          
      }//_______________________ Koniec funkcji MainSide_Paint __________________________


   //*-----------------------------------------------------------------------
      void  Host_WiFi_Event(void)
      {
         if (List_Event(X_up, Y_up, X_down, Y_down, 0 , 15, 0,0)<0)
        {
          //zapis do flash
                  unsigned int Save = __get_interrupt_state();  
                  __disable_interrupt();
                    FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.WiFi.Domena)), (char*) (&(NET.WiFi.Domena)),15);
                    FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.WiFi.Subdomena)), (char*) (&(NET.WiFi.Subdomena)), 15);
                    FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.WiFi.Path)), (char*) (&(NET.WiFi.Path)), 15);
                  __set_interrupt_state(Save);
                  __enable_interrupt(); 
           //koniec zapisu do flash
          
             
                 
                  
          MenuLevel=(MenuLevel/10);
          
           
          
        } 
         else if ((Y_down<112) || (Y_up<112))
        {
            char pY=72; 
            if (Button(Speech[g_leng].Zmien,15,pY+1,34,13)==1)
            {
              TextToChange=NET.WiFi.Domena;
              TextToChangeLength=15;
              TextToChangeMaxPixSize=100;
              MenuLevel=(MenuLevel*10+1);
              WiFiReConnect=1;
            }
            pY=37; 
            if (Button(Speech[g_leng].Zmien,15,pY+1,34,13)==1)
            {
              TextToChange=NET.WiFi.Subdomena;
              TextToChangeLength=15;
              TextToChangeMaxPixSize=100;
              MenuLevel=(MenuLevel*10+1);
              WiFiReConnect=1;
            }
            pY=2; 
            if (Button(Speech[g_leng].Zmien,15,pY+1,34,13)==1)
            {
              TextToChange=NET.WiFi.Path;
              TextToChangeLength=15;
              TextToChangeMaxPixSize=100;
              MenuLevel=(MenuLevel*10+1);
              WiFiReConnect=1;
              
            }
            
            
            
            
            if (Button("?",36,112,11,13)==1) 
             {
               if (g_leng==0)
               {
                 MESSAGE(Komentarz1_PL); 
               }else{
                 MESSAGE(Komentarz1_En); 
               }
             }
            
          
        }
         MenuPaint();
          
      
      }//_______________________________________________________________________
    
     
      
      
 
                                         

      //*-----------------------------------------------------------------------
      void NetPeriodicity_Paint(void)
      {
  
            List_Paint(Speech[g_leng].Okresowosc,1,65,0,0,3," ",0 , 0, 0, 0, 0, 0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])                                                                                                                                     
            
            char txt[9];
            char liter=sprintf(txt, "%d", NET.WiFi.HttpQueryPeriod*100);
            txt[liter++]=' ';
            txt[liter++]='m';
            txt[liter++]='s';
            Label(txt,0,13,92,liter,40,1,127,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
                
            
            txt[1]=0;
            txt[0]=31;
            Button(txt,12,68,20,18); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
            txt[0]=30;
            Button(txt,32,68,20,18); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
            
            Button("?",50,90,11,13);
            
      }//_______________________ Koniec funkcji MainSide_Paint __________________________
 
      
      //*-----------------------------------------------------------------------
      void NetPeriodicity_Event(void)
      {
        
             int ans=List_Event(X_up, Y_up, X_down, Y_down, 0 , 15, 0,0);
              
            char bt=0; 
            
            char LabelButt[2];  
            LabelButt[1]=0;
            LabelButt[0]=31;
            if (Button(LabelButt,12,68,20,18)==1) bt=1; //(etykieta ,char poz_X , char poz_Y ,char width, char height)
            LabelButt[0]=30;
            if (Button(LabelButt,32,68,20,18)==1) bt=2; //(etykieta ,char poz_X , char poz_Y ,char width, char height)
              
              
            if ((bt==1) && (NET.WiFi.HttpQueryPeriod>3)) NET.WiFi.HttpQueryPeriod--;
            if ((bt==2) && (NET.WiFi.HttpQueryPeriod<51)) NET.WiFi.HttpQueryPeriod++;
            
             if (bt>0)
             {
                 
               MenuPaint();
             }
             
             if (Button("?",50,90,11,13)==1) 
             {
               if (g_leng==0)
               {
                 MESSAGE(Komentarz0_PL); 
               }else{
                 MESSAGE(Komentarz0_En); 
               }
             }
             
            if (ans<0) {
                    //zapis do flash
                    unsigned int Save = __get_interrupt_state();                            
                    __disable_interrupt();
                      FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.WiFi.HttpQueryPeriod)), (char*) (&(NET.WiFi.HttpQueryPeriod)), 1);
                    __set_interrupt_state(Save);
                    __enable_interrupt();
                    //koniec zapisu do flash
                    
                    MenuLevel=(MenuLevel/10);
                    CurModifSheet=0; 
                    MenuPaint();
                  }
           
      }//_______________________________________________________________________
      
      
      
    //*-----------------------------------------------------------------------
      void IpTCP_Paint(void)
      {
         if (NET.WiFi.IpNotAuto==1)
         {
            List_Paint(Speech[g_leng].Siec,2,15,0,0,3, "Auto IP -on", "IP config" , 0, 0, 0, 0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])                                                                                                                                     
         }else{
            List_Paint(Speech[g_leng].Siec,1,60,0,0,3," ",0 , 0, 0, 0, 0, 0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])                                                                                                                                     
            Button("Auto IP -off",2,75,60,13); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
         }
      }//_______________________ Koniec funkcji MainSide_Paint __________________________


        
       
       
      //*-----------------------------------------------------------------------
      void IpTCP_Event(void)
      {
        if (NET.WiFi.IpNotAuto==1)
         {
              int ans=List_Event(X_up, Y_up, X_down, Y_down, 2 , 15, 0,0); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
        
              if (ans)
                {
                if (ans==2) MenuLevel=(MenuLevel*10+ans);
                if (ans==1)   //w³¹cz auto
                {
                    NET.WiFi.IpNotAuto=0; 

                    
                   //zapis do flash
                    unsigned int Save = __get_interrupt_state();  
                    __disable_interrupt();
                      FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.WiFi.IpNotAuto)), (char*) (&(NET.WiFi.IpNotAuto)), 1);
                    __set_interrupt_state(Save);
                    __enable_interrupt();
                    //koniec zapisu do flash
                    
                     NET.Status.FreezTime=0;
                   NET.Status.HostLink=DISCONNECTED;
                   NET.Status.AccesPointAssociate=DISCONNECTED;
                   NET.Status.Timeout=0;
                   NET.Status.HttpNextQueryTime=0;
                   NET.Status.FreezTime=0;
                  
                }
                if (ans<0) {
                  MenuLevel=(MenuLevel/10);
                  CurModifSheet=0; 
                }
                MenuPaint();
                } 
         }else{
             int ans=List_Event(X_up, Y_up, X_down, Y_down, 0 , 15, 0,0);
              if (ans<0) {
                    MenuLevel=(MenuLevel/10);
                    CurModifSheet=0; 
                  }
             
           
             if (Button("Auto IP -off",2,75,60,13)==1)
             {
                   NET.Status.FreezTime=0;
                   NET.Status.HostLink=DISCONNECTED;
                   NET.Status.AccesPointAssociate=DISCONNECTED;
                   NET.Status.Timeout=0;
                   NET.Status.HttpNextQueryTime=0;
                   NET.Status.FreezTime=0;
               
                   NET.WiFi.IpNotAuto=1;    //wy³¹cz auto
                   //zapis do flash
                    unsigned int Save = __get_interrupt_state();  
                    __disable_interrupt();
                      FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.WiFi.IpNotAuto)), (char*) (&(NET.WiFi.IpNotAuto)), 1);
                    __set_interrupt_state(Save);
                    __enable_interrupt();
                    //koniec zapisu do flash
                    
                    
               MenuPaint();
             }
          
         }
           
      }//_______________________________________________________________________
      
      
  
      
//sprawdza poprawnoœæ zapisu IP
//funkcja czyœci wpis je¿eli nie poprawny ze standardem IP       
//0   -brak wpisu
//1   -wpis prawid³owy   
//-1   -wpis niepoprawny       
signed char CheckIP(char *Text)
{
  signed char IpOk=1;
  
  char znak=0;
  
  if (Text[0]==0) IpOk=0; 
  while ((znak<12) && (Text[znak]>0))  
  {
     if (((Text[znak]>='0')&&(Text[znak]<='9')) || (Text[znak]=='.'))  
     {
     }else{
       IpOk=-1;
     }
     znak++; 
  }
  znak=0;
  if (IpOk<0) while(znak<14) Text[znak++]=0;
  return IpOk;
}
      
      
      
      
      
void  IpConfig_Paint(void)
      {
          
           if (CheckIP(NET.WiFi.IP)<0)
          {
              if (g_leng==0) MESSAGE(Komentarz2_PL); else  MESSAGE(Komentarz2_En);
          }else if (CheckIP(NET.WiFi.Mask)<0)
          {
              if (g_leng==0) MESSAGE(Komentarz3_PL); else  MESSAGE(Komentarz3_En);
          }else if (CheckIP(NET.WiFi.Gate)<0)
          {
              if (g_leng==0) MESSAGE(Komentarz4_PL); else  MESSAGE(Komentarz4_En);
          }else{           


        
          List_Paint("IP/TCP",1,108,0,0,3," ",0 , 0, 0, 0, 0, 0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])                                                                                                                                     

          char pY=72;    //pozycja dolna
          Label(Speech[g_leng].Adres_IP,0,1,pY+25,15,62,1,127,1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
          Label(".................................",0,2,pY+14,25,59,1,127,0);
          
          Label(NET.WiFi.IP,0,2,pY+16,15,59,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw)           
          Button(Speech[g_leng].Zmien,15,pY+1,34,13); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
          
          pY=37;    
          Label(Speech[g_leng].Maska,0,1,pY+25,15,62,1,127,1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
          Label("..................................",0,2,pY+14,25,59,1,127,0);
          Label(NET.WiFi.Mask,0,2,pY+16,15,59,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw)           
          Button(Speech[g_leng].Zmien,15,pY+1,34,13); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
          
          pY=2;    
          Label(Speech[g_leng].Brama,0,1,pY+25,15,62,1,127,1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
          Label("..................................",0,2,pY+14,25,59,1,127,0);
          Label(NET.WiFi.Gate,0,2,pY+16,15,59,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw)           
          Button(Speech[g_leng].Zmien,15,pY+1,34,13); //(etykieta ,char poz_X , char poz_Y ,char width, char height)

          
          }
          
      }//_______________________ Koniec funkcji MainSide_Paint __________________________


   //*-----------------------------------------------------------------------
      void  IpConfig_Event(void)
      {
         if (List_Event(X_up, Y_up, X_down, Y_down, 0 , 15, 0,0)<0)
        {
          //zapis do flash
                  unsigned int Save = __get_interrupt_state();  
                  __disable_interrupt();
                    FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.WiFi.IP)), (char*) (&(NET.WiFi.IP)),15);
                    FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.WiFi.Mask)), (char*) (&(NET.WiFi.Mask)), 15);
                    FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.WiFi.Gate)), (char*) (&(NET.WiFi.Gate)), 15);
                  __set_interrupt_state(Save);
                  __enable_interrupt(); 
           //koniec zapisu do flash
          
           NET.Status.FreezTime=g_counter+FREEZ_TIME;
             NET.Status.HostLink=DISCONNECTED;
             NET.Status.AccesPointAssociate=DISCONNECTED;
             NET.Status.Timeout=0;
             NET.Status.HttpNextQueryTime=0;
             NET.Status.FreezTime=0;        
                  
          MenuLevel=(MenuLevel/10);
          
           
          
        } 
         else if ((Y_down<112) || (Y_up<112))
        {
            char pY=72; 
            if (Button(Speech[g_leng].Zmien,15,pY+1,34,13)==1)
            {
              TextToChange=NET.WiFi.IP;
              TextToChangeLength=15;
              TextToChangeMaxPixSize=100;
              AlfaType=2;
              char znak=0;
              while(znak<14) NET.WiFi.IP[znak++]=0;
              MenuLevel=(MenuLevel*10+1);
              
             
            }
            pY=37; 
            if (Button(Speech[g_leng].Zmien,15,pY+1,34,13)==1)
            {
              TextToChange=NET.WiFi.Mask;
              TextToChangeLength=15;        
              TextToChangeMaxPixSize=100;
              AlfaType=2;
              char znak=0;
              while(znak<14) NET.WiFi.Mask[znak++]=0;
              MenuLevel=(MenuLevel*10+1);
              
            }
            pY=2; 
            if (Button(Speech[g_leng].Zmien,15,pY+1,34,13)==1)
            {
              TextToChange=NET.WiFi.Gate;
              TextToChangeLength=15;
              TextToChangeMaxPixSize=100;
              AlfaType=2;
              char znak=0;
              while(znak<14) NET.WiFi.Gate[znak++]=0;
              MenuLevel=(MenuLevel*10+1);
              
            }
            
            
            

          
        }
         MenuPaint();
          
      
      }//_______________________________________________________________________
    
     

      
      char TransferInfo=0;
      void  NetInfo_Paint(void)
      {
          char txt[14];
          
          List_Paint(Speech[g_leng].Status,1,108,0,0,3," ",0 , 0, 0, 0, 0, 0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])                                                                                                                                     

          char pY=100;    //pozycja dolna
          Label(Speech[g_leng].Punkt_dostepu,0,1,pY,15,62,1,127,1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 

          Label(Speech[g_leng].Serwer,0,1,pY-20,15,62,1,127,1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 

          Label(Speech[g_leng].Transfer,0,1,pY-45,15,62,1,127,1);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 

          
          CopyToCanvasPoint((char*)Siec, 4, pY-72, 56,24);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);           
      
          
          float fHQP;
          float TRANS;
          char dl;
          fHQP= NET.WiFi.HttpQueryPeriod;
          fHQP/=10; 
          TRANS= NET.Status.SendBytes+NET.Status.ResBytes;
          TRANS/=fHQP;     //transfer na sekundê w B
          TRANS*=2628; //transfer na rok w kB
          TRANS/=1000000; //transfer na rok w GB
          
          dl=FloToStr(TRANS*1.2, txt, 5);
          txt[dl++]='G';txt[dl++]='B';txt[dl++]='/';txt[dl++]=Speech[g_leng].Miesiac[0];txt[dl++]=Speech[g_leng].Miesiac[1];txt[dl++]=Speech[g_leng].Miesiac[2];txt[dl++]=Speech[g_leng].Miesiac[3]; txt[dl++]='.';   txt[dl]=0;
          Label(txt,0,4, pY-88,dl,15,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
       
          

          
      }//_______________________ Koniec funkcji  __________________________
    
      
   //*-----------------------------------------------------------------------
      void  NetInfo_Event(void)
      {
         if (List_Event(X_up, Y_up, X_down, Y_down, 0 , 15, 0,0)<0)
        {     
          TransferInfo=0;         
          MenuLevel=(MenuLevel/10);
          MenuPaint();
        }else{
            if ((X_down>1) && (X_down<63) && (Y_down>10) && (Y_down<60))
            {
               TransferInfo=1;
            }
            
        }
    
         
          
      
      }//_______________________________________________________________________

      
      void  NetInfo_Refresh(void)
      {
        //static char Dziel;
          char txt[6];
        //if (Dziel<5)
        //{
          static char  AccesPointAssociateTestOk=0;  //0-brak informacji, 1-Nipod³¹czony, 2 pod³¹czony
          char pY=100;    //pozycja dolna
          
          if (NET.Status.AccesPointAssociateTest<200) NET.Status.AccesPointAssociateTest++;
          if (NET.Status.AccesPointAssociateTest==200)
          {
             AccesPointAssociateTestOk=1;
             NET.Status.AccesPointAssociateTest=0;  
          }
          if (NET.Status.AccesPointAssociateTest==0xFF)
          {
             AccesPointAssociateTestOk=2;
             NET.Status.AccesPointAssociateTest=0;
          }
         
          if (AccesPointAssociateTestOk==2)
          {
            Label(Speech[g_leng].Polaczony,0,6,pY-10,15,53,1,127,0); 
          }
          else if  (AccesPointAssociateTestOk==1)
          {
            Label(Speech[g_leng].Niepolaczony,0,6,pY-10,15,53,1,127,0); 
          }else if  (AccesPointAssociateTestOk==0)
          {
            Label("???",0,6,pY-10,15,53,1,127,0); 
          }      
          

          
          if (NET.Status.HostLink==CONNECTED)
          {   
            Label(Speech[g_leng].Polaczony,0,6,pY-30,15,53,1,127,0); 
          }else{
            Label(Speech[g_leng].Niepolaczony,0,6,pY-30,15,53,1,127,0); 
          }
          
          CleanCanvasArea(20, pY-74,15,27,0);//(char X, char Y, char X_Lenght, char Y_Lenght, char Colour)
          
          
                  
          if ((NET.Status.SendPacketTime+250>g_counter) || ((NET.Status.ResPacketTime<NET.Status.SendPacketTime) && (NET.Status.SendPacketTime+12000>g_counter))) 
          {
            CopyToCanvasPoint((char*)StrzalkaPrawo,21, pY-60, 11,5);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);           
            if (TransferInfo)
            {
              sprintf(txt, "%d",  NET.Status.SendBytes, 'B');
              Label(txt,0,20, pY-56,15,15,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
            }
          }
          if (NET.Status.ResPacketTime+250>g_counter) 
          {
            CopyToCanvasPoint((char*)StrzalkaLewo, 21, pY-65, 11,5);  //void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);           
            if (TransferInfo)
            {
            sprintf(txt, "%d",  NET.Status.ResBytes);
            Label(txt,0,20, pY-74,15,15,1,127,0);  //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw) 
            }
          }
          
          
          
      }//_______________________ Koniec funkcji MainSide_Paint __________________________

      
      
      
      
      
    
      //*-----------------------------------------------------------------------
      void Zmienne_Paint(void)
      {

         List_Paint(Speech[g_leng].Zmienne,2,15,0,0,3, Speech[g_leng].ZmDwust, Speech[g_leng].ZmAnalog, 0,  0,0,0,0,0); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])
                                                                                                                               
      }//_______________________ Koniec funkcji MainSide_Paint __________________________



      //*-----------------------------------------------------------------------
      void Zmienne_Event(void)
      {
 
        char ans=List_Event(X_up, Y_up, X_down, Y_down, 2 , 15, 0,0); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        if (ans)
          {
          if ((ans>0) && (ans<=2)) MenuLevel=(MenuLevel*10+ans);
          if (ans<0) {
            MenuLevel=(MenuLevel/10);
           CurModifSheet=0; 
          }
          MenuPaint();
          } 
      }//_______________________________________________________________________

      

      
      char ZM_OFFSET=0;
      //*-----------------------------------------------------------------------
      void ZmienneDwustanowe_Paint(void)
      {  

            List_Paint(Speech[g_leng].ZmDwust,1,108,0,0,3," ",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      
            
            
           char LabelButt[2];
           LabelButt[1]=0;
           LabelButt[0]=30;
           Button(LabelButt,34,3,19,19); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
           LabelButt[0]=31;
           Button(LabelButt,13,3,19,19); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
            V_line(1,2,125,1); // (char X, int Y, char lenght, char typ)
            
      }//_______________________________________________________________________ 




      //*------------------------------------------- ----------------------------
      void ZmienneDwustanowe_Event(void)
      {
        int ans;

       
        ans=List_Event(X_up, Y_up, X_down, Y_down, 0 , 15, 0,0); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        char Repaint=0;

        
        
        if (ans)
          {
          if (ans<0) {
            
            
            ZM_OFFSET=0;
            MenuLevel=(MenuLevel/100);

             Repaint=1;

          }
          
          }else{
              
              char LabelButt[2];
              LabelButt[1]=0;
              LabelButt[0]=30;
              if (Button(LabelButt,34,3,19,19)==2)
              {
                 if (ZM_OFFSET>0) ZM_OFFSET--;
              }
              LabelButt[0]=31;
              if (Button(LabelButt,13,3,19,19)==2) 
              {  
                if (ZM_OFFSET<99-8) ZM_OFFSET++;
              }
            
          } 
        
          if (Repaint) MenuPaint();
        
        
      }//_______________________________________________________________________
      
      
      
      
      //*------------------------------------------- ----------------------------
      void ZmienneDwustanowe_Repaint(void)
      {


             char PozY=99;
             for (char i=0; i<8; i++)
             {
                char Txt[6]="b";
                
               CharToStr(i+ZM_OFFSET, &Txt[1], 2);
               Label(Txt,0,2,PozY-(11*i)+1,3,15,1,127,1);  

               //char IlLiter=sIntToStr( MainStage.StageI[CurrentNode].Value[i], &(Txt[0])); //(char Value, char *Vstring, char ill);
               
                
               char IlLiter=CharToStr((Variables[((i+ZM_OFFSET)/8)]>>((i+ZM_OFFSET)%8)) &0x1, Txt, 1);
               Label(Txt,0,25,PozY-(11*i)+1,IlLiter,35,1,127,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
               
               H_line(0,PozY-(11*i),63,1); // (char X, int Y, char lenght, char typ)
             }
   

     
        
      }//_______________________________________________________________________


 
   
//*-----------------------------------------------------------------------
      void ZmienneAnalogowe_Paint(void)
      {  

            List_Paint(Speech[g_leng].ZmAnalog,1,108,0,0,3," ",0,0,0,0,0,0,0 ); //(char name[7], char Rows, char RowWith, char Y, char FormStyle, char algin, char p1[12], char p2[12] , char p3[12] , char p4[12] , char p5[12] , char p6[12] , char p7[12] , char p8[12])      
            
            
           char LabelButt[2];
           LabelButt[1]=0;
           LabelButt[0]=30;
           Button(LabelButt,34,3,19,19); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
           LabelButt[0]=31;
           Button(LabelButt,13,3,19,19); //(etykieta ,char poz_X , char poz_Y ,char width, char height)
            V_line(1,2,125,1); // (char X, int Y, char lenght, char typ)
            
      }//_______________________________________________________________________ 




      //*------------------------------------------- ----------------------------
      void ZmienneAnalogowe_Event(void)
      {
        int ans;

       
        ans=List_Event(X_up, Y_up, X_down, Y_down, 0 , 15, 0,0); //(char X_up, char Y_up, char X_down, char Y_down, char Rows, char RowWith, char FormStyle, char Y)
  
        char Repaint=0;

        
        
        if (ans)
          {
          if (ans<0) {
            
            
            ZM_OFFSET=0;
            MenuLevel=(MenuLevel/100);

             Repaint=1;

          }
          
          }else{
              
              char LabelButt[2];
              LabelButt[1]=0;
              LabelButt[0]=30;
              if (Button(LabelButt,34,3,19,19)==2)
              {
                 if (ZM_OFFSET>0) ZM_OFFSET--;
              }
              LabelButt[0]=31;
              if (Button(LabelButt,13,3,19,19)==2) 
              {  
                if (ZM_OFFSET<20-8) ZM_OFFSET++;
              }
            
          } 
        
          if (Repaint) MenuPaint();
        
        
      }//_______________________________________________________________________
      
      
      
      
      //*------------------------------------------- ----------------------------
      void ZmienneAnalogowe_Repaint(void)
      {

             char IlLiter;
             char PozY=99;
             for (char i=0; i<8; i++)
             {
                char Txt[6]="a";
                
               CharToStr(i+ZM_OFFSET, &Txt[1], 2);
               Label(Txt,0,2,PozY-(11*i)+1,3,15,1,127,1);  

               IlLiter=sIntToStr( VariablesAnal[i+ZM_OFFSET], &(Txt[0])); //(char Value, char *Vstring, char ill);

               Label(Txt,0,25,PozY-(11*i)+1,IlLiter,35,1,127,0);   //(char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, negatyw) 
               
               H_line(0,PozY-(11*i),63,1); // (char X, int Y, char lenght, char typ)
             }
   

     
        
      }//_______________________________________________________________________
      
      
      
         