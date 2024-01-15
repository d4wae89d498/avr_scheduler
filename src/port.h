#ifndef PORT_H
# define PORT_H
# include <stdint.h>
# define A ((port){.ddr=&DDRA, .port=&PORTA, .pin=&PINA})
# define B ((port){.ddr=&DDRB, .port=&PORTB, .pin=&PINB})
# define C ((port){.ddr=&DDRC, .port=&PORTC, .pin=&PINC})
# define D ((port){.ddr=&DDRD, .port=&PORTD, .pin=&PIND})

typedef struct
{
	uint8_t	*ddr;
	uint8_t	*port;
	uint8_t	*pin;
} port;

#endif
