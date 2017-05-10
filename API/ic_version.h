/**
 * @file    ic_version.h
 * @author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @author  Wojtek Weclewski <w.weclewski@inteliclinic.com>
 * @date    March, 2017
 * @brief   Brief description
 *
 * Description
 */

#ifndef IC_VERSION_H
#define IC_VERSION_H

#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Subversion number
 */
typedef enum {
  NUC_SUBVERSION_RLEASE = 0x00,
  NUC_SUBVERSION_MAJOR,
  NUC_SUBVERSION_MINOR,
  NUC_SUBVERSION_PATCH
}e_nucSubversion;

/**
 * @brief Get nuc version
 *
 * Function returns version coded into 4 byte integer.
 *
 * x.x.x.x
 * | | | |
 * | | | -----------------
 * | | ------------       |
 * | -------       |      |
 * |        |      |      |
 * release  major  minor  patch
 *
 * MSB represents release number, LSB a patch number
 *
 * @return version coded into 4 byte integer
 */
int nuc_get_version();

/**
 * @brief Get specified subversion
 *
 * @param[in]  subversion  choose subversion @ref
 *
 * @return 1 byte with specified subversion
 *
 */
int nuc_get_version_element(e_nucSubversion subversion);

/**
 * @brief Get release subversion
 *
 * @return 1 byte with specified subversion
 *
 */
int nuc_get_version_release();

/**
 * @brief Get major subversion
 *
 * @return 1 byte with specified subversion
 *
 */
int nuc_get_version_major();

/**
 * @brief Get minor subversion
 *
 * @return 1 byte with specified subversion
 *
 */
int nuc_get_version_minor();

/**
 * @brief Get patch subversion
 *
 * @return 1 byte with specified subversion
 *
 */
int nuc_get_version_patch();

/**
 * @brief Get version formated in cstring
 *
 * @param[out]  version   pointer to allocated char buffer. 32 bytes is recomended(max string
 *                        size)
 * @param[in]   len       buffer size
 *
 * @return false if buffer is to small
 *
 */
bool nuc_get_version_string(char *version, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* IC_VERSION_H */
