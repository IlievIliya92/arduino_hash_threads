#ifndef _MGR_H_
#define _MGR_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************** INCLUDE FILES *******************************/
#include "generic_t.h"

/*********************************** DEFINES **********************************/

/************************** INTERFACE DATA DEFINITIONS ************************/

/************************* INTERFACE FUNCTION PROTOTYPES **********************/
typedef void (*result_cb_t)(uint8_t *input_string, uint32_t result, void *arg);


genericTask_t *getMgrTask(void);
genericTask_t *getProcessTask(void);

#ifdef __cplusplus
}
#endif

#endif /* _MGR_H_ */
