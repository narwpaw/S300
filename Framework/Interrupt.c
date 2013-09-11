//****************************************************************************//
//*--------------------------------------------------------------------------*//
//*                                 CEURON                                   *//                     
//*--------------------------------------------------------------------------*//
//*--------------------------------------------------------------------------*//
//*  Nazwa pliku                  : Interrupt.c                              *//
//*  Opis                         : Plik przerwañ                            *//
//*  Autor                        : Pawe³ Narwojsz                           *//
//*  Data utrzorzenia             : 22.02.2012                               *//
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


#include  "Interrupt.h"
    #include <intrinsics.h>


char VBUS_old;    
void PioInterrupt(void)
{

    unsigned int status = AT91F_PIO_GetInterruptStatus(PIO_RfIntBUS);

   // static char VBUS_old; 
    //static char RfIntBUS_old; 

    



    if (status & AT91C_VBUS) 
    {
       /*
      if ((!((AT91C_BASE_PIOA->PIO_PDSR) & AT91C_VBUS)) && (VBUS_old==1))
       {
         VBUS_old=0;
         ISR_VBus();
       }
        */
       if (((AT91C_BASE_PIOA->PIO_PDSR) & AT91C_VBUS) && (VBUS_old==0))
       {
         VBUS_old=1;
         ISR_VBus();
       }
      
    }
    
    
    if (status & RfIntBUS)
    {  
      if ((( AT91C_BASE_PIOA->PIO_PDSR) & RfIntBUS)==0)  
      {

         RF_Int();

      }
    }



}





void InitPioInterrupt(void)
{
         //ConfigureRFBus();
        AT91F_PMC_EnablePeriphClock(AT91C_BASE_PMC, 1 << ID_RfIntBUS);
         
       // Configure and enable the Vbus detection interrupt
        AT91F_AIC_ConfigureIt(AT91C_BASE_AIC,
                          AT91C_ID_PIOA,
                          PIO_INT_PRIOR, //AT91C_AIC_PRIOR_LOWEST, AT91C_AIC_PRIOR_HIGHEST
                          0,//AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL,
                          PioInterrupt);  
        
        
        AT91F_PIO_InterruptEnable(AT91C_PIO_VBUS, AT91C_VBUS);
    AT91F_AIC_EnableIt(AT91C_BASE_AIC, AT91C_ID_VBUS);
      
}







