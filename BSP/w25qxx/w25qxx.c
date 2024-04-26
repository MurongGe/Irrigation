//
// Created by Zhou 2024/1/7
//

/** Includes ------------------------------------------------------------------*/
#include "w25qxx.h"
#include "usart.h"

uint16_t WQX_SIZE = 2 * 1024; /** >>! w25qxx capicity kbytes **/
uint16_t DevID = 0xff;
static __ALIGNED(4) uint8_t W25QXXcopyBuff[WQX_SecSize];

static uint8_t WQX_RW_BYTE(uint8_t byte);

static int8_t WQX_SendBytes(uint8_t *pBuff, uint32_t NumOfBytes);

static int8_t WQX_ReceiveBytes(uint8_t *pBuff, uint32_t NumOfBytes);

void WQX_Send_Addr(uint32_t addr);

uint8_t WQX_Read_SR(uint8_t SR_th);

void WQX_Write_SR(uint8_t SR_th, uint8_t SR_Val);

static void WQX_Write_NoCheck(uint8_t *pBuff, uint32_t WriteAddr, uint32_t NumofByte);

static void WQX_WriteEnable(void);

static void WQX_PageWrite(uint8_t *pBuff, uint32_t WriteAddr, uint16_t NumByteToWrite);

static void WQX_SectorErase(uint32_t SectorAddr);

/********初始化驱动接口***********/
void WQX_Init(void)
{
  uint8_t temp;
  WQX_RW_BYTE(0xff);

  NOR_FLASH_CS_HIGH(); // 必须从高到低的电平跳变

  DevID = WQX_ReadDeviceID();

  if (DevID == DevFlashID_Q16)
  {
    printf("NOR Flash is W25Q16: 0x%x\r\n", DevID);
    WQX_SIZE = 2 * 1024;
  }
  else if (DevID == DevFlashID_Q64)
  {
    printf("NOR Flash is W25Q64: 0x%x\r\n", DevID);
    WQX_SIZE = 8 * 1024;
  }
  else if (DevID == DevFlashID_Q128)
  {
    printf("NOR Flash is W25Q128: 0x%x\r\n", DevID);
    WQX_SIZE = 16 * 1024;
  }
  else if (DevID == DevFlashID_Q256)
  {
    printf("NOR Flash is W25Q256: 0x%x\r\n", DevID);
    WQX_SIZE = 32 * 1024;
  }
  else
    printf("No SPI Flash ID: 0x%x\r\n", DevID);

  if (DevID == DevFlashID_Q256) /* SPI FLASH为W25Q256, 必须使能4字节地址模式 */
  {
    temp = WQX_Read_SR(3); /* 读取状态寄存器3，判断地址模式 */

    if ((temp & 0X01) == 0) /* 如果不是4字节地址模式,则进入4字节地址模式 */
    {
      WQX_WriteEnable();     /* 写使能 */
      temp |= 1 << 1;        /* ADP=1, 上电4位地址模式 */
      WQX_Write_SR(3, temp); /* 写SR3 */

      NOR_FLASH_CS_LOW();
      WQX_RW_BYTE(FLASH_Enable4ByteAddr); /* 使能4字节地址指令 */
      NOR_FLASH_CS_HIGH();
    }
  }
}

/**
 * @brief  使用SPI发送一个字节的数据
 * @param  byte：要发送的数据
 * @retval 返回接收到的数据
 */
static uint8_t WQX_RW_BYTE(uint8_t byte)
{
  uint8_t Rdata = 0;
  if (HAL_SPI_TransmitReceive(WQX_SPI_INTERFACE, &byte, &Rdata, 1, W25Q128_SPI_TIMEOUT) != HAL_OK)
  {
    printf("spi error: WQX_RW_BYTE\r\n");
    return 0;
  }
  return Rdata;
}

/**
 * @brief  使用SPI发送 多个字节的数据
 * @param  byte：要发送的数据
 * @retval 返回接收到的数据
 */
