/**
 * @file    ic_status.c
 * @author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @author  Wojtek Weclewski <w.weclewski@inteliclinic.com>
 * @date    January, 2017
 * @brief   LTC devices commands frame buliders.
 */

#include "ic_frame_constructor.h"

bool status_cmd_gen_func(char *array, size_t *len, uint16_t id) {
  if (array == NULL) return false;
  if (*len<FRAME_SIZE) return false;

  memset(array, 0, FRAME_SIZE);

  priv_status_cmd_payload_set(ARRAY, id);

  priv_wrap_frame(ARRAY, STATUS_CMD);
  SET_FRAME_SIZE(*len);

  return true;
}

bool status_rsp_gen_func(char *array, size_t *len, s_devsFunc devs_func, uint8_t active_data_streams, uint16_t id) {
  if (array == NULL) return false;
  if (*len<FRAME_SIZE) return false;

  memset(array, 0, FRAME_SIZE);

  priv_status_rsp_payload_set(ARRAY, id, devs_func, active_data_streams);

  priv_wrap_frame(ARRAY, RESP(STATUS_CMD));
  SET_FRAME_SIZE(*len);

  return true;
}

