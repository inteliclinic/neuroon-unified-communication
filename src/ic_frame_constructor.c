/**
 * @file    ic_frame_constructor.c
 * @author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @author  Wojtek Weclewski <w.weclewski@inteliclinic.com>
 * @date    February, 2017
 * @brief   Brief description
 *
 * Description
 */

#include "ic_frame_constructor.h"

#pragma GCC visibility push(hidden)
typedef enum{
  RGB_LED_RED = 0x00,
  RGB_LED_GREEN = 0x01,
  RGB_LED_BLUE = 0x02
}e_rgbLed;

const uint8_t colors[RGB_LED_COLOR_NO_OF_COLORS][3] =
{
  {RED_COLOR_RED_LED, RED_COLOR_GREEN_LED, RED_COLOR_BLUE_LED},
  {GREEN_COLOR_RED_LED, GREEN_COLOR_GREEN_LED, GREEN_COLOR_BLUE_LED},
  {BLUE_COLOR_RED_LED, BLUE_COLOR_GREEN_LED, BLUE_COLOR_BLUE_LED},
  {WHITE_COLOR_RED_LED, WHITE_COLOR_GREEN_LED, WHITE_COLOR_BLUE_LED},
  {TEAL_COLOR_RED_LED, TEAL_COLOR_GREEN_LED, TEAL_COLOR_BLUE_LED},
  {0, 0, 0},
};

void priv_set_sync(u_cmdFrameContainer *frame){
  frame->frame.sync = SYNC_BYTE;\
}

void priv_set_cmd(u_cmdFrameContainer *frame, e_cmd cmd){
  frame->frame.cmd = (cmd);
}

void priv_set_cmd_id(u_cmdFrameContainer *frame, uint16_t id){
  frame->frame.payload.device_cmd.id = id;
}

void priv_calculate_crc(u_cmdFrameContainer *frame){
  frame->frame.crc = crc8_calculate(frame->data, sizeof(u_cmdFrameContainer)-1);
}

void priv_left_red_set_val(u_cmdFrameContainer *frame, uint8_t intensity){
  DEV_SET(frame->frame.payload.device_cmd.device,DEV_LEFT_RED_LED);
  frame->frame.payload.device_cmd.intensity.left_red_led = intensity;
}

void priv_left_green_set_val(u_cmdFrameContainer *frame, uint8_t intensity){
  DEV_SET(frame->frame.payload.device_cmd.device,DEV_LEFT_GREEN_LED);
  frame->frame.payload.device_cmd.intensity.left_green_led = intensity;
}

void priv_left_blue_set_val(u_cmdFrameContainer *frame, uint8_t intensity){
  DEV_SET(frame->frame.payload.device_cmd.device,DEV_LEFT_BLUE_LED);
  frame->frame.payload.device_cmd.intensity.left_blue_led = intensity;
}

void priv_right_red_set_val(u_cmdFrameContainer *frame, uint8_t intensity){
  DEV_SET(frame->frame.payload.device_cmd.device,DEV_RIGHT_RED_LED);
  frame->frame.payload.device_cmd.intensity.right_red_led = intensity;
}

void priv_right_green_set_val(u_cmdFrameContainer *frame, uint8_t intensity){
  DEV_SET(frame->frame.payload.device_cmd.device,DEV_RIGHT_GREEN_LED);
  frame->frame.payload.device_cmd.intensity.right_green_led = intensity;
}

void priv_right_blue_set_val(u_cmdFrameContainer *frame, uint8_t intensity){
  DEV_SET(frame->frame.payload.device_cmd.device,DEV_RIGHT_BLUE_LED);
  frame->frame.payload.device_cmd.intensity.right_blue_led = intensity;
}

void priv_vibrator_set_val(u_cmdFrameContainer *frame, uint8_t intensity){
  DEV_SET(frame->frame.payload.device_cmd.device,DEV_VIBRATOR);
  frame->frame.payload.device_cmd.intensity.vibrator = intensity;
}

void priv_power_led_set_val(u_cmdFrameContainer *frame){
  DEV_SET(frame->frame.payload.device_cmd.device,DEV_POWER_LED);
}

void priv_set_device(u_cmdFrameContainer *frame, uint8_t device){
  frame->frame.payload.device_cmd.device = device;
}

void priv_wrap_frame(u_cmdFrameContainer *frame, e_cmd cmd){
  priv_set_sync(frame);
  priv_set_cmd(frame, cmd);
  priv_calculate_crc(frame);
}

void priv_set_function(u_cmdFrameContainer *frame, e_funcType function, uint32_t duration,
    uint16_t period){
  frame->frame.payload.device_cmd.func_type = function;
  if((function != FUN_TYPE_OFF && function != FUN_TYPE_BLINK)){
      frame->frame.payload.device_cmd.func_parameter.periodic_func.period = period;
      frame->frame.payload.device_cmd.func_parameter.periodic_func.duration = duration;
  }else{
    frame->frame.payload.device_cmd.func_parameter.on_func.duration = 0;
  }
}

