#include <board.h>
#include <os_kernel.h>

void Board_Init(void){

    nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x0000);
    SCB->CCR|=SCB_CCR_STKALIGN_Msk;
    systick_clksource_set(SYSTICK_CLKSOURCE_HCLK_DIV8);

    os_kernel_init();

    /* Configure the NVIC Preemption Priority Bits */
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    //nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
    NVIC_SetPriority(PendSV_IRQn, 0xFF);
    NVIC_SetPriority(SysTick_IRQn, 0xFF);
    SysTick_Config(SystemCoreClock/OS_TICKS_PER_SECOND); /* 10ms = tick */
}