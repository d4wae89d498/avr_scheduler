#include "usart.h"
#include "pin.h"
#include "process.h"

static const pin internal_light = { .port = B, .id = 5, .type = OUTPUT };
static const pin red_light 		= { .port = B, .id = 4, .type = OUTPUT };
static const pin blue_light 	= { .port = B, .id = 3, .type = OUTPUT };
static const pin white_light 	= {	.port = B, .id = 2, .type = OUTPUT };
static const pin input1 		= { .port = B, .id = 1, .type = INPUT, .pull = NC };

static sem serial_sem = DEFAULT_SEM;

#define serial_safe_write(X) _serial_safe_write(X, sizeof(X) - 1)
static void	_serial_safe_write(char *str, int len)
{
	sem_wait(&serial_sem);
	usart_write_str_nl(str, len);
	sem_post(&serial_sem);
}

static void		task_red()
{
	while (1)
	{
		serial_safe_write("bonjour le monde!! <very very long string !!!!!!!!!! Yey not long enough>");
	//	pin_write(red_light, 0);
	//	_delay_ms(1000);
	//	pin_write(red_light, 1);
	//	_delay_ms(1000);
	}
}

static void		task_blue()
{
	while (1)
	{
		serial_safe_write("hello world! <very very long string !!!!!!!!!! Yey not long enough>");
	//	pin_write(blue_light, 1);
	//	_delay_ms(700);
	//	pin_write(blue_light, 0);
	//	_delay_ms(700);
	}
}


static void		task_serial()
{
	int i = 0;
	while (1)
	{
		serial_safe_write("hola quetal! <very very long string !!!!!!!!!! Yey not long enough>");

	//	_delay_ms(700);
	//	PRINT("c: [");
	//	usart_write_char('0' + i);
	//	PRINT_NL("]");
	//	_delay_ms(700);
		i = i + 1;
		if (i >= 10)
			i = 0;
	}
}

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
}

int main(void) {
	setup();

	process_run(task_blue);
	process_run(task_serial);
	process_run(task_red);

	_delay_ms(777);
	PRINT_NL("Scheduler started");
	PRINT_NL("- - - - - - - - -");

	sei();
	while (1)
		;
   	return 0;
}

