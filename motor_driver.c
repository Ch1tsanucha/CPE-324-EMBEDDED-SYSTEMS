#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "motor_driver.h"
#include "lcd_driver.h"

void fd(int speed) {
    
  TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);  // Fast PWM
    TCCR0B |= (1 << CS01);  // Prescaler = 8, starts Timer0



    speed += 100;
    OCR0A = speed - 10;
    OCR0B = speed;
}

void tr(int speed) {
      TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);  // Fast PWM
    TCCR0B |= (1 << CS01);  // Prescaler = 8, starts Timer0

    
    speed += 100;
    OCR0A = speed - 10;
    OCR0B = 0;
}

void tl(int speed) {
    TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);  // Fast PWM
    TCCR0B |= (1 << CS01);  // Prescaler = 8, starts Timer0

    
    speed += 100;
    OCR0A = 0;
    OCR0B = speed;
}

void st() {
    
    TCCR0A &= ~((1 << COM0A1) | (1 << COM0B1));
    TCCR1A &= ~((1 << COM1A1) | (1 << COM1B1));
    TCCR0B &= ~((1 << CS00) | (1 << CS01) | (1 << CS02));  // Timer0 stop
    TCCR1B &= ~((1 << CS10) | (1 << CS11) | (1 << CS12));  // Timer1 stop


    DDRD |= (1 << DDD6) | (1 << DDD5);   // OC0A (PD6), OC0B (PD5)
    DDRB |= (1 << DDB1) | (1 << DDB2);   // OC1A (PB1), OC1B (PB2)
    
   
    PORTB &= ~(1 << PB1);  // IN1 = 0
    PORTB &= ~(1 << PB2);  // IN2 = 1

    PORTD &= ~(1 << PD5);  // IN3 = 0
    PORTD &= ~(1 << PD6);  // IN4 = 1
    
    OCR0A = 0;
    OCR0B = 0;
    OCR1A = 0;
    OCR1B = 0;

}

void sr() {
    
    TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);  // Fast PWM
    TCCR0B |= (1 << CS01);  // Prescaler = 8, starts Timer0

    TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);  // 8-bit Fast PWM
    TCCR1B |= (1 << WGM12) | (1 << CS11);  // Prescaler = 8, starts Timer1
    
    OCR1A = 200;
    OCR0B = 200;
}

void sl() {
      TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);  // Fast PWM
    TCCR0B |= (1 << CS01);  // Prescaler = 8, starts Timer0

    TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);  // 8-bit Fast PWM
    TCCR1B |= (1 << WGM12) | (1 << CS11);  // Prescaler = 8, starts Timer1
    
    OCR0A = 200;
    OCR1B = 200;
}

void bk() {

     TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);  // Fast PWM
    TCCR0B |= (1 << CS01);  // Prescaler = 8, starts Timer0

    TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);  // 8-bit Fast PWM
    TCCR1B |= (1 << WGM12) | (1 << CS11);  // Prescaler = 8, starts Timer1
    
    OCR1A = 200;
    OCR1B = 200;
}

void motor_init() {
    DDRD |= (1 << DDD6) | (1 << DDD5);   // OC0A (PD6), OC0B (PD5)
    DDRB |= (1 << DDB1) | (1 << DDB2);   // OC1A (PB1), OC1B (PB2)
}

// --- Line Tracking ---
// Line Tracking
int track(void) {

    
    if (!(PINC & (1 << PINC0) && (PINC & (1 << PINC1)))) {
        if (PINC & (1 << PINC0)) {
            tr(50);
        } else if (PINC & (1 << PINC1)) {
            tl(50);
        } else {
            fd(40);
        }
    }
    else{
        st();
        return 1;
    }
}

void srcheck() {
    sr();
    _delay_ms(100);
    while (!(PINC & (1 << PINC1))); 
    sr();
    st();
    while ((PINC & (1 << PINC1)));
    sr();
    st();
}

void slcheck() {
    sl();
    _delay_ms(100);
    while ((PINC & (1 << PINC1))); 
    sl();
    st();
    while (!(PINC & (1 << PINC1))); 
    sl();
    st();
     while ((PINC & (1 << PINC1))); 
    sl();
    st();
    _delay_ms(100);
   
}

void Tableone() {
    char buffer[16];
     track();
//     st();
//      _delay_ms(100);
//       sprintf(buffer, "Stop");
//       lcd_print(buffer);
//     srcheck();
//     st();
//      _delay_ms(100);
//     sr();
//     st();
//     _delay_ms(100);
}