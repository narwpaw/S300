/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support  -  ROUSSET  -
 * ----------------------------------------------------------------------------
 * Copyright (c) 2006, Atmel Corporation

 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaiimer below.
 *
 * - Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the disclaimer below in the documentation and/or
 * other materials provided with the distribution.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

/*
$Id: board.h,v 1.1.2.1 2006/12/05 08:33:25 danielru Exp $
*/

  #include "LCD.h"



#ifndef _BOARD_H
#define _BOARD_H

//------------------------------------------------------------------------------
//      Definitions
//------------------------------------------------------------------------------

// Board revisions
#define REV_A           0
#define REV_B           1

// Default revision is REV_B
#ifndef REVISION
    #define REVISION    REV_B
#endif

// Slow clock used at startup (32kHz)
#define SLOWCLOCK       32768

// Main clock
#if defined(AT91SAM7SEK)  || defined(AT91SAM7XEK) || \
    defined(AT91SAM7A3EK) || defined(AT91SAM7SEEK)

    #define AT91C_MASTER_CLOCK      48000000

#elif defined(AT91RM9200)
    #define AT91C_BAUDRATE            19200
    #define AT91C_MASTER_CLOCK      60000000
    #define PLLAR   0x20263E04 // 179,712000 MHz for PCK
    #define PLLBR   0x10483E0E // 48,054857 MHz (divider by 2 for USB) // NAT first tests can be done with this config
                               //    MUL= 72 0x48 DIV=14 0xE
    #define PLLBR1  0x102E3E09 // 48,054857 MHz (divider by 2 for USB) // BENOIT suggestion
                               //     MUL= 46 0x2E DIV=0x9
    #define PLLBR2  0x10483E0E // 48,054857 MHz (divider by 2 for USB)
    #define MCKR    0x00000202 // PCK/3 = MCK Master Clock = 59,904000MHz with PLLA selected

#elif defined(AT91SAM9260EK) || defined(AT91SAM9261EK) || defined(AT91SAM9263EK)
    #define AT91C_MASTER_CLOCK      99300000
    #define SDRAM_BASE_ADDR         0x20000000
    #define PLLBR   0x10483E0E // 48,054857 MHz (divider by 2 for USB) // NAT first tests can be done with this config
                               //    MUL= 72 0x48 DIV=14 0xE
#else
    #error core_board.h Main clock
#endif





#if defined(AT91SAM7SEK)

                         // AT91C_PIO_PA3  // DS4
    #define LED_PIO         AT91C_BASE_PIOA
    #define LED_PIO_ID      AT91C_ID_PIOA

#elif defined(AT91SAM7A3EK)
    #define LED_POWER       AT91C_PIO_PA20  // DS1
    #define LED_USB         AT91C_PIO_PA21  // DS2
    #define LED_MEM         AT91C_PIO_PA24  // DS3
                         // AT91C_PIO_PA25  // DS4
    #define LED_PIO         AT91C_BASE_PIOA
    #define LED_PIO_ID      AT91C_ID_PIOA

#elif defined(AT91SAM7XEK)
    #define LED_POWER       AT91C_PIO_PB19  // DS1
    #define LED_USB         AT91C_PIO_PB20  // DS2
    #define LED_MEM         AT91C_PIO_PB21  // DS3
                         // AT91C_PIO_PB22  // DS4
    #define LED_PIO         AT91C_BASE_PIOB
    #define LED_PIO_ID      AT91C_ID_PIOB

#elif defined(AT91SAM7SEEK)
    #define LED_POWER       AT91C_PIO_PA0   // Yellow PowerLED
    #define LED_USB         AT91C_PIO_PA1   // Green  DS1
    #define LED_MEM         AT91C_PIO_PA2   // Green  DS2
    #define LED_PIO         AT91C_BASE_PIOA
    #define LED_PIO_ID      AT91C_ID_PIOA

