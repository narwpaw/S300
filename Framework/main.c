//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                                 CEURON                                   *//                     
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : main.c                                   *//
//*  Opis                         : G³ówny plik programu                     *//
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
  


//Sta³e ------------------------------------------------------------------------
#ifdef S300
    const char identyfikacja[]="S300 ";                                          //nazwa urz¹dzeni 
#endif
#ifdef S300i
    const char identyfikacja[]="S300-i ";                                          //nazwa urz¹dzeni 
#endif
#ifdef S300iMax
    const char identyfikacja[]="S300-iMax ";                                          //nazwa urz¹dzeni 
#endif
#ifdef S300iOld
    const char identyfikacja[]="S300-i(Old) ";                                          //nazwa urz¹dzeni 
#endif
    
    const char system[]="1.5";                                                  //nazwa urz¹dzeni 
    
//____________________________Koniec sta³ych ___________________________________



//Konfiguracja -----------------------------------------------------------------
    #define LightLCD_Time              20                                       //czas (s sek.) wygaszania podœwietlenia
//___________________________Koniec konfiguracji________________________________




//Definicje --------------------------------------------------------------------
    #define MSG_SIZE 	                1000                                    
    #define TAB_SIZE                    1000                                    
    #define USB_BUFFER_SIZE             300                                     //bufor odbiorczy USB  
   
   // #define NOTRACES              `                                              //Wy³¹cza debagowania poprzez US1
    
//___________________________Koniec definicji___________________________________



// Pliki do³¹czane -------------------------------------------------------------

     #include "main_H.h"
     #include "WizFi.h"



//-----------------------Koniec plików do³¹czanych -----------------------------










char APC;





//Zmienne globalne -------------------------------------------------------------
    char g_czas_petli = 10;                                                     //  Czas przebiegu pêtli programu w ms
    unsigned int komunikacja_ok=0;                                              //1-ok, 0-brak
    int  rej_prog[10];
    int LifeCouter = 0, LifeCouterMax = 0;                                      // Zmienne funkcji LifeBlinking
    byte USB_busy;
    int FlashProgramOfset =  256;                                               //0-wykonywany program A, 
    int FlashProgramReserveOfset =  256+UseProgramSize;                         //0-wykonywany program A, 
    int  advY =0;                                                                  //chwilowa wspolzedna Y TP
    int  advX =0;                                                                  //chwilowa wspolzedna X TP
     signed short  Y_down  =-1;                                                           //wspolzedna Y TP (naciœniêcie)
     signed short  X_down  =-1;                                                           //wspolzedna X TP (naciœniêcie)
     signed short  Y_up    =-1;                                                           //wspolzedna Y TP (puszczenie)
     signed short  X_up    =-1;                                                           //wspolzedna X TP (puszczenie)
    int  g_leng  =0;                                                            //jêzyk 1-polski, 2 angielski
    char g_level =0;                                                            //poziom dostêpu 0-ograniczony, 1- operator, 2-administrator
    char g_Password_1[8]={"1111"};                                              //haslo poziomu 1  
    char g_Password_2[8]={"2222"};                                              //haslo poziomu 1  
    long int g_Message =0;                                                             //1-wiadomosc ekranowa
    int g_info=0;                                                                   
    char g_PC_Connected;                                                       //1-pod³aczony komputer
    unsigned long long  g_counter;                                              //globalny licznik milisekundowy od w³¹czenia/restartu sterownika
    unsigned long       g_seconds_counter;                                      //globalny licznik sekundowy od w³¹czenia/restartu sterownika
    
//    static char US0_Buffer[US_BUFFER_SIZE];                                    //bufor odbiorczy USART 0
//    static char US0_Buffer2[US_BUFFER_SIZE];                                   //zapasowy bufor odbiorczy USART 0
    char AdressModbus = 1;                                                      //Adres urz¹dzenia dla protoko³u modbus
    int g_LightCouterLCD = 0;                                                   //licznik podœwietlenia lcd
    char g_LightLCDEnable = 0;                                                   //podœwietlenia lcd
    char g_ScreenSever_On;                                                      //1-Wygaszacz aktywny
    char g_ScreenSever_Time_On;                                                 //godzina w³¹czenia wygaszacz
    char g_ScreenSever_Time_Off;                                                //godzina wy³¹czenia wygaszacza
    char g_SlaveConnect=0;                                                   //
    char g_Stop_Program = 0;                                                    //1- zatrzymuje program
    char g_Restart_Program = 1;                                                 //1- restartuje program
    unsigned long long g_NextTimeProgram = 0;                                                    //
    char g_NeedPass=0;  //wymóg has³a
    char AlarmOn=0;                                                             //1=alarm dzwiêkowy
    char Speaker[SPK_Amount];                                                   //sygna³ dŸwiêkowy
    _AlarmScreenActionType AlarmScreenAction;
    char ChangeingIdMode=0;                                                     //tryb zmiany ID blokuje ustanawianie po³¹czeñ mostowych
    _Net NET; 
    char WypMaxJasnosci;                                                        //jasnoœc ekranu przy aktywnym ekranie
    char WypMinJasnosci;                                                        //jasnoœc ekranu przy nieaktywnym ekranie
    char gWypMinJasnosciTest;                                                   //test wygl¹du ekranu z  jsnoœci¹ WypMinJasnosci   
    long long FuseLastWiFiDataIn;                                               //zabezpieczenie przed blokad¹ DMA USART. FuseLastWiFiDataIn zawiera czas ostatniej odpowiedzi od usarta. Je¿eli WiFi jest w³¹czone a USART milczy od 30s to restartuj mikroprocesor
    unsigned char ChangeStageFlag=10;                                           //ile razy (maksymalnie) sterownik ma wys³aæ zmiane stanu wyjœcia
    //___________________________Koniec zmiennych globalnych________________________





//Zmienne lokalne -------------------------------------------------------------
    char LightLCD_OFF_Counter;                                                  //zmiena œciemniacza LCD
    char LightLCD_OFF_Counter2;                                                 //zmiena œciemniacza LCD
    char LightLCD_OFF_Curent;                                                   //zmiena œciemniacza LCD
    volatile char SlaveMode=0;
   // volatile char SwitchToMasterBlock;
    volatile long long TimeInMasterMode;                                         //zmienna dla kontroli czasu w trybie slave
    volatile char ReciveFrameToMe=0;                                            //zmienna u¿ywana do prze³¹czania w tryb tymczasowy master po odebraniu ramki pobrania statusu z poprawnym id a nastêpnie ramki do obcego lub ponownym odebraniu ramki z poprawnym id i rozkazem pobrania statusu. Ramka pobrania statusu (odczyt rej. 0) zawsze wystêpuje na pocz¹tku kolejki ramek do pojedyñczego wêz³a 
//___________________________Koniec zmiennych lplkalnych________________________


    
     

    

//Zmienne zewnêtrzne -------------------------------------------------------------    
    extern char       LCD_Canvas[8][128];
    extern char       Label_move;
    extern int        MenuLevel;      //poziom menu
    extern int        Suw_poz_analog;  
    extern int        ScrollMenu_poz;
    extern char       g_US0_Connect_OK;
    extern char       g_US1_Connect_OK;
    extern char       g_US0_Comm_TIMEOUT;
    extern _ZD_MODBUS ZD_MODBUS_0[ZD_MODBUS_Tab_Max];
    extern char       g_Ans_ZD_MODBUS_0;
    extern char       g_Current_ZD_MODBUS_0;
    extern char       g_ERR_ZD_MODBUS_0;
    extern int        g_US0_Comm_MinCykle;
    extern char       US0_MasterMODBUS_ENABLE;
    extern char       g_US1_Comm_TIMEOUT;
    extern _ZD_MODBUS ZD_MODBUS_1[ZD_MODBUS_Tab_Max];
    extern char       g_Ans_ZD_MODBUS_1;
    extern char       g_Current_ZD_MODBUS_1;
    extern char       g_ERR_ZD_MODBUS_1;
    extern int        g_US1_Comm_MinCykle;
    extern char       US1_MasterMODBUS_ENABLE;
    extern char       Busy_program;
    extern _Trap      Trap;
    extern _Correct korekta_Y;
    extern _Correct korekta_X;
    extern  char Synchronizacja;
    extern char         RF_CurrentTask;
    //extern unsigned char  SPI_Buffer_Rx[SPI_Read_BUFFER_SIZE];                         //bufor odbiorczy SPI
    //extern unsigned char  SPI_Buffer_Tx[SPI_Write_BUFFER_SIZE];                        //bufor odbiorczy SPI
    extern void ReadWriteSpi (unsigned char* pReadBuffer, int nReadLen, unsigned char* pWriteBuffer, int nWriteLen, bool bHoldCS);
    extern void SPI_init(void) ;
    
    extern _MainStage  MainStage; 
    extern _ProgramTab gProg;
    extern void RF_Init(void);
    extern _RF_Status RF_Status;
    extern char g_RF_TimeOutCounter;
    extern   _RF_TaskTab  RF_TaskTab;
    extern _TransQuality TransQuality;
    extern _DateTime  *pDateTime;
    extern char Variables[];
    extern unsigned int CyklPerSek;
    extern char Buff_TX_1[];
    AT91PS_MEM pAT91C_IFLASH_MEM=AT91C_IFLASH_MEM;
    extern short int VariablesAnal[];
    extern char Variables[];
    extern unsigned char  US1_Buffer[];
    extern  unsigned  short  TimeToSwithToMaster;
    extern _NeuronsListUnit NeuronsList[];
    extern  const unsigned int NODE_ID;
//___________________________Koniec zmiennych zewnêtrznych________________________    
    
    
    
//Zmienne tymczasowe -----------------------------------------------------------
    char tmp_i =0; 
    float licznik = 0.0;
    int licznik2 = 0;
    int licznik3 = 0;
    char *memory;
    unsigned long sss;
    int int1 =0;
    

    _DateTime DateTime;
    
     
    //___________________________Koniec zmiennych tymczasowych______________________


    

//Funkcje zewnêtrzne -----------------------------------------------------------
    extern RAM ram;
    extern ConversFC Convers_DW_B;
    extern void InitFlash(void);
    extern void Program (_ProgramTab *ProgTab);
    extern void at91flashWriteDW(int sektor, int nr_DW, unsigned long wartDW );
    extern void ScreenEvent(void);
    extern int Suw_poz;
    extern void MenuPaint(void);
    extern void at91flashWrite();
    extern void ConfigureIrBus(void);
    extern void MenuRefresh();
    //extern unsigned short crc16(unsigned char* modbusframe,int Length);
    //extern unsigned char ModBus_CommS(unsigned char* modbusframe);
    //extern void ModBus_CommS_US0();
    extern void US0_init(void);
    extern void US1_init(void);
    extern char ModBus_MASTER_TX(char US_Nr);
    extern void ReadTP(_TPoint *Point);
    //extern BYTE ModBus_MASTER_Return(BYTE* modbusframe);
