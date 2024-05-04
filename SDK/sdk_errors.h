#ifndef INCLUDED_SDK_ERRORS_H
#define INCLUDED_SDK_ERRORS_H

////////////////////////////////////////////////////////////////////////////////
////

#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

////////////////////////////////////////////////////////////////////////////////
////

#define SDK_EOK          (0)
#define SDK_ERROR        (-1)
#define SDK_ENOMEM       (-2)
#define SDK_EFULL        (-3)
#define SDK_EEMPTY       (-4)
#define SDK_ETIMEOUT     (-5)
#define SDK_EINVAL       (-6)
#define SDK_EOVERFLOW    (-7)

////////////////////////////////////////////////////////////////////////////////
////

typedef sdk_int_t sdk_err_t;

#endif /* INCLUDED_SDK_ERRORS_H */