#elif defined(AT91RM9200EK)
    #define LED_POWER       AT91C_PIO_PB0   // Green  DS2
    #define LED_USB         AT91C_PIO_PB1   // Yellow DS4
    #define LED_MEM         AT91C_PIO_PB2   // Red    DS6
    #define LED_PIO         AT91C_BASE_PIOB
    #define LED_PIO_ID      AT91C_ID_PIOB

#elif defined(AT91SAM9260EK)
    #define LED_POWER       AT91C_PIO_PA9    // Only 1 led
    #define LED_USB         AT91C_PIO_PA6    // DS5
    #define LED_MEM         AT91C_PIO_PA9    // Only 1 led
    #define LED_PIO         AT91C_BASE_PIOA
    #define LED_PIO_ID      AT91C_ID_PIOA

#elif defined(AT91SAM9261EK)
    #define LED_POWER       AT91C_PIO_PA23  // Power LED
  //  #define LED_USB         AT91C_PIO_PA13  // DS8
    #define LED_MEM         AT91C_PIO_PA14  // DS7
    #define LED_PIO         AT91C_BASE_PIOA
    #define LED_PIO_ID      AT91C_ID_PIOA

#elif defined(AT91SAM9263EK)
// PWM1
// PWM2

    // theses values only for compil:
    #define LED_POWER       AT91C_PIO_PA23
 //   #define LED_USB         AT91C_PIO_PA13
    #define LED_MEM         AT91C_PIO_PA14
    #define LED_PIO         AT91C_BASE_PIOA
    #define LED_PIO_ID      AT91C_ID_PIOA
#else
#error message core_board.h led
#endif

// Switches
#if defined(AT91SAM7SEK)
    #define SWITCH1        AT91C_PIO_PA19  // BP1
    #define SWITCH2        AT91C_PIO_PA20  // BP2
    #define SWITCH3        AT91C_PIO_PA15  // BP3
    #define SWITCH4        AT91C_PIO_PA14  // BP4
    #define SWITCH_PIO     AT91C_BASE_PIOA
    #define SWITCH_PIO_ID  AT91C_ID_PIOA

#elif defined(AT91SAM7A3EK)
    #define SWITCH1        AT91C_PIO_PB8    // Joystick UP
    #define SWITCH2        AT91C_PIO_PB12   // Joystick LEFT
    #define SWITCH3        AT91C_PIO_PB13   // Joystick RIGHT
    #define SWITCH4        AT91C_PIO_PB9    // Joystick DOWN
    #define SWITCH5        AT91C_PIO_PB14   // Joystick PUSH
    #define SWITCH_PIO     AT91C_BASE_PIOB
    #define SWITCH_PIO_ID  AT91C_ID_PIOB

#elif defined(AT91SAM7XEK)
    #define SWITCH1        AT91C_PIO_PA21  // Joystick UP
    #define SWITCH2        AT91C_PIO_PA23  // Joystick LEFT
    #define SWITCH3        AT91C_PIO_PA24  // Joystick RIGHT
    #define SWITCH4        AT91C_PIO_PA22  // Joystick DOWN
    #define SWITCH5        AT91C_PIO_PA25  // Joystick PUSH
    #define SWITCH_PIO     AT91C_BASE_PIOA
    #define SWITCH_PIO_ID  AT91C_ID_PIOA

#elif defined(AT91SAM7SEEK)
    #define SWITCH1        AT91C_PIO_PB23 // Joystick UP
    #define SWITCH2        AT91C_PIO_PB24 // Joystick DOWN
    #define SWITCH3        AT91C_PIO_PB26 // Joystick RIGHT
    #define SWITCH4        AT91C_PIO_PB27 // Joystick LEFT
    #define SWITCH5        AT91C_PIO_PB25 // Push Left Clic : PB25 : BP1
    #define SWITCH6        AT91C_PIO_PB22 // Right Clic : PB22 : BP2
    #define SWITCH_PIO     AT91C_BASE_PIOB
    #define SWITCH_PIO_ID  AT91C_ID_PIOB

