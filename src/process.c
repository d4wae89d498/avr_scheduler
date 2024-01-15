#include "process.h"
#include <avr/interrupt.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "usart.h"

uint8_t nb_processes = 0;
process processes[MAX_PROCESSES];
volatile int task = 0;
volatile int nb_blocked = 0;

sem sem_init()
{
	return DEFAULT_SEM;
}

void sem_wait(sem *s)
{
	cli();
	s->queue[s->size] = task;
	if (s->size++ > 0 && processes[task].state != BLOCKED)
	{
		nb_blocked += 1;
		processes[task].state = BLOCKED;
		scheduler_switch();
	}
	sei();
}

void sem_post(sem *s)
{
	cli();
	uint8_t i = 1;
	while (i < s->size)
	{
		s->queue[i - 1] = s->queue[i];
		i += 1;
	}
	s->size -= 1;
	if (processes[s->queue[0]].state != RUNNING)
	{
		nb_blocked -= 1;
		processes[s->queue[0]].state = RUNNING;
		scheduler_switch();
	}
	sei();
}

void scheduler_init()
{
	int i = 0;
	while (i < MAX_PROCESSES)
	{
		processes[i] = (process){
			.f = NULL,
			.stack_pointer = 0,
			.state = UNDEFINED};
		int y = 0;
		while (y < STACK_SIZE)
		{
			processes[i].stack[y++] = 0;
			y += 1;
		}
		i += 1;
	}
}

void process_run(process_function f)
{
	if (nb_processes >= MAX_PROCESSES)
	{
		// todo: handle error: too many process
	}
	int i = 0;
	while (i < MAX_PROCESSES)
	{
		if (processes[i].state == UNDEFINED)
		{
			processes[i].f = f;
			processes[i].state = NOT_RUNNING;
			processes[i].stack_pointer = (uint16_t)processes[i].stack + STACK_SIZE - 1;
			break ;
		}
		i += 1;
	}
	nb_processes += 1;
}

void scheduler_restore_task(int task)
{
}

void scheduler_switch()
{
	if (nb_processes < 1)
		return;
	cli();
	SAVE_CONTEXT();
	processes[task].stack_pointer = SP;

	while (task < MAX_PROCESSES)
	{
		task += 1;
		if (processes[task].state != UNDEFINED && (processes[task].state != BLOCKED)) // || nb_processes == nb_blocked))
			break;
	}

	if (task >= MAX_PROCESSES)
		task = 0;
	//	PRINT("switching to task #");
	//	usart_write_char('0' + task);
	//	PRINT_NL("");
	SP = processes[task].stack_pointer;
	if (processes[task].state == RUNNING)
	{
		RESTORE_CONTEXT();
	}
	else if (processes[task].state == NOT_RUNNING)
	{
		processes[task].state = RUNNING;
		sei();
		processes[task].f();
		processes[task].state = UNDEFINED;
		//		PRINT("task #");
		//		usart_write_char('0' + task);
		//		PRINT_NL(" has finished");
		nb_processes -= 1;
		scheduler_switch();
	}
	else
	{
		// TODO: handle error: unknow process state
	}
}
