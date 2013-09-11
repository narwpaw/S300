//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                 IASE Microcontroller Software Support                    *//                       
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : LCD.h                                    *//
//*  Opis                         : Biblioteka obs³ugi LCD                   *//
//*  Autor                        : Pawe³ Narwojsz                           *//
//*  Data utrzorzenia             : 15.09.2007                               *//
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

//#define _LCD_NEGATIV_OLD_   //w³¹czenie negatywowej obs³ugi magistrali wyœwietlacza



#ifndef _LCD_
#define _LCD_

typedef char __Char[15];
typedef __Char __String;





#define AT_LCDD_0 (AT91_LCD *) // (RTTC) Real-time Status Register
#define  LCD_PIO             AT91C_BASE_PIOA

extern char RowsPoz;


//!-----------------------------------------------------------------------------
//!  Konfiguracja pinów wyœwietlacza
//!-----------------------------------------------------------------------------
//!----------------------------------------------------------------------------- 

    #define   mDI          2  // data / instruction (0 - instruction, 1 - data)
    #define   mRW          3       // data read/write
    #define   mENABLE      30  // H->L enable signal
    #define   mDB0         29  // DATA DB0-DB7
    #define   mDB1         28  //
    #define   mDB2         27  //
    #define   mDB3         4  //
    #define   mDB4         8  //
    #define   mDB5         26  //
    #define   mDB6         25  //
    #define   mDB7         15  //
    #define   mCS1         16  // chip select CS1
    #define   mCS2         23  // chip select CS2
    #define   mLightLCD    0  // LCD LIGHT

//_______________________ Koniec konfiguracji pinów wyœwietlacza ________________________________
      


#define   DI          1<<mDI
#define   RW          1<<mRW  
#define   ENABLE      1<<mENABLE 
#define   DB0         1<<mDB0  
#define   DB1         1<<mDB1  
#define   DB2         1<<mDB2  
#define   DB3         1<<mDB3  
#define   DB4         1<<mDB4  
#define   DB5         1<<mDB5  
#define   DB6         1<<mDB6  
#define   DB7         1<<mDB7  
#define   CS1         1<<mCS1  
#define   CS2         1<<mCS2 
#define   LightLCD    1<<mLightLCD





/* Ustawienia drugie
#define   DI          1<<30  // data / instruction (0 - instruction, 1 - data)
#define   RW          1<<29       // data read/write
#define   ENABLE      1<<28  // H->L enable signal
#define   DB0         1<<27  // DATA DB0-DB7
#define   DB1         1<<31  //
#define   DB2         1<<11  //
#define   DB3         1<<12  //
#define   DB4         1<<26  //
#define   DB5         1<<25  //
#define   DB6         1<<8  //
#define   DB7         1<<23  //
#define   CS1         1<<2  // chip select CS1
#define   CS2         1<<15  // chip select CS2

#define   mDB0         27  // DATA DB0-DB7
#define   mDB1         31  //
#define   mDB2         11  //
#define   mDB3         12  //
#define   mDB4         26  //
#define   mDB5         25  //
#define   mDB6         8  //
#define   mDB7         23  //
*/


/*ustawienia pierwsze
#define   DI          1<<30  // data / instruction (0 - instruction, 1 - data)
#define   RW          1<<29       // data read/write
#define   ENABLE      1<<28  // H->L enable signal
#define   DB0         1<<27  // DATA DB0-DB7
#define   DB1         1<<31  //
#define   DB2         1<<11  //
#define   DB3         1<<12  //
#define   DB4         1<<26  //
#define   DB5         1<<25  //
#define   DB6         1<<24  //
#define   DB7         1<<23  //
#define   CS1         1<<22  // chip select CS1
#define   CS2         1<<21  // chip select CS2

#define   mDB0         27  // DATA DB0-DB7
#define   mDB1         31  //
#define   mDB2         11  //
#define   mDB3         12  //
#define   mDB4         26  //
#define   mDB5         25  //
#define   mDB6         24  //
#define   mDB7         23  //

*/

#define LCD_PIO         AT91C_BASE_PIOA
#define LCD_PIO_ID      AT91C_ID_PIOA
#define LCD_BUS_MASK      DI | RW | ENABLE | DB0 | DB1 | DB2 | DB3 | DB4 | DB5 | DB6 | DB7 



typedef volatile unsigned int AT91_LCD;
typedef struct _LCD_0 {
  
 AT91_LCD LCDD_0[64];
 AT91_LCD LCDD_1[64];
 AT91_LCD LCDD_2[64];
 AT91_LCD LCDD_3[64];
    
} LCD_0; 


typedef struct{
                unsigned short x;
                unsigned short y;
              }_TPoint;




void at91DisplayOnOff(char on_off);
void at91DisplayStartLine(void);
void at91DisplayStartPage(char lcd_start_page);
void at91DisplayYAdress(char display_Y);
void at91DisplayData(char lcd_data);
void at91ChipSelect(char chip_select);
void RepaintLCD(void);
void CleanCanvas(void);
void HalfCleanCanvas(void);
void CleanCanvasArea(char X, char Y, char X_Lenght, char Y_Lenght, char Colour);
void CopyToCanvas(char Tabela[8][128]);
void CopyToCanvasPoint(char *Tabela, char X, char Y, char X_Lenght, char Y_Lenght);
//void List_Paint(char name[10], char Rows, char RowWith, char Y, char FormStyle, char Algin, char p1[15], char p2[15] , char p3[15] , char p4[15] , char p5[15] , char p6[15] , char p7[15] , char p8[15]);
void Ask_Paint(char name[10], char p1[15], char p2[15]);
int Ask_Event(int X_up, int Y_up, int X_down, int Y_down);
int List_Event(int X_up, int Y_up, int X_down, int Y_down, char Rows, char RowWith, char FormStyle, char Y);
void ReadTP(_TPoint *Point);
unsigned short ReadADC (char Channel);
void Init_LCD_TS(void);
char Label (char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw); 
char FloToStr (float Value, char Vstring[8],char ill);
void TimeToStr (char godz, char min, char sek, char Vstring[8]);
void DateToStr (char dzin, char mies, char rok, char Vstring[8]);
void CharToStrData (char Value, char *Vstring);
void HexToStr (int Value, char Vstring[8],char length);
void H_line (char X, int Y, char lenght, char typ);
void V_line (char X, int Y, char lenght, char typ);
void Number_Paint(char name[10], char p1[2], char p2[2]);
int Number_Event(int X_up, int Y_up, int X_down, int Y_down);
void Message_Paint(char algin, char p1[15], char p2[15], char p3[15],char p4[15],char p5[15]);
void ScrollList_Paint(char name[15], int Size_scroll, int *_Suw_poz, char poz);
char CharToStr (char Value, char *Vstring, char ill);
char Button (char _Label[15], char poz_X , char poz_Y ,char width, char height);
void CanvasPixel(char X, char Y, char Colour);
char Zone (char _Label[15], char poz_X , char poz_Y ,char width, char height, char LineType);
char sIntToStr (signed short int Value, char *Vstring);
void CheckLCD(void);
void Message(char p1[15], char p2[15], int MenuLevelJump);
void List_Paint(char name[10], char Rows, char RowWith, char Y, char FormStyle, char Algin, __String p,...);
void Label18 (char *Vstring, char alig, char X, int Y, char maxChar, char clean, char minY, char maxY, char negatyw);
void GetTouchPoint(signed short *Xd,signed short  *Yd, signed short *Xu, signed short  *Yu);
void Rectangle(char X1,char Y1,char Width,char Height,...);
char LabelLength (char *Vstring, char maxChar); 
#endif



