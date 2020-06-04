#ifndef _GENERIC_H_
#define _GENERIC_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************** INCLUDE FILES *******************************/
#include "freeRTOS/task.h"
#include "freeRTOS/semphr.h"

/*********************************** DEFINES **********************************/
#define IO_SIZE      15
#define QUEUE_BUFFS  5

/************************** INTERFACE DATA DEFINITIONS ************************/
typedef void (*voidVoid_t)(void);
typedef void (*voidVoidPtr_t)(void *);

/* Generic tasks structure */
typedef struct {
   voidVoid_t initTask;
   voidVoidPtr_t runTask;
   char *name;
   const configSTACK_DEPTH_TYPE stackDepth;
   UBaseType_t priority;
   void *data;
   void *args;
} genericTask_t;

typedef enum event {
   newInput = 1,
   newOutput
} Event_t;

typedef struct genericCmdMsg
{
   Event_t eventType; // 1: new input, 2: calculation result
   uint8_t input[IO_SIZE];
   int inputLen;
   uint32_t hash;
} genericCmdMsg_t;

#ifdef __cplusplus
}
#endif

#endif /* _GENERIC_H_ */

