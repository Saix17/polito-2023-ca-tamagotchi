#include "functions.h"
#include "../GLCD/GLCD.h"
#include "../Kurama.h"

void init_screen(void){	
	GUI_Text(68, 10, (uint8_t *) "Age: 00:00:00", Black, White);
	GUI_Text(20, 40, (uint8_t *) "Happiness", Black, White);
	GUI_Text(155, 40, (uint8_t *) "Satiety", Black, White);
	RedHeart( 10, 60 );
	RedHeart( 43, 60 );
	RedHeart( 76, 60 );
	FullBowl( 140, 60 );
	FullBowl( 170, 60 );
	FullBowl( 200, 60 );
	DrawKurama( 70, 135 );
	DrawSpeaker(200, 230);
	LCD_DrawRectangle (218, 240, 3, 6, Black);
	LCD_DrawRectangle (224, 237, 3, 12, Black);
	LCD_DrawRectangle (230, 233, 3, 18, Black);
	GUI_Text(44, 280, (uint8_t *) "Meal", Black, White);
	GUI_Text(160, 280, (uint8_t *) "Snack", Black, White);
	return;
}
