/**
 * @file    ic_frame_handle_batch.h
 * @author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @author  Wojtek Weclewski <w.weclewski@inteliclinic.com>
 * @date    September, 2016
 * @brief   Brief description
 *
 * Description
 */

#ifndef IC_FRAME_HANDLE_BATCH_H
#define IC_FRAME_HANDLE_BATCH_H

#include <stdbool.h>
#include <stdint.h>

/// ---===CMD FIELD===--- ///
typedef enum __attribute__((packed)){
  RGB_LED_CMD       = 0x01,
  POWER_LED_CMD     = 0x02,
  VIBRATOR_CMD      = 0x03,
  PULSEOXIMETER_CMD = 0x04,
  E_ALARM_CMD       = 0x05,
  DEVICE_CMD        = 0x10,
  STATUS_CMD        = 0x11,
  /*
   * .
   * .
   * .
   *
   */
  DFU_CMD           = 0x20,
  FEED_CMD          = 0x21,
  UNLOCK_MASK       = 0xAF,
  TEST_CMD          = 0xBB,
  FLASH_BQ_CMD      = 0xBD,
  SHUTDOWN_CMD      = 0xDF
}e_cmd;

///Make response command code from command code.
#define RESP(CMD) ((CMD)|(0x80))

/**
 * @brief Function Type
 *
 * Enumeration contains set of available devices behavior.
 */
typedef enum __attribute__((packed)) {
  FUN_TYPE_OFF = 0x01, /*!< Turn off the device */
  FUN_TYPE_ON,         /*!< Turn on the device using provided intesity*/
  FUN_TYPE_SIN_WAVE,   /*!< Execute sinus wave on device, not available for power leds*/
  FUN_TYPE_BLINK,      /*!< Execute stroboscope like effect on device, not available for vibrator*/
  FUN_TYPE_SQUARE,     /*!< Execute square function on device, not available for power leds*/
  FUN_TYPE_SAW,        /*!< Execute saw like function on device, not available for power leds*/
  FUN_TYPE_TRIANGLE,   /*!< Execute triangular function on device, not available for power leds*/
  FUN_TYPE_RAMP        /*!< Execute ramp function on device, not available for power leds*/
}e_funcType;

/**
 * @brief Mode of request to AFE4400 driver.
 *
 * User may WRITE to and READ from AFE4400 register or execute predefined function.
 */
typedef enum __attribute__((packed)) {
  READ_REG = 0x01,
  WRITE_REG,
  EXEC_FUNC
}e_reqMode;

/**
 * @brief AFE4400 function type.
 *
 * AFE4400 predefined functions.
 */
typedef enum __attribute__((packed)){
  HDW_INIT = 0x01,
  STD_VAL_INIT,
  POWERDOWN_ON,
  POWERDOWN_OFF,
  SELF_TEST
}e_poxFuncType;

typedef uint32_t t_afe4400RegisterConf;
typedef uint8_t t_afe4400Register;

/**
 * @brief Emergency alarm types.
 */
typedef enum __attribute__((packed)){
  ALARM_SOFT = 0x01,
  ALARM_MEDIUM,
  ALARM_HARD,
  ALARM_OFF
}e_alarmType;

/// ---===PAYLOAD FIELD===--- ///

/**
 * @brief Devices functions.
 *
 * Container for informations about current devices functions.
 */
typedef struct __attribute__((packed)){
  e_funcType func_of_right_red_led;
  e_funcType func_of_right_green_led;
  e_funcType func_of_right_blue_led;
  e_funcType func_of_left_red_led;
  e_funcType func_of_left_green_led;
  e_funcType func_of_left_blue_led;
  e_funcType func_of_vibrator;
  e_funcType func_of_pwr_led;
}s_devsFunc;

/*====================== TODO: DO WYWALENIA ========================*/
typedef struct __attribute__((packed)){
  enum __attribute__((packed)){
    LEFT_RGB    = 0x01,
    RIGHT_RGB   = 0x02,
    BOTH_RGB    = 0x03
  }led;
  uint8_t rr;
  uint8_t gr;
  uint8_t br;
  uint8_t rl;
  uint8_t gl;
  uint8_t bl;
}s_rgbCmd;

typedef struct __attribute__((packed)){
  enum __attribute__((packed)){
    STOP_FLASH = 0x00,
    PWR_FLASH    = 0x01
    //PWR_CONT = 0x02 //UNSTABLE!!!
  }power_led;
  // uint8_t intensity;
}s_powerCmd;

typedef struct __attribute__((packed)){
  enum __attribute__((packed)){
    STOP_VIB  = 0x00,
    SAW_VIB       = 0x01,
    TRIANGLE_VIB  = 0x02,
    SIN_VIB       = 0x03,
    SQUARE_VIB    = 0x04,
    CONST_VIB     = 0x05
  }function;
  uint8_t intensity;
}s_vibraCmd;

