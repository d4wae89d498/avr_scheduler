#include "usart.h"
#include <avr/io.h>
#define BAUDRATE 103

void usart_init() {
  // THIS IS GIVEN
  /* ACHTUNG : we suppose UBRR value < 0xff */
  /* Not true in all case */
  uint8_t baudrate = BAUDRATE;
  /* Set baud rate */
  UBRR0H = 0;
  UBRR0L = baudrate;

  /* Enable transmitter */
  UCSR0B = (1<<TXEN0);

  /* Set frame format */
  UCSR0C = 0x06;
}

void usart_write_char(char data) {
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)));
    // Put data into buffer, sends the data
    UDR0 = data;
}

void usart_write_str(char *str, uint16_t len) {
	while (len--)
		usart_write_char(*str++);
}

void usart_write_str_nl(char *str, uint16_t len) {
	while (len--)
		usart_write_char(*str++);
	usart_write_char('\r');
	usart_write_char('\n');
}
