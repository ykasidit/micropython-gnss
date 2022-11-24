/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Jim Mussared
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "py/runtime.h"
#include "py/mperrno.h"
#include "py/mphal.h"

#if MICROPY_PY_BLUETOOTH && MICROPY_BLUETOOTH_NIMBLE

#define DEBUG_printf(...) // printf("nimble (esp32): " __VA_ARGS__)

#include "esp_nimble_hci.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"

#include "extmod/nimble/modbluetooth_nimble.h"

#include "esp_bt.h"

STATIC void ble_host_task(void *param) {
    DEBUG_printf("ble_host_task\n");
    nimble_port_run(); // This function will return only when nimble_port_stop() is executed.
    nimble_port_freertos_deinit();
}

void mp_bluetooth_nimble_port_hci_init(void) {
    DEBUG_printf("mp_bluetooth_nimble_port_hci_init\n");
    esp_nimble_hci_and_controller_init();
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT, ESP_PWR_LVL_P7);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_CONN_HDL0, ESP_PWR_LVL_P7);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_CONN_HDL1, ESP_PWR_LVL_P7);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_CONN_HDL2, ESP_PWR_LVL_P7);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_CONN_HDL3, ESP_PWR_LVL_P7);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_CONN_HDL4, ESP_PWR_LVL_P7);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_CONN_HDL5, ESP_PWR_LVL_P7);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_CONN_HDL6, ESP_PWR_LVL_P7);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_CONN_HDL7, ESP_PWR_LVL_P7);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_CONN_HDL8, ESP_PWR_LVL_P7);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, ESP_PWR_LVL_P7);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_SCAN, ESP_PWR_LVL_P7);
}

void mp_bluetooth_nimble_port_hci_deinit(void) {
    DEBUG_printf("mp_bluetooth_nimble_port_hci_deinit\n");

    esp_nimble_hci_and_controller_deinit();
}

void mp_bluetooth_nimble_port_start(void) {
    DEBUG_printf("mp_bluetooth_nimble_port_start\n");
    nimble_port_freertos_init(ble_host_task);
}

void mp_bluetooth_nimble_port_shutdown(void) {
    DEBUG_printf("mp_bluetooth_nimble_port_shutdown\n");

    // Despite the name, these is an ESP32-specific (no other NimBLE ports have these functions).
    // Calls ble_hs_stop() and waits for stack shutdown.
    nimble_port_stop();

    // Shuts down the event queue.
    nimble_port_deinit();

    // Mark stack as shutdown.
    mp_bluetooth_nimble_ble_state = MP_BLUETOOTH_NIMBLE_BLE_STATE_OFF;
}

#endif
