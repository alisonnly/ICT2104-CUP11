//#include <msp430.h>
#include <cc430F6137.h>
#include "temperature.h"
#include "acceleration.h"
#include "as.h"

/////////////////////////////////////////////////////////////////////////////
//prototype
void buzzeralert();
void timercountdown();
void accCount();
void buzzerstop();
/////////////////////////////////////////////////////////////////////////
//global variable
signed short d = 0;
signed short m = 180;    //predefined 3 minutes in seconds
unsigned short interruptFlag = 2;    //used in timer interrupt, set it to 2 first so that it will execute the acc to wake up the watch first. 
unsigned char thousands,hundreds,tens,ones;  //for the displaying on the watch.
int flag = 0;
///////////////////////////////////////////////
void timercountdown()
{
	//initialize timer 
	TA0CCR0 = 32728 - 1;    				//Is to set the counter register value to 32727 so that the aclk will count until this value and trigger an interrupt. 
	TA0CCTL0 |= CCIE;     					//this is to enable interrupt. We used OR operator instead of equal is to keep/preserved the previous setting by adding additional flag.
	TA0CTL |= TASSEL_1 + MC_1; 				//set up to use aclk and up mode. 
	__bis_SR_register(LPM3_bits + GIE);  	//set to low power mode 3 and enable interrupts so that even though the watch is sleeping, it is able to awake when interrupt
}



void buzzeralert()
{
	PMAPPWD = 0x02D52;                        // Get write-access to port mapping regs //something like password [same as WDT]
	P2MAP7 = PM_TA1CCR1A;                     // Map TA1CCR1 output to P2.7 
	PMAPPWD = 0;                              // Lock port mapping registers

	P2DIR |= BIT7;                     		// set P2.7 to output. P2.7 is the buzzer
	P2SEL |= BIT7;                     		// P2.7 options select 
	//TA1CCTL1 |= CCIE;
	TA1CCR0 = 32;                            // PWM Period/2
	TA1CCTL1 = OUTMOD_6;                      // CCR1 toggle/set
	TA1CCR1 = 8;                             // CCR1 PWM duty cycle
	TA1CTL = TASSEL_1 + MC_1;                 // set to ACLK, up mode  


}

void buzzerstop() {

	TA1CCR0 = 0;                 //clear the counter           
	TA1CCR1 = 0;                 //clear the counter so that there is no more toggling meaning duty cycle will be flat so no buzzer sound would be produced.             
	//TA1CCTL1 &= ~CCIE;   
	clearScreen();
	interruptFlag = 2;   //set back interrupt flag to 2 so that it will go into the timer interrupt for acc 
	timercountdown();

}

void accCount(){

	accelFunc();
	if(sAccel.count >= 10){
		__delay_cycles(40000);
		interruptFlag = 1;

		sAccel.count = 0;
		m=180;    //set it back to 3 minutes because previously the minutes is already countdown to 0.
		timercountdown();
	}

}

int main(void){

	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	displayFunction();

	//this part of code says I want to do interrupt in port 2
	P2IES |= BIT2 ;  //p2.2 hi/lo edge (falling edge)
	P2IFG &= ~BIT2 ; //p2.2 IFG cleared
	P2IE |= BIT2; //p2.2 interrupt enabled
			
	as_init();
	reset_acceleration();
	
	// welcomeScreen();
	// __delay_cycles(10000);
	timercountdown();

}


#pragma vector = TIMER0_A0_VECTOR   
__interrupt void TIMER0_A0_ISR(void)
{
	if(interruptFlag == 1){  //when the interrupt flag is 1 it will do temperature and timer countdown
		temperatureFunction(flag);
		displaySuitText(flag);
		if (temperatureDegC > 25){
			flag = 1;

			if( m > 0){             // timer is still counting down
				hundreds = m/60;    //to get the second digit counting from left
				tens = (m%60)/10;   //to get the third digit counting from left
				ones = (m % 60) % 10;   //to get the fourth digit counting from left
				m--;    				//decrease by 1 every seconds timer trigger interrupt. 
			} // m>0 end

			else if(m == 0){    //timer stop counting because it has reach 0 seconds
				hundreds = 0;   //make it to 0 so that it will display on the watch 0s
				tens = 0;
				ones = 0;
				displayTimer(thousands, hundreds, tens, ones);
				buzzeralert();   //call this function to activate the ringing of the buzzer
			    d =1;
				TA0CCR0 = 0;  //to stop it from counting.
			} // m == 00 end
			
			displayTimer(thousands, hundreds, tens, ones);
		} //temp end
	}//flag end
	else if (interruptFlag == 2){  //when the interrupt flag is 2, it will wake up from sleep to trigger acc.
		accCount();
	}

}

//when the watch detect falling edge it will trigger the button interrupt.
//PORT2 interrupt service routine
#pragma vector = PORT2_VECTOR  
__interrupt void pushbutton_ISR (void) 
{
	switch(__even_in_range(P2IV, 10)) 
	{
	case 0x00: break; //none interrupt
	case P2IV_P2IFG0: //pin 2.0 => 0x02   P2IV_P2IFG0 is the definition of 0x02 so you can also write 0x02
		break;
	case 0x04: break; //p2.1
	case P2IV_P2IFG2:  //this is the button for the middle button(light button in between up and down button) of the watch which is to stop the buzzer from continuously ringing. 
		if(d == 1){
		buzzerstop();  //call stop buzzer function 
		}

		break;
	case 0x08:
;
		break;
	case 0x0A: break;
	case 0x0C: break;
	case 0x0E: break;
	case 0x10: break;
	default: break;

	}
}