#elif defined(AT91RM9200EK)
    // no button on the board
    #define SWITCH1        AT91C_PIO_PB23 // NO BUTTON
    #define SWITCH2        AT91C_PIO_PB24 // ONLY for COMPIL
    #define SWITCH3        AT91C_PIO_PB26 // ONLY for COMPIL
    #define SWITCH4        AT91C_PIO_PB27 // ONLY for COMPIL
    #define SWITCH_PIO     AT91C_BASE_PIOB// ONLY for COMPIL
    #define SWITCH_PIO_ID  AT91C_ID_PIOB

#elif defined(AT91SAM9260EK)
    #define SWITCH1        AT91C_PIO_PA30 // BP3
    #define SWITCH2        AT91C_PIO_PA31 // BP4
    #define SWITCH3        AT91C_PIO_PA30 // Only 2 BP
    #define SWITCH4        AT91C_PIO_PA31 // Only 2 BP
    #define SWITCH_PIO     AT91C_BASE_PIOA
    #define SWITCH_PIO_ID  AT91C_ID_PIOA

#elif defined(AT91SAM9261EK)
    #define SWITCH1        AT91C_PIO_PA27 // BP3
    #define SWITCH2        AT91C_PIO_PA26 // BP4
    #define SWITCH3        AT91C_PIO_PA25 // BP5
    #define SWITCH4        AT91C_PIO_PA24 // BP6
    #define SWITCH_PIO     AT91C_BASE_PIOA
    #define SWITCH_PIO_ID  AT91C_ID_PIOA

#elif defined(AT91SAM9263EK)
// PC4
// PC4

#elif defined(MISTRALREVB)
    #define SWITCH1        AT91C_PIO_PA21
    #define SWITCH2        AT91C_PIO_PA23
    #define SWITCH3        AT91C_PIO_PA24
    #define SWITCH4        AT91C_PIO_PA22
    #define SWITCH_PIO     AT91C_BASE_PIOA
    #define SWITCH_PIO_ID  AT91C_ID_PIOA

#else
    #error core_board.h switch
#endif


#if !defined(IR_BUS_DEF)
    #define IR_BUS              AT91C_PIO_PA15
    #define IR_PIO_BUS          AT91C_BASE_PIOA
    #define IR_ID_BUS           AT91C_ID_PIOA
#endif // !defined(IR_BUS_DEF)



// Bus power
// If USB_BUS_POWERED if defined, device is powered through VBUS
// VBUS PIO
#if !defined(USB_BUS_POWERED)
    #if defined(AT91SAM7SEK)


          #define AT91C_VBUS              AT91C_PIO_PA1    //dla MSU101
         // #define AT91C_VBUS              AT91C_PIO_PA8    //dla S300
        //#define AT91C_VBUS              AT91C_PIO_PA13   //dla makiety
       // #define AT91C_VBUS              AT91C_PIO_PA24   //dla makiety

    //    #define 1H_BUS   AT91C_PIO_PA0    //

        #define AT91C_PIO_VBUS          AT91C_BASE_PIOA
        #define AT91C_ID_VBUS           AT91C_ID_PIOA

    #elif defined(AT91SAM7SEEK)
        #define AT91C_VBUS              AT91C_PIO_PC19
        #define AT91C_PIO_VBUS          AT91C_BASE_PIOC
        #define AT91C_ID_VBUS           AT91C_ID_PIOC

    #elif defined(AT91SAM7XEK) || defined(AT91SAM7A3EK)
        #define USB_BUS_POWERED

    #elif defined(AT91RM9200EK)
        #define AT91C_VBUS              AT91C_PIO_PD4
        #define AT91C_PIO_VBUS          AT91C_BASE_PIOD
        #define AT91C_ID_VBUS           AT91C_ID_PIOD

    #elif defined(AT91SAM9260EK)
        #define AT91C_VBUS              AT91C_PIO_PC5
        #define AT91C_PIO_VBUS          AT91C_BASE_PIOC
        #define AT91C_ID_VBUS           AT91C_ID_PIOC

    #elif defined(AT91SAM9261EK)
        #define AT91C_VBUS              AT91C_PIO_PB29
        #define AT91C_PIO_VBUS          AT91C_BASE_PIOB
        #define AT91C_ID_VBUS           AT91C_ID_PIOB

    #elif defined(AT91SAM9263EK)
        #define AT91C_VBUS              AT91C_PIO_PA25
        #define AT91C_PIO_VBUS          AT91C_BASE_PIOA
        #define AT91C_ID_VBUS           AT91C_ID_PIOA

    #else
        #error core_board.h BUS POWERED
    #endif
