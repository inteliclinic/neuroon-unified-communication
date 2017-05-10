/**
 * @file    ic_characteristics.c
 * @author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @author  Wojtek Weclewski <w.weclewski@inteliclinic.com>
 * @date    February, 2017
 * @brief   Brief description
 *
 * Description
 */

#include <stdbool.h>
#include <string.h>
#include "ic_frame_handle.h"

#define UUID_DATA_STREAM0_TX_CHARACTERISTIC 0x0201
#define UUID_DATA_STREAM1_TX_CHARACTERISTIC 0x0202
#define UUID_STATUS_STREAM_CHARACTERISTIC   0x0301
#define UUID_RESPONSE_TX_CHARACTERISTIC     0x0302
#define UUID_CMD_RX_CHARACTERISTIC          0x0501

#define UUID_SETTINGS_TX_CHARACTERISTIC     0x0701
#define UUID_SETTINGS_RX_CHARACTERISTIC     0x0702
#define UUID_DFU_TX_CHARACTERISTIC          0x0801
#define UUID_DFU_RX_CHARACTERISTIC          0x0802

const char ic_neuroon_service_UUID[] = "D09E0100-D97F-E2D3-840C-A11CB81C0886";
const char ic_dfu_service_UUID[]     = "C7350100-D97F-E2D3-840C-A11CB81C0886";

const char data_stream0_UUID[] = "0201";
const char data_stream1_UUID[] = "0202";
const char data_status_UUID[]  = "0301";
const char response_UUID[]     = "0302";
const char cmd_UUID[]          = "0501";

const char settings_tx_UUID[]  = "0701";
const char settings_rx_UUID[]  = "0702";
const char dfu_tx_UUID[]       = "0801";
const char dfu_rx_UUID[]       = "0802";

const char *neuroon_charecteristics[] = { "0201", "0202", "0301",
                                          "0302", "0501" };

const char *dfu_charecteristics[]     = { "0701", "0702",
                                          "0801", "0802" };

bool nuc_init(char characteristics[NO_CHARECTERISTICS][UUID_LENGTH+1]){
  size_t cnt = 0;
  for(size_t i=0; i<(sizeof(neuroon_charecteristics)/sizeof(const char *)); ++i){
    strncpy(characteristics[cnt], ic_neuroon_service_UUID, UUID_LENGTH + 1);
    strncpy(&characteristics[cnt++][4], neuroon_charecteristics[i], 4);
  }

  for(size_t i=0; i<(sizeof(dfu_charecteristics)/sizeof(const char *)); ++i){
    strncpy(characteristics[cnt], ic_dfu_service_UUID, UUID_LENGTH + 1);
    strncpy(&characteristics[cnt++][4], dfu_charecteristics[i], 4);
  }
  return true;
}
