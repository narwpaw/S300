//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                 IASE Microcontroller Software Support                    *//                       
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : Framework.h                              *//
//*  Opis                         : Biblioteka systemu operacyjneg           *//
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

#define   LIVE_TAB_SIZE 260


#ifndef __Framework__
#define __Framework__

#include "common.h"
#include "core/device.h"                                                        //do³¹cza bibliotreki do odpowiedniego procesora



//#define DIRECT_Q_MESURE
//#define _LIVE_RJESTR

//Definicje --------------------------------------------------------------------
            #define SourProgFlashFerstSect   100                                //Pocz¹tkowy sektor przestrzeni pamiêci flash przeznaczonej na program u¿ytkowy
            #define SourProgSpace            0x800                              //Max wielkoœæ prognramu u¿ytkowego (32 kB)
            //#define FirstByteProgram         0x5D                                 //Pirewszy bajt w³aœciwego programu u¿ytkowego w obszarze pamiêci programu u¿ytkowego (zawsze = 0xEF, 2 - 00)
            
    #define ScreenNr          8     //iloœæ mo¿liwych kombinacji ekranu (iloœæ stron) 
            //Organizacja u¿ytkowej pamiêci flash                               //Obszar 0x0-0x20000 (128 kB) zarezetrwowany dla pamiêci framework
              //#define FlashProgram_A           0x20000                          //Pocz¹tek pamiêci programu A (max wielkoœæ programu - 64 kB);
              //#define FlashProgram_B           0x30000                          //Pocz¹tek pamiêci programu B (max wielkoœæ programu - 64 kB);
              #define FlashPagesOffset         0x100000
    #define ScreenSetTab_0_Nr 8
    #define ScreenSetTab_1_Nr 5
    #define TimetableTasks 4

    #define SPK_Amount 4

#define AlarmTimeToActive  60         //czas do aktywacji alarmu/syreny w [s]
#define AlarmSirenTime     300        //d³ugoœc alarmu dzwiêkowego w [s]

//___________________________Koniec definicji___________________________________





//definicje typów --------------------------------------------------------------
         //   typedef unsigned char   bool; 		
            typedef unsigned char   byte;
            typedef short int       sint;
            typedef unsigned char   BYTE;
            typedef unsigned short  WORD;
         //   #define true 		      1
         //   #define false 		      0
//_______________________Koniec definicji typów________________________________




// *****************************************************************************
//              Struktura RAM (framework pamiêc RAM)
// *****************************************************************************

  
#define  CommVarRAM_Size          200


            
            //! Info: komunikacja wr. redundancji  10000 B
            

    //   #pragma pack(1) 

       typedef struct RAM_STRUCT 
        {
          union{
                WORD           VarCom_Word[CommVarRAM_Size];                              //bufor komunikacyjny
                BYTE           VarCom_Byte[2*CommVarRAM_Size];
                
                };


          
        }RAM;
       
       
       
       typedef struct __Order
       {
           char Order;  //rozkaz 0-nic, 1-w³, 2-wy³
           char Q;      //nr zewnêtrznego urz¹dzenia 
           char Port;   //nr kana³u w urz¹dzeniu
       }_Order;
       
       

      
    
       
         
           #define SpeakerPin                (1<<31)
           #define TxPin                     AT91C_PIO_PA24
         
    //      #pragma pack()  

// *****************************************************************************
//       Funkcje komunikacji z RS-Win
// *****************************************************************************
          
         void komunikcja_RSwin(char *ptr1, char *ptr2, unsigned int *ptr3);
         char CheckSum(char *TabLiczb, unsigned int *IlLiczb);                  //funkcja oblicza sumê kontroln¹
         void MasterToAnaVar(unsigned long *adr, unsigned long *vart);                    //funkcja konwersji zmiennej analogowej z formatu MASTER na format Float i zapis do ram
         void MasterToBinVar(unsigned long *adr, char *vart);                         //funkcja konwersji zmiennej dwustanowej z formatu MASTER na format Float i zapis do ram
         unsigned long AnaVarToMaster(unsigned long *adr);                      //funkcja konwersji zmiennej analogowej z formatu Float na format MASTER 
         char BinVarToMaster(unsigned long *adr);                               //funkcja konwersji zmiennej dwustanowej z formatu Float na format MASTER
         typedef union ConversFC_Union{
                        unsigned long DWvar;
                        byte          Bvar[4];
                      }ConversFC;
         
