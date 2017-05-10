/**
 * @file    ic_version.c
 * @author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @author  Wojtek Weclewski <w.weclewski@inteliclinic.com>
 * @date    March, 2017
 * @brief   Brief description
 *
 * Description
 */

#include <stdlib.h>
#include "version.h"
#include "ic_version.h"
#include "string.h"

#ifndef GIT_VERSION
#define GIT_VERSION "NULL"
#endif

static const char *version_string = GIT_VERSION;

int nuc_get_version(){
  int version = 0x00;
  const char *_ptr = version_string;
  for (int i=0; i<4; ++i){
    version |= (atoi(_ptr)<<((3-i)*8)) & 0xFF<<((3-i)*8);
    _ptr = strpbrk(_ptr, ".")+1;
  }
  return version;
}

int nuc_get_version_element(e_nucSubversion subversion){
  int version = nuc_get_version();
  return (version>>(3-subversion)*8)&0xFF;
}

int nuc_get_version_release(){
  return nuc_get_version_element(NUC_SUBVERSION_RLEASE);
}

int nuc_get_version_major(){
  return nuc_get_version_element(NUC_SUBVERSION_MAJOR);
}

int nuc_get_version_minor(){
  return nuc_get_version_element(NUC_SUBVERSION_MINOR);
}

int nuc_get_version_patch(){
  return nuc_get_version_element(NUC_SUBVERSION_PATCH);
}

bool nuc_get_version_string(char *version, size_t len){
  if(len<strlen(version)+1) return false;
  memcpy(version, version_string, strlen(version_string)+1);
  return true;
}
