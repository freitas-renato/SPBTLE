/**
 * @file bluenrg_interface.c
 *
 * @brief BlueNRG interface initialization
 *
 * @author Renato Freitas <renato.freitas@thunderatz.org>
 *
 * @date 12/2018
 */

#ifndef _BLUENRG_INTERFACE_C_
#define _BLUENRG_INTERFACE_C_
#ifdef __cplusplus
extern "C" {
#endif

#include "bluenrg_interface.h"

#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"
#include "bluenrg_utils.h"
#include "hci_tl.h"
#include "user_service.h"
// #include "stm32f4_nucleo_f401re.h"

/*****************************************
 * Private Defines
 *****************************************/
#define BDADDR_SIZE 6

/*****************************************
 * Private Variables
 *****************************************/
extern volatile uint8_t set_connectable;
extern volatile int connected;

/*****************************************
 * Private Function Prototypes
 *****************************************/
static void User_Process(void);

#if PRINT_CSV_FORMAT
extern volatile uint32_t ms_counter;
/**
 * @brief  This function is a utility to print the log time
 *         in the format HH:MM:SS:MSS (DK GUI time format)
 * @param  None
 * @retval None
 */
void print_csv_time(void) {
    uint32_t ms = HAL_GetTick();
    PRINT_CSV("%02d:%02d:%02d.%03d", ms / (60 * 60 * 1000) % 24, ms / (60 * 1000) % 60, (ms / 1000) % 60, ms % 1000);
}
#endif

tBleStatus BLE_init(void) {
    const char* name = "BLE-TR";
    uint8_t SERVER_BDADDR[] = {0x12, 0x34, 0x00, 0xE1, 0x80, 0x03};
    uint8_t bdaddr[BDADDR_SIZE];
    uint16_t service_handle, dev_name_char_handle, appearance_char_handle;

    uint8_t hwVersion;
    uint16_t fwVersion;
    tBleStatus ret;

    hci_init(user_notify, NULL);

    if ((ret = getBlueNRGVersion(&hwVersion, &fwVersion)) != BLE_STATUS_SUCCESS) {
        PRINTF("Get BNRG version failed \n");
        return ret;
    } else {
        PRINTF("HWver %d\nFWver %d\n", hwVersion, fwVersion);
    }

    /*
    * Reset BlueNRG again otherwise we won't
    * be able to change its MAC address.
    * aci_hal_write_config_data() must be the first
    * command after reset otherwise it will fail.
    */
    hci_reset();
    HAL_Delay(100);

    BLUENRG_memcpy(bdaddr, SERVER_BDADDR, sizeof(SERVER_BDADDR));

    // Set BLE MAC address
    if ((ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN, bdaddr)) !=
        BLE_STATUS_SUCCESS) {
        PRINTF("Setting bdaddr failed.\n");
        return ret;
    } else {
        PRINTF("Setting bdaddr success.\n");
    }

    if ((ret = aci_gatt_init()) != BLE_STATUS_SUCCESS) {
        PRINTF("GATT_Init failed.\n");
        return ret;
    } else {
        PRINTF("GATT_Init success.\n");
    }

    if ((ret = aci_gap_init_IDB05A1(GAP_PERIPHERAL_ROLE_IDB05A1, 0, strlen(name), &service_handle,
                                    &dev_name_char_handle, &appearance_char_handle)) != BLE_STATUS_SUCCESS) {
        PRINTF("GAP_Init failed.\n");
        return ret;
    } else {
        PRINTF("GAP_Init success.\n");
    }

    if ((ret = aci_gatt_update_char_value(service_handle, dev_name_char_handle, 0, strlen(name), (uint8_t*) name)) !=
        BLE_STATUS_SUCCESS) {
        PRINTF("aci_gatt_update_char_value failed.\n");
        return ret;
    } else {
        PRINTF("aci_gatt_update_char_value success.\n");
    }

    if ((ret = aci_gap_set_auth_requirement(MITM_PROTECTION_REQUIRED, OOB_AUTH_DATA_ABSENT, NULL, 7, 16,
                                            USE_FIXED_PIN_FOR_PAIRING, 123456, BONDING)) == BLE_STATUS_SUCCESS) {
        PRINTF("BLE stack initialized!\n");
    } else {
        PRINTF("Error initializing :(\n");
        return ret;
    }

    // Add services here

    ret = Add_User_Service();
    if (ret != BLE_STATUS_SUCCESS) {
        PRINTF("Error while adding user service.\n");
    }
    // Select transmiting power level
    ret = aci_hal_set_tx_power_level(1, 4);
    return ret;
}

/*
 * BlueNRG-MS background task
 */
void BLE_Process(void) {
    User_Process();
    hci_user_evt_proc();
}

static void User_Process(void) {
    if (set_connectable) {
        setConnectable();
        set_connectable = FALSE;
    }

    if (connected) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    } else {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    }
}

#ifdef __cplusplus
}
#endif
#endif /* __BLUENRG_INTERFACE_C */
