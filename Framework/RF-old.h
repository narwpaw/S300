#ifndef RF_h
#define RF_h



//#define RF_Master  //urz¹dzenie pracuje jako master (sterownik)
#define RF_Slave     //urz¹dzenie pracuje jako slave 

#define RF_ID         0x20135       //ID urz¹dzenia
#define RF_ID_Master  0x20145       //Id master - tymczasowe 
#define RF_TimeOut    15            //wstêpnie ustawiony timeout w [ms] (obejmuje czas odbioru)
#define RF_ChannelQuanti  20        //Iloœæ kana³ów
#define RF_ChannelWidth   0.300     //Szerokoœæ kana³u / odstêpy pomiedzy kana³ami w MHz
#define RF_BaseFrq        860.32    //Czêstotliwoœc noœna kana³u 0 w MHz  
#define RF_BuffSizeRX     20        //Wielkoœæ bufora (kolejki fifo) odbiorczego 
#define RF_BuffSizeTX     20        //Wielkoœæ bufora (kolejki fifo) nadawczego
#define RF_Tasks          40        //maksymalna iloœæ zadañ 

#define RfIntBUS              AT91C_PIO_PA10    //dla MSU101
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
  char Init;      //je¿eli 1- uk³ad w trakcie inicjalizacji
  char EnRx;      // 1- uk³ad w trybie odbioru
  char EnTx;      // 1- uk³ad w trybie odbioru
  char InitRx;    // 1- uk³ad w trybie oczekiwania na wy³¹czenie TX / sprawdzania wy³¹czenia TX
  char Resive;    // 1- uk³ad w trakcie odbioru
  char Transmit;  // 1- uk³ad w trakcie odbioru
  char ResByte;   //iloœæ odebranych danych
  char EnRxWait;  // 1- uk³ad oczekuje na prze³¹czenie w tryb odbioru które nast¹pi po wys³aniu wszystkich danych
  char Ch;        //Nr aktualnego kana³u
  void (*RF_Status)(void);  //konstruktor
  void (*EnableTX)(void);
  void (*EnableRX)(void);

}_RF_Status;



//Kody rozkazów transmisji RF
#define R_RegOrd        0x1   //Odczyt pojedyñczego rejestru -rozkaz
#define R_RegRes        0x9   //Odczyt pojedyñczego rejestru -odpowiedŸ
#define W_RegOrd        0x2   //Zapis pojedyñczego rejestru  -rozkaz
#define W_RegRes        0xA   //Zapis pojedyñczego rejestru  -odpowiedŸ




//struktura danych rozkazu RF
typedef struct __RF_Order
{
        unsigned int             ID    : 20,     //Id Slave
                                 Order : 4,      //Kod rozkazu
                                 Auxi  : 8;      //Pole pomocnicze, np. Nr rejestru (konieczne z powodu wyrównania bajtowego) 
} _RF_Order;


typedef struct __RF_6B
        {
        unsigned short  CRC;                      //suma crc
        }_RF_6B;


typedef struct __RF_8B
        {
        unsigned short  Val;                      //wartoœæ
        unsigned short  CRC;                      //suma crc
        }_RF_8B;

#pragma pack(1)
typedef union __RF_Command
{
  struct
  {
    unsigned int             ID    : 20,     //Id Slave
                             Order : 4,      //Kod rozkazu
                             Auxi  : 8;      //Pole pomocnicze, np. Nr rejestru (konieczne z powodu wyrównania bajtowego) 
    union                                    //Dane + CRC
    {
      _RF_6B  B6;                            //Dane + CRC dla rozkazu d³ugoœci 6B
      _RF_8B  B8;                            //Dane + CRC dla rozkazu d³ugoœci 8B
    };
  };
  char Streem[8];                             //Strumieñ rozkazu  
}_RF_Command, *_pRF_Command;
  #pragma pack()  



//tablica zadañ uk³adu transmisji radiowej
typedef struct{
            char        Node;  //nr wêz³a sieci
            _RF_Order   Command; //nr ID + Rozkaz
}_RF_Task;




typedef _RF_Task _RF_TaskTab[RF_Tasks];


void RF_ReciveByte(char Res);
char SPI_RFM12_Execute(char *Buffer_Tx, char *Buffer_Rx);
void RF_Init(void);
void RF_transmit(void);
void RF_SendData(unsigned char *data, char size);
void RF_ExecuteOrder(_RF_Command *Command);

void RF_ReciverTest(void);
void RF_TransmiterTest(void);
void RF_MasterExecut(void);
#endif