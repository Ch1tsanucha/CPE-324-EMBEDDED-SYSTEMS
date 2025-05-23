#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lcd_driver.h"
#include "keypad_driver.h"
#include "motor_driver.h"
#include "ultrasonic_driver.h"

volatile uint8_t update_lcd_flag = 0;
int flag = 1;

typedef struct {
    char name[8];
    int encoder_position;
} TableMap;

ISR(TIMER1_COMPA_vect) {
    update_lcd_flag = 1;
}

void setup_timer1() {
    TCCR1A = 0;
    TCCR1B = (1 << WGM12);                // CTC mode
    OCR1A = 7811;                         // ~1Hz with 1024 prescaler
    TIMSK1 |= (1 << OCIE1A);              // Enable compare interrupt
    TCCR1B |= (1 << CS12) | (1 << CS10);  // Start timer with 1024 prescaler
}

void global_setup() {
    cli();
    i2c_init();
    lcd_init();
    setup_ultrasonic();
    motor_init();
    setup_timer1();

    DDRC &= ~((1 << PINC0) | (1 << PINC1));
    PORTC |= ((1 << PINC0) | (1 << PINC1));
    sei(); // enable global interrupts
}

void wait_for_key(char *key_out) {
    char key;
    do {
        key = keypad_scan();
        _delay_ms(50);
    } while (key == '\0');
    *key_out = key;
}

void turn_until_line(uint8_t pin, void (*turn_func)(), char *buffer) {
    while ((PINC & (1 << pin))) {
        lcd_cmd(0x01);
        sprintf(buffer, "Turning...");
        lcd_print(buffer);
        turn_func();
        _delay_ms(50);
    }
    st();
}

void execute_path_sequence(char direction_key, char *buffer) {
    lcd_cmd(0x01);
    sprintf(buffer, "Tracking...");
    lcd_print(buffer);

    if (track() == 1) {
        fd(50);
        _delay_ms(200);
        st();
        _delay_ms(50);

        lcd_cmd(0x01);
        sprintf(buffer, "Spinning...");
        lcd_print(buffer);

        if (direction_key == '1') {
            // left turn
            turn_until_line(PINC0, sl, buffer);
            turn_until_line(PINC0, sl, buffer);
            turn_until_line(PINC0, sl, buffer);
        } else if (direction_key == '6') {
            // right turn
            turn_until_line(PINC1, sr, buffer);
            turn_until_line(PINC1, sr, buffer);
            turn_until_line(PINC1, sr, buffer);
        }

        while (track() != 1);
        bk();
        _delay_ms(200);
        st();
        _delay_ms(200);

        turn_until_line(PINC0, sl, buffer);
        turn_until_line(PINC0, sl, buffer);
        turn_until_line(PINC0, sl, buffer);

        st();
        _delay_ms(2000);

        while (track() != 1);
        fd(50);
        _delay_ms(200);
        st();
        _delay_ms(50);

        lcd_cmd(0x01);
        sprintf(buffer, "Final spin...");
        lcd_print(buffer);

        turn_until_line(PINC0, sl, buffer);
        turn_until_line(PINC0, sl, buffer);
        turn_until_line(PINC0, sl, buffer);
    }
}

int main(void) {
    TableMap tables[9];
    char key = '\0';
    int line = 0;
    char buffer[32];

    for (int i = 0; i < 9; i++) {
        sprintf(tables[i].name, "%d", i + 1);
        tables[i].encoder_position = i + 1;
    }

    global_setup();

    lcd_cmd(0x01);
    lcd_cmd(0x80);
    lcd_cmd(0xC0);
    sprintf(buffer, "Started");
    lcd_print(buffer);
    _delay_ms(1000);

    lcd_cmd(0x01);
    sprintf(buffer, "Select table:");
    lcd_print(buffer);
    wait_for_key(&key);

    lcd_cmd(0x01);
    sprintf(buffer, "Key: %c", key);
    lcd_print(buffer);
    _delay_ms(500);

    for (int i = 0; i < 9; i++) {
        if (tables[i].name[0] == key) {
            line = tables[i].encoder_position;
            lcd_cmd(0x01);
            sprintf(buffer, "A%c", key);
            lcd_print(buffer);
            break;
        }
    }

    while (1) {
        if (update_lcd_flag) {
            update_lcd_flag = 0;

            float distance_cm = measure_distance();
            if (distance_cm < 10 && flag) {
                st();
                lcd_cmd(0x01);
                sprintf(buffer, "Stop");
                lcd_print(buffer);
                flag = 0;
            } else {
                flag = 1;
            }
        }

        if (flag) {
            if (key == '0') {
                lcd_cmd(0x01);
                sprintf(buffer, "Waiting...");
                lcd_print(buffer);
                wait_for_key(&key);
                lcd_cmd(0x01);
                sprintf(buffer, "Key: %c", key);
                lcd_print(buffer);
                _delay_ms(500);
            }

            if (key == '1' || key == '6') {
                execute_path_sequence(key, buffer);
                key = '0'; // reset key to loop again
            }
        }

        _delay_ms(10);
    }

    return 0;
}
