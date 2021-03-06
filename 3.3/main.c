/*
 * Lab2_2-2.c
 *
 * Created: 9/18/2018 5:05:52 PM
 * Author : rww14
 */ 

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

unsigned int count = 0;
unsigned int count2 = 0;
unsigned int overflows = 0;
unsigned long ticks, diff, edge1, edge2;
double pulse_width;
unsigned int val;

ISR (TIMER1_COMPA_vect){
	count++;
	if (count == 1){
		OCR1A = TCNT1 + 20;		//us pulse
	}
	else if (count == 2){
		clear(TIMSK1, OCIE1A);	//disable output compare interrupt
		clear(TCCR1A, COM1A1);	//disconnect OC1A
		clear(TCCR1A, COM1A0);
		
		set(TIMSK1, ICIE1);		//enable input capture interrupt
		set(TCCR1B, ICES1);		//capture rising edge
		count = 0;
	}
	else {
		//do nothing
	}
}

ISR (TIMER1_CAPT_vect){
	count2++;
	if (count2 == 1){
		set(TIMSK1, TOIE1);		//overflow interrupt enable
		overflows = 0;
		edge1 = ICR1;
		clear(TCCR1B, ICES1);	//capture falling edge
		//set(TIFR1, ICF1);		//clear flag dont need to when using interrupt
	}
	else if (count2 == 2){
		clear(TIMSK1, TOIE1);	//disable overflow interrupt
		clear(TIMSK1, ICIE1);	//disable input capture interrupt
		edge2 = ICR1;
		if (edge2<edge1){
			overflows--;
		}
		diff = edge2 - edge1;
		ticks = overflows * 65535 + diff;
		
		clear(TCCR1A, COM1A1);	//toggle OC1A on match
		set(TCCR1A, COM1A0);
		set(TIMSK1, OCIE1A);	//enable output compare interrupt
		
		clear(PORTB, 1);
		OCR1A = TCNT1 + 200;	//delay for 200 us
		
		count2 = 0;
	}
	else {
		//do nothing
	}
	
}

ISR (TIMER1_OVF_vect){
	overflows++;
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int getGroupNumber(int inputADCval, int ADCmax, int ADCmin, int groupMAX, int groupMIN)
{
	int band=0;
	band = (ADCmax-ADCmin+1)/(groupMAX-groupMIN+1);
	return ((groupMAX-groupMIN)-((inputADCval-ADCmin)/band));
}

int main(void)
{
	uart_init();
	
	//-------Declare I/O-------//
	set(DDRB, 1);		//set PB1 to output
	clear(DDRB, 0);		//set PB0 as input
	set(DDRD, 6);		//set PD6 as output
	clear(DDRC, 0);		//set C0 as input
	set(DDRB, 2);		//set PB2, 3, 4 to output
	set(DDRB, 3);
	set(DDRB, 4);
	
	//-------Setup Timer 1-------//
	clear(TCCR1B, CS10);	//clock source on, /8 prescale
	set(TCCR1B, CS11);
	clear(TCCR1B, CS12);
	
	clear(TCCR1B, WGM13);	//normal mode, up to 65535
	clear(TCCR1B, WGM12);		
	clear(TCCR1A, WGM11);
	clear(TCCR1A, WGM10);
	
	clear(TCCR1A, COM1A1);	//toggle OC1A on match
	set(TCCR1A, COM1A0);
	
	set(TIMSK1, OCIE1A);	//output compare A match interrupt enable
	
	//-------Setup timer 2-------//
	TCCR0A = 0x42;	//0100 0010 toggle OC0A on match, CTC mode up to OCR0A
	TCCR0B = 0x03;	//0000 0011 /64 prescale
	
	//-------ADC setup-------//
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

	//-----//
	clear(PORTB, 1);
	OCR1A = TCNT1 + 20;		//20=10 us pulse at 2 MHz
	
	sei();				//enable global interrupts
	
	while (1)
	{
// 			printf("edge1 =  %li  ", edge1);
// 			printf("edge2 =  %li", edge2);
//			printf("ticks =  %lu \n", ticks);

			if (ticks >= 15000){
				ticks = 15000;
			}
			else if (ticks <= 400){
				ticks = 400;
			}
			val = map(ticks, 400, 15000, 61, 123);
			//printf("val =  %u \n", val);
			if (val < 68){
				OCR0A = 61;	//
			}
			else if (val >= 68 && val < 76){
				OCR0A = 65;	//
			}
			else if (val >= 76 && val < 84){
				OCR0A = 71;	//
			}
			else if (val >= 84 && val < 92){
				OCR0A = 79;	//
			}
			else if (val >= 92 && val < 99){
				OCR0A = 88;	//
			}
			else if (val >= 99 && val < 107){
				OCR0A = 95;
			}
			else if (val >= 107 && val < 115){
				OCR0A = 110;
			}
			else if (val >= 115 && val < 123){
				OCR0A = 123;
			}
			
			//OCR0A = val;		//used for continuous output
			
			//printf("ADC = %i \n", ADC);
			int number = 7 - getGroupNumber(ADC, 920, 500, 7, 0);
			if (number > 7){
				number = 7;
			}
			//printf("group number = %i \n", number);
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
		return(0);
 }


	

	

	


