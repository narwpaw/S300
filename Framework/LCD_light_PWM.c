       
#include "Framework.h"
#include "core/device.h"                                                        //aduje bibliotreki dla odpowiedniego procesora


#define MCK 48054841

// Include Standard files
//#include "Board.h"
/*  In this project we are assuming to have master clock equal to
47.9232 MHz
    on the AT91SAM7SEK.*/

/*    PWM Clock source Domain */
#define         MCKtoPWM      0
#define         MCK_2toPWM    1
#define         MCK_4toPWM    2
#define         MCK_8toPWM    3
#define         MCK_16toPWM   4
#define         MCK_32toPWM   5
#define         MCK_64toPWM   6
#define         MCK_128toPWM  7
#define         MCK_256toPWM  8
#define         MCK_512toPWM  9
#define         MCK_1024toPWM 10
#define         CLHAtoPWM     11
#define         CLHBtoPWM     12

#define         CALG_OFF       0
#define         CALG_ON       1<<8  // The period is center aligned(OFF, left aligned)

#define         CPOL_OFF      0<<0  // The ouput waveform starts at a high level (OFF, low level)
#define         CPOL_ON       1<<9  // The ouput waveform starts at a high level (OFF, low level)

#define         CPD_ON        1<<10 // The period will be modified at the next period start event
#define         CPD_OFF       0<<10

/* Select the wanted frequency in hertz.
This choice has to be in accordance with the selected clock source */
//#define   FREQUENCY    4792  // In Hertz
#define   FREQUENCY    500000  // In Hertz

/* In our example, we are targetting to get out 100 Hertz frequency. In
this goal, we use MCK_256toPWM */
#define   PWM_PERIOD_VALUE            (MCK/(1*FREQUENCY))




//ustaw wype³nienie w procentach
int Set_LCD_Light_PWM_Fill(char FillProcent)
{
   //AT91C_BASE_PWMC->PWMC_DIS = 0x1;
   AT91C_BASE_PWMC_CH0->PWMC_CDTYR = PWM_PERIOD_VALUE/(100/FillProcent);
   //AT91C_BASE_PWMC->PWMC_ENA = 0x1;
}






//*--------------------------------------------------------------------------------------
//* Function Name       : Main
//* Object              : Software entry point
//* Input Parameters    : none.
//* Output Parameters   : none.
//*--------------------------------------------------------------------------------------
int Init_LCD_Light_PWM(char Level)
{
        /* Valid the PWM's Clock at PMC level */
        AT91F_PMC_EnablePeriphClock(AT91C_BASE_PMC,1<<AT91C_ID_PWMC);

        /* Disable the PWM the channel 0 */
        AT91C_BASE_PWMC->PWMC_DIS = 0x1;

        /* Enabling a PWM output through the PA0: PWM Channel 0*/
        AT91F_PIO_CfgPeriph(AT91C_BASE_PIOA, AT91C_PA0_PWM0, 0);

        /* Period Updating */
        AT91C_BASE_PWMC_CH0->PWMC_CMR = (CPD_ON | CPOL_ON | CALG_ON | MCKtoPWM);
        AT91C_BASE_PWMC_CH0->PWMC_CPRDR=PWM_PERIOD_VALUE; 

        /* Duty Cycle Updating */
        AT91C_BASE_PWMC_CH0->PWMC_CMR = (CPD_OFF | CPOL_ON| CALG_ON | MCKtoPWM);
        Set_LCD_Light_PWM_Fill(100);//100% wype³nienia

        /* Interrupt enabling at peripheral level only in order to have synchronisation between Software and the UP-DATE Register (to avoid over writting) */
        AT91C_BASE_PWMC->PWMC_IER = 1;

        /* Enable the PWM the channel 0*/
        AT91C_BASE_PWMC->PWMC_ENA = 0x1;

}