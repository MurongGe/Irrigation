/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "../BSP/w25qxx/w25qxx.h"

/* Definitions of physical drive number for each drive */
//#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
//#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
//#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */
#define   DEV_SPI_FLASH 0

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
  DRESULT res;
  if(pdrv == DEV_SPI_FLASH)
  {
    res = RES_OK;
  } else {
    res = RES_ERROR;
  }
  return res;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
  DRESULT res;
  if(pdrv == DEV_SPI_FLASH)
  {
    res = RES_OK;
  } else {
    res = RES_ERROR;
  }
  return res;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
  DRESULT res;
  if(pdrv == DEV_SPI_FLASH)
  {
    WQX_Disk_Read((uint8_t*)buff, sector * WQX_SECTOR_SIZE, count * WQX_SECTOR_SIZE);
    res = RES_OK;
  } else {
    res = RES_ERROR;
  }
  return res;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	if(pdrv == DEV_SPI_FLASH)
  {
    WQX_Disk_Write((uint8_t*)buff, sector * WQX_SECTOR_SIZE, count * WQX_SECTOR_SIZE);
    res = RES_OK;
  } else {
    res = RES_ERROR;
  }
  return res;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
  DRESULT res = 0;
  if(pdrv == DEV_SPI_FLASH)
  {
    switch (cmd) {
      case CTRL_SYNC:
        res = RES_OK;
        break;

      case GET_SECTOR_SIZE:
        *(WORD *)buff = WQX_SECTOR_SIZE;
        res = RES_OK;
        break;

      case GET_BLOCK_SIZE:
        *(WORD *)buff = WQX_SECTOR_OF_BLOCK;
        res = RES_OK;
        break;

      case GET_SECTOR_COUNT:
        *(DWORD *)buff = WQX_CAPCITY_SECTOR;
        res = RES_OK;
        break;

      default:
        res = RES_PARERR;
        break;
    }
  } else {
    res = RES_ERROR;
  }
  return res;
}

DWORD get_fattime (void)
{
  return ((2024-1980)<<25)|((2)<<21)|((9)<<16)|((11)<<11)|((46)<<5)|((1)<<0);
}