//typedef struct __attribute__((packed)){
  //enum __attribute__((packed)){
    //OXIMETER_OFF = 0x00,
    //OXIMETER_ON = 0x01,
    //RED_LED_ON = 0x02,
    //RED_LED_OFF = 0x03,
    //IR_LED_ON = 0x04,
    //IR_LED_OFF = 0x05
  //}function;
  //uint8_t intensity;
//}s_oximeterCmd;
/*====================== TODO: koniec DO WYWALENIA ========================*/

typedef struct __attribute__((packed)){
  /// ---===ID FIELD===--- ///
  uint16_t id;

  /*struct {*/
    /*uint8_t right_red_led : 1;*/
    /*uint8_t right_green_led : 1;*/
    /*uint8_t right_blue_led : 1;*/
    /*uint8_t left_red_led : 1;*/
    /*uint8_t left_green_led : 1;*/
    /*uint8_t left_blue_led : 1;*/
    /*uint8_t vibrator : 1;*/
    /*uint8_t pwr_led : 1;*/
  /*}device;*/
  uint8_t device;

  e_funcType func_type;

  union{
    struct __attribute__((packed)){
      uint32_t duration;
    }on_func;
    struct __attribute__((packed)){
      uint32_t duration;
      uint16_t period;
    }periodic_func;
  }func_parameter;

  struct __attribute__((packed)){
    uint8_t right_red_led;
    uint8_t right_green_led;
    uint8_t right_blue_led;
    uint8_t left_red_led;
    uint8_t left_green_led;
    uint8_t left_blue_led;
    uint8_t vibrator;
  }intensity;
}s_deviceCmd;

typedef struct __attribute__((packed)){
  /// ---===ID FIELD===--- ///
  uint16_t id;

  /*struct {*/
    /*uint8_t right_red_led : 1;*/
    /*uint8_t right_green_led : 1;*/
    /*uint8_t right_blue_led : 1;*/
    /*uint8_t left_red_led : 1;*/
    /*uint8_t left_green_led : 1;*/
    /*uint8_t left_blue_led : 1;*/
    /*uint8_t vibrator : 1;*/
    /*uint8_t pwr_led : 1;*/
  /*}device;*/
  uint8_t device;

  e_funcType func_type;

  uint32_t duration;
  uint16_t period;

  bool state_code;
}s_deviceRsp;

typedef struct __attribute__((packed)){
  /// ---===ID FIELD===--- ///
  uint16_t id;

  e_reqMode mode;

  union {
    e_poxFuncType function;
    struct __attribute__((packed)){
      t_afe4400Register reg;
      t_afe4400RegisterConf reg_val;
    }reg_service;
  }request;
}s_poxCmd;

typedef struct __attribute__((packed)){
  /// ---===ID FIELD===--- ///
  uint16_t id;

  e_reqMode mode;

  union {
    e_poxFuncType function;
    struct __attribute__((packed)){
      t_afe4400Register reg;
      t_afe4400RegisterConf reg_val;
    }reg_service;
  }request;

  bool state_code;
}s_poxRsp;

typedef struct __attribute__((packed)){
  /// ---===ID FIELD===--- ///
  uint16_t id;

  e_alarmType type;
  uint32_t time_to_alarm;
  uint16_t timeout;
}s_alarmCmd;

typedef struct __attribute__((packed)){
  /// ---===ID FIELD===--- ///
  uint16_t id;

  e_alarmType type;
  uint32_t time_to_alarm;
  uint16_t timeout;

  bool state_code;
}s_alarmRsp;

typedef struct __attribute__((packed)){
  /// ---===ID FIELD===--- ///
  uint16_t id;
  /*for future development*/
}s_statusCmd;

typedef struct __attribute__((packed)){
  /// ---===ID FIELD===--- ///
  uint16_t id;

  s_devsFunc devs_func;

  struct {
    uint8_t is_eeg_stream_active : 1;
    uint8_t is_ir_stream_active : 1;
    uint8_t is_red_stream_active : 1;
    uint8_t is_acc_stream_active : 1;
    uint8_t is_temp_stream_active : 1;
  }active_data_stream;
}s_statusRsp;

typedef union {
  uint8_t data[17];
  s_rgbCmd rgb_cmd;
  s_powerCmd power_cmd;
  s_vibraCmd vibra_cmd;
  //s_oximeterCmd oximeter_cmd;
  s_deviceCmd device_cmd;
  s_deviceRsp device_rsp;
  s_poxCmd pox_cmd;
  s_poxRsp pox_rsp;
  s_alarmCmd alarm_cmd;
  s_alarmRsp alarm_rsp;
  s_statusCmd status_cmd;
  s_statusRsp status_rsp;
  bool enable_dfu;
  bool enable_feed;
  bool unlock_mask;
}u_BLECmdPayload;

#endif /* !IC_FRAME_HANDLE_BATCH_H */
