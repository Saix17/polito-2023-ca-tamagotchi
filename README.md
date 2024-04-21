Hi! This is a project developed at Politecnico di Torino for the Computer Architectures course - 2023.

# Description
Tamagotchi is a famous toy from the end of the ’90s and the beginning of the new millennium. The goal of the game was to take care of
a virtual pet (hunger and happiness)! 

Our pet is Kurama ( Of course everyone knows Kurama.. Right?! :D )

# Project Structure

The main function is contained in the sample.c file. It performs some initialization functions and waits for new commands.

the init_screen(void) function inserts the some visuals components into the screen:
- Age
- Happiness
- Satiety
- Kurama
- Commands (Meal & Snack)

![Kurama](https://github.com/Saix17/polito-2023-ca-tamagotchi/assets/32308426/75db73e6-66cb-4027-9132-d7fd3ee8499c)

Then, also timers, RIT and joystick were initialized!

Timers have different priorities to assure a correct a time estimation. Or... our pet could live a little longer due delays caused by other tasks, but of course we don't want it to happen (?) 