static int8_t WQX_SendBytes(uint8_t *pBuff, uint32_t NumOfBytes)
{
  uint32_t i;
  uint32_t times = NumOfBytes / WQX_SPI_TRANS_MAX;

  NumOfBytes %= WQX_SPI_TRANS_MAX;

  for (i = 0; i < times; i++)
  {
    if (HAL_SPI_Transmit(WQX_SPI_INTERFACE, pBuff + i * WQX_SPI_TRANS_MAX, WQX_SPI_TRANS_MAX, W25Q128_SPI_TIMEOUT) != HAL_OK)
    {
      printf("spi error: WQX_SendBytes\r\n");
      return -1;
    }
  }
  if (HAL_SPI_Transmit(WQX_SPI_INTERFACE, pBuff + i * WQX_SPI_TRANS_MAX, NumOfBytes, W25Q128_SPI_TIMEOUT) != HAL_OK)
  {
    printf("spi error: WQX_SendBytes\r\n");
    return -1;
  }
  return 0;
}

/**
 * @brief  使用SPI接收多个字节的数据
 * @param  byte：要发送的数据
 * @retval 返回接收到的数据
 */
static int8_t WQX_ReceiveBytes(uint8_t *pBuff, uint32_t NumOfBytes)
{
  uint32_t i;
  uint32_t times = NumOfBytes / WQX_SPI_TRANS_MAX;

  NumOfBytes %= WQX_SPI_TRANS_MAX;

  for (i = 0; i < times; i++)
  {
    if (HAL_SPI_Receive(WQX_SPI_INTERFACE, pBuff + i * WQX_SPI_TRANS_MAX, WQX_SPI_TRANS_MAX, W25Q128_SPI_TIMEOUT) != HAL_OK)
    {
      printf("spi error: WQX_ReceiveBytes\r\n");
      return -1;
    }
  }
  if (HAL_SPI_Receive(WQX_SPI_INTERFACE, pBuff + i * WQX_SPI_TRANS_MAX, NumOfBytes, W25Q128_SPI_TIMEOUT) != HAL_OK)
  {
    printf("spi error: WQX_SendBytes\r\n");
    return -1;
  }

  return 0;
}

/**
 * @brief 发送24位或32位（Q256）地址
 */
void WQX_Send_Addr(uint32_t addr)
{
  if (DevID == DevFlashID_Q256)
    WQX_RW_BYTE((uint8_t)(addr >> 24));

  WQX_RW_BYTE((uint8_t)(addr >> 16));
  WQX_RW_BYTE((uint8_t)(addr >> 8));
  WQX_RW_BYTE((uint8_t)addr);
}

/**
 * @brief 读取W25QXX状态寄存器（有三个）
 * @param SR_th 状态寄存器 1~3
 * @retval 寄存器值
 */
uint8_t WQX_Read_SR(uint8_t SR_th)
{

  uint8_t temp;
  switch (SR_th)
  {
  case 1:
    temp = FLASH_ReadStatusReg1; /* 读状态寄存器1指令 */
    break;
  case 2:
    temp = FLASH_ReadStatusReg2; /* 读状态寄存器2指令 */
    break;
  case 3:
    temp = FLASH_ReadStatusReg3; /* 读状态寄存器3指令 */
    break;
  default:
    temp = FLASH_ReadStatusReg1;
    break;
  }
  NOR_FLASH_CS_LOW();
  WQX_RW_BYTE(temp);              /* 发送读寄存器命令 */
  temp = WQX_RW_BYTE(Dummy_Byte); /* 读取一个字节 */
  NOR_FLASH_CS_HIGH();
  return temp;
}

/**
 * @brief 写 W25QXX状态寄存器（有三个）
 * @param SR_th 状态寄存器 1~3
 * @param SR_Val 写入寄存器的值
 */
void WQX_Write_SR(uint8_t SR_th, uint8_t SR_Val)
{

  uint8_t temp;
  switch (SR_th)
  {
  case 1:
    temp = FLASH_WriteStatusReg1; /* 写状态寄存器1指令 */
    break;

  case 2:
    temp = FLASH_WriteStatusReg2; /* 写状态寄存器2指令 */
    break;

  case 3:
    temp = FLASH_WriteStatusReg3; /* 写状态寄存器3指令 */
    break;

  default:
    temp = FLASH_WriteStatusReg1;
    break;
  }
  NOR_FLASH_CS_LOW();
  WQX_RW_BYTE(temp);   /* 发送写寄存器命令 */
  WQX_RW_BYTE(SR_Val); /* 写一个字节 */
  NOR_FLASH_CS_HIGH();
}

