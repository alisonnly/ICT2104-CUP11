//#include <msp430.h>
#include <cc430F6137.h>
#include "temperature.h"
#include "acceleration.h"
#include "as.h"


void buzzeralert();
void timercountdown();
void accCount();
void buzzerstop();
/////////////////////////////////////////////////////////////////////////
void timercountdown()
{
	//initialise
	//max
	TA0CCR0 = 32728 - 1;// (max can only count up to this number) //is already in seconds
	//enable timer interrupt
	TA0CCTL0 |= CCIE;   //TA0CCR0 = CCIE //preserve(keep) the previous setting by adding additional flag.
	TA0CTL |= TASSEL_1 + MC_1; //assign aclk and up mode
	__bis_SR_register(LPM3_bits + GIE);  //set to low power mode and enable interrupts so that even though i am sleeping, i am able to awake when interrupt


}
signed short d = 0;
signed short m = 10;
signed short sec2 = 0;
signed short sec1 = 0;
unsigned short interruptFlag = 2;  //first time flag to decrease sec
unsigned char thousands,hundreds,tens,ones;
int flag = 0;

#pragma vector = TIMER0_A0_VECTOR   //
__interrupt void TIMER0_A0_ISR(void)
{
	if(interruptFlag == 1){
	temperatureFunction(flag);
	displaySuitText(flag);
	if (temperatureDegC > 25){
		flag = 1;

		if( m > 0){
				hundreds = m/60;
				tens = (m%60)/10;
				ones = (m % 60) % 10;
				 m--;
			} // m>0 end

		else if(m == 0){
				hundreds = 0;
				tens = 0;
				ones = 0;
				displayTimer(thousands, hundreds, tens, ones);
				buzzeralert();
				 d =1;
				TA0CCR0 = 0;  //to stop it from keep counting.

			} // m == 00 end
			displayTimer(thousands, hundreds, tens, ones);

	} //temp end

	}//flag end

	else if (interruptFlag == 2){
			accCount();
		}

}


void buzzeralert()
{
	PMAPPWD = 0x02D52;                        // Get write-access to port mapping regs ///basically to enable pin2.7 else it is protected//like password
	P2MAP7 = PM_TA1CCR1A;                     // Map TA1CCR1 output to P2.7  //put PWM signal (port) into the buzzer(p2.7)
	PMAPPWD = 0;                              // Lock port mapping registers

	P2DIR |= BIT7;                     		// P2.7 output   //P2.7 is the buzzer
	P2SEL |= BIT7;                     		// P2.7 options select //is like a special function to select that port to do PWM
	//TA1CCTL1 |= CCIE;
	TA1CCR0 = 32;                            // PWM Period/2
	TA1CCTL1 = OUTMOD_6;                      // CCR1 toggle/set
	TA1CCR1 = 8;                             // CCR1 PWM duty cycle
	TA1CTL = TASSEL_1 + MC_1;                 // ACLK, up-down mode  //the MC and clock source i can choose any


}

void buzzerstop() {

	TA1CCR0 = 0;                            // PWM Period/2
	TA1CCR1 = 0;                             // CCR1 PWM duty cycle
	TA1CCTL1 &= ~CCIE;   //come out of interrupt
	clearScreen();
	interruptFlag = 2;
	timercountdown();

}

void accCount(){

	accelFunc();
	if(sAccel.count >= 10){
		__delay_cycles(40000);
		interruptFlag = 1;

		sAccel.count = 0;
		m=10;
		timercountdown();
	}

}

int main(void){

	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	displayFunction();

	    //this part of code says I want to do interrupt in port 2
	        P2IES |= BIT0 + BIT2 ;  //p2.0 and p2.2 hi/lo edge
	        P2IFG &= ~BIT0 & ~BIT2 ; //p2.0 and p2.2 IFG cleared
	        P2IE |= BIT0 + BIT2; //p2.0 and p2.2 interrupt enabled
	        as_init();
	        reset_acceleration();
	      //  __bis_SR_register(GIE);
	       // welcomeScreen();
	       // __delay_cycles(10000);
	timercountdown();

}



//PORT2 interrupt service routine
#pragma vector = PORT2_VECTOR  //to tell where the interrupt coming from
__interrupt void pushbutton_ISR (void)  //pushbutton_ISR is any name , there must not have any return value
{
	switch(__even_in_range(P2IV, 10)) // i am looking at p2 interrupt vector , 10 --> smaller the better
	{
	case 0x00: break; //none interrupt
	case P2IV_P2IFG0: //pin 2.0 => 0x02   P2IV_P2IFG0 is the definition of 0x02 so you can also write 0x02
		break;
	case 0x04: break; //p2.1
	case P2IV_P2IFG2:
		if(d == 1){
		buzzerstop();
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
