//
// Created by Zhou 2024/1/7
//

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WQX_H
#define __WQX_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include "spi.h"
/* USER CODE END Includes */

/* Exported macro ------------------------------------------------------------*/

/** SPI Interface **/
#define WQX_SPI_TRANS_MAX 	((uint32_t)1024 * 60)
#define W25Q128_SPI_TIMEOUT 0xffffff
#define WQX_SPI_INTERFACE 	&hspi3
#define WQX_CS_PIN 					WQX_CS2_Pin
#define WQX_CS_PIN_Port 		WQX_CS2_GPIO_Port
#define NOR_FLASH_CS_LOW() 	(WQX_CS_PIN_Port->BSRR |= (WQX_CS_PIN << 16))
#define NOR_FLASH_CS_HIGH() (WQX_CS_PIN_Port->BSRR |= WQX_CS_PIN)

/** ======================= USER CODE BEGIN Private defines ========================= **/
/**
 * @brief 芯片说明
 * >>>>W25Q64有8M字节，分为0-127个Blcok块（64KB），每个块有有16个扇区sector（4KB）                                                                               ，
 *						每个扇区有16个页Page（256 BYTE）：以sector为最小擦除单位
 *
 *	feacture	\写入数据之前必须先擦除
 *				    \擦除会把数据全部置1
 *				    \写入数据会把1改为0
 *				    \擦除时必须按最小单位（扇区）擦除
 ** ================================================================================== **/

/** W25Qxx SIZE **/
extern uint16_t WQX_SIZE;
#define WQX_SecSize 4096
#define WQX_PageSize 256
#define SPI_FLASH_PerWritePageSize 256

/** FLASH ID  **/
#define sFLASH_ID_Q16 	0xEF3015  // W25X16
#define sFLASH_ID_Q64 	0XEF4017  // W25Q64
#define sFLASH_ID_Q128 	0XEF4018 	// W25Q128
#define DevFlashID_Q16 	0x15     	// W25Q16
#define DevFlashID_Q64 	0x16     	// W25Q64
#define DevFlashID_Q128 0x17    	// W25Q128
#define DevFlashID_Q256 0x18    	// W25Q256

#define WQX_USE_FATFS 1 /** >>! 使用文件系统 **/
#define WQX_TEST 0      /** 测试是否读写正常 **/
#define USE_4KRW_MODE 0

/** 定义W25QXX与文件系统相关的 **/
#if WQX_USE_FATFS
#define WQX_SECTOR_SIZE 4096U                                              // W25QXX 操作扇区
#define WQX_OFFSET_SECTOR ((uint32_t)(4 * 1024 * 1024 / WQX_SECTOR_SIZE))   // 向后偏移4M字节 = 4*1024*1024/512=4096*2
#define WQX_SECTOR_OF_BLOCK 1                                        // 每个BLOCK个sector
#define WQX_CAPCITY_SECTOR ((uint32_t)(16 * 1024 * 1024 / WQX_SECTOR_SIZE)) // 向后偏移4M空间

#endif

/** 命令定义-开头 **/
#define FLASH_WriteEnable 		0x06
#define FLASH_WriteDisable 		0x04
#define FLASH_ReadStatusReg1 	0x05
#define FLASH_ReadStatusReg2 	0x35
#define FLASH_ReadStatusReg3 	0x15
#define FLASH_WriteStatusReg1 0x01
#define FLASH_WriteStatusReg2 0x31
#define FLASH_WriteStatusReg3 0x11
#define FLASH_ReadData 				0x03
#define FLASH_FastReadData 		0x0B
#define FLASH_FastReadDual 		0x3B
#define FLASH_FastReadQuad 		0xEB
#define FLASH_PageProgram 		0x02
#define FLASH_PageProgramQuad 0x32
#define FLASH_BlockErase 			0xD8
#define FLASH_SectorErase 		0x20
#define FLASH_ChipErase 			0xC7
#define FLASH_PowerDown 			0xB9
#define FLASH_ReleasePowerDown 0xAB
#define FLASH_DeviceID 				0xAB
#define FLASH_ManufactDeviceID 0x90
#define FLASH_JedecDeviceID 	0x9F
#define FLASH_Enable4ByteAddr 0xB7
#define FLASH_Exit4ByteAddr 	0xE9
#define FLASH_SetReadParam 		0xC0
#define FLASH_EnterQPIMode 		0x38
#define FLASH_ExitQPIMode 		0xFF
#define Dummy_Byte 						0xFF

/** function **/
void WQX_Init(void);

void WQX_WAKEUP(void);

void WQX_WaitBusy(void);

void WQX_PowerDown(void);

int8_t Get_WQX_State(void);

void WQX_ChipErase(void);

uint32_t WQX_ReadID(void);

uint32_t WQX_ReadDeviceID(void);

void WQX_Disk_Write(uint8_t *pBuff, uint32_t WriteAddr, uint32_t Numofbyte);

void WQX_Disk_Read(uint8_t *pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);

void WQX_4K_Sector_Write(uint8_t *pBuff, uint16_t Sector_addr, uint16_t Sector_Nbr);

void WQX_4K_Sector_Read(uint8_t *pBuff, uint16_t Sector_addr, uint16_t Sector_Nbr);

void WQX_test(void);

#ifdef __cplusplus
}
#endif

#endif /* __WQX_H */
