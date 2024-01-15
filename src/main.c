#include "usart.h"
#include "pin.h"
#include "process.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>


static const pin 	internal_light = {
	.port = B,
	.id = 5,
	.type = OUTPUT,
};

static const pin 	red_light = {
	.port = B,
	.id = 4,
	.type = OUTPUT,
};

static const pin 	blue_light = {
	.port = B,
	.id = 3,
	.type = OUTPUT,
};

static const pin 	white_light = {
	.port = B,
	.id = 2,
	.type = OUTPUT,
};

static const pin 	input1 = {
	.port = B,
	.id = 1,
	.type = INPUT,
	.pull = NC
};

static void	setup(void)
{
	cli();
    usart_init();
	pin_init(internal_light);
	pin_init(red_light);
	pin_init(blue_light);
	pin_init(white_light);
	pin_init(input1);
	scheduler_init();
	TCCR2B = 0b00000100; // Clock / 256 soit 16 micro-s et WGM22 = 0
  	TIMSK2 = 0b00000001; // Interruption locale autorisée par TOIE2
}

void		task_red()
{
	while (1)
	{
		pin_write(red_light, 0);
		_delay_ms(1000);
		pin_write(red_light, 1);
		_delay_ms(1000);
	}
}

void		task_blue()
{
	while (1)
	{
		pin_write(blue_light, 1);
		_delay_ms(700);
		pin_write(blue_light, 0);
		_delay_ms(700);
	}
}


void		task_serial()
{
	int i = 0;
	while (1)
	{
		_delay_ms(700);
		PRINT("c: [");
		usart_write_char('0' + i);
		PRINT_NL("]");
		_delay_ms(700);
		i = i + 1;
		if (i >= 10)
			i = 0;
	}
}


int 	tick = 0;
int		white_state = 0;

ISR(TIMER2_OVF_vect)
{

	tick += 1;
	if (tick >= 500)
	{
		if (!white_state)
		{
			pin_write(white_light, 1);
			white_state = 1;
		}
		else
		{
			pin_write(white_light, 0);
			white_state = 0;
		}
		tick = 0;
		scheduler_switch_to();
	}
}


int main(void) {
	setup();



	process_run(task_blue);
	process_run(task_serial);
	process_run(task_red);

	sei();

	while (1)
		;
   	return 0;
}

