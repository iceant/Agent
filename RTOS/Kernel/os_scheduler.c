#include <os_scheduler.h>
#include <os_memory.h>
#include <os_list.h>
#include <os_macros.h>
#include <cpu.h>
#include <cpu_stack.h>
#include <os_interrupt.h>
////////////////////////////////////////////////////////////////////////////////
////

static os_list_t os_scheduler__ready_list[OS_PRIORITY_MAX]={0};
static os_list_t os_scheduler__yield_list={0};
static os_list_t os_scheduler__pending_list={0};
static int os_scheduler__state={0};
static os_uint_t os_scheduler__schedule_nest={0};
static cpu_lock_t os_scheduler__lock={0};
static os_thread_t * os_scheduler__current_thread=0;
static os_uint_t os_scheduler__current_tick=0;

#define OS_SCHEDULER_LOCK() cpu_lock_lock(&os_scheduler__lock)
#define OS_SCHEDULER_UNLOCK() cpu_lock_unlock(&os_scheduler__lock)

////////////////////////////////////////////////////////////////////////////////
////
C__STATIC_FORCEINLINE os_thread_t * os_scheduler__pop_highest(void){
    os_priority_t highest_priority = os_priority_get_highest();
    os_list_t * head = &os_scheduler__ready_list[highest_priority];
    if(OS_LIST_IS_EMPTY(head)){
        os_priority_unmark(highest_priority);
        OS_SCHEDULER_UNLOCK();
        return 0;
    }
    os_list_node_t *node = OS_LIST_NEXT(head);
    os_thread_t * thread = OS_CONTAINER_OF(node, os_thread_t, ready_node);
    OS_LIST_REMOVE(&thread->ready_node);
    if(OS_LIST_IS_EMPTY(head)){
        os_priority_unmark(highest_priority);
    }
    return thread;
}

C__STATIC_FORCEINLINE void os_scheduler__pending_list_push_back(os_thread_t * thread){
    if(thread==0)return;
    OS_LIST_REMOVE(&thread->ready_node);
    OS_LIST_INSERT_BEFORE(&os_scheduler__pending_list, &thread->ready_node);
}

C__STATIC_FORCEINLINE void os_scheduler__ready_list_push_back(os_thread_t * thread){
    if(thread==0)return;
    os_priority_t priority = thread->curr_priority;
    os_list_t * head = &os_scheduler__ready_list[priority];
    os_list_node_t * node;
    for(node = OS_LIST_NEXT(head); node!=head;){
        os_thread_t* p = OS_CONTAINER_OF(node, os_thread_t, ready_node);
        if(p==thread){
            return;
        }
        node = OS_LIST_NEXT(node);
    }
    OS_LIST_REMOVE(&thread->ready_node);
    thread->state = OS_THREAD_STATE_READY;
    OS_LIST_INSERT_BEFORE(head, &thread->ready_node);
    os_priority_mark(priority);
}
////////////////////////////////////////////////////////////////////////////////
////


os_err_t os_scheduler_init(void){
    os_size_t i;
    for(i=0; i< OS_ARRAY_SIZE(os_scheduler__ready_list); i++){
        OS_LIST_INIT(&os_scheduler__ready_list[i]);
    }
    OS_LIST_INIT(&os_scheduler__yield_list);
    OS_LIST_INIT(&os_scheduler__pending_list);

    os_scheduler__current_tick = 0;
    os_scheduler__current_thread = 0;
    os_scheduler__schedule_nest = 0;
    cpu_lock_init(&os_scheduler__lock);
    os_scheduler__state = OS_SCHEDULER_STATE_INITIALIZED;

    return OS_SCHEDULER_EOK;
}

int os_scheduler_state(void)
{
    return os_scheduler__state;
}

static void os_scheduler__timer_timeout(os_timer_t * timer, void* userdata){
    os_thread_t * thread = (os_thread_t *)userdata;
    thread->error = OS_THREAD_ERROR_TIMEOUT;
    if(os_interrupt_nest()>0U){
        os_scheduler__pending_list_push_back(thread);
    }else{
        os_scheduler__ready_list_push_back(thread);
    }
}

void os_scheduler_timed_wait(os_thread_t * thread, os_tick_t ticks){
    os_timer_add(&thread->timer_node, os_scheduler__timer_timeout, thread, ticks, OS_TIMER_FLAG_ONCE);
}

