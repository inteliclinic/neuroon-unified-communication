/**
 * @file    ic_low_level_control.h
 * @author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @author  Wojtek Weclewski <w.weclewski@inteliclinic.com>
 * @date    October, 2016
 * @brief   Low level Neuroon mask control API interface
 *
 * Functions provided with this file populates memory with data understandable
 * by Neuroon mask. They grant direct access to low level mask control.
 */
#ifndef IC_LOW_LEVEL_CONTROL_H
#define IC_LOW_LEVEL_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <limits.h> // CHAR_BIT macro
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "ic_frame_handle.h"

#if CHAR_BIT != 8
#error "Char is not 8 bit!"
#endif

/** @defgroup LOW_LEVEL_NEUROON_MASK_API low level Neuroon mask conrol module
 *
 * @{
 */

/**
 * @brief Device Type
 *
 * Enumeration contains set of available devices.
 */
typedef enum __attribute__((packed)){
  DEV_RIGHT_RED_LED   = 0x01,
  DEV_RIGHT_GREEN_LED = 0x02,
  DEV_RIGHT_BLUE_LED  = 0x04,
  DEV_LEFT_RED_LED    = 0x08,
  DEV_LEFT_GREEN_LED  = 0x10,
  DEV_LEFT_BLUE_LED   = 0x20,
  DEV_VIBRATOR        = 0x40,
  DEV_POWER_LED       = 0x80
}e_deviceType;

#define DEV_SET(dev_array,dev)    ((dev_array) |= (dev))
#define DEV_RESET(dev_array,dev)  ((dev_array) &= ~(dev))
#define DEV_CHECK(dev_array,dev)  (((dev_array) & (dev)) ? true : false)

/** @} */

/** @defgroup RGB_LED_CONTROL low level RGB control submodule
 *  @ingroup LOW_LEVEL_NEUROON_MASK_API
 *  @{
 */

///LTC edge values
#define DEV_MIN_PERIOD    200
#define DEV_INF_DURATION  0
#define DEV_MAX_INTENSITY 63

/**
 * @brief Colors coding
 *
 * Enumeration contains colors coding
 */
typedef enum{
  RGB_LED_COLOR_RED  = 0x00,  /*!< Red rgb LED color */
  RGB_LED_COLOR_GREEN,        /*!< Green rgb LED color */
  RGB_LED_COLOR_BLUE,         /*!< Blue rgb LED color */
  RGB_LED_COLOR_WHITE,        /*!< White rgb LED color */
  RGB_LED_COLOR_TEAL,         /*!< Teal rgb LED color */
  RGB_LED_COLOR_CUSTOM,       /*!< Custom rgb LED color */
  RGB_LED_COLOR_NO_OF_COLORS  /*!< Reserved */
}e_rgbLedColor;

/**
 * @brief Active LED
 *
 * Enumeration is used to choose which LED will be controlled
 */
typedef enum{
  RGB_LED_SIDE_LEFT = 0x00, /*!< Select left rgb LED */
  RGB_LED_SIDE_RIGHT,       /*!< Select right rgb LED */
  RGB_LED_SIDE_BOTH         /*!< Select both rgb LEDs */
}e_rgbLedSide;

/**
 * @brief Low level control function
 *
 * Function lets client construct full device frame
 *
 * @param[out]    array     20 bytes array where frame will be stored. At least 20 bytes has to be
 * allocated.
 * @param[in,out] len       As input, provides data, of how big array has been allocated. As output
 * provides data of how much data has actually been used.
 * @param[in]     device    select influenced devices using @ref e_deviceType
 * @param[in]     func      choose how device will behave @ref e_funcType
 * @param[in]     intensity 7 elemetn array containing 0-63 values of device intensity.
 * @param[in]     duration  set duration of function in ms
 * @param[in]     period    set period in ms (for periodical function only @ref e_funcType)
 * @param[in]     id        Command counter for commands validate purpose.
 *
 * @return if returned @ref ERROR_UUID it means that either array pointer is NULL or array length is
 * not sufficient. In other cases returns index of UUID array (@ref nuc_init)
 *
 * Example:
 * @code
 *  char bleInputFrame[20];
 *  size_t bleInputFrameLength;
 *  uint8_t intensityArray[7] = {10, 20, 30, 10, 20, 30, 50};
 *  uint16_t externalID;
 *
 *  ...
 *
 *  if(device_set_func(bleInputFrame, &bleInputFrameLength, DEV_LEFT_RED_LED|DEV_VIBRATOR,
 *  FUN_TYPE_TRIANGLE, intensityArray, 1000, 500, externalID) == ERROR_UUID){
 *    errorHandle();
 *  }
 * @endcode
 */
