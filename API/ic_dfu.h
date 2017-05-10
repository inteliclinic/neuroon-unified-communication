/**
 * @file    ic_dfu.h
 * @author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @author  Wojtek Weclewski <w.weclewski@inteliclinic.com>
 * @date    October, 2016
 * @brief   Neuroon DFU API interface
 *
 * Functions provided with this file populates memory with data understandable
 * by Neuroon mask in DFU mode and make mask enter DFU mode.
 */

#ifndef IC_DFU_H
#define IC_DFU_H

#ifdef __cplusplus
extern "C" {
#endif

#include <limits.h> // CHAR_BIT macro
#include <stddef.h> // size_t type
#include <stdint.h>
#include <stdio.h>  // FILE type

#if CHAR_BIT != 8
#error "Char is not 8 bit!"
#endif

/** @defgroup DFU_API
 *
 * @{
 */


/**
 * @brief Firmaware type
 *
 * */
typedef enum{
  SD_FIRMWARE = 0x00,   /*!< Nordic Soft Device */
  APP_FIRMWARE = 0x01,  /*!< Neuroon firmware */
  DFU_FIRMWARE = 0x02   /*!< DFU */
}e_firmwareType;

/**
 * @brief Firmaware major release
 *
 * */
typedef enum{
  LEGACY_NEUROON_FIRMWARE = 0x00, /*!< firmware 2.0.x.x */
  NEW_NEUROON_FIRMWARE = 0x01     /*!< firmware 2.1.x.x */
}e_firmwareMilestone;

/**
 * @brief Next step order
 *
 * Encoded orders for application to execute
 */
typedef enum{
  DFU_SEND_NEXT_DATASET = 0x00, /*!< send data stored in output frame */
  DFU_RESEND_DATASET,           /*!< resend previous data set */
  DFU_TERMINATE,                /*!< Mask encoutered critical error in DFU mode */
  DFU_END                       /*!< Update finished */
}e_dfuAction;

/**
 * @brief Generate "go to dfu" command
 *
 * @param[out]    frame     pointer to 20 bytes array where frame will be stored
 * @param[in,out] len       pointer to size_t value where function will put length of array
 * @param[in]     firmware  choose firmware type
 *
 * @return characteristic index
 *
 */
int goto_dfu(char *frame, size_t *len, e_firmwareMilestone firmware);

/**
 * @brief Generate start update command for dfu
 *
 * @param[out]    frame     pointer to 20 bytes array where frame will be stored
 * @param[in,out] len       pointer to size_t value where function will put lenght of array
 * @param[in]     fb        pointer to memory with storred binary file
 * @param[in]     file_len  length of fb buffer
 * @param[in]     firm      @ref e_firmwareType binary file type
 * @param[in]     version   binary version build from 4 bytes. Ex 16777985 = 1.0.3.1
 *
 * @return characteristic index
 */
int dfu_start_update(char *frame, size_t *len, char *fb, size_t file_len, e_firmwareType firm,
    uint32_t version);

/**
 * @brief Generate start update command for dfu
 *
 * @param[out]    frame     pointer to 20 bytes array where frame will be stored
 * @param[in,out] len       pointer to size_t value where function will put lenght of array
 * @param[in]     fp        pointer to file handle(@ref FILE)
 * @param[in]     firm      @ref e_firmwareType binary file type
 * @param[in]     version   binary version build from 4 bytes. Ex 16777985 = 1.0.3.1
 *
 * @return characteristic index
 */
int dfu_start_update_fp(char *frame, size_t *len, FILE *fp, e_firmwareType firm, uint32_t version);

/**
 * @brief Receive data from dfu response characteristic
 *
 * @param[in]     response_frame  20 byte array with response frame
 * @param[in]     response_len    response array length
 * @param[out]    frame           pointer to 20 bytes array where binary data will be stored
 * @param[in,out] len             pointer to size_t value where function will put output frame
 *                                length
 * @param[out]    action          next step for update
 *
 * @return characteristic index
 */
int dfu_response_sink(char *response_frame, size_t response_len, char* frame, size_t *len,
    e_dfuAction *action);


/** @} */ //End of DFU_API

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* !IC_DFU_H */
