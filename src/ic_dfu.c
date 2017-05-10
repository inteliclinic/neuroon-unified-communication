/**
 * @file    ic_dfu.c
 * @author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @author  Wojtek Weclewski <w.weclewski@inteliclinic.com>
 * @date    January, 2017
 * @brief   Brief description
 *
 * Description
 */

#include <stdint.h>
#include "ic_dfu.h"
#include "ic_frame_constructor.h"

#define DATA_END_FLAG         0x00
#define ERASE_FLAG            0x01
#define APP_START_FLAG        0x02
#define HEADER_RECEIVED_FLAG  0x03
#define EXT_DUMP_FLAG         0x04
#define INT_DUMP_FLAG         0x05
#define RESET_FLAG            0x06
#define RESPONSE_PACKET_FLAG  0x07

#define DFU_CRC_RECEIVED_FLAG 0x20
#define DFU_CRC_OK_FLAG       0x21
#define DFU_CRC_ERROR_FLAG    0x22

#define FLASH_READY_COMMAND   0x11
#define DFU_RESET_COMMAND     0x12

static struct {
  FILE *fp;
  char *file_buffer;
  uint32_t buffer_pointer;
  uint16_t frame_index;
  bool update_started;
  struct __attribute__((packed)){
    uint32_t app_type;
    uint32_t bin_version;
    uint32_t bin_length;
    uint32_t bin_crc;
    uint32_t bin_src_addr;
    uint32_t bin_dst_addr;
  }header;
}dfu_instance = {.update_started = false};

struct __attribute__((packed)) data_frame{
  uint16_t crc;
  uint16_t frame_index;
  uint8_t data[16];
};

static uint16_t crc16(uint8_t* data, uint32_t length);
static uint16_t crc16_ext(uint8_t* data_p, uint32_t length, uint16_t crc);
/*static uint16_t crc16_kn_ext(uint8_t data, uint16_t crc);*/
static void fill_frame(struct data_frame *frame, size_t *len);

int goto_dfu(char *array, size_t *len, e_firmwareMilestone firmware){
  if (array == NULL) return ERROR_UUID;
  if(*len<FRAME_SIZE) return ERROR_UUID;

  memset(array, 0, FRAME_SIZE);

  if(firmware == LEGACY_NEUROON_FIRMWARE){
    *len = 1;
    array[0] = BLE_CMD_GOTO_DFU;
    return CMD_UUID;
  }

  priv_set_dfu(ARRAY);
  ARRAY->frame.payload.enable_dfu = true;
  *len = FRAME_SIZE;

  priv_wrap_frame(ARRAY, DFU_CMD);
  return CMD_UUID;
}

int dfu_start_update(char *frame, size_t *len, char *fb, size_t file_len, e_firmwareType firm,
    uint32_t version){
  if(*len < 20) return ERROR_UUID;
  dfu_instance.header.app_type = firm;
  dfu_instance.header.bin_version = version;
  dfu_instance.header.bin_length = file_len%16 ? file_len+(16 - file_len%16) : file_len;
  dfu_instance.file_buffer = (char*)calloc(dfu_instance.header.bin_length, sizeof(char));
  if(dfu_instance.file_buffer == NULL) return ERROR_UUID;
  dfu_instance.buffer_pointer = 0;
  memcpy(dfu_instance.file_buffer, fb, file_len);
  dfu_instance.header.bin_crc = crc16_ext((uint8_t*)dfu_instance.file_buffer, dfu_instance.header.bin_length, 0xFFFF);
  frame[0] = HEADER_RECEIVED_FLAG;
  memcpy(&frame[1], &dfu_instance.header, 16);
  *len = FRAME_SIZE;
  dfu_instance.update_started = true;
  dfu_instance.frame_index = 0;
  return SETTINGS_RX_UUID;
}

