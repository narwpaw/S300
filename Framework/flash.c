#include "flash.h"
#include <intrinsics.h>




void InitFlash(void)
{

  AT91C_BASE_MC->MC_FMR = (((0x48)<<16) | AT91C_MC_FWS_2FWS);     // konfiguracja MC_FMR

  SET(AT91C_BASE_MC->MC_FMR, ((AT91C_MASTER_CLOCK / 666666)) << 16);
    
  
}




//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    at91flashWrite
//* Funkcja zapisu do flash
//*-----------------------------------------------------------------------------
//*  ofset  - przesuniêcie
//*  cell   - pierwsza komórka zapisu
//*  value  - wartoœæ do zapisu
//*  length - wielkoœæ danych
//*----------------------------------------------------------------------------

__ramfunc void at91flashWrite(  unsigned int ofset, unsigned int cell, char *value,  int length)
 {
   
   unsigned int Page=ofset+cell/256;
   unsigned int komorka=ofset+cell%256;
   int length_tmp;
   int data = 0; 
   unsigned long *memory;
   

   
  // TRACE_INFO("\n\r---  FlashWrite  --- Ofset: %d, Komorka: %d, Rozmar: %dB\n\r",ofset,cell,length );  
   
   union 
   {
               char tab_B[256];
      unsigned long tab_DW[64];
   };

   do
   {
     if ((komorka+length)>256) {length_tmp=256-komorka;} else {length_tmp = length;};

      memory=(unsigned long*) (Page*256);  //zmiana z memory=(unsigned long*) (ofset+page*256);
     for (unsigned int i=0; i<=63; i++)
      {  
        tab_DW[i]=memory[i];     
      }  
     
     for (int i=komorka; i<=komorka+length_tmp-1; i++)
      {
        tab_B[i]=(unsigned long)value[data];
        data++;
      }
  
     for (unsigned int i=0; i<=63; i++)
      {
        AT91C_IFLASH_MEM->MEM_PAGE_0[i]=tab_DW[i];
      }  
   
     unsigned int Status = AT91C_BASE_MC->MC_FSR;

     SET(AT91C_BASE_MC->MC_FCR,
        AT91C_MC_CORRECT_KEY
        | AT91C_MC_FCMD_START_PROG
        | (((Page) << 8) & AT91C_MC_PAGEN));   //zmaina page+(ofset/256)
    
     while((AT91C_BASE_MC->MC_FSR & AT91C_MC_FRDY) != AT91C_MC_FRDY);   
    
     komorka = 0;
     length = length-length_tmp;
     Page++;
    
   }while (length>0); 
    

}












#define FLASH_PAGE_SIZE_BYTE 512




//*---------------------------------------------------------------------------- 
//* \fn    AT91F_Flash_Ready 
//* \brief Czekanie na zapis Flash'a 
//*---------------------------------------------------------------------------- 
__ramfunc int AT91F_Flash_Ready (void) 
{ 
    unsigned int status; 
    status = 0; 

    // Wait the end of command 
        while ((status & AT91C_MC_FRDY) != AT91C_MC_FRDY ) 
        { 
          status = AT91C_BASE_MC->MC_FSR; 
        } 
        return status; 
} 



