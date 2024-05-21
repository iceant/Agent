#ifndef INCLUDED_OS_SCHEDULER_H
#define INCLUDED_OS_SCHEDULER_H

////////////////////////////////////////////////////////////////////////////////
////
#ifndef INCLUDED_OS_TYPES_H
#include <os_types.h>
#endif /*INCLUDED_OS_TYPES_H*/

#ifndef INCLUDED_OS_ERRORS_H
#include <os_errors.h>
#endif /*INCLUDED_OS_ERRORS_H*/

#ifndef INCLUDED_OS_THREAD_H
#include <os_thread.h>
#endif /*INCLUDED_OS_THREAD_H*/

////////////////////////////////////////////////////////////////////////////////
////
#define OS_SCHEDULER_EOK        OS_EOK
#define OS_SCHEDULER_ERROR      OS_ERROR
#define OS_SCHEDULER_EINWORK    (0x1001)

#define OS_SCHEDULER_STATE_UNINITIALIZED    (0)
#define OS_SCHEDULER_STATE_INITIALIZED      (1)
#define OS_SCHEDULER_STATE_STARTED          (2)
#define OS_SCHEDULER_STATE_TERMINATED       (-1)

////////////////////////////////////////////////////////////////////////////////
////
os_err_t os_scheduler_init(void);

int os_scheduler_state(void);

void os_scheduler_on_systick(void);

os_err_t os_scheduler_startup(void); /* start the first task */

os_err_t os_scheduler_schedule(void); /* schedule in interrupt */

os_err_t os_scheduler_schedule_in_application(void); /* schedule in application */

os_err_t os_scheduler_push_back(os_thread_t * thread);

os_err_t os_scheduler_push_front(os_thread_t * thread);

os_err_t os_scheduler_suspend(os_thread_t * thread);

os_err_t os_scheduler_resume(os_thread_t * thread);

os_err_t os_scheduler_yield(os_thread_t * thread);

void os_scheduler_timed_wait(os_thread_t * thread, os_tick_t ticks);

os_thread_t* os_scheduler_current_thread(void);

void os_scheduler_push(os_thread_t * thread);

os_uint_t os_scheduler_skipped(void);

#endif /*INCLUDED_OS_SCHEDULER_H*/
