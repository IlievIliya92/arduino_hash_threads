#ifndef _HASH_H_
#define _HASH_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************** INCLUDE FILES *******************************/
#include <stdint.h>

/*********************************** DEFINES **********************************/

/************************** INTERFACE DATA DEFINITIONS ************************/

/************************* INTERFACE FUNCTION PROTOTYPES **********************/
uint32_t hashcalc (const uint8_t *data, int len);

#ifdef __cplusplus
}
#endif

#endif /* _HASH_H_ */
