#ifndef USART_H
# include <stdint.h>
# include <avr/interrupt.h>
# include <avr/io.h>
# include <util/delay.h>
# define USART_H
# define PRINT(X) (usart_write_str(X, sizeof(X) - 1))
# define PRINT_NL(X) (usart_write_str_nl(X, sizeof(X) - 1))

void usart_init();
void usart_write_char(char data);
void usart_write_str(char *str, uint16_t len);
void usart_write_str_nl(char *str, uint16_t len);

#endif