//_______________________ Koniec funkcji zewnêtrznych __________________________
    

/*
$Id: serial_example.c,v 1.1.2.1 2006/12/05 08:33:25 danielru Exp $
*/




//------------------------------------------------------------------------------
//      Prototypes
//------------------------------------------------------------------------------

//! \brief  Initialization callback
static void CBK_Init(const S_usb *pUsb);

//! \brief  Suspend callback
static void CBK_Suspend(const S_usb *pUsb);

//! \brief  Resume callback
static void CBK_Resume(const S_usb *pUsb);

//! \brief  New request callback
static void CBK_NewRequest(const S_usb *pUsb);

//! \brief  New reset callback
//static void CBK_Reset(const S_usb *pUsb);

//! \brief  New SOF callback
//static void CBK_SOF(const S_usb *pUsb);

//------------------------------------------------------------------------------
//      Internal variables
//------------------------------------------------------------------------------

//! \brief  List of endpoints (including endpoint 0) used by the device.
//! \see    S_usb_endpoint
static S_usb_endpoint pEndpoints[] = {

    USB_ENDPOINT_SINGLEBANK, // Control endpoint 0
    USB_ENDPOINT_DUALBANK,   // Data out endpoint
    USB_ENDPOINT_DUALBANK,   // Data in endpoint
    USB_ENDPOINT_SINGLEBANK, // Notification endpoint
};

//! \brief  Variable used to store the last received SETUP packet.
//! \see    S_usb_request
//! \see    S_usb
static S_usb_request sSetup;

//! \brief  Variable used to store the current device state
//! \see    S_usb
static unsigned int dState;

//! \brief  List of implemented callbacks
//! \see    S_usb_callbacks
//! \see    S_usb
static const S_usb_callbacks sCallbacks = {

    CBK_Init,
    0, // CBK_Reset
    0, //CBK_Suspend,
    CBK_Resume,
    CBK_NewRequest,
    0  // CBK_SOF
};

//! \brief  USB driver instance
//! \see    S_usb
static const S_usb sUsb = {

    &sDefaultDriver,
    pEndpoints,
    SER_NUM_ENDPOINTS,
    &sCallbacks,
    &sSetup,
    &dState
};

//! \brief  CDC serial class driver instance
//! \see    S_ser
static S_ser sSer;

// Buffer for receiving data from the USB
static unsigned char pUsbBuffer[USB_BUFFER_SIZE];


//------------------------------------------------------------------------------
//      Internal Functions
//------------------------------------------------------------------------------

// Interrupt Service Routines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//! \brief  Handler for the USB controller interrupt
//!
//!         Defers the call to the USB_Handler function.
//------------------------------------------------------------------------------
void ISR_Driver(void)
{
    USB_Handler(&sUsb);
}//_______________________ Koniec funkcji ISR_Driver ____________________________




//------------------------------------------------------------------------------
//! \brief  Handler for the VBus state change interrupt
//!
//! This method calls the USB_Attach function to perform the necessary
//! operations.
//------------------------------------------------------------------------------
//#if !defined(USB_BUS_POWERED)
char ForwardUSB2Serialferst=0;


 

void ISR_VBus(void)
{
  
  
  USB_Attach(&sUsb);

  if ((( AT91C_BASE_PIOA->PIO_PDSR) & AT91C_VBUS)>0)
  {
    g_PC_Connected=1;
    g_Stop_Program=1;
    
    if (ISSET(USB_GetState(&sUsb), USB_STATE_POWERED)) 
    {
      MenuLevel=1;
      g_PC_Connected=1;
      MenuPaint();
      RepaintLCD();
      RepaintLCD();
     
      USB_Connect(&sUsb);   
      

     // if (ForwardUSB2Serialferst!=1)
     // {
        ForwardUSB2Serialferst=1; 
     // }
      
      g_LightCouterLCD = 0;     //W³¹cza podœwietlenie wyœwietlacza LCD
      if (g_LightLCDEnable==0) g_LightLCDEnable=1;

    }
      else
    {

      USB_Disconnect(&sUsb);  
      g_PC_Connected=0;
      MenuPaint();
      g_LightCouterLCD = 0;     //W³¹cza podœwietlenie wyœwietlacza LCD
      if (g_LightLCDEnable==0) g_LightLCDEnable=1;
      Trap.Enable=0;
      Trap.Row=0;
      Trap.Col=0;
      Trap.Activ=0;
      g_Stop_Program=0;
      

    }
    MenuPaint();
  }
  else
  {
    USB_Disconnect(&sUsb);  
    g_PC_Connected=0;
    g_Stop_Program=0;
    MenuPaint();
    g_LightCouterLCD = 0;     //W³¹cza podœwietlenie wyœwietlacza LCD
    if (g_LightLCDEnable==0) g_LightLCDEnable=1;
    
    Trap.Enable=0;
    Trap.Row=0;
    Trap.Col=0;
    Trap.Activ=0;
    

  } 

}//_____________________________________________________________________________
//#endif // !defined(USB_BUS_POWERED)





// Callbacks
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//! \brief  Callback invoked during the initialization of the USB driver
//!
//!         Configures and enables USB controller and VBus monitoring interrupts
//! \param  pUsb    Pointer to a S_usb instance
//------------------------------------------------------------------------------
static void CBK_Init(const S_usb *pUsb)
{

  TRACE_INFO("\n\rInicjalizacja przerwania VBUS od podlaczenia USB  \n\r" ); 
    
    // Configure and enable the USB controller interrupt
    AT91F_AIC_ConfigureIt(AT91C_BASE_AIC,
                          USB_GetDriverID(pUsb),
                          USB_CON_PRIOR,
                          0, //AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL,
                          ISR_Driver);

    AT91F_AIC_EnableIt(AT91C_BASE_AIC, USB_GetDriverID(pUsb));

//#ifndef USB_BUS_POWERED
   
    AT91F_PIO_CfgInput(AT91C_BASE_PIOA,AT91C_VBUS);
    AT91C_BASE_PIOA->PIO_PPUDR = AT91C_VBUS;
   
    // Configure VBus monitoring
    BRD_ConfigureVBus(USB_GetDriverInterface(pUsb));

    
    
    
    
  /*  
    // Configure and enable the Vbus detection interrupt
    AT91F_AIC_ConfigureIt(AT91C_BASE_AIC,
                          AT91C_ID_VBUS,
                          USB_CON_PRIOR, //AT91C_AIC_PRIOR_LOWEST,
                          0,//AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL,
                          ISR_VBus);

    
    AT91F_PIO_InterruptEnable(AT91C_PIO_VBUS, AT91C_VBUS);
    AT91F_AIC_EnableIt(AT91C_BASE_AIC, AT91C_ID_VBUS);
  
   
    
    
    
    
    */
    
    
/*
#else
    // Power up the USB controller
    USB_Attach(pUsb);
  
#endif
*/

}//_____________________________________________________________________________























//------------------------------------------------------------------------------
//! \brief  Callback invoked when the device becomes suspended
//!
//!         Disables LEDs (if they are used) and then puts the device into
//!         low-power mode. When traces are used, the device does not enter
//!         low-power mode to avoid losing some outputs.
//! \param  pUsb    Pointer to a S_usb instance
//------------------------------------------------------------------------------
static void CBK_Suspend(const S_usb *pUsb)
{

#if defined(NOTRACES)
    DEV_Suspend();
#endif
}//_____________________________________________________________________________






//------------------------------------------------------------------------------
//! \brief  Callback invoked when the device leaves the suspended state
//!
//!         The device is first returned to a normal operating mode and LEDs are
//!         re-enabled. When traces are used, the device does not enter
//!         low-power mode to avoid losing some outputs.
//! \param  pUsb    Pointer to a S_usb instance
//------------------------------------------------------------------------------
static void CBK_Resume(const S_usb *pUsb)
{
#if defined(NOTRACES)
    DEV_Resume();
#endif

}//_____________________________________________________________________________






//------------------------------------------------------------------------------
//! \brief  Callback invoked when a new SETUP request is received
//!
//!         The new request if forwarded to the standard request handler,
//!         which performs the enumeration of the device.
//! \param  pUsb   Pointer to a S_usb instance
//------------------------------------------------------------------------------
static void CBK_NewRequest(const S_usb *pUsb)
{
    SER_RequestHandler(&sSer);
}//_____________________________________________________________________________






//------------------------------------------------------------------------------
//! \brief  Callback invoked when a Reset request is received
//!
//! \param  pUsb   Pointer to a S_usb instance
//------------------------------------------------------------------------------
//static void CBK_Reset(const S_usb *pUsb)
//{
    // Put your reset handler here
//}//_____________________________________________________________________________




//------------------------------------------------------------------------------
//! \brief  Callback invoked when a SOF is received
//!
//! \param  pUsb   Pointer to a S_usb instance
//------------------------------------------------------------------------------
//static void CBK_SOF(const S_usb *pUsb)
//{
    // Put your SOF handler here
//}//_____________________________________________________________________________




void SendViaUSB(unsigned char *Buff, unsigned short Size)
{
  SER_Write(&sSer, Buff,Size,0, 0); 
}


// Other functions
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//! \brief  Forwards data receiving from the USB host through the USART
//!
//!         This function operates asynchronously.
//! \param  pBuffer           Must be 0 when function is first called
//! \param  bStatus           Same as above
//! \param  dBytesTransferred Same as above
//! \param  dBytesRemaining   Same as above
//------------------------------------------------------------------------------
static void ForwardUSB2Serial(unsigned char *pBuffer,
                              unsigned char bStatus,
                              unsigned int dBytesTransferred,
                              unsigned int dBytesRemaining)
{
    //Zmienne lokalne ----------------------------------------------------
            static char buff_tx[900];                                          //Bufor danych wysy³anych do USB
         
            unsigned int rozmiar_tx=0;                                          //Iloœæ danych do wys³ania przez USB
          //---------------------koniec zmiennych lokalnych--------------------
        

    if (pBuffer != 0) {                                                                   
        if (bStatus != SER_STATUS_SUCCESS) 
        {
          
          TRACE_WARNING("\n\r W: ForwardUSB2Serial: B³¹d transmisji USB\n\r");
        }
        else 
        {

            pBuffer[dBytesTransferred] = 0;
           komunikcja_RSwin((char *)pBuffer,buff_tx,&rozmiar_tx);
           SER_Write(&sSer, buff_tx,rozmiar_tx,0, 0);                                                                        
        }
    }


    do {

        bStatus = SER_Read(&sSer,
                           pUsbBuffer,
    
                           USB_BUFFER_SIZE,
                           (Callback_f) ForwardUSB2Serial,
                           pUsbBuffer);
    }
    while (bStatus != SER_STATUS_SUCCESS);
 
}//_____________________________________________________________________________






