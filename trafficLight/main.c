/* ===========================================================
 *
 *	This project simulate a simple trafic light
 *	with avr-libc.
 *	Before the red light expires, the light
 *	flashes four times to signal the end of
 *	its duration, and the right of way will pass
 *	to the opposite side.
 *
 *	ebazomik
 *
 * ========================================================== */

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define PORTB_UP(pin) ((PORTB) |= (1 << (pin)))
#define PORTB_DOWN(pin) ((PORTB) &= ~ (1 << (pin)))

/* testPin turns the pin provided by the parameter [pin]
 * on and off with a delay of 200 ms. */
void testPin(int pin){
	PORTB_UP(pin);
	_delay_ms(200);
	PORTB_DOWN(pin);
}


/* blinkingLoop turns the pin provided by the parameter [pin]
 * on and off with two delay of 200m for a loop that repeats
 * [n] times */
void blinkingLoop(uint8_t n, int pin){
	for (uint8_t i = 1; i < n; i++){
		PORTB_UP(pin);
		_delay_ms(200);
		PORTB_DOWN(pin);
		_delay_ms(200);
	}
}


int main(void){

	/* Set pins from PORTB in OUTPUT mode, from pin 0 to pin 5,
	 * on Arduind correspond top pin8, pin9, pin10, pin11, pin12,
	 * and pin13 */
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5);
		
	/* Test pins before run program */
	testPin(PB0);
	testPin(PB1);
	testPin(PB2);
	testPin(PB3);
	testPin(PB4);
	testPin(PB5);

	_delay_ms(2000);


	/* It depends on how the hardware was assembled.
	 * I used a breadboard and will use the terms
	 * “dx” and “sx” since the pins are mirrored.  */
	while(1){

		/* Turn on dx red light and green sx ad wait 5 sec */
		PORTB_UP(PB0);
		PORTB_UP(PB5);
		_delay_ms(5000);

		/* Turn off dx red light and green sx and wait 200ms */
		PORTB_DOWN(PB0);
		PORTB_DOWN(PB5);
		_delay_ms(200);

		/* Turn on sx yellow light before start blinking red
		 * light on dx side */
		PORTB_UP(PB4);
		blinkingLoop(5, PB0);

		/* After blinking turn off sx yellow light and red
		 * on dx side */
		PORTB_DOWN(PB4);
		PORTB_DOWN(PB5);

		/* Repeat same logic in the opposite side */
		PORTB_UP(PB2);
		PORTB_UP(PB3);
		_delay_ms(5000);

		PORTB_DOWN(PB2);
		PORTB_DOWN(PB3);
		_delay_ms(200);

		PORTB_UP(PB1);
		blinkingLoop(5, PB3);

		PORTB_DOWN(PB1);

	}

}
