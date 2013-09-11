//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                  Microcontroller Software Support                        *//                       
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : RF_Exe.f                                 *//
//*  Opis                         : Plik nag��wkowy dla pliku    RF_Exe      *//
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


#ifndef RF_exe_h
#define RF_exe_h

#include "RF.h"

#include "Application_prog.h"

 

//#define RF_ID         100139
//#define RF_ID         100141       //ID urz�dzenia   //domy�lne-testowe 131385



static const unsigned int NODE_ID=9999999;
//#define RF_ID_R       3000000+RF_Status.RF_ID  //ID urz�dzenia + typ urz�dzenia




#define MAX_TIME_FOR_SLAVE_ASK   350     //maksymalny czas na odpytanie slave. Po tym czasie prze��czam si� na tryb Master
#define SLAVE_MODE_TIMEOUT       10      //timeout trybu slave - zerowany odebraniem danych do w�a�ciwego w�z�a. 

//Kody rozkaz�w transmisji RF
#define R_RegOrd        0x1   //Odczyt pojedy�czego rejestru -rozkaz
#define R_RegRes        0x9   //Odczyt pojedy�czego rejestru -odpowied�
#define W_RegOrd        0x2   //Zapis pojedy�czego rejestru  -rozkaz
#define W_RegRes        0xA   //Zapis pojedy�czego rejestru  -odpowied�
#define R_RegOutOrd     0x3   //Odczyt pojedy�czego rejestru wyj�ciowego -rozkaz
#define R_RegOutRes     0xB   //Odczyt pojedy�czego rejestru wyj�ciowego -odpowied�
#define R_WhoIs         0x4   //Ramka rog�o�na: kto si� zg�asza
#define R_Iam           0xC   //Odpowied� na ramk� rozg�osn�: ja jestem
#define W_NewIdOrd      0x5   //Zapis nowego numeru ID  -rozkaz
#define W_NewIdRes      0xD   //Zapis nowego numeru ID  -odpowied�/potwierdzenie
#define RW_RegOrd       0x6   //Zapis + dczyt pojedy�czego rejestru -rozkaz
#define RW_RegRes       0xE   //Zapis + dczyt pojedy�czego rejestru -odpowied�
#define R_BC_Order      0x7   //Ramka rozg�o�na z zadaniem odpytania 
#define W_BC_Order      0x8   //Ramka rozg�o�na z zadaniem zapisu 
#define RW_BC_Res       0xF   //Ramka rozg�o�na -odpowied�



//typy urz�dze�
//1xxxxxx 	- W�ze� uniwersalny, stale w��czony, z komunikacj� RS485
//2xxxxxx	- W�ze� uniwersalny, stale w��czony, z komunikacj� RS485 oraz komunikacja radiowa
//3xxxxxx 	- W�ze� uniwersalny, stale w��czony, komunikacja radiowa
//4xxxxxx	- Pilot, komunikacja radiowa
//5xxxxxx 	- Czujnik za��czany okresowo, komunikacja radiowa


//131385





#define RF_TimeOut        9            //wst�pnie ustawiony timeout w [ms] (obejmuje czas odbioru)
#define RF_ChannelQuanti  40        //Ilo�� kana��w
#define RF_ChannelWidth   0.300     //Szeroko�� kana�u / odst�py pomiedzy kana�ami w MHz
#define RF_BaseFrq        860.32    //Cz�stotliwo�c no�na kana�u 0 w MHz  
#define RF_BuffSizeRX     20        //Wielko�� bufora (kolejki fifo) odbiorczego 
#define RF_BuffSizeTX     20        //Wielko�� bufora (kolejki fifo) nadawczego
#define RF_Tasks          NumberNeurons*2   //maksymalna ilo�� zada� 



//struktura danych rozkazu RF
typedef struct __RF_Order
{
        unsigned int             ID    : 20,     //Id Slave
                                 Order : 4,      //Kod rozkazu
                                 Auxi  : 8;      //Pole pomocnicze, np. Nr rejestru (konieczne z powodu wyr�wnania bajtowego) 
} _RF_Order;


typedef struct __RF_6B
        {
        unsigned short  CRC;                      //suma crc
        }_RF_6B;


typedef struct __RF_8B
        {
        unsigned short  Val;                      //warto��
        unsigned short  CRC;                      //suma crc
        
        }_RF_8B;




#pragma pack(1)
typedef union __RF_Command
{
  struct
  {
    unsigned int             ID    : 20,     //Id Slave
                             Order : 4,      //Kod rozkazu
                             Auxi  : 8;      //Pole pomocnicze, np. Nr rejestru (konieczne z powodu wyr�wnania bajtowego) 
    union                                    //Dane + CRC
    {
      _RF_6B  B6;                            //Dane + CRC dla rozkazu d�ugo�ci 6B
      _RF_8B  B8;                            //Dane + CRC dla rozkazu d�ugo�ci 8B
    };
  };
  struct  //struktura rozkazu o zmian� ID
  {
    unsigned long long       CHID_CUR_ID    : 20,     //Bierz�cy Id - Slave
                             CHID_Order     : 4,      //Kod rozkazu
                             CHID_KODE      : 4,      //kod kontrolny
                             CHID_NEW_ID    : 20,     //Nowy Id - Slave
                             CHID_CRC       : 16;     //suma crc
  };
    struct  //struktura rozkazu wywo�ania og�lnego (broadcasting)
  {
    unsigned long long       BC_ID1       : 20,     //Id rozg�o�ne
                             BC_Order     : 4,      //Kod rozkazu 
                             BC_REG      : 4,      //pytanie (kto ma po��czenie z w�z�em, potrzebuje rejestr nr..
                             BC_ID2       : 20,     //Id szukane
                             BC_CRC       : 16;     //suma crc
  };
  
  
  char Streem[8];                             //Strumie� rozkazu  
}_RF_Command, *_pRF_Command;
  #pragma pack()  



//tablica zada� uk�adu transmisji radiowej
typedef struct{
            char        Node;  //nr w�z�a sieci
            _RF_Order   Command; //nr ID + Rozkaz
}_RF_Task;


typedef _RF_Task _RF_TaskTab[RF_Tasks];


typedef struct __RF_NodeTest
{
  char          NodeNr;     //numer sprawdzanego w�zla
  unsigned int  Sent;       //danych wys�anych
  unsigned int  SentSyn;     //danych wys�anych zsynchronizowane z odebranymi
  unsigned int  Recived;    //danych odebranych
}_RF_NodeTest;






typedef enum{
      mS300      =1,
      mS300i     =2,
      mS300iMax  =3,
      MG1       =4,
      MG2       =5,      
      MR1       =6,
      MR2       =7,      
      MGZ1      =8,
      WB1       =9,
      WB2       =10,
      CT1       =11,
      CO1       =12,
      PP1_7     =13,
      PDR1      =14
    }_NodeModel;


typedef union{
          struct
          {
            _NodeModel      Model;
            unsigned char   SoftVersion     :4,
                            SoftSubVersion  :4;
          };
          unsigned short    NodeInfoWord;
          unsigned char     NodeInfoBytes[2];
}_NodeInfo;




char RF_ReciveFarm(char *Buff, char Size);
void RF_SendData(unsigned char *data, char size);
void RF_ExecuteOrder(_RF_Command *Command);
void RF_MasterExecut(void);
void SwitchToMaster(void);
void SwitchToSlave(void);



#endif 