//------------------Koniec funkcji komunikacji z RS-Win`em----------------------        



         
// *****************************************************************************
//       Funkcje obs³ugi programu u¿ytkowego
// *****************************************************************************
          
          void SetModbusTask (void); 
          float GetAnaValue (int byte_nr_tmp);                                  //Funkcja interpretuj¹ca zapis wartoœci/zmiennej w programie u¿ytkowym
          char GetBinValue (sint byte_nr_tmp);                                  //Funkcja interpretuj¹ca zapis wartoœci/zmiennej dwustanowej w programie u¿ytkowym
          void ModVarAna (sint zmienna, float wartosc);                         //Funkcja zmiany wartoœci zmiennej analogowej. Wartoœæ zostanie zmieniona je¿eli zmienna nie jest zaforsowana
          void ModVarBin (sint zmienna, byte wartosc);                          //Funkcja zmiany wartoœci zmiennej binarnej. Wartoœæ zostanie zmieniona je¿eli zmienna nie jest zaforsowana
          void GetBinVarPack(WORD adr, WORD Length, unsigned char *Tab );       //Funkcja zwraca paczkê wartoœci zmienncyh dwustanowych z zakresu B000 - B800
          void GetBinVarPackComm(WORD adr, WORD Length,unsigned char *Tab );    //* Funkcja przepisuje paczkê wartoœci zmiennych dwustanowej komunikacyjnej DA00 - DDFF do Tab oraz zwraca flage zmiany wartoœci
          WORD GetBinVarWordPack(int adr);                                      //Funkcja zwraca Word 16 kolejnych wartoœci zmienncyh dwustanowych z zakresu B000 - B700
          WORD GetBinCommWordPack(int adr);                                      //Funkcja zwraca Word 16 kolejnych wartoœci zmienncyh komunikacyjnych dwustanowych z zakresu DA00 - DDFF
          typedef union ConversProgFC_Union{
                        unsigned long DWvar;
                        float          Fvar;
                      }Convers_DW_F;
          
          void ZD_MODBUS_set(void); 
          
          sint ABS    (sint byte_nr, sint *PrzesunWewRam); 
          sint MNS    (sint byte_nr, sint *PrzesunWewRam); 
          sint SUM    (sint byte_nr, sint *PrzesunWewRam);
          sint ODM    (sint byte_nr, sint *PrzesunWewRam); 
          sint MUL    (sint byte_nr, sint *PrzesunWewRam); 
          sint DIV    (sint byte_nr, sint *PrzesunWewRam); 
          sint INW    (sint byte_nr, sint *PrzesunWewRam); 
          sint PKW    (sint byte_nr, sint *PrzesunWewRam);
          sint POT    (sint byte_nr, sint *PrzesunWewRam); 
          sint MAX    (sint byte_nr, sint *PrzesunWewRam); 
          sint MIN    (sint byte_nr, sint *PrzesunWewRam); 
          sint LIM    (sint byte_nr, sint *PrzesunWewRam);
          sint LMS    (sint byte_nr, sint *PrzesunWewRam);
          sint ZF10   (sint byte_nr, sint *PrzesunWewRam); 
          sint SYG1   (sint byte_nr, sint *PrzesunWewRam); 
          sint KLU    (sint byte_nr, sint *PrzesunWewRam);
          sint PAZB   (sint byte_nr, sint *PrzesunWewRam);
          sint SUL    (sint byte_nr, sint *PrzesunWewRam);
          sint ILL    (sint byte_nr, sint *PrzesunWewRam);
          sint SUB    (sint byte_nr, sint *PrzesunWewRam); 
          sint ILB    (sint byte_nr, sint *PrzesunWewRam); 
          sint SUB16  (sint byte_nr, sint *PrzesunWewRam);
          sint NEG    (sint byte_nr, sint *PrzesunWewRam);
          sint BZW    (sint byte_nr, sint *PrzesunWewRam);
          sint TLI    (sint byte_nr, sint *PrzesunWewRam);
          sint TFL    (sint byte_nr, sint *PrzesunWewRam);
          sint TLI1   (sint byte_nr, sint *PrzesunWewRam);
          sint PST    (sint byte_nr, sint *PrzesunWewRam);
          sint PWB    (sint byte_nr, sint *PrzesunWewRam);
          sint PWA    (sint byte_nr, sint *PrzesunWewRam);
          sint SKL    (sint byte_nr, sint *PrzesunWewRam);
          sint B23    (sint byte_nr, sint *PrzesunWewRam);
          sint SUN    (sint byte_nr, sint *PrzesunWewRam);
          sint MNF    (sint byte_nr, sint *PrzesunWewRam);
          sint A23    (sint byte_nr, sint *PrzesunWewRam);
          sint DIF    (sint byte_nr, sint *PrzesunWewRam);
          sint FFDT   (sint byte_nr, sint *PrzesunWewRam);
          sint FFR    (sint byte_nr, sint *PrzesunWewRam);
          sint DYS    (sint byte_nr, sint *PrzesunWewRam);
          sint ILN    (sint byte_nr, sint *PrzesunWewRam);
          sint LNZ    (sint byte_nr, sint *PrzesunWewRam);
          sint SYG2   (sint byte_nr, sint *PrzesunWewRam);
          sint ZF8    (sint byte_nr, sint *PrzesunWewRam);
          sint INT    (sint byte_nr, sint *PrzesunWewRam);
          sint PAZB   (sint byte_nr, sint *PrzesunWewRam); 
          sint PBZB   (sint byte_nr, sint *PrzesunWewRam);
          sint PBZM   (sint byte_nr, sint *PrzesunWewRam);
          sint PADB   (sint byte_nr, sint *PrzesunWewRam);
          sint PBDB   (sint byte_nr, sint *PrzesunWewRam);
          sint PBDM   (sint byte_nr, sint *PrzesunWewRam);
            
          sint MOD_S  (sint byte_nr, sint *PrzesunWewRam);
          sint COM_S (sint byte_nr, sint *PrzesunWewRam);
          
