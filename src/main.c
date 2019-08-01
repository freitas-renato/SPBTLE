/**
 * @file main.c
 *
 * @brief Main function
 */

#include "mcu.h"
#include "spbtle.h"
#include "robonitor.h"

/*****************************************
 * Private Constant Definitions
 *****************************************/

#define LED_TOGGLE_DELAY_MS 500

/*****************************************
 * Main Function
 *****************************************/
int main(void) {
    tBleStatus ret;

    static char* strat[] = {
        "Estrela", "Dibre", "Arco Esquerda", "Defensiva", "Emergencia", "aaa", "bbb"
    };

    mcu_init();
    ret = ble_init("ThunderBlue", ble_led_on, ble_led_toggle);
    ret = add_strategy_list(strat, 7);

    if (ret != BLE_STATUS_SUCCESS) {
        while (1) {
        }
    }

    for (;;) {
        ble_process();

        mcu_sleep(200);
    }
}
