#include "pin.h"

void 	pin_init(pin p) {

	if (p.type == OUTPUT)
		SET_PIN(*p.port.ddr, p.id);
	else
	{
		CLEAR_PIN(*p.port.ddr, p.id);
		if (p.pull == HIGH)
			SET_PIN(*p.port.port, p.id);
		else if (p.pull == LOW)
			CLEAR_PIN(*p.port.port, p.id);
	}
}

void	pin_write(pin p, uint8_t v)
{
	if (v)
		SET_PIN(*p.port.port, p.id);
    else
		CLEAR_PIN(*p.port.port, p.id);
}

uint8_t pin_read(pin p)
{
	return READ_PIN(*p.port.pin, p.id);
}
