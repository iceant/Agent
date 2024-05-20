#ifndef INCLUDED_CPU_H
#define INCLUDED_CPU_H

#define CPU_32BIT



////////////////////////////////////////////////////////////////////////////////
////



#ifndef INCLUDED_RTOS_COMPILER_H
#include <rtos_compiler.h>
#endif /*INCLUDED_RTOS_COMPILER_H*/

#ifndef INCLUDED_CPU_TYPES_H
#include <cpu_types.h>
#endif /*INCLUDED_CPU_TYPES_H*/



////////////////////////////////////////////////////////////////////////////////
////

#if   defined ( __CC_ARM )
/*ARMCC*/
    #include "ARMCC/cpu_functions.h"
#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
/*Clang*/
    #include "CLANG/cpu_functions.h"
#elif defined ( __GNUC__ )
/*GCC*/
    #include "GCC/cpu_functions.h"
#endif


////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_CPU_SPINLOCK_H
#include <cpu_spinlock.h>
#endif /*INCLUDED_CPU_SPINLOCK_H*/

#ifndef INCLUDED_CPU_INTERRUPT_H
#include <cpu_interrupt.h>
#endif /*INCLUDED_CPU_INTERRUPT_H*/

#ifndef INCLUDED_CPU_ATOMIC_H
#include <cpu_atomic.h>
#endif /*INCLUDED_CPU_ATOMIC_H*/

#ifndef INCLUDED_CPU_LOCK_H
#include <cpu_lock.h>
#endif /*INCLUDED_CPU_LOCK_H*/

#ifndef INCLUDED_CPU_LOCK_H
#include <cpu_lock.h>
#endif /*INCLUDED_CPU_LOCK_H*/






#endif /*INCLUDED_CPU_H*/
