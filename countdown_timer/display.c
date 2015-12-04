#include "display.h"

void displayFunction() {

	// Clear entire display memory
	LCDBMEMCTL |= LCDCLRBM + LCDCLRM;

	//Refresh rate of LCD display is the number of times per second in which the display draws the data it is being given

	// LCD_FREQ = ACLK/16/8 = 256Hz
	// Frame frequency = 256Hz/4 = 64Hz, LCD mux 4, LCD on
	LCDBCTL0 = (LCDDIV0 + LCDDIV1 + LCDDIV2 + LCDDIV3) | (LCDPRE0 + LCDPRE1) | LCD4MUX | LCDON;


	// LCB_BLK_FREQ = ACLK/8/4096 = 1Hz
	LCDBBLKCTL = LCDBLKPRE0 | LCDBLKPRE1 | LCDBLKDIV0 | LCDBLKDIV1 | LCDBLKDIV2 | LCDBLKMOD0;

	// I/O to COM outputs
	P5SEL |= (BIT5 | BIT6 | BIT7);
	P5DIR |= (BIT5 | BIT6 | BIT7);

	// Activate LCD output
	LCDBPCTL0 = 0xFFFF;         // Select LCD segments S0-S15
	LCDBPCTL1 = 0x00FF;         // Select LCD segments S16-S22


	//LCDM8 = SEG_E + SEG_G;     // Display Character



	// LCD_B Base Address is 0A00H page 30 y in SALS554 document
	// show 'h'
//	lcdmem  = (unsigned char *)0x0A29;
//	*lcdmem = (unsigned char)(*lcdmem | SEG_A + SEG_B + SEG_C);

}


//Display countdown timer
void displayTimer(thousands, hundreds, tens, ones) {

			LCDM2 = LCD_Char_Map[thousands];     // Display thousands and store in memory 0x0A21
			LCDM3 = LCD_Char_Map[hundreds];      // Display hundreds and store in memory 0x0A22
			LCDM4 = LCD_Char_Map[tens];          // Display tens and store in memory 0x0A23
			LCDM6 = LCD_Char_Map[ones];          // Display ones and store in memory 0x0A24
}

//Display temperature
void displayTemperature(first, second, third) {
	LCDM2 = LCD_Char_Map[first];     	// Display first digit and store in memory 0x0A21
	LCDM3 = LCD_Char_Map[second];     	// Display second digit and store in memory 0x0A22
	LCDM1 = SEG_C;						// Display "." and store in memory 0x0A20
	LCDM4 = LCD_Char_Map[third];        // Display third digit and store in memory 0x0A23
	LCDM5 = SEG_G;						// Display degree symbol and store in memory 0x0A24
	LCDM6 = LCD_Char_Map[11];			// Display "C" and store in memory 0x0A25
}

//Display "Good" or"Bad" during temperature measurement
void displaySuitText(flag){
	//Display "BAd" as temperature is not suitable yet
	if (flag == 0) { //When temperature is not suitable yet
		LCDM11 =	SEG_C + SEG_A + SEG_B + SEG_F + SEG_E + SEG_G + SEG_H;	//Display "B" and store in memory 0x0A2A
		LCDM10 = SEG_C + SEG_A + SEG_B + SEG_F + SEG_E + SEG_G; 			//Display "B" and store in memory 0x0A29
		LCDM9 = SEG_E + SEG_G + SEG_B + SEG_C + SEG_H;						//Display "B" and store in memory 0x0A28
	}
	//Display "good" as temperature is suitable
	if (flag == 1) {	//When temperature is over pre-defined temperature (suitable)
		LCDM11 = SEG_F + SEG_A + SEG_B + SEG_E + SEG_G + SEG_H;				//Display "g" and store in memory 0x0A2A
		LCDM10 = SEG_B + SEG_C + SEG_E + SEG_H;								//Display "0" and store in memory 0x0A29
		LCDM9 = SEG_B + SEG_C + SEG_E + SEG_H;								//Display "o" and store in memory 0x0A28
		LCDM8 = SEG_E + SEG_G + SEG_H + SEG_C + SEG_B;						//Display "d" and store in memory 0x0A27
		__delay_cycles(1000000);											//Delay 1s
		clearScreen();														//Clear the entire screen
		flag += 1;															//Increment the flag.
	}
}

//void welcomeScreen(d) {
//	if (d != 2){
//		LCDM3 = LCD_Char_Map[11];      // Display Character
//		LCDM4 = LCD_Char_Map[13];          // Display Character
//		LCDM6 = LCD_Char_Map[14];          // Display Character
//		LCDM10 = SEG_E + SEG_G;
//		LCDM9 = SEG_E + SEG_G;
//	}
//
//}

//Clear the screen
void clearScreen() {
	LCDM1 = 0;			//Content in memory 0x0A20 becomes 0
	LCDM2 = 0;			//Content in memory 0x0A21 becomes 0
	LCDM3 = 0;			//Content in memory 0x0A22 becomes 0
	LCDM4 = 0;			//Content in memory 0x0A23 becomes 0
	LCDM5 = 0;			//Content in memory 0x0A24 becomes 0
	LCDM6 = 0;			//Content in memory 0x0A25 becomes 0
	LCDM7 = 0;			//Content in memory 0x0A26 becomes 0
	LCDM8 = 0;			//Content in memory 0x0A27 becomes 0
	LCDM9 = 0;			//Content in memory 0x0A28 becomes 0
	LCDM10 = 0;			//Content in memory 0x0A29 becomes 0
	LCDM11 = 0;			//Content in memory 0x0A2A becomes 0
	LCDM12 = 0;			//Content in memory 0x0A2B becomes 0
}


