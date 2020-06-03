/******************************** INCLUDE FILES *******************************/
#include "freeRTOS/FreeRTOS.h"
#include "freeRTOS/task.h"
#include "freeRTOS/queue.h"
#include "freeRTOS/semphr.h"

#include "rtos.h"
/******************************** LOCAL DEFINES *******************************/

/******************************** GLOBALDATA *******************************/

/********************************* LOCAL DATA *********************************/

/******************************* INTERFACE DATA *******************************/

/******************************* LOCAL FUNCTIONS ******************************/
static void rtos_createTask(genericTask_t *task)
{
    if (task->runTask != NULL)
        xTaskCreate(task->runTask, (const portCHAR *)task->name,
                    task->stackDepth, task->args, task->priority, NULL );

    return;
}


static void cmd_scdlrInit(void)
{
    /* Start the Scheduler */
    vTaskStartScheduler();

    return;
}

static
genericTask_t schdlr = {
    cmd_scdlrInit,
    NULL,
    "SCDLRTASK",
    5,
    1,
    NULL,
    NULL
};

/***************************** INTERFACE FUNCTIONS ****************************/

genericTask_t *getSchdlrGetTask(void)
{
    return &schdlr;
}


void rtos_start(genericTask_t *task[])
{
    int i = 0;

    for (i = 0; i < TASKS; i++) {
        task[i]->initTask();
        rtos_createTask(task[i]);
    }

    return;
}
