/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include <stdio.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h"
#include "../Myfunctions/Kurama.h"
#include "../Myfunctions/functions.h"
#include "../music/music.h"


/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile uint8_t hr = 0;
volatile uint8_t min = 0;
volatile uint8_t sec = 0;
volatile uint8_t countB = 3;
volatile uint8_t countH = 3;
extern uint8_t reset;
extern uint8_t meal;
extern uint8_t snack;
extern uint8_t volume;
extern uint8_t touch_done;
extern uint8_t VChanged;

uint16_t SinTable[45] =                                     
{
	  136, 155, 174, 191, 208, 224, 237, 249, 259, 
		266, 270, 272, 272, 268, 262, 254, 243, 231, 
		216, 200, 183, 164, 145, 126, 107,  89, 72,
		56,  41,  28,  18,   9,   3,   1,   0,   2,   
		6,  13,  23, 34,  48,  64,  80,  98, 117
};

void TIMER0_IRQHandler (void) {
	static int anim;
	static int act_sec;
	
	if(VChanged==1){
		if(volume==3){
			LCD_DrawRectangle (218, 240, 3, 6, Black);
			LCD_DrawRectangle (224, 237, 3, 12, Black);
			LCD_DrawRectangle (230, 233, 3, 18, Black);
			VChanged=0;
		}else if(volume==2){
			LCD_DrawRectangle (218, 240, 3, 6, Black);
			LCD_DrawRectangle (224, 237, 3, 12, Black);
			LCD_DrawRectangle (230, 233, 3, 18, White);
			VChanged=0;
		}else if(volume==1){
			LCD_DrawRectangle (218, 240, 3, 6, Black);
			LCD_DrawRectangle (224, 237, 3, 12, White);
			LCD_DrawRectangle (230, 233, 3, 18, White);
			VChanged=0;
		}else if(volume==0){
			LCD_DrawRectangle (218, 240, 3, 6, White);
			LCD_DrawRectangle (224, 237, 3, 12, White);
			LCD_DrawRectangle (230, 233, 3, 18, White);
			VChanged=0;
		}
	}
	
	if(countB !=0 && countH!=0){// if the pet is alive
		if(touch_done==2){
				RedHeart(50, 110);
				RedHeart(160, 110);
				RedHeart(105, 100);
				RedHeart(15, 100);
				RedHeart(195, 100);
				CloseEye(104,181);
				act_sec=sec;
				touch_done--;
		}else if(sec == act_sec+2 && touch_done==1){
				LCD_DrawRectangle (50, 110, 30, 30, White);
				LCD_DrawRectangle (160, 110, 30, 30, White);
				LCD_DrawRectangle (105, 100, 30, 30, White);
				LCD_DrawRectangle (15, 100, 30, 30, White);
				LCD_DrawRectangle (195, 100, 30, 30, White);
				OpenEye(104,183);
				touch_done=0;
			
			if(countH<3){
					countH++;
					if(countH == 3)
					RedHeart( 76, 60 );
				else if(countH == 2)
					RedHeart( 43, 60 );
				}
			}
		if(snack == 0 && meal == 0){
			if(sec%2==0)
				Animation1( 70, 135 );
			else
				Animation0( 70, 135 );
		} else if(snack==1 && anim == 2){
			LCD_DrawRectangle (150, 135, 50, 100, White);
			Walk2Kurama(50, 135);
			anim--;
			}else if(snack==1 && anim == 1){
				LCD_DrawRectangle (10, 200, 30, 30, White);
					countH++;
					if(countH == 3)
						RedHeart( 76, 60 );
					else if(countH == 2)
						RedHeart( 43, 60 );
					LCD_DrawRectangle (50, 135, 20, 100, White);
					DrawKurama( 70, 135 );
					GUI_Text(160, 280, (uint8_t *) "Snack", Black, White);
					anim--;
					snack=0;
				}else if(snack==1){
					// animation for snack
					anim=2;
					DrawOnigiri( 10, 200 );
					Walk1Kurama(70, 135 );
				}else if(meal==1 && anim == 2){
					LCD_DrawRectangle (150, 135, 50, 100, White);
					Walk2Kurama(50, 135);
					anim--;
				}else if(meal==1 && anim == 1){
					EmptyBowl( 10, 200 );
					countB++;
					if(countB == 3)
						FullBowl( 200, 60 );
					else if(countB == 2)
						FullBowl( 170, 60 );
					LCD_DrawRectangle (50, 135, 20, 100, White);
					DrawKurama( 70, 135 );  //x70->170 y135->235
					LCD_DrawRectangle (10, 200, 30, 30, White);
					GUI_Text(44, 280, (uint8_t *) "Meal", Black, White);
					meal=0;
					anim--;
				} else if(meal==1){
					//animation for meal
					anim=2;
					FullBowl( 10, 200 );
					Walk1Kurama(70, 135 );
				}
	}
  LPC_TIM0->IR |= 1;			// clear interrupt flag
	return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
	static int drawn = 0;
	if(reset==1){  // if the reset button is pressed, the tamagotchi starts again
		GUI_Text(56, 115, (uint8_t *) "                ", White, White);
		GUI_Text(100, 280, (uint8_t *) "     ", White, White);
		countB=3;
		countH=3;
		init_screen();
		hr=0;
		min=0;
		sec=0;
		reset=0;
		drawn=0;
		LPC_TIM1->IR |= 1;			// clear interrupt flag
	  return;
	}
		
	if(countB !=0 && countH!=0){	
		char timechar[50];
		sec++;
		sprintf(timechar, "%d", sec);
	
		if(sec<10){
			GUI_Text(164, 10, (uint8_t *) timechar, Black, White);
		}else if (sec <60)
			GUI_Text(156, 10, (uint8_t *) timechar, Black, White);
		else {
			sec=0;
			min++;
			sprintf(timechar, "%d:0%d", min, sec);
			if(min<10){
				GUI_Text(140, 10, (uint8_t *) timechar, Black, White);
			}else if (min <60)
				GUI_Text(132, 10, (uint8_t *) timechar, Black, White);
			else {
				sec=0;
				min=0;
				hr++;
				sprintf(timechar, "%d:0%d:0%d", hr, min, sec);
				if(hr<10){
					GUI_Text(116, 10, (uint8_t *) timechar, Black, White);
				}else
					GUI_Text(108, 10, (uint8_t *) timechar, Black, White);
			}
		}
  
		if(sec%8==0){
			if(countH == 3){
				BlackHeart( 76, 60 );
				countH--;
			}else if(countH == 2){
				BlackHeart( 43, 60 );
				countH--;
			}else{
				BlackHeart( 10, 60 );
				countH--;
			}
			if(countB == 3){
				EmptyBowl( 200, 60 );
				countB--;
			}else if(countB == 2){
				EmptyBowl( 170, 60 );
				countB--;
			}else{
				EmptyBowl( 140, 60 );
				countB--;
			}
		}
	}else{
		if (drawn==0){
		GUI_Text(56, 115, (uint8_t *) "Kurama left you!", Black, White);
		DeadKurama( 70, 135 );
		GUI_Text(44, 280, (uint8_t *) "    ", White, White);
		GUI_Text(100, 280, (uint8_t *) "Reset", Black, White);
		GUI_Text(160, 280, (uint8_t *) "     ", White, White);
		drawn=1;
		}
	}
	
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}


void TIMER2_IRQHandler (void)
{
	disable_timer(3);
	LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}

void TIMER3_IRQHandler (void)
{
	/* DAC management */	
	static int sineticks=0;
	LPC_DAC->DACR = (SinTable[sineticks]*volume) <<6;
	sineticks++;
	if(sineticks==45) sineticks=0;
  LPC_TIM3->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
