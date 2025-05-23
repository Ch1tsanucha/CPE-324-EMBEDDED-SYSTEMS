#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "ultrasonic_driver.h"


#define TRIG_PIN PC3
#define ECHO_PIN PC2

void setup_ultrasonic() {
    DDRC |= (1 << TRIG_PIN);
    DDRC &= ~(1 << ECHO_PIN);
    PORTC &= ~(1 << TRIG_PIN);
}

float measure_distance() {
    uint32_t duration = 0;
    uint32_t timeout = 0;

    PORTC &= ~(1 << TRIG_PIN);
    _delay_us(2);
    PORTC |= (1 << TRIG_PIN);
    _delay_us(10);
    PORTC &= ~(1 << TRIG_PIN);

    // Wait for ECHO to go high
    while (!(PINC & (1 << ECHO_PIN)) && timeout++ < 60000) _delay_us(1);
    if (timeout >= 60000) return -1.0;

    timeout = 0;
    while ((PINC & (1 << ECHO_PIN)) && timeout++ < 60000) {
        _delay_us(1);
        duration++;
    }

    if (timeout >= 60000) return -1.0;

    // Speed of sound = 343 m/s = 0.0343 cm/us -> dist = duration * 0.0343 / 2
    return duration * 0.0343 / 2;
}