//-----------------------------------------------------------------------------
//!  Nazwa funkcji :    Delay
//!  Funkcja opóŸnienia
//-----------------------------------------------------------------------------
void Delay (unsigned long a) 
{ 
    while (--a!=0);   AT91F_WDTRestart(AT91C_BASE_WDTC);
}//_______________________ Koniec funkcji opóŸnienia ____________________________




char *memory_read;

//-----------------------------------------------------------------------------
//! Nazwa funkcji :    InitFramework
//! Funkcja ustawieñ wstêpnych systemu
//-----------------------------------------------------------------------------
int tProgramTabSize=ProgramTabSize;
char *SourRam;
unsigned int  stop=2000;
unsigned int  siz;
void InitFramework (void) 
{ 
   
 //  TRACE_INFO("\n\ InitFramework \n\r");
    


                                  FlashProgramOfset=FlashProgram_A_Ofset;
                       
                                  
                                //  TRACE_INFO("\n\r---  Strona flash = A  \n\r" ); 

    
    
//    TRACE_INFO(" ---  Przepisuje program z FLASH do RAM     -Ofset : %d\n\r",AT91C_IFLASH_MEM->FlashProgram+FlashProgramReserveOfset);
    //przepisz program z flash do ram
    char *memory_read_prog;
    memory_read_prog=(char*)(AT91C_IFLASH_MEM->FlashProgram+FlashProgramOfset);

  
     SourRam= (char*)&gProg;

   
    siz=sizeof(gProg);
    for (unsigned int k = 0 ; k <siz; k++) 
    {      
          SourRam[k]=memory_read_prog[k]; 
    }
    
   
  
    for (char i=0; i<ScreenNr; i++) 
   {
     //je¿eli ustawienie automatyczne temperatury
     if (gProg.Screen[i].Thermo.AutoOn) 
     {
       
         //je¿eli dzieñ typu pierwszego
         if (((gProg.Screen[i].Thermo.Week>>DateTime.DayOfWeek) & 0x1)==1)       
         {
            if (((gProg.Screen[i].Thermo.Chart[0]>>DateTime.Hour) & 0x1)==1)
            {                 
              //ustaw wy¿sz¹ temperaturê
              if (gProg.Screen[i].Thermo.Temperature[0]>gProg.Screen[i].Thermo.Temperature[1])
              {
                gProg.Screen[i].Thermo.SetTemperature=gProg.Screen[i].Thermo.Temperature[0]; 
              }else{
                gProg.Screen[i].Thermo.SetTemperature=gProg.Screen[i].Thermo.Temperature[1]; 
              }
            }else{
              //ustaw ni¿sz¹ temperaturê
              if (gProg.Screen[i].Thermo.Temperature[0]>gProg.Screen[i].Thermo.Temperature[1])
              {
                gProg.Screen[i].Thermo.SetTemperature=gProg.Screen[i].Thermo.Temperature[1]; 
              }else{
                gProg.Screen[i].Thermo.SetTemperature=gProg.Screen[i].Thermo.Temperature[0]; 
              }
            }
         }
     }
   }
 
    
 
    //koniec
   
   
   
   
   //czyszczenie zmiennych programowych
      for (int LineNr=0; LineNr<MaxPrcLines; LineNr++)
      {
        for (int PrcInLine=0; PrcInLine<MaxPrcInLine; PrcInLine++)
        {
          gProg.Line[LineNr].Proc[PrcInLine].LastIn=0;
          gProg.Line[LineNr].Proc[PrcInLine].Out=0;
        }
      }
 

      //Ustawienie wartoœci analogowych wêz³ów na niewiarygodne (0x8AAA)
      for (char NN=1; NN<NumberNeurons; NN++)
      {
        for (char NR=0; NR<NumberReg; NR++)
        {
          MainStage.StageI[NN].Value[NR]=0x8AAA;
        }
      }
    
    
       //przepisanie wartoœci konfiguracyjnych
    WypMaxJasnosci=pAT91C_IFLASH_MEM->Config.ScreenBrightActive;
    WypMinJasnosci=pAT91C_IFLASH_MEM->Config.ScreenBrightInactive;
    if ((WypMaxJasnosci<10) || (WypMaxJasnosci>100)) WypMaxJasnosci=100;
    if (WypMinJasnosci==0) WypMinJasnosci=14;
     
}//_______________________ Koniec funkcji InitFramework ____________________________






//char gLcdLightDuty=2;
void Timer_10us(void)                        //timer isr 
{ 
    unsigned int dummy0 = AT91C_BASE_TC1->TC_SR;      
  
 /*
  
  //PWM podœwietlenia
  static char LcdLightCounterPWM=6;        //licznik podœwietlenia
  
  
AT91F_PIO_SetOutput(LCD_PIO,(unsigned int)LightLCD); 
  
  /*
  static long long ScreenSaverCounter=0;
  if (g_LightLCDEnable)
  {
    AT91F_PIO_SetOutput(LCD_PIO,(unsigned int)LightLCD);    
    ScreenSaverCounter=0;
  }else{
  
    if (LcdLightCounterPWM<6)
  {
    LcdLightCounterPWM++;
   
  }else{
    LcdLightCounterPWM=0; 
    
    if ((ScreenSaverCounter<500000) && (g_ScreenSever_On) ) 
    {
      ScreenSaverCounter++;  
      AT91F_PIO_SetOutput(LCD_PIO,(unsigned int)LightLCD);
    }
    if ((!g_ScreenSever_On) || (g_ScreenSever_On && !((g_ScreenSever_Time_On<=DateTime.Hour) || (g_ScreenSever_Time_Off>DateTime.Hour))))   AT91F_PIO_SetOutput(LCD_PIO,(unsigned int)LightLCD);
  }
  if  (LcdLightCounterPWM==gLcdLightDuty) AT91F_PIO_ClearOutput(LCD_PIO,(unsigned int)LightLCD);
  }
  */
  

  //----koniec PWM podœwietlenia ------
  

  
}//_____________________________________________________________________________








//-----------------------------------------------------------------------------
//!  Nazwa funkcji :    Timer_1_Setup
//!  Funkcja inicjalizacji Timer1 - 1ms
//-----------------------------------------------------------------------------
//! 
//-----------------------------------------------------------------------------
void Timer_10us_Setup(void) 
{ 

  
 //AT91C_BASE_PIOA->PIO_PDR = P_TIOA0;   //disable PIOA 
 //AT91C_BASE_PIOA->PIO_BSR = P_TIOA0;   //select peripheral B 
 AT91C_BASE_AIC->AIC_SVR[AT91C_ID_TC1] = (unsigned int)Timer_10us;  //set AIC             
 AT91C_BASE_AIC->AIC_SMR[AT91C_ID_TC1] = (AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL | TIMER_10US_PRIOR); 
 AT91C_BASE_PMC->PMC_PCER = (1<<AT91C_ID_TC1);      //enable clock 
 AT91C_BASE_TCB->TCB_BCR = 0;                  //no SYNC trigger 
 //no external clocks 
 AT91C_BASE_TCB->TCB_BMR = AT91C_TCB_TC0XC0S_NONE | AT91C_TCB_TC1XC1S_NONE | AT91C_TCB_TC2XC2S_NONE; 
 AT91C_BASE_TC1->TC_CCR = AT91C_TC_SWTRG | AT91C_TC_CLKEN;                   //enable and start clock 
 AT91C_BASE_TC1->TC_CMR = AT91C_TC_ACPC_TOGGLE | AT91C_TC_WAVE  | AT91C_TC_WAVESEL_UP_AUTO  | AT91C_TC_CLKS_TIMER_DIV2_CLOCK;//CPC 
 //clock: MCK/1024  = 48054841/ DIV
 //AT91C_BASE_TC0->TC_RC = 46928;    => 1 sec 
 //! DIV5 - 1024
 //! DIV4 - 128
 //! DIV3 - 32
 //! DIV2 - 8
 
 //AT91C_BASE_TC3->TC_RC = 5090;                  //1ms  (z wyliczeñ 6007)
 //AT91C_BASE_TC0->TC_RC = 5286;                    //880us 
 AT91C_BASE_TC1->TC_RC = 200;                    //440us  
 AT91C_BASE_TC1->TC_IER = AT91C_TC_CPCS;          //enable RC compare int 
 AT91C_BASE_TC1->TC_IDR = ~AT91C_TC_CPCS;         //disable other 
 AT91C_BASE_AIC->AIC_IECR = (1<<AT91C_ID_TC1);       //enable int 
 AT91C_BASE_AIC->AIC_ICCR = (1<<AT91C_ID_TC1);       //clear int 
 
 
 //AT91C_BASE_TC1->TC_CMR |= 1<<6;
}//_____________________________________________________________________________




void TimetableExecute(void)
{

  
  volatile static __Timetable *Timetable=AT91C_IFLASH_MEM->Config.Timetable;
  
  static  char Out[ScreenNr][TimetableTasks];
  char Hour;
  for (char sc=0; sc<ScreenNr; sc++)
  {
     for (char tn=0; tn<4; tn++)
     {
         

        
         if (Timetable[sc].Task[tn].Enable==1) 
         {    
           
                   if (gProg.Screen[sc].Timetable.Min1h)
                   {
                     if(Timetable[sc].Task[tn].Hour>0) {
                       Hour=Timetable[sc].Task[tn].Hour-1;
                     }else{
                       Hour=23;
                     }
                   }else{
                     Hour=Timetable[sc].Task[tn].Hour;
                   }
        
           
             if ((Hour==DateTime.Hour)&& (Timetable[sc].Task[tn].Minute==DateTime.Minute) && ((Timetable[sc].Task[tn].Week) & (1<<DateTime.DayOfWeek-1))  && (DateTime.Second==0))
             {
               if (gProg.Screen[sc].Timetable.ActionType==2)
               {
                 if (Out[sc][tn])
                 {
                   if (gProg.Screen[sc].Timetable.ActorNode<0)  //je¿eli zmienna
                   {
                      Variables[gProg.Screen[sc].Timetable.ActorPort/8]&= ~(1<<(gProg.Screen[sc].Timetable.ActorPort%8));
                   }else{  //je¿eli wêze³
                      PortStageSet(&(MainStage.StageQ[gProg.Screen[sc].Timetable.ActorNode].Port), gProg.Screen[sc].Timetable.ActorPort, 0);  
                      MainStage.SendStageChange[gProg.Screen[sc].Timetable.ActorNode]=ChangeStageFlag;
                   }
                   Out[sc][tn]=0;
                 }
               }else{
                 
                 if (Out[sc][tn]==0) {
                   if (gProg.Screen[sc].Timetable.ActorNode<0)  //je¿eli zmienna
                   {
                      Variables[gProg.Screen[sc].Timetable.ActorPort/8] |= 1<<(gProg.Screen[sc].Timetable.ActorPort%8);
                   }else{  //je¿eli wêze³
                    PortStageSet(&(MainStage.StageQ[gProg.Screen[sc].Timetable.ActorNode].Port), gProg.Screen[sc].Timetable.ActorPort, 1);  
                    MainStage.SendStageChange[gProg.Screen[sc].Timetable.ActorNode]=ChangeStageFlag;}
                   }
                 Out[sc][tn]=1;
                 
               }
             }else{
               if ((gProg.Screen[sc].Timetable.ActionType==0) && (Out[sc][tn]==1)) 
               {
                 if (gProg.Screen[sc].Timetable.ActorNode<0)  //je¿eli zmienna
                   {
                     Variables[gProg.Screen[sc].Timetable.ActorPort/8]&= ~(1<<(gProg.Screen[sc].Timetable.ActorPort%8));
                   }else{  //je¿eli wêze³
                    PortStageSet(&(MainStage.StageQ[gProg.Screen[sc].Timetable.ActorNode].Port), gProg.Screen[sc].Timetable.ActorPort, 0);
                    MainStage.SendStageChange[gProg.Screen[sc].Timetable.ActorNode]=ChangeStageFlag;
                   }
               }
               Out[sc][tn]=0;
               
             }
         
         }
         
         
     }
     
  }
}



