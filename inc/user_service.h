/**
 * @file user_service.h
 *
 * @brief BLE default service initialization
 *
 * @author Renato Freitas <renato.freitas@thunderatz.org>
 *
 * @date 12/2018
 */

#ifndef _USER_SERVICE_H_
#define _USER_SERVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include "bluenrg_aci_const.h"
#include "bluenrg_gap.h"
#include "bluenrg_hal_aci.h"
#include "hci.h"
#include "hci_const.h"
#include "hci_le.h"
#include "sm.h"
#include "string.h"

uint8_t bt_recv;

tBleStatus Add_User_Service(void);
tBleStatus User_Update(uint8_t value);
void Read_Request_CB(uint16_t handle);
void setConnectable(void);
void GAP_ConnectionComplete_CB(uint8_t addr[6], uint16_t handle);
void GAP_DisconnectionComplete_CB(void);
void user_notify(void* pData);

#ifdef __cplusplus
}
#endif
#endif /* _USER_SERVICE_H_ */