#endif // !defined(USB_BUS_POWERED)

// Pull-ups
#if defined(AT91SAM7SEK)
    #define AT91C_PULLUP            AT91C_PIO_PA16
    #define AT91C_PIO_PULLUP        AT91C_BASE_PIOA
    #define AT91C_ID_PULLUP         AT91C_ID_PIOA

#elif defined(AT91SAM7A3EK)
    #define AT91C_PULLUP            AT91C_PIO_PB1
    #define AT91C_PIO_PULLUP        AT91C_BASE_PIOB
    #define AT91C_ID_PULLUP         AT91C_ID_PIOB

#elif defined(AT91RM9200)
    #define AT91C_PULLUP            AT91C_PIO_PD5
    #define AT91C_PIO_PULLUP        AT91C_BASE_PIOD
    #define AT91C_ID_PULLUP         AT91C_ID_PIOD

#endif

// Internal flash wait states
#if defined(AT91SAM7SEK) || defined(AT91SAM7XEK) || defined(AT91SAM7A3EK)
    #define AT91C_FLASH_WAIT_STATES AT91C_MC_FWS_1FWS

#elif defined(AT91SAM7SEEK)
    #define AT91C_FLASH_WAIT_STATES AT91C_MC_FWS_2FWS
#endif

//------------------------------------------------------------------------------
//      Internal functions
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//! \brief  Sets the correct number of wait states in the flash controller
//------------------------------------------------------------------------------
extern inline void BRD_SetFlashWaitStates()
{
#if defined(AT91C_BASE_FLASH)
    SET(AT91C_BASE_FLASH->FLA_FMR, AT91C_FLASH_WAIT_STATES);
#endif
}

//------------------------------------------------------------------------------
//! \brief  Lights on specified LED.
//! \param  dLed Pio ID of LED to light on
//------------------------------------------------------------------------------
extern inline void AT91F_LED_On(unsigned int dLed)
{
    AT91F_PIO_ClearOutput(LED_PIO, dLed);
}

//------------------------------------------------------------------------------
//! \brief  Lights off specified LED.
//! \param  dLed Pio ID of LED to light off
//------------------------------------------------------------------------------
extern inline void AT91F_LED_Off(unsigned int dLed)
{
    AT91F_PIO_SetOutput(LED_PIO, dLed);
}

//------------------------------------------------------------------------------
//! \brief  Initializes the PIO for LED control
//------------------------------------------------------------------------------