//------------------Koniec funkcji komunikacji z RS-Win`em----------------------
          
          
          
void SaveToReg(char Data1, char Data2);
void CleanReg(void);
static void ForwardUSB2Serial(unsigned char *pBuffer,
                              unsigned char bStatus,
                              unsigned int dBytesTransferred,
                              unsigned int dBytesRemaining);



typedef union _ConversWpis{
                        unsigned long long Wpis_DW;
                        char               Wpis_B[8];
                      }ConversB_DW_Wpis;



typedef struct{
          signed char x;    //offset x dla ekranu dotykowego
          signed char y;    //offset y dla ekranu dotykowego
          char reserve[2];  //rezerwa i wyrównanie do 4B
        }_Offset;



typedef struct {
        float  A;       //wspó³czynnik kierunkowy funkcji y=Ax+B
        float  B;       //przesuniêcie funkcji y=Ax+B
        }_Correct;

typedef struct __FLASH_REG  
                          {
                            _Correct Cor_Y;
                            _Correct Cor_X;
                          }_FLASH_REG;


typedef struct{
                char  Enable;  //1- aktywny
                char  Week;    //tygodniowy przebieg aktywnoœci alarmów
                char  Hour;    //godznia uaktywnienia
                char  Minute;  //minuta uaktywnienia
              }__Task; 

typedef struct{
                char    TaskAmount;
                __Task  Task[TimetableTasks];
                
                
              }__Timetable; 

typedef struct{
                char        On;             //1- WiFi aktywn
                char        IpNotAuto;       //Nieautomatyczny Ip
                char        DNS_NotAuto;     //nie automatyczny DNS
                char        rez2;
                char        IP[15];
                char        Mask[15];
                char        Gate[15];
                char        DNS1[15];
                char        DNS2[15];
                char        Key[15];
                char        SSID[15];
                char        Domena[15];
                char        Subdomena[15];
                char        Path[15];
                char        rez3[4];
                short       HttpQueryPeriod; // x100[ms] okres odpytywania hosta
                short       rez4;
              }_WiFi;

 

