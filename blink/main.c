/* ===================================================
 *
 *	This "Hello word" program of electronic aims
 *	to go deeper into atMega328p microcontroller.
 *	"Blink" use delay function and set digital pin,
 *	active and deactive it for create this effect.
 *	This is my first low level electronic project,
 *	every suggestion is apreciated.
 *
 *	ebazomik
 *
 * ================================================== */


/* ====== CLOCK FREQUENCY ====== */
#define F_CPU			16000000UL        // 16Mhz atMega328p ARDUINO UNO

/* ====== REGISTERS I/O ====== */
#define DDRB			*(volatile unsigned char*) 0x24
#define PORTB			*(volatile unsigned char*) 0x25

/* ====== REGISTRI TIMER1 ====== */
#define TCCR1B		*(volatile unsigned char*) 0x81
#define TCNT1H		*(volatile unsigned char*) 0x85
#define TCNT1L		*(volatile unsigned char*) 0x84
#define OCR1AH		*(volatile unsigned char*) 0x89
#define OCR1AL		*(volatile unsigned char*) 0x88
#define TIFR1			*(volatile unsigned char*) 0x36

/* ====== REGISTRO STATUS ====== */
#define SREG			*(volatile unsigned char*) 0x5F

/* ====== BIT MASK TIMER1 ====== */
#define CS10			0
#define CS12      2
#define WGM12     3
#define OCF1A     1

/* In this function, to generate a 16-bit delay,
 * interrupts are disabled in order to correctly
 * set the high bite and low bite registers for 
 * TCNT1 and OCR1A respectively, to avoid race conditions. */

void delayMs(unsigned short ms){

	// Save SREG and disable interrupt
	unsigned char oldSreg = SREG;
	SREG &= ~(1 << 7);
	
	// Stop timer
	TCCR1B = 0;	

	// Set zero TCNT1 (first high bites then low bites)
	TCNT1H = 0;
	TCNT1L = 0;

	// Calculate ticks specific for 16Mhz with 1024 prescaler
	// 15625 is result of: 16000000 / (1024 × 1000)
	// 16000000 F_CPU
	// 1024 prescaler
	// 1000 for convert in second

	unsigned int ticks = (ms * 15625UL) / 1000;
	if(ticks > 0) ticks--;

	// Max value of ticks for 16bit delay
	if(ticks > 65535) ticks = 65535;

	// Output compare register
	OCR1AH = (ticks >> 8) & 0xFF;
	OCR1AL = ticks & 0xFF;

	// Reset flag OCF1A (write 1 for reset)
	TIFR1 = (1 << OCF1A);

	// Restore interrupt
	SREG = oldSreg;

	// Set prescaler a 1024 and CTC mode
	TCCR1B = (1 << WGM12) | (1 << CS12 ) | ( 1 << CS10 );

	// Wait TIFR1 equal OCF1A
	while(!(TIFR1 & (1 << OCF1A))){}
	
	// Stop timer
	TCCR1B = 0;
}



int main(void){
	
	// Set pin 13 output mode 
	DDRB |= (1 << 5);

	while(1){

		// Pin 13 high
		PORTB |= (1 << 5);
		delayMs(3000);

		// Pin 13 low
		PORTB &= ~ (1 << 5);
		delayMs(1000);

	}

	return 0;
}
