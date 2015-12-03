#include <msp430.h>
#include <cc430F6137.h>


#define SEG_A    BIT4
#define SEG_B    BIT5
#define SEG_C    BIT6
#define SEG_D    BIT7
#define SEG_F    BIT0
#define SEG_G    BIT1
#define SEG_E    BIT2
#define SEG_H    BIT3

//define lcd segment for each characters
const unsigned char  LCD_Char_Map[] =
{
	SEG_A + SEG_B + SEG_C + SEG_D + SEG_E + SEG_F,         // Displays "0" or "O"
	SEG_B + SEG_C,                                         // Displays "1"
	SEG_A + SEG_B + SEG_D + SEG_E + SEG_G,                 // Displays "2"
	SEG_A + SEG_B + SEG_C + SEG_D + SEG_G,                 // Displays "3"
	SEG_B + SEG_C + SEG_F + SEG_G,                         // Displays "4"
	SEG_A + SEG_C + SEG_D + SEG_F + SEG_G,                 // Displays "5" or "S"
	SEG_A + SEG_C + SEG_D + SEG_E + SEG_F + SEG_G,         // Displays "6"
	SEG_A + SEG_B + SEG_C,                                 // Displays "7"
	SEG_A + SEG_B + SEG_C + SEG_D + SEG_E + SEG_F + SEG_G, // Displays "8"
	SEG_A + SEG_B + SEG_C + SEG_D + SEG_F + SEG_G,         // Displays "9"
	SEG_C + SEG_D + SEG_E + SEG_G,						   // Displays "o"
	SEG_A + SEG_D + SEG_E + SEG_F,						   // Displays "C"
	SEG_B + SEG_C + SEG_D + SEG_E + SEG_G,				   // Displays "d"
	SEG_B + SEG_C + SEG_D + SEG_E + SEG_F,				   // Display "U"
	SEG_A + SEG_B + SEG_G + SEG_F + SEG_E				   // Display "P"
};

void displayFunction();
void displayTimer(thousands, hundreds, tens, ones);
void clearScreen();
void displayTemperature(first, second, third);
void displaySuitText(flag);
//void welcomeScreen(d);
