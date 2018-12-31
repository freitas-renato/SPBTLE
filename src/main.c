/**
 * @file main.c
 *
 * @brief Main function
 */

#include "mcu.h"
#include "user_service.h"

/*****************************************
 * Private Constant Definitions
 *****************************************/

#define LED_TOGGLE_DELAY_MS 500

/*****************************************
 * Main Function
 *****************************************/

int main(void) {
    tBleStatus ret;
    bt_recv = 0;

    mcu_init();
    ret = BLE_init();

    if (ret != BLE_STATUS_SUCCESS) {
        while (1) {
        };
    }

    for (;;) {
        BLE_Process();
        mcu_sleep(LED_TOGGLE_DELAY_MS);

        if (button_pressed()) {
            User_Update(bt_recv++);
        }
    }
}
