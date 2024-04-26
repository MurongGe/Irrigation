//
// Created by ShunX on 2024/4/13.
//

#include "sensor_adc.h"
#include "stdio.h"
#include "myiic.h"


#define PCF8591_ADDR 0x90

//IN0 测试地 IN1 光敏电阻 IN2 放大器输出 IN3 滑动变阻器
uint8_t ADConver_Res[4];

#define PCF_COMMAND 00    //0000 0000顺序4通道AD转换

void I2C_Write_Bytes(uint8_t Slave_Addr, uint8_t Reg_WR_Addr, uint8_t *buff, uint8_t len) {
  iic_start();
  iic_send_byte(Slave_Addr);  //地址 & 写（0x00）
  iic_wait_ack();
  iic_send_byte(Reg_WR_Addr);  //写 寄存器地址
  iic_wait_ack();
  while (len--) {

    iic_send_byte(*buff);
    buff++;
    iic_wait_ack();
  }
  iic_stop();
}


uint8_t I2C_Read_Bytes(uint8_t Slave_Addr, uint8_t Reg_WR_Addr) {
  uint8_t temp;
  iic_start();
  iic_send_byte(Slave_Addr);  //先写 设备地址
  iic_wait_ack();
  iic_send_byte(Reg_WR_Addr);  //再写 寄存器地址
  iic_wait_ack();
  //重新开始
  iic_start();
  iic_send_byte(Slave_Addr | 0x01);  //开始读 设备地址|0x01
  iic_wait_ack();
  temp = iic_read_byte(0);
  iic_stop();
  return temp;
}

void PCF_Get_ADC(void) {
  ADConver_Res[0] = I2C_Read_Bytes(PCF8591_ADDR, PCF_COMMAND);
  ADConver_Res[1] = I2C_Read_Bytes(PCF8591_ADDR, PCF_COMMAND + 1);
  ADConver_Res[2] = I2C_Read_Bytes(PCF8591_ADDR, PCF_COMMAND + 2);
  ADConver_Res[3] = I2C_Read_Bytes(PCF8591_ADDR, PCF_COMMAND + 3);
}