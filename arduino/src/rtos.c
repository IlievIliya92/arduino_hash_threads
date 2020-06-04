/******************************** INCLUDE FILES *******************************/
#include "freeRTOS/FreeRTOS.h"
#include "freeRTOS/task.h"
#include "freeRTOS/queue.h"
#include "freeRTOS/semphr.h"

#include <util/delay_basic.h>

#include "rtos.h"
/******************************** LOCAL DEFINES *******************************/

/******************************** GLOBALDATA *******************************/

/********************************* LOCAL DATA *********************************/

/******************************* INTERFACE DATA *******************************/

/******************************* LOCAL FUNCTIONS ******************************/
static void rtos_createTask(genericTask_t *task)
{
    xTaskCreate(task->runTask, (const portCHAR *)task->name,
                task->stackDepth, task->args, task->priority, NULL );

    return;
}


static void cmd_scdlrInit(void)
{
    _delay_loop_2(10);

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


void rtos_start(genericTask_t *tasks[])
{
    taskId_t taskId = 0;

    for (taskId = 0; taskId < TASKS; taskId++) {
        tasks[taskId]->initTask();

        /* No need to run createTask to start the scheduler */
        if (taskId != SCHEDULER)
            rtos_createTask(tasks[taskId]);
    }

    return;
}