void ThermostatsTimeChartExecute(void)
{
  
  signed   short H_Tmp;
  signed   short L_Tmp;
  signed   short New_Temp;
  static  signed short LastTemp[ScreenNr];
    
  for (signed int sc=0; sc<ScreenNr; )
  {
    if (((sc==0) && (gProg.Screen[sc].PaternDesign==MainScreenTermo)) || 
        ((sc) && (gProg.Screen[sc].PaternDesign==SubScreenTermo)))
    {
           //je¿eli ustawienie automatyczne temperatury
           if (gProg.Screen[sc].Thermo.AutoOn) 
           {                       
                if (gProg.Screen[sc].Thermo.Temperature[0]>gProg.Screen[sc].Thermo.Temperature[1])
                    {
                      H_Tmp=gProg.Screen[sc].Thermo.Temperature[0]; 
                      L_Tmp=gProg.Screen[sc].Thermo.Temperature[1]; 
                    }else{
                      H_Tmp=gProg.Screen[sc].Thermo.Temperature[1];
                      L_Tmp=gProg.Screen[sc].Thermo.Temperature[0]; 
                    }
                
               //je¿eli dzieñ wykresu pierwszego
               if (((gProg.Screen[sc].Thermo.Week>>(DateTime.DayOfWeek-1)) & 0x1)==1)       
               {

                 
                    if (((gProg.Screen[sc].Thermo.Chart[0]>>DateTime.Hour) & 0x1)==1)
                    {                 
                      //ustaw wy¿sz¹ temperaturê
                        New_Temp=H_Tmp; 
                    }else{
                      //ustaw ni¿sz¹ temperaturê
                        New_Temp=L_Tmp;
                    }
               }else{

                    if (((gProg.Screen[sc].Thermo.Chart[1]>>DateTime.Hour) & 0x1)==1)
                    {                 
                      //ustaw wy¿sz¹ temperaturê
                        New_Temp=H_Tmp; 
                    }else{
                      //ustaw ni¿sz¹ temperaturê
                        New_Temp=L_Tmp;
                    }
               }
               if (LastTemp[sc]!=New_Temp)  
                  {
                      gProg.Screen[sc].Thermo.SetTemperature=New_Temp;
                      LastTemp[sc]=gProg.Screen[sc].Thermo.SetTemperature;
                  }
           }else{
              LastTemp[sc]=-9999;
           }
     }
    sc++;
  }
}


 // int MainScreenTermo=ScreenSetTab_0_Nr;
 // int SubScreenTermo=ScreenSetTab_1_Nr;
  //int MainScreenTimetable=ScreenSetTab_0_Nr+1;
  //int SubScreenTimetable=ScreenSetTab_1_Nr+1;
void ThermostatsExecute(void)
{
  short  SensorNodeValue;
 
  static char Hysteresis[ScreenNr];
  
  for (char sc=0; sc<ScreenNr; sc++)
  {
    if (((sc==0) && (gProg.Screen[sc].PaternDesign==MainScreenTermo)) || 
        ((sc) && (gProg.Screen[sc].PaternDesign==SubScreenTermo)))
    {
           SensorNodeValue=0;
           if (gProg.Screen[sc].Thermo.SensorNode<0) //je¿eli zmienna
           {
             SensorNodeValue=VariablesAnal[gProg.Screen[sc].Thermo.SensorReg];
           }else{
             SensorNodeValue=MainStage.StageI[gProg.Screen[sc].Thermo.SensorNode].Value[gProg.Screen[sc].Thermo.SensorReg];
           }
      
           //zeruje stan wyjœcia je¿eli temperatura wy¿sza od za³o¿onej 
           if (SensorNodeValue>=gProg.Screen[sc].Thermo.SetTemperature)
           {
             if (gProg.Screen[sc].Thermo.ActorNode<0)  //je¿eli zmienna
             {
                Variables[gProg.Screen[sc].Thermo.ActorPort/8] &= ~(1<<(gProg.Screen[sc].Thermo.ActorPort%8));
             }else{
                PortStageSet(&(MainStage.StageQ[gProg.Screen[sc].Thermo.ActorNode].Port), gProg.Screen[sc].Thermo.ActorPort, 0);  
                MainStage.SendStageChange[gProg.Screen[sc].Thermo.ActorNode]=ChangeStageFlag;
             }
             Hysteresis[sc]=ThermostatHysteresis; //ustawia histerezê
           }
           
           //ustawia na 1 stan wyjœcia je¿eli temperatura ni¿sza od za³o¿onej - histereza
           if (SensorNodeValue>0)
           if (SensorNodeValue<=gProg.Screen[sc].Thermo.SetTemperature-Hysteresis[sc])
           {
             Hysteresis[sc]=0;
             if (gProg.Screen[sc].Thermo.ActorNode<0)  //je¿eli zmienna
             {
                Variables[gProg.Screen[sc].Thermo.ActorPort/8] |= 1<<(gProg.Screen[sc].Thermo.ActorPort%8);
             }else{
                PortStageSet(&(MainStage.StageQ[gProg.Screen[sc].Thermo.ActorNode].Port), gProg.Screen[sc].Thermo.ActorPort, 1);   
                MainStage.SendStageChange[gProg.Screen[sc].Thermo.ActorNode]=ChangeStageFlag;
             }
           }
           
           
    }
  }
}



char out[10]={1,1,1,1,1,1,1,1,1,1};
void AlarmExecute(void)
{

static long long AlarmOnTime[ScreenNr];  //czas do odliczania dla alarmu
static long long AlarmOffTime[ScreenNr];
  
  for (char sc=0; sc<ScreenNr; sc++)
  {
    if ((((sc==0) && (gProg.Screen[sc].PaternDesign==MainScreenAlarm)) || 
        ((sc) && (gProg.Screen[sc].PaternDesign==SubScreenAlarm))))
    {
      if (AlarmScreenAction[sc].Active) 
      {
                 //zeruje stan wyjœcia je¿eli zda¿enie od czujnika             
            if (PortStageRead(MainStage.StageI[gProg.Screen[sc].Alarm.SensorNode].Port, gProg.Screen[sc].Alarm.SensorPort)==0x1)  
                 {
                   if (AlarmOnTime[sc]==0) {AlarmOnTime[sc]=g_counter;}
                   if ((g_counter>AlarmOnTime[sc]+AlarmTimeToActive*1000) && (out[sc]==0))
                   {
                      PortStageSet(&(MainStage.StageQ[gProg.Screen[sc].Alarm.ActorNode].Port), gProg.Screen[sc].Alarm.ActorPort, 0);
                      MainStage.SendStageChange[gProg.Screen[sc].Alarm.ActorNode]=ChangeStageFlag;
                      AlarmOn=1; 
                      out[sc]=1;
                      AlarmOffTime[sc]=g_counter;
                   }
                   if ((AlarmOffTime>0) && (g_counter>AlarmOffTime[sc]+AlarmSirenTime*1000) && (out[sc]==1))
                   {
                      PortStageSet(&(MainStage.StageQ[gProg.Screen[sc].Alarm.ActorNode].Port), gProg.Screen[sc].Alarm.ActorPort, 1); 
                      MainStage.SendStageChange[gProg.Screen[sc].Alarm.ActorNode]=ChangeStageFlag;
                      AlarmOn=0; 
                      out[sc]=1;
                      AlarmOffTime[sc]=0;
 
                   }
                   
                  
                   
                 }
      }else{
        if (out[sc]==1)
        {
            out[sc]=0;
            AlarmOnTime[sc]=0;
            AlarmOn=0; 
            PortStageSet(&(MainStage.StageQ[gProg.Screen[sc].Alarm.ActorNode].Port), gProg.Screen[sc].Alarm.ActorPort, 1);
            MainStage.SendStageChange[gProg.Screen[sc].Alarm.ActorNode]=ChangeStageFlag;
        }
      }
           
    }
  }
}







//-----------------------------------------------------------------------------
//! Nazwa funkcji :    Timer_1h  
//! Okres: 1 godzina
//-----------------------------------------------------------------------------
//  
//----------------------------------------------------------------------------- 
void Timer_1h(void)                  
{ 
    ThermostatsTimeChartExecute();
}//_____________________________________________________________________________



//-----------------------------------------------------------------------------
//! Nazwa funkcji :    Timer_1min  
//! Okres: 1min
//-----------------------------------------------------------------------------
//  
//----------------------------------------------------------------------------- 
void Timer_1min(void)                  
{ 
   TimetableExecute();
   ThermostatsExecute();
   
}//_____________________________________________________________________________






//-----------------------------------------------------------------------------
//! Nazwa funkcji :    Timer_1s  
//! Okres: 1s
//! Funkcja obs³ugi przerwania USART 0
//-----------------------------------------------------------------------------
//  
//----------------------------------------------------------------------------- 
int XY_Tab[11];
int LiczOds = 0;
#define x_offset 1
#define y_offset 6
char display_cykl =0;
int X_up_old =-1;
int Y_up_old =-1;
int X_down_old =-1;
int Y_down_old =-1;
char g_PC_Connected_timeout=5;


unsigned int CyklPerSekCount=0; 
unsigned int TransLoopTime=0;
unsigned int CyklPerSek=1;
int MenuLevel_old; //tmp
 char FlagChangeTime=0;