/**
 * @brief  W25QXX扇区擦除 需要50ms左右
 * @param  SectorAddr (0~4095)对于16M bytes
 * @retval 无
 */
static void WQX_SectorErase(uint32_t SectorAddr)
{

  SectorAddr *= WQX_SecSize; /** 换算成 24 位地址 */

  WQX_WriteEnable();

  NOR_FLASH_CS_LOW();
  WQX_RW_BYTE(FLASH_SectorErase);
  WQX_Send_Addr(SectorAddr);
  NOR_FLASH_CS_HIGH();

  WQX_WaitBusy();
}

/**
 * @brief  擦除FLASH扇区，整片擦除,大约需要30s左右
 * @note  擦除后数据全是1
 * @param  无
 * @retval 无
 */
void WQX_ChipErase(void)
{

  WQX_WriteEnable();

  NOR_FLASH_CS_LOW();
  WQX_RW_BYTE(FLASH_ChipErase);
  NOR_FLASH_CS_HIGH();

  WQX_WaitBusy();
}

/**
 * @brief 对FLASH按页写入数据，调用本函数写入数据前需要先擦除写入的空间
 * @param pBuffer，要写入数据的指针
 * @param WriteAddr，写入地址(24bit)
 * @param NumByteToWrite，写入数据长度，不能超出W25QXX的页尾，否则会重头重写该页
 */
static void WQX_PageWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{

  WQX_WriteEnable(); // 写使能

  NOR_FLASH_CS_LOW();
  WQX_RW_BYTE(FLASH_PageProgram);
  WQX_Send_Addr(WriteAddr);
  WQX_SendBytes(pBuffer, NumByteToWrite);
  NOR_FLASH_CS_HIGH();

  WQX_WaitBusy();
}

/**
 * @brief  无检查直接写入
 * @param	pBuffer，要写入数据的指针
 * @param WriteAddr，写入地址(24bit)
 * @param  NumByteToWrite
 * @retval 无
 */
static void WQX_Write_NoCheck(uint8_t *pBuff, uint32_t WriteAddr, uint32_t NumofByte)
{
  uint16_t Pageleftbyte;
  Pageleftbyte = 256 - (WriteAddr % 256); // 起始页剩余字节数
  if (NumofByte <= Pageleftbyte)
    Pageleftbyte = NumofByte; // 说明第一页都写不完
  while (1)
  {
    WQX_PageWrite(pBuff, WriteAddr, Pageleftbyte); // 按页的顺序写
    if (NumofByte == Pageleftbyte)
      break; // 写入完成

    // 未写完，此时要更新，写入下一页的字节数 Pageleftbyte
    pBuff += Pageleftbyte;
    WriteAddr += Pageleftbyte;
    NumofByte -= Pageleftbyte;
    if (NumofByte > 256)
      Pageleftbyte = 256; // 剩余字节数 大于一页
    else
      Pageleftbyte = NumofByte; // 剩余字节数 不足或等于一页
  }
}

/**
 * @brief  任意地址带擦除的写入
 * @param	pBuffer，要写入数据的指针
 * @param WriteAddr，写入地址(24bit)
 * @param  NumByteToWrite 要写入的字节数
 * @retval 无
 */
