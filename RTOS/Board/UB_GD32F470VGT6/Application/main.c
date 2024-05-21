/*!
    \file    main.c
    \brief   led spark with systick

    \version 2024-01-15, V3.2.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2024, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "main.h"
#include <board.h>
#include <stdio.h>
#include <os_kernel.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
////
static uint8_t BootThread_Stack[2048];
static os_thread_t BootThread;


static uint8_t Worker1_Stack[2048];
static os_thread_t Worker1;

static void Thread_Entry(void* p){
    os_size_t nCount = 0;
    while(1){
        printf("[Task-%s]: %d\n", os_thread_self()->name, nCount++);
        for(int i=0; i<0x3fffff; i++);
    }
}

////////////////////////////////////////////////////////////////////////////////
////
static uint8_t BSP_USART0__RxThreadStack[2048];
static os_thread_t BSP_USART0__RxThread;
static os_semaphore_t BSP_USART0__RxSem={0};
static os_size_t write_idx = 0;
static os_size_t read_idx = 0;
static uint8_t BSP_USART0__RxBuffer[256]={0};


static void BSP_USART0__RxHandler(uint16_t data, void* ud){
    BSP_USART0__RxBuffer[write_idx++] = data & 0xFF;
    os_semaphore_release(&BSP_USART0__RxSem);
}

static void BSP_USART0__RxThreadEntry(void* p)
{
    while(1){
        os_semaphore_take(&BSP_USART0__RxSem, OS_WAIT_INFINITY);
        if(BSP_USART0__RxBuffer[write_idx-1]=='\n'){
            printf("[USART0] Rx: %s", BSP_USART0__RxBuffer);
            write_idx = 0;
            memset(BSP_USART0__RxBuffer, 0, sizeof(BSP_USART0__RxBuffer));
        }
        if(write_idx==sizeof(BSP_USART0__RxBuffer)){
            write_idx = 0;
            memset(BSP_USART0__RxBuffer, 0, sizeof(BSP_USART0__RxBuffer));
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
////


/*!
    \brief    main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    Board_Init();

    os_kernel_init();

    printf("BSP_USART0__RxThreadEntry startup\n");
    os_semaphore_init(&BSP_USART0__RxSem, "USART0_RxSem", 0, OS_SEMAPHORE_FLAG_FIFO);
    BSP_USART0_SetRxHandler(BSP_USART0__RxHandler, 0);

    os_thread_init(&BSP_USART0__RxThread, "USART0_RxThd", BSP_USART0__RxThreadEntry, 0, BSP_USART0__RxThreadStack, sizeof(BSP_USART0__RxThreadStack), 19,
                   os_tick_from_millisecond(200));
    os_thread_startup(&BSP_USART0__RxThread);

    printf("os_tick_from_millisecond(50)=%d\n", os_tick_from_millisecond(50));

    os_thread_init(&BootThread, "Boot", Thread_Entry, 0, BootThread_Stack, sizeof(BootThread_Stack), 20,
                   os_tick_from_millisecond(200));
    os_thread_startup(&BootThread);

    os_thread_init(&Worker1, "Worker1", Thread_Entry, 0, Worker1_Stack, sizeof(Worker1_Stack), 20,
                   os_tick_from_millisecond(200));
    os_thread_startup(&Worker1);

    os_kernel_startup();


    size_t nCount = 0;

    while(1) {
        printf("count=%d\n", nCount++);
        for(int i=0; i<0x3FffFF; i++){}
    }
}
