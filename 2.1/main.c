#include <avr/interrupt.h> // common defines and macros
#include <avr/io.h> // derivative information
#include <stdio.h>
#include <stdlib.h>
/*#include "uart.h"*/
unsigned int count = 17; // 1 kHz signal, half_period given in bus cycles Assume your clock is 2MHz. 1 cycle is 0.5us

int main(void){
	
	DDRD |= 0x40; // set Port D6 to output
	TCCR0A |= 0x42; // toggle D6 on a compare match,
	TCCR0B |= 0x05; // start the clock, /1024 prescale
					//Mode 2 CTC, up to OCRA
	
	OCR0A = count; //Starts the OC process at arbitrary value
	
	while(1){
		
	}
	return(0);
}

