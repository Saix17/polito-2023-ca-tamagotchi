/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../GLCD/GLCD.h"
#include "../Myfunctions/functions.h"
#include "../Myfunctions/Kurama.h"
#include "../TouchPanel/TouchPanel.h"
#include "../adc/adc.h"
#include "../timer/timer.h"
#include "../music/music.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

//volatile int left=0;
//volatile int right=0;	
#define UPTICKS 1

extern uint8_t countH;
extern uint8_t countB;
volatile uint8_t reset = 0;
volatile uint8_t meal = 0;
volatile uint8_t snack = 0;
volatile uint8_t left=0;
volatile uint8_t right=0;
volatile uint8_t touch_done;

NOTE endsong[] = 
{
	{f4, time_semicroma},
  {e4, time_semicroma},
  {b3, time_croma},
  {b3, time_semiminima},
  {pause, time_semicroma},
  {e4, time_croma},
  {g4, time_croma},
  {a4, time_semiminima},
  {e4, time_minima},
  {pause, time_semicroma},
	{g4, time_semiminima},
	{g4, time_croma},
  {f4, time_croma},
	{g4, time_croma},
  {a4, time_croma},
  {d4, time_croma},
  {b4, time_minima},
  {a4, time_semiminima},
  {f4, time_semiminima}
};

NOTE cuddlesong[] = 
{
	{c4, time_croma},
  {f4, time_croma},
  {g4, time_croma},
	{pause, time_croma},
  {a4, time_minima},
  {g4, time_semiminima},
  {f4, time_minima},
  {c4, time_croma},
  {f4, time_croma},
	{g4, time_croma},
	{a4, time_minima},
  {b4, time_semiminima},
	{a4, time_croma},
  {b4, time_semiminima},
  {c5, time_semiminima},
};

NOTE eatsong[] = 
{ 
	{c4, time_croma},
  {a4, time_croma},
  {b4, time_croma}
};

void RIT_IRQHandler (void)
{	
	static int eatsound =0;
	static int cudsound = 0;
	static uint8_t food=0;
  static uint8_t currentNote = 0;
	static uint8_t ticks = 0;
	ADC_start_conversion();
	
	if(countH != 0 && countB != 0){
		if(!isNotePlaying() && eatsound==1 && food!=0){
		++ticks;
		if(ticks == UPTICKS)
		{
			ticks = 0;
			playNote(eatsong[currentNote]);
			currentNote++;
		}
		if(currentNote == (sizeof(eatsong) / sizeof(eatsong[0])))
	{
		eatsound=0;
		currentNote=0;
	}
	}else if(!isNotePlaying() && cudsound==1){
		++ticks;
		if(ticks == UPTICKS)
		{
			ticks = 0;
			playNote(cuddlesong[currentNote]);
			currentNote++;
		}
		if(currentNote == (sizeof(cuddlesong) / sizeof(cuddlesong[0])))
	{
		cudsound=0;
		currentNote=0;
	}
}
		if(meal == 0 && snack == 0){
			if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	
			// Joytick LEFT pressed
				right=0;
				left=1;
				switch(left){
					case 1:
						GUI_Text(44, 280, (uint8_t *) "Meal", White, Black);
						GUI_Text(160, 280, (uint8_t *) "Snack", Black, White);
						break;
					default:
						break;
					}
			}
	
		if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	
			// Joytick Right pressed
			left=0;
			right=1;
			switch(right){
				case 1:
					GUI_Text(44, 280, (uint8_t *) "Meal", Black, White);
					GUI_Text(160, 280, (uint8_t *) "Snack", White, Black);
					break;
				default:
					break;
			}
		}
	
		if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){
			// Joytick Select pressed
			if(!isNotePlaying()){
			reset_timer(3);
		  init_timer(3, 2000);
		  enable_timer(3);
			reset_timer(2);
		  init_timer(2, time_croma);
		  enable_timer(2);
			}
			if(left == 1 && countB < 3)
				food = 1;
			else if(right == 1 && countH < 3)
				food = 2;
			switch(food){
				case 1:
					meal=1;
					break;
				case 2:
					snack=1;
					break;
				default:
					break;
			}
			eatsound=1;
		}
	}
		
	if(getDisplayPoint(&display, Read_Ads7846(), &matrix )){
			if(display.y < 235 && display.y> 135  && display.x > 70 && display.x < 170){
				touch_done=2;
				cudsound=1;
			}
			
	}

	}else {
		if(!isNotePlaying()){
		++ticks;
		if(ticks == UPTICKS)
		{
			ticks = 0;
			playNote(endsong[currentNote]);
			currentNote++;
		}
	}
		if(currentNote == (sizeof(endsong) / sizeof(endsong[0])))
	{
		currentNote=0;
	}
		if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){
		// Joytick Select pressed
		static int select=1;
		if(!isNotePlaying()){
			reset_timer(3);
		  init_timer(3, 2000);
		  enable_timer(3);
			reset_timer(2);
		  init_timer(2, time_croma);
		  enable_timer(2);
			}
		switch(select){
			case 1:
				left=0;
			  right=0;
			  reset=1;
				currentNote=0;
				eatsound=0;
				cudsound=0;
				break;
			default:
				break;
		}
	}
		}
	LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
