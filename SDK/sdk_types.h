#ifndef INCLUDED_SDK_TYPES_H
#define INCLUDED_SDK_TYPES_H

////////////////////////////////////////////////////////////////////////////////
////
#ifndef INCLUDED_STDINT_H
#define INCLUDED_STDINT_H
#include <stdint.h>
#endif /*INCLUDED_STDINT_H*/

#ifndef INCLUDED_STDBOOL_H
#define INCLUDED_STDBOOL_H
#include <stdbool.h>
#endif /*INCLUDED_STDBOOL_H*/

#ifndef INCLUDED_STDDEF_H
#define INCLUDED_STDDEF_H
#include <stddef.h>
#endif /*INCLUDED_STDDEF_H*/

#ifndef INCLUDED_LIMITS_H
#define INCLUDED_LIMITS_H
#include <limits.h>
#endif /*INCLUDED_LIMITS_H*/

#ifndef INCLUDED_INTTYPES_H
#define INCLUDED_INTTYPES_H
#include <inttypes.h>
#endif /*INCLUDED_INTTYPES_H*/


////////////////////////////////////////////////////////////////////////////////
////

#if defined(CPU_32BIT)
    typedef int32_t         sdk_int_t;
    typedef uint32_t        sdk_uint_t;
    typedef sdk_int_t       sdk_intptr_t;
    typedef sdk_uint_t      sdk_uintptr_t;
    typedef sdk_uint_t      sdk_size_t;
    #define SDK_PRId        PRId32
    #define SDK_PRIi        PRIi32
    #define SDK_PRIo        PRIo32
    #define SDK_PRIu        PRIu32
    #define SDK_PRIx        PRIx32
    #define SDK_PRIX        PRIX32
#elif defined(CPU_64BIT)
    typedef int64_t         sdk_int_t;
    typedef uint64_t        sdk_uint_t;
    typedef sdk_int_t       sdk_intptr_t;
    typedef sdk_uint_t      sdk_uintptr_t;
    typedef sdk_uint_t      sdk_size_t;
    #define SDK_PRId        PRId64
    #define SDK_PRIi        PRIi64
    #define SDK_PRIo        PRIo64
    #define SDK_PRIu        PRIu64
    #define SDK_PRIx        PRIx64
    #define SDK_PRIX        PRIX64
#endif

#define sdk_bool_t  bool
#define SDK_TRUE    true
#define SDK_FALSE   false


#endif /* INCLUDED_SDK_TYPES_H */
