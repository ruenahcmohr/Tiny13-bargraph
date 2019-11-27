/********************

Rues not-LM3914

PB0  charlie pin
PB1  charlie pin
PB2  charlie pin
PB3  charlie pin
PB4  adc level control
PB5 reset






wiring:
  1) wire up 12 leds in a charlie array, with no resistors
  2) wire a resistor (330 ohms?) from each of the 4 t13 charlie pins (as above)
     to each node on the charlie array
  3) put a 10k resistor ("10k" being from 4.7k to 50k) from +5V to reset
  4) set up whatever analog voltage source you want (0-5V) to the adc input
  5) power with 5V (or hack design and use anything from 1.8V up to 5)
  
more wiring:
  ) ok fine, here is how you wire a charlie array (you should know this!)
  
  A ---|>|--- B
  
  A ---|<|--- B
  
  B ---|>|--- C
  
  B ---|<|--- C 
  
  C ---|>|--- D 
  
  C ---|<|--- D
  
  D ---|>|--- A  
  
  D ---|<|--- A  
  
  A ---|>|--- C 
  
  A ---|<|--- C
  
  B ---|>|--- D 
  
  B ---|<|--- D  
          

compiling: (it works on my machine)

> make

installing: (it works on my machine)
  I use a usb avrisp2 clone programmer (or) a usbasp
  
  >make usbinstall
       or
  >make aspinstall
  
  should work


setup:

1)  change the "orderfix" array to the 1:1 test array
2)  slowly go from 0 to 5V on the adc input {
   2.1) create an array of numbers that describes, counting from 0
          which leds lit up in order.
          for example { if, at 0 volts, your 6th led (0, 1, 2, 3, 4, 5, [6]) lights up, start with 6
                        if the next led to light up is the 7th, make the next number 7
                        if the next led to light up is the 4th (0, 1, 2, 3, [4]) make the next number 4
                      }
   }
3) wewrite orderFix with your list of numbers.
4) if (that didn't fix it) goto setup;

This is easier than trying to make the whole charlieplex array in the right order.
  
  


*****************/


#include<avr/io.h>
#include<avr/interrupt.h>

#define OUTPUT  1
#define INPUT   0


#define SetBit(BIT, PORT)     PORT |= (1<<BIT)
#define ClearBit(BIT, PORT)   PORT &= ~(1<<BIT)
#define IsHigh(BIT, PORT)    (PORT & (1<<BIT)) != 0
#define IsLow(BIT, PORT)     (PORT & (1<<BIT)) == 0

// this order repair array could have been removed in favor of rewriting ledon(), but I wanted to 
//  make it easy for anyone to modify the code, reset to the test array

// unsigned char orderFix[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 } // 1:1 test array

unsigned char orderFix[] = { 6, 7, 4, 5, 3, 2, 1, 0, 8, 9, 10, 11 };
volatile unsigned int level;


void init(void) ;
void ledon(unsigned char led) ;

int main( void ) {

  unsigned char n;
 
  init();
  
  sei();
  
  while(1) {
    n = level/79;
    if (n > 11) n = 11;        
    ledon(orderFix[n]);
  }
  
 return 0;
}


/*! \brief Initialisation
 */
void init(void) {
  // Set clock prescaler: 0 gives full 4.8 MHz from internal oscillator.
  CLKPR = (1 << CLKPCE);
  CLKPR = 0;

  DDRB = INPUT << PB0 | INPUT << PB1 |INPUT << PB2 |INPUT << PB3 |INPUT << PB4 |INPUT << PB5 ;

 
 // adc init
 
 // ADC, 9.6Mhz / 128
  ADMUX  = ( (1 << MUX1) );
  ADCSRA = ( (1 << ADEN) | (1 << ADIF) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)  | (1 << ADATE) | (1 << ADSC)  ) ;

 
}


void ledon(unsigned char led) {

  switch(led) {
    case  0:  PORTB = 0x01;   DDRB  = 0x03;   break;
    case  1:  PORTB = 0x02;   DDRB  = 0x03;   break;
    case  2:  PORTB = 0x01;   DDRB  = 0x05;   break;
    case  3:  PORTB = 0x04;   DDRB  = 0x05;   break;
    case  4:  PORTB = 0x01;   DDRB  = 0x09;   break;
    case  5:  PORTB = 0x08;   DDRB  = 0x09;   break;       
    case  6:  PORTB = 0x04;   DDRB  = 0x06;   break;
    case  7:  PORTB = 0x02;   DDRB  = 0x06;   break;
    case  8:  PORTB = 0x02;   DDRB  = 0x0A;   break;
    case  9:  PORTB = 0x08;   DDRB  = 0x0A;   break;
    case 10:  PORTB = 0x04;   DDRB  = 0x0C;   break;
    case 11:  PORTB = 0x08;   DDRB  = 0x0C;   break;       
  }

}


ISR(  ADC_vect) {   
  
  level = ADC ;
 
}



