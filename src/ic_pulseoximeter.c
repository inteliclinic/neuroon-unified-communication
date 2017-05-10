/**
 * @file    ic_pulseoximeter.c
 * @author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @author  Wojtek Weclewski <w.weclewski@inteliclinic.com>
 * @date    October, 2016
 * @brief   Pulse-oximeter commands frame builders.
 */

#include "ic_frame_constructor.h"

int pox_hdw_init(char *array, size_t *len, uint16_t id){
  if (array == NULL) return false;
  if(*len<FRAME_SIZE) return false;

  memset(array, 0, FRAME_SIZE);

  priv_pox_set_cmd_id(ARRAY, id);
  priv_pox_set_function(ARRAY, HDW_INIT);

  priv_wrap_frame(ARRAY, PULSEOXIMETER_CMD);
  SET_FRAME_SIZE(*len);

  return CMD_UUID;
}

int pox_std_val_init(char *array, size_t *len, uint16_t id){
  if (array == NULL) return false;
  if(*len<FRAME_SIZE) return false;

  memset(array, 0, FRAME_SIZE);

  priv_pox_set_cmd_id(ARRAY, id);
  priv_pox_set_function(ARRAY, STD_VAL_INIT);

  priv_wrap_frame(ARRAY, PULSEOXIMETER_CMD);
  SET_FRAME_SIZE(*len);

  return CMD_UUID;
}

int pox_powerdown_on(char *array, size_t *len, uint16_t id){
  if (array == NULL) return false;
  if(*len<FRAME_SIZE) return false;

  memset(array, 0, FRAME_SIZE);

  priv_pox_set_cmd_id(ARRAY, id);
  priv_pox_set_function(ARRAY, POWERDOWN_ON);

  priv_wrap_frame(ARRAY, PULSEOXIMETER_CMD);
  SET_FRAME_SIZE(*len);

  return CMD_UUID;
}

int pox_powerdown_off(char *array, size_t *len, uint16_t id){
  if (array == NULL) return false;
  if(*len<FRAME_SIZE) return false;

  memset(array, 0, FRAME_SIZE);

  priv_pox_set_cmd_id(ARRAY, id);
  priv_pox_set_function(ARRAY, POWERDOWN_OFF);

  priv_wrap_frame(ARRAY, PULSEOXIMETER_CMD);
  SET_FRAME_SIZE(*len);

  return CMD_UUID;
}

int pox_read_register(char *array, size_t *len, t_afe4400Register reg, uint16_t id){
  if (array == NULL) return false;
  if(*len<FRAME_SIZE) return false;

  memset(array, 0, FRAME_SIZE);

  priv_pox_set_cmd_id(ARRAY, id);
  priv_pox_read_register(ARRAY, reg);

  priv_wrap_frame(ARRAY, PULSEOXIMETER_CMD);
  SET_FRAME_SIZE(*len);

  return CMD_UUID;
}

int pox_write_register(char *array, size_t *len, t_afe4400Register reg, t_afe4400RegisterConf reg_val, uint16_t id){
  if (array == NULL) return false;
  if(*len<FRAME_SIZE) return false;

  memset(array, 0, FRAME_SIZE);

  priv_pox_set_cmd_id(ARRAY, id);
  priv_pox_write_register(ARRAY, reg, reg_val);

  priv_wrap_frame(ARRAY, PULSEOXIMETER_CMD);
  SET_FRAME_SIZE(*len);

  return CMD_UUID;
}

int pox_self_test(char *array, size_t *len, uint16_t id){
  if (array == NULL) return false;
  if(*len<FRAME_SIZE) return false;

  memset(array, 0, FRAME_SIZE);

  priv_pox_set_cmd_id(ARRAY, id);
  priv_pox_set_function(ARRAY, SELF_TEST);

  priv_wrap_frame(ARRAY, PULSEOXIMETER_CMD);
  SET_FRAME_SIZE(*len);

  return CMD_UUID;
}

