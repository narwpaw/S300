#ifndef RF_h
#define RF_h



//#define RF_Master  //urz�dzenie pracuje jako master (sterownik)
#define RF_Slave     //urz�dzenie pracuje jako slave 

#define RF_ID         0x20135       //ID urz�dzenia
#define RF_ID_Master  0x20145       //Id master - tymczasowe 
#define RF_TimeOut    15            //wst�pnie ustawiony timeout w [ms] (obejmuje czas odbioru)
#define RF_ChannelQuanti  20        //Ilo�� kana��w
#define RF_ChannelWidth   0.300     //Szeroko�� kana�u / odst�py pomiedzy kana�ami w MHz
#define RF_BaseFrq        860.32    //Cz�stotliwo�c no�na kana�u 0 w MHz  
#define RF_BuffSizeRX     20        //Wielko�� bufora (kolejki fifo) odbiorczego 
#define RF_BuffSizeTX     20        //Wielko�� bufora (kolejki fifo) nadawczego
#define RF_Tasks          40        //maksymalna ilo�� zada� 

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
  char Init;      //je�eli 1- uk�ad w trakcie inicjalizacji
  char EnRx;      // 1- uk�ad w trybie odbioru
  char EnTx;      // 1- uk�ad w trybie odbioru
  char InitRx;    // 1- uk�ad w trybie oczekiwania na wy��czenie TX / sprawdzania wy��czenia TX
  char Resive;    // 1- uk�ad w trakcie odbioru
  char Transmit;  // 1- uk�ad w trakcie odbioru
  char ResByte;   //ilo�� odebranych danych
  char EnRxWait;  // 1- uk�ad oczekuje na prze��czenie w tryb odbioru kt�re nast�pi po wys�aniu wszystkich danych
  char Ch;        //Nr aktualnego kana�u
  void (*RF_Status)(void);  //konstruktor
  void (*EnableTX)(void);
  void (*EnableRX)(void);

}_RF_Status;



//Kody rozkaz�w transmisji RF
#define R_RegOrd        0x1   //Odczyt pojedy�czego rejestru -rozkaz
#define R_RegRes        0x9   //Odczyt pojedy�czego rejestru -odpowied�
#define W_RegOrd        0x2   //Zapis pojedy�czego rejestru  -rozkaz
#define W_RegRes        0xA   //Zapis pojedy�czego rejestru  -odpowied�




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
  char Streem[8];                             //Strumie� rozkazu  
}_RF_Command, *_pRF_Command;
  #pragma pack()  



//tablica zada� uk�adu transmisji radiowej
typedef struct{
            char        Node;  //nr w�z�a sieci
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