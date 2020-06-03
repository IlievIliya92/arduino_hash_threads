/******************************** INCLUDE FILES *******************************/
/* I/O include files */
#include "freeRTOS/lib_io/serial.h"

#include "generic_t.h"
#include "cmd_get.h"
#include "mgr.h"
#include "rtos.h"

/******************************** LOCAL DEFINES *******************************/

/******************************** GLOBALDATA *******************************/
extern xComPortHandle xSerialPort;

/********************************* LOCAL DATA *********************************/

/******************************* LOCAL FUNCTIONS ******************************/

/***************************** MAIN  ****************************/
int main(void) __attribute__((OS_main));
int main(void)
{
    /* Tasks array */
    genericTask_t *rtosTasks[TASKS];

    rtosTasks[MGR] = getMgrTask();
    rtosTasks[PROCESS] = getProcessTask();
    rtosTasks[GET_CMD] = getCmdGetTask();
    rtosTasks[SCHEDULER] = getSchdlrGetTask();

    /* Starts all the tasks from Tasks array */
    rtos_start(rtosTasks);

    avrSerialxPrint_P(&xSerialPort, PSTR("\r\n\n Failed to start the application!\r\n"));
}