void Timer_1s(void)                        //timer isr 
{ 
 static char TryIndirect, TryIndirectSlow;   //zmienna s³u¿y do usuwania trybu indirect w przypadku braku nawi¹zania po³¹czenia w okreœlonym czasie
   
  
  if ((AlarmOn) || (Speaker[1]==1)) 
  {
      ButtonBipCounter=g_counter+500;
      AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; 
  }

   g_seconds_counter++;

  
  if (CyklPerSekCount>0)
  {
   TransLoopTime=(1000/CyklPerSekCount);     //czas obiegu programu w ms
   CyklPerSek=CyklPerSekCount;               //iloœæ cykli na sekunde
  }else{
   TransLoopTime=0;
  }
  CyklPerSekCount=0;

     
  
  int ChangeStageFlagTMP=CHANGE_STAGE_TIME*CyklPerSek;
  if (ChangeStageFlagTMP>0xFF) ChangeStageFlagTMP=0xFF;
  if (ChangeStageFlagTMP<10) ChangeStageFlagTMP=10;
   ChangeStageFlag=ChangeStageFlagTMP;
  
   //Wy³¹czanie podœwietlenia wyœwietlacza
  
  if (g_LightCouterLCD==0) 
  {
    if (g_LightLCDEnable==0) g_LightLCDEnable=1;
  }
  if ((g_LightCouterLCD < LightLCD_Time) && (g_PC_Connected==0))
  {
      g_LightCouterLCD++;
        
  } 
  
  
  if ((g_LightCouterLCD >= LightLCD_Time) && (g_PC_Connected==0))

  {
   g_LightLCDEnable=0; 
  }

  
  if ((DateTime.DayOfWeek)==0) DateTime.DayOfWeek=Monday;
  
  if (DateTime.Second<59) {DateTime.Second++;} else {DateTime.Second=0; DateTime.Minute++;};
  if (DateTime.Minute>59) {
      DateTime.Minute=0; 
      DateTime.Hour++;
         //zmiana czasu z zimowego na letni
       if ((DateTime.Mounth==3) &&  (DateTime.Day>=25) && (DateTime.DayOfWeek==Sunday) && (DateTime.Hour==2) && (DateTime.Second==0))
       {
         DateTime.Hour=3;
         SetRTC_DateTime();  //Ustawinie: sekundy, minuty,  godziny, dzieñ tyg, dnia, miesi¹ca, roku

       }
       //zmiana czasu z letniego na zimowy
       if ((DateTime.Mounth==10) &&  (DateTime.Day>=25) && (DateTime.DayOfWeek==Sunday) && (DateTime.Hour==3) && (DateTime.Second==0))
       {
         if (FlagChangeTime==0)
         {
            FlagChangeTime=1; 
            DateTime.Hour=2;
            SetRTC_DateTime();  //Ustawinie: sekundy, minuty,  godziny, dzieñ tyg, dnia, miesi¹ca, roku
         }else{
            FlagChangeTime=0; 
         }
       }
  }
  
  if (DateTime.Hour>23)   {
    DateTime.Hour=0;     
    DateTime.DayOfWeek++; DateTime.Day++;

  }
  
  
  switch (DateTime.Mounth)
        {
          case 1  : if (DateTime.Day>31) {DateTime.Day=1; DateTime.Mounth++;} break;
          case 2  : if ((DateTime.Year%4 == 0) && (DateTime.Year%100 != 0) || (DateTime.Year%400 == 0))  //je¿eli rok przestêpny
                        {if (DateTime.Day>29) {DateTime.Day=1; DateTime.Mounth++;}     
                        }else{if (DateTime.Day>28) {DateTime.Day=1; DateTime.Mounth++;}
                        }
          break;
          case 3  : if (DateTime.Day>31) {DateTime.Day=1; DateTime.Mounth++;} break;
          case 4  : if (DateTime.Day>30) {DateTime.Day=1; DateTime.Mounth++;} break;
          case 5  : if (DateTime.Day>31) {DateTime.Day=1; DateTime.Mounth++;} break;
          case 6  : if (DateTime.Day>30) {DateTime.Day=1; DateTime.Mounth++;} break;
          case 7  : if (DateTime.Day>31) {DateTime.Day=1; DateTime.Mounth++;} break;
          case 8  : if (DateTime.Day>31) {DateTime.Day=1; DateTime.Mounth++;} break;
          case 9  : if (DateTime.Day>30) {DateTime.Day=1; DateTime.Mounth++;} break;
          case 10 : if (DateTime.Day>31) {DateTime.Day=1; DateTime.Mounth++;} break;
          case 11 : if (DateTime.Day>30) {DateTime.Day=1; DateTime.Mounth++;} break;
          case 12 : if (DateTime.Day>31) 
          {
            DateTime.Day=1; DateTime.Mounth=1; DateTime.Year++;
          } break;
          
  
  
  }  
  if (DateTime.DayOfWeek>Sunday) DateTime.DayOfWeek=Monday;
  

  
  
  

    
  if (MenuLevel_old!=MenuLevel)
  {
    MenuLevel_old=MenuLevel;    //na koniec usun¹c MenuLevel_old
  }
  
   char czas_tmp[9];
  
  TimeToStr(DateTime.Hour, DateTime.Minute, DateTime.Second, czas_tmp);
  czas_tmp[8]=13;

  
  //zeruj wskaŸniki poprawnoœci po³aczeñ
  if (g_US0_Connect_OK) g_US0_Connect_OK--;
  if (g_US1_Connect_OK) g_US1_Connect_OK--;
  

  //synchronizacja RTC
if ((DateTime.Hour==2) && (DateTime.Minute==57) && (DateTime.Second==0)) GetDateTimeFromRTC(); 
//if (DateTime.Second=30) GetDateTimeFromRTC(); 

  //przesuniêcie bitowe dle pomiaru jakoœci po³¹czenia z Node`ami z okresowoœci¹ 1min
  static char CountMin=0;
  if (CountMin<60)
  {
    CountMin++; 
  }else{
    CountMin=0;
  
    if (TransQuality.SlowMensurIndex<7)
    {
      TransQuality.SlowMensurIndex++; 
    }else{
      TransQuality.SlowMensurIndex=0;
    }
    
    for(char NN=1; NN<NumberNeurons; NN++)   TransQuality.SlowMensur[NN] &= ~(1<<TransQuality.SlowMensurIndex);
    
    char TryIndirectTmp, TryIndirectSlowTmp;
    
    TryIndirectTmp=TryIndirect;
    TryIndirectSlowTmp=TryIndirectSlow;
      
          //Sprawdzenie wiarygodnoœci wêz³ów na podstawie d³ugotrwa³ego pomiaru jakoœci
      for (char NN=1; NN<NumberNeurons; NN++)
      {
        if (TransQuality.SlowMensur[NN-1]==0)
        {
            for (char NR=0; NR<NumberReg; NR++)
            {
              MainStage.StageI[NN].Value[NR]=0x8AAA;
              
                  if (ChangeingIdMode==0)
                  {
                      if (NeuronsList[NN-1].NeuronType==NodeRF)   //je¿eli wêze³ ze sta³¹ komunikacj¹
                      {
    
                            if (TryIndirect==0) //je¿eli rozkaz rozg³oœny nie dodany
                            {
                              if ((!NeuronsList[NN-1].ReadSuspend) && (!AT91C_IFLASH_MEM->Config.SlaveMode))      AddRfTask_SearchNodeTask(NN);                //Dodaj rozkaz do tablicy zadañ RF - kto ma dostêp do wêz³a. Rozkaz dodawany jest po perwszej minucie pracy sterownika. Je¿eli wêze³ zerwie po³¹czenie to dodawany jest po 8 minutach
                              TryIndirectTmp=1;
                            }else if (TryIndirect==1){ //je¿eli rozkaz rozg³oœny dodany ale nikt nie znalaz³ wêz³a
                              AddRfTask_RemoveSearchNodeTask(NN); 
                              TryIndirectTmp=2;
                            }
                          
                      
                      }
                      else if (NeuronsList[NN-1].NeuronType==NodeRF_Period) //je¿eli wêze³ z komunikacja d³ugo okresow¹ 
                      {
                          if (g_counter>360000)
                          {
                            if (TryIndirectSlow==0)  //je¿eli rozkaz rozg³oœny nie dodany
                            {
                              if ((TransQuality.SlowMensurIndex==0) && (!NeuronsList[NN-1].ReadSuspend) && (!AT91C_IFLASH_MEM->Config.SlaveMode))      AddRfTask_SearchNodeTask(NN);                //Dodaj rozkaz do tablicy zadañ RF - kto ma dostêp do wêz³a.  Rozkaz dodawany jest w ósmej minucie pracy sterownika. Je¿eli wêze³ zerwie po³¹czenie to dodawany jest po 8 minutach
                              TryIndirectSlowTmp=1;
                            }else if ((TryIndirectSlow==1) && (g_counter>720000)) { //je¿eli rozkaz rozg³oœny dodany ale nikt nie znalaz³ wêz³a w dodatkowym czasie
                              AddRfTask_RemoveSearchNodeTask(NN); 
                              TryIndirectSlowTmp=2;
                            }
                          }
                      }
                  }
     
            }
        }
      }
      
  TryIndirect=TryIndirectTmp;
  TryIndirectSlow=TryIndirectSlowTmp;
      
      
  }

                     

  if (DateTime.Second==0) Timer_1min();
  if ((DateTime.Second==0) && (DateTime.Minute==0))  Timer_1h();
  if (DateTime.Second==0) TimetableExecute();
     AlarmExecute();
     
     

   //sprawdzenie wy³adowania baterii i wyœwietlenie ostrzenia       
    if ((DateTime.Hour==10) && (DateTime.Minute==0) && (DateTime.Second==0)   && (DateTime.DayOfWeek==Saturday))
    {
   
       for (char i=0; i<NumberNeurons; i++)
       {
         if ((MainStage.StageI[i].Port& 0x8000) && (((unsigned short)(MainStage.StageI[i].Port))!=0x8AAA))
         {
            char txt[NodeLabelLethers+1];
            for (char k=0; k<NodeLabelLethers; k++) txt[k]=gProg.NodeLabel[i-1][k];
            txt[NodeLabelLethers]=0;

            
            Message_Paint(2,"Wyladowana","bateria","   ","Wezel:", txt);         

            ButtonBipCounter=g_counter+8000;
            AT91C_BASE_PIOA->PIO_SODR =SpeakerPin;  
         }

       } 
      
   }    
    

   
   
   //wygaszacz ekranu
   static char LCD_last_fill=100;
   char WypMin;
   if ((!g_ScreenSever_On) || (g_ScreenSever_On && !((g_ScreenSever_Time_On<=DateTime.Hour) || (g_ScreenSever_Time_Off>DateTime.Hour))))  
   {  
      WypMin=WypMinJasnosci;   //wype³nienie bez wygaszacza
   }else{
      WypMin=4;    //wype³nienie przy wygaszaczu
   }
     if (gWypMinJasnosciTest)
     {
        AT91C_BASE_PWMC->PWMC_DIS = 0x1;
        Set_LCD_Light_PWM_Fill(WypMinJasnosci); 
        AT91C_BASE_PWMC->PWMC_ENA = 0x1;
     }
     else if (g_LightLCDEnable==1)
     {
       if (LCD_last_fill!=WypMaxJasnosci)
       {
          AT91C_BASE_PWMC->PWMC_DIS = 0x1;
          Set_LCD_Light_PWM_Fill(WypMaxJasnosci);  //maksymalne wype³nienie
          AT91C_BASE_PWMC->PWMC_ENA = 0x1;
          LCD_last_fill=WypMaxJasnosci;
       }
     }else {
       if (LCD_last_fill>WypMin)
       {
          if (LCD_last_fill>60) LCD_last_fill=61;
          LCD_last_fill-=2;
          AT91C_BASE_PWMC->PWMC_DIS = 0x1;
          Set_LCD_Light_PWM_Fill(LCD_last_fill); 
          AT91C_BASE_PWMC->PWMC_ENA = 0x1;
       }
    }
    
    
    if ((RF_Status.RfFuse>0) && (RF_Status.RfFuse<100)) RF_Status.RfFuse++;
   
   
   
   //bezpiecznik przed zablokowaniem siê DMA USARTA lub WiFi 
   if (NET.WiFi.On)
   {
     (FuseLastWiFiDataIn<g_counter-30000);   //zabezpieczenie przed blokad¹ DMA USART. FuseLastWiFiDataIn zawiera czas ostatniej odpowiedzi od usarta. Je¿eli WiFi jest w³¹czone a USART milczy od 30s to restartuj mikroprocesor
   }else{
      FuseLastWiFiDataIn=g_counter;
   }
   
   
   //wyczyszczenie po³¹czeñ mostowych z którymi nie mam kontaktu po up³yniêciu czasu MAX_TIME_TO_REM_BRIDGE_NO_CON
    if (RF_Status.TimeToRemBridgeNoCom) 
    {
      if (RF_Status.TimeToRemBridgeNoCom==1)
      {
                for (char ND=0; ND<NumberNeurons-1; ND++)
                    {
                      if ((NeuronsList[ND].Bridge) &&  (TransQuality.QuickMensur[ND]==0))
                      {
                         NeuronsList[ND].NeuronType=0;
                         NeuronsList[ND].NeuronID=0;
                         NeuronsList[ND].Bridge=0;
                      }
                    }
      }
      RF_Status.TimeToRemBridgeNoCom--;
    }

}




