/**
 * @file    ic_uc_crc8.h
 * @author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @author  Wojtek Weclewski <w.weclewski@inteliclinic.com>
 * @date    September, 2016
 * @brief   Brief description
 *
 * Description
 */

#ifndef IC_UC_CRC8_H
#define IC_UC_CRC8_H

#include <stdint.h>

uint8_t crc8_calculate (const uint8_t *data, int len);

#endif /* !IC_UC_CRC8_H */