int device_set_func(char *array, size_t *len, uint8_t device, e_funcType func, uint8_t *intensity,
    uint32_t duration, uint16_t period, uint16_t id);

/**
 * @brief Turn chosen LED ON
 *
 * LED will turn ON with full power
 *
 * @param[out]    array   20 bytes array where frame will be stored. At least 20 bytes has to be
 * allocated.
 * @param[in,out] len     As input, provides data, of how big array has been allocated. As output
 * provides data of how much data has actually been used.
 * @param[in]     rgb_led choose LED using @ref e_rgbLedSide
 * @param[in]     id      Command counter for commands validate purpose.
 *
 * @return if returned @ref ERROR_UUID it means that either array pointer is NULL or array length is
 * not sufficient. In other cases returns index of UUID array (@ref nuc_init)
 *
 * Example:
 * @code
 *  char bleInputFrame[20];
 *  size_t bleInputFrameLength;
 *
 *  ...
 *
 *  if(!rgb_led_ON(bleInputFrame, &bleInputFrameLength, RGB_LED_SIDE_BOTH){
 *    errorHandle();
 *  }
 * @endcode
 */
int rgb_led_ON(char *array, size_t *len, e_rgbLedSide rgb_led, uint16_t id);

/**
 * @brief Set LED color
 *
 * Color will be set with provided intensity
 *
 * @param[out]    array     20 bytes array where frame will be stored. At least 20 bytes has to be
 * allocated.
 * @param[in,out] len       As input, provides data, of how big array has been allocated. As output
 *                          provides data of how much data has actually been used.
 * @param[in]     rgb_led   choose LED using @ref e_rgbLedSide
 * @param[in]     color     choose LED color using @ref e_rgbLedColor
 * @param[in]     intensity 0-63 value of led intensity. Values bigger than 63
 * will be cut to 63.
 * @param[in]     id      Command counter for commands validate purpose.
 *
 * @return if returned @ref ERROR_UUID it means that either array pointer is NULL or array length is
 * not sufficient. In other cases returns index of UUID array (@ref nuc_init)
 */
int rgb_led_set_color(char *array, size_t *len, e_rgbLedSide rgb_led,
    e_rgbLedColor color, uint8_t intensity, uint16_t id);

/**
 * @brief Turn ON RGB LED with predefined function
 *
 * @param[out]    array     20 bytes array where frame will be stored. 20 bytes must
 * be allocated.
 * @param[in,out] len       As input, provides data, of how big array has been allocated. As output
 *                          provides data of how much data has actually been used.
 * @param[in]     rgb_led   choose LED using @ref e_rgbLedSide
 * @param[in]     func      choose how device will behave @ref e_funcType
 * @param[in]     color     choose led color @ref e_rgbLedColor
 * @param[in]     intensity 0-63 value of led intensity. Values bigger than 63 will be cut to 63.
 * @param[in]     duration  set duration of function in ms
 * @param[in]     period    set period in ms (for periodical function only @ref e_funcType)
 * @param[in]     id      Command counter for commands validate purpose.
 *
 * @return if returned @ref ERROR_UUID it means that either array pointer is NULL or array length is
 * not sufficient. In other cases returns index of UUID array (@ref nuc_init)
 */
int rgb_led_set_func(char *array, size_t *len, e_rgbLedSide rgb_led,
    e_funcType func, e_rgbLedColor color, uint8_t intensity, uint32_t duration,
    uint16_t period, uint16_t id);

/** @} */ //end of RGB_LED_CONTROL

/** @defgroup VIBRATOR_CONTROL low level vibrator control submodule
 *  @ingroup LOW_LEVEL_NEUROON_MASK_API
 *  @{
 */

