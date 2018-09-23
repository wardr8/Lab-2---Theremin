#include <avr/interrupt.h> // common defines and macros
#include <avr/io.h> // derivative information
#include <stdio.h>
#include <stdlib.h>
/*#include "uart.h"*/
unsigned int count = 18; // 1 kHz signal, half_period given in bus cycles Assume your clock is 2MHz. 1 cycle is 0.5us

ISR (TIMER0_COMPA_vect){
	OCR0A += count;
	//TCNT0 = 0;
}

int main(void){
	
	DDRD |= 0x40; // set Port D6 to output
	TCCR0A |= 0x40; // toggle D6 on a compare match,
	TCCR0B |= 0x05; // start the clock, /1024 prescale
	//Mode 2 CTC, up to OCRA
	TIMSK0 = 0x02; // enable output compare A interrupt
	OCR0A += TCNT0 + count; //Starts the OC process at arbitrary value
	sei(); // enable global interrupts and let the ISR do the work now
	while(1){
		
	}
	return(0);
}