//-----------------------------------------------------------------------------
//! Nazwa funkcji :    Timer_10ms
//! Okres: 10ms
//! Timer zale¿ny od Timer_1ms
//-----------------------------------------------------------------------------
//  
//----------------------------------------------------------------------------- 
void Timer_10ms(void)                      
{ 
  unsigned int dummy = AT91C_BASE_TC0->TC_SR;                                   // Odczyt statusu  

 X_up=-1;
 Y_up=-1;

 X_down=-1;
 Y_down=-1;


  GetTouchPoint(&X_down, &Y_down, &X_up, &Y_up);

 
   //Akcja przycisku up
  if ((X_up>0)&&(Y_up>0)) 
  {
     ScreenEvent();
     g_LightCouterLCD = 0;     //W³¹cza podœwietlenie wyœwietlacza LCD
     if (g_LightLCDEnable==0) g_LightLCDEnable=1;
     AT91C_BASE_PIOA->PIO_CODR =SpeakerPin; 
     ButtonBipCounter=0;
  }
  
  if (LiczOds>=5) {
  
            //Akcja przycisku down
      if ((X_down>0)&&(Y_down>0) /*&& (g_LightLCDEnable==0)*/) 
      {
         ScreenEvent();
         g_LightCouterLCD = 0;     //W³¹cza podœwietlenie wyœwietlacza LCD
         g_LightLCDEnable=1;
      }  
      
      MenuRefresh(); 

    

    
    RepaintLCD();
    RepaintLCD();

    
    LiczOds=0;

  }
  
  else 
  {
    LiczOds++; 
  }
  
    
  
  static int Spek2_Counter;
  if (Spek2_Counter<202)Spek2_Counter++;
  if ((Speaker[2]==1) && (Spek2_Counter>150)) 
  {
      Spek2_Counter=0;
      ButtonBipCounter=g_counter+150;
      AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; 
  }

  if ((Speaker[3]==1) && (Spek2_Counter==150)) 
  {
      ButtonBipCounter=g_counter+100;
      AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; 
  }  
  if ((Speaker[3]==1) && (Spek2_Counter>200)) 
  {
      Spek2_Counter=0;
      ButtonBipCounter=g_counter+100;
      AT91C_BASE_PIOA->PIO_SODR =SpeakerPin; 
  }    

 AT91F_WDTRestart(AT91C_BASE_WDTC);

}




void Timer_10ms_Setup(void) 
{ 


 AT91C_BASE_AIC->AIC_SVR[AT91C_ID_TC0] = (unsigned int)Timer_10ms;  //set AIC             
 AT91C_BASE_AIC->AIC_SMR[AT91C_ID_TC0] = (AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL | TIMER_10mS_PRIOR ); 
 AT91C_BASE_PMC->PMC_PCER = (1<<AT91C_ID_TC0);      //enable clock 
 AT91C_BASE_TCB->TCB_BCR = 0;                  //no SYNC trigger 
 //no external clocks 
 AT91C_BASE_TCB->TCB_BMR = AT91C_TCB_TC0XC0S_NONE | AT91C_TCB_TC1XC1S_NONE | AT91C_TCB_TC2XC2S_NONE; 
 AT91C_BASE_TC0->TC_CCR = AT91C_TC_SWTRG | AT91C_TC_CLKEN;                   //enable and start clock 
 AT91C_BASE_TC0->TC_CMR = AT91C_TC_ACPC_TOGGLE | AT91C_TC_WAVE  | AT91C_TC_WAVESEL_UP_AUTO  | AT91C_TC_CLKS_TIMER_DIV5_CLOCK;//CPC 
  //! DIV5 - 1024
 //! DIV4 - 128
 //! DIV3 - 32
 //! DIV2 - 8
 //clock: MCK/1024  = 48054841/1024  =  46928 Hz 
 //AT91C_BASE_TC0->TC_RC = 46928,55;    => 1 sec 
                         
 AT91C_BASE_TC0->TC_RC = 469;                  //10ms 
 AT91C_BASE_TC0->TC_IER = AT91C_TC_CPCS;         //enable RC compare int 
 AT91C_BASE_TC0->TC_IDR = ~AT91C_TC_CPCS;         //disable other 
 AT91C_BASE_AIC->AIC_IECR = (1<<AT91C_ID_TC0);       //enable int 
 AT91C_BASE_AIC->AIC_ICCR = (1<<AT91C_ID_TC0);       //clear int 
  

}//_____________________________________________________________________________





//-----------------------------------------------------------------------------
//! Nazwa funkcji :    Timer_1ms
//! Okres: 1ms
//! Funkcje:
//!         Funkcja obs³ugi przerwania USART 0  Czas 1 ms
//!         Triger Timer_10ms 
//-----------------------------------------------------------------------------
//  
//----------------------------------------------------------------------------- 


int Counter=0;
int US0_Counter=0;
int US1_Counter=0;
extern char SendModbusEnable;
char CounterForTimer10ms=0;
short int CurTimeQual=0;

void Timer_1ms(void)                        //timer isr 
{ 
  unsigned int dummy2 = AT91C_BASE_TC2->TC_SR;  
  
  g_counter++;

  

  
  
  /*
  


  
  //obs³uga US0
  if (US0_MasterMODBUS_ENABLE)
  {
  if (US0_Counter>g_US0_Comm_MinCykle) 
  {
    
      if (g_Ans_ZD_MODBUS_0 == g_US0_Comm_TIMEOUT)
      {
          ZD_MODBUS_0[g_Current_ZD_MODBUS_0].Error ++;
      }
    
      
      if (g_Ans_ZD_MODBUS_0 >= g_US0_Comm_TIMEOUT)
      {

          //AT91C_BASE_PIOA->PIO_SODR = AT91C_PIO_PA4; AT91C_BASE_PIOA->PIO_CODR = AT91C_PIO_PA4;
   
          g_Ans_ZD_MODBUS_0=0;
          SendModbusEnable=0;
        //  ModBus_MASTER_TX(0);
          
          US0_Counter=0;
      }
      else
      {
          g_Ans_ZD_MODBUS_0++;

      }
    
  }
  else
  {
    US0_Counter++; 
  }
  }
  


  if (US1_MasterMODBUS_ENABLE)
  {
  if (US1_Counter>g_US0_Comm_MinCykle) 
  {
    
      if (g_Ans_ZD_MODBUS_1 == g_US1_Comm_TIMEOUT)
      {
          ZD_MODBUS_1[g_Current_ZD_MODBUS_1].Error ++;
      }
      
      if (g_Ans_ZD_MODBUS_1 >= g_US1_Comm_TIMEOUT)
      {
   
          g_Ans_ZD_MODBUS_1=0;
          ModBus_MASTER_TX(1);
          
          US0_Counter=1;
      }
      else
      {
          g_Ans_ZD_MODBUS_1++;

      }
    
  }
  else
  {
    US1_Counter++; 
  }
  }
*/
  
  

if (ButtonBipCounter<g_counter) AT91C_BASE_PIOA->PIO_CODR =SpeakerPin;

  
  
  static int RF_SlaveCounter;

  
  
//TimeOut RF -----
 
  
if (RF_Status.AllReady)
{
  
  //Prze³¹czanie w tryb Master + zabezpieczenie przed utkniêciem w trybie slave na d³u¿ej ni¿ 400ms;
  if (SlaveMode)                                        
  {
    if (RF_Status.SlaveModeTimeout>0) RF_Status.SlaveModeTimeout--;
    if (RF_Status.TimeInSlaveMode<MAX_TIME_FOR_SLAVE_ASK+100) {RF_Status.TimeInSlaveMode++;}else {RF_Status.SlaveModeTimeout=0;}
    if (RF_Status.SlaveModeTimeout==0) 
    {
      SwitchToMaster();
    }else{
      if ((RF_Status.EnRx==0) && (RF_Status.InitRx==0))
      {
        RF_Status.EnableRX();
      }
    }
  }else{
    if (RF_Status.TimeInMasterMode<0xFF) RF_Status.TimeInMasterMode++;
    
  }
         
  
  static char PraboblySlaveConstChanel=0xFF;   //prawdopodobny kana³ slave (by³a wczeœniej na nim komunikacja) 0xff - brak kana³u
  
  if (RF_Status.MasterMode)
  {
    if ((!SlaveMode) && (g_Stop_Program==0))
    if (g_RF_TimeOutCounter>0)
    {
      g_RF_TimeOutCounter--;

 
    }else{
       g_RF_TimeOutCounter=RF_TimeOut; 
       RF_MasterExecut();
    }
  }else{
        
        RF_SlaveCounter++;
     
        
          if (RF_Status.ConfResiveSlave==2) //je¿eli poprawnie odebrano dane 
          {
            PraboblySlaveConstChanel=RF_Status.ChSlave;
            RF_SlaveCounter=0;   //- zeruj licznik
            RF_Status.ConfResiveSlave=1;
            g_SlaveConnect=1;    //ustawia flagê poprawnej transmisji slave
          }else{
  
            short LongWait=1000;
            if (PraboblySlaveConstChanel==RF_Status.ChSlave) LongWait=10000;
            
            if (((RF_SlaveCounter==100)  && (RF_Status.ConfResiveSlave==0)) //je¿eli nie odebrano danych w czasie 100ms
            || ((RF_SlaveCounter>=LongWait) && (RF_Status.ConfResiveSlave<2))  //je¿eli nie odebrano danych o w³aœciwym ID w czasie 1000ms
              )
            {

                 SPI_SendWord(0x8219,Cs_RF); 
                 RF_SlaveCounter=0;   //- zeruj licznik
                 RF_Status.ConfResiveSlave=0;
                 g_SlaveConnect=0; 
                 
                 if (RF_Status.AllReady) if (RF_Status.ChSlave<RF_ChannelQuanti-1) {RF_Status.ChSlave++;} else {RF_Status.ChSlave=0;}
                 SPI_SendWord(0xA000 | RF_SetFreqReg(RF_Status.ChSlave),Cs_RF);  
                 RF_Status.MasterMode=0;  
                 RF_Status.EnableRX();
              
            }
          
          }
         
          
  }
}          

  

  
      //uruchomienie Timer_1s co 1000 cykli-------------------------------------------
      static short CounterForTimer1s; 
      if (CounterForTimer1s<1000)
      {
          CounterForTimer1s++;
      }else{
          CounterForTimer1s=0;
          Timer_1s();
      }



  
  //przesuniêcie bitowe dle pomiaru jakoœci po³¹czenia z Node`ami z okresowoœci¹ TimeUnitQual
  if (++CurTimeQual>=TimeUnitQual)
  {
    CurTimeQual=0;
    if (TransQuality.MensurIndex<32)
    {
      TransQuality.MensurIndex++; 
    }else{
      TransQuality.MensurIndex=0;
    }
    if (TransQuality.QuickMensurIndex<4)
    {
      TransQuality.QuickMensurIndex++; 
    }else{
      TransQuality.QuickMensurIndex=0;
    }

    for (char i=0; i<NumberNeurons; i++) 
    {
      TransQuality.Mensur[i] &= ~(0x1 << TransQuality.MensurIndex);
      TransQuality.QuickMensur[i] &= ~(0x1 << TransQuality.QuickMensurIndex);
    }                 
  }

  

          
       


}//_____________________________________________________________________________





