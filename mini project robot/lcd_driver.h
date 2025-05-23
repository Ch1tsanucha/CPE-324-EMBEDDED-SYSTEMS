#ifndef LCD_DRIVER_H
#define	LCD_DRIVER_H

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>

//#define F_CPU 16000000UL
//#define LCD_ADDR 0x4E  // 0x27 << 1 = 0x4E (7-bit address)
//#define LCD_EN  0x04
//#define LCD_RW  0x02
//#define LCD_RS  0x01
//#define LCD_BACKLIGHT 0x08

// I2C functions
void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_write(uint8_t data);
uint8_t i2c_read_ack(void);
uint8_t i2c_read_nack(void);

// LCD functions
void lcd_send_nibble(uint8_t nibble, uint8_t control);
void lcd_cmd(uint8_t cmd);
void lcd_data(uint8_t data);
void lcd_init(void);
void lcd_print(char *str);

#endif	/* LCD_DRIVER_H */
