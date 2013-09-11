//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                 IASE Microcontroller Software Support                    *//                       
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : RS_WIN.c                                 *//
//*  Opis                         : Plik komunikacji z programem RS-win      *//
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


#include <intrinsics.h>
#include "core/common.h"
#include "core/device.h"
#include "core/trace.h"
#include "flash.h"
#include "Framework.h"
#include "Application_prog.h"

     
RAM ram;
extern char *pProg;
extern char WiFiDebug;
//extern FlashPagesOffset;
ConversFC Convers_DW_B;




//Zmienne zewnêtrzne -------------------------------------------------
    extern char g_czas_petli;
    extern const char identyfikacja[];
    extern int FlashProgramOfset;
    extern int FlashProgramReserveOfset;
    //extern char start_cykl;
    extern char g_Stop_Program;                                                     //1- zatrzymuje program
    extern char g_Restart_Program;                                                  //1- restartuje program
    extern _ProgramTab gProg;
    extern void Delay (unsigned long a);
    extern _Trap      Trap;
//_______________Koniec globalnych zmiennych zewnêtrznych ______________________

    

//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    komunikcja_RSwin
//* Interpretacja i wykonanie rozkazów wysy³anych z programu RS-win
//*-----------------------------------------------------------------------------
void komunikcja_RSwin(char *ptr1, char *ptr2, unsigned int *ptr3)
{


    //Zmienne lokalne ----------------------------------------------------------

      unsigned long adres =0;;
    //----------------Koniec zmiennych lokalnych -------------------------------
  
 
    //Zmienne tymczasowe -------------------------------------------------------
      int tmp =0;
      
    //----------------Koniec zmiennych tymczasowych ---------------------------- 
  
  
      
      
    //Zapis do dowolnego segmentu ----------------------------------------------
    if ((ptr1[0]==0xAD)& (ptr1[1]==0x05)) 
    {
      //Zapis programu zrodlowego
      adres = ptr1[4]+(ptr1[5]*0x100);
      if (ptr1[3]==0x70)
      {


             unsigned int Save = __get_interrupt_state();            
                __disable_interrupt();
                
            if (adres==0) 
            {
              //CleanFlash((int)AT91C_IFLASH_MEM->FlashProgram+FlashProgramOfset,0,sizeof(gProg));
              CleanFlash((char*)&AT91C_IFLASH_MEM->FlashProgram+FlashProgramOfset,sizeof(gProg));
            }
            
               
                at91flashWrite((int)AT91C_IFLASH_MEM->FlashProgram+FlashProgramOfset,adres,ptr1+7,ptr1[6]);
            
                
                __set_interrupt_state(Save);
                __enable_interrupt();  
           
            
            //Potwierdzenie
            ptr2[0]=0xAC;
            ptr2[1]=0x02;
            *ptr3=2;
            ptr2[2]=CheckSum(ptr2, ptr3);
            
              /*
                     memory_read=(char*)&gProg+adres;
            
            for (char i=0; i<ptr1[6]; i++)
            {
              *memory_read=*(ptr1+7+i);
              memory_read++;
            }
                
            */
        
          //przepisz program z flash do ram
          char *memory_read_prog;
          memory_read_prog=(char*)(AT91C_IFLASH_MEM->FlashProgram+FlashProgramOfset);
      
          
          char *SourRam= (char*)&gProg;
          int tProgramTabSize=ProgramTabSize;
          for (int k = 0 ; k <= tProgramTabSize ; k++) 
          { 
                SourRam[k]=memory_read_prog[k]; 
                
          }
      } 
         
      
      if (ptr1[3]==0x10)
      {
         char *memory_read;
            memory_read=(char*)&gProg+adres;
            
            for (char i=0; i<ptr1[6]; i++)
            {
              *memory_read=*(ptr1+7+i);
              memory_read++;
            } 
            
            
           //Potwierdzenie
            ptr2[0]=0xAC;
            ptr2[1]=0x02;
            *ptr3=2;
            ptr2[2]=CheckSum(ptr2, ptr3);   
      
      }
      
    }//-----------Koniec zapisu do dowolnego segmentu --------------------------         
    
    
    
    //Odczyt z dowolnego segmentu ----------------------------------------------
    if ((ptr1[0]==0xAD)& (ptr1[1]==0x03)) 
    {
      

      //Odczyt programu zrodlowego
      if ((ptr1[2]==0x00) & (ptr1[3]==0x10) /*& (ptr1[4]!=0x40)*/)
      {
        
        tmp = 2;
        
        ptr2[0] =0xAC;
        ptr2[1] =0x04;
        //*ptr3=2;
        
        adres = ptr1[4]+(ptr1[5]*0x100);
        
        char *memory_read;
        
        //memory_read=(char*)(AT91C_IFLASH_MEM->FlashProgram+FlashProgramReserveOfset);
        memory_read=(char*)&gProg;
        
        
        for (int k = 0 ; k <= (ptr1[6]-1) ; k++) 
        { 

            tmp++;
            
            ptr2[2+k] =memory_read[adres+k]; 
            
        
        }
        
        
        

        *ptr3=tmp;
        ptr2[tmp]=CheckSum(ptr2, ptr3);
        
      }
//tmp
      if ((ptr1[2]==0x00) & (ptr1[3]>0x70) & (ptr1[3]<0xC0)  /*& (ptr1[4]!=0x40)*/)
      {
        
        tmp = 2;
        
        ptr2[0] =0xAC;
        ptr2[1] =0x04;
        //*ptr3=2;
        
        adres = ptr1[4]+(ptr1[5]*0x100);
        for (int k = 0 ; k <= (ptr1[6]-1) ; k++) 
        { 
            ptr2[2+k] = pProg[adres+k];
            tmp++;
        }

        *ptr3=tmp;
        ptr2[tmp]=CheckSum(ptr2, ptr3);
        
      } 
//kon tmp
      
    }//------------------Koniec odczytu z dowolnego segmentu--------------------    
    
    

    //Odczyt z segmentu 0 ------------------------------------------------------
    if ((ptr1[0]==0xAC)& (ptr1[1]==0x03) & (ptr1[2]!=0x14) & (ptr1[2]!=0x80) ) 
    {
      
      //odczyt zmiennych dwustanwych
      if (   (ptr1[2]+(ptr1[3]*0x100)>= 0xC000) &&  (ptr1[2]+(ptr1[3]*0x100)<= 0xC7FF)  )
      {
        
        
        ptr2[0] =0xAC;
        ptr2[1] =0x04;
        tmp = 2;
        
        for (int k = 0; k <= ptr1[4]-1; k++) 
        { 
          adres = (ptr1[2]+(ptr1[3]*0x100)-0xC000 +k);
          ptr2[2+k] = BinVarToMaster(&adres);     
          tmp++;
        }
        
      }//Koniec odczytu zmiennych dwustanowych
            
            
      //Odczyt zmiennych analogowych
      if (  (ptr1[2]+(ptr1[3]*0x100)>= 0xC800) &&  (ptr1[2]+(ptr1[3]*0x100)<= 0xDFFF) )
      {   
        
        ptr2[0] =0xAC;
        ptr2[1] =0x04;
        tmp = 2;
        
        for (int k = 0; k <= ptr1[4]-1; k=k+4) 
        { 
          
          adres = ((ptr1[2]+(ptr1[3]*0x100)-0xC800)/4)  +(k/4);
          
          Convers_DW_B.DWvar = AnaVarToMaster(&adres);  
          
          // zmiana kolejnoœci dla nowego RSWINa
         /*
          ptr2[k+2]   =Convers_DW_B.Bvar[3];
          ptr2[k+3]   =Convers_DW_B.Bvar[2];
          ptr2[k+4]   =Convers_DW_B.Bvar[0];
          ptr2[k+5]   =Convers_DW_B.Bvar[1];
         */
          ptr2[k+2]   =Convers_DW_B.Bvar[0];
          ptr2[k+3]   =Convers_DW_B.Bvar[1];
          ptr2[k+4]   =Convers_DW_B.Bvar[2];
          ptr2[k+5]   =Convers_DW_B.Bvar[3];
          
          tmp=tmp+4;
         }
       }//Koniec odczytu zmiennych analogowych
       *ptr3=tmp;
       ptr2[tmp]=CheckSum(ptr2, ptr3);
    
    }//-------------------Koniec odczytu z segmentu 0---------------------------
    
    
    
    //Zapis do segmentu 0-------------------------------------------------------
    if ((ptr1[0]==0xAC)& (ptr1[1]==0x05)) 
    {

      //Zapis zmiennych dwustanowych
      if ( (ptr1[2]+(ptr1[3]*0x100)>= 0xC000) &&  (ptr1[2]+(ptr1[3]*0x100)<= 0xC7FF))
      {
        for (int k = 0; k <= ptr1[4]-1; k++) 
        { 
          adres = (ptr1[2]+(ptr1[3]*0x100)-0xC000+k);
          MasterToBinVar(&adres, &ptr1[5]);              //Konwersja z formatu Master i zapis
        }        
      }//Koniec zapisu zmiennych dwustanowch
            
            
      
      //Zapis zmiennej analogowej
      if ( (ptr1[2]+(ptr1[3]*0x100)>= 0xC800) &&  (ptr1[2]+(ptr1[3]*0x100)<= 0xDFFF))
      {
                                                 

        for (int k = 0; k <= ptr1[4]-1; k=k+4) 
        { /*
          // zmiana kolejnoœci dla nowego RSWINa
          
          Convers_DW_B.Bvar[3] = ptr1[5+(k)+0];
          Convers_DW_B.Bvar[2] = ptr1[5+(k)+1];
          Convers_DW_B.Bvar[0] = ptr1[5+(k)+2];
          Convers_DW_B.Bvar[1] = ptr1[5+(k)+3];
          */
          Convers_DW_B.Bvar[0] = ptr1[5+(k)+0];
          Convers_DW_B.Bvar[1] = ptr1[5+(k)+1];
          Convers_DW_B.Bvar[2] = ptr1[5+(k)+2];
          Convers_DW_B.Bvar[3] = ptr1[5+(k)+3];
          
          
          adres= (((ptr1[2]+(ptr1[3]*0x100)-0xC800))/4)+(k/4);
          MasterToAnaVar(&adres, &Convers_DW_B.DWvar);

        } //koniec petli k
        
      }//Koniec zapisu zmiennych analogowych
            
            
            
            
        //Potwierdzenie
        ptr2[0]=0xAC;
        ptr2[1]=0x02;
        *ptr3=2;
        //koniec potwoerdzenia
        
         ptr2[2]=CheckSum(ptr2, ptr3);                                        //suma kontrolna
 
    }//------------------Koniec zapisu do segmentu 0---------------------------       
       
    

    
    //!!! Standardowe pytania -------------------------------------------------- 
   
        //Przepisz program uzytkowy z Ram do flash
        if ( (ptr1[0]==0xAC) && (ptr1[1]==0x06) && (ptr1[2]==0x4E) )
          {
            ptr2[0]=0xAC;
            ptr2[1]=0x02;
            *ptr3=2;
            ptr2[2]=CheckSum(ptr2, ptr3);
            
            
             RamToFlash();
             ProgramChangeExecute(&gProg); 

          }


    
          //Ustaw pu³apkê
          if (
               (ptr1[0]==0xAA)&&
               (ptr1[1]==0xAA)&&
               (ptr1[2]==0x01)
              )
            {
             
              Trap.Row=ptr1[3];
              Trap.Col=ptr1[4];
              Trap.Enable=1;
              Trap.Activ=0;
              Trap.Change=1;
              
              
              //Potwierdzenie
               ptr2[0]=0xAB;
               ptr2[1]=0xAB;
               *ptr3=2;
               ptr2[2]=CheckSum(ptr2, ptr3);                                        //suma kontrolna
              //koniec potwoerdzenia

            }//koniec "Ustaw pu³apkê"
         
    
    
          //Usuñ pu³apki
          if (
               (ptr1[0]==0xAA)&&
               (ptr1[1]==0xAA)&&
               (ptr1[2]==0x02)
              )
            {
              
              Trap.Enable=0;
              Trap.Row=0;
              Trap.Col=0;
              Trap.Activ=0;
             
              //Potwierdzenie
               ptr2[0]=0xAB;
               ptr2[1]=0xAB;
               *ptr3=2;
               ptr2[2]=CheckSum(ptr2, ptr3);                                        //suma kontrolna
              //koniec potwoerdzenia

            }//koniec "Ustaw pu³apkê"
         
    
    
           //Odczytaj stany "Output" procedur (ptr1[3]-startowy rz¹d procedur, ptr1[4]-ilosc rzedow do odczytu
          if (
               (ptr1[0]==0xAA)&&
               (ptr1[1]==0xAA)&&
               (ptr1[2]==0x03)
              )
            {

                
        
                ptr2[0] =0xAC;
                ptr2[1] =0x04;

                tmp = 2;

                for (int Row = ptr1[3] ; Row<ptr1[3]+ptr1[4] ; Row++) 
                { 
                  for (int Col = 0 ; Col<MaxPrcInLine ; Col++) 
                  { 
                    ptr2[tmp++] =gProg.Line[Row].Proc[Col].Out>>8; 
                    ptr2[tmp++] =gProg.Line[Row].Proc[Col].Out & 0xFF; 
                  }  
                }
            

                *ptr3=tmp;
                ptr2[tmp]=CheckSum(ptr2, ptr3);

              
              
            }//koniec "Ustaw pu³apkê"
          
    
    
    
        //Komunikacja startowa  
          //pytanie 0 - pytanie o prêdkoœæ pêtli programu
          if (
               (ptr1[0]==0xAC)&&
               (ptr1[1]==0x03)&&
               (ptr1[2]==0x14)&&
               (ptr1[3]==0xFF)&&
               (ptr1[4]==0x02)&&
               (ptr1[5]==0x3C)
              )
            {
              //odpowiedŸ
              ptr2[0]=0xAC;
              ptr2[1]=0x04;
              ptr2[2]=g_czas_petli / 10;
              ptr2[3]=0x00;
              *ptr3=4;
              ptr2[4]=CheckSum(ptr2, ptr3);
            }//koniec pytania 0
         
  
         //pytanie 1 
         if (
               (ptr1[0]==0xAD)&&
               (ptr1[1]==0x03)&&
               (ptr1[2]==0x00)&&
               (ptr1[3]==0xA0)&&
               (ptr1[4]==0x0D)&&
               (ptr1[5]==0x00)&&
               (ptr1[6]==0x01)&&
               (ptr1[7]==0xA2)
            
              )
          {
              //odpowiedŸ
              ptr2[0]=0xAC;
              ptr2[1]=0x04;
              ptr2[2]=0xFF;
              
              *ptr3=3;
              ptr2[3]=CheckSum(ptr2, ptr3);
            
          }//koniec pytania 1
         
         
         //pytanie 2  (pytanie o sterownik)
         if (
               (ptr1[0]==0xAC)&&
               (ptr1[1]==0x03)&&
               (ptr1[2]==0x80)&&
               (ptr1[3]==0xFF)&&
               (ptr1[4]==0x10)&&
               (ptr1[5]==0xC2)
              )
          {
              //odpowiedŸ
              ptr2[0]=0xAC;
              ptr2[1]=0x04;
              
              for(int k=2; k<=11; k++)
              {
                ptr2[k]=identyfikacja[k-2];
              }
              
              *ptr3=18; 
              ptr2[18]=CheckSum(ptr2, ptr3);
          }
         
         
         //pytanie 3 // 1-jest program, 2 - brak programu
         if (
               (ptr1[0]==0xAD)&&
               (ptr1[1]==0x03)&&
               (ptr1[2]==0x00)&&
               (ptr1[3]==0x60)&&
               (ptr1[4]==0x0D)&&
               (ptr1[5]==0x00)&&
               (ptr1[6]==0x01)&&
               (ptr1[7]==0xE2)
              )
          {
              //odpowiedŸ
              ptr2[0]=0xAC;
              ptr2[1]=0x04;
              ptr2[2]=0x01;
              //ptr2[3]=0x4F;
             
              *ptr3=3;
              ptr2[3]=CheckSum(ptr2, ptr3);
          }
         
 
         //pytanie 4 - pytanie o nazwe aktualnie dzia³j¹cego programu u¿ytkowego
         if (
               (ptr1[0]==0xAD)&&
               (ptr1[1]==0x03)&&
               (ptr1[2]==0x00)&&
               (ptr1[3]==0x90)&&
               (ptr1[4]==0x40)&&
               (ptr1[5]==0x00)&&
               (ptr1[6]==0x20)&&
               (ptr1[7]==0x60)
              )
          {
              //odpowiedŸ
              ptr2[0]=0xAC;
              ptr2[1]=0x04;
              
              for (int k=2; k<=32; k++)
              {
               ptr2[k]=pProg[0x40 - 2 +k]; 
              }

              *ptr3=33;
              ptr2[33]=CheckSum(ptr2, ptr3);
              //Suma=true;
              
          }
    
    
    
          if (
               (ptr1[0]==0xAC)&&
               (ptr1[1]==0xAC)&&
               (ptr1[2]==0x01)
              )
            {

               WiFiDebug=1;
              
              //Potwierdzenie
               ptr2[0]=0xAC;
               ptr2[1]=0xAC;
               *ptr3=2;
               ptr2[2]=CheckSum(ptr2, ptr3);                                        //suma kontrolna
              //koniec potwoerdzenia

            }//koniec
    
    
    
           if (
               (ptr1[0]==0xAC)&&
               (ptr1[1]==0xAC)&&
               (ptr1[2]==0x02)
              )
            {

               WiFiDebug=0;
              
              //Potwierdzenie
               ptr2[0]=0xAC;
               ptr2[1]=0xAC;
               *ptr3=2;
               ptr2[2]=CheckSum(ptr2, ptr3);                                        //suma kontrolna
              //koniec potwoerdzenia

            }//koniec


    
     /*    
         //pytanie 5 
         if (
               (ptr1[0]==0xAD)&&
               (ptr1[1]==0x03)&&
               (ptr1[2]==0x00)&&
               (ptr1[3]==0x10)&&
               (ptr1[4]==0x40)&&
               (ptr1[5]==0x00)&&
               (ptr1[6]==0x20)&&
               (ptr1[7]==0xE0)
              )
          {
             //odpowiedŸ
              ptr2[0]=0xAC;
              ptr2[1]=0x04;
              ptr2[2]=0x73;
              ptr2[3]=0x41;
              ptr2[4]=0x73;
              ptr2[5]=0x42;
              ptr2[6]=0x45;
              ptr2[7]=0xD6;
              ptr2[8]=0x84;
              ptr2[9]=0x75;
              ptr2[10]=0xA5;
              ptr2[11]=0xE8;
              ptr2[12]=0x47;
              ptr2[13]=0x00;
              ptr2[14]=0xA0;
              ptr2[15]=0x27;
              ptr2[16]=0x21;
              ptr2[17]=0xDE;
              ptr2[18]=0x44;
              ptr2[19]=0x5D;

              *ptr3=20;   
              ptr2[20]=CheckSum(ptr2, ptr3);
              
     
          }
         
        */
    
         //rozkaz uruchomienia nowo wgranego programu
         if (
               (ptr1[0]==0xAC)&&
               (ptr1[1]==0x01)&&
               (ptr1[2]==0x53)
              )
          {
              //zatrzymaj program
              g_Stop_Program=1;
              //ustaw restart programu
              g_Restart_Program=1;
              //cykl startowy
              //start_cykl = 0;
            
              //przepisz program z flash do ram
              char *memory_read_prog;
              memory_read_prog=(char*)(AT91C_IFLASH_MEM->FlashProgram+FlashProgramReserveOfset);
              for (int k = 0 ; k <= UseProgramSize ; k++) 
              { 
                  pProg[k]=memory_read_prog[k]; 
              }
              //koniec

              *ptr3=0;
             
            
             
             //wznów wykonywanie programu
              g_Stop_Program=0; 
          }    
    
    
         
         //pytanie 6 
         if (
               (ptr1[0]==0xAD)&&
               (ptr1[1]==0x03)&&
               (ptr1[2]==0x00)&&
               (ptr1[3]==0x10)&&
               (ptr1[4]==0xC3)&&
               (ptr1[5]==0x00)&&
               (ptr1[6]==0x04)&&
               (ptr1[7]==0x00)
              )
          {
              //odpowiedŸ
              ptr2[0]=0xAC;
              ptr2[1]=0x04;
              ptr2[2]=0x70;
              ptr2[3]=0xC9;
              ptr2[4]=0x90;
              ptr2[5]=0x5D;
              //ptr2[6]=0x2A;
              *ptr3=6;    
              ptr2[6]=CheckSum(ptr2, ptr3);
          }
    
   
         
}

