/**
 * @file    ic_frame_constructor.c
 * @author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @author  Wojtek Weclewski <w.weclewski@inteliclinic.com>
 * @date    October, 2016
 * @brief   LTC devices commands frame buliders.
 */

#include "ic_dfu.h"
#include "ic_frame_constructor.h"

//////////////////////////////////

int device_set_func(char *array, size_t *len, uint8_t device, e_funcType func, uint8_t *intensity,
    uint32_t duration, uint16_t period, uint16_t id){ //TODO: intensity array... there has to be a
                                                      //      better way...
  if (array == NULL) return ERROR_UUID;
  if(*len<FRAME_SIZE) return ERROR_UUID;

  memset(array, 0, FRAME_SIZE);

  priv_set_cmd_id(ARRAY, id);
  priv_set_device(ARRAY, device);
  priv_set_function(ARRAY, func, duration, period);

  for(unsigned int i=0; i<sizeof(ARRAY->frame.payload.device_cmd.intensity); ++i)
    ((uint8_t *)&ARRAY->frame.payload.device_cmd.intensity)[i] = device&(0x01<<i)?intensity[i]:0;

  priv_wrap_frame(ARRAY, DEVICE_CMD);
  SET_FRAME_SIZE(*len);
  return CMD_UUID;
}

int rgb_led_ON(char *array, size_t *len, e_rgbLedSide rgb_led, uint16_t id){
  if (array == NULL) return ERROR_UUID;
  if(*len<FRAME_SIZE) return ERROR_UUID;

  memset(array, 0, FRAME_SIZE);

  priv_set_cmd_id(ARRAY, id);
  priv_set_function(ARRAY, FUN_TYPE_ON, 0, 0);

  if(rgb_led != RGB_LED_SIDE_LEFT)
    priv_rgb_led_right_ON(ARRAY, false);
  if (RGB_LED_SIDE_RIGHT)
    priv_rgb_led_left_ON(ARRAY, false);

  priv_wrap_frame(ARRAY, DEVICE_CMD);
  SET_FRAME_SIZE(*len);

  return CMD_UUID;
}

int rgb_led_set_color(char *array, size_t *len, e_rgbLedSide rgb_led,
    e_rgbLedColor color, uint8_t intensity, uint16_t id){
  if (array == NULL) return ERROR_UUID;
  if(*len<FRAME_SIZE) return ERROR_UUID;

  memset(array, 0, FRAME_SIZE);

  priv_set_cmd_id(ARRAY, id);
  priv_set_function(ARRAY, FUN_TYPE_ON, 0, 0);

  if(rgb_led != RGB_LED_SIDE_LEFT)
    priv_rgb_led_right_set_color(ARRAY, color, intensity, false);
  if(rgb_led != RGB_LED_SIDE_RIGHT)
    priv_rgb_led_left_set_color(ARRAY, color, intensity, false);

  priv_wrap_frame(ARRAY, DEVICE_CMD);
  SET_FRAME_SIZE(*len);

  return CMD_UUID;
}

int rgb_led_set_func(char *array, size_t *len, e_rgbLedSide rgb_led,
    e_funcType func, e_rgbLedColor color, uint8_t intensity, uint32_t duration,
    uint16_t period, uint16_t id){
  if (array == NULL) return ERROR_UUID;
  if(*len<FRAME_SIZE) return ERROR_UUID;

  memset(array, 0, FRAME_SIZE);

  priv_set_cmd_id(ARRAY, id);
  priv_set_function(ARRAY, func, duration, period);
  if(rgb_led != RGB_LED_SIDE_LEFT)
    priv_rgb_led_right_set_color(ARRAY, color, intensity, false);
  if(rgb_led != RGB_LED_SIDE_RIGHT)
    priv_rgb_led_left_set_color(ARRAY, color, intensity, false);

  priv_wrap_frame(ARRAY, DEVICE_CMD);
  SET_FRAME_SIZE(*len);

  return CMD_UUID;
}

int vibrator_ON(char *array, size_t *len, uint16_t id){
  if (array == NULL) return ERROR_UUID;
  if(*len<FRAME_SIZE) return ERROR_UUID;

  memset(array, 0, FRAME_SIZE);

  priv_set_cmd_id(ARRAY, id);
  priv_vibrator_set_val(ARRAY, 255);
  priv_set_function(ARRAY, FUN_TYPE_ON, 0, 0);

  priv_wrap_frame(ARRAY, DEVICE_CMD);
  SET_FRAME_SIZE(*len);

  return CMD_UUID;
}

int vibrator_set_value(char *array, size_t *len, uint8_t intensity, uint16_t id){
  if (array == NULL) return ERROR_UUID;
  if (*len<FRAME_SIZE) return ERROR_UUID;

  memset(array, 0, FRAME_SIZE);

  priv_set_cmd_id(ARRAY, id);
  priv_vibrator_set_val(ARRAY, intensity);
  priv_set_function(ARRAY, FUN_TYPE_ON, 0, 0);

  priv_wrap_frame(ARRAY, DEVICE_CMD);
  SET_FRAME_SIZE(*len);

  return CMD_UUID;
}

int vibrator_set_func(char *array, size_t *len, e_funcType func, uint8_t intensity,
    uint32_t duration, uint16_t period, uint16_t id){
  if (array == NULL) return ERROR_UUID;
  if (*len<FRAME_SIZE) return ERROR_UUID;

  memset(array, 0, FRAME_SIZE);

  priv_set_cmd_id(ARRAY, id);
  priv_vibrator_set_val(ARRAY, intensity);
  priv_set_function(ARRAY, func, duration, period);

  priv_wrap_frame(ARRAY, DEVICE_CMD);
  SET_FRAME_SIZE(*len);

  return CMD_UUID;
}

int pwr_led_ON(char *array, size_t *len, uint16_t id){
  if (array == NULL) return ERROR_UUID;
  if (*len<FRAME_SIZE) return ERROR_UUID;

  memset(array, 0, FRAME_SIZE);

  priv_set_cmd_id(ARRAY, id);
  priv_power_led_set_val(ARRAY);
  priv_set_function(ARRAY, FUN_TYPE_ON, 0, 0);

  priv_wrap_frame(ARRAY, DEVICE_CMD);
  SET_FRAME_SIZE(*len);

  return CMD_UUID;
}

int pwr_led_set_value(char *array, size_t *len, uint16_t id){
  if (array == NULL) return ERROR_UUID;
  if (*len<FRAME_SIZE) return ERROR_UUID;

  memset(array, 0, FRAME_SIZE);

  priv_set_cmd_id(ARRAY, id);
  priv_power_led_set_val(ARRAY);
  priv_set_function(ARRAY, FUN_TYPE_ON, 0, 0);

  priv_wrap_frame(ARRAY, DEVICE_CMD);
  SET_FRAME_SIZE(*len);

  return CMD_UUID;
}

int pwr_led_set_func(char *array, size_t *len, e_funcType func,
    uint32_t duration, uint16_t period, uint16_t id){
  if (array == NULL) return ERROR_UUID;
  if (*len<FRAME_SIZE) return ERROR_UUID;

  memset(array, 0, FRAME_SIZE);

  priv_set_cmd_id(ARRAY, id);
  priv_power_led_set_val(ARRAY);
  priv_set_function(ARRAY, func, duration, period);

  priv_wrap_frame(ARRAY, DEVICE_CMD);
  SET_FRAME_SIZE(*len);

  return CMD_UUID;
}

