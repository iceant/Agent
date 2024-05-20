#ifndef INCLUDED_CPU_ATOMIC_H
#define INCLUDED_CPU_ATOMIC_H

////////////////////////////////////////////////////////////////////////////////
////
#ifndef INCLUDED_CPU_FUNCTIONS_H
#include <cpu_functions.h>
#endif /*INCLUDED_CPU_FUNCTIONS_H*/

////////////////////////////////////////////////////////////////////////////////
////
typedef volatile cpu_int_t cpu_atomic_t;

C__STATIC_FORCEINLINE int cpu_atomic_add_return(cpu_atomic_t* v, int i)
{
    cpu_uint_t tmp;
    int result;
    cpu_DMB();
    do{
        result = cpu_LDREXW(v);
        result = result + i;
        tmp = cpu_STREXW(result, v);
    }while(tmp!=0);
    cpu_DMB();
    return result;
}


C__STATIC_FORCEINLINE int cpu_atomic_sub_return(cpu_atomic_t * v, int i){
    cpu_DMB();
    cpu_uint_t tmp;
    int result;
    do{
        result = cpu_LDREXW(v);
        result = result - i;
        tmp = cpu_STREXW(result, v);
    }while(tmp!=0);
    cpu_DMB();
    return result;
}

C__STATIC_FORCEINLINE int cpu_atomic_cmpxchg(cpu_atomic_t * v, int old_value, int new_value){
    int oldval;
    cpu_uint_t res;
    cpu_DMB();
    do{
        oldval = cpu_LDREXW(v);
        res = 0;
        if(oldval==old_value){
            res = cpu_STREXW(new_value, v);
        }
    }while(res);
    cpu_DMB();
    return old_value;
}

#endif /*INCLUDED_CPU_ATOMIC_H*/