//-----------------------------------------------------------------------------
//!  Nazwa funkcji :    Timer_1_Setup
//!  Funkcja inicjalizacji Timer1 - 1ms
//-----------------------------------------------------------------------------
//! 
//-----------------------------------------------------------------------------
void Timer_1ms_Setup(void) 
{ 
  //TRACE_INFO("T2_setup");  
  //TRACE_INFO("\n\r"); 
  
 //AT91C_BASE_PIOA->PIO_PDR = P_TIOA0;   //disable PIOA 
 //AT91C_BASE_PIOA->PIO_BSR = P_TIOA0;   //select peripheral B 
 AT91C_BASE_AIC->AIC_SVR[AT91C_ID_TC2] = (unsigned int)Timer_1ms;  //set AIC             
 AT91C_BASE_AIC->AIC_SMR[AT91C_ID_TC2] = (AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL | TIMER_1mS_PRIOR ); 
 AT91C_BASE_PMC->PMC_PCER = (1<<AT91C_ID_TC2);      //enable clock 
 AT91C_BASE_TCB->TCB_BCR = 0;                  //no SYNC trigger 
 //no external clocks 
 AT91C_BASE_TCB->TCB_BMR = AT91C_TCB_TC0XC0S_NONE | AT91C_TCB_TC1XC1S_NONE | AT91C_TCB_TC2XC2S_NONE; 
 AT91C_BASE_TC2->TC_CCR = AT91C_TC_SWTRG | AT91C_TC_CLKEN;                   //enable and start clock 
 AT91C_BASE_TC2->TC_CMR = AT91C_TC_ACPC_TOGGLE | AT91C_TC_WAVE  | AT91C_TC_WAVESEL_UP_AUTO  | AT91C_TC_CLKS_TIMER_DIV2_CLOCK;//CPC 
 //clock: MCK/1024  = 48054841/ DIV
 //AT91C_BASE_TC0->TC_RC = 46928;    => 1 sec 
 //! DIV5 - 1024
 //! DIV4 - 128
 //! DIV3 - 32
 //! DIV2 - 8
 
 AT91C_BASE_TC2->TC_RC = 6007;                  //1ms  (z wyliczeñ 6007)
 AT91C_BASE_TC2->TC_IER = AT91C_TC_CPCS;         //enable RC compare int 
 AT91C_BASE_TC2->TC_IDR = ~AT91C_TC_CPCS;         //disable other 
 AT91C_BASE_AIC->AIC_IECR = (1<<AT91C_ID_TC2);       //enable int 
 AT91C_BASE_AIC->AIC_ICCR = (1<<AT91C_ID_TC2);       //clear int 



}//_____________________________________________________________________________










void RamToFlash(void)
{
          //if (g_PC_Connected==0)
          //{
            DispWait=DispWaitOn; 
            Wait_Paint();
          //} 
            gProg.Preambu=ProgPreambu;
            gProg.ProgFormatTyp=_ProgFormatTyp;
            gProg.Size=sizeof(gProg);
            gProg.ModyfiData[0]='2';gProg.ModyfiData[1]='0';
            CharToStr(DateTime.Year, &(gProg.ModyfiData[2]), 2); 
            gProg.ModyfiData[4]='-';
            CharToStr(DateTime.Mounth, &(gProg.ModyfiData[5]), 2);
            gProg.ModyfiData[7]='-'; 
            CharToStr(DateTime.Day, &(gProg.ModyfiData[8]), 2); 
            gProg.ModyfiData[10]=' ';
            CharToStr(DateTime.Hour, &(gProg.ModyfiData[11]), 2); 
            gProg.ModyfiData[13]=':';
            CharToStr(DateTime.Minute, &(gProg.ModyfiData[14]), 2); 
            gProg.ModyfiData[16]=':';
            CharToStr(DateTime.Second, &(gProg.ModyfiData[17]), 2); 
            for (char i=0; i<19; i++) if (gProg.ModyfiData[i]==0) gProg.ModyfiData[i]='0';

              unsigned int Save = __get_interrupt_state();            
              __disable_interrupt();
              //CleanFlash(AT91C_IFLASH_MEM->FlashProgram,sizeof(gProg));
                FlashWrite((char*)(AT91C_IFLASH_MEM->FlashProgram+FlashProgramOfset), (char*)&gProg,sizeof(gProg)); //(char *ptrMem, char *value, int length)
              //at91flashWrite((int)AT91C_IFLASH_MEM->FlashProgram+FlashProgramOfset,0,(char*)&gProg,sizeof(gProg));
              __set_interrupt_state(Save);
              __enable_interrupt(); 

 DispWait=DispWaitOff; 



}





int testcount=0;
void TestHendel(void)
{
  AT91F_PIO_GetInterruptStatus(AT91C_PIO_VBUS);
  if ((AT91C_BASE_PIOA->PIO_PDSR) & AT91C_VBUS) testcount++;
    

}


void TestInit(void)
{
//#ifndef USB_BUS_POWERED
    AT91F_PIO_CfgInput(AT91C_BASE_PIOA,AT91C_VBUS);
    AT91C_BASE_PIOA->PIO_PPUDR = AT91C_VBUS;
    
    // Configure VBus monitoring
        AT91F_PMC_EnablePeriphClock(AT91C_BASE_PMC, 1 << AT91C_ID_VBUS);
    AT91F_PIO_CfgPullup(AT91C_PIO_VBUS, ~AT91C_VBUS);
    AT91F_PIO_CfgInput(AT91C_PIO_VBUS, AT91C_VBUS);

    // Configure and enable the Vbus detection interrupt
    AT91F_AIC_ConfigureIt(AT91C_BASE_AIC,
                          AT91C_ID_VBUS,
                          5, //AT91C_AIC_PRIOR_LOWEST,
                          0,//AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL,
                          ISR_VBus);

    AT91F_PIO_InterruptEnable(AT91C_PIO_VBUS, AT91C_VBUS);
    AT91F_AIC_EnableIt(AT91C_BASE_AIC, AT91C_ID_VBUS);
}






//------------------------------------------------------------------------------
//!          Main
//------------------------------------------------------------------------------

//char ekran_tab[8][128]; 

char string[25];

void watchdogEnable(int millis)
{
  int period = (millis * 256) / 1000;
  AT91C_BASE_WDTC->WDTC_WDMR =  AT91C_WDTC_WDRSTEN |        // enable reset on timeout
                                AT91C_WDTC_WDDBGHLT |       // respect debug mode
                                AT91C_WDTC_WDIDLEHLT |      // respect idle mode
                                ((period << 16 ) & AT91C_WDTC_WDD) | // delta is as wide as the period, so we can restart anytime
                                (period & AT91C_WDTC_WDV);  // set the period
}










#define WATCHDOG_KEY (0xA5 << 24)

char TimeCompil[]= __TIME__;
char DateCompil[]= __DATE__;

extern  char VBUS_old;


#ifdef _LIVE_RJESTR 
int LIVE_TAB_IDX=0;  
unsigned short LIVE_TAB[LIVE_TAB_SIZE];  //   | A-Odczyt, B-potwierdzenie odczytu, C-Zapis F-flaga priorytetu | Nr wêz³a |  wartoœæ | | 
#endif
   

AT91PS_MEM FLASH;  

