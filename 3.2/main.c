/*
 * Lab2_3-1.c
 *
 * Created: 9/20/2018 6:19:38 PM
 * Author : rww14
 */ 

#include <avr/io.h>
#define F_CPU 16000000
#include <avr/interrupt.h> // common defines and macros
#include <avr/io.h> // derivative information
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "uart.h"

#define set(reg,bit)		(reg) |= (1<<(bit))
#define clear(reg,bit)		(reg) &= ~(1<<(bit))
#define toggle(reg,bit)		(reg) ^= (1<<(bit))
#define check(reg,bit)		(bool)(reg & (1<<(bit)))

int main(void)
{
	uart_init();
	clear(DDRC, 0);		//set C0 as input
	set(DDRB, 2);		//set PB2, 3, 4 to output
	set(DDRB, 3);
	set(DDRB, 4);
	
	set(ADMUX, REFS0);	//setting reference voltage for ADC to Vcc
	clear(ADMUX, REFS1);
	
	set(ADCSRA, ADPS2);	//set ADC prescaler to 125 kHz based on system clock
	set(ADCSRA, ADPS1);
	set(ADCSRA, ADPS0);
	
	clear(ADCSRB, ADTS2);	//free running mode
	clear(ADCSRB, ADTS1);
	clear(ADCSRB, ADTS0);
	
	set(ADCSRA, ADATE);	//auto trigger (bc free running)
	
	set(DIDR0, ADC0D);	//disable digital input for ADC0
	
	clear(ADMUX, MUX3);	//ADC0 - pin C0
	clear(ADMUX, MUX2);
	clear(ADMUX, MUX1);
	clear(ADMUX, MUX0);
	
	set(ADCSRA, ADEN);	//enable ADC subsystem
	set(ADCSRA, ADSC);	//begin conversion
	
    while (1) 
    {
		//printf("ADC = %i \n", ADC);
		int number = 7 - getGroupNumber(ADC, 920, 500, 7, 0);
		if (number > 7){
			number = 7;
		}
		printf("group number = %i \n", number);
		if (number == 0){
			clear(PORTB, 2);
			clear(PORTB, 3);
			clear(PORTB, 4);
		}
		else if (number == 1){
			set(PORTB, 2);
			clear(PORTB, 3);
			clear(PORTB, 4);
		}
		else if (number == 2){
			clear(PORTB, 2);
			set(PORTB, 3);
			clear(PORTB, 4);
		}
		else if (number == 3){
			set(PORTB, 2);
			set(PORTB, 3);
			clear(PORTB, 4);
		}
		else if (number == 4){
			clear(PORTB, 2);
			clear(PORTB, 3);
			set(PORTB, 4);
		}
		else if (number == 5){
			set(PORTB, 2);
			clear(PORTB, 3);
			set(PORTB, 4);
		}
		else if (number == 6){
			clear(PORTB, 2);
			set(PORTB, 3);
			set(PORTB, 4);
		}
		else if (number == 7){
			set(PORTB, 2);
			set(PORTB, 3);
			set(PORTB, 4);
		}
		
		
    }
}

int getGroupNumber(int inputADCval, int ADCmax, int ADCmin, int groupMAX, int groupMIN)
{
	int band=0;
	band = (ADCmax-ADCmin+1)/(groupMAX-groupMIN+1);
	return ((groupMAX-groupMIN)-((inputADCval-ADCmin)/band));
}

