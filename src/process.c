#include "process.h"
#include <avr/interrupt.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "usart.h"

uint8_t	nb_processes = 0;
process processes[MAX_PROCESSES];

void	scheduler_init()
{
	int i = 0;
	while (i < MAX_PROCESSES)
	{
		processes[i++] = (process) {
			.f = NULL,
			.stack_pointer = 0,
			.state = UNDEFINED
		};
		int y = 0;
		while (y < STACK_SIZE)
		{
			processes[i].stack[y] = 0;
			y += 1;
		}
		i += 1;
	}
}

void process_run(process_function f)
{
	processes[nb_processes].f = f;
	processes[nb_processes].state = NOT_RUNNING;
	//if (nb_processes)
	//	processes[nb_processes].stack_pointer = 0x500;
	//else
	//	processes[nb_processes].stack_pointer = 0x700;
	processes[nb_processes].stack_pointer = (uint16_t) processes[nb_processes].stack + STACK_SIZE - 1;
	nb_processes += 1;

}
void	scheduler_switch_to()
{
	static int 	task;
	if (nb_processes < 2)
        return;
	cli();
	PRINT("current task was: ");
	usart_write_char('0' + task);
	PRINT(": ");
	if (processes[task].state == RUNNING)
	{
		PRINT_NL("running task has paused");
		SAVE_CONTEXT();
		processes[task].stack_pointer = SP;
	}
	else
		PRINT_NL("was not running");
	task = !task;
	PRINT("new task is: ");
	usart_write_char('0' + task);
	PRINT(": ");
	SP = processes[task].stack_pointer;
	if (processes[task].state == NOT_RUNNING)
	{
		PRINT_NL(" launching task");
		process_function f = processes[task].f;
		processes[task].state = RUNNING;
		f();
	}
	else if (processes[task].state == RUNNING)
	{
		PRINT_NL(" resuming");
		RESTORE_CONTEXT();
		sei();
	}
	else
		; // TODO : handle error
}
