//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                  Microcontroller Software Support                        *//                       
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : RF.h                                     *//
//*  Opis                         : Plik nag³ówkowy dla RF.c                 *//
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


#ifndef RF_h
#define RF_h



//#define RF_Master  //urz¹dzenie pracuje jako master (sterownik)
#define RF_Slave     //urz¹dzenie pracuje jako slave 



#define RF_ChannelWidth   0.300     //Szerokoœæ kana³u / odstêpy pomiedzy kana³ami w MHz
#define RF_BaseFrq        860.32    //Czêstotliwoœc noœna kana³u 0 w MHz  
#define RF_BuffSizeRX     20        //Wielkoœæ bufora (kolejki fifo) odbiorczego 
#define RF_BuffSizeTX     20        //Wielkoœæ bufora (kolejki fifo) nadawczego
#define MAX_TIME_TO_REM_BRIDGE_NO_CON  30//liczba sekund przez które próbuje siê skomunikowaæ mostowo modu³ w tybie slave - mesh


//#define RfIntBUS              AT91C_PIO_PA3    //dla makiety
#define RfIntBUS              AT91C_PIO_PA10       //dla TP64128
#define PIO_RfIntBUS          AT91C_BASE_PIOA
#define ID_RfIntBUS           AT91C_ID_PIOA


#define RF_SetFreqReg(ch) (short int)(200*(RF_BaseFrq + ch*RF_ChannelWidth) -172000)



typedef struct __RF_Rx
{
  char Buff[RF_BuffSizeRX];
  char ReciveBytes;
}_RF_Rx;


typedef struct __RF_Tx
{
  char Buff[RF_BuffSizeTX];  //bufor typu kolejka
  char BytesToSend;
}_RF_Tx;







typedef struct __RF_Status
{
  unsigned char   Init;      //je¿eli 1- uk³ad w trakcie inicjalizacji
  unsigned char   EnRx;      // 1- uk³ad w trybie odbioru
  unsigned char   EnTx;      // 1- uk³ad w trybie nadawania
  unsigned char   InitTx;    // 1- uk³ad w trybie prze³¹czania na nadajnik. Trwa oczekiwanie na w³¹czenie nadajnika
  unsigned char   InitRx;    // 1- uk³ad w trybie prze³¹czania na odbiornik. Trwa oczekiwanie na wy³¹czenie nadajnika
  unsigned char   TxStarting; //1- Tx startuje
  unsigned char   RxStarting; //1- Rx startuje
  unsigned char   Resive;    // 1- uk³ad w trakcie odbioru
  unsigned char   Transmit;  // 1- uk³ad w trakcie nadawania
  unsigned char   ResByte;   //iloœæ odebranych danych
  unsigned char   EnRxWait;  // 1- uk³ad oczekuje na prze³¹czenie w tryb odbioru które nast¹pi po wys³aniu wszystkich danych
  unsigned char   ConfResive; //potwierdzenie transmisji do sterownika w trybie master. 1-odebrano dane, 2-odebrano dane z poprawnym ID
  unsigned char   ConfResiveSlave; //potwierdzenie transmisji do sterownika wtrybie slave. 1-odebrano dane, 2-odebrano dane z poprawnym ID
  unsigned char   MasterMode;  //tryb RF master
  unsigned char   Ch;         //Nr aktualnego kana³u dla trybu Master
  unsigned char   ChSlave;    //Nr aktualnego kana³u dla trybu Slave
  unsigned char   AllReady;   //wszystko gotowe 
  unsigned char   RfFuse;     //zabezpieczenie przed zablokowaniem RF. Przy inicjalizacji RF RfFuse=0. Po odebraniu poprawnej ramki RfFuse ustawiany jest na 1;  Timer 1s inkrementuje RfFuse. Je¿eli RfFuse dojdzie do okreœlonego czasu to inicjalizuje siê RF
  unsigned char   TimeToRemBridgeNoCom;   //czas do usuniêcia wêz³ów mostowych (u¿ywanych w transmisji mesh) z którymi nie ma komunikacji
  unsigned char   SlaveModeTimeout;       //timeout dla prze³¹czenia z trybu slave na master
  unsigned short  TimeInMasterMode;    //czas przebywania w trybie master. Zmienna s³u¿y pilnowaniu nie przekroczenia czasu 350ms w trybie master
  unsigned short  TimeInSlaveMode;    //czas przebywania w trybie slave. Zmienna s³u¿y pilnowaniu nie przekroczenia czasu 350ms w trybie slave
  unsigned int  RF_ID;        //numer wêz³a skrócony      
  unsigned int    RF_ID_R;      //pe³ny numer wêz³a
  void            (*RF_Status)(void);  //konstruktor
  void            (*EnableTX)(void);
  void            (*EnableRX)(void);
}_RF_Status;







void RF_ReciveByte(char Res);
void SPI_RFM12_Execute(char *Buffer_Tx, char *Buffer_Rx);
void RF_Init(void);
void RF_transmit(void);
void RF_SendFarm(unsigned char *data, char size);
void RF_ReciverTest(void);
void RF_TransmiterTest(void);
char RF_CheckChannel(void);





#endif
