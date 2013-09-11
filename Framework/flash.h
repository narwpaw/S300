//flash.h

#ifndef _FLASH_H_
#define _FLASH_H_

#include "common.h"                                                 
#include "core/device.h"                                                        //do³¹cza bibliotreki do odpowiedniego procesora                                                      //³aduje bibliotreki dla odpowiedniego procesora
#include "Framework.h"
#include "core/common.h"
#include "core/trace.h"
#include "core/board.h"


#define __RAMFUNC __attribute__ ((long_call, section (".fastrun"))) 

void InitFlash(void);
void at91flashErase(void);
__ramfunc void at91flashWrite(  unsigned int ofset,  unsigned int cell,  char *value,  int length);
__ramfunc int AT91F_Flash_Write( unsigned int Flash_Address ,int size ,unsigned int * buff); 
//__ramfunc void CleanFlash(int ofset,unsigned int cell, int length);
__ramfunc void CleanFlash(char *ptrMem, int length);

//volatile void FlashWrite(volatile char *ptrMem, volatile char *value,volatile  int length);
void FlashWrite( char *ptrMem, char *value, int length);
__ramfunc  void FlashWriteExe(volatile int  PtrInt, volatile int  page, volatile int  komorka, char *value, int length);   
#endif
