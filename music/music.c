#include "music.h"

#include "../timer/timer.h"

void playNote(NOTE note)
{
	if(note.freq != pause)
	{
		reset_timer(3);
		init_timer(3, note.freq);
		enable_timer(3);
	}
	reset_timer(2);
	init_timer(2, note.duration);
	enable_timer(2);
}

BOOL isNotePlaying(void)
{
	return ((LPC_TIM3->TCR != 0));
}
