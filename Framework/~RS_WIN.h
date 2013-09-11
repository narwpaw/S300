//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                 IASE Microcontroller Software Support                    *//                       
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : RS_WIN.h                                 *//
//*  Opis                         : Plik komunikacji z programem RS-win      *//
//*  Autor                        : Pawe³ Narwojsz                           *//
//*  Data utrzorzenia             : 09.09.2011                               *//
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


#ifndef _RSWIN_
#define _RSWIN_



    typedef enum {
                    RsWin_RamSection      =0xAC,
                    RsWin_RamOrFlash      =0xAD,
                  }_RsWin_Section;
    
    
    
    typedef enum {
                    RsWin_OrderRead           =0x03,
                    RsWin_OrderWrite          =0x05,
                    RsWin_OrderRestart        =0x01,
                    RsWin_OrderRemember       =0x06,
                    RsWin_Confirm             =0x02,
                    RsWin_Reply               =0x04,
                    RsWin_OrderConfigAnaGroup =0x08,
                    RsWin_OrderConfigBinGroup =0x09,
                    RsWin_NewSystem           =0x0D,
                  }_RsWin_Order;
    
    //adresy specjalne RS_WIN
                    #define    RsWin_ProgramSpeed   0xFF14     //Szybkoœæ pêtli programu u¿ytkowego (wg.RsWin obszar ram)
                    #define    RsWin_DeviceNameOld  0xFF80     //Nazwa urzadzenia (wg.RsWin (do RsWin 3.9) obszar ram)
                    #define    RsWin_DeviceName     0xFF90     //Nazwa urzadzenia (wg.RsWin obszar ram)
    
    
    //struktura danych odczytanych z USB
    #pragma pack(1)
    typedef struct
            {
              _RsWin_Section  Section; 
              _RsWin_Order    Order;   
              union 
                 { 
                   //odcztyt zapis ram
                   struct {    
                            unsigned short  Adress;     //adres
                            char            Length;     //liczba bajtów do odczytu
                            char            Data;         //suma kontrolna //dla odczytu
                          }RamReadWrite;  
                   //odczyt zapis dowolnego obszaru
                   struct { 
                            unsigned short  Section;     //adres
                            unsigned short  Adress;     //adres
                            char            Length;     //liczba bajtów do odczytu
                            char            Data;         //suma kontrolna //dla odczytu
                          }ReadWrite;  
                   
                   //Zapis konfiguracji zbioru zmiennych (analogów/dwustanów)
                   struct { 
                            char            Kode;     //kod zestawu danych
                            char            Length;   //liczba bajtów do odczytu
                            unsigned short  VarSymbol;//zmienna          
                          }ConfigGroup; 
                   
                   //odczyt zapis systemu
                   struct { 
                            unsigned short  Section;     //adres
                            unsigned long   Adress;     //adres
                            char            Length;     //liczba bajtów do odczytu
                            char            Data;         //suma kontrolna //dla odczytu
                          }ReadWriteSys;  
                   char   SK;
                 };
            }*_pRsWinResive;
    #pragma pack()
    
    //struktura danych do wys³ania do USB
    #pragma pack(1)
    typedef struct
            {
                _RsWin_Section    Section; 
                _RsWin_Order      Order;  
                char              SK;
            }*_pRsWinReply;
    #pragma pack()
    
    
    short int RsWin(_pRsWinResive Rx, char* Tx);
    char RsWin_SK(char *TabLiczb, unsigned char IlLiczb);
    short int RsWin_ReadUserProgram(unsigned short Adress, unsigned char Length, char *Tx, char* Memory);
    short int RsWin_ReadRam(unsigned short  Adress,unsigned char Length, char *Tx);
    short int RsWin_WriteRam(unsigned short  Adress,unsigned char Length,char *Data, char *Tx);
    short int RsWin_WriteUserProgram(unsigned short Adress, unsigned char Length, char *Data, char* Tx);
    short int RsWin_CurentProgramPlace(unsigned short Adress, char *Tx);
    char BinToMaster(char *adr);
    void MasterToBin(char *adr, char *vart);
    short int RsWin_ConfigBinGroup(char  Kode,unsigned char Length,short *Symbol, char *Tx);
    short int RsWin_ConfigAnaGroup(char  Kode,unsigned char Length,short *Symbol, char *Tx);
    short int RsWin_WriteSystem(unsigned long Adress, unsigned char Length, char *Data, char* Tx);
    short int RsWin_ReadSystem(unsigned long Adress, unsigned char Length, char *Tx, char* Memory);


#endif
