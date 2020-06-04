/******************************** INCLUDE FILES *******************************/
#include <string.h>
#include <stdbool.h>
#include <util/delay_basic.h>

#include "freeRTOS/FreeRTOS.h"
#include "freeRTOS/semphr.h"
#include "freeRTOS/queue.h"

/* serial interface include file. */
#include "freeRTOS/lib_io/serial.h"
#include "freeRTOS/lib_io/digitalAnalog.h"

#include "cmd_get.h"

/******************************** LOCAL DEFINES *******************************/

/******************************** GLOBALDATA *******************************/
extern xComPortHandle xSerialPort;

/********************************* LOCAL DATA *********************************/

/******************************* INTERFACE DATA *******************************/
QueueHandle_t xCmdQueue = NULL;

/************************ LOCAL FUNCTIONS PROTOTYPES***************************/

/******************************* LOCAL FUNCTIONS ******************************/
static int cmd_getReceiveInput(uint8_t *buff, uint8_t len)
{
    uint8_t c;
    uint8_t i = 0;

    for (;;) {
        while (!xSerialGetChar(&xSerialPort, &c))
            vTaskDelay(1);

        if (c == '\r') break;
        if ((c == '\b') && i) {
            --i;
            continue;
        }
        if (c >= ' ' && i < len - 1) {  /* Received chars */
            buff[i++] = c;
        }
    }

    xSerialRxFlush(&xSerialPort);
    buff[i] = 0;

    return i;
}

static void cmd_getInit(void)
{
    xSerialPort = xSerialPortInitMinimal(USART0, BAUD, portSERIAL_BUFFER_TX, portSERIAL_BUFFER_RX);
    xCmdQueue = xQueueCreate(QUEUE_BUFFS, sizeof(genericCmdMsg_t));

    return;
}

static void cmd_getTask(void *pvParameters)
{
    (void) pvParameters;
    genericCmdMsg_t cmdMsg;
    uint8_t cmd_buff[IO_SIZE];

    while(1)
    {
        cmdMsg.inputLen = cmd_getReceiveInput(cmd_buff, IO_SIZE);

        memset(cmdMsg.input, 0x00, sizeof(cmdMsg.input));
        memcpy(cmdMsg.input, cmd_buff, cmdMsg.inputLen );
        cmdMsg.eventType = newInput;

        xQueueSend(xCmdQueue, &cmdMsg, portMAX_DELAY);
    }

    return;
}

/***************************** INTERFACE FUNCTIONS ****************************/
genericTask_t cmd = {
    cmd_getInit,
    cmd_getTask,
    "CMDTASK",
    100,
    1,
    NULL,
    NULL
};

genericTask_t *getCmdGetTask(void)
{
    return &cmd;
}
