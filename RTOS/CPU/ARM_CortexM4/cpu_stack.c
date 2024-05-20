#include <cpu_stack.h>
#include <cpu_functions.h>
#include <cpu_lock.h>
////////////////////////////////////////////////////////////////////////////////
////

#define ALIGN_DOWN(size, align) ((size) & ~((align) - 1))

#ifndef CPU_REG
#define CPU_REG(ADDRESS)  (*((volatile cpu_uint_t *)(ADDRESS)))
#endif
////////////////////////////////////////////////////////////////////////////////
////


typedef struct cpu_stack_exception_frame_s{
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t psr;
}cpu_stack_exception_frame_t;

typedef struct cpu_stack_frame_s{
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    cpu_stack_exception_frame_t exception_frame;
}cpu_stack_frame_t;

////////////////////////////////////////////////////////////////////////////////
////
static volatile uint8_t cpu_stack__switch_flag = CPU_STACK_SWITCH_FLAG_OFF;
static void** cpu_stack__curr_p=0;
static void** cpu_stack__next_p=0;
static cpu_lock_t cpu_stack__lock = 0;

////////////////////////////////////////////////////////////////////////////////
////

/*
17  PSR
16  PC
15  LR
14  R12
13  R3
12  R2
11  R1
10  R0          <<-- PSP
09  R11
08  R10
07  R9
06  R8
05  R7
04  R6
03  R5
02  R4
01  CONTROL
00  EXC_RETURN/LR              <<--- PSP_array[0]
--------------------
 */

int cpu_stack_init(void* thread_entry, void* parameter, uint8_t * stack_addr, int stack_size, void** return_sp)
{
    if (stack_size < 18 * 4) return -1;

    cpu_uint_t sp = ((cpu_uint_t) stack_addr + stack_size - 18 * 4);

    CPU_REG(sp + (17 << 2)) = (cpu_uint_t) 0x01000000; /*xPSR*/
    CPU_REG(sp + (16 << 2)) = (cpu_uint_t) thread_entry; /*PC*/
    CPU_REG(sp + (10 << 2)) = (cpu_uint_t) parameter; /*R0*/
    CPU_REG(sp + (1 << 2)) = (cpu_uint_t) 0x03; /*Init CONTROL register with NO_PRIVILEGE|NO_FP*/
    CPU_REG(sp) = (cpu_uint_t) 0xFFFFFFFDUL; /*EXC_RETURN*/

    if (return_sp) {
        *return_sp = (void *) sp;
    }

    return 0;

}

#if defined(__CC_ARM)
void __svc( 0 ) cpu_stack_switch_in_privilege( void ) ;
#elif defined(__GNUC__)
#define cpu_stack_switch_in_privilege() C__ASM volatile ("svc 0":::"memory")
#endif

int cpu_stack_switch(void** from_stack_p, void** to_stack_p)
{
    if(cpu_stack__switch_flag==CPU_STACK_SWITCH_FLAG_ON){
        return -1;
    }

    cpu_lock_lock(&cpu_stack__lock);
    cpu_stack__switch_flag = CPU_STACK_SWITCH_FLAG_ON;
    cpu_stack__curr_p = from_stack_p;
    cpu_stack__next_p = from_stack_p;
    cpu_lock_unlock(&cpu_stack__lock);

    if(cpu_in_privilege()){
        /*设置中断需要特权，已在特权模式，直接设置*/
        CPU_REG(SCB_ICSR) |= SCB_ICSR_PENDSVSET_Msk;
    }else{
        cpu_stack_switch_in_privilege();
    }
}