#if (USE_4KRW_MODE == 0)
void WQX_Disk_Write(uint8_t *pBuff, uint32_t WriteAddr, uint32_t Numofbyte)
{
  uint16_t i, SecLeftbyte;
  uint16_t OptSecAddr, SecOffsetByte;
  // 第一次更新扇区特征变量                重复特征变量法
  SecOffsetByte = WriteAddr % WQX_SecSize;   // 操作扇区偏移字节
  OptSecAddr = WriteAddr / WQX_SecSize;      // 操作扇区编号
  SecLeftbyte = WQX_SecSize - SecOffsetByte; // 该扇区剩余字节数

  //  if (WriteAddr >= 12 * 1024 * 1024)
  //    return; // 保护后4M字库

  if (SecLeftbyte >= Numofbyte)
    SecLeftbyte = Numofbyte; // 写入数据不足一个扇区
  while (1)
  {
    WQX_Disk_Read(W25QXXcopyBuff, OptSecAddr * WQX_SecSize, WQX_SecSize); // 读出整个扇区
    for (i = 0; i < SecLeftbyte; i++)
    {
      if (W25QXXcopyBuff[SecOffsetByte + i] != 0xFF)
        break;
    }
    if (i < SecLeftbyte) // 需要擦除
    {
      WQX_SectorErase(OptSecAddr);
      for (i = 0; i < SecLeftbyte; i++) // 复制要写入的数据
      {
        W25QXXcopyBuff[SecOffsetByte + i] = pBuff[i];
      }
      WQX_Write_NoCheck(W25QXXcopyBuff, OptSecAddr * WQX_SecSize, WQX_SecSize);
    }
    else // 剩余地址不需要擦除，可以直接写
      WQX_Write_NoCheck(pBuff, WriteAddr, SecLeftbyte);

    // 一页的扇区操作结束，更新 扇区变量特征
    if (SecLeftbyte == Numofbyte)
      break; // 扇区操作结束
    else
    {
      WriteAddr += SecLeftbyte;
      pBuff += SecLeftbyte;
      Numofbyte -= SecLeftbyte;
      SecOffsetByte = 0; // 计算写入数据 操作扇区偏移字节
      OptSecAddr++;      // 计算写入数据 起始扇区编号

      if (Numofbyte > WQX_SecSize) // 下一个扇区不够
        SecLeftbyte = WQX_SecSize;
      else // 下一个扇区写不满或刚好写满
        SecLeftbyte = Numofbyte;
    }
  }
}
#endif

/**
 * @brief  读取FLASH数据
 * @param 	pBuffer，存储读出数据的指针
 * @param   ReadAddr，读取地址
 * @param   NumByteToRead，读取数据长度
 * @retval 无
 */
void WQX_Disk_Read(uint8_t *pBuff, uint32_t ReadAddr, uint32_t NumByteToRead)
{
  NOR_FLASH_CS_LOW();
  WQX_RW_BYTE(FLASH_ReadData);
  WQX_Send_Addr(ReadAddr);
  WQX_ReceiveBytes(pBuff, NumByteToRead);
  NOR_FLASH_CS_HIGH();
  WQX_WaitBusy();
}

/**
 * @brief  读取FLASH ID
 * @param 	无
 * @retval FLASH ID
 */
uint32_t WQX_ReadID(void)
{
  uint32_t ID;
  uint8_t temp;

  NOR_FLASH_CS_LOW();
  WQX_RW_BYTE(FLASH_JedecDeviceID); //	0X9F JEDEC ID(
  temp = WQX_RW_BYTE(Dummy_Byte);
  ID = (uint32_t)temp;
  temp = WQX_RW_BYTE(Dummy_Byte);
  ID = (ID << 8) | (uint32_t)temp;
  temp = WQX_RW_BYTE(Dummy_Byte);
  ID = (ID << 8) | (uint32_t)temp;
  NOR_FLASH_CS_HIGH();

  return ID;
}

/**
 * @brief  读取FLASH Device ID（并且唤醒FLASH）
 * @param 	无
 * @retval FLASH Device ID
 */
uint32_t WQX_ReadDeviceID(void)
{
  uint32_t Temp;

  NOR_FLASH_CS_LOW();
  WQX_RW_BYTE(FLASH_DeviceID); // 0XAB
  WQX_RW_BYTE(Dummy_Byte);
  WQX_RW_BYTE(Dummy_Byte);
  WQX_RW_BYTE(Dummy_Byte);
  Temp = WQX_RW_BYTE(Dummy_Byte);
  NOR_FLASH_CS_HIGH();

  return Temp;
}

/**
 * @brief  向FLASH发送 写使能 命令
 * @param  none
 * @retval none
 */
static void WQX_WriteEnable(void)
{
  NOR_FLASH_CS_LOW();
  WQX_RW_BYTE(FLASH_WriteEnable);
  NOR_FLASH_CS_HIGH();
}

/**
 * @brief  等待WIP(BUSY)标志被置0，即等待到FLASH内部数据写入完毕
 * @param  none
 * @retval none
 */
void WQX_WaitBusy(void)
{
  uint8_t FLASH_Status = 0;
  do
  {
    FLASH_Status = WQX_Read_SR(1);
  } while ((FLASH_Status & 0x01) == 0x01); /* 正在写入标志 */
}

// 进入掉电模式
void WQX_PowerDown(void)
{
  NOR_FLASH_CS_LOW();
  WQX_RW_BYTE(FLASH_PowerDown);
  NOR_FLASH_CS_HIGH();
}