/**
 * @brief Turn ON vibrator.
 *
 * @param[out]    array     20 bytes array where frame will be stored. 20 bytes must
 * be allocated.
 * @param[in,out] len       As input, provides data, of how big array has been allocated.
 * @param[in]     id      Command counter for commands validate purpose.
 *
 * @return if returned @ref ERROR_UUID it means that either array pointer is NULL or array length is
 * not sufficient. In other cases returns index of UUID array (@ref nuc_init)
 */
int vibrator_ON(char *array, size_t *len, uint16_t id);

/**
 * @brief Set vibrator intensity
 *
 * Vibrator will be turned ON with provided intensity
 *
 * @param[out]    array     20 bytes array where frame will be stored. At least 20 bytes has to be
 * allocated.
 * @param[in,out] len       As input, provides data, of how big array has been allocated.
 *                          provides data of how much data has actually been used.
 * @param[in]     intensity 0-255 value of vibrator intensity.
 * @param[in]     id      Command counter for commands validate purpose.
 *
 * @return if returned @ref ERROR_UUID it means that either array pointer is NULL or array length is
 * not sufficient. In other cases returns index of UUID array (@ref nuc_init)
 */
int vibrator_set_value(char *array, size_t *len, uint8_t intensity, uint16_t id);

/**
 * @brief Turn ON vibrator with predefined function
 *
 * @param[out]    array     20 bytes array where frame will be stored. 20 bytes must
 * be allocated.
 * @param[in,out] len       As input, provides data, of how big array has been allocated.
 *                          provides data of how much data has actually been used.
 * @param[in]     func      choose how device will behave @ref e_funcType
 * @param[in]     intensity 0-255 value of vibrator intensity.
 * @param[in]     duration  set duration of function in ms
 * @param[in]     period    set period in ms (for periodical function only @ref e_funcType)
 * @param[in]     id      Command counter for commands validate purpose.
 *
 * @return if returned @ref ERROR_UUID it means that either array pointer is NULL or array length is
 * not sufficient. In other cases returns index of UUID array (@ref nuc_init)
 */
int vibrator_set_func(char *array, size_t *len, e_funcType func, uint8_t intensity,
    uint32_t duration, uint16_t period, uint16_t id);

/** @} */ //end of VIBRATOR_CONTROL

/** @defgroup POWER_LED_CONTROL low level power LED control submodule
 *  @ingroup LOW_LEVEL_NEUROON_MASK_API
 *  @{
 */

/**
 * @brief Turn ON power led.
 *
 * @param[out]    array     20 bytes array where frame will be stored. 20 bytes must
 * be allocated.
 * @param[in,out] len       As input, provides data, of how big array has been allocated.
 * @param[in]     id      Command counter for commands validate purpose.
 *
 * @return if returned @ref ERROR_UUID it means that either array pointer is NULL or array length is
 * not sufficient. In other cases returns index of UUID array (@ref nuc_init)
 */
int pwr_led_ON(char *array, size_t *len, uint16_t id);

/**
 * @brief Set power led intensity
 *
 * pwr_led will be turned ON with provided intensity
 *
 * @param[out]    array     20 bytes array where frame will be stored. At least 20 bytes has to be
 * allocated.
 * @param[in,out] len       As input, provides data, of how big array has been allocated.
 *                          provides data of how much data has actually been used.
 * @param[in]     id      Command counter for commands validate purpose.
 *
 * @return if returned @ref ERROR_UUID it means that either array pointer is NULL or array length is
 * not sufficient. In other cases returns index of UUID array (@ref nuc_init)
 */
int pwr_led_set_value(char *array, size_t *len, uint16_t id);

/**
 * @brief Turn ON power led with predefined function
 *
 * @param[out]    array     20 bytes array where frame will be stored. 20 bytes must
 * be allocated.
 * @param[in,out] len       As input, provides data, of how big array has been allocated.
 *                          provides data of how much data has actually been used.
 * @param[in]     func      choose how device will behave @ref e_funcType
 * @param[in]     duration  set duration of function in ms
 * @param[in]     period    set period in ms (for periodical function only @ref e_funcType)
 * @param[in]     id      Command counter for commands validate purpose.
 *
 * @return if returned @ref ERROR_UUID it means that either array pointer is NULL or array length is
 * not sufficient. In other cases returns index of UUID array (@ref nuc_init)
 */
