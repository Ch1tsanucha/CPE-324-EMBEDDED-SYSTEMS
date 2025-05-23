#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <stdlib.h>
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
    TCCR1B = (1 << WGM12);
    OCR1A = 7811; // ~1Hz interrupt
    TIMSK1 |= (1 << OCIE1A);
    TCCR1B |= (1 << CS12) | (1 << CS10); // prescaler 1024
}

void global_setup() {
    cli();
    i2c_init();
    lcd_init();
    setup_ultrasonic();
    motor_init();
    setup_timer1();
    sei(); // Enable global interrupts
}

int main(void) {
    TableMap tables[10];
    int line;

    for (int i = 0; i < 9; i++) {
        sprintf(tables[i].name, "%d", i + 1);
        tables[i].encoder_position = i + 1;
    }

    global_setup();

    // Set line sensor pins as input with pull-up
    DDRC &= ~((1 << PINC0) | (1 << PINC1));
    PORTC |= ((1 << PINC0) | (1 << PINC1));

    char *buffer = (char *)malloc(32 * sizeof(char));
    if (buffer == NULL) return 0;

    lcd_cmd(0x01);
    _delay_ms(2);
    lcd_cmd(0x80);
    lcd_cmd(0xC0);
    sprintf(buffer, "Started");
    lcd_print(buffer);
    _delay_ms(1000);

    char key;
    while (1) {
        lcd_cmd(0x01);
        key = keypad_scan();
        _delay_ms(2);
        if (key != '\0') {
            lcd_data(key);
            _delay_ms(500);
            lcd_cmd(0x01);
            break;
        }
    }

    for (int i = 0; i < 9; i++) {
        if (tables[i].name[0] == key) {
            lcd_cmd(0x01);
            line = tables[i].encoder_position;
            sprintf(buffer, "A%c", tables[i].name[0]);
            lcd_print(buffer);
            break;
        }
    }

    while (1) {
        if (flag) {
            if (key == '0') {
        lcd_cmd(0x01);
        _delay_ms(2);
        lcd_cmd(0x80);
        sprintf(buffer, "Waiting...");
        lcd_print(buffer);
        _delay_ms(500);

        while (1) {
            key = keypad_scan();
            if (key != '\0') {
                lcd_cmd(0x01);
                lcd_data(key);
                _delay_ms(500);
                break;
            }
        }
    }
            if (key == '1') {
                sprintf(buffer, "track");
                lcd_print(buffer);
                if (track() == 1) {
                    fd(50);
                    _delay_ms(200);
                    st();
                    _delay_ms(50);

                    lcd_cmd(0x01);
                    sprintf(buffer, "spin");
                    lcd_print(buffer);

                    while ((PINC & (1 << PINC0))) {
                        sprintf(buffer, "turn");
                        lcd_print(buffer);
                        sl();
                    }
                    st();

                    while (!(PINC & (1 << PINC0))) {
                        sprintf(buffer, "turn");
                        lcd_print(buffer);
                        sl();
                    }
                    st();

                    while ((PINC & (1 << PINC0))) {
                        sprintf(buffer, "turn");
                        lcd_print(buffer);
                        sl();
                    }
                    st();

                    while (track() != 1);
                    bk();
                    _delay_ms(200);
                    st();
                    _delay_ms(200);
                    
                    while ((PINC & (1 << PINC0))) {
                        sprintf(buffer, "turn");
                        lcd_print(buffer);
                        sl();
                    }
                    st();

                    while (!(PINC & (1 << PINC0))) {
                        sprintf(buffer, "turn");
                        lcd_print(buffer);
                        sl();
                    }
                    st();

                    while ((PINC & (1 << PINC0))) {
                        sprintf(buffer, "turn");
                        lcd_print(buffer);
                        sl();
                    }
                    st();
                    
                     st();
                    _delay_ms(2000);
                    
                    while (track() != 1);
                    fd(50);
                    _delay_ms(200);
                    st();
                    _delay_ms(50);

                    lcd_cmd(0x01);
                    sprintf(buffer, "spin");
                    lcd_print(buffer);
                    
                      while ((PINC & (1 << PINC0))) {
                        sprintf(buffer, "turn");
                        lcd_print(buffer);
                        sl();
                    }
                    st();

                    while (!(PINC & (1 << PINC0))) {
                        sprintf(buffer, "turn");
                        lcd_print(buffer);
                        sl();
                    }
                    st();

                    while ((PINC & (1 << PINC0))) {
                        sprintf(buffer, "turn");
                        lcd_print(buffer);
                        sl();
                    }
                    st();
                    
        
                    
                  
                    key = '0';
                  
                    
                }
            }

            if (key == '6') {
                sprintf(buffer, "track");
                lcd_print(buffer);
                if (track() == 1) {
                    fd(50);
                    _delay_ms(300);
                    st();
                    _delay_ms(50);

                    lcd_cmd(0x01);
                    sprintf(buffer, "spin");
                    lcd_print(buffer);

                    while ((PINC & (1 << PINC1))) {
                        sprintf(buffer, "turn");
                        lcd_print(buffer);
                        sr();
                    }
                    st();

                    while (!(PINC & (1 << PINC1))) {
                        sprintf(buffer, "turn");
                        lcd_print(buffer);
                        sr();
                    }
                    st();

                    while ((PINC & (1 << PINC1))) {
                        sprintf(buffer, "turn");
                        lcd_print(buffer);
                        sr();
                    }
                    st();

                    _delay_ms(1000);
                    
                     while (track() != 1);
                    bk();
                    _delay_ms(200);
                    st();
                    _delay_ms(200);
                    
                    while ((PINC & (1 << PINC0))) {
                        sprintf(buffer, "turn");
                        lcd_print(buffer);
                        sl();
                    }
                    st();

                    while (!(PINC & (1 << PINC0))) {
                        sprintf(buffer, "turn");
                        lcd_print(buffer);
                        sl();
                    }
                    st();

                    while ((PINC & (1 << PINC0))) {
                        sprintf(buffer, "turn");
                        lcd_print(buffer);
                        sl();
                    }
                    st();
                    
                     st();
                    _delay_ms(2000);
                    
                    while (track() != 1);
                    fd(50);
                    _delay_ms(200);
                    st();
                    _delay_ms(50);

                    lcd_cmd(0x01);
                    sprintf(buffer, "spin");
                    lcd_print(buffer);
                    
                      while ((PINC & (1 << PINC0))) {
                        sprintf(buffer, "turn");
                        lcd_print(buffer);
                        sl();
                    }
                    st();

                    while (!(PINC & (1 << PINC0))) {
                        sprintf(buffer, "turn");
                        lcd_print(buffer);
                        sl();
                    }
                    st();

                    while ((PINC & (1 << PINC0))) {
                        sprintf(buffer, "turn");
                        lcd_print(buffer);
                        sl();
                    }
                    st();
                    
                        while (track() != 1);
                    bk();
                    _delay_ms(200);
                    st();
                    _delay_ms(200);
                    
                    while ((PINC & (1 << PINC0))) {
                        sprintf(buffer, "turn");
                        lcd_print(buffer);
                        sl();
                    }
                    st();

                    while (!(PINC & (1 << PINC0))) {
                        sprintf(buffer, "turn");
                        lcd_print(buffer);
                        sl();
                    }
                    st();

                    while ((PINC & (1 << PINC0))) {
                        sprintf(buffer, "turn");
                        lcd_print(buffer);
                        sl();
                    }
                    st();
                    
                    
                    key = '0';
                }
            }
        }

        if (update_lcd_flag) {
            update_lcd_flag = 0;

            float distance_cm = measure_distance();
            lcd_cmd(0x01);
            _delay_ms(2);
            lcd_cmd(0x80);

            if (distance_cm < 10 && flag) {
                st();
                sprintf(buffer, "Stop");
                lcd_print(buffer);
                flag = 0;
            } else {
                flag = 1;
            }
        }

        _delay_us(10000); // 10ms
    }

    free(buffer);
    return 0;
}