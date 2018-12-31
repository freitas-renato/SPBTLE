/**
 * @file user_service.c
 *
 * @brief BLE default service initialization
 *
 * @author Renato Freitas <renato.freitas@thunderatz.org>
 *
 * @date 12/2018
 */

#include "user_service.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"

/*****************************************
 * Private Variables
 *****************************************/
__IO uint32_t connected = FALSE;
__IO uint8_t set_connectable = 1;
__IO uint16_t connection_handle = 0;
__IO uint8_t notification_enabled = FALSE;

uint16_t userServHandle, userCharHandle;

/*****************************************
 * Private Macros
 *****************************************/
#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, \
                      uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0)                                    \
    do {                                                                                                         \
        uuid_struct[0] = uuid_0;                                                                                 \
        uuid_struct[1] = uuid_1;                                                                                 \
        uuid_struct[2] = uuid_2;                                                                                 \
        uuid_struct[3] = uuid_3;                                                                                 \
        uuid_struct[4] = uuid_4;                                                                                 \
        uuid_struct[5] = uuid_5;                                                                                 \
        uuid_struct[6] = uuid_6;                                                                                 \
        uuid_struct[7] = uuid_7;                                                                                 \
        uuid_struct[8] = uuid_8;                                                                                 \
        uuid_struct[9] = uuid_9;                                                                                 \
        uuid_struct[10] = uuid_10;                                                                               \
        uuid_struct[11] = uuid_11;                                                                               \
        uuid_struct[12] = uuid_12;                                                                               \
        uuid_struct[13] = uuid_13;                                                                               \
        uuid_struct[14] = uuid_14;                                                                               \
        uuid_struct[15] = uuid_15;                                                                               \
    } while (0)

#define COPY_USER_SERVICE_UUID(uuid_struct)                                                                        \
    COPY_UUID_128(uuid_struct, 0x8c, 0x6d, 0x37, 0x20, 0xab, 0x19, 0x11, 0xe8, 0xb5, 0x68, 0x08, 0x00, 0x20, 0x0c, \
                  0x9a, 0x66);

#define COPY_USER_CHAR_UUID(uuid_struct)                                                                           \
    COPY_UUID_128(uuid_struct, 0x45, 0xf6, 0x92, 0x40, 0xab, 0xce, 0x11, 0xe8, 0xb5, 0x68, 0x08, 0x00, 0x20, 0x0c, \
                  0x9a, 0x66);

/*****************************************
 * Public Functions Bodies Definitions
 *****************************************/

tBleStatus Add_User_Service(void) {
    //@TODO add user service
    tBleStatus ret;
    uint8_t uuid[16];
    uint8_t char_uuid[16];

    // Copy service UUID defined above (generated randomly) to a local uuid variable
    COPY_USER_SERVICE_UUID(uuid);

    // Create primary service, returns 'userServHandle' handler
    ret = aci_gatt_add_serv(UUID_TYPE_128, uuid, PRIMARY_SERVICE, 7, &userServHandle);
    if (ret != BLE_STATUS_SUCCESS) {
        PRINTF("Error adding new Service.\n");
        return ret;
    }

    // Copy characteristic UUID defined above to a local uuid variable
    COPY_USER_CHAR_UUID(char_uuid);
    // Add GATT characteristic, returns 'userCharHandle' handler
    ret = aci_gatt_add_char(userServHandle, UUID_TYPE_128, char_uuid, 20,
                            CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RESP | CHAR_PROP_READ, ATTR_PERMISSION_NONE,
                            GATT_NOTIFY_ATTRIBUTE_WRITE, 16, 1, &userCharHandle);

    if (ret != BLE_STATUS_SUCCESS) {
        PRINTF("Error while adding new charac.\n");
        return ret;
    }

    /* More characteristics could be added here */

    return ret;
}

tBleStatus User_Update(uint8_t value) {
    tBleStatus ret;

    ret = aci_gatt_update_char_value(userServHandle, userCharHandle, 0, 1, &value);

    if (ret != BLE_STATUS_SUCCESS) {
        PRINTF("Failed to update characteristic value.\n");
        return ret;
    }

    return BLE_STATUS_SUCCESS;
}

/**
 * @brief  Puts the device in connectable mode.
 *         If you want to specify a UUID list in the advertising data, those data can
 *         be specified as a parameter in aci_gap_set_discoverable().
 *         For manufacture data, aci_gap_update_adv_data must be called.
 * @param  None
 * @retval None
 */
