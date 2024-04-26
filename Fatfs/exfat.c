//
// Created by ShunX on 2024/2/9.
//

#include "exfat.h"
#include "ff.h"

/**
 * @brief       获取磁盘剩余容量
 * @param       pdrv : 磁盘编号("0:"~"9:")
 * @param       total: 总容量 (KB)
 * @param       free : 剩余容量 (KB)
 * @retval      0, 正常; 其他, 错误代码
 */
uint8_t exfuns_get_free(uint8_t *pdrv, uint32_t *total, uint32_t *free)
{
  FATFS *fs1;
  uint8_t res;
  uint32_t fre_clust = 0, fre_sect = 0, tot_sect = 0;

  /* 得到磁盘信息及空闲簇数量 */
  res = (uint32_t)f_getfree((const TCHAR *)pdrv, (DWORD *)&fre_clust, &fs1);

  if (res == 0)
  {
    tot_sect = (fs1->n_fatent - 2) * fs1->csize;    /* 得到总扇区数 */
    fre_sect = fre_clust * fs1->csize;              /* 得到空闲扇区数 */
#if FF_MAX_SS!=512  /* 扇区大小不是512字节,则转换为512字节 */
    tot_sect *= fs1->ssize / 512;
    fre_sect *= fs1->ssize / 512;
#endif
    *total = tot_sect >> 1;     /* 单位为KB */
    *free = fre_sect >> 1;      /* 单位为KB */
  }

  return res;
}


