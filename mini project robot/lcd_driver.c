#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "lcd_driver.h"
#define LCD_ADDR 0x4E  // ???????? 0x27 << 1 = 0x4E (7-bit addr 0x27)

// LCD control bits
#define LCD_EN  0x04    // Enable bit
#define LCD_RW  0x02    // Read/Write bit (0 = write)
#define LCD_RS  0x01    // Register select bit (0 = cmd, 1 = data)
#define LCD_BACKLIGHT 0x08




void i2c_init() {
    TWBR = 72;      // 16MHz, 100kHz SCL
    TWSR = 0x00;   
    TWCR = (1<<TWEN); // enable TWI
}

void i2c_start() {
    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
}

void i2c_stop() {
    TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
    while (TWCR & (1<<TWSTO)); 
}

void i2c_write(uint8_t data) {
    TWDR = data;
    TWCR = (1<<TWINT) | (1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
}

uint8_t i2c_read_ack() {
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
    while (!(TWCR & (1<<TWINT)));
    return TWDR;
}

uint8_t i2c_read_nack() {
    TWCR = (1<<TWINT) | (1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    return TWDR;
}

void lcd_send_nibble(uint8_t nibble, uint8_t control) {

    uint8_t data = (nibble & 0xF0) | control | LCD_BACKLIGHT;
    
    i2c_start();
    i2c_write(LCD_ADDR);
    i2c_write(data | LCD_EN);  
    _delay_us(1);
    i2c_write(data & ~LCD_EN); 
    i2c_stop();
    _delay_us(50);
}


void lcd_cmd(uint8_t cmd) {
    lcd_send_nibble(cmd & 0xF0, 0x00);        
    lcd_send_nibble((cmd << 4) & 0xF0, 0x00);
    if (cmd == 0x01 || cmd == 0x02) _delay_ms(2);
}

void lcd_data(uint8_t data) {
    lcd_send_nibble(data & 0xF0, LCD_RS);        
    lcd_send_nibble((data << 4) & 0xF0, LCD_RS); 
    _delay_us(50);
}


void lcd_init() {
    _delay_ms(50);      
    
    lcd_send_nibble(0x30, 0x00);
    _delay_ms(5);
    lcd_send_nibble(0x30, 0x00);
    _delay_us(150);
    lcd_send_nibble(0x30, 0x00);
    _delay_us(150);
    
    lcd_send_nibble(0x20, 0x00);  
    _delay_us(150);
    
    lcd_cmd(0x28); // 4-bit, 2 lines, 5x8 font
    lcd_cmd(0x0C); // Display ON, Cursor OFF
    lcd_cmd(0x06); // Entry mode: Increment cursor
    lcd_cmd(0x01); // Clear display
    _delay_ms(2);
}

void lcd_print(char *str) {
    while (*str) {
        lcd_data(*str++);
    }
}