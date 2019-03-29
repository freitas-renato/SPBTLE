/**
 * @file robonitor.c
 *
 * @brief Robonitor BLE services and characteristics creation.
 *
 * @author Renato Freitas <renato.freitas@thunderatz.org>
 * @author Daniel Nery <daniel.nery@thunderatz.org>
 *
 * @date 04/2019
 */

#include "robonitor.h"

#include "ble_types.h"
#include "bluenrg_conf.h"
#include "bluenrg_gatt_server.h"

/*****************************************
 * Private Defines
 *****************************************/

// Board MAC address identifier
#define BOARD_BDADDR 0x02

#define TR_UUID(uuid_13, uuid_12) \
    0x93, 0x3d, 0x87, 0xBD, 0x63, 0xD6, 0x14, 0xAB, 0xE9, 0x11, 0x47, 0x23, uuid_13, uuid_12, 0x14, 0x14

#define STRATEGIES_SERVICE_UUID TR_UUID(0x00, 0x01)
#define STRATEGIES_CHAR_UUID TR_UUID(0x00, 0x00)
#define STRATEGIES_LIST_CHAR_UUID TR_UUID(0x01, 0x00)

#define TESTE_SERVICE_UUID TR_UUID(0x01, 0x01)
#define CHAR_TEST TR_UUID(0x01, 0x02)



/*****************************************
 * Private Variables
 *****************************************/
static uint8_t zzz = 0;


/*****************************************
 * Private Functions Bodies Definitions
 *****************************************/

static void strategies_char_cb(uint8_t data_len, uint8_t* data) {
    for (uint8_t j = 0; j < data_len; j++) {
        bt_recv[zzz] = data[j];
        zzz++;
    }
}


/*****************************************
 * Public Functions Bodies Definitions
 *****************************************/

tBleStatus ble_init(char* name, led_function_t led_on, led_function_t led_toggle) {
    tBleStatus ret;

    // Main service creation
    static ble_service_t default_service = {
        .uuid = { STRATEGIES_SERVICE_UUID },
        .characteristic_count = 2,
        .characteristics =
            {
                {
                    .uuid = { STRATEGIES_CHAR_UUID },
                    .properties = CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RESP | CHAR_PROP_READ,
                    .cb = strategies_char_cb,
                },
                {
                    .uuid = { STRATEGIES_LIST_CHAR_UUID },
                    .properties = CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RESP | CHAR_PROP_READ,
                    .cb = NULL,
                }
            },
    };

    // static ble_service_t TESTE_serv = {
    //     .uuid = { TESTE_SERVICE_UUID },
    //     .characteristic_count = 1,
    //     .characteristics =
    //         {
    //             {
    //                 .uuid = { CHAR_TEST },
    //                 .properties = CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RESP | CHAR_PROP_READ,
    //                 .cb = strategies_char_cb,
    //             },
    //         },
    // };

    // All services should be added here, then ble_init()
    ble_service_t* default_services[] = {
        &default_service,
        // &TESTE_serv,
    };

    ret = SPBTLE_init(name, BOARD_BDADDR, default_services, 1);
    ble_leds(led_on, led_toggle);

    return ret;
}