inline void AT91F_LCD_Init()
{
    AT91F_PMC_EnablePeriphClock(AT91C_BASE_PMC, 1 << LCD_PIO_ID);
    AT91F_PIO_CfgOutput(LED_PIO, DI);
    AT91F_PIO_CfgOutput(LED_PIO, ENABLE);
    AT91F_PIO_CfgOutput(LED_PIO, DB0);
    AT91F_PIO_CfgOutput(LED_PIO, DB1);
    AT91F_PIO_CfgOutput(LED_PIO, DB2);
    AT91F_PIO_CfgOutput(LED_PIO, DB3);
    AT91F_PIO_CfgOutput(LED_PIO, DB4);
    AT91F_PIO_CfgOutput(LED_PIO, DB5);
    AT91F_PIO_CfgOutput(LED_PIO, DB6);
    AT91F_PIO_CfgOutput(LED_PIO, DB7);
    AT91F_PIO_CfgOutput(LED_PIO, CS1);
    AT91F_PIO_CfgOutput(LED_PIO, CS2);
    AT91F_PIO_CfgOutput(LED_PIO, LightLCD);
                
     LED_PIO->PIO_SODR =  LCD_BUS_MASK | DI  | ENABLE; 

}


//------------------------------------------------------------------------------
//! \brief  Toggle the status of specified LED
//! \param  dLed Pio ID of LED
//------------------------------------------------------------------------------
extern inline void AT91F_LED_Toggle(unsigned int dLed)
{
    if (ISSET(AT91F_PIO_GetInput(LED_PIO), dLed)) {

        AT91F_PIO_ClearOutput(LED_PIO, dLed);
    }
    else {

        AT91F_PIO_SetOutput(LED_PIO, dLed);
    }
}

//------------------------------------------------------------------------------
//! \brief  Initializes the PIO for BUTTON control
//------------------------------------------------------------------------------
extern inline void AT91F_BUTTON_Init()
{
    AT91F_PMC_EnablePeriphClock(AT91C_BASE_PMC, 1 << SWITCH_PIO_ID);
    AT91F_PIO_CfgInput(SWITCH_PIO, SWITCH1);
    AT91F_PIO_CfgInput(SWITCH_PIO, SWITCH2);
    AT91F_PIO_CfgInput(SWITCH_PIO, SWITCH3);
    AT91F_PIO_CfgInput(SWITCH_PIO, SWITCH4);
#if defined(AT91SAM7SEK) || defined(AT91SAM9261EK) || defined(AT91SAM9260EK)
    AT91F_PIO_CfgPullup(SWITCH_PIO, SWITCH1 | SWITCH2 | SWITCH3 | SWITCH4);
#elif defined(AT91SAM7XEK) || defined(AT91SAM7A3EK)
    AT91F_PIO_CfgInput(SWITCH_PIO, SWITCH5);
    AT91F_PIO_CfgPullup(SWITCH_PIO, SWITCH1 | SWITCH2 | SWITCH3 | SWITCH4 | SWITCH5);
#elif defined(AT91SAM7SEEK)
    AT91F_PIO_CfgInput(SWITCH_PIO, SWITCH5 | SWITCH6);
    AT91F_PIO_CfgPullup(SWITCH_PIO, SWITCH1 | SWITCH2 | SWITCH3 | SWITCH4 | SWITCH5 |\
                        SWITCH6);
#endif
}

//------------------------------------------------------------------------------
//      Macros
//------------------------------------------------------------------------------
#if defined(NOLEDS)
    #define LED_INIT()
    #define LED_ON(led)
    #define LED_OFF(led)
    #define LED_TOGGLE(led)
#else
    #define LED_INIT()      AT91F_LED_Init()
    #define LED_ON(led)     AT91F_LED_On(led)
    #define LED_OFF(led)    AT91F_LED_Off(led)
    #define LED_TOGGLE(led) AT91F_LED_Toggle(led)
    
    
    
#endif

  #define LCD_INIT()      AT91F_LCD_Init() 



//------------------------------------------------------------------------------
//      Prototypes
//------------------------------------------------------------------------------

extern void BRD_ConfigureVBus(void *pInterface);
extern bool BRD_IsVBusConnected(void *pInterface);
extern void BRD_ConfigurePullUp(void *pInterface);
extern void BRD_ConnectPullUp(void *pInterface);
extern void BRD_DisconnectPullUp(void *pInterface);
extern bool BRD_IsPullUpConnected(void *pInterface);

#endif // _BOARD_H