void os_scheduler_on_systick(void){
    register os_thread_t * curr_thread;
    register os_bool_t curr_thread_need_schedule = OS_FALSE;
    register os_bool_t timer_need_schedule = OS_FALSE;

    OS_SCHEDULER_LOCK();
    os_scheduler__current_tick++;
    curr_thread = os_scheduler__current_thread;
    if(curr_thread!=0){
        if(curr_thread->state==OS_THREAD_STATE_RUNNING){
            curr_thread->remain_ticks--;
            if(curr_thread->remain_ticks == 0){
                curr_thread->state = OS_THREAD_STATE_YIELD;
                os_scheduler__current_thread = 0;
                curr_thread_need_schedule = OS_TRUE;
            }
        }
    }

    timer_need_schedule = os_timer_tick();

    if(curr_thread_need_schedule==OS_TRUE || (timer_need_schedule==OS_TRUE)){
        if(os_interrupt_nest()>0U){
            // push to pend list
            // wait for schedule
            os_scheduler__pending_list_push_back(curr_thread);
        }else{
            // push to ready list
            // schedule now
            os_scheduler_schedule();
        }
    }

    OS_SCHEDULER_UNLOCK();
}

os_uint_t os_scheduler_nest(){
    return os_scheduler__schedule_nest;
}

os_err_t os_scheduler_startup(void){

    OS_SCHEDULER_LOCK();

    os_thread_t * thread = os_scheduler__pop_highest();
    os_scheduler__ready_list_push_back(os_scheduler__current_thread);

    thread->state = OS_THREAD_STATE_RUNNING;
    if(thread->remain_ticks ==0 ){
        thread->remain_ticks = thread->init_ticks;
    }
    os_scheduler__current_thread = thread;

    OS_SCHEDULER_UNLOCK();

    cpu_stack_switch(0, &thread->sp);

    return OS_SCHEDULER_EOK;
}

os_err_t os_scheduler_schedule(void)
{
    register os_thread_t * curr_thread;
    register os_thread_t * next_thread;

    if(os_interrupt_nest()>0U){
        return OS_SCHEDULER_ERROR;
    }

    OS_SCHEDULER_LOCK();

    next_thread = os_scheduler__pop_highest();

    curr_thread = os_scheduler__current_thread;
    if(curr_thread!=0){
        if(curr_thread->state==OS_THREAD_STATE_YIELD){
            os_scheduler__ready_list_push_back(curr_thread);
        }
    }

    os_list_node_t * node;
    os_list_t * head = &os_scheduler__pending_list;
    if(!OS_LIST_IS_EMPTY(head)){
        for(node = OS_LIST_NEXT(head); node!=head;){
            os_thread_t * thread = OS_CONTAINER_OF(node, os_thread_t, ready_node);
            node = OS_LIST_NEXT(node);
            OS_LIST_REMOVE(&thread->ready_node);
            os_scheduler__ready_list_push_back(thread);
        }
    }

    if(next_thread==0){
        next_thread = os_scheduler__pop_highest();
    }

    if(next_thread==0){
        OS_SCHEDULER_UNLOCK();
        return OS_SCHEDULER_ERROR;
    }

    OS_SCHEDULER_UNLOCK();

    cpu_stack_switch((curr_thread==0)?0:&curr_thread->sp, &next_thread->sp);

    return OS_SCHEDULER_EOK;
}


os_err_t os_scheduler_push_back(os_thread_t * thread)
{
    OS_SCHEDULER_LOCK();
    os_scheduler__ready_list_push_back(thread);
    OS_SCHEDULER_UNLOCK();

    return OS_SCHEDULER_EOK;
}

os_err_t os_scheduler_push_front(os_thread_t * thread){
    OS_SCHEDULER_LOCK();
    os_priority_t priority = thread->curr_priority;
    os_list_t * head = &os_scheduler__ready_list[priority];
    OS_LIST_REMOVE(&thread->ready_node);
    OS_LIST_INSERT_AFTER(head, &thread->ready_node);
    os_priority_mark(priority);
    OS_SCHEDULER_UNLOCK();

    return OS_SCHEDULER_EOK;
}

os_err_t os_scheduler_suspend(os_thread_t * thread){
    OS_SCHEDULER_LOCK();
    thread->state = OS_THREAD_STATE_SUSPEND;
    thread->remain_ticks = 0;
    OS_LIST_REMOVE(&thread->ready_node);
    OS_LIST_REMOVE(&thread->wait_node);
    os_timer_remove(&thread->timer_node);
    OS_SCHEDULER_UNLOCK();
    return OS_SCHEDULER_EOK;
}

os_err_t os_scheduler_resume(os_thread_t * thread){
    return os_scheduler_push_back(thread);
}

os_err_t os_scheduler_yield(os_thread_t * thread)
{
    OS_SCHEDULER_LOCK();
    thread->state = OS_THREAD_STATE_YIELD;
    OS_LIST_REMOVE(&thread->ready_node);
    OS_LIST_INSERT_BEFORE(&os_scheduler__yield_list, &thread->ready_node);
    OS_SCHEDULER_UNLOCK();
    return OS_SCHEDULER_EOK;
}

os_thread_t* os_scheduler_current_thread(void)
{
    return os_scheduler__current_thread;
}
