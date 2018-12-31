/**
 * @file bluenrg_interface.h
 *
 * @brief BlueNRG interface initialization
 *
 * @author Renato Freitas <renato.freitas@thunderatz.org>
 *
 * @date 12/2018
 */

#ifndef _BLUENRG_INTERFACE_H_
#define _BLUENRG_INTERFACE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "bluenrg_def.h"

tBleStatus BLE_init(void);
void BLE_Process(void);

#ifdef __cplusplus
}
#endif
#endif /* _BLUENRG_INTERFACE_H_ */
