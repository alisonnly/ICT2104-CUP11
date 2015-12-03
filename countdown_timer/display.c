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

void displayTimer(thousands, hundreds, tens, ones) {

			LCDM2 = LCD_Char_Map[thousands];     // Display Character
			LCDM3 = LCD_Char_Map[hundreds];      // Display Character
			LCDM4 = LCD_Char_Map[tens];          // Display Character
			LCDM6 = LCD_Char_Map[ones];          // Display Character
}

void displayTemperature(first, second, third) {
	LCDM2 = LCD_Char_Map[first];     // Display Character
	LCDM3 = LCD_Char_Map[second];      // Display Character
	LCDM1 = SEG_C;
	LCDM4 = LCD_Char_Map[third];          // Display Character
	LCDM5 = SEG_G;
	LCDM6 = LCD_Char_Map[11];


}

void displaySuitText(flag){
	if (flag == 0) {
		LCDM11 =	SEG_C + SEG_A + SEG_B + SEG_F + SEG_E + SEG_G + SEG_H;
		LCDM10 = SEG_C + SEG_A + SEG_B + SEG_F + SEG_E + SEG_G;
		LCDM9 = SEG_E + SEG_G + SEG_B + SEG_C + SEG_H;
	}
	if (flag == 1) {
		LCDM11 = SEG_F + SEG_A + SEG_B + SEG_E + SEG_G + SEG_H;
		LCDM10 = SEG_B + SEG_C + SEG_E + SEG_H;
		LCDM9 = SEG_B + SEG_C + SEG_E + SEG_H;
		LCDM8 = SEG_E + SEG_G + SEG_H + SEG_C + SEG_B;
		__delay_cycles(1000000);
		clearScreen();
		flag += 1;
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

void clearScreen() {
	LCDM1 = 0;
	LCDM2 = 0;
	LCDM3 = 0;
	LCDM4 = 0;
	LCDM5 = 0;
	LCDM6 = 0;
	LCDM7 = 0;
	LCDM8 = 0;
	LCDM9 = 0;
	LCDM10 = 0;
	LCDM11 = 0;
	LCDM12 = 0;
//	LCDM13 = 0;
//	LCDM14 = 0;
//	LCDM15 = 0;
//	LCDM16 = 0;
//	LCDM17 = 0;
//	LCDM18 = 0;
//	LCDM19 = 0;
//	LCDM20 = 0;
//	LCDM21 = 0;
//	LCDM22 = 0;
//	LCDM23 = 0;
//	LCDM24 = 0;

}


