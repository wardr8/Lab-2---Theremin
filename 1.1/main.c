/*
 * Lab2_1-1.c
 *
 * Created: 9/13/2018 6:21:58 PM
 * Author : rww14
 */ 
//functions
#define set(reg,bit)		(reg) |= (1<<(bit))
#define clear(reg,bit)		(reg) &= ~(1<<(bit))
#define toggle(reg,bit)		(reg) ^= (1<<(bit))
#define check(reg,bit)		(bool)(reg & (1<<(bit)))

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>

int main(void)
{
	
	/*Setup I/O*/
	set(DDRB, 5);	//set PB5 to output
	clear(DDRB, 0);	//set PB0 to input
	set(PORTB, 0);	//enable pull up resistor
	
	//setup timer
	clear(TCCR1A, COM1A1);
	clear(TCCR1A, COM1A0);	//disable OC1A (PD6) on overflow
	
	clear(TCCR1A, WGM10);
	clear(TCCR1A, WGM11);
	clear(TCCR1B, WGM12);	//set mode to Normal, up to 0xFFFF (65535)
	clear(TCCR1B, WGM13);
	
	set(TCCR1B, CS10);
	clear(TCCR1B, CS11);
	clear(TCCR1B, CS12);	//set clock source, no prescale
	
	clear(TCCR1B, ICES1);	//negative (falling) edge used as trigger
	set(TIMSK1, ICIE1);		//input capture interrupt enabled. interrupt vector executed when ICF flag in TIFR1 is set
	
	sei();		//enable global interrupts
	
	while (1)
	{
		
		
	}
}

ISR(TIMER1_CAPT_vect){
	toggle(PORTB, 5);
	//set(TIFR1, ICF1);
	toggle(TCCR1B, ICES1);
}