__ramfunc int AT91F_Flash_Write( unsigned int Flash_Address ,int size ,unsigned int * buff) 
{ 
    // set the Flash controller base address 
    AT91PS_MC ptMC = AT91C_BASE_MC; 
    unsigned int i, page, status; 
    unsigned int * Flash; 
    
    // init flash pointer 
    Flash = (unsigned int *) Flash_Address; 
    // Get the Flash page number 
    page = ((Flash_Address - (unsigned int)AT91C_IFLASH ) /FLASH_PAGE_SIZE_BYTE); 
    // copy the new value 
    

    
    
    for (i=0; (i < FLASH_PAGE_SIZE_BYTE) & (size > 0) ;i++, Flash++,buff++,size-=4 )
    { 
    *Flash=*buff; 
    } 
    
    
    __disable_interrupt();
    unsigned long ulIntrStat = AT91C_BASE_AIC->AIC_IMR; 
    AT91C_BASE_AIC->AIC_IDCR = 0xFFFFFFFF; 
    
    // Write the write page command 
    ptMC->MC_FCR = AT91C_MC_CORRECT_KEY | AT91C_MC_FCMD_START_PROG | (AT91C_MC_PAGEN & (page <<8)) ; 
    
    //for a certain ammount of time you cant do any operation over flash memory. 
    for(i=0;i<100000;i++) asm("nop"); 
    
    //Enable ints 
    AT91C_BASE_AIC->AIC_IECR = ulIntrStat; 
    
    // Wait the end of command 
    status = AT91F_Flash_Ready(); 
    
     __enable_interrupt();
    // Check the result 
    if ( (status & ( AT91C_MC_PROGE | AT91C_MC_LOCKE ))!=0) 
    return false; 
    return true; 
} 




   /*

//__ramfunc int AT91F_Flash_Write( unsigned int Flash_Address ,int size ,unsigned int * buff) 
__ramfunc  void FlashWrite(volatile char *ptrMem,volatile char *value,volatile int length)
{ 
  

    // set the Flash controller base address 
    AT91PS_MC ptMC = AT91C_BASE_MC; 
    unsigned int i, page, status; 
    unsigned int * Flash; 
    
    // init flash pointer 
    Flash = (unsigned int *) Flash_Address; 
    // Get the Flash page number 
    page = ((Flash_Address - (unsigned int)AT91C_IFLASH ) /FLASH_PAGE_SIZE_BYTE); 
    // copy the new value 




  volatile int page=     (int)ptrMem/256;
   volatile int komorka = (int)ptrMem%256;   
   volatile int PtrInt=(int)ptrMem;
   volatile int length_tmp;
   volatile int data = 0; 
   volatile int *UptrMem;
   volatile char* ptrMem2;

   if ((PtrInt>0x00140000) || (PtrInt<0x00120000) || (komorka>0xFF))
   {
       char mess[]="Blad zapisu. Przekroczenie dozwolonego obszaru";
       MESSAGE(mess); 
   }else{
 
       union 
       {
                   char B[256];
          unsigned long DW[64];
       }tab;
    
       do
       {
         if ((komorka+length)>256) {length_tmp=256-komorka;} else {length_tmp = length;};
         
         //przepisanie strony pamiêci do RAMu
         ptrMem2=(char*)(page*256);
         for (short i=0; i<256; i++)
          {  
            tab.B[i]=ptrMem2[i];     
          }  
         
            //dodanie nowych danych
            for (int i=komorka; i<=komorka+length_tmp-1; i++)
            {
              tab.B[i]=value[data];
              data++;
            }
      
            //UptrMem=(int*)ptrMem;  
            UptrMem=(int*)(page*256);
              
            //zapis do tymczasowego obszar
            //for (unsigned int i=0; i<=63; i++) AT91C_IFLASH_MEM->MEM_PAGE_0[i]=tab_DW[i];
            for (unsigned int i=0; i<=63; i++) UptrMem[i]=tab.DW[i];
       
            //oczekiwanie na flagê FRDY 
            while((AT91C_BASE_MC->MC_FSR & AT91C_MC_FRDY) != AT91C_MC_FRDY);  
            //volatile unsigned int Status = AT91C_BASE_MC->MC_FSR;
            
            //Zapis FCMD i PAGENB do MC_FCR
            SET(AT91C_BASE_MC->MC_FCR,
                AT91C_MC_CORRECT_KEY
                | AT91C_MC_FCMD_START_PROG
                | ((page << 8) & AT91C_MC_PAGEN)); 
        
            //oczekiwanie na flagê FRDY 
            long long MaxCount=0;
            while(((AT91C_BASE_MC->MC_FSR & AT91C_MC_FRDY) != AT91C_MC_FRDY) || (MaxCount>0xFFFE)) {MaxCount++;};  
            if (MaxCount==0xFFFF)  
            {
              char mess[]="Blad zapisu do flash";
              MESSAGE(mess); 
            }
            //Sprawdzenie czy nie naruszona blokada regionu
            if (AT91C_BASE_MC->MC_FSR & AT91C_MC_LOCKE) 
            {
               MESSAGE("B³¹d naruszenia zablokowanego regionu flash"); 
            }
            
            //Sprawdzenie czy zapis wykonany poprawnie
            if (AT91C_BASE_MC->MC_FSR & AT91C_MC_PROGE) 
            {
              MESSAGE("Z³y klucz lub b³edna komenda zapisu flash"); 
            }
            
         komorka = 0;
         length = length-length_tmp;
         page++;
        
       }while (length>0); 
   }
  
}
*/    
    
    






