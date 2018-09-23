/*
 * GccApplication1.c
 *
 * Created: 9/10/2018 6:36:38 PM
 * Author : rww14
 */ 
#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

#define set(reg,bit)		(reg) |= (1<<(bit))
#define clear(reg,bit)		(reg) &= ~(1<<(bit))
#define toggle(reg,bit)		(reg) ^= (1<<(bit))
#define check(reg,bit)		(bool)(reg & (1<<(bit)))


int main(void)
{
	/*Setup */
	set(DDRB, 5);	//set PB5 to output, PB0 to input
	clear(DDRB, 0);
	set(PORTB, 0);	//activate PB0 pull-up resistor 
    clear(PORTB, 5);
	
    while (1) 
    {

		if (check(PINB, 0) == 0){
			set(PORTB, 5);
		}
		else {
			clear(PORTB, 5);
		}
		
    }
}
