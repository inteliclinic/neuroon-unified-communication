/**
 * @file    ic_emergency_alarm.c
 * @author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @author  Wojtek Weclewski <w.weclewski@inteliclinic.com>
 * @date    February, 2017
 * @brief   Brief description
 *
 * Description
 */

#include "ic_frame_constructor.h"

int alarm_set(char *array, size_t *len, e_alarmType type, uint32_t time, uint16_t timeout, uint16_t id){
  if (array == NULL) return false;
  if(*len<FRAME_SIZE) return false;

  memset(array, 0, FRAME_SIZE);

  priv_alarm_set_cmd_id(ARRAY, id);
  priv_alarm_set_conf(ARRAY, type, time, timeout);

  priv_wrap_frame(ARRAY, E_ALARM_CMD);
  SET_FRAME_SIZE(*len);

  return CMD_UUID;
}

int alarm_off(char *array, size_t *len, uint16_t id){
  if (array == NULL) return false;
  if(*len<FRAME_SIZE) return false;

  memset(array, 0, FRAME_SIZE);

  priv_alarm_set_cmd_id(ARRAY, id);
  priv_alarm_set_type(ARRAY, ALARM_OFF);

  priv_wrap_frame(ARRAY, E_ALARM_CMD);
  SET_FRAME_SIZE(*len);

  return CMD_UUID;
}