/* Ex.:
 *
 *  tBleStatus ret;
 *  const char local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME,'B','l','u','e','N','R','G'};
 *  const uint8_t serviceUUIDList[] = {AD_TYPE_16_BIT_SERV_UUID,0x34,0x12};
 *  const uint8_t manuf_data[] = {4, AD_TYPE_MANUFACTURER_SPECIFIC_DATA, 0x05, 0x02, 0x01};
 *
 *  ret = aci_gap_set_discoverable(ADV_DATA_TYPE, ADV_INTERV_MIN, ADV_INTERV_MAX, PUBLIC_ADDR,
 *                                 NO_WHITE_LIST_USE, 8, sizeof(local_name), 3, serviceUUIDList, 0, 0);
 *  ret = aci_gap_update_adv_data(5, manuf_data);
 *
 */
void setConnectable(void) {
    tBleStatus ret;

    const char local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME, 'B', 'l', 'u', 'e', 'N', 'R', 'G', '-', 'T', 'R'};

    /* disable scan response */
    hci_le_set_scan_resp_data(0, NULL);
    PRINTF("General Discoverable Mode.\n");

    ret = aci_gap_set_discoverable(ADV_DATA_TYPE, ADV_INTERV_MIN, ADV_INTERV_MAX, PUBLIC_ADDR, NO_WHITE_LIST_USE,
                                   sizeof(local_name), local_name, 0, NULL, 0, 0);
    if (ret != BLE_STATUS_SUCCESS) {
        PRINTF("Error while setting discoverable mode (%d)\n", ret);
    }
}

/**
 * @brief  This function is called when there is a LE Connection Complete event.
 * @param  uint8_t Address of peer device
 * @param  uint16_t Connection handle
 * @retval None
 */
void GAP_ConnectionComplete_CB(uint8_t addr[6], uint16_t handle) {
    connected = TRUE;
    connection_handle = handle;

    PRINTF("Connected to device:");
    for (uint32_t i = 5; i > 0; i--) {
        PRINTF("%02X-", addr[i]);
    }
    PRINTF("%02X\n", addr[0]);
}

/**
 * @brief  This function is called when the peer device gets disconnected.
 * @param  None
 * @retval None
 */
void GAP_DisconnectionComplete_CB(void) {
    connected = FALSE;
    PRINTF("Disconnected\n");
    /* Make the device connectable again. */
    set_connectable = TRUE;
    notification_enabled = FALSE;
}

/**
 * @brief  Read request callback.
 * @param  uint16_t Handle of the attribute
 * @retval None
 */
void Read_Request_CB(uint16_t handle) {
    //@TODO implement read_request_callback

    if (handle == userCharHandle + 1) {
        User_Update(10);
    }
}

/**
 * @brief  Callback processing the ACI events.
 * @note   Inside this function each event must be identified and correctly
 *         parsed.
 * @param  void* Pointer to the ACI packet
 * @retval None
 */
void user_notify(void* pData) {
    hci_uart_pckt* hci_pckt = pData;
    /* obtain event packet */
    hci_event_pckt* event_pckt = (hci_event_pckt*) hci_pckt->data;

    if (hci_pckt->type != HCI_EVENT_PKT)
        return;

    switch (event_pckt->evt) {
        case EVT_DISCONN_COMPLETE: {
            GAP_DisconnectionComplete_CB();
        } break;

        case EVT_LE_META_EVENT: {
            evt_le_meta_event* evt = (void*) event_pckt->data;

            switch (evt->subevent) {
                case EVT_LE_CONN_COMPLETE: {
                    evt_le_connection_complete* cc = (void*) evt->data;
                    GAP_ConnectionComplete_CB(cc->peer_bdaddr, cc->handle);
                } break;
            }
        } break;

        case EVT_VENDOR: {
            evt_blue_aci* blue_evt = (void*) event_pckt->data;
            switch (blue_evt->ecode) {
                case EVT_BLUE_GATT_READ_PERMIT_REQ: {
                    evt_gatt_read_permit_req* pr = (void*) blue_evt->data;
                    Read_Request_CB(pr->attr_handle);
                } break;

                case EVT_BLUE_GATT_ATTRIBUTE_MODIFIED: {
                    evt_gatt_attr_modified_IDB05A1* evt = (evt_gatt_attr_modified_IDB05A1*) blue_evt->data;
                    if (evt->attr_handle == userCharHandle + 1) {
                        bt_recv = evt->att_data[evt->offset];
                    }
                } break;
            }
        } break;
    }
}
