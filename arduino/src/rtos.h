#ifndef _RTOS_H_
#define _RTOS_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************** INCLUDE FILES *******************************/
#include "generic_t.h"

/*********************************** DEFINES **********************************/
typedef enum tasks_t {
    MGR = 0,
    PROCESS,
    GET_CMD,
    SCHEDULER,
    TASKS
} tasks_t;

/************************** INTERFACE DATA DEFINITIONS ************************/

/************************* INTERFACE FUNCTION PROTOTYPES **********************/
genericTask_t *getSchdlrGetTask(void);

void rtos_start(genericTask_t *task[]);

#ifdef __cplusplus
}
#endif

#endif /* _RTOS_H_ */