void priv_rgb_led_left_ON(u_cmdFrameContainer *frame, bool set_func){
  if(set_func == true) priv_set_function(frame, FUN_TYPE_ON, 0, 0);
  priv_left_red_set_val(frame, 60);
  priv_left_green_set_val(frame, 60);
  priv_left_blue_set_val(frame, 10);
}

void priv_rgb_led_right_ON(u_cmdFrameContainer *frame, e_funcType set_func){
  if(set_func == true) priv_set_function(frame, FUN_TYPE_ON, 0, 0);
  priv_right_red_set_val(frame, 60);
  priv_right_green_set_val(frame, 60);
  priv_right_blue_set_val(frame, 10);
}

void priv_rgb_led_left_set_color(u_cmdFrameContainer *frame, e_rgbLedColor color,
    uint8_t intensity, bool set_func){
  if(set_func == true) priv_set_function(frame, FUN_TYPE_ON, 0, 0);
  priv_left_red_set_val(frame,(colors[color][RGB_LED_RED]*intensity)/MAX_LED_VAL);
  priv_left_green_set_val(frame, (colors[color][RGB_LED_GREEN]*intensity)/MAX_LED_VAL);
  priv_left_blue_set_val(frame, (colors[color][RGB_LED_BLUE]*intensity)/MAX_LED_VAL);
}

void priv_rgb_led_right_set_color(u_cmdFrameContainer *frame, e_rgbLedColor color,
    uint8_t intensity, bool set_func){
  if(set_func == true) priv_set_function(frame, FUN_TYPE_ON, 0, 0);
  priv_right_red_set_val(frame,(colors[color][RGB_LED_RED]*intensity)/MAX_LED_VAL);
  priv_right_green_set_val(frame, (colors[color][RGB_LED_GREEN]*intensity)/MAX_LED_VAL);
  priv_right_blue_set_val(frame, (colors[color][RGB_LED_BLUE]*intensity)/MAX_LED_VAL);
}

void priv_response_set(u_cmdFrameContainer *frame, uint16_t id, uint8_t device, e_funcType func,
    uint32_t duration, uint16_t period, bool state_code){
  frame->frame.payload.device_rsp.id = id;
  frame->frame.payload.device_rsp.device = device;
  frame->frame.payload.device_rsp.func_type = func;
  frame->frame.payload.device_rsp.duration = duration;
  frame->frame.payload.device_rsp.period = period;
  frame->frame.payload.device_rsp.state_code = state_code;
}

void priv_response_copy(u_cmdFrameContainer *frame, u_BLECmdPayload *payload){
  memcpy(&(frame->frame.payload), payload, sizeof(u_BLECmdPayload));
}

void priv_pox_set_cmd_id(u_cmdFrameContainer *frame, uint16_t id){
  frame->frame.payload.pox_cmd.id = id;
}

void priv_pox_set_function(u_cmdFrameContainer *frame, e_poxFuncType function){
  frame->frame.payload.pox_cmd.mode = EXEC_FUNC;
  frame->frame.payload.pox_cmd.request.function = function;
}

void priv_pox_read_register(u_cmdFrameContainer *frame, t_afe4400Register reg){
  frame->frame.payload.pox_cmd.mode = READ_REG;
  frame->frame.payload.pox_cmd.request.reg_service.reg = reg;
}

void priv_pox_write_register(u_cmdFrameContainer *frame, t_afe4400Register reg, t_afe4400RegisterConf reg_val){
  frame->frame.payload.pox_cmd.mode = WRITE_REG;
  frame->frame.payload.pox_cmd.request.reg_service.reg = reg;
  frame->frame.payload.pox_cmd.request.reg_service.reg_val = reg_val;
}

void priv_alarm_set_cmd_id(u_cmdFrameContainer *frame, uint16_t id){
  frame->frame.payload.alarm_cmd.id = id;
}

void priv_alarm_set_conf(u_cmdFrameContainer *frame, e_alarmType type, uint32_t time, uint16_t timeout){
  frame->frame.payload.alarm_cmd.type = type;
  frame->frame.payload.alarm_cmd.time_to_alarm = time;
  frame->frame.payload.alarm_cmd.timeout = timeout;
}

void priv_alarm_set_type(u_cmdFrameContainer *frame, e_alarmType type){
  frame->frame.payload.alarm_cmd.type = type;
}

void priv_status_cmd_payload_set(u_cmdFrameContainer *frame, uint16_t id){
  frame->frame.payload.device_cmd.id = id;
}

void priv_status_rsp_payload_set(u_cmdFrameContainer *frame, uint16_t id, s_devsFunc devs_func, uint8_t active_data_streams){
  frame->frame.payload.status_rsp.id = id;
  frame->frame.payload.status_rsp.devs_func = devs_func;
  memcpy(&(frame->frame.payload.status_rsp.active_data_stream), &(active_data_streams), sizeof(active_data_streams));
}

void priv_set_dfu(u_cmdFrameContainer *frame){
  frame->frame.payload.enable_dfu = true;
}

#pragma GCC visibility pop
