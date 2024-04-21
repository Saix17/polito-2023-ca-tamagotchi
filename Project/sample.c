/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "timer/timer.h"
#include "Myfunctions/functions.h"
#include "RIT/RIT.h"
#include "joystick/joystick.h"
#include "TouchPanel/TouchPanel.h"
#include "adc/adc.h"


#define SIMULATOR 1

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

int main(void)
{
  SystemInit();  												/* System Initialization (i.e., PLL)  */
	
  LCD_Initialization();                 /* LCD Inizialization */
	TP_Init();
	TouchPanel_Calibrate();
	LCD_Clear(White);
	ADC_init();
	init_screen();												/* It loads what needs to appear in the screen at the start */
	//init_timer(2, 0xC8);
	//init_timer(2, 0xCCC ); 						    /* 8us * 25MHz = 200 ~= 0xC8 */
	//enable_timer(2);
	init_timer(1, 0x017D7840);         /* 1 sec timer1 initialization
	//init_timer(1, 0x00100000); 						 Approximated timer initialization for the simulator */
	enable_timer(1);
	init_timer(0, 0x017D7840);         /* 1 sec timer0 initialization
	//init_timer(0, 0x100000); 						   Approximated timer for the simulator */
	enable_timer(0);
	
	joystick_init();											/* Joystick Initialization            */
	init_RIT(0x004C4B40);		          		/* RIT Initialization 50 msec       	*/
	//init_RIT(0x000CCCCC);									/* approximated RIT Initialization for the simulator */
	enable_RIT();													/* RIT enabled												*/
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);

	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);
  while (1)	
  {
		__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