//*-----------------------------------------------------------------------------
//* Nazwa funkcji :    FlashWrite
//* Funkcja zapisu do flash
//*-----------------------------------------------------------------------------
//*  ofset  - przesuniêcie
//*  cell   - pierwsza komórka zapisu
//*  ptrMem - wskaŸnik na komórke pamiêci flash
//*  value  - wartoœæ do zapisu
//*  length - wielkoœæ danych
//*-----------------------------------------------------------------------------
void FlashWrite( char *ptrMem, char *value, int length)
 {
   volatile int page    = (int)ptrMem/256;
   volatile int komorka = (int)ptrMem%256; 
   volatile int PtrInt  = (int)ptrMem;
   
   FlashWriteExe(PtrInt, page, komorka, value, length);
 } 
   


__ramfunc  void FlashWriteExe(volatile int  PtrInt,  volatile int  page, volatile int  komorka, char *value, int length)
 {

   volatile int length_tmp;
   volatile int data = 0; 
   volatile int *UptrMem;
   volatile char* ptrMem2;

   if ((PtrInt>(int)0x00140000) || (PtrInt<(int)0x00120000) || (komorka>0xFF))
   {
       char mess[]="Blad zapisu. Przekroczenie dozwolonego obszaru";
       MESSAGE(mess); 
   }else{
 
       union 
       {
                   char B[256];
          unsigned long DW[64];
       }tab;
    
       do
       {
         if ((komorka+length)>256) {length_tmp=256-komorka;} else {length_tmp = length;};
         
         //przepisanie strony pamiêci do RAMu
         ptrMem2=(char*)(page*256);
         for (short i=0; i<256; i++)
          {  
            tab.B[i]=ptrMem2[i];     
          }  
         
            //dodanie nowych danych
            for (int i=komorka; i<=komorka+length_tmp-1; i++)
            {
              tab.B[i]=value[data];
              data++;
            }
      
            //UptrMem=(int*)ptrMem;  
            UptrMem=(int*)(page*256);
              
            //zapis do tymczasowego obszar
            //for (unsigned int i=0; i<=63; i++) AT91C_IFLASH_MEM->MEM_PAGE_0[i]=tab_DW[i];
            for (unsigned int i=0; i<=63; i++) UptrMem[i]=tab.DW[i];
       
            //oczekiwanie na flagê FRDY 
            while((AT91C_BASE_MC->MC_FSR & AT91C_MC_FRDY) != AT91C_MC_FRDY);  
            //volatile unsigned int Status = AT91C_BASE_MC->MC_FSR;
            
            //Zapis FCMD i PAGENB do MC_FCR
            SET(AT91C_BASE_MC->MC_FCR,
                AT91C_MC_CORRECT_KEY
                | AT91C_MC_FCMD_START_PROG
                | ((page << 8) & AT91C_MC_PAGEN)); 
        
            //oczekiwanie na flagê FRDY 
            long long MaxCount=0;
            while(((AT91C_BASE_MC->MC_FSR & AT91C_MC_FRDY) != AT91C_MC_FRDY) || (MaxCount>0xFFFE)) {MaxCount++;};  
            if (MaxCount==0xFFFF)  
            {
              char mess[]="Blad zapisu do flash";
              MESSAGE(mess); 
            }
            //Sprawdzenie czy nie naruszona blokada regionu
            if (AT91C_BASE_MC->MC_FSR & AT91C_MC_LOCKE) 
            {
               MESSAGE("B³¹d naruszenia zablokowanego regionu flash"); 
            }
            
            //Sprawdzenie czy zapis wykonany poprawnie
            if (AT91C_BASE_MC->MC_FSR & AT91C_MC_PROGE) 
            {
              MESSAGE("Z³y klucz lub b³edna komenda zapisu flash"); 
            }
            
         komorka = 0;
         length = length-length_tmp;
         page++;
        
       }while (length>0); 
   }
    
}







