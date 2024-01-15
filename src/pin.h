#ifndef PIN_H
# define PIN_H
# include <stdint.h>
# include "port.h"
# define SET_PIN(reg, bit) ((reg) |= (1 << (bit)))
# define CLEAR_PIN(reg, bit) ((reg) &= ~(1 << (bit)))
# define READ_PIN(reg, bit) (((reg) >> (bit)) & 1)

typedef enum
{
	NC = -1,
	LOW = 0,
	HIGH
} pin_state;

typedef enum
{
    INPUT = 0,
    OUTPUT = 1
} pin_type;

typedef struct
{
	pin_type	type;
	pin_state	pull;
	uint8_t		id;
	port		port;
} pin;

void 	pin_init(pin p);
uint8_t pin_read(pin p);
void 	pin_write(pin p, uint8_t value);

#endif