int pwr_led_set_func(char *array, size_t *len, e_funcType func,
    uint32_t duration, uint16_t period, uint16_t id);

/** @} */ //end of POWER_LED_CONTROL

/** @defgroup PULSE_OXIMETER_CONTROL low level pulse-oximeter control submodule
 *  @ingroup LOW_LEVEL_NEUROON_MASK_API
 *  @{
 */

/**
 * @brief Build command frame, which configures CS for AFE4400 - NRF51 communication and AFE4400
 * control registers (for normal work - turn on SPI, powerdown OFF, autodiagnostics, etc.)
 *
 * @param[out]    array     20 bytes array where frame will be stored. 20 bytes must
 * be allocated.
 * @param[in,out] len       as input, provides data, of how big array has been allocated.
 *                          provides data of how much data has actually been used.
 * @param[in]     id        command counter for commands validate purpose.
 *
 * @return if returned @ref ERROR_UUID it means that either array pointer is NULL or array length is
 * not sufficient. In other cases returns index of UUID array (@ref nuc_init)
 */
int pox_hdw_init(char *array, size_t *len, uint16_t id);

/**
 * @brief Build command frame, which configures all of AFE4400 registers (without control registers)
 * with Neuroon standard values (details in @ref ic_afe4400.h)
 *
 * @param[out]    array     20 bytes array where frame will be stored. 20 bytes must
 * be allocated.
 * @param[in,out] len       as input, provides data, of how big array has been allocated.
 *                          provides data of how much data has actually been used.
 * @param[in]     id        command counter for commands validate purpose.
 *
 * @return if returned @ref ERROR_UUID it means that either array pointer is NULL or array length is
 * not sufficient. In other cases returns index of UUID array (@ref nuc_init)
 */
int pox_std_val_init(char *array, size_t *len, uint16_t id);

/**
 * @brief Build command frame, which turns ON powerdown in AFE4400 (details in AFE4400 documentation)
 *
 * @param[out]    array     20 bytes array where frame will be stored. 20 bytes must
 * be allocated.
 * @param[in,out] len       as input, provides data, of how big array has been allocated.
 *                          provides data of how much data has actually been used.
 * @param[in]     id        command counter for commands validate purpose.
 *
 * @return if returned @ref ERROR_UUID it means that either array pointer is NULL or array length is
 * not sufficient. In other cases returns index of UUID array (@ref nuc_init)
 */
int pox_powerdown_on(char *array, size_t *len, uint16_t id);

/**
 * @brief Build command frame, which turns OFF powerdown in AFE4400 (details in AFE4400 documentation)
 *
 * @param[out]    array     20 bytes array where frame will be stored. 20 bytes must
 * be allocated.
 * @param[in,out] len       as input, provides data, of how big array has been allocated.
 *                          provides data of how much data has actually been used.
 * @param[in]     id        command counter for commands validate purpose.
 *
 * @return if returned @ref ERROR_UUID it means that either array pointer is NULL or array length is
 * not sufficient. In other cases returns index of UUID array (@ref nuc_init)
 */
int pox_powerdown_off(char *array, size_t *len, uint16_t id);

/**
 * @brief Build command frame, which is a read from selected AFE4400 register request. Register
 * configuration is written in pox response frame.
 *
 * @param[out]    array     20 bytes array where frame will be stored. 20 bytes must
 * be allocated.
 * @param[in,out] len       as input, provides data, of how big array has been allocated.
 *                          provides data of how much data has actually been used.
 * @param[in]     reg       afe4400 register number/name (look into afe4400 documentation or @ref
 * ic_afe4400.h).
 * @param[in]     id        command counter for commands validate purpose.
 *
 * @return if returned @ref ERROR_UUID it means that either array pointer is NULL or array length is
 * not sufficient. In other cases returns index of UUID array (@ref nuc_init)
 */
