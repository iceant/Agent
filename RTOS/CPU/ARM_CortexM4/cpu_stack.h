#ifndef INCLUDED_CPU_STACK_H
#define INCLUDED_CPU_STACK_H

////////////////////////////////////////////////////////////////////////////////
////
#ifndef INCLUDED_CPU_TYPES_H
#include <cpu_types.h>
#endif /*INCLUDED_CPU_TYPES_H*/

#ifndef INCLUDED_CPU_REGISTERS_H
#include <cpu_registers.h>
#endif /*INCLUDED_CPU_REGISTERS_H*/


////////////////////////////////////////////////////////////////////////////////
////
#define CPU_STACK_SWITCH_FLAG_ON    1
#define CPU_STACK_SWITCH_FLAG_OFF   0

////////////////////////////////////////////////////////////////////////////////
////
extern void** cpu_stack__curr_p;
extern void** cpu_stack__next_p;
extern volatile uint8_t cpu_stack__switch_flag;


////////////////////////////////////////////////////////////////////////////////
////
int cpu_stack_init(void* thread_entry, void* parameter, uint8_t * stack_address, int stack_size, void** return_sp);


int cpu_stack_switch(void** from_stack_p, void** to_stack_p);



#endif /*INCLUDED_CPU_STACK_H*/
