/**
 * @file    test.c
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    January, 2017
 * @brief   Brief description
 *
 * Description
 */

#include <stdio.h>
#include <stdlib.h>
#include "ic_dfu.h"
#include "ic_frame_handle.h"
#include "ic_low_level_control.h"
#include "ic_version.h"

#define ARRAY_SIZE 20
#define PRINT_ARRAY(a,l)do{\
  for (unsigned int i=0; i<l; ++i){\
    printf("0x%02x ", a[i]&0xFF);}\
  printf("\n\r");\
}while(0);

int main(void){
  char array[ARRAY_SIZE];
  size_t len = sizeof(array);

  if(vibrator_ON(array, &len, 1)){
    PRINT_ARRAY(array, len);
  }
  else
    return -1;
  if(goto_dfu(array, &len, NEW_NEUROON_FIRMWARE)){
    PRINT_ARRAY(array, len);
  }
  else
    return -1;

  char chars[9][37];
  nuc_init(chars);

  for (size_t i=0; i<(sizeof(chars)/sizeof(chars[0])); ++i){
    printf("%s\n", chars[i]);
  }

  char cstring[20];
  nuc_get_version_string(cstring, sizeof(cstring));
  printf("%s\n",cstring);
  printf("%d\n",nuc_get_version());

  printf("release:\t%d\n",nuc_get_version_release());
  printf("major:\t\t%d\n",nuc_get_version_major());
  printf("minor:\t\t%d\n",nuc_get_version_minor());
  printf("patch:\t\t%d\n",nuc_get_version_patch());


  return 0l;
}
