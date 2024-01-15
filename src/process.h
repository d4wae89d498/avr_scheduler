#ifndef PROCESS_H
# define PROCESS_H
# ifndef MAX_PROCESSES
#  define MAX_PROCESSES 9
# endif
# ifndef STACK_SIZE
#  define STACK_SIZE 128
# endif
# include <stdarg.h>
# include <stdint.h>
# define SAVE_CONTEXT()						\
  asm volatile (						\
		"push	r0				\n\t"	\
		"in		r0, __SREG__	\n\t"		\
		"cli					\n\t"	\
		"push	r0				\n\t"	\
		"push	r1				\n\t"	\
		"push	r2				\n\t"	\
		"push	r3				\n\t"	\
		"push	r4				\n\t"	\
		"push	r5				\n\t"	\
		"push	r6				\n\t"	\
		"push	r7				\n\t"	\
		"push	r8				\n\t"	\
		"push	r9				\n\t"	\
		"push	r10				\n\t"	\
		"push	r11				\n\t"	\
		"push	r12				\n\t"	\
		"push	r13				\n\t"	\
		"push	r14				\n\t"	\
		"push	r15				\n\t"	\
		"push	r16				\n\t"	\
		"push	r17				\n\t"	\
		"push	r18				\n\t"	\
		"push	r19				\n\t"	\
		"push	r20				\n\t"	\
		"push	r21				\n\t"	\
		"push	r22				\n\t"	\
		"push	r23				\n\t"	\
		"push	r24				\n\t"	\
		"push	r25				\n\t"	\
		"push	r26				\n\t"	\
		"push	r27				\n\t"	\
		"push	r28				\n\t"	\
		"push	r29				\n\t"	\
		"push	r30				\n\t"	\
		"push	r31				\n\t"	\
		 );
# define RESTORE_CONTEXT()				\
  asm volatile (						\
		"pop	r31				\n\t"	\
		"pop	r30				\n\t"	\
		"pop	r29				\n\t"	\
		"pop	r28				\n\t"	\
		"pop	r27				\n\t"	\
		"pop	r26				\n\t"	\
		"pop	r25				\n\t"	\
		"pop	r24				\n\t"	\
		"pop	r23				\n\t"	\
		"pop	r22				\n\t"	\
		"pop	r21				\n\t"	\
		"pop	r20				\n\t"	\
		"pop	r19				\n\t"	\
		"pop	r18				\n\t"	\
		"pop	r17				\n\t"	\
		"pop	r16				\n\t"	\
		"pop	r15				\n\t"	\
		"pop	r14				\n\t"	\
		"pop	r13				\n\t"	\
		"pop	r12				\n\t"	\
		"pop	r11				\n\t"	\
		"pop	r10				\n\t"	\
		"pop	r9				\n\t"	\
		"pop	r8				\n\t"	\
		"pop	r7				\n\t"	\
		"pop	r6				\n\t"	\
		"pop	r5				\n\t"	\
		"pop	r4				\n\t"	\
		"pop	r3				\n\t"	\
		"pop	r2				\n\t"	\
		"pop	r1				\n\t"	\
		"pop	r0				\n\t"	\
		"out 	__SREG__, r0	\n\t"	\
		"pop	r0				\n\t"	\
		 );
# define DEFAULT_SEM ((sem) {.size = 0})
typedef void (*process_function)(void);
typedef enum
{
	UNDEFINED = 0,
	NOT_RUNNING,
	RUNNING,
	BLOCKED
}	process_state;

typedef struct
{
	uint16_t			stack_pointer;
	process_function	f;
	process_state		state;
	uint8_t 			stack[STACK_SIZE];

}	process;

typedef struct
{
	uint8_t		queue[MAX_PROCESSES];
	uint8_t		size;
}	sem;

sem 	sem_init();
void	sem_wait(sem *s);
void	sem_post(sem *s);
void	scheduler_init();
void	process_run(process_function f);
void	scheduler_run();
void 	scheduler_switch();




#endif