//SUMA KONTEROLNA---------------------------------------------------------------
 byte SK=0;
char CheckSum(char *TabLiczb, unsigned int *IlLiczb)
{
        SK=0;
        for (int k = 0; k<*IlLiczb; k++) 
        {
          SK += TabLiczb[k]; 
        }
        SK = 0 - SK;
        TabLiczb[*IlLiczb]=SK;
        *IlLiczb=*IlLiczb+1;
        return  SK;
        
}
//------------------------------------------------------------------------------

               
//Funkcja konwersji zmiennej analogowej z formatu MASTER na format Float oraz zapisu do RAM------- 
void MasterToAnaVar(unsigned long *adr, unsigned long *vart)           //(nr. zmiennej, wartoœæ do konwersji)
{
          
  ConversFC Convers_DW_Btmp;
  unsigned long zm;

  
          
          Convers_DW_Btmp.DWvar = *vart;   
          zm=Convers_DW_Btmp.DWvar;
          
}
//------------------------------------------------------------------------------


//Funkcja konwersji zmiennej dwustanowej z formatu MASTER na format Float------- 
void MasterToBinVar(unsigned long *adr, char *vart)           //(nr. zmiennej, wartoœæ do konwersji)
{
 
  /*
        //ustawienie "1"
        if ((*vart & 0xC0)==0x80)
        { 
          ram.BinVarRAM[(*adr/8)] |= (0x1 << (*adr%8));
        }
        if (((*vart & 0xC0)==0x00) || ((*vart & 0xC0)==0x40))
        { 
          ram.BinVarRAM[(*adr/8)] &= ~(0x1 << (*adr%8));
        }
        //Ustawienie forsowanie
        if ((*vart & 0x1F)==0x15) 
        { 
          ram.BinVarRAMfors[(*adr/8)] |= (0x1 << (*adr%8));
        }
         else
        { 
          ram.BinVarRAMfors[(*adr/8)] &= ~(0x1 << (*adr%8));
        }
  */
}
//------------------------------------------------------------------------------


//funkcja konwersji zmiennej dwustanowej z formatu Float na format MASTER ------
char BinVarToMaster(unsigned long *adr)                    
{

}
//------------------------------------------------------------------------------


//funkcja konwersji zmiennej analogowej z formatu Float na format MASTER -------
unsigned long AnaVarToMaster(unsigned long *adr)
{
  ConversFC Convers_DW_Btmp;
  unsigned long zm;

          
     
          
          Convers_DW_Btmp.DWvar = 0;

          return zm;
          
          
}
//------------------------------------------------------------------------------

