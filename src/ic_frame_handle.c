/**
 * @file    ic_frame_handle.c
 * @author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @author  Wojtek Weclewski <w.weclewski@inteliclinic.com>
 * @date    July, 2016
 * @brief   Brief description
 *
 * Description
 *
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "ic_frame_handle.h"
#include "ic_crc8.h"

/**
 * @fn frame_validate (uint8_t *data, uint16_t len)
 * @brief
 *
 * This function does things
 *
 * @param [in] data
 * @param [in] len
 *
 */
bool neuroon_cmd_frame_validate (uint8_t *data, uint16_t len){
  u_cmdFrameContainer *cmd_frame_container = (u_cmdFrameContainer *)data;

  if(len == 0)
    return false;

  if(cmd_frame_container->frame.sync != SYNC_BYTE)
    return false;

  if(cmd_frame_container->frame.crc !=
      crc8_calculate(cmd_frame_container->data, sizeof(u_cmdFrameContainer) - 1))
    return false;

  return true;
}
