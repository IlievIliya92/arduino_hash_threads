/******************************** INCLUDE FILES *******************************/
#include <string.h>
#include <stdbool.h>
#include <util/delay_basic.h>

#include "freeRTOS/FreeRTOS.h"
#include "freeRTOS/semphr.h"
#include "freeRTOS/queue.h"
#include "freeRTOS/portable.h"

/* serial interface include file. */
#include "freeRTOS/lib_io/serial.h"

#include "freeRTOS/lib_io/digitalAnalog.h"

#include "mgr.h"
#include "hash.h"

/******************************** LOCAL DEFINES *******************************/

typedef struct msg
{
  uint8_t input[IO_SIZE];
  int inputLen;
  result_cb_t callback;
  void *callback_arg;
} msg_t;

typedef enum states_t {
    OFF = 0,
    ON
} states_t;


/******************************** GLOBALDATA *******************************/
extern xComPortHandle xSerialPort;
extern QueueHandle_t xCmdQueue;

/********************************* LOCAL DATA *********************************/
static QueueHandle_t xMsgQueue = NULL;

/******************************* INTERFACE DATA *******************************/

/************************ LOCAL FUNCTIONS PROTOTYPES***************************/


/******************************* LOCAL FUNCTIONS ******************************/
static
void dioSetValue(states_t state)
{
    state == ON ? PORTB |=  _BV(PORTB5) : (PORTB &= ~_BV(PORTB5));

    return;
}

static void mgr_processInput(void *arg)
{
   while (1)
   {
        msg_t msg;
        uint32_t hash;
        int i = 0;

        memset(&msg, 0x0, sizeof(msg_t));
        xQueueReceive(xMsgQueue, &msg, portMAX_DELAY);

        for (i = 0; i < ITERATIONS; i++)
            hash = hashcalc(msg.input, msg.inputLen);

        msg.callback(msg.input, msg.inputLen, hash, msg.callback_arg);
   }

   return;
}

static void mgr_processInputInit(void)
{
    /* Initialize L diode as output */
    DDRB |= _BV(DDB5);

    dioSetValue(OFF);

    return;
}


static void mgr_Init(void)
{
    xMsgQueue = xQueueCreate(QUEUE_BUFFS, sizeof(msg_t));

    return;
}

static int inputAsync_calc(const uint8_t *input_string, int len, result_cb_t callback, void *arg)
{
   msg_t msg;

   memset(&msg, 0x0, sizeof(msg_t));
   memcpy(msg.input, input_string, len);
   msg.inputLen = len;
   msg.callback = callback;
   msg.callback_arg = arg;

   if (xQueueSend(xMsgQueue, &msg, portMAX_DELAY)) {
      return 0;
   } else {
      return -1;
   }
}

static void resultCallback(uint8_t *input_string, int inputLen, uint32_t result, void *arg)
{
    genericCmdMsg_t cmdMsg;

    memset(&cmdMsg, 0x00, sizeof(genericCmdMsg_t));
    cmdMsg.eventType = newOutput;
    cmdMsg.inputLen = inputLen;
    cmdMsg.hash = result;
    memcpy(cmdMsg.input, input_string, inputLen);

    xQueueSend(xCmdQueue, &cmdMsg, portMAX_DELAY);

    return;
}

static void mgr_Task(void *pvParameters)
{
    (void) pvParameters;
    states_t processing = OFF;

    genericCmdMsg_t cmdMsg;
    memset(&cmdMsg, 0x0, sizeof(genericCmdMsg_t));

    while(1) {
        xQueueReceive(xCmdQueue, &cmdMsg, portMAX_DELAY);

        if (cmdMsg.eventType == newInput) {
            dioSetValue(ON);
            processing = ON;
            inputAsync_calc(cmdMsg.input, cmdMsg.inputLen, resultCallback, NULL);
        }
        else if (cmdMsg.eventType == newOutput) {
            processing = OFF;
            avrSerialxPrintf(&xSerialPort, "%s -> ", cmdMsg.input);
            avrSerialxPrintf(&xSerialPort, "%d", cmdMsg.hash);

            dioSetValue(OFF);
        }
        avrSerialxPrintf(&xSerialPort, "\r\n");
        avrSerialxPrintf(&xSerialPort, "Processing: %s", processing == ON ? "ON" : "OFF");
        avrSerialxPrintf(&xSerialPort, "\r\n");
    }

    return;
}

/***************************** INTERFACE FUNCTIONS ****************************/
genericTask_t mgr = {
    mgr_Init,
    mgr_Task,
    "MGRTASK",
    128,
    4,
    NULL,
    NULL
};

genericTask_t inprocess = {
    mgr_processInputInit,
    mgr_processInput,
    "PROCESS",
    128,
    2,
    NULL,
    NULL
};


genericTask_t *getMgrTask(void)
{
    return &mgr;
}


genericTask_t *getProcessTask(void)
{
    return &inprocess;
}
