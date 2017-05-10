/**
 * @file    ic_response.c
 * @author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @author  Wojtek Weclewski <w.weclewski@inteliclinic.com>
 * @date    February, 2017
 * @brief   Brief description
 *
 * Description
 */

#include "ic_frame_constructor.h"

static bool dev_resp_cmp(char *sent_frame, char *rsp_frame);
static bool pox_resp_cmp(char *sent_frame, char *rsp_frame);
static bool alarm_resp_cmp(char *sent_frame, char *rsp_frame);

int dev_resp_frame_gen_func(char *array, size_t *len, uint8_t device, e_funcType func, uint32_t duration, uint16_t period, bool state_code, uint16_t id) {
  if (array == NULL) return ERROR_UUID;
  if (*len<FRAME_SIZE) return ERROR_UUID;

  memset(array, 0, FRAME_SIZE);

  priv_response_set(ARRAY, id, device, func, period, duration, state_code);

  priv_wrap_frame(ARRAY, RESP(DEVICE_CMD));
  SET_FRAME_SIZE(*len);

  return RESPONSE_UUID;
}

bool frame_resp_cmp(char *sent_frame, char *rsp_frame){
  switch(CAST_AR(sent_frame)->frame.cmd){
    case DEVICE_CMD:
      return dev_resp_cmp(sent_frame, rsp_frame);
    case PULSEOXIMETER_CMD:
      return pox_resp_cmp(sent_frame, rsp_frame);
    case E_ALARM_CMD:
      return alarm_resp_cmp(sent_frame, rsp_frame);
    default:
      return false;
  }
}

bool resp_frame_copy_func(char *array, size_t *len, char *payload, e_cmd cmd_type) {
  if (array == NULL) return false;
  if (*len<FRAME_SIZE) return false;

  memset(array, 0, FRAME_SIZE);

  priv_response_copy(ARRAY, PAYLOAD);
  priv_wrap_frame(ARRAY, RESP(cmd_type));
  SET_FRAME_SIZE(*len);

  return true;
}

static bool dev_resp_cmp(char *sent_frame, char *rsp_frame){
  if(CAST_AR(rsp_frame)->frame.crc != crc8_calculate(CAST_AR(rsp_frame)->data,
        sizeof(u_cmdFrameContainer)-1)) return false;
  if(RESP(CAST_AR(sent_frame)->frame.cmd) == CAST_AR(rsp_frame)->frame.cmd)
    if(CAST_AR(sent_frame)->frame.payload.device_cmd.id == CAST_AR(rsp_frame)->frame.payload.device_rsp.id)
      return true;
  return false;
}

static bool pox_resp_cmp(char *sent_frame, char *rsp_frame){
  if(CAST_AR(rsp_frame)->frame.crc != crc8_calculate(CAST_AR(rsp_frame)->data,
        sizeof(u_cmdFrameContainer)-1)) return false;
  if(RESP(CAST_AR(sent_frame)->frame.cmd) == CAST_AR(rsp_frame)->frame.cmd)
    if(CAST_AR(sent_frame)->frame.payload.pox_cmd.id == CAST_AR(rsp_frame)->frame.payload.pox_rsp.id)
      return true;
  return false;
}

static bool alarm_resp_cmp(char *sent_frame, char *rsp_frame){
  if(CAST_AR(rsp_frame)->frame.crc != crc8_calculate(CAST_AR(rsp_frame)->data,
        sizeof(u_cmdFrameContainer)-1)) return false;
  if(RESP(CAST_AR(sent_frame)->frame.cmd) == CAST_AR(rsp_frame)->frame.cmd)
    if(CAST_AR(sent_frame)->frame.payload.alarm_cmd.id == CAST_AR(rsp_frame)->frame.payload.alarm_rsp.id)
      return true;
  return false;
}
