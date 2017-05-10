/**
 * @file    ic_frame_constructor.h
 * @author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @author  Wojtek Weclewski <w.weclewski@inteliclinic.com>
 * @date    January, 2017
 * @brief   Brief description
 *
 * Description
 */

#ifndef IC_UC_FRAME_CONSTRUCTOR_H
#define IC_UC_FRAME_CONSTRUCTOR_H

#include <stdlib.h>
#include <string.h>
#include "ic_frame_handle.h"
#include "ic_low_level_control.h"
#include "ic_crc8.h"

#define MAX_LED_VAL 63
#define	BLE_CMD_GOTO_DFU 0x31
#define SATURATION(v,s) v = v>s ? s : v
#define ARRAY ((u_cmdFrameContainer *)array)
#define CAST_AR(array) ((u_cmdFrameContainer *)array)
#define PAYLOAD ((u_BLECmdPayload *)payload)
#define SET_FRAME_SIZE(size) size=sizeof(u_cmdFrameContainer)

#define FRAME_SIZE 20

///////////// COLORS /////////////
#define RED_COLOR_RED_LED     0x3F
#define RED_COLOR_GREEN_LED   0x00
#define RED_COLOR_BLUE_LED    0x00
#define GREEN_COLOR_RED_LED   0x00
#define GREEN_COLOR_GREEN_LED 0x3F
#define GREEN_COLOR_BLUE_LED  0x00
#define BLUE_COLOR_RED_LED    0x00
#define BLUE_COLOR_GREEN_LED  0x00
#define BLUE_COLOR_BLUE_LED   0x3F
#define WHITE_COLOR_RED_LED   0x3F
#define WHITE_COLOR_GREEN_LED 0x3F
#define WHITE_COLOR_BLUE_LED  0x3F
#define TEAL_COLOR_RED_LED    0x00
#define TEAL_COLOR_GREEN_LED  0x1F
#define TEAL_COLOR_BLUE_LED   0x1F

void priv_set_sync(u_cmdFrameContainer *frame);
void priv_set_cmd(u_cmdFrameContainer *frame, e_cmd cmd);
void priv_set_cmd_id(u_cmdFrameContainer *frame, uint16_t id);
void priv_calculate_crc(u_cmdFrameContainer *frame);
void priv_left_red_set_val(u_cmdFrameContainer *frame, uint8_t intensity);
void priv_left_green_set_val(u_cmdFrameContainer *frame, uint8_t intensity);
void priv_left_blue_set_val(u_cmdFrameContainer *frame, uint8_t intensity);
void priv_right_red_set_val(u_cmdFrameContainer *frame, uint8_t intensity);
void priv_right_green_set_val(u_cmdFrameContainer *frame, uint8_t intensity);
void priv_right_blue_set_val(u_cmdFrameContainer *frame, uint8_t intensity);
void priv_vibrator_set_val(u_cmdFrameContainer *frame, uint8_t intensity);
void priv_power_led_set_val(u_cmdFrameContainer *frame);
void priv_set_device(u_cmdFrameContainer *frame, uint8_t device);
void priv_wrap_frame(u_cmdFrameContainer *frame, e_cmd cmd);
void priv_set_function(u_cmdFrameContainer *frame, e_funcType function, uint32_t duration,
    uint16_t period);
void priv_rgb_led_left_ON(u_cmdFrameContainer *frame, bool set_func);
void priv_rgb_led_right_ON(u_cmdFrameContainer *frame, e_funcType set_func);
void priv_rgb_led_left_set_color(u_cmdFrameContainer *frame, e_rgbLedColor color,
    uint8_t intensity, bool set_func);
void priv_rgb_led_right_set_color(u_cmdFrameContainer *frame, e_rgbLedColor color,
    uint8_t intensity, bool set_func);
void priv_response_set(u_cmdFrameContainer *frame, uint16_t id, uint8_t device, e_funcType func,
    uint32_t duration, uint16_t period, bool state_code);
void priv_response_copy(u_cmdFrameContainer *frame, u_BLECmdPayload *payload);
void priv_pox_set_cmd_id(u_cmdFrameContainer *frame, uint16_t id);
void priv_pox_set_function(u_cmdFrameContainer *frame, e_poxFuncType function);
void priv_pox_read_register(u_cmdFrameContainer *frame, t_afe4400Register reg);
void priv_pox_write_register(u_cmdFrameContainer *frame, t_afe4400Register reg,
    t_afe4400RegisterConf reg_val);
void priv_alarm_set_cmd_id(u_cmdFrameContainer *frame, uint16_t id);
void priv_alarm_set_conf(u_cmdFrameContainer *frame, e_alarmType type, uint32_t time, uint16_t timeout);
void priv_alarm_set_type(u_cmdFrameContainer *frame, e_alarmType type);
void priv_status_cmd_payload_set(u_cmdFrameContainer *frame, uint16_t id);
void priv_status_rsp_payload_set(u_cmdFrameContainer *frame, uint16_t id, s_devsFunc devs_func,
    uint8_t active_data_streams);
void priv_set_dfu(u_cmdFrameContainer *frame);

#endif /* !IC_UC_FRAME_CONSTRUCTOR_H */