int pox_read_register(char *array, size_t *len, t_afe4400Register reg, uint16_t id);

/**
 * @brief Build command frame, which is a write to selected AFE4400 register request.
 *
 * @param[out]    array     20 bytes array where frame will be stored. 20 bytes must
 * be allocated.
 * @param[in,out] len       As input, provides data, of how big array has been allocated.
 *                          provides data of how much data has actually been used.
 * @param[in]     reg       AFE4400 register number/name (look into AFE4400 documentation or @ref
 * ic_afe4400.h).
 * @param[in]     reg_val   AFE4400 register configuration (look into AFE4400 documentation or
 * @ref ic_afe4400.h).
 * @param[in]     id        Command counter for commands validate purpose.
 *
 * @return if returned @ref ERROR_UUID it means that either array pointer is NULL or array length is
 * not sufficient. In other cases returns index of UUID array (@ref nuc_init)
 */
int pox_write_register(char *array, size_t *len, t_afe4400Register reg, t_afe4400RegisterConf reg_val, uint16_t id);

/**
 * @brief Build command frame, which turns ON AFE4400 autodiagnostics (details in AFE4400 documentation). Result of autodiagnostics is written into DIAG register.
 *
 * @param[out]    array     20 bytes array where frame will be stored. 20 bytes must
 * be allocated.
 * @param[in,out] len       as input, provides data, of how big array has been allocated.
 *                          provides data of how much data has actually been used.
 * @param[in]     id        command counter for commands validate purpose.
 *
 * @return if returned @ref ERROR_UUID it means that either array pointer is NULL or array length is
 * not sufficient. In other cases returns index of UUID array (@ref nuc_init)
 */
int pox_self_test(char *array, size_t *len, uint16_t id);

/** @} */ //end of PULSE_OXIMETER_CONTROL

/** @defgroup EMERGENCY_ALARM_CONTROL low level emergency alarm control submodule
 *  @ingroup LOW_LEVEL_NEUROON_MASK_API
 *  @{
 */

/**
 * @brief Build command frame, which resets parameters of emergency alarm module.
 *
 * @param[out]    array     20 bytes array where frame will be stored. 20 bytes must
 * be allocated.
 * @param[in,out] len       as input, provides data, of how big array has been allocated.
 *                          provides data of how much data has actually been used.
 * @param[in]     id        command counter for commands validate purpose.
 *
 * @return if returned @ref false it means that either array pointer is
 * null or array length is nof sufficient
 */
/*bool alarm_init(char *array, size_t *len, uint16_t id);*/

/**
 * @brief Build command frame, which turns ON emergency alarm and configures its parameters. Alarm
 * will start after "time" seconds if there will be no BLE connection with central for "timeout" seconds.
 *
 * @param[out]    array     20 bytes array where frame will be stored. 20 bytes must
 * be allocated.
 * @param[in,out] len       as input, provides data, of how big array has been allocated.
 *                          provides data of how much data has actually been used.
 * @param[in]     type      alarm mode @ref e_alarmType
 * @param[in]     time      time (in seconds) to emergency alarm start
 * @param[in]     timeout   if there will be no BLE connection with central for "timeout" seconds,
 *                          an emergency alarm will occur at set time
 * @param[in]     id        command counter for commands validate purpose.
 *
 * @return if returned @ref ERROR_UUID it means that either array pointer is NULL or array length is
 * not sufficient. In other cases returns index of UUID array (@ref nuc_init)
 */
int alarm_set(char *array, size_t *len, e_alarmType type, uint32_t time, uint16_t timeout, uint16_t id);

/**
 * @brief Build command frame, which turns OFF emergency alarm module (emergency alarm will not occur at the time of awakening).
 *
 * @param[out]    array     20 bytes array where frame will be stored. 20 bytes must
 * be allocated.
 * @param[in,out] len       as input, provides data, of how big array has been allocated.
 *                          provides data of how much data has actually been used.
 * @param[in]     id        command counter for commands validate purpose.
 *
 * @return if returned @ref ERROR_UUID it means that either array pointer is NULL or array length is
 * not sufficient. In other cases returns index of UUID array (@ref nuc_init)
 */