void main()
{
    FLASH=AT91C_IFLASH_MEM;  
  
    LCD_INIT();
    Init_LCD_TS();           //inicjalizacja Touchpanel`a
    CheckLCD();
    
    CleanCanvas(); 
    MenuPaint();    
    RepaintLCD();
    RepaintLCD();     
  
    RF_Status.Init=1;
    
    
    _NodeInfo  NodeInfo;
    #ifdef S300
      NodeInfo.Model=mS300;
    #endif
    #ifdef S300i  
      NodeInfo.Model=mS300i;
    #endif
    #ifdef S300iMax 
      NodeInfo.Model=mS300iMax;  
    #endif  
    #ifdef S300iOld  
      NodeInfo.Model=mS300i;
    #endif  
    
    //char SysV=(system[0])-0x30;  
    NodeInfo.SoftVersion=(system[0])-0x30;  
    //SysV=(system[2])-0x30;  
    NodeInfo.SoftSubVersion=(system[2])-0x30;  
    MainStage.StageQ[0].Value[0xA]=NodeInfo.NodeInfoWord;  
    
    
    AT91F_PIO_CfgOutput(AT91C_BASE_PIOA, SpeakerPin);
    AT91C_BASE_PIOA->PIO_CODR =SpeakerPin;
    
    //uruchomienie Watchdog`a na 60s 
    /*watchdogEnable(60000);  
    AT91F_WDTRestart(AT91C_BASE_WDTC);
    AT91C_BASE_WDTC->WDTC_WDMR=(unsigned int) AT91C_WDTC_WDV|AT91C_WDTC_WDRSTEN | AT91C_WDTC_WDD|0x0<<15;

    AT91C_BASE_WDTC->WDTC_WDMR=AT91C_WDTC_WDDIS; 
    */
   
    
    SpiMaster_Init();
    
    __enable_interrupt(); 

   InitRTC();  
    GetDateTimeFromRTC(); 
 
    InitFramework();

    SER_Init(&sSer, &sUsb);  // Initialize the serial driver

    InitFlash();  
  
    
    Init_LCD_Light_PWM(WypMaxJasnosci);
    
    __enable_interrupt();    
 
  
      
    #ifndef _DEBUG_
    //******************************************************************************
    //Ustawienia dla pierwszego uruchomienia starownika ****************************
      if (pAT91C_IFLASH_MEM->Config.FerstRun!=0x0A)
      {
        char WriteTmp;
       //zapis ustawieñ do flasha 
      
       unsigned int Save = __get_interrupt_state();            
       __disable_interrupt();
           //uruchomiono sterownik  
           WriteTmp=0xA;    FlashWrite((char*)&(AT91C_IFLASH_MEM->Config.FerstRun),&WriteTmp,1);
       __set_interrupt_state(Save);
       __enable_interrupt(); 
  

        }
    #endif 
 
     
      
    //Koniec ustawieñ dla pierwszego uruchomienia starownika ***********************
    //******************************************************************************

    ProgramChangeExecute(&gProg); 
    ThermostatsTimeChartExecute();
    
    
 //   US0_init();                                                                //inicjalizacja USART0
   
#ifdef __WIFI_MODULE    
    US1_init();                                                                 //inicjalizacja USART1
#endif
    
 //   Timer_10us_Setup();
    Timer_1ms_Setup();
    Timer_10ms_Setup();
    
    //Timer_1s();   
      
    for (int aaa=0; aaa<0xEFFFFF; aaa++)
            {
              asm( "nop" );
            }       

    g_ScreenSever_On=pAT91C_IFLASH_MEM->Config.ScreenSever_On;
    g_ScreenSever_Time_On=pAT91C_IFLASH_MEM->Config.ScreenSever_Time_On;
    g_ScreenSever_Time_Off=pAT91C_IFLASH_MEM->Config.ScreenSever_Time_Off;
    


    
    //odczyt NODE_ID
    if ((AT91C_IFLASH_MEM->NODE_ID!=0) && (AT91C_IFLASH_MEM->NODE_ID!=0xFFFFFFFF))
    {
      RF_Status.RF_ID_R=AT91C_IFLASH_MEM->NODE_ID;//pe³ne id
      RF_Status.RF_ID=AT91C_IFLASH_MEM->NODE_ID-(AT91C_IFLASH_MEM->NODE_ID/1000000)*1000000;
    }else{
      RF_Status.RF_ID_R=9999999; //pe³ne id
      RF_Status.RF_ID=999999;  //skrócone id (rejestrowe)
    }
    
    
 
//uruchomienie RF
    
    //odczyt kanalu z flasha ------------------------------ 
    
    SlaveMode=pAT91C_IFLASH_MEM->Config.SlaveMode;
    RF_Status.Ch=pAT91C_IFLASH_MEM->Config.RF_channel;
    if (RF_Status.Ch==0) RF_Status.Ch=1;
    
    
    InitPioInterrupt();
    
    RF_Init();
    
    
    char InitRF=1;
    
    while (InitRF==1)
    { 
        InitRF=RF_Status.Init;
        Delay(0x1);
    };

    

    //sprawdzenie kana³u transmisji radiowej
    if (!SlaveMode) //je¿eli master
    {
        SPI_SendWord(0xA000 | RF_SetFreqReg(RF_Status.Ch),Cs_RF); 
        RF_Status.EnableRX();
        Delay(0x100000);
        while (RF_CheckChannel())                                                                                            
        {
          
            if (RF_Status.Ch<RF_ChannelQuanti-1) {RF_Status.Ch++;} else {RF_Status.Ch=1;}
            SPI_SendWord(0xA000 | RF_SetFreqReg(RF_Status.Ch),Cs_RF); 
        }  
        RF_Status.MasterMode=1;
    }else{  //je¿eli slave
      
      /*
        SPI_SendWord(0xA000 | RF_SetFreqReg(RF_Status.Ch),Cs_RF); 
        RF_Status.EnableRX();
        Delay(0x100000);
        while (RF_CheckChannel())                                                                                            
        {
          
            if (RF_Status.Ch<RF_ChannelQuanti-1) {RF_Status.Ch++;} else {RF_Status.Ch=1;}
            SPI_SendWord(0xA000 | RF_SetFreqReg(RF_Status.Ch),Cs_RF); 
        }  
      */
      
        RF_Status.ChSlave=1;
        SPI_SendWord(0xA000 | RF_SetFreqReg(RF_Status.ChSlave),Cs_RF);  
        RF_Status.MasterMode=0;  
        RF_Status.EnableRX();
    }

    RF_Status.AllReady=1;
    
    
    


    
    
    //odczyt has³a------------------------------------------    
    if (pAT91C_IFLASH_MEM->Config.NeedPass1==0xEC)
    {//je¿eli nie wymagane has³o
     g_NeedPass=0; 
     for (char i=0; i<8; i++) g_Password_1[i]=pAT91C_IFLASH_MEM->Config.Pass1[i];
    }else if (pAT91C_IFLASH_MEM->Config.NeedPass1==0xEB)
    {//je¿eli nie wymagane has³o
     g_NeedPass=1; 
     for (char i=0; i<8; i++) g_Password_1[i]=pAT91C_IFLASH_MEM->Config.Pass1[i];
    }else
    {//jezeli nie zdefiniowano  - zdefiniuj i zapisz has³o domyœlne i ustaw jako nie wymagane
     
     //zapis has³a do flash
     unsigned int Save = __get_interrupt_state();  
     __disable_interrupt();
     //at91flashWrite((int*)(&(AT91C_IFLASH_MEM->Config.Pass1[0])),0,g_Password_1,9);
     
      FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.Pass1[0])), g_Password_1, 9);
     char NeedPassFlag=0xEC;
     g_NeedPass=0; 
     //at91flashWrite((int*)(&(AT91C_IFLASH_MEM->Config.NeedPass1)),0,&NeedPassFlag,1);
     FlashWrite((char*)(&(AT91C_IFLASH_MEM->Config.NeedPass1)), &NeedPassFlag, 1);
     __set_interrupt_state(Save);
     //koniec zapisu do flash
    }
    //koniec odczytu has³a ---------------------------------------- 


    
    
    
    
    USB_Connect(&sUsb);    
   __enable_interrupt();   
   
   
 
#ifdef __WIFI_MODULE   
   InitWizFi(AT91C_BASE_US1, &NET);
#endif

   
   
    /*
   //ustawienie wys³ania stanów  do wszystkich wêz³ów po restarcie
    for (char i=0; i<NumberNeurons; i++)
    {
      MainStage.SendStageChange[i]=ChangeStageFlag; 
    }
   */
   
   
    while (1) 
    {
        AT91F_WDTRestart(AT91C_BASE_WDTC);
        
        
        if (ForwardUSB2Serialferst==1)
        {
            // Start USB->Serial forward mechanism
            ForwardUSB2Serial(0, 0, 0, 0);
            ForwardUSB2Serialferst=2;
        }
      
       
        
       
       if ((!((AT91C_BASE_PIOA->PIO_PDSR) & AT91C_VBUS)) && (VBUS_old==1))
       {
         VBUS_old=0;
         ISR_VBus();
       }
       /*
       if (((AT91C_BASE_PIOA->PIO_PDSR) & AT91C_VBUS) && (VBUS_old==0))
       {
         VBUS_old=1;
         ISR_VBus();
       } 
       */
       
        
        
         
        if ((g_NextTimeProgram<=g_counter) || (g_NextTimeProgram>g_counter+ProgramPeriod))
         {
          
           
             char WaitingRF_WrTask=0;
             if (g_NextTimeProgram<g_counter+1000)    //blokowanie wykonania programu jezeli istnieje oczekuj¹ce ¿¹danie wys³ania stanu do któregokolwiek wêz³a. Blokowanie nie mo¿e przekroczyæ 1s. 
             {
                 char Nd=1;
                 while ((Nd<NumberNeurons) && (WaitingRF_WrTask==0))
                  {
                      if ((MainStage.SendStageChange[Nd]>0))    
                      {
                          WaitingRF_WrTask=1;
                      }
                      Nd++;  
                  } 
             }

            if (WaitingRF_WrTask==0)
            {
              g_NextTimeProgram=g_counter+ProgramPeriod;
              Program(&gProg);
            }
            
            
            
            
         }
         
        
        if ((g_counter>4000) && (MenuLevel==0)) 
        { 
            MenuLevel=1;   
          #ifdef _TESTER_ 

            MenuLevel=1134;
          #endif   
          MenuPaint();
        }
        

          
#ifdef __WIFI_MODULE     
        if (NET.WiFi.On)
        {
            //wifi-------------------------------------------------------------------
            if ((g_counter>6000) /*&&  (g_counter%200==0)*/)
            {
              CheckVarChangeHTTP(Buff_TX_1 ,AT91C_BASE_US1, &NET);
            }
        
            WiFi(US1_Buffer, Buff_TX_1 ,AT91C_BASE_US1, &NET);
        }
        //CheckConnectionStatus();
#endif  
  
        
        
        //bezpiecznik RF zabezpiecza przed zablokowaniem RFa
        if (RF_Status.RfFuse>=60) 
        {
            #ifdef _DEBUG_
                     MESSAGE("Bezpiecznik RF"); 
            #endif
                      
            RF_Init();          
        }   
    }
   
    
}