typedef struct{   
          _Offset Ofset;     //offset x,y dla ekranu dotykowego
          char    NeedPass1; //haslo1 wymagane jeœeli = 0xEB, haslo1 nie wymagane jeœeli = 0xEC, inna wartoœæ haslo domyslne
          char    Pass1[9];  //has³o 1
          char    RF_channel; //kanal RF
          char    FerstRun;    //Pierwsze uruchomienie starownika je¿eli ró¿ne od 0xA;
          char    SlaveMode; // Tryb slave/master
          char    SoundOff;   // Dzwiêk wy³¹czony
          char    ScreenSever_On; //1-Wygaszacz aktywny
          char    ScreenSever_Time_On; //godzina w³¹czenia wygaszacz
          char    ScreenSever_Time_Off; //godzina wy³¹czenia wygaszacz
          char    ThermostatWeek;        //wybór ThermostatChart_1 lub ThermostatChart_2 w zale¿noœci od dnia tygodnia
          char    AlarmPass[ScreenNr][4];           //has³a do wbudowanej centralki alarmowej
          char    ScreenBrightActive;             //podœwietlenie ekranu aktywnego w %
          char    ScreenBrightInactive;           //podœwietlenie ekranu nieaktywnego w %
          char    rez[4];               //rezerwa
          short   ThermostatT1;          //temperatura T1 termostatu
          short   ThermostatT2;          //temperatura T2 termostatu
          long    ThermostatChart_1;      //dzienny przebieg (1) temperatury dla termostatu    
          long    ThermostatChart_2;      //dzienny przebieg (2) temperatury dla termostatu  
          _Correct      Cor_Y;
          _Correct      Cor_X;
          __Timetable   Timetable[ScreenNr];  //ustawienia terminarza
          _WiFi   WiFi;
        }_Config;
      


  

#define AT91C_IFLASH_MEM     ((AT91PS_MEM)      0x00130000) // Obszar flash 	


#define UseProgramSize   55000//0x10000                   //przestrzeñ na programu u¿ytkowy (64kB) 

#define FlashProgramPage   0;
#define FlashProgram_A_Ofset    0;
//#define FlashProgram_B_Ofset    UseProgramSize+256;
#define AT91C_RSTC_KEY        ((unsigned int) 0xA5 << 24) // (RSTC) Password


typedef struct _AT91S_MEM {                       // struktura dziel¹ca pojedyñcz¹ stronê jak ni¿ej:
	    AT91_REG	         MEM_PAGE_0[64]; 	  // 256 B      //strona tymczasowego zapisu pamiêci flash
            char	         FlashProgram[UseProgramSize];  // 48 kB *2 +256B(info dodatkowe)    0x120100
            _Config              Config;
            int                  NODE_ID;
} AT91S_MEM, *AT91PS_MEM;



typedef struct __AlarmScreenAction{
                  char  Active;
                  char  SecToActive;
                  
              }_AlarmScreenAction;

typedef _AlarmScreenAction _AlarmScreenActionType[ScreenNr];
extern _AlarmScreenActionType AlarmScreenAction;





//*----------------------------------------------------------------------------
//* \fn    AT91F_PIO_CfgNoPullup
//* \brief Disable pullup on PIO
//*----------------------------------------------------------------------------
__inline void AT91F_PIO_CfgNoPullup(
	AT91PS_PIO pPio,             // \arg pointer to a PIO controller
	unsigned int pullupEnable)   // \arg enable pullup on PIO
{
		// Connect or not Pullup
	pPio->PIO_PPUER = ~pullupEnable;
	pPio->PIO_PPUDR = pullupEnable;
}



extern long long ButtonBipCounter;
extern unsigned long       g_seconds_counter;                                      //globalny licznik sekundowy od w³¹czenia/restartu sterownika
#endif