int alarm_off(char *array, size_t *len, uint16_t id);
/** @} */ //end of EMERGENCY_ALARM_CONTROL

/** @defgroup RESPONSE_CONTROL low level response control submodule
 *  @ingroup LOW_LEVEL_NEUROON_MASK_API
 *  @{
 */

/**
 * @brief Build complete response frame for LTC devices.
 *
 * @param[out]    array     20 bytes array where frame will be stored. 20 bytes must
 * be allocated.
 * @param[in,out] len       As input, provides data, of how big array has been allocated.
 *                          provides data of how much data has actually been used.
 * @param[in]     device    list of devices which should be activate by last command.
 * Devices listed by put "1" in proper bit field.
 * @param[in]     func      choose how device will behave @ref e_funcType
 * @param[in]     duration  set duration of function in ms
 * @param[in]     period    set period in ms (for periodical function only @ref e_funcType)
 * @param[in]     state_code  true if command was accepted
 * @param[in]     id        Command counter for commands validate purpose. Copied from
 * DEVICE_CMD frame.
 *
 * @return if returned @ref ERROR_UUID it means that either array pointer is NULL or array length is
 * not sufficient. In other cases returns index of UUID array (@ref nuc_init)
 */
int dev_resp_frame_gen_func(char *array, size_t *len, uint8_t device, e_funcType func, uint32_t duration, uint16_t period, bool state_code, uint16_t id);

/**
 * @brief Compare received response vs sent frame;
 *
 * @param[in] sent_frame  20 bytes array with sent cmd frame
 * @param[in] rsp_frame   20 bytes array with received response frame
 *
 * @return  if returned @ref false it means that either frames are not compatible or there is a CRC
 *          error.
 */
bool frame_resp_cmp(char *sent_frame, char *rsp_frame);

/**
 * @brief Build complete response frame by payload copy - device and pulse-oximeter.
 *
 * @param[out]    array     20 bytes array where frame will be stored. 20 bytes must
 * be allocated.
 * @param[in,out] len       As input, provides data, of how big array has been allocated.
 *                          provides data of how much data has actually been used.
 * @param[in]     payload   Payload to copy @ref u_BLECmdPayload and @ref u_cmdFrameContainer
 *
 * @return if returned @ref false it means that either array pointer is
 * NULL or array length is nof sufficient
 */
bool resp_frame_copy_func(char *array, size_t *len, char *payload, e_cmd cmd_type);
/** @} */ //end of RESPONSE_CONTROL

/** @defgroup STATUS_CONTROL low level status frame control submodule
 *  @ingroup LOW_LEVEL_NEUROON_MASK_API
 *  @{
 */

/**
 * @brief Build complete status command frame.
 *
 * @param[out]    array     20 bytes array where frame will be stored. 20 bytes must
 * be allocated.
 * @param[in,out] len       As input, provides data, of how big array has been allocated.
 *                          provides data of how much data has actually been used.
 * @param[in]     id        Command counter for commands validate purpose.
 *
 * @return if returned @ref false it means that either array pointer is
 * NULL or array length is nof sufficient
 */
bool status_cmd_gen_func(char *array, size_t *len, uint16_t id);

/**
 * @brief Build complete status response frame.
 *
 * @param[out]    array     20 bytes array where frame will be stored. 20 bytes must
 * be allocated.
 * @param[in,out] len       As input, provides data, of how big array has been allocated.
 *                          provides data of how much data has actually been used.
 * @param[in]     devs_func structure with information about current devices behaviour @ref
 * s_devsFunc
 * @param[in]     active_data_streams five LSBs specify which of the data streams are currently
 * active (from MSB: EEG,
 * IR poxy, RED poxy, ACC, TEMP)
 * @param[in]     id        Command counter for commands validate purpose. Copied from
 * STATUS_CMD frame.
 *
 * @return if returned @ref false it means that either array pointer is
 * NULL or array length is nof sufficient
 */
bool status_rsp_gen_func(char *array, size_t *len, s_devsFunc devs_func, uint8_t active_data_streams, uint16_t id);
/** @} */ //end of STATUS_CONTROL
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* !IC_UC_FRAME_CONSTRUCTOR_H */