int dfu_start_update_fp(char *frame, size_t *len, FILE *fp, e_firmwareType firm, uint32_t version){
  if(*len < 20) return ERROR_UUID;
  dfu_instance.fp = fp;
  dfu_instance.header.app_type = firm;
  dfu_instance.header.bin_version = version;
  fseek(fp, 0l, SEEK_END);
  dfu_instance.header.bin_length = ftell(fp);
  int diff = dfu_instance.header.bin_length % 16;
  dfu_instance.header.bin_length += diff != 0 ? (16 - diff) : 0;
  rewind(fp);
  dfu_instance.file_buffer = (char*)calloc(dfu_instance.header.bin_length, sizeof(char));
  if(dfu_instance.file_buffer == NULL) return ERROR_UUID;
  dfu_instance.buffer_pointer = 0;
  fread(dfu_instance.file_buffer, sizeof(char), dfu_instance.header.bin_length, fp);
  frame[0] = HEADER_RECEIVED_FLAG;
  memcpy(&frame[1], &dfu_instance.header, 16);
  *len = FRAME_SIZE;
  dfu_instance.update_started = true;
  return SETTINGS_RX_UUID;
}

int dfu_response_sink(char *response_frame, size_t response_len, char* frame, size_t *len,
    e_dfuAction *action){
  if(!dfu_instance.update_started)return ERROR_UUID;
  if(response_len == 0) return ERROR_UUID;
  if(dfu_instance.header.bin_length==dfu_instance.buffer_pointer){
    frame[0] = DATA_END_FLAG;
    *action = DFU_SEND_NEXT_DATASET;
    dfu_instance.buffer_pointer = 0;
    return SETTINGS_RX_UUID;
  }
  switch (response_frame[0]){
    case DFU_RESET_COMMAND:
      *action = DFU_END;
      return DFU_RX_UUID;
    case HEADER_RECEIVED_FLAG:
    case FLASH_READY_COMMAND:
    case DFU_CRC_OK_FLAG:
      *action = DFU_SEND_NEXT_DATASET;
      break;
    case DFU_CRC_ERROR_FLAG:
      if(dfu_instance.buffer_pointer >=16){
        dfu_instance.buffer_pointer -= 16*(dfu_instance.frame_index -
            *(uint16_t *)&response_frame[1]);
      }
      dfu_instance.frame_index = *(uint16_t *)&response_frame[1];
      *action = DFU_SEND_NEXT_DATASET;
      break;
    default:
      *action = DFU_TERMINATE;
      return ERROR_UUID;
  }
  fill_frame((struct data_frame *)frame, len);
  return DFU_RX_UUID;
}

static uint16_t crc16(uint8_t* data, uint32_t length){
  uint8_t x;
  uint16_t crc = 0xFFFF;
  while (length--){
    x = crc >> 8 ^ *data++;
    x ^= x>>4;
    crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x <<5)) ^ ((uint16_t)x);
  }
  return crc;
}

static uint16_t crc16_ext(uint8_t* data_p, uint32_t length, uint16_t crc){
  uint8_t x;
  while (length--){
    x = crc >> 8 ^ *data_p++;
    x ^= x>>4;
    crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x <<5)) ^ ((uint16_t)x);
  }
  return crc;
}

/*
 *static uint16_t crc16_kn_ext(uint8_t data, uint16_t crc){
 *  uint8_t x;
 *  x = crc >> 8 ^ data;
 *  x ^= x>>4;
 *  crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x <<5)) ^ ((uint16_t)x);
 *  return crc;
 *}
 */

static void fill_frame(struct data_frame *frame, size_t *len){
  frame->frame_index = dfu_instance.frame_index++;
  memcpy(frame->data, &dfu_instance.file_buffer[dfu_instance.buffer_pointer], 16);
  frame->crc = crc16((uint8_t *)&frame->frame_index, 18);
  *len = sizeof(struct data_frame);
  dfu_instance.buffer_pointer += 16;
}
