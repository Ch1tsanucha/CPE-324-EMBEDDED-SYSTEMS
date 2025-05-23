#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "keypad_driver.h"

#define KEYPAD_ROW_DDR    DDRD
#define KEYPAD_ROW_PORT   PORTD
#define KEYPAD_ROW_PIN    PIND

#define KEYPAD_COL_DDRD   DDRD
#define KEYPAD_COL_PORTD  PORTD
#define KEYPAD_COL_PIND   PIND

#define KEYPAD_COL_DDRB   DDRB
#define KEYPAD_COL_PORTB  PORTB
#define KEYPAD_COL_PINB   PINB

#define ROW0          PD0
#define ROW1          PD1
#define ROW2          PD2
#define ROW3          PD3

#define COL0          PD4
#define COL1          PD7
#define COL2          PB6
#define COL3          PB0

char keypad_scan() {
    const char keypad[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    uint8_t row_pins[4] = {ROW0, ROW1, ROW2, ROW3};
    uint8_t col_pins[4] = {COL0, COL1, COL2, COL3};

    // Initialize rows as outputs and set HIGH
    KEYPAD_ROW_DDR |= (1 << ROW0) | (1 << ROW1) | (1 << ROW2) | (1 << ROW3);
    KEYPAD_ROW_PORT |= (1 << ROW0) | (1 << ROW1) | (1 << ROW2) | (1 << ROW3);

    // Initialize columns as inputs with pull-ups enabled
    // Columns on PORTD
    KEYPAD_COL_DDRD &= ~((1 << COL0) | (1 << COL1));      // COL0 and COL1 pins on PORTD?
    KEYPAD_COL_PORTD |= (1 << COL0) | (1 << COL1);

    // Columns on PORTB
    KEYPAD_COL_DDRB &= ~((1 << COL2) | (1 << COL3));     
    KEYPAD_COL_PORTB |= (1 << COL2) | (1 << COL3);

    for (uint8_t row = 0; row < 4; row++) {
        // Drive current row LOW
        KEYPAD_ROW_PORT &= ~(1 << row_pins[row]);

        _delay_us(5); // wait for signal to settle

        for (uint8_t col = 0; col < 4; col++) {
            uint8_t pressed = 0;

            if (col == 0 || col == 1) {
                // COL0 and COL1 are on PORTD
                pressed = ! (KEYPAD_COL_PIND & (1 << col_pins[col]));
            } else if (col == 2 || col == 3) {
                // COL2 and COL3 are on PORTB
                pressed = ! (KEYPAD_COL_PINB & (1 << col_pins[col]));
            }

            if (pressed) {
                _delay_ms(20); // debounce delay

                // Re-check after debounce delay
                if (col == 0 || col == 1) {
                    pressed = !(KEYPAD_COL_PIND & (1 << col_pins[col]));
                } else if (col == 2 || col == 3) {
                    pressed = !(KEYPAD_COL_PINB & (1 << col_pins[col]));
                }

                if (pressed) {
                    // Wait for key release
                    while (1) {
                        if (col == 0 || col == 1) {
                            pressed = !(KEYPAD_COL_PIND & (1 << col_pins[col]));
                        } else if (col == 2 || col == 3) {
                            pressed = !(KEYPAD_COL_PINB & (1 << col_pins[col]));
                        }
                        if (!pressed) break;
                    }

                    // Set current row HIGH before returning
                    KEYPAD_ROW_PORT |= (1 << row_pins[row]);

                    return keypad[row][col];
                }
            }
        }

        // Set current row HIGH again after scanning
        KEYPAD_ROW_PORT |= (1 << row_pins[row]);
    }

    return '\0'; // No key pressed
}