// 唤醒
void WQX_WAKEUP(void)
{
  NOR_FLASH_CS_LOW();
  WQX_RW_BYTE(FLASH_ReleasePowerDown);
  NOR_FLASH_CS_HIGH(); // 等待TRES1
}

/**
 * @brief 查询 W25QXX状态
 * @retval 0:就绪 1：忙状态
 */
int8_t Get_WQX_State(void)
{
  uint8_t FLASH_Status = 0;
  FLASH_Status = WQX_Read_SR(1);

  if ((FLASH_Status & 0x01) == 0x01)
  {
    return 1;
  }
  else
    return 0;
}

/**
 * @brief 4KB(一个扇区）读
 * @note 对于16MB容量，Sector_addr :0~4096;
 *
 * @param pBuff: 读数据地址
 * @param Sector_addr 扇区地址
 * @param Sector_Nbr 扇区个数
 */
void WQX_4K_Sector_Read(uint8_t *pBuff, uint16_t Sector_addr, uint16_t Sector_Nbr)
{
  WQX_Disk_Read(pBuff, Sector_addr * WQX_SecSize, Sector_Nbr * WQX_SecSize);
}

#if (USE_4KRW_MODE)
/**
 * @brief 4KB(一个扇区）写
 * @note 对于16MB容量，Sector_addr :0~4096;
 *
 * @param pBuff: 写数据地址
 * @param Sector_addr 扇区地址
 * @param Sector_Nbr 扇区个数
 */
void WQX_4K_Sector_Write(uint8_t *pBuff, uint16_t Sector_addr, uint16_t Sector_Nbr)
{
  uint32_t i, j;
  uint32_t pages = (uint32_t)Sector_Nbr * WQX_SecSize / WQX_PageSize;

  for (i = 0; i < Sector_Nbr; i++)
  {
    WQX_Disk_Read(W25QXXcopyBuff, (Sector_addr + i) * WQX_SecSize, WQX_SecSize);
    for (j = 0; j < WQX_SecSize; j++)
    {
      if (W25QXXcopyBuff[j] != 0xff)
      {
        WQX_SectorErase(Sector_addr + i);
        break;
      }
    }
  }
  for (i = 0; i < pages; i++)
  {
    WQX_PageWrite((pBuff + i * WQX_PageSize), Sector_addr * WQX_SecSize + i * WQX_PageSize,
                  WQX_PageSize);
  }
}
#endif

#if (WQX_TEST)

#define TEST_BUFF_SIZE ((uint32_t)4096 * 24)
#define WQX_TEST_ADDR ((uint32_t)4096 * 4000)
#define WQX_TEST_DATA (uint8_t)0x88
uint8_t wqx_buf[TEST_BUFF_SIZE] = {0};

void WQX_test(void)
{
  uint32_t te;
  uint32_t ts;
  uint32_t i;

  for (i = 0; i < TEST_BUFF_SIZE; i++)
  {
    wqx_buf[i] = WQX_TEST_DATA;
  }

  /** 写 */
  ts = HAL_GetTick();
  //WQX_4K_Sector_Write(wqx_buf, WQX_TEST_ADDR / 4096, TEST_BUFF_SIZE / 4096);
  WQX_Disk_Write(wqx_buf, WQX_TEST_ADDR, TEST_BUFF_SIZE);
  te = HAL_GetTick();
  printf("WQX W >>:%ld ms\r\n", te - ts);

  for (i = 0; i < TEST_BUFF_SIZE; i++)
    wqx_buf[i] = 0x00;

  ts = HAL_GetTick();
  //WQX_4K_Sector_Read(wqx_buf, WQX_TEST_ADDR / 4096, TEST_BUFF_SIZE / 4096);
  WQX_Disk_Read(wqx_buf, WQX_TEST_ADDR, TEST_BUFF_SIZE);
  te = HAL_GetTick();
  printf("WQX R >>:%ld ms\r\n", te - ts);

  for (i = 0; i < TEST_BUFF_SIZE; i++)
  {
    if (wqx_buf[i] != WQX_TEST_DATA)
    {
      printf("wqx rw error\r\n");
      break;
    }
  }
  if (i == TEST_BUFF_SIZE)
    printf("wqx test ok\r\n");
}

#endif
