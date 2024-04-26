//
// Created by ShunX on 2024/2/11.
//

#include "fsmain.h"
#include "ff.h"
#include <stdio.h>
#include "exfat.h"

FATFS fs0;
BYTE fbuf[512];

void fs_init(void) {
  FRESULT f_res = f_mount(&fs0, "0:", 1);
  printf("f_mount res :%d\r\n", f_res);
  if (f_res == FR_NO_FILESYSTEM) {
    f_res = f_mkfs("0:", 0, fbuf, sizeof(fbuf));
    printf("f_mks res: %d\r\n", f_res);
  }

  /*uint32_t fre,ftol;
  exfuns_get_free((uint8_t*)"0:", &ftol, &fre);
  printf("spi disk total:%ld KB free:%ld KB\r\n", ftol, fre);

  static DIR dir;
  static FILINFO fif0;
  f_opendir(&dir, "0:");

  f_res = f_readdir(&dir, &fif0);
  printf("fn:%s \tfsz:%lu \tatr:%x \tres:%d\r\n", fif0.fname, fif0.fsize, fif0.fattrib,f_res);
  f_res = f_readdir(&dir, &fif0);
  printf("fn:%s \tfsz:%lu \tatr:%x \tres:%d\r\n", fif0.fname, fif0.fsize, fif0.fattrib,f_res);
  f_res = f_readdir(&dir, &fif0);
  printf("fn:%s \tfsz:%lu \tatr:%x \tres:%d\r\n", fif0.fname, fif0.fsize, fif0.fattrib,f_res);
  f_res = f_readdir(&dir, &fif0);
  printf("fn:%s \tfsz:%lu \tatr:%x \tres:%d\r\n", fif0.fname, fif0.fsize, fif0.fattrib,f_res);
  f_res = f_readdir(&dir, &fif0);
  printf("fn:%s \tfsz:%lu \tatr:%x \tres:%d\r\n", fif0.fname, fif0.fsize, fif0.fattrib,f_res);
  f_res = f_readdir(&dir, &fif0);
  printf("fn:%s \tfsz:%lu \tatr:%x \tres:%d\r\n", fif0.fname, fif0.fsize, fif0.fattrib,f_res);

  f_closedir(&dir);*/
}