__ramfunc void CleanFlash(char *ptrMem, int length)
 {
   int page=     (int)ptrMem/256;
   int komorka = (int)ptrMem%256;   
   int length_tmp;
   int data = 0; 
   int *UptrMem;

   union 
   {
               char B[256];
      unsigned long DW[64];
   }tab;

   do
   {
     if ((komorka+length)>256) {length_tmp=256-komorka;} else {length_tmp = length;};
     
     //przepisanie strony pamiêci do RAMu
     ptrMem=(char*)(page*256);
     for (short i=0; i<256; i++)
      {  
        tab.B[i]=ptrMem[i];     
      }  
     
        //dodanie nowych danych
        for (int i=komorka; i<=komorka+length_tmp-1; i++)
        {
          tab.B[i]=0;
          data++;
        }
  
        //UptrMem=(int*)ptrMem;  
        UptrMem=(int*)(page*256);
          
        //zapis do tymczasowego obszar
        //for (unsigned int i=0; i<=63; i++) AT91C_IFLASH_MEM->MEM_PAGE_0[i]=tab_DW[i];
        for (unsigned int i=0; i<=63; i++) UptrMem[i]=tab.DW[i];
   
        //oczekiwanie na flagê FRDY 
        while((AT91C_BASE_MC->MC_FSR & AT91C_MC_FRDY) != AT91C_MC_FRDY);  
        //volatile unsigned int Status = AT91C_BASE_MC->MC_FSR;
        
        //Zapis FCMD i PAGENB do MC_FCR
        SET(AT91C_BASE_MC->MC_FCR,
            AT91C_MC_CORRECT_KEY
            | AT91C_MC_FCMD_START_PROG
            | ((page << 8) & AT91C_MC_PAGEN)); 
    
        //oczekiwanie na flagê FRDY 
        while((AT91C_BASE_MC->MC_FSR & AT91C_MC_FRDY) != AT91C_MC_FRDY);  
        
        //Sprawdzenie czy nie naruszona blokada regionu
        if (AT91C_BASE_MC->MC_FSR & AT91C_MC_LOCKE) 
        {
          // MESSAGE("B³¹d naruszenia zablokowanego regionu flash"); 
        }
        
        //Sprawdzenie czy zapis wykonany poprawnie
        if (AT91C_BASE_MC->MC_FSR & AT91C_MC_PROGE) 
        {
          //MESSAGE("Z³y klucz lub b³edna komenda zapisu flash"); 
        }
        
     komorka = 0;
     length = length-length_tmp;
     page++;
    
   }while (length>0); 

    
}



/*
__ramfunc void CleanFlash(int ofset,unsigned int cell, int length)
  {
   int page=     cell/256;
   int komorka = cell%256;
   int length_tmp;
   int data = 0; 
   unsigned long *memory;
   bool find; 
   int tmp_i =0;
   
   
 //  TRACE_INFO("\n\r---  CleanFlash  --- Ofset: %d, Komorka: %d, Rozmar: %dB\n\r",ofset,cell,length );

   
   union 
   {
               char tab_B[256];
      unsigned long tab_DW[64];
   };

   do
   {
     if ((komorka+length)>256) {length_tmp=256-komorka;} else {length_tmp = length;};
     
     
      memory=(unsigned long*) (ofset+page*256);

     find = false;

      tmp_i=0;
      do 
      {
        if (memory[tmp_i]>0)
        {
          find = true;
          break;
        }
        tmp_i++;
      } while (tmp_i<=63);

      
     if (find==true)    
     {
        data=0;
        for (int i=komorka; i<=komorka+length_tmp-1; i++)
          {
            tab_B[i]=0;
            data++;
          }
  
        for (unsigned int i=0; i<=63; i++)
          {
            AT91C_IFLASH_MEM->MEM_PAGE_0[i]=tab_DW[i];
          }  
   
        volatile unsigned int Status = AT91C_BASE_MC->MC_FSR;

        SET(AT91C_BASE_MC->MC_FCR,
            AT91C_MC_CORRECT_KEY
            | AT91C_MC_FCMD_START_PROG
            | (((page+(ofset/256)) << 8) & AT91C_MC_PAGEN)); 
    
        while((AT91C_BASE_MC->MC_FSR & AT91C_MC_FRDY) != AT91C_MC_FRDY);   
      }
     
     komorka = 0;
     length = length-length_tmp;
     page++;
    
   }while (length>0); 
    
 //  TRACE_INFO("\n\r ---Koniec  CleanFlash  --- \n\r");
}

*/




 void at91flashErase(void)
 {
   // erase flash
     AT91C_BASE_MC->MC_FCR =(0x5A<<24) | AT91C_MC_FCMD_ERASE_ALL;
     // wait for flash done/ready
     while(!(AT91C_BASE_MC->MC_FSR & AT91C_MC_FRDY));

 }

