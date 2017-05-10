/**
 * @file    ic_frame_handle.h
 * @author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @author  Wojtek Weclewski <w.weclewski@inteliclinic.com>
 * @date    July, 2016
 * @brief   Brief description
 *
 * Description
 */

#ifndef IC_FRAME_HANDLE_H
#define IC_FRAME_HANDLE_H

#ifdef __cplusplus
extern "C" {
#endif

#define UUID_LENGTH             36  //
#define NEUROON_CHARECTERISTICS 5   // Dear God, forgive me for it being a fixed value
#define DFU_CHARECTERISTICS     4   //
#define NO_CHARECTERISTICS NEUROON_CHARECTERISTICS+DFU_CHARECTERISTICS

#define SYNC_BYTE   0xEE

#include "include/ic_frame_handle_batch.h"

typedef enum{
  DATA_STREAM0_UUID   = 0x00,
  DATA_STREAM1_UUID   = 0x01,
  STATUS_STREAM_UUID  = 0x02,
  RESPONSE_UUID       = 0x03,
  CMD_UUID            = 0x04,
  SETTINGS_TX_UUID    = 0x05,
  SETTINGS_RX_UUID    = 0x06,
  DFU_TX_UUID         = 0x07,
  DFU_RX_UUID         = 0x08,
  ERROR_UUID          = 0xFF
}e_characteristics;

typedef struct __packed {
  /// ---===SYNC FIELD===--- ///
  uint8_t sync;

  /// ---===CMD FIELD===--- ///
  e_cmd cmd;

  /// ---===PAYLOAD FIELD===--- ///
  u_BLECmdPayload payload;

  /// ---===CRC FIELD===--- ///
  uint8_t crc;
} s_frame;

typedef union {
  uint8_t data[sizeof(s_frame)];
  s_frame frame;
} u_cmdFrameContainer;

typedef union {
  uint8_t raw_data[20];
  struct __attribute__((packed)){
    uint32_t  time_stamp; //4
    int32_t   ir_sample;  //8
    int32_t   red_sample; //12
    int16_t   acc[3];     //18
    int8_t    temp[2];    //20
  }frame;
} u_otherDataFrameContainer;

typedef union {
  uint8_t raw_data[20];
  struct __attribute__((packed)) {
    uint32_t time_stamp;
    int16_t eeg_data[8];  // measurements
  }frame;
}u_eegDataFrameContainter;

bool neuroon_cmd_frame_validate (uint8_t *data, uint16_t len);
bool nuc_init(char characteristics[NO_CHARECTERISTICS][UUID_LENGTH+1]);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !IC_FRAME_HANDLE_H */
