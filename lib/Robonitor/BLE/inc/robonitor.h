/**
 * @file robonitor.h
 *
 * @brief Robonitor BLE services and characteristics creation.
 *
 * @author Renato Freitas <renato.freitas@thunderatz.org>
 * @author Daniel Nery <daniel.nery@thunderatz.org>
 *
 * @date 04/2019
 */

#ifndef _ROBONITOR_H_
#define _ROBONITOR_H_

#include "spbtle.h"

/*****************************************
 * Public Variables
 *****************************************/
uint8_t* bt_recv[16];

/*****************************************
 * Public Functions Prototypes
 *****************************************/

/**
 * @brief   Initialize SPBTLE device with defined services.
 *
 * @param   name        Advertising name of the bluetooth device.
 * @param   led_on      Function that turn on BLE led indicator
 * @param   led_toggle  Function that blinks BLE led indicator
 * @retval  Value indicating success or error code.
 */
tBleStatus ble_init(char* name, led_function_t led_on, led_function_t led_toggle);

/**
 * @brief   Adds sumo strategy list
 *
 * @param   strategies      Array with strategy names
 * @param   strategy_count  Number of strategies to be added
 * @retval  Value indicating success or error code.
 */
tBleStatus add_strategy_list(char* strategies[], uint8_t strategy_count);

#endif // _ROBONITOR